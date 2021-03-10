// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Importa\Monta\VTMonta.h>
#include <PlataformaSinap\Fontes\Log\VTLog.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTDominio.h>
#include <PlataformaSinap\Fontes\Rede\VTGerador.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoRede.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Rede.h>
#include "TPreMonta1.h"
#include "..\Eqpto\TCircuito.h"
#include "..\Eqpto\TEt.h"
#include "..\Eqpto\TUtc.h"

// ---------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta1(VTApl *apl)
{
	return (new TPreMonta1(apl));
}

// ---------------------------------------------------------------------------
__fastcall TPreMonta1::TPreMonta1(VTApl *apl) : TPreMonta(apl)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall TPreMonta1::~TPreMonta1(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSec_Barra(sdi_BARRA_BT &sdi)
{
	// nada a fazer
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSec_Capacitor(sdi_CAPACITOR_BT &sdi)
{
	// variáveis locais
	strCAPACITOR str;

	try
	{ // cofigura Log
		if (plog)
			plog->DefineValor("Eqpto", "Capacitor");
		if (plog)
			plog->DefineValor("Código", sdi.codigo);
		// verifica se o Capacitor série pertence à Rede secundária selecionada
		if ((et == NULL) || (et->id != sdi.redebt_id))
		{
			if (plog)
				plog->Add("Não está associado a rede secundária");
			return (true);
		}
		// preenche estrutura strCAPACITOR
		str.cap_id = sdi.id;
		str.codigo = sdi.codigo;
		// insere Capacitor na Barra MT
		str.bar_id = et->barra_mt->Id;
		str.ligacao = lgTRIANG;
		str.vnom = et->sec_vnom_kv;
		str.q_mvar = sdi.qnon_kvar * 0.001;
		str.esq.posbar = 50.;
		str.esq.dxbar = 0;
		str.esq.dybar = -DYBAR;
		// insere Capacitor série em VTMonta
		if (monta->InsereCapacitor(str) == NULL)
			return (false);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSec_Consumidor(sdi_CONS_BT &sdi)
{
	// variáveis locais
	VTCarga *carga;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Carga");
		// verifica se o Consumidor  pertence à Rede secundária selecionada
		if ((et == NULL) || (et->id != sdi.redebt_id))
		{
			if (plog)
				plog->Add("Não está associado a rede secundária");
			return (true);
		}
		// cria Carga temporária do consumidor
		if ((carga = CriaCargaConsumidorBT(sdi, et->barra_mt->Id)) == NULL)
		{
			return (true);
		}
		// atualiza demanda da Carga equivalente da ET
		AtualizaCargaEquivalente(et, carga);
		// retira Carga da sua Barra e destrói objeto
		carga->pbarra->RemoveEqbar(carga);
		delete carga;
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSec_ConsIP(sdi_CONS_IP &sdi)
{
	// variáveis locais
	VTBarra *barra;
	VTCarga *carga = NULL;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Carga IP");
		if (plog)
			plog->DefineValor("Código", IntToStr(sdi.id));
		// verifica se o Consumidor  pertence à Rede secundária selecionada
		if (et == NULL)
		{
			if (plog)
				plog->Add("Não está associado a rede secundária");
			return (true);
		}
		// cria Carga temporária de IP
		if ((carga = CriaCargaConsumidorIP(sdi, et->barra_mt->Id)) == NULL)
			return (false);
		// atualiza demanda da Carga equivalente da ET
		AtualizaCargaEquivalente(et, carga);
		// retira Carga da sua Barra e destrói objeto
		carga->pbarra->RemoveEqbar(carga);
		DLL_DeleteEqpto(carga);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSec_Gerador(sdi_GERADOR_BT &sdi)
{
	// variáveis locais
	double s_mva, p_mw, q_mvar;
	strGERADOR str;
	VTCarga *carga;

	try
	{ // configura Resumo
		if (plog)
			plog->DefineValor("Eqpto", "Gerador");
		if (plog)
			plog->DefineValor("Código", sdi.codigo);
		// proteção
		if ((et == NULL) || (et->id != sdi.circ_id))
		{
			if (plog)
				plog->Add("Não está associado a rede secundária");
			return (true);
		}
		// inicia estrutura strGerador
		str.obra = false;
		str.ger_id = sdi.id;
		// insere Gerador na Barra MT
		str.bar_id = et->barra_mt->Id;
		str.codigo = sdi.codigo;
		str.extern_id = "";
		str.tiposup = supPV;
		str.tipo_geracao = gerPCH;
		str.ligacao = lgEST_AT;
		str.vnom = et->sec_vnom_kv;
		str.snom_mva = sdi.snom_kva * 0.001;
		str.smax_mva = str.snom_mva * 1.2;
		str.fplim = 0.9;
		str.z0.r = 0.;
		str.z0.x = 0.05;
		str.z1.r = 0.;
		str.z1.x = 0.05;
		// define curva de geração própria
		str.curva.id = -1;
		str.curva.tipica = false;
		str.curva.num_pat = pat01HORA;
		str.curva.num_valor = nvCURVA_PQVT;
		// define pot.ativa e reativa a partir de Snom e considerando fatpot = 0.92
		s_mva = str.snom_mva;
		p_mw = s_mva * 0.92;
		q_mvar = sqrt((s_mva * s_mva) - (p_mw * p_mw));
		for (int np = 0; np < pat01HORA; np++)
		{ // define hora inicial e final do patamar
			str.curva.patamar[np].hm_ini = pat_edp[np].hm_ini;
			str.curva.patamar[np].hm_fim = pat_edp[np].hm_fim;
			// define valores de ponta/fora de ponta
			if (pat_edp[np].ponta)
			{ // demanda de ponta
				str.curva.patamar[np].valor[indP] = p_mw;
				str.curva.patamar[np].valor[indQ] = q_mvar;
				str.curva.patamar[np].valor[indV] = 1.;
				str.curva.patamar[np].valor[indT] = 0.;
			}
			else
			{ // demanda fora de ponta
				str.curva.patamar[np].valor[indP] = p_mw;
				str.curva.patamar[np].valor[indQ] = q_mvar;
				str.curva.patamar[np].valor[indV] = 1.;
				str.curva.patamar[np].valor[indT] = 0.;
			}
		}
		// define coordenadas p/ esquemático
		str.esq.dxbar = -2000;
		str.esq.dybar = 0;
		// insere Gerador em VTMonta
		if (!monta->InsereGerador(str))
			return (false);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSec_Link(sdi_LINK_BT &sdi)
{
	// nada a fazer
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSec_PtoCarga(sdi_PCARGA_BT &sdi)
{
	// nada a fazer
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSec_Rede(sdi_INSTAL_TRAFO &sdi)
{
	// variáveis locais
	strREDE str_rede;
	VTBarra *barra_mt, *barra_bt;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Rede", sdi.codigo);
		if (plog)
			plog->DefineValor("Eqpto", "Rede BT");
		if (plog)
			plog->DefineValor("Código", sdi.codigo);
		// reinicia ponteiro p/ ET corrente
		et = NULL;
		// reinicia controle de Barra do Monta
		monta->InsereBarraStop();
		monta->InsereBarraStart();
		// verifica se existe Circuito selecionado
		if ((circuito == NULL) || (circuito->rede->Id != sdi.circ_id))
		{
			if (plog)
				plog->Add("Não está associado a rede primária");
			return (false);
		}
		// verifica se é uma EP
		// não há este tipo de informação
		// proteção: verifica se já existe uma ET c/ mesmo ID
		if (circuito->ExisteEt(sdi.id_rede_bt))
		{
			if (plog)
				plog->Add("ID repetido");
			return (false);
		}
		// determina Barra do primário
		if ((barra_mt = circuito->ExisteBarra(sdi.barra_mt_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe barra de conexão c/ rede primária");
			return (true);
		}
		// redefine código da Barra com o código da instalação
		barra_mt->Codigo = sdi.codigo;
		// cria objeto Et e insere na lista de ET do circuito
		if ((et = CriaEt(sdi, barra_mt, NULL)) == NULL)
			return (false);
		// NÃO cria Barra BT p/ ligar os Trafos da ET
		// et->barra_bt = InsereSec_BarraTrafo(et, sdi.barra_bt_id);
		// insere Et no Circuito
		circuito->InsereEt(et);
		// cria Carga p/ montar carga equivalente de todos os consumidores da ET
		InsereSec_CargaEquivalente(et);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSec_Trecho(sdi_TRECHO_BT &sdi)
{
	// nada a fazer
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSec_Utc(sdi_UTC_BT &sdi)
{
	// nada a fazer
	return (true);
}


// -----------------------------------------------------------------------------
// eof

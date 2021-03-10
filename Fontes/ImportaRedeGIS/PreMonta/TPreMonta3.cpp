// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Arranjo\VTArranjo.h>
#include <PlataformaSinap\Fontes\Importa\Monta\VTMonta.h>
#include <PlataformaSinap\Fontes\Log\VTLog.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTDominio.h>
#include <PlataformaSinap\Fontes\Rede\VTGerador.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoRede.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TPreMonta3.h"
#include "..\Empresa\VTCondutor.h"
#include "..\Empresa\VTDadosEmpresa.h"
#include "..\Eqpto\TCircuito.h"
#include "..\Eqpto\TEt.h"
#include "..\Eqpto\TPtoCargaBT.h"
#include "..\Eqpto\TUtc.h"

// ---------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta3(VTApl *apl)
{
	return (new TPreMonta3(apl));
}

// ---------------------------------------------------------------------------
__fastcall TPreMonta3::TPreMonta3(VTApl *apl) : TPreMonta(apl)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall TPreMonta3::~TPreMonta3(void)
{
	// nada a fazer
}
/* função escrita para o caso de terem barras com ID repetido
// ---------------------------------------------------------------------------
bool __fastcall TPreMonta3::InsereSec_Barra(sdi_BARRA_BT &sdi)
{
	// variáveis locais
	strBARRA str;
	VTBarra *barra = NULL;

	try
	{ // configura log
		if (plog)
			plog->DefineValor("Eqpto", "Barra");
		if (plog)
			plog->DefineValor("Código", sdi.codigo);
		// proteção
		if (et == NULL)
		{
			if (plog)
				plog->Add("Não está associada a rede secundária");
			return (false);
		}
		// incrementa número de Barras da ET e verifica se é a primeira
		et->num_barra++;
		if (et->num_barra == 1)
		{ // cria Trafos da ET e insere em VTMonta
			InsereSec_Trafo(et);
		}
		// verifica se a barra é válida (função chamada somente para inserir trafos)
		if (sdi.id < 0)
			return (true);
		// proteção: verifica se é a Barra inicial da ET, já criada na inserção da rede
		if (sdi.id == et->barra_bt->Id)
		{ // redefine coordenadas da Barra inicial da ET
			et->barra_bt->utm.x = sdi.x_utm_m * 100;
			et->barra_bt->utm.y = sdi.y_utm_m * 100;
			et->barra_bt->esq.x1 = et->barra_bt->esq.x2 = et->barra_bt->utm.x;
			et->barra_bt->esq.y1 = et->barra_bt->esq.y2 = et->barra_bt->utm.y;
			return (true);
		}
		// cria uma nova Barra
		str.bar_id = FalsoId(); // sdi.id;    //tem id repetido entre BTs
		str.codigo = sdi.codigo;
		str.extern_id = IntToStr(sdi.id); // "";
		str.vnom = et->sec_vnom_kv;
		str.utm.x = sdi.x_utm_m * 100;
		str.utm.y = sdi.y_utm_m * 100;
		// esquemático
		str.esq.x1 = str.utm.x;
		str.esq.y1 = str.utm.y;
		str.esq.x2 = str.utm.x;
		str.esq.y2 = str.utm.y;
		// insere Barra em VTMonta
		if ((barra = (VTBarra*)monta->InsereBarraEqpto(str)) != NULL)
		{ // salva Barra na Et
			et->InsereBarra(barra);
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}
*/
//função original
// ---------------------------------------------------------------------------
bool __fastcall TPreMonta3::InsereSec_Barra(sdi_BARRA_BT &sdi)
{
	// variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	strBARRA str;
	VTBarra *barra, *clone;

	try
	{ // configura log
		if (plog)
			plog->DefineValor("Eqpto", "Barra");
		if (plog)
			plog->DefineValor("Código", sdi.codigo);
		// proteção
		if (et == NULL)
		{
			if (plog)
				plog->Add("Não está associada a rede secundária");
			return (false);
		}
		if((barra = redes->ExisteBarra(sdi.id)) != NULL)
		{
		   clone = barra->Clone();
		   clone->Id = FalsoId();
		   et->InsereBarra(clone);
		   redes->InsereBarra(clone);
		   return(true);
		}
		// incrementa número de Barras da ET e verifica se é a primeira
		et->num_barra++;
		if (et->num_barra == 1)
		{ // cria Trafos da ET e insere em VTMonta
			InsereSec_Trafo(et);
		}
		// verifica se a barra é válida (função chamada somente para inserir trafos)
		if (sdi.id < 0)
			return (true);
		// proteção: verifica se é a Barra inicial da ET, já criada na inserção da rede
		if (sdi.id == et->barra_bt->Id)
		{ // redefine coordenadas da Barra inicial da ET
			et->barra_bt->utm.x = sdi.x_utm_m * 100;
			et->barra_bt->utm.y = sdi.y_utm_m * 100;
			et->barra_bt->esq.x1 = et->barra_bt->esq.x2 = et->barra_bt->utm.x;
			et->barra_bt->esq.y1 = et->barra_bt->esq.y2 = et->barra_bt->utm.y;
			return (true);
		}
		// cria uma nova Barra
		str.bar_id = sdi.id;
		str.codigo = sdi.codigo;
		str.extern_id = IntToStr(sdi.id); // "";
		str.vnom = et->sec_vnom_kv;
		str.utm.x = sdi.x_utm_m * 100;
		str.utm.y = sdi.y_utm_m * 100;
		// esquemático
		str.esq.x1 = str.utm.x;
		str.esq.y1 = str.utm.y;
		str.esq.x2 = str.utm.x;
		str.esq.y2 = str.utm.y;
		// insere Barra em VTMonta
		if ((barra = (VTBarra*)monta->InsereBarraEqpto(str)) != NULL)
		{ // salva Barra na Et
			et->InsereBarra(barra);
		}
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
bool __fastcall TPreMonta3::InsereSec_Capacitor(sdi_CAPACITOR_BT &sdi)
{
	// variáveis locais
	strCAPACITOR str;
	VTBarra *barra;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Capacitor");
		if (plog)
			plog->DefineValor("Código", sdi.codigo);
		// verifica se o Capacitor série pertence à Rede secundária selecionada
		if ((et == NULL) || (et->rede->Id != sdi.redebt_id))
		{
			if (plog)
				plog->Add("Não está associado a rede secundária");
			return (true);
		}
		// determina barra
		barra = et->ExisteBarra(sdi.barra_id);
		if (barra == NULL)
		{
			if (plog)
				plog->Add("Não existe barra do capacitor BT");
			return (true);
		}
		// preenche estrutura strCAPACITOR
		str.cap_id = sdi.id;
		str.codigo = sdi.codigo;
		str.bar_id = barra->Id; // sdi.barra_id;
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
bool __fastcall TPreMonta3::InsereSec_Consumidor(sdi_CONS_BT &sdi)
{
	// variáveis locais
	TPtoCargaBT *ptc;
	VTCarga *carga;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Carga");
		// verifica se o Consumidor  pertence à Rede secundária selecionada
		if ((et == NULL) || (et->rede->Id != sdi.redebt_id))
		{
			if (plog)
				plog->Add("Não está associado a rede secundária");
			return (true);
		}
		// determina PtoServicoBT do consumidor
		if ((ptc = et->ExistePtoCargaBT(sdi.pcarga_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe pto.carga");
			return (true);
		}
		// Cria carga temporaria do consumidor
		if ((carga = CriaCargaConsumidorBT(sdi, ptc->barra->Id)) != NULL)
		{
			et->InsereCarga(carga);
		}
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
bool __fastcall TPreMonta3::InsereSec_ConsIP(sdi_CONS_IP &sdi)
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
		// determina barra
		barra = et->ExisteBarra(sdi.barra_id);
		if (barra == NULL)
		{
			if (plog)
				plog->Add("Não existe barra de iluminação pública");
			return (true);
		}
		// cria Carga de IP
		CriaCargaConsumidorIP(sdi, barra->Id); // sdi.barra_id);
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
bool __fastcall TPreMonta3::InsereSec_Gerador(sdi_GERADOR_BT &sdi)
{
	// variáveis locais
	double s_mva, p_mw, q_mvar;
	strGERADOR str;
	VTCarga *carga;
	VTBarra *barra;

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
		// determina barra
		barra = et->ExisteBarra(sdi.barra_id);
		if (barra == NULL)
		{
			if (plog)
				plog->Add("Não existe barra do gerador BT");
			return (true);
		}
		// inicia estrutura strGerador
		str.obra = false;
		str.ger_id = sdi.id;
		str.bar_id = barra->Id; // sdi.barra_id;
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
bool __fastcall TPreMonta3::InsereSec_Link(sdi_LINK_BT &sdi)
{
	// variáveis locais
	AnsiString codigo;
	strTRECHO str_tre;
	VTArranjo *arranjo;
	VTBarra *barra1, *barra2;
	VTTrecho *trecho;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Link BT");
		if (plog)
			plog->DefineValor("Código", "trecho_id=" + IntToStr(sdi.trecho_id));
		// verifica se há uma Et para o Link
		if (et == NULL)
		{
			if (plog)
				plog->Add("Não está associado a rede secundária");
			return (true);
		}
		// determina Trecho associado
		if ((trecho = et->ExisteTrecho(sdi.trecho_id)) == NULL)
		{
			if (plog)
				plog->Add("Não está associado a um trecho");
			return (true);
		}
		// verifica se Barras iguais
		if (sdi.barra_id1 == sdi.barra_id2)
		{ // não cria trecho com barras iguais
			if (plog)
				plog->Add("Barra inicial igual à barra final");
			return (true);
		}
		// verifica se existem as Barras
		barra1 = et->ExisteBarra(sdi.barra_id1);
		barra2 = et->ExisteBarra(sdi.barra_id2);
		if ((barra1 == NULL) || (barra2 == NULL))
		{
			if (plog)
				plog->Add("Não existem as barras");
			return (true);
		}
		// preenche estrutura strTRECHO
		str_tre.tre_id = FalsoId();
		str_tre.rede_id = et->rede->Id;
		str_tre.extern_id = "";
		str_tre.codigo = trecho->Codigo;
		str_tre.bar1_id = barra1->Id;
		str_tre.bar2_id = barra2->Id;
		str_tre.fases = trecho->arranjo->Fases;
		str_tre.comp_km = Distancia_cm(barra1->utm.x, barra1->utm.y, barra2->utm.x,
			barra2->utm.y) * 1e-5;
		str_tre.esq.posbar1 = 50.;
		str_tre.esq.posbar2 = 50.;
		// insere Trecho com o Arranjo definido
		if (monta->InsereTrecho(str_tre, barra1, barra2, trecho->arranjo) == NULL)
			return (true);
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
bool __fastcall TPreMonta3::InsereSec_PtoCarga(sdi_PCARGA_BT &sdi)
{
	// variáveis locais
	TPtoCargaBT *ptc;
	VTBarra *barra;

	try
	{ // configura Resumo
		if (plog)
			plog->DefineValor("Eqpto", "Pto.Carga BT");
		if (plog)
			plog->DefineValor("Código", sdi.codigo);
		// proteção
		if ((et == NULL) || (et->id != sdi.redebt_id))
		{
			if (plog)
				plog->Add("Não está associado a rede secundária");
			return (true);
		}
		// determina Barra do PtoServico
		if ((barra = et->ExisteBarra(sdi.barra_id)) == NULL)
		{
			if (plog)
				plog->Add("Barra inexistente");
			return (true);
		}
		// cria um objeto PtoCargaBT
		if ((ptc = NewObjPtoCargaBT()) == NULL)
		{
			if (plog)
				plog->Add(ERRO_DE_SISTEMA);
			return (false);
		}
		ptc->id = sdi.id;
		ptc->codigo = sdi.codigo;
		ptc->barra = barra;
		// insere PtoCargaBT na ET
		et->InserePtoCargaBT(ptc);
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
bool __fastcall TPreMonta3::InsereSec_Rede(sdi_INSTAL_TRAFO &sdi)
{
	// variáveis locais
	strREDE str_rede;
	VTBarra *barra_mt;

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
		if (circuito->ExisteEt(sdi.id))
		{
			if (plog)
				plog->Add("ID repetido: " + IntToStr(sdi.id));
			return (false);
		}
		// verifica se a tensão de baixa é válida
		if (IsDoubleZero(sdi.vbaixa_v))
		{
			if (plog)
				plog->Add("Tensão nominal do secundário nula");
			// define tensão em função do tipo da ET
			sdi.vbaixa_v = 220;
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
		// cria Barra BT p/ ligar os Trafos da ET
		et->barra_bt = InsereSec_BarraTrafo(et, sdi.barra_bt_id);
		// insere Et no Circuito
		circuito->InsereEt(et);
		// insere Barra BT na Et
		// et->InsereBarra(barra_bt);
		// insere Rede secundária em VTMonta
		str_rede.dominio_id = dominio->Id;
		str_rede.rede_id = et->id;
		str_rede.mrede_id = -1;
		str_rede.cluster_id = -1;
		str_rede.barini_id = et->barra_mt->Id;
		str_rede.color = clBlack;
		str_rede.codigo = sdi.codigo;
		str_rede.tiporede.id = ID_NULO;
		str_rede.tiporede.segmento = redeSEC;
		str_rede.tiporede.codigo = "";
		// str_rede.estatica          = false;
		str_rede.estatica = true;
		str_rede.radial = true;
		if ((et->rede = (VTRede*)monta->InsereRede(str_rede)) == NULL)
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
//https://sinapsisenergia.teamwork.com/#/tasks/18236926
bool __fastcall TPreMonta3::InsereSec_Trecho(sdi_TRECHO_BT &sdi)
{
	// variáveis locais
	bool ramal = false;
    bool arranjo_invalido = false;
	int fases_tag;
	strTRECHO str_tre;
	VTArranjo *arranjo;
	VTBarra *barra1, *barra2;
	VTTrecho *trecho;
	VTSuporte *suporte;
	VTCondutor *condutor;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Trecho");
		if (plog)
			plog->DefineValor("Código", sdi.codigo);
		// verifica se o Trecho pertence à Et selecionado
		if ((et == NULL) || (et->id != sdi.redebt_id))
		{
			if (plog)
				plog->Add("Trecho não está associado a rede secundária");
			return (true);
		}
		// verifica se Barras iguais
		if (sdi.barra1_id == sdi.barra2_id)
		{ // não cria trecho com barras iguais
			if (plog)
				plog->Add("Barra inicial igual à barra final");
			return (true);
		}
		// verifica se existem as Barras
		// barra1 = monta->ExisteBarra(sdi.barra1_id);
		// barra2 = monta->ExisteBarra(sdi.barra2_id);
		barra1 = et->ExisteBarra(sdi.barra1_id);
		barra2 = et->ExisteBarra(sdi.barra2_id);
		if ((barra1 == NULL) || (barra2 == NULL))
		{
			if (plog)
				plog->Add("Não existem as barras");
			return (true);
		}
		// determina Arranjo do Trecho
		// fases_tag = fases->ExisteFase(sdi.fases);
		// SEMPRE assume p/ o Trecho as mesmas fases da ET (sem terra). excluindo neutro p/ IP
		fases_tag = et->FasesSecundario() & faseABCN;


		// verifica se cado_id está definido
		if (sdi.cabo_fase_id <= 0)
		{
			if (plog)
				plog->Add("Cabo não definido");
			// assume Cabo default de acordo com o tipo do trecho
			switch (sdi.tipo)
			{
			case 40: // rede secundária
			case 50: // serviço - assume secundária
				condutor = empresa->CondutorDefaultBT;
				break;
			case 20: // rede IP privada
			case 30: // rede IP
			case 1060: // rede IP
			case 1070: // piloto para rede IP
				condutor = empresa->CondutorDefaultIP;
				break;
			case 1040: // ramal de ligacao
				ramal = true;
				condutor = empresa->CondutorDefaultRL;
				break;
			default: // serviço - assume secundária
				condutor = empresa->CondutorDefaultBT;
				break;
			}
			// verifica se foi selecionado um Condutor default
			if (condutor != NULL)
			{
				sdi.cabo_fase_id = condutor->id;
			}
            arranjo_invalido = true;
		}
		// verifica se existe Arranjo (criando um novo Arrano se for necessário)
		if ((arranjo = ExisteArranjo(sdi.cabo_fase_id, fases_tag, ramal)) == NULL)
		{
			if (plog)
				plog->Add("Não existe arranjo");
			//assume default
			arranjo = arranjoBT;
			// verifica se precisa trocar as fases do arranjoBT
			if (arranjo->Fases != fases_tag)
			{
				arranjo = ArranjoBTcerto(fases_tag);
			}
            arranjo_invalido = true;
//			return (true);
		}
		// determina tag p/ fases do trecho e inclui o neutro
		fases_tag = fases->ExisteFase(sdi.fases) | faseN;
		// preenche estrutura strTRECHO
		str_tre.tre_id = sdi.id;
		str_tre.rede_id = et->rede->Id;
		str_tre.extern_id = "";
		str_tre.codigo = sdi.codigo;
		str_tre.bar1_id = barra1->Id; // sdi.barra1_id;
		str_tre.bar2_id = barra2->Id; // sdi.barra2_id;
		str_tre.fases = fases_tag;
		str_tre.comp_km = sdi.comp_m * 0.001;
		str_tre.esq.posbar1 = 50.;
		str_tre.esq.posbar2 = 50.;
		// insere Trecho com o Arranjo definido
		if ((trecho = (VTTrecho*)monta->InsereTrecho(str_tre, barra1, barra2, arranjo)) != NULL)
		{ // remove Trecho da sua Rede e insere em TEt
			et->rede->RemoveLigacao(trecho);
			et->InsereTrecho(trecho);
            if(arranjo_invalido)
            {
                lisTRECHO_ALT->Add(trecho);
            }
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}


// -----------------------------------------------------------------------------
// eof

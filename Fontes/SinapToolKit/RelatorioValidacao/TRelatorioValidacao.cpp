// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRelatorioValidacao.h"
#include "TSED.h"
#include "TAlimentador.h"
#include <DateUtils.hpp>
//#include <PlataformaSinap\Fontes
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Arranjo\VTArranjos.h>
#include <PlataformaSinap\Fontes\Arranjo\VTArranjo.h>
#include <PlataformaSinap\Fontes\Potencia\VTDemanda.h>
#include <PlataformaSinap\Fontes\Classes\VTClasse.h>
#include <PlataformaSinap\Fontes\Classes\VTClasses.h>
#include <PlataformaSinap\Fontes\Curvas\VTCurva.h>
#include <PlataformaSinap\Fontes\Flow\VTFlow.h>
#include <PlataformaSinap\Fontes\Edita\VTEdita.h>
#include <PlataformaSinap\Fontes\Rede\VTFases.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTCapacitor.h>
#include <PlataformaSinap\Fontes\Rede\VTCluster.h>
#include <PlataformaSinap\Fontes\Rede\VTRegulador.h>
#include <PlataformaSinap\Fontes\Rede\VTGerador.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoRede.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\Fontes\Radial\VTArvore.h>
#include <PlataformaSinap\Fontes\Radial\VTNode.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Radial.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// -----------------------------------------------------------------------------
//VTRelatorioValidacao* NewObjRelatorioValidacao(VTApl *apl)
//{
//	return (new TRelatorioValidacao(apl));
//}

// -----------------------------------------------------------------------------
__fastcall TRelatorioValidacao::TRelatorioValidacao(VTApl *apl)
{
	// inicia dados
	this->apl = apl;
	lisSED_ = new TList;
	subestacaoResto = new TSED(apl,NULL);
	lisSED_->Add(subestacaoResto);

}

// -----------------------------------------------------------------------------
__fastcall TRelatorioValidacao::~TRelatorioValidacao(void)
{
	// nada a fazer
	if (lisSED_)
	{
		LimpaTList(lisSED_);
		delete lisSED_;
		lisSED_ = NULL;
	}
	if (arvore_)
	{
		delete arvore_;
		arvore_ = NULL;
	}
//	if (subestacaoResto)
//	{
//		delete subestacaoResto;
//        subestacaoResto = NULL;
//	}
}
// -----------------------------------------------------------------------------
bool __fastcall TRelatorioValidacao::Executa(AnsiString diretorio)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	TSED *sed = NULL;
	AnsiString filepath;
	bool sucesso = true;
	TStrings *arquivoRedes;
	AnsiString agora;
	TList *lisRede;

	try
	{
		//verifica se o diretorio existe
		if (!DirectoryExists(diretorio))
		{
			if (!ForceDirectories(diretorio))
			{
				throw Exception("Diretorio invalido para relatorio de validacao.");
			}
		}
		//1o monta a arvore de redes  e estrutura sed-pri
		MontaArvore();
		//percorre todas seds gerando os relatorios
		for (int i = 0; i < lisSED_->Count; i++)
		{
			sed = (TSED*) lisSED_->Items[i];

			//cria rel
			CriaRelatorio(diretorio,sed);
		}
		//monta arquivo com as quantidades por rede
		arquivoRedes = new TStringList;
		//cria header
		arquivoRedes->Add("rede;tipo_rede;agrupada;radial;estatica;barra;trecho;trafo;chave;regulador;carga;capacitor;gerador;suprimento;");
		agora = Date().FormatString("yyyymmdd") + Time().FormatString("hhmmsszzz");
		filepath =  diretorio + "\\" + agora + "_redes.csv";
		//percorre todas redes
		lisRede = new TList;
		redes->LisRedeCarregada(lisRede);
		sucesso = CriaRelatorioEqpto(arquivoRedes, lisRede, "");
		arquivoRedes->SaveToFile(filepath);
		delete lisRede;
		delete arquivoRedes;
	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}
// -----------------------------------------------------------------------------
TSED* __fastcall TRelatorioValidacao::ExisteSub(VTRede *redeSub)
{
	TSED *sub = NULL;
	std::map<VTRede*, TSED*>::iterator it;

	it = mapSub.find(redeSub);
	if (it != mapSub.end())
	{
		sub = it->second;
	}

    return sub;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15641481
bool __fastcall TRelatorioValidacao::ExisteGD(void)
{
	// variáveis locais
	bool existe = false;
	TList *lisGER = new TList();
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se há geradores
	lisGER->Clear();
	redes->LisEqpto(lisGER, eqptoGERADOR);
	if (lisGER->Count > 0)
		existe = true;
	// destroi lista
	delete lisGER;
	return (existe);
}
// -----------------------------------------------------------------------------
void __fastcall TRelatorioValidacao::AgregaEqpto(TAlimentador *alim, TList *lisEXT, int tipo_eqpto)
{
	VTRede *rede = NULL;

	//verifica se o alimentdor eh nulo
	if (alim == NULL)
		{return;}
	//verifica se o lista eh nulo
	if (lisEXT == NULL)
		{return;}
	//verifica se a rede esta nula
	if (alim->redePri == NULL)
		{return;}
	//pega os eqpto proprios
	alim->redePri->LisEqpto(lisEXT, tipo_eqpto);
	//pega das redes bt
	for (int i = 0; i < alim->lisREDESSEC->Count; i++)
	{
		rede = (VTRede*)alim->lisREDESSEC->Items[i];
		rede->LisEqpto(lisEXT, tipo_eqpto);
	}

}
// -----------------------------------------------------------------------------
void __fastcall TRelatorioValidacao::AnalisaNodeRede(VTNode *node)
{
	TList *lisRedeNode;
	TList *lisNodeJus;
	VTNode *nodeFilho;
	VTRede *rede, *redeSUB;
	TAlimentador *alimentador;
	TSED	 *sub;

	VTRede *redeSec;

	lisRedeNode = node->LisObjectNode();
	for (int nr = 0; nr < lisRedeNode->Count; nr++)
	{
		rede = (VTRede*) lisRedeNode->Items[nr];
		sub = NULL;
		redeSUB = NULL;
		if (rede->TipoRede == NULL)
		{
            continue;
		}
		//agrega valores de acordo com o tipo de rede
		switch (rede->TipoRede->Segmento)
		{
		case redePRI:
			redeSUB = RedeSEDmae(node);
			sub = ExisteSub(redeSUB);
			if (sub != NULL)
			{
				alimentador = sub->InsereAlimentador(rede);
				alimentador->redePri = rede;
				alimentador->redeSED = redeSUB;
				ListaRedesSec(node, alimentador->lisREDESSEC);
			}
			else
			{
				if (subestacaoResto != NULL)
				{
					alimentador = subestacaoResto->InsereAlimentador(rede);
					alimentador->redePri = rede;
					alimentador->redeSED = redeSUB;
					ListaRedesSec(node, alimentador->lisREDESSEC);
				}
            }
			break;
		case redeETD:
			if(lisRedeNode->Count == 1)
			{
			   sub = InsereSubestacao(rede);
			}
			break;
		default:
		;
		}
	}
	//olha os nodes filhos
	lisNodeJus = node->LisNode();
	for (int nf = 0; nf < lisNodeJus->Count; nf++)
	{
		nodeFilho = (VTNode*) lisNodeJus->Items[nf];
		//chamada recursiva
		AnalisaNodeRede(nodeFilho);
	}
}
// -----------------------------------------------------------------------------
bool __fastcall TRelatorioValidacao::CriaRelatorio(AnsiString dir, TSED *sub)
{
	bool sucesso = true;
	TStrings *arquivo;
	AnsiString filedir, filepath;
	TAlimentador *alim;
	TList *lisEQPTO = NULL;
	//arquivos
	TStrings *arquivoCargas;
	TStrings *arquivoCap;
	TStrings *arquivoChv;
	TStrings *arquivoTrafo;
	TStrings *arquivoRegulador;
	TStrings *arquivoTrecho;
	TStrings *arquivoGerador;
//	TStrings *arquivoRede;
	AnsiString adicionalSed ="";
	AnsiString adicionalCirc ="";
	AnsiString adicional = "";
	AnsiString header, headerSED, headerAlim;


	lisEQPTO = new TList;
	arquivoCargas = new TStringList;
	arquivoCap = new TStringList;
	arquivoChv = new TStringList;
	arquivoTrafo = new TStringList;
	arquivoRegulador = new TStringList;
	arquivoTrecho = new TStringList;
	arquivoGerador = new TStringList;
//	arquivoRede = new TStringList;
	//adiciona cabeçalho
	headerSED = "codigo_sub; externId_sub;";
	headerAlim = "codigo_alim; externId_alim;";
	header = "rede;carga;classe;especial;qtde_cons;fases;tipica;E(MWh/mes);curva;modelo;barra;consumo_gis (MWh/mes);";
	arquivoCargas->Add(header + headerSED + headerAlim);
	header = "rede;capacitor;q_kvar;curva;ligacao;vnom;barra;";
	arquivoCap->Add(header + headerSED + headerAlim);
	header = "rede;chave;tipo;iadm_a;estado;barra1;barra2;";
	arquivoChv->Add(header + headerSED + headerAlim);
	header = "rede;trafo;snom_kva;ligacao;r0_pu;x0_pu;r1_pu;x1_pu;perda_ferro;rotacao_fase;pri_vnom_kv;pri_tap_kv;pri_ater_r;pri_ater_x;sec_vnom_kv;sec_tap_kv;sec_ater_r;sec_ater_x;ltc_ajuste;ltc_barra;ltc_v_pu;barra1;barra2;";
	arquivoTrafo->Add(header + headerSED + headerAlim);
	header = "rede;regulador;snom_kva;modelo;ligacao;passo;dv_pu;v_pu;ajuste_auto;by_pass;";
	arquivoRegulador->Add(header + headerSED + headerAlim);
	header = "rede;trecho;arranjo;comprimento_m;r0_ohm;x0_ohm;b0_s;r1_ohm;x1_ohm;b1_s;barra1;barra2;tipo; cabo_fase; cabo_neutro; fases;";
	arquivoTrecho->Add(header + headerSED + headerAlim);
	header = "rede;gerador;tipo;ligacao;vnom_kv;snom_kva;r0_pu;x0_pu;r1_pu;x1_pu;fatpot;barra;";
	arquivoGerador->Add(header + headerSED + headerAlim);
	//guarda info da sed
	adicionalSed = sub->codigo + ";";
	if (sub->redeSED != NULL)
	{
		adicionalSed += sub->redeSED->Extern_id + ";";
	}
	else
	{
		adicionalSed += ";";
	}
	if (sub->redeSED  != NULL)
	{
        //primeiro faz pros items da sed
		adicional = adicionalSed + ";;";
		//adiciona relatorios de carga
		lisEQPTO->Clear();
		sub->redeSED->LisEqpto(lisEQPTO, eqptoCARGA);
		sucesso = CriaRelatorioEqpto(arquivoCargas, lisEQPTO, adicional);
		//adiciona relatorios de capacitor
		lisEQPTO->Clear();
		sub->redeSED->LisEqpto(lisEQPTO, eqptoCAPACITOR);
		sucesso = CriaRelatorioEqpto(arquivoCap, lisEQPTO, adicional);
		//adiciona relatorios de chave
		lisEQPTO->Clear();
		sub->redeSED->LisEqpto(lisEQPTO, eqptoCHAVE);
		sucesso = CriaRelatorioEqpto(arquivoChv, lisEQPTO, adicional);
		//adiciona relatorios de trafo
		lisEQPTO->Clear();
		sub->redeSED->LisEqpto(lisEQPTO, eqptoTRAFO);
		sucesso = CriaRelatorioEqpto(arquivoTrafo, lisEQPTO, adicional);
		//adiciona relatorios de regulador
		lisEQPTO->Clear();
		sub->redeSED->LisEqpto(lisEQPTO, eqptoREGULADOR);
		sucesso = CriaRelatorioEqpto(arquivoRegulador, lisEQPTO, adicional);
		//adiciona relatorios de trecho
		lisEQPTO->Clear();
		sub->redeSED->LisEqpto(lisEQPTO, eqptoTRECHO);
		sucesso = CriaRelatorioEqpto(arquivoTrecho, lisEQPTO, adicional);
	}

	//percorre cada limentador
	for (int i = 0; i < sub->lisALIM->Count; i++)
	{
		alim = (TAlimentador*) sub->lisALIM->Items[i];
		if (alim->redePri != NULL)
		{
			adicionalCirc = alim->redePri->Codigo + ";";
			adicionalCirc += alim->redePri->Extern_id + ";";
		}
		else
		{
			adicionalCirc = ";;";
		}
		adicional = adicionalSed + adicionalCirc;
		//adiciona relatorios de carga
		lisEQPTO->Clear();
		AgregaEqpto(alim, lisEQPTO, eqptoCARGA);
		sucesso = CriaRelatorioEqpto(arquivoCargas, lisEQPTO, adicional);
		//adiciona relatorios de capacitor
		lisEQPTO->Clear();
		AgregaEqpto(alim, lisEQPTO, eqptoCAPACITOR);
		sucesso = CriaRelatorioEqpto(arquivoCap, lisEQPTO, adicional);
		//adiciona relatorios de chave
		lisEQPTO->Clear();
		AgregaEqpto(alim, lisEQPTO, eqptoCHAVE);
		sucesso = CriaRelatorioEqpto(arquivoChv, lisEQPTO, adicional);
		//adiciona relatorios de trafo
		lisEQPTO->Clear();
		AgregaEqpto(alim, lisEQPTO, eqptoTRAFO);
		sucesso = CriaRelatorioEqpto(arquivoTrafo, lisEQPTO, adicional);
		//adiciona relatorios de regulador
		lisEQPTO->Clear();
		AgregaEqpto(alim, lisEQPTO, eqptoREGULADOR);
		sucesso = CriaRelatorioEqpto(arquivoRegulador, lisEQPTO, adicional);
		//adiciona relatorios de trecho
		lisEQPTO->Clear();
		AgregaEqpto(alim, lisEQPTO, eqptoTRECHO);
		sucesso = CriaRelatorioEqpto(arquivoTrecho, lisEQPTO, adicional);
		//adiciona relatorios de geradores
		lisEQPTO->Clear();
		AgregaEqpto(alim, lisEQPTO, eqptoGERADOR);
		sucesso = CriaRelatorioEqpto(arquivoGerador, lisEQPTO, adicional);

	}
	//salva todos arquivos
	if(sub->redeSED == NULL)
	{
		AnsiString agora = Date().FormatString("yyyymmdd") + Time().FormatString("hhmmsszzz");
		filedir = dir + "\\" + agora + sub->codigo;
	}
	else
	{
		filedir = dir + "\\" + sub->codigo;
	}
	//salva
	filepath = filedir + "_cargas.csv";
	arquivoCargas->SaveToFile(filepath);
	//salva
	filepath = filedir + "_capacitores.csv";
	arquivoCap->SaveToFile(filepath);
	//salva
	filepath = filedir + "_chaves.csv";
	arquivoChv->SaveToFile(filepath);
	//salva
	filepath = filedir + "_trafos.csv";
	arquivoTrafo->SaveToFile(filepath);
	//salva
	filepath = filedir + "_reguladores.csv";
	arquivoRegulador->SaveToFile(filepath);
    //salva
	filepath = filedir + "_trechos.csv";
	arquivoTrecho->SaveToFile(filepath);
	//salva
	filepath = filedir + "_geradores.csv";
	arquivoGerador->SaveToFile(filepath);
	//DELETA
	if (arquivoCargas)
		{delete arquivoCargas;}
	if (arquivoCap)
		{delete arquivoCap;}
	if (arquivoChv)
		{delete arquivoChv;}
	if (arquivoTrafo)
		{delete arquivoTrafo;}
	if (arquivoRegulador)
		{delete arquivoRegulador;}
	if (arquivoTrecho)
		{delete arquivoTrecho;}
	if (arquivoGerador)
		{delete arquivoGerador;}
//	if (arquivoRede)
//		{delete arquivoRede;}
    return sucesso;

}
// -----------------------------------------------------------------------------
bool __fastcall TRelatorioValidacao::CriaRelatorioEqpto(TStrings *strings, TList *lisEqpto,  AnsiString adicional)
{
	bool retorno = true;
	VTEqpto *eqpto = NULL;
	AnsiString linha = "";

	try
	{
		if (strings == NULL)
			{return false;}
		if (lisEqpto == NULL)
			{return false;}
		//percorre todos eqptos
		for (int i = 0; i < lisEqpto->Count; i++)
		{
			eqpto = (VTEqpto*)lisEqpto->Items[i];

			//tenta montar linha
			try
			{
				linha = MontaLinhaEqpto(eqpto);
				//adiciona info extra
				linha += adicional;
				strings->Add(linha);
			}
			catch (...)
			{
				//faz nada
            }
		}

	}
	catch (...)
	{
		retorno = false;
	}

	return retorno;
}
// ---------------------------------------------------------------------------
VTRede* __fastcall TRelatorioValidacao::ExisteRede(VTBarra *pbarra)
{
	// variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	// determina uma Barra que contenha a Barra indicada
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (rede->ExisteBarra(pbarra))
			return (rede);
	}
	return (NULL);
}
// -----------------------------------------------------------------------------
TSED* __fastcall TRelatorioValidacao::InsereSubestacao(VTRede *redeSed)
{
	TSED *al = NULL;

	if (redeSed != NULL)
	{
        al = ExisteSub(redeSed);
		if (al == NULL)
		{
			//verifica se existe
			al = new TSED(apl, redeSed);
			lisSED_->Add(al);
			mapSub.insert(std::pair<VTRede*, TSED*>(redeSed, al));
		}
	}

	return al;
}
// -----------------------------------------------------------------------------
void       __fastcall TRelatorioValidacao::MontaArvore(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
//	VTRede *rede;
	TList *lisNodeRaiz;
	VTNode *nodeRaiz;
	bool existe_gd = false;

	//antes de organizar a hierarquia as redes é preciso remover os trechos
	//que só possuem neutro, gerando malhas malucas
	//RemoveTrechosBTSomenteN();
	//remove gds existentes para montar arvore radial
	existe_gd = ExisteGD();
	if (existe_gd)
		RemoveGD();
	//inicia arvore com hierarquia de redes
	if (arvore_ != NULL)
	{
        delete arvore_;
	}
	arvore_ = DLL_NewObjArvore(apl);
	arvore_->Clear();
	arvore_->TipoArvore = arvoreREDE;
	arvore_->Inicia(redes);
	// reinsere GDs
	if (existe_gd)
		InsereGD();
	//lista todos os nós raiz
	lisNodeRaiz = arvore_->LisNodeRaiz();
	//dado os nós, analisa cada um
	for (int nn = 0 ; nn < lisNodeRaiz->Count; nn++)
	{
		nodeRaiz = (VTNode*)lisNodeRaiz->Items[nn];
		AnalisaNodeRede(nodeRaiz);
	}
	//adiciona a rede isolada na sub lixo
	if (subestacaoResto != NULL)
	{
        subestacaoResto->InsereAlimentador(redes->RedeIsolada());
	}
}
// -----------------------------------------------------------------------------
AnsiString __fastcall TRelatorioValidacao::MontaQuantidadesRede(VTRede *rede)
{
	// rede;tipo_rede;agrupada;radial;estatica;barra;trecho;trafo;chave;regulador;carga;capacitor;gerador;suprimento;
	AnsiString campo = "";

	try
	{ 	//rede
		campo = rede->Codigo + ";";
		//tipo_rede		if (rede->TipoRede != NULL)		{			campo += rede->TipoRede->Codigo + ";";
		}		else		{			campo += "indefinido;";
        }		//agrupada		if (rede->Cluster)		{
			campo += rede->Cluster->Codigo;
		}		else		{			campo += ";";
        }		//radial		campo += (rede->Radial) ? "Sim;" : "Não;";		//estatica		campo += (rede->Estatica) ? "Sim;" : "Não;";		//barra		campo += IntToStr(rede->NumeroDeBarras()) + ";";		//trecho		campo += IntToStr(rede->NumeroDeLigacoes(eqptoTRECHO)) + ";";		//trafo		campo += IntToStr(rede->NumeroDeLigacoes(eqptoTRAFO)) + ";";		//chave		campo += IntToStr(rede->NumeroDeLigacoes(eqptoCHAVE)) + ";";		//regulador		campo += IntToStr(rede->NumeroDeLigacoes(eqptoREGULADOR)) + ";";		//carga		campo += IntToStr(rede->NumeroDeEqbar(eqptoCARGA)) + ";";		//capacitor		campo += IntToStr(rede->NumeroDeEqbar(eqptoCAPACITOR)) + ";";		//gerador		campo += IntToStr(rede->NumeroDeEqbar(eqptoGERADOR)) + ";";		//suprimento
		campo += IntToStr(rede->NumeroDeEqbar(eqptoSUPRIMENTO)) + ";";
	}
	catch (Exception &e)
	{
	}

	return campo;

}
// -----------------------------------------------------------------------------
AnsiString __fastcall TRelatorioValidacao::MontaLinhaEqpto(VTEqpto *eqpto)
{
	AnsiString linha = "";
	int tipo;
	VTCarga *carga;
	VTCapacitor *cap;
	VTChave *chv;
	VTTrafo *trafo;
	VTRegulador *reg;
	VTTrecho *trecho;
	VTRede *rede;
    VTGerador *ger;

	if (eqpto == NULL)
		{throw Exception ("eqpto nulo.");}
	//verifica o tipo
	tipo = eqpto->Tipo();
	switch (tipo)
	{
		case eqptoCARGA:
			carga = (VTCarga*)eqpto;
			linha = MontaLinhaCarga(carga);
			break;
		case eqptoCAPACITOR:
			cap = (VTCapacitor*)eqpto;
			linha = MontaLinhaCapacitor(cap);
			break;
		case eqptoCHAVE:
			chv = (VTChave*)eqpto;
			linha = MontaLinhaChave(chv);
			break;
		case eqptoTRAFO:
			trafo = (VTTrafo*)eqpto;
			linha = MontaLinhaTrafo(trafo);
			break;
		case eqptoREGULADOR:
			reg = (VTRegulador*)eqpto;
			linha = MontaLinhaRegulador(reg);
			break;
		case eqptoTRECHO:
			trecho = (VTTrecho*)eqpto;
			linha = MontaLinhaTrecho(trecho);
			break;
		case eqptoREDE:
			rede = (VTRede*)eqpto;
			linha = MontaQuantidadesRede(rede);
			break;
		case eqptoGERADOR:
			ger = (VTGerador*)eqpto;
			linha = MontaLinhaGerador(ger);
		default:
			//throw Exception ("eqpto tipo invalido.");
			break;
	}

	return linha;
}
// -----------------------------------------------------------------------------
AnsiString __fastcall TRelatorioValidacao::MontaLinhaCarga(VTCarga *carga)
{
    // variáveis locais
	AnsiString campo = "";
	VTRede *rede = NULL;
	VTCarga *pcarga;
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
//	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	AnsiString txt;
	//rede;carga;classe;especial;qtde_cons;fases;tipica;E(MWh/mes);curva;modelo;barra;
    //extras : consumo_gis (MWh/mes);
	try
	{
		pcarga = carga;
		// atualiza campos
		rede = ExisteRede(pcarga->pbarra);
		if (rede != NULL)
		{
			campo = rede->Codigo + ";"; //rede
		}
		else
		{
			campo = "Indefinida;"; //rede
        }
		campo +=  pcarga->Codigo + ";"; //carga
		campo +=  pcarga->classe->Codigo + ";"; //classe
		//especial
		txt = (pcarga->VeryImportant) ? "Sim" : "Não";
		campo +=  txt + ";"; //especial
		campo +=  IntToStr(pcarga->NumConsTotal) + ";"; //qtde_cons
		campo += fases->AsStrUpper[pcarga->Fases]+ ";"; //fases
		txt =  (pcarga->curva->Tipica) ? "Sim" : "Não";
		campo +=   txt + ";"; //especial
		campo += DoubleToStr("%6.5f",demanda->Emes_mwh[pcarga])+ ";";//E(MWh/mes)
		//curva
        if (int(pcarga->curva->Tipica) == 0)
			campo += "própria;";
		else
			campo += pcarga->curva->Codigo + ";";
		//modelo
		switch (pcarga->ModeloCarga)
		{
			case 1:
				campo += "Icte;";
			case 2:
				campo += "Scte;";
			case 3:
				campo +="Zcte;";
			default:
				campo += "Inválida;";
				break;
		}
		campo += pcarga->pbarra->Codigo + ";"; //barra
		//extra
		//consumo gis
		campo += DoubleToStr("%6.5f",pcarga->Energia_mwhmes)+ ";";//E(MWh/mes)
	}
	catch (Exception &e)
	{
	}

	return campo;
}
// -----------------------------------------------------------------------------
AnsiString __fastcall TRelatorioValidacao::MontaLinhaCapacitor(VTCapacitor *capacitor)
{
	//rede;capacitor;q_kvar;curva;ligacao;vnom;barra;
	AnsiString campo = "";
	VTRede *rede;

	try
	{
		rede = ExisteRede(capacitor->pbarra);
		if (rede != NULL)
		{
			campo = rede->Codigo + ";"; //rede
		}
		else
		{
			campo = "Indefinida;"; //rede
		}
		campo +=  capacitor->Codigo + ";"; //capacitor
		campo += DoubleToStr("%6.5f",capacitor->q * 1000.)+ ";";//q_kvar
		campo +=  capacitor->curva->Codigo + ";"; //curva
		campo +=  TipoDeLigacao(capacitor->ligacao) + ";"; //ligacao
		campo += DoubleToStr("%6.5f",capacitor->vnom)+ ";";//vnom
		campo +=  capacitor->pbarra->Codigo + ";";//barra
	}
	catch (Exception &e)
	{

	}

	return campo;
}
// -----------------------------------------------------------------------------
AnsiString __fastcall TRelatorioValidacao::MontaLinhaChave(VTChave *chave)
{
	//rede;chave;tipo;iadm_a;estado;barra1;barra2;
	AnsiString campo = "";
	AnsiString txt;
	VTChave *pchave;
	VTTipoChave *tipoChave;

	try
	{
		pchave = chave;
		// preenche dados da Rede
		if (pchave->rede != NULL)
		{
			campo =  pchave->rede->Codigo + " ;"; //rede
		}
		else
		{
			campo = "Indefinida;";
        }
		campo +=  pchave->Codigo + ";"; //chave
		//tipo
        tipoChave = pchave->TipoChave;
		if (tipoChave)
		{
			txt = pchave->TipoChave->Codigo;
			// verifica se base fusível
			if (pchave->TipoChave->Tipo == chaveBF)
			{ // verifica se foi definido o elo
				if (!pchave->EloFusivel.IsEmpty())
				{ // inclui informação do elo
					txt = txt + " " + pchave->EloFusivel;
				}
			}
			campo += txt + ";"; //tipo
		}
		else
		{
			campo += "Indefinida;"; //tipo
		}
		campo += DoubleToStr("%6.5f",pchave->Inom_amp)+ ";";//iadm_a
		//estado
		if (chave->Aberta)
			{campo += "Aberta;";}
		else
			{campo += "Fechada;";}
		campo +=  pchave->pbarra1->Codigo + ";";//barra1;
		campo +=  pchave->pbarra2->Codigo + ";";//barra2;

	}
	catch (Exception &e)
	{
	}

	return campo;

}
// -----------------------------------------------------------------------------
AnsiString __fastcall TRelatorioValidacao::MontaLinhaTrafo(VTTrafo *trafo)
{
	// rede;trafo;snom_kva;ligacao;r0_pu;x0_pu;r1_pu;x1_pu;perda_ferro;rotacao_fase;pri_vnom_kv;pri_tap_kv;
	//pri_ater_r;pri_ater_x;sec_vnom_kv;sec_tap_kv;sec_ater_r;sec_ater_x;ltc_ajuste;ltc_barra;ltc_v_pu;barra1;barra2;
	AnsiString campo = "";
	// variáveis locais
	AnsiString txt = "Trafo";
	AnsiString tabela = "Trafo";
	double defasagem_grau;
	VTTrafo *ptrafo;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));


	try
	{
		ptrafo = trafo;
		defasagem_grau = ptrafo->DefasagemPriSec();
		// rede
		if (ptrafo->rede != NULL)
		{
			campo = ptrafo->rede->Codigo;
		}
		else
		{
			campo = "Indefinida;";
		}
		//trafo		campo +=  ptrafo->Codigo + ";"; //chave		//snom_kva		campo += DoubleToStr("%6.5f",ptrafo->snom * 1000.)+ ";";		//ligacao        if (ptrafo->Monofasico)		{
			txt = TipoDeLigacao(ptrafo->pri.ligacao) + ":" +
				fases->AsStrABCN[ptrafo->pri.fase] + "/" + fases->AsStrABCN[ptrafo->sec.fase];

		}
		else
		{
			txt = TipoDeLigacao(ptrafo->pri.ligacao) + TipoDeLigacao(ptrafo->sec.ligacao);
		}		campo += txt + ";"; //ligacao		//r0_pu		campo += DoubleToStr("%6.5f",ptrafo->z0.r)+ ";";		//x0_pu		campo += DoubleToStr("%6.5f",ptrafo->z0.x)+ ";";		//r1_pu		campo += DoubleToStr("%6.5f",ptrafo->z1.r)+ ";";		//x1_pu		campo += DoubleToStr("%6.5f",ptrafo->z1.x)+ ";";		//perda_ferro		campo += DoubleToStr("%6.5f",ptrafo->perda_ferro)+ ";";		//rotacao_fase		if (ptrafo->Monofasico)		{
			campo += DoubleToStr("%6.5f",0.)+ ";";
		}
		else
		{
			campo += DoubleToStr("%6.5f",defasagem_grau)+ ";";
		}		//pri_vnom_kv		campo += DoubleToStr("%6.5f",ptrafo->pri.vnom)+ ";";		//pri_tap_kv		campo += DoubleToStr("%6.5f",ptrafo->pri.tap)+ ";";		//pri_ater_r		campo += DoubleToStr("%6.5f",ptrafo->pri.zterra.r)+ ";";		//pri_ater_x		campo += DoubleToStr("%6.5f",ptrafo->pri.zterra.x)+ ";";		//sec_vnom_kv		campo += DoubleToStr("%6.5f",ptrafo->sec.vnom)+ ";";		//sec_tap_kv		campo += DoubleToStr("%6.5f",ptrafo->sec.tap)+ ";";		//sec_ater_r		campo += DoubleToStr("%6.5f",ptrafo->sec.zterra.r)+ ";";		//sec_ater_x		campo += DoubleToStr("%6.5f",ptrafo->sec.zterra.x)+ ";";		//ltc_ajuste		if (ptrafo->ltc.ajuste_auto.bar_ref == NULL)		{
			campo += "tap fixo;";
		}
		else
		{
			campo += (ptrafo->ltc.tipo == ltcPRIMARIO) ? "primário;" : "secundário;";
		}		//ltc_barra		if (ptrafo->ltc.ajuste_auto.bar_ref == NULL)		{
			campo += ";";
		}
		else
		{
			campo += ptrafo->ltc.ajuste_auto.bar_ref->Codigo + ";";
		}		//ltc_v_pu		if (ptrafo->ltc.ajuste_auto.bar_ref == NULL)		{
            campo += ";";
		}
		else
		{
			campo += DoubleToStr("%6.5f",ptrafo->ltc.ajuste_auto.v_pu)+ ";";
		}		//barra1		campo +=  ptrafo->pbarra1->Codigo + ";"; //		//barra2
		campo +=  ptrafo->pbarra2->Codigo + ";"; //

	}
	catch (Exception &e)
	{
	}

	return campo;
}
// -----------------------------------------------------------------------------
AnsiString __fastcall TRelatorioValidacao::MontaLinhaRegulador(VTRegulador *regulador)
{
	// rede;regulador;snom_kva;modelo;ligacao;passo;dv_pu;v_pu;ajuste_auto;by_pass;
	AnsiString campo = "";
	AnsiString txt = "Regulador";
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));


	try
	{
		//rede
		if (regulador->rede != NULL)
		{
			campo = regulador->rede->Codigo + ";";
		}
		else
		{
			campo = "Indefinida;";
		}
		//regulador		campo +=  regulador->Codigo + ";"; //chave		//snom_kva		campo += DoubleToStr("%6.5f",regulador->snom * 1000.)+ ";";		//modelo		campo += TraduzModeloRegulador(regulador->modelo) + ";";		//ligacao		campo += TipoDeLigacao(regulador->ligacao) + ":" + fases->AsStrABCN[regulador->fases] + ";";		//passo		campo += IntToStr(regulador->cfg_fixo.passo) + ";";		//dv_pu		campo += DoubleToStr("%6.5f",regulador->cfg_auto.dv_pu)+ ";";		//v_pu		campo += DoubleToStr("%6.5f",regulador->cfg_auto.v_pu)+ ";";		//ajuste_auto		campo += ";";		//by_pass
		campo += regulador->by_pass ? "sim;" : "não;";
	}
	catch (Exception &e)
	{
	}

	return campo;
}
// -----------------------------------------------------------------------------
AnsiString __fastcall TRelatorioValidacao::MontaLinhaTrecho(VTTrecho *trecho)
{
	// rede;trecho;arranjo;comprimento_m;r0_ohm;x0_ohm;b0_s;r1_ohm;x1_ohm;b1_s;barra1;barra2;
	//extras tipo; cabo_fase; cabo_neutro; fases;
	AnsiString campo = "";
	double r0, x0, b0;
	double r1, x1, b1;
	int compr_cod_arranjo = 58;
	AnsiString tabela = "Trecho";
	AnsiString cod_arranjo;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	TStrings *nome_arranjo;

	try
	{
		// obtém valores de impedância em ohm
		trecho->Z0_ohm(r0, x0, b0);
		trecho->Z1_ohm(r1, x1, b1);
		//rede
		if (trecho->rede != NULL)
		{
			campo = trecho->rede->Codigo  +";";
		}
		else
		{
			campo = "Indefinida;";
		}
		//trecho		campo +=  trecho->Codigo + ";"; //chave		//arranjo		if (trecho->ArranjoTipico)		{
			// DVK 2016.06.16 verifica comprimento do nome do arranjo
			campo += trecho->arranjo->Codigo + ";";
		}
		else
		{
			campo += "próprio;";
		}		//comprimento_m		campo += DoubleToStr("%6.5f",trecho->Comprimento_m)+ ";";		//r0_ohm		campo += DoubleToStr("%6.5f", r0)+ ";";		//x0_ohm		campo += DoubleToStr("%6.5f", x0)+ ";";		//b0_s		campo += DoubleToStr("%6.5f", b0)+ ";";		//r1_ohm		campo += DoubleToStr("%6.5f", r1)+ ";";		//x1_ohm		campo += DoubleToStr("%6.5f", x1)+ ";";		//b1_s		campo += DoubleToStr("%6.5f", b1)+ ";";		//barra1		campo += trecho->pbarra1->Codigo + ";";		//barra2
		campo += trecho->pbarra2->Codigo + ";";

		//extras
		if (trecho->ArranjoTipico)
		{
			AnsiString nome = "";
			nome_arranjo = new TStringList;
			nome = trecho->arranjo->Codigo;
			nome = StringReplace(nome, "[", "", TReplaceFlags() << rfReplaceAll);
			ExtraiStrings(nome, "]", nome_arranjo);
			//tipo
			if (nome_arranjo->Count > 0)
			{
				campo += nome_arranjo->Strings[0] + ";";
			}
			else
			{
				campo += ";;;";
			}
			//cabo_fase
			if (nome_arranjo->Count > 1)
			{
				campo += nome_arranjo->Strings[1] + ";";
			}
			else
			{
				campo += ";;";
			}
			//cabo_neutro
			if (nome_arranjo->Count > 2)
			{
				campo += nome_arranjo->Strings[2] + ";";
			}
			else
			{
				campo += ";";
			}
			delete nome_arranjo;
		}
		else
		{
			campo += ";;;";
		}
		//fases
		campo += fases->AsStrABCN[trecho->arranjo->Fases] + ";";

	}
	catch (Exception &e)
	{
	}
	return campo;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TRelatorioValidacao::MontaLinhaGerador(VTGerador *gerador)
{
    // rede;gerador;tipo;ligacao;vnom_kv;snom_kva;r0_pu;x0_pu;r1_pu;x1_pu;fatpot;barra;
	AnsiString campo = "";
	VTGerador *pger;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try
	{
			pger = gerador;
			rede = ExisteRede(pger->pbarra);
			//rede
			if (rede != NULL)
			{
				campo = rede->Codigo +";";
			}
			else
			{
				campo = "Indefinida;";
			}
			//gerador			campo += pger->Codigo +";";			//tipo			campo += TipoDeSuprimento(pger->tiposup) +";";			//ligacao			campo += TipoDeLigacao(pger->ligacao) +";";			//vnom_kv			campo += DoubleToStr("%6.5f",pger->vnom)+ ";";			//snom_kva			campo += DoubleToStr("%6.5f",pger->snom * 1000)+ ";";			//r0_pu			campo += DoubleToStr("%6.5f",pger->z0.r)+ ";";			//x0_pu			campo += DoubleToStr("%6.5f",pger->z0.x)+ ";";			//r1_pu			campo += DoubleToStr("%6.5f",pger->z1.r)+ ";";			//x1_pu			campo += DoubleToStr("%6.5f",pger->z1.x)+ ";";			//fatpot			campo += DoubleToStr("%6.5f",pger->fplim)+ ";";			//barra			campo += pger->pbarra->Codigo +";";
	}
	catch (Exception &e)
	{
	}
;
	return campo;
}
// ---------------------------------------------------------------------------
void __fastcall TRelatorioValidacao::ListaRedesSec(VTNode *primario, TList *lisREDESEC)
{
	TList *lisNodesFilhos;
	VTNode *nodeFilho;
	TList *lisRedeNode;
	VTRede *rede;

	lisREDESEC->Clear();
	//olha todas redes filhas
	lisNodesFilhos = primario->LisNode();
	for (int nf = 0; nf < lisNodesFilhos->Count; nf++)
	{
		nodeFilho = (VTNode*) lisNodesFilhos->Items[nf];
		lisRedeNode = nodeFilho->LisObjectNode();
		for (int nr = 0; nr < lisRedeNode->Count; nr++)
		{
			rede = (VTRede*) lisRedeNode->Items[nr];
			if (rede->TipoRede->Segmento == redeSEC)
			{
				if (lisREDESEC->IndexOf(rede) < 0)
				{
					lisREDESEC->Add(rede);
				}
			}
		}
	}
	/* TODO : testar performance contra usar assing laOR e retirar o que nao for redeSEC */
}
// ---------------------------------------------------------------------------
void __fastcall TRelatorioValidacao::InsereGD(void)
{
	// variáveis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// desfaz ultima ação, que foi remover todas as GDs
	edita->ActionUndo->Execute();

}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15641481
void __fastcall TRelatorioValidacao::RemoveGD(void)
{
	// variáveis locais
	TList *lisGER = new TList();
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// limpa lista
	lisGER->Clear();
	// determina todos os geradores e remove todos eles
	redes->LisEqpto(lisGER, eqptoGERADOR);
	edita->RetiraLisEqpto(lisGER);

	// destroi lista
	delete lisGER;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TRelatorioValidacao::TipoDeLigacao(int tipo_lig)
{
	switch (tipo_lig)
	{
	case lgEST_AT:
		return ("Yat");
	case lgTRIANG:
		return ("D");
	case lgEST_ISO:
		return ("Y");
	case lgMONOFASICO:
		return ("Mono");
	case lgDELTA_ABERTO:
		return ("Delta Aberto");
	case lgDELTA_FECHADO:
		return ("Delta Fechado");
	}
	return ("inválida");
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TRelatorioValidacao::TipoDeSuprimento(int suprimento)
{
	switch (suprimento)
	{
	case supPV:
		return ("PV");
	case supPQ:
		return ("PQ");
	case supVF:
		return ("VF");
	}
	return ("Inválida");
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TRelatorioValidacao::TraduzModeloRegulador(int tipo)
{
	switch (tipo)
	{
	case reguFIXO:
		return ("Tap fixo");
	case reguAUTO_UNI:
		return ("Tap automático unidirecional");
	case reguAUTO_BI:
		return ("Tap automático bidirecional");
	case reguAUTO_FIXA:
		return ("Tap automático barra fixa");
	case reguENERQCT:
		return ("Regulador BT");
	default:
		return ("Indefinido");
	}
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16555154
VTRede* __fastcall TRelatorioValidacao::RedeSEDmae(VTNode *node)
{
	TList *lisRedeNode;
	VTNode *nodePai;
	VTRede *rede;
	VTRede *redeSub = NULL;

	nodePai = node->Pai;
	if (nodePai == NULL)
	{
        return NULL;
	}
	lisRedeNode = nodePai->LisObjectNode();
	for (int nr = 0; nr < lisRedeNode->Count; nr++)
	{
		rede = (VTRede*) lisRedeNode->Items[nr];
		if (rede->TipoRede == NULL)
			{continue;}
		//agrega valores de acordo com o tipo de rede
		if (rede->TipoRede->Segmento == redeETD)
		{
			if(lisRedeNode->Count == 1)
				{redeSub = rede;}
		}
	}

	return redeSub;
}
// -----------------------------------------------------------------------------
// eof

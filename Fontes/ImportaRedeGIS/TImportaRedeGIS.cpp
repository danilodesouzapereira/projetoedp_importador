// arquivos incluídos ---------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Log\VTLog.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <ModulosOpcionais\Fontes\ImportaRedesBatch\VTBatchImportaRedesTask.h>
// Project include
#include <TImportaRedeGIS.h>
#include <Empresa\VTDadosEmpresa.h>
#include "..\Empresa\VTCondutor.h"
#include <Empresa\VTPrimario.h>
#include <Empresa\VTSubestacao.h>
#include <Forms\TFormImportadorEgrid.h>


// -------------------------------------------------------------------------------------------------
VTSub* __fastcall NewObjSub(void)
{
	try
	{
		return (new TSub());
	}
	catch (Exception &e)
	{
	}
	return (NULL);
}

// -------------------------------------------------------------------------------------------------
__fastcall TSub::TSub(void)
{
	// cria lista
	PD.lisPRI = new TStringList();
	PD.lisPRI_SEL = new TStringList();
	PD.lisSUB_FILHA = new TList();
}

// -------------------------------------------------------------------------------------------------
__fastcall TSub::~TSub(void)
{
	// destrói lista
	DestroiTObject(PD.lisPRI);
	DestroiTObject(PD.lisPRI_SEL);
    DestroiTObject(PD.lisSUB_FILHA);
}

// ---------------------------------------------------------------------------
VTImportaRedeGIS* __fastcall NewObjImportaRedeGIS(TComponent *Owner, VTApl *apl_owner)
{
	try
	{
		return (new TImportaRedeGIS(Owner, apl_owner));
	}
	catch (Exception &e)
	{
	}
	// erro na criação do objeto
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TImportaRedeGIS::TImportaRedeGIS(TComponent *Owner, VTApl *apl_owner)
{
	// salva ponteiro p/ objetos
	this->Owner = Owner;
	this->apl_owner = apl_owner;
	// cria lista
	lisSubestacao = new TList();
	lisAllSub     = new TList();
	// cria conversor, dados_empresa e ligamaefilha
	apl_owner->Add(conversor = NewObjConversor(apl_owner));
	apl_owner->Add(empresa = NewObjDadosEmpresa(apl_owner));
//	apl_owner->Add(new TLigaMaeFilha(apl_owner));
}

// ---------------------------------------------------------------------------
__fastcall TImportaRedeGIS::~TImportaRedeGIS(void)
{
	// destrói lista e seus objetos
	LimpaTList(lisAllSub);
	DestroiTObject(lisAllSub);
	// destrói lista sem destruir seus objetos
	DestroiTObject(lisSubestacao);
	// destrói objetos
//	DestroiTObject(conversor);
//	DestroiTObject(empresa);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaRedeGIS::ArquivoEgrid(void)
{
	return true;
}

// -------------------------------------------------------------------------------------------------
VTSub* __fastcall TImportaRedeGIS::ExisteSub(VTSubestacao *subestacao)
{
	// variáveis locais
	VTSub *sub;

	//proteção
    if(subestacao == NULL) return NULL;
	for(int n = 0; n < lisAllSub->Count; n++)
	{
		sub = (VTSub*)lisAllSub->Items[n];
		if(sub->CodSub.AnsiCompareIC(subestacao->Codigo) == 0)
			return sub;
	}
    return NULL;
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaRedeGIS::ImportaDadosEmpresa(AnsiString arq_egrid)
{
	// variáveis locais
	bool sucesso = false;

	try
	{ // proteção
		if (conversor == NULL)
			return false;
		conversor->BatchMode = true;
		// importa dados iniciais do egrid
        empresa->Clear();
		sucesso = conversor->ImportaDadosEmpresa(arq_egrid);
		//cria objetos VTSub com todas as subestações do egrid
		if(sucesso) IniciaLisAllSub();
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaRedeGIS::ImportaEgrid(strOPImporta *opcao, int destino)
{
	// variáveis locais
	bool sucesso = false;

	try
	{ // proteção
		if (conversor == NULL)
			return false;
		if (lisSubestacao->Count == 0)
			return false;
		conversor->BatchMode = true;
		// importa redes
		sucesso = conversor->ImportaRedes(lisSubestacao, opcao, destino);
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
void __fastcall TImportaRedeGIS::IniciaLisAllSub(void)
{ //obs.: EDP não possui subestações filhas
	// variaveis locais
//	TLigaMaeFilha *ligaMaeFilha = (TLigaMaeFilha*)apl_owner->GetObject(__classid(TLigaMaeFilha));
	VTDadosEmpresa *empresa = (VTDadosEmpresa*)apl_owner->GetObject(__classid(VTDadosEmpresa));
	VTPrimario *primario;
	VTSub *sub, *subfilha;
	VTSubestacao *subestacao, *subestacaofilha;
	TList *lisALL_SUB = empresa->LisSubestacao();
	TList *lisPRI, *lisSUB_FILHA;

	//cria lista
//	lisSUB_FILHA = new TList();
	for(int n = 0; n < lisALL_SUB->Count; n++)
	{
		subestacao = (VTSubestacao*)lisALL_SUB->Items[n];
		//cria objeto VTSub
		sub = NewObjSub();
		lisAllSub->Add(sub);
		sub->CodSub = subestacao->Codigo;
		lisPRI = subestacao->LisPrimario;
		for(int p = 0; p < lisPRI->Count; p++)
		{//insere primarios
			primario = (VTPrimario*)lisPRI->Items[p];
			sub->Primarios->Add(primario->Codigo);
		}
	}
	//percorre novamente subestações
//	for(int n = 0; n < lisALL_SUB->Count; n++)
//	{
//		subestacao = (VTSubestacao*)lisALL_SUB->Items[n];
//		sub = ExisteSub(subestacao);
//		if(sub == NULL) continue;
//		lisSUB_FILHA->Clear();
//		ligaMaeFilha->ExisteFilha(subestacao, lisSUB_FILHA);
//		for(int s = 0; s < lisSUB_FILHA->Count; s++)
//		{//insere sub filha
//			subestacaofilha = (VTSubestacao*)lisSUB_FILHA->Items[s];
//			subfilha = ExisteSub(subestacaofilha);
//			if(subfilha == NULL) continue;
//			sub->SubFilha->Add(subfilha);
//		}
//	}
	//destroi lista
//	delete lisSUB_FILHA;
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaRedeGIS::InsereSubestacao(TList *lisSub)
{
	// variaveis locais
	AnsiString pri;
	bool primario_selecionado = false;
	VTPrimario *primario;
	VTSub *sub;
	VTSubestacao *subestacao;
	VTDadosEmpresa *empresa = (VTDadosEmpresa*)apl_owner->GetObject(__classid(VTDadosEmpresa));

	// proteção
	if (empresa == NULL)
		return false;
	// limpa lista
	lisSubestacao->Clear();
	for (int n = 0; n < lisSub->Count; n++)
	{
		sub = (VTSub*)lisSub->Items[n];
		// localiza subestacao
		subestacao = empresa->ExisteSubestacao(sub->CodSub);
		if (subestacao == NULL)
			continue;
		// reinicia flag que indica se houve algum primario selecionado
		primario_selecionado = false;
		for (int p = 0; p < sub->PrimariosSel->Count; p++)
		{
			pri = sub->PrimariosSel->Strings[p];
			// localiza primario
			primario = subestacao->ExistePrimario(pri);
			if (primario == NULL)
				continue;
			// marca primário como selecionado
			primario->Selecionado = true;
			// altera flag
			primario_selecionado = true;
		}
		// insere subestação somente se houver algum primario selecionado
		if (primario_selecionado)
			lisSubestacao->Add(subestacao);
	}
	return (lisSubestacao->Count > 0);
}

// -------------------------------------------------------------------------------------------------
TList* __fastcall TImportaRedeGIS::LisAllSub(void)
{
	// retorna lista com todos objetos VTSub
    return lisAllSub;
}

// ---------------------------------------------------------------------------
bool __fastcall TImportaRedeGIS::ImportaLisArqPrimario(TStrings *files, strOPImporta *opcao)
{
	// //variáveis locais
	// bool         sucesso;
	// VTConversor  *conversor;
	//
	// try{//cria objeto Conversor
	// conversor = NewObjConversor(apl_owner);
	// //importa arquivos txt
	// sucesso = conversor->ImportaLisArqPrimario(files, opcao);
	// //destrói objeto Conversor
	// delete conversor;
	// }catch(Exception &e)
	// {
	// sucesso = false;
	// }
	// return(sucesso);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TImportaRedeGIS::ImportaLisArqSubestacao(TStrings *files, strOPImporta *opcao)
{
	// variáveis locais
	VTBatchImportaRedesTask *batchImporta = (VTBatchImportaRedesTask*)apl_owner->GetObject(__classid(VTBatchImportaRedesTask));

	bool sucesso = true;
	AnsiString filepath;
	TList *lisTMP;

	try
	{
		// protecao
		if (files == NULL)
		{
			if(batchImporta!=NULL){batchImporta->LogAdd("Ausência de arquivo para importação.");}
			throw new Exception("Ausência de arquivo para importação.");
		}
		if (files->Count > 0)
		{
			filepath = files->Strings[3]; //   arqEgrid
			if (filepath.IsEmpty())
			{
			if(batchImporta!=NULL){batchImporta->LogAdd("Caminho do arquivo Egrid está vazio..");}
				throw new Exception("Caminho do arquivo Egrid está vazio.");
			}
		}
		//
		empresa->LisRegional()->Clear();
		empresa->LisLocalidade()->Clear();
		empresa->LisSubestacao()->Clear();
		empresa->LisPrimario()->Clear();
		//
		conversor->BatchMode = true;
		//
		sucesso = conversor->ImportaDadosEmpresa(filepath);
		lisTMP = empresa->LisSubestacao();
		// Condutores default
		empresa->CondutorDefaultBT =(VTCondutor*) empresa->LisCondutor()->Items[47];
		empresa->CondutorDefaultMT =(VTCondutor*) empresa->LisCondutor()->Items[261];
		empresa->CondutorDefaultIP =(VTCondutor*) empresa->LisCondutor()->Items[358];
		empresa->CondutorDefaultRL =(VTCondutor*) empresa->LisCondutor()->Items[201];
//		// define pasta para o caso de salvar as redes
		 if(batchImporta!=NULL)
		{
			batchImporta->LogAdd("\n Cabo a ser utilizado caso o cabo do trecho não esteja definido :" , false);
			batchImporta->LogAdd(" - Rede Primária: " + empresa->CondutorDefaultMT->codigo, false);
			batchImporta->LogAdd(" - Rede Secundária: " + empresa->CondutorDefaultBT->codigo, false);
			batchImporta->LogAdd(" - Iluminação Pública: " + empresa->CondutorDefaultIP->codigo, false);
			batchImporta->LogAdd(" - Ramal de Ligação: " + empresa->CondutorDefaultRL->codigo, false);
		}
		conversor->Pasta = files->Strings[1]; // pastaDestino
		conversor->BatchMode = true;
//		// importa arquivos do .EGRID
		if (sucesso)
		{
			conversor->ImportaRedes(lisTMP, opcao, 2); // destino 0 = memoria
		}
		// destrói objeto Conversor
		//delete conversor;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TImportaRedeGIS::SelecionaArqTxt(TOpenDialog *OpenDialog, TStrings *files)
{
	// //variáveis locais
	// VTConversor  *conversor;
	//
	// try{//cria objeto Conversor
	// conversor = NewObjConversor(apl_owner);
	// //seleciona arquivos txt
	// conversor->SelecionaArqTxt(OpenDialog, files);
	// //destrói objeto Conversor
	// delete conversor;
	// }catch(Exception &e)
	// {
	// }
}

// ---------------------------------------------------------------------------
void __fastcall TImportaRedeGIS::SelecionaLisArqTxt(TOpenDialog *OpenDialog, TStrings *files)
{
	// //variáveis locais
	// VTConversor  *conversor;
	//
	// try{//cria objeto Conversor
	// conversor = NewObjConversor(apl_owner);
	// //seleciona arquivos txt
	// conversor->SelecionaLisArqTxt(OpenDialog, files);
	// //destrói objeto Conversor
	// delete conversor;
	// }catch(Exception &e)
	// {
	// }
}

// ---------------------------------------------------------------------------
bool __fastcall TImportaRedeGIS::ShowModalFormImportaRedeGIS(void)
{
	// variáveis locais
	TForm *form_child;
	bool sucesso = false;
//	TFormOracle *Form;
	TFormImportadorEgrid *Form;

	try
	{ // cria TFormOracle como janela modal
//		Form = new TFormOracle(Owner, apl_owner);
		Form = new TFormImportadorEgrid(Owner, apl_owner, NULL);
		sucesso = Form->ShowModal();
		if (Form->ModalResult == mrCancel)
		{
			//if ((form_child = ExisteForm("TFormChild", Owner)) != NULL)
			form_child = (TForm*)Owner->Owner;
			if(form_child->ClassName().CompareIC("TFormChild") != 0)
			{
                delete Form;
			}
			else
			{
				delete Form;
				form_child->Close();
			}
		}
		else
		{
			delete Form;
		}
	}
	catch (Exception &e)
	{
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TImportaRedeGIS::ShowModalFormImportaRedeGIS_SDBT(void)
{
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TImportaRedeGIS::ShowModalFormImportaRedeGIS_SDMT(void)
{
	return (false);
}

// -----------------------------------------------------------------------------
// eof

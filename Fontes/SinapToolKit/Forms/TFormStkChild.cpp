// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Licenca\VTLicenca.h>
#include <PlataformaSinap\Fontes\Licenca\VTLicencas.h>
#include <PlataformaSinap\Fontes\MultiMonitor\VTMultiPanel.h>
#include <PlataformaSinap\Fontes\Sinap\VTSinapChild.h>
#include <PlataformaSinap\Fontes\Stk\VTStkLicenca.h>

#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Gateway.h>
#include <PlataformaSinap\DLL_Inc\CalcIndCont.h>
#include <PlataformaSinap\DLL_Inc\ImportaRedeGIS.h>
#include <PlataformaSinap\DLL_Inc\Simulador.h>
#include <PlataformaSinap\DLL_Inc\Stk.h>

#include <ModulosOpcionais\DLL_Inc\AlocaChaves.h>
#include <ModulosOpcionais\DLL_Inc\ExtratorOcorrencias.h>
#include <ModulosOpcionais\DLL_Inc\ImportaRedeAccess.h>
#include <ModulosOpcionais\DLL_Inc\GeralMainFormAlocaChavesP1.h>

#include "TFormStkChild.h"
#include "..\Socorro\VTConecta.h"
#include "TRelatorioValidacao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormStkChild::TFormStkChild(TComponent *Owner, VTApl *apl, int stk_option)
	: TForm(Owner)
{
	// variáveis locais
	bool expand = true;
	VTSinapChild *sinap_child = (VTSinapChild*) apl->GetObject(__classid(VTSinapChild));
	VTLicenca *licenca;
	VTLicencas *licencas = (VTLicencas*) apl->GetObject(__classid(VTLicencas));
	AnsiString cod_empresa = "Não cadastrada";

	// salva ponteiro de objetos externos
	this->apl = apl;
	// obtém dados da Licenca
	licenca = licencas->ExisteLicenca();
	if (licenca != NULL)
		cod_empresa = licenca->EmpresaStr;
	// trata caso de licença MASTER
	if (licenca != NULL)
	{ // habilita importador Access
		if (licenca->Tipo == tipoMASTER)
			ActionImportaAccess->Visible = true;
		else
			ActionImportaAccess->Visible = false;
	}
	// insere Menu da empresa
	sinap_child->FormStkAdd("STK: " + cod_empresa, ActionList, expand);
	// insere Menu para Enerq
	// sinap_child->FormStkAdd("STK: Laboratório", ActionListEnerq, expand);
	// configura Action a ser executada toda vez que uma rede for carregada
	sinap_child->ActionRedeCarregada = ActionRedeCarregada;
	// verifica se foi definida alguma opção
	switch (stk_option)
	{
	case 1: // executa módulo de importação de rede do Oracle

		ActionImportaRedeGIS->Execute();
		break;
	default: // nada a fazer
		// ActionImportaRedeGISExecute(this);
		break;
	}

}

// ---------------------------------------------------------------------------
__fastcall TFormStkChild::~TFormStkChild(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkChild::ActionAlocaChavesExecute(TObject *Sender)
{
	TForm *Form;
	VTMultiPanel *MultiPanel = (VTMultiPanel*)apl->GetObject(__classid(VTMultiPanel));
	Form = DLL_NewFormAlocaChaves(this, apl, MultiPanel->PanelAtivo);
	Form->Show();
}


// ---------------------------------------------------------------------------
void __fastcall TFormStkChild::ActionEnumeracaoExecute(TObject *Sender)
{
	TForm *Form;
	VTMultiPanel *MultiPanel = (VTMultiPanel*)apl->GetObject(__classid(VTMultiPanel));
	// Form = DLL_NewFormCalcIndCont(this, apl, MultiPanel->PanelAtivo);
	Form = DLL_Geral_NewGeralFormAlocaChavesP1(this, apl, MultiPanel->PanelAtivo);
	Form->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkChild::ActionGatewayExecute(TObject *Sender)
{
	// variáveis locais
	TForm *Form;

	// cria TFormImportaEqpto e exibe como janela normal
	Form = DLL_NewFormGateway(this, apl);
	Form->ShowModal();
	delete Form;
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkChild::ActionImportaAccessExecute(TObject *Sender)
{
	// variáveis locais
	VTImportaRedeGIS *ImportaRedeGIS;

	if ((ImportaRedeGIS = DLL_NewObjImportaRedeAccess(this, apl)) != NULL)
	{
		ImportaRedeGIS->ShowModalFormImportaRedeGIS();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkChild::ActionImportaRedeGISExecute(TObject *Sender)
{
	// variáveis locais
	VTImportaRedeGIS *ImportaRedeGIS;

	if ((ImportaRedeGIS = DLL_NewObjImportaRedeGIS(this, apl)) != NULL)
	{
		ImportaRedeGIS->ShowModalFormImportaRedeGIS();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkChild::ActionOcorrenciasExecute(TObject *Sender)
{
	TForm *Form;
	VTMultiPanel *MultiPanel = (VTMultiPanel*)apl->GetObject(__classid(VTMultiPanel));
	Form = DLL_NewFormExtratorDados(this, apl, MultiPanel->PanelAtivo);
	Form->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkChild::ActionRedeCarregadaExecute(TObject *Sender)
{
	// variáveis locais
	VTConecta *conecta;

	try
	{ // cria objeto para conectar as chaves de socorro
		conecta = NewObjConecta(apl);
		if (conecta != NULL)
		{
			conecta->Executa();
			// destrói objeto
			delete conecta;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkChild::ActionSimuladorClientExecute(TObject *Sender)
{
	// veriáveis locais
	TForm *form;
	// exibe tela do Servidor

	form = DLL_NewFormClient(this, apl, SelecionaParentParaJanela());
	if (form)
		form->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkChild::ActionSimuladorServerExecute(TObject *Sender)
{
	// veriáveis locais
	TForm *form;
	// exibe tela do Servidor

	form = DLL_NewFormServer(this, apl, SelecionaParentParaJanela());
	if (form)
		form->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkChild::Atualiza(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
TWinControl* __fastcall TFormStkChild::SelecionaParentParaJanela(void)
{
	// variáveis locais
	VTMultiPanel *MultiPanel = (VTMultiPanel*)apl->GetObject(__classid(VTMultiPanel));

	return (MultiPanel->PanelAtivo);
}

// ---------------------------------------------------------------------------
// eof

void __fastcall TFormStkChild::ActionRelExtracaoExecute(TObject *Sender)
{
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	TRelatorioValidacao *rel;

	rel = new TRelatorioValidacao(apl);
	if (rel->Executa(path->DirExporta()))
	{
		Aviso("Relatórios exportados com sucesso para " + path->DirExporta());
	}
	else
	{
		Aviso("FALHA.");
    }

	delete rel;
}
//---------------------------------------------------------------------------


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
	// vari�veis locais
	bool expand = true;
	VTSinapChild *sinap_child = (VTSinapChild*) apl->GetObject(__classid(VTSinapChild));
	VTLicenca *licenca;
	VTLicencas *licencas = (VTLicencas*) apl->GetObject(__classid(VTLicencas));
	AnsiString cod_empresa = "N�o cadastrada";

	// salva ponteiro de objetos externos
	this->apl = apl;
	// obt�m dados da Licenca
	licenca = licencas->ExisteLicenca();
	if (licenca != NULL)
		cod_empresa = licenca->EmpresaStr;
	// trata caso de licen�a MASTER
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
	// sinap_child->FormStkAdd("STK: Laborat�rio", ActionListEnerq, expand);
	// configura Action a ser executada toda vez que uma rede for carregada
	sinap_child->ActionRedeCarregada = ActionRedeCarregada;
	// verifica se foi definida alguma op��o
	switch (stk_option)
	{
	case 1: // executa m�dulo de importa��o de rede do Oracle

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
	// vari�veis locais
	TForm *Form;

	// cria TFormImportaEqpto e exibe como janela normal
	Form = DLL_NewFormGateway(this, apl);
	Form->ShowModal();
	delete Form;
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkChild::ActionImportaAccessExecute(TObject *Sender)
{
	// vari�veis locais
	VTImportaRedeGIS *ImportaRedeGIS;

	if ((ImportaRedeGIS = DLL_NewObjImportaRedeAccess(this, apl)) != NULL)
	{
		ImportaRedeGIS->ShowModalFormImportaRedeGIS();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkChild::ActionImportaRedeGISExecute(TObject *Sender)
{
	// vari�veis locais
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
	// vari�veis locais
	VTConecta *conecta;

	try
	{ // cria objeto para conectar as chaves de socorro
		conecta = NewObjConecta(apl);
		if (conecta != NULL)
		{
			conecta->Executa();
			// destr�i objeto
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
	// veri�veis locais
	TForm *form;
	// exibe tela do Servidor

	form = DLL_NewFormClient(this, apl, SelecionaParentParaJanela());
	if (form)
		form->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkChild::ActionSimuladorServerExecute(TObject *Sender)
{
	// veri�veis locais
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
	// vari�veis locais
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
		Aviso("Relat�rios exportados com sucesso para " + path->DirExporta());
	}
	else
	{
		Aviso("FALHA.");
    }

	delete rel;
}
//---------------------------------------------------------------------------


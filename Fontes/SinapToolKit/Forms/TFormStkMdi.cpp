// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Sinap\VTSinapMdi.h>
#include <PlataformaSinap\Fontes\Stk\VTStkLicenca.h>
#include <PlataformaSinap\DLL_Inc\Stk.h>
#include "TFormStkMdi.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormStkMdi::TFormStkMdi(TComponent *Owner, VTApl *apl_owner) : TForm(Owner)
{
	// salva ponteiro para objeto
	this->apl = apl_owner;
	// configura MainMenu: inclui MenuItem para executar importador de rede
	ConfiguraMainMenu();
	// DVC: 18/12/18
    //Foi tirado o botão que ficava do lado de IntegraGis (STK:EDPES ou STK:EDPSP)
	// configura PopupMenuImporta: inclui MenuItem para executar importador de rede
	//ConfiguraPopupMenuImporta();
}

// ---------------------------------------------------------------------------
__fastcall TFormStkMdi::~TFormStkMdi(void)
{
    //nada
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkMdi::ActionImportaOracleExecute(TObject *Sender)
{
	// variáveis locais
	VTSinapMdi *sinap_mdi = (VTSinapMdi*)apl->GetObject(__classid(VTSinapMdi));

	// proteção
	if (sinap_mdi == NULL)
		return;
	// executa Action para criar um novo FormChild
	sinap_mdi->ActionNewChild->OnExecute((TObject*)1);
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkMdi::ConfiguraMainMenu(void)
{
//	// variáveis locais
//	TAction *Action;
//	TMenuItem *MenuItem, *MenuItemEDP;
//	VTStkLicenca *stk;
//	VTSinapMdi *sinap_mdi = (VTSinapMdi*)apl->GetObject(__classid(VTSinapMdi));
//
//	// proteção
//	if (sinap_mdi == NULL)
//		return;
//	// obtém dados da Licenca
//	stk = DLL_NewObjStkLicenca(apl);
//	// inclui MenuItem identificando integração com GIS
//	MenuItemEDP = new TMenuItem(sinap_mdi->MainMenu);
//	MenuItemEDP->Action = NULL;
//	MenuItemEDP->Hint = "";
//	MenuItemEDP->Caption = "STK: " + stk->EmpresaNome();
//	MenuItemEDP->GroupIndex = 0;
//	MenuItemEDP->RadioItem = false;
//	MenuItemEDP->AutoCheck = false;
//	MenuItemEDP->Checked = false;
//	sinap_mdi->MainMenu->Items->Insert(3, MenuItemEDP);
//	// inclui MenuItem para cada Action de ActionList
//	for (int n = 0; n < ActionList->ActionCount; n++)
//	{
//		Action = (TAction*)ActionList->Actions[n];
//		// cria novo MenuItem
//		MenuItem = new TMenuItem(sinap_mdi->MainMenu);
//		MenuItem->Action = Action;
//		MenuItem->Hint = "";
//		MenuItem->Caption = Action->Caption;
//		MenuItem->GroupIndex = 0;
//		MenuItem->RadioItem = false;
//		MenuItem->AutoCheck = false;
//		MenuItem->Checked = false;
//		MenuItemEDP->Add(MenuItem);
//	}
//	// destrói objeto StkLicenca
//	delete stk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormStkMdi::ConfiguraPopupMenuImporta(void)
{
	// variáveis locais
	TAction *Action;
	TMenuItem *MenuItem;
	VTSinapMdi *sinap_mdi = (VTSinapMdi*)apl->GetObject(__classid(VTSinapMdi));

	// proteção
	if (sinap_mdi == NULL)
		return;
	// inclui MenuItem para cada Action de ActionList
	for (int n = 0; n < ActionList->ActionCount; n++)
	{
		Action = (TAction*)ActionList->Actions[n];
		// cria novo MenuItem
		MenuItem = new TMenuItem(sinap_mdi->PopupMenuImporta);
		MenuItem->Action = Action;
		MenuItem->Hint = "";
		MenuItem->Caption = Action->Caption;
		MenuItem->GroupIndex = 0;
		MenuItem->RadioItem = false;
		MenuItem->AutoCheck = false;
		MenuItem->Checked = false;
		sinap_mdi->PopupMenuImporta->Items->Add(MenuItem);
	}
}

// ---------------------------------------------------------------------------
// eof

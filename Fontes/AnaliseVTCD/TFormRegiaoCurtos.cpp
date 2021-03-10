//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormRegiaoCurtos.h"
#include "VTCalculaVTCD.h"
#include "TRegiaoVTCD.h"
#include "TUcsv.h"
#include "VTBarraVTCD.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\RedeCC\ResCurto.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormRegiaoCurtos *FormRegiaoCurtos;
//---------------------------------------------------------------------------
__fastcall TFormRegiaoCurtos::TFormRegiaoCurtos(TComponent* Owner, VTApl *apl_owner, TRegiaoVTCD *regiaoVTCD, TWinControl *parent)
	: TForm(Owner)
   {
   this->Parent = parent;
   this->apl = apl_owner;
   this->regiaoVTCD = regiaoVTCD;

   this->Caption = regiaoVTCD->chave->Codigo;

   ListaCurtos();

   }
//---------------------------------------------------------------------------
__fastcall TFormRegiaoCurtos::~TFormRegiaoCurtos(void)
   {

   }
//---------------------------------------------------------------------------
void __fastcall TFormRegiaoCurtos::FormClose(TObject *Sender, TCloseAction &Action)

   {
   Action = caFree;

   }
//---------------------------------------------------------------------------
void __fastcall TFormRegiaoCurtos::ListaCurtos(void)
   {
   VTBarraVTCD  *pbarV;
   TUcsv *ucsv;//str_UCSV     *ucsv;
   str_ResUCSV  *res_ucsv;
   TListItem    *item;
   AnsiString   aux;
   TListColumn *column;
   AnsiString  TipoCurto[5];

   TipoCurto[0] =  "defeito3F";
   TipoCurto[1] =  "defeito2F";
   TipoCurto[2] =  "defeitoFT";
   TipoCurto[3] = "defeitoFTZ";
   TipoCurto[4] = "defeito2FT";

   if(regiaoVTCD->lisBarrasCurto->Count == 0) return;
   lvRegiaoCurtos->Items->Clear();

   pbarV = (VTBarraVTCD*)regiaoVTCD->lisBarrasCurto->First();
   if(pbarV->lisResVTCD->Count > 0)
	 {
	 for(int ndef = 0; ndef < NUM_MAX_DEF; ndef++)///NUM_MAX_DEF = 5
		{
		if(pbarV->FlagExecuta[ndef] == false) continue;
		aux.sprintf("icc_%s", TipoCurto[ndef]);
		column = lvRegiaoCurtos->Columns->Add();
		column->Caption = aux;
		column->Width   = 120;
		for(int nres = 0; nres < pbarV->lisResVTCD->Count; nres++)
		   {
		   aux.sprintf("vpu_%i", nres);
		   column = lvRegiaoCurtos->Columns->Add();
		   column->Caption = aux;
		   column->Width   = 120;
		   }
		}
	 }
   for(int nb = 0; nb < regiaoVTCD->lisBarrasCurto->Count; nb++)
	  {
	  pbarV = (VTBarraVTCD*)regiaoVTCD->lisBarrasCurto->Items[nb];
	  item = lvRegiaoCurtos->Items->Add();
	  item->Data = pbarV;
	  item->Caption = pbarV->Barra->Codigo;
	  aux.sprintf("%6.2f", pbarV->Na);
	  item->SubItems->Add(aux);

	  for(int ndef = 0; ndef < NUM_MAX_DEF; ndef++)///NUM_MAX_DEF = 5
		 {
		 if(pbarV->FlagExecuta[ndef] == false) continue;
		 aux.sprintf("%6.2f", pbarV->icc[ndef][0]); item->SubItems->Add(aux);
		 for(int nres = 0; nres < pbarV->lisResVTCD->Count; nres++)
			{
			res_ucsv = (str_ResUCSV*)pbarV->lisResVTCD->Items[nres];
			aux.sprintf("%6.2f", res_ucsv->vtcd[ndef][0]); item->SubItems->Add(aux);
			}
		 }
	  }
   }
//---------------------------------------------------------------------------
void __fastcall TFormRegiaoCurtos::ActionSelBarrasExecute(TObject *Sender)
   {
   VTGrafico      *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   TListItem *item;
   TList     *lisBarras;
   lisBarras = new TList();
   for(int n = 0; n < lvRegiaoCurtos->Items->Count; n++)
	  {
	  item = lvRegiaoCurtos->Items->Item[n];
	  if(!item->Selected) continue;
	  lisBarras->Add(((VTBarraVTCD*)item->Data)->Barra);
	  }

   grafico->Moldura(lisBarras);
   }
//---------------------------------------------------------------------------

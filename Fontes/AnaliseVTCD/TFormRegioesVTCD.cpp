//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormRegioesVTCD.h"
#include "TFormRegiaoCurtos.h"
#include "TRegiaoVTCD.h"
#include "VTBarraVTCD.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormRegioesVTCD *FormRegioesVTCD;
// ---------------------------------------------------------------------------
// declaração
ORDENACAO listViewSort;

//---------------------------------------------------------------------------
__fastcall TFormRegioesVTCD::TFormRegioesVTCD(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	: TForm(Owner)
   {
   this->Parent = parent;
   //this->Parent = parent;
   apl = apl_owner;

   //Inicia formRegiaoCurtos
   formRegiaoCurtos = NULL;

   gbBarrasCurto->Visible = false;
   }
//---------------------------------------------------------------------------
__fastcall TFormRegioesVTCD::~TFormRegioesVTCD(void)
   {

   }
//---------------------------------------------------------------------------
void __fastcall TFormRegioesVTCD::MostraRegioes(TList *lisRegioes)
   {
   TListItem *item;
   //TList *lisBlocosRegiao;
   TRegiaoVTCD *regiaoVTCD;
   AnsiString  aux;

   /*lbRegioes->Items->Clear();
   for(int n = 0; n < lisRegioes->Count; n++)
	  {
	  //lisBlocosRegiao = (TList*)lisRegioes->Items[n];
	  regiaoVTCD = (TRegiaoVTCD*)lisRegioes->Items[n];
	  aux.sprintf("%i-%s", n, regiaoVTCD->chave->Codigo);
	  lbRegioes->AddItem(aux, regiaoVTCD);
	  }
   */
   lvRegioes->Items->Clear();
   for(int n = 0; n < lisRegioes->Count; n++)
	  {
	  //lisBlocosRegiao = (TList*)lisRegioes->Items[n];
	  regiaoVTCD = (TRegiaoVTCD*)lisRegioes->Items[n];
	  item = lvRegioes->Items->Add();
	  item->Data = regiaoVTCD;
	  item->Caption = regiaoVTCD->chave->rede->Codigo;
	  item->SubItems->Add(regiaoVTCD->chave->Codigo);
	  }
   if(lvRegioes->Items->Count > 0)
	 {lvRegioes->ItemIndex = 0;
	  ActionMostraRegiaoExecute(NULL);
     }
   }
//---------------------------------------------------------------------------
void __fastcall TFormRegioesVTCD::FormClose(TObject *Sender, TCloseAction &Action)

   {
   //this->Parent->Show();
   Action = caFree;
   }
//---------------------------------------------------------------------------
void __fastcall TFormRegioesVTCD::ActionMostraRegiaoExecute(TObject *Sender)
   {
   VTBloco *bloco;
   VTGrafico      *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   int index;
   //TList *lisBlocosRegiao;
   TRegiaoVTCD *regiaoVTCD;
   TList *lisLigaBlocoRegiao;

   if((index = lvRegioes->ItemIndex) < 0) return;
   lisLigaBlocoRegiao = new TList();
   //lisBlocosRegiao = (TList*)lbRegioes->Items->Objects[index];

   regiaoVTCD = (TRegiaoVTCD*)lvRegioes->Items->Item[index]->Data;
   //regiaoVTCD = (TRegiaoVTCD*)lvRegioes->Items->Objects[index];
   for(int n = 0; n < regiaoVTCD->lisBlocosRegiao->Count; n++)
	  {
	  bloco = (VTBloco*)regiaoVTCD->lisBlocosRegiao->Items[n];
	  lisLigaBlocoRegiao->Assign(bloco->LisLigacao(), laOr);
	  }
   grafico->DestacaEqpto(lisLigaBlocoRegiao, clGray, 10);
   //grafico->Moldura(regiaoVTCD->lisEqProt);


   delete lisLigaBlocoRegiao;

   //ActionMostraEqProtExecute(NULL);

   if(formRegiaoCurtos) delete formRegiaoCurtos;
   formRegiaoCurtos = new TFormRegiaoCurtos(this->Owner, apl, regiaoVTCD, gbBarrasCurto);
   formRegiaoCurtos->Align = alClient;
   formRegiaoCurtos->Show();

   gbBarrasCurto->Visible = true;
   //ActionMostraBarrasCurtoExecute(NULL);
   }
//---------------------------------------------------------------------------

void __fastcall TFormRegioesVTCD::ActionMostraEqProtExecute(TObject *Sender)
   {
   VTBloco *bloco;
   VTGrafico      *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   int index;
   TRegiaoVTCD *regiaoVTCD;
   TList *lisLigaBlocoRegiao;

   if((index = lvRegioes->ItemIndex) < 0) return;

   //lisBlocosRegiao = (TList*)lbRegioes->Items->Objects[index];
   regiaoVTCD = (TRegiaoVTCD*)lvRegioes->Items->Item[index]->Data;//Objects[index];

   grafico->Moldura(regiaoVTCD->lisEqProt);
   }
//---------------------------------------------------------------------------

void __fastcall TFormRegioesVTCD::ActionMostraBarrasCurtoExecute(TObject *Sender)

   {
   VTBloco *bloco;
   VTGrafico   *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   int index;
   TRegiaoVTCD *regiaoVTCD;
   TList *lisLigaBlocoRegiao;

   if((index = lvRegioes->ItemIndex) < 0)
	  {
	  if(lvRegioes->Items->Count == 0) return;
      index = 0;
	  }

   //lisBlocosRegiao = (TList*)lbRegioes->Items->Objects[index];
   regiaoVTCD = (TRegiaoVTCD*)lvRegioes->Items->Item[index]->Data;
   //regiaoVTCD = (TRegiaoVTCD*)lvRegioes->Items->Objects[index];
   TList *lisBarra;
   VTBarraVTCD *pbarV;

   lisBarra = new TList();
   for(int n = 0; n < regiaoVTCD->lisBarrasCurto->Count; n++)
	  {
	  pbarV = (VTBarraVTCD*)regiaoVTCD->lisBarrasCurto->Items[n];
	  lisBarra->Add(pbarV->Barra);
	  }
   grafico->Moldura(lisBarra);
   delete lisBarra;


   }
//---------------------------------------------------------------------------

void __fastcall TFormRegioesVTCD::lvRegioesColumnClick(TObject *Sender, TListColumn *Column)

   {
	// verifica se selecionou a mesma coluna
	if (listViewSort.ColumnIndex == Column->Index)
	{ // inverte ordem
		listViewSort.OrdemCrescente = !listViewSort.OrdemCrescente;
	}
	else
	{ // redefine índice da coluna usada p/ ordenação
		listViewSort.ColumnIndex = Column->Index;
	}
	// força reordenação
	lvRegioes->AlphaSort();
   }
//---------------------------------------------------------------------------

void __fastcall TFormRegioesVTCD::lvRegioesCompare(TObject *Sender, TListItem *Item1,
		  TListItem *Item2, int Data, int &Compare)
   {
	// variáveis locais
	int index = listViewSort.ColumnIndex - 1;
	double d1, d2;
	AnsiString aux1, aux2;

	switch (listViewSort.ColumnIndex)
	{
	case 0: // Rede

		aux1 = Item1->Caption + "-" + Item1->SubItems->Strings[0];
		aux2 = Item2->Caption + "-" + Item2->SubItems->Strings[0];
		Compare = CompareText(aux1, aux2);
		break;
	case 1:

		aux1 = Item1->SubItems->Strings[0];
		aux2 = Item2->SubItems->Strings[0];
		Compare = CompareText(aux1, aux2);
	default:
		break;
	}
	// verifica se ordem crescente/decrescente
	if (!listViewSort.OrdemCrescente)
		Compare = -Compare;
   }
//---------------------------------------------------------------------------


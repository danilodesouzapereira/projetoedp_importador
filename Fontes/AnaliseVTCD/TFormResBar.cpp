//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormResBar.h"
#include "VTBarraVTCD.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTEqbar.h>
#include <PlataformaSinap\Fontes\RedeCC\ResCurto.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormResBar *FormResBar;
//---------------------------------------------------------------------------
__fastcall TFormResBar::TFormResBar(TComponent* Owner, VTApl *apl, VTBarraVTCD *pbarV, AnsiString codAlt)
	: TForm(Owner)
	{
	this->apl = apl;
	this->pbarV = pbarV;
	this->codAlt= codAlt;
	IniciaLView();
	}
//---------------------------------------------------------------------------
__fastcall TFormResBar::~TFormResBar(void)
	{
	//variáveis locais
	}

//---------------------------------------------------------------------------
void __fastcall TFormResBar::IniciaLView(void)
	{
	TListItem    *item;
	AnsiString   titulo;
	double v00;
	VTEqbar *eqbar;
	TList   *lisEqBar;

	lisEqBar = pbarV->Barra->LisEqbar();
	eqbar = NULL;
	for(int n = 0; n < lisEqBar->Count; n++)
		{
		eqbar = (VTEqbar*)lisEqBar->Items[n];
		if(eqbar->Tipo() == eqptoCARGA) break;
		eqbar = NULL;
		}

	if(eqbar == NULL)
		titulo.sprintf("%s: Barra: %i - %s", codAlt, pbarV->Barra->Id, pbarV->Barra->Codigo);
	else
		titulo.sprintf("%s: Carga: %i - %s", codAlt, eqbar->Id, eqbar->Codigo);

	this->Caption = titulo;

	LViewResBar->Clear();
	item = LViewResBar->Items->Add();
	item->Caption = "TOTAL";
	v00 = pbarV->Naf[NUM_MAX_DEF][0.1];
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[NUM_MAX_DEF][0.9]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[NUM_MAX_DEF][0.7]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[NUM_MAX_DEF][0.4]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", v00));

	item = LViewResBar->Items->Add();
	item->Caption = "3F";
	v00 = pbarV->Naf[defeito3F][0.1];
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeito3F][0.9]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeito3F][0.7]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeito3F][0.4]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", v00));

	item = LViewResBar->Items->Add();
	item->Caption = "2F";
	v00 = pbarV->Naf[defeito2F][0.1];
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeito2F][0.9]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeito2F][0.7]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeito2F][0.4]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", v00));

	item = LViewResBar->Items->Add();
	item->Caption = "FT";
	v00 = pbarV->Naf[defeitoFT][0.1];
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeitoFT][0.9]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeitoFT][0.7]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeitoFT][0.4]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", v00));

	item = LViewResBar->Items->Add();
	item->Caption = "FTZ";
	v00 = pbarV->Naf[defeitoFTZ][0.1];
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeitoFTZ][0.9]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeitoFTZ][0.7]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeitoFTZ][0.4]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", v00));

	item = LViewResBar->Items->Add();
	item->Caption = "2FT";
	v00 = pbarV->Naf[defeito2FT][0.1];
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeito2FT][0.9]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeito2FT][0.7]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[defeito2FT][0.4]-v00));
	item->SubItems->Add(DoubleToStr("%10.3f", v00));

	LViewResBar->ItemIndex = 0;
	ReiniciaGraf(0, "Total de curtos");

	FormActivate(NULL);
	}

//---------------------------------------------------------------------------
void __fastcall TFormResBar::FormClose(TObject *Sender, TCloseAction &Action)
	{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	grafico->Moldura();
	Action = caFree;
	}
//---------------------------------------------------------------------------
void __fastcall TFormResBar::FormActivate(TObject *Sender)
	{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	grafico->Moldura();
	grafico->Moldura(pbarV->Barra);
	}
//---------------------------------------------------------------------------
void __fastcall TFormResBar::LViewResBarSelectItem(TObject *Sender,
		TListItem *Item, bool Selected)
	{
	int idef;
	AnsiString titulo;

	switch (Item->Index) {
				  case 0: idef = NUM_MAX_DEF; titulo = "Total de curtos"; break;
				  case 1: idef = defeito3F; titulo = "curtos 3F" ; break;
				  case 2: idef = defeito2F; titulo = "curtos 2F" ; break;
				  case 3: idef = defeitoFT; titulo = "curtos FT" ; break;
				  case 4: idef = defeitoFTZ; titulo = "curtos FTZ" ; break;
				  case 5: idef = defeito2FT; titulo = "curtos 2FT" ; break;
					}

	ReiniciaGraf(idef, titulo);
	}
//---------------------------------------------------------------------------
void __fastcall TFormResBar::ReiniciaGraf(int idef, AnsiString tit)
	{
	TChartSeries *serie;
	double v00;

	serie = chtAfunda->Series[0];
	serie->Clear();
	chtAfunda->Title->Caption = "Eventos/ano - " + tit;

	v00 = pbarV->Naf[idef][0.1];
	serie->Add(pbarV->Naf[idef][0.9]-v00, "SAIFI90", clRed);
	serie->Add(pbarV->Naf[idef][0.7]-v00, "SAIFI70", clGreen);
	serie->Add(pbarV->Naf[idef][0.4]-v00, "SAIFI40", clYellow);
	serie->Add(v00, "Interr.", clBlue);




	}
//---------------------------------------------------------------------------


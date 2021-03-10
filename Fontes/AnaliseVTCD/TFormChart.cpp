//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TformChart.h"
#include "VTHist.h"
#include <PlataformaSinap\DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformChart *formChart;
//---------------------------------------------------------------------------
__fastcall TformChart::TformChart(TComponent* Owner)
        : TForm(Owner)
	{
	Top = 0;
	Left = 0;
	tChart->Canvas->Pen->Width = 2;
	}
//---------------------------------------------------------------------------
void __fastcall TformChart::AddLinhaVertical(double X, AnsiString nome, TColor color, TPenStyle pst)
   {
   strPoint* strPonto;

   strPonto = new strPoint;
	strPonto->X = X;
   strPonto->serie = nome;

   linhaVert->Add(strPonto);

   tChart->Refresh();
   //if(color = "")
   //   DrawLinhaVertical(serie->color);
   //else
      DrawLinhaVertical(color, pst);
   }
//---------------------------------------------------------------------------
void __fastcall TformChart::AddSerie(VTHist* hist, AnsiString titulo,  AnsiString nome, int tipo, bool diferenca, bool f_abs, double f_mult)
   {
	TChartSeries *serie;
	double last, valor;
   int ninterv = hist->NintervHist();
	double passo;// = hist->PassoHist();
	double *lista;// = hist->Hist();
	int noc;// = hist->Noc();
   double vaux;

	serie = NULL;
	serie = FindSeries (nome);

   //Cria Serie se nao existir
   if (serie==NULL)
      {
      switch(tipo)
         {
         case 1:
           serie = new TLineSeries(this);
           break;
         case 2:
           serie = new TLineSeries(this);
           ((TCustomSeries*)serie)->Pointer->Visible = true;
           break;
         default:
           serie = new TBarSeries(this);
           serie->Marks->Visible = false;
           break;
         }


      serie->ParentChart=tChart;
      serie->ShowInLegend = true;
      if(titulo == "") serie->ShowInLegend = false;
      tChart->Legend->Alignment = laBottom;

      serie->Active = true;
      serie->Name = nome;
      serie->AllowSinglePoint = true;
      serie->Title = titulo;
      }

   //limpa pontos existentes
   serie->Clear();

  //exibe ou nao os pontos
  if (mostraPontos)
   {
   //ignora o erro se o tipo de serie nao suportar pontos (p.ex. barras)
//   try {((TCustomSeries*)serie)->Pointer->Visible = true;} catch(...){};
   }

   //Adiciona pontos do histograma na serie
   vaux = 0.;
   last = 0;
   for(int i = 0; i < ninterv; i++)
      {
      valor = lista[i];
      vaux += (valor - vaux);

      if((!f_abs)&&(noc > 0))
         valor /= (double)noc; //normaliza o valor


		serie->AddXY(((i+1) * passo),(valor - last)*f_mult);
      if (diferenca) last = valor;

      if(vaux ==  noc) break;
      }

   }
//---------------------------------------------------------------------------
void __fastcall TformChart::AddSerieA(VTHist* hist, double passo, AnsiString titulo,
                                      AnsiString nome, int tipo, bool diferenca, bool f_abs, double f_mult)
   {
	TChartSeries *serie;
   TList        *lisInterv;
   Interv       *pinterv;
	//double       passo = hist->PassoHist();

	int          noc = hist->NPtos;//hist->LisPtos->Count;// = hist->Noc();



	serie = NULL;
	serie = FindSeries(nome);

	//Cria Serie se nao existir
	if (serie==NULL)
		{
		switch(tipo)
			{
			case 1:
			  serie = new TLineSeries(this);
			  break;
			case 2:
			  serie = new TLineSeries(this);
			  ((TCustomSeries*)serie)->Pointer->Visible = true;
			  break;
			default:
			  serie = new TBarSeries(this);
			  serie->Marks->Visible = false;
			  ((TBarSeries*)serie)->CustomBarWidth = 10;
			  break;
			}


		serie->ParentChart=tChart;
		serie->ShowInLegend = true;
		if(titulo == "") serie->ShowInLegend = false;
		tChart->Legend->Alignment = laBottom;

		serie->Active = true;
		serie->Name = nome;
		serie->AllowSinglePoint = true;
		serie->Title = titulo;
		}

	//limpa pontos existentes
	serie->Clear();

	//Gera lista de intervalos
	lisInterv = hist->GeraInterv(passo);
	//passo = hist->p;
	//Adiciona pontos do histograma na serie
	for(int i = 0; i < lisInterv->Count; i++)
		{
		pinterv = (Interv*)lisInterv->Items[i];
		serie->AddXY(pinterv->ni * passo + passo/2, (double)pinterv->n / (double)noc);
		}
	}
//---------------------------------------------------------------------------
void __fastcall TformChart::AddSerieB(VTHist* hist, AnsiString titulo,  AnsiString nome, int tipo, bool diferenca, bool f_abs, double f_mult)
	{
	TChartSeries *serie;
	//TList        *lisPontos = hist->LisPtos;// = hist->LisPontos();
	Ponto        *ponto;
	int           noc = hist->LisPtos->Count;// = hist->Noc();
   int          nacum;
   //double       pant;
	double       aux;

   serie = NULL;
   serie = FindSeries (nome);

   //Cria Serie se nao existir
   if (serie==NULL)
      {
      switch(tipo)
         {
         case 1:
           serie = new TLineSeries(this);
           break;
         case 2:
           serie = new TLineSeries(this);
           ((TCustomSeries*)serie)->Pointer->Visible = true;
           break;
         default:
			  serie = new TBarSeries(this);
			  serie->Marks->Visible = false;
			  break;
         }


      serie->ParentChart=tChart;
      serie->ShowInLegend = true;
      if(titulo == "") serie->ShowInLegend = false;
      tChart->Legend->Alignment = laBottom;

      serie->Active = true;
      serie->Name = nome;
      serie->AllowSinglePoint = true;
		serie->Title = titulo;
		}

	//limpa pontos existentes
	serie->Clear();

	//Adiciona pontos do histograma na serie
	nacum = 0;
	//pant  = -1.;

	for(int i = 0; i < hist->LisPtos->Count; i++)
		{
		ponto = (Ponto*)hist->LisPtos->Items[i];
		aux = (double)ponto->ifreqAc / (double)hist->NPtos;
		serie->AddXY(ponto->pnovo, aux);
		}
	}
//---------------------------------------------------------------------------
void __fastcall TformChart::AddSerie(int ninterv, double passo, double *lista, AnsiString titulo,  AnsiString nome, int noc, int tipo, bool diferenca)
	{

	TChartSeries *serie;
	double last, valor;

	serie = NULL;
	serie = FindSeries (nome);

	//Cria Serie se nao existir
	if (serie==NULL)
      {
      switch(tipo)
         {
         case 1:
           serie = new TLineSeries(this);
           break;
         case 2:
           serie = new TLineSeries(this);
           ((TCustomSeries*)serie)->Pointer->Visible = true;
           break;
         default:
           serie = new TBarSeries(this);
           serie->Marks->Visible = false;
           break;
         }


      serie->ParentChart=tChart;
      serie->ShowInLegend = true;
      if(titulo == "") serie->ShowInLegend = false;

      serie->Active = true;
      serie->Name = nome;
      serie->AllowSinglePoint = true;
      serie->Title = titulo;
      }

   //limpa pontos existentes
   serie->Clear();

   //Adiciona pontos do histograma na serie
   last = 0;
   for(int i = 0; i < ninterv; i++)
      {
      valor = lista[i]/(double)noc; //normaliza o valor
      serie->AddXY(((i+1) * passo),(valor - last));
      if (diferenca) last = valor;
      }

   }
//---------------------------------------------------------------------------
void __fastcall TformChart::AddPontos(double X, double Y, AnsiString titulo, AnsiString nome)
	{
	TChartSeries *serie;
	double last, valor;


	serie = NULL;
	serie = FindSeries (nome);

	//Cria Serie se nao existir
	if (serie==NULL)
		{
		serie = new TLineSeries(this);
		serie->ParentChart=tChart;
		serie->ShowInLegend = true;
      if(titulo == "") serie->ShowInLegend = false;
      serie->Active = false;
      serie->Name = nome;
      serie->AllowSinglePoint = true;
      serie->Title = titulo;
      }

   ((TCustomSeries*)serie)->Pointer->Visible = false;
   serie->AddXY(X,Y);

   }
//---------------------------------------------------------------------------
void __fastcall TformChart::AddPontosBar(double X, double Y, AnsiString titulo, AnsiString nome)
   {
   TChartSeries *serie;
   double last, valor;


   serie = NULL;
   serie = FindSeries (nome);

   //Cria Serie se nao existir
   if (serie==NULL)
      {
      serie = new TBarSeries(this);
      serie->ParentChart=tChart;
      serie->ShowInLegend = true;
      if(titulo == "") serie->ShowInLegend = false;
      serie->Active = false;
      serie->Name = nome;
      serie->AllowSinglePoint = true;
      serie->Title = titulo;
      serie->Marks->Visible = false;
      }

   serie->AddXY(X,Y);

   }
//---------------------------------------------------------------------------
TChart* __fastcall TformChart::Chart()
   {
   return tChart;
   }
//---------------------------------------------------------------------------
void __fastcall TformChart::DefineZoom(double valLeft, double valRight, AnsiString nome)
   {
   TRect rect, chartRect;
   TChartSeries* serie;
   double maxX, minX;

   serie = NULL;

   //Varre lista de series e verifica se a serie já existe
   serie = FindSeries (nome);

   //nao executa se a serie nao existe
   if (serie == NULL) return;

   tChart->LeftAxis->Automatic = true;
   tChart->BottomAxis->Automatic = true;

   tChart->BottomAxis->AutomaticMaximum = true;
   tChart->BottomAxis->AutomaticMinimum = true;


   chartRect = tChart->ChartRect;

   maxX = tChart->BottomAxis->CalcPosValue(100);
   minX = tChart->BottomAxis->CalcPosValue(0);

   rect.Left   = (valLeft/(maxX-minX))*100;
   if (rect.Left < 0) rect.Left =0;

   rect.Right  = (valRight/(maxX-minX))*100;
   if (rect.Right > 100) rect.Right = 100;

   rect.Top = 100;
   rect.Bottom = 0;

   tChart->ZoomRect(rect);

   tChart->LeftAxis->Automatic = true;

   }
//---------------------------------------------------------------------------
void __fastcall TformChart::DefineZoom(double valLeft, double valTop, double valRight, double valBottom)
   {
   TRect rect;

   rect.Left   = valLeft;
   rect.Top    = valTop;
   rect.Right  = valRight;
   rect.Bottom = valBottom;

   tChart->ZoomRect(rect);
   }
//---------------------------------------------------------------------------
void __fastcall TformChart::DelSerie(AnsiString nome)
   {
   TChartSeries* serie;


   if(nome != "")
     {
     serie = FindSeries(nome);
     if (serie == NULL) return;
     tChart->RemoveSeries(serie);
     delete serie;
     }
   else
     {
     for(int num = tChart->SeriesList->Count -1; num >= 0; num--)
      {
		serie = tChart->Series[num];   //SeriesList->
		tChart->RemoveSeries(serie);
      delete serie;
      }
     }

   }
//---------------------------------------------------------------------------
void __fastcall TformChart::DrawLinhaVertical(TColor color, TPenStyle pst)
   {
   TChartSeries* serie;
   strPoint* ponto;
   int posX;

   for (int num = 0; num < linhaVert->Count; num++)
      {

      ponto = (strPoint*) linhaVert->Items[num];
      serie = FindSeries (ponto->serie);

      if (serie == NULL) continue;

      posX = serie->CalcXPosValue(ponto->X);

      if (posX < tChart->ChartRect.Left || posX > tChart->ChartRect.Right) continue;

      tChart->Canvas->Pen->Width = 2;
      tChart->Canvas->Pen->Color = serie->SeriesColor;
      if(!(color == clNone)) tChart->Canvas->Pen->Color = color;

      if(!(pst == psSolid)) tChart->Canvas->Pen->Style = pst;


      tChart->Canvas->Line(posX, tChart->ChartRect.Top + 1 , posX, tChart->ChartRect.Bottom - 1);
      }
   }
//---------------------------------------------------------------------------
void __fastcall TformChart::DrawlisPontos()
   {
   TChartSeries* serie;
   strPoint* ponto;
   int posX, posY;

   for (int num = 0; num < lisPontos->Count; num++)
      {
      ponto = (strPoint*) lisPontos->Items[num];
      serie = FindSeries (ponto->serie);

      if (serie == NULL) continue;

      posX = serie->CalcXPosValue(ponto->X);
      posY = serie->CalcYPosValue(ponto->Y);

      if (posX < tChart->ChartRect.Left || posX > tChart->ChartRect.Right) continue;
      if (posY < tChart->ChartRect.Top || posY > tChart->ChartRect.Bottom) continue;

      tChart->Canvas->Pen->Width = 2;
      tChart->Canvas->Pen->Color = serie->SeriesColor;

      tChart->Canvas->FillRect(TRect(posX-3 ,posY-3,posX+3 ,posY+3));
      }

   }
//---------------------------------------------------------------------------
void __fastcall TformChart::EixoMinMax(double aMin, double aMax)
   {
   // Add horizontal Axis
	tChart->BottomAxis->SetMinMax(aMin, aMax);
	tChart->BottomAxis->Increment = 0.1;
   }
//---------------------------------------------------------------------------
TChartSeries* __fastcall TformChart::FindSeries(AnsiString nome)
   {
   TChartSeries* serie;

   serie = NULL;

   //Varre lista de series e verifica se a serie já existe
   for (int num = 0; num < tChart->SeriesList->Count; num ++)
      {
      serie = tChart->Series[num];    //SeriesList->
      if (serie->Name == nome)
         break;
      else
         serie = NULL;
      }

   return serie;

   }
//---------------------------------------------------------------------------
void __fastcall TformChart::FormCreate(TObject *Sender)
   {
   linhaVert = new TList;
   lisPontos = new TList;
   }
//---------------------------------------------------------------------------
void __fastcall TformChart::FormDestroy(TObject *Sender)
   {
   LimpaLinhaVertical();
   LimpalisPontos();
   }
//---------------------------------------------------------------------------
void __fastcall TformChart::FormResize(TObject *Sender)
   {
   Top = 0;
   Left = 0;

   tChart->Width = ClientWidth;
   tChart->Height = ClientHeight;

   Width = this->ClientWidth;
   Height = this->ClientHeight;

   }
//---------------------------------------------------------------------------
void __fastcall TformChart::LimpaLinhaVertical()
   {
   strPoint* ponto;

   for (int num = 0; num < linhaVert->Count; num++)
      {
      ponto = (strPoint*) linhaVert->Items[num];
      delete ponto;
      }

   linhaVert->Clear();

   tChart->Refresh();
   }
//---------------------------------------------------------------------------
void __fastcall TformChart::LimpalisPontos()
   {
   strPoint* ponto;

   for (int num = 0; num < lisPontos->Count; num++)
      {
      ponto = (strPoint*) lisPontos->Items[num];
      delete ponto;
      }

   lisPontos->Clear();

   tChart->Refresh();
   }
//---------------------------------------------------------------------------
void __fastcall TformChart::MostraPontos(bool mostra)
   {
   mostraPontos = mostra;
   }
//---------------------------------------------------------------------------
void __fastcall TformChart::MostraSerie(AnsiString nome, bool flag)
   {
   TChartSeries *serie;

   serie = NULL;
   serie = FindSeries(nome);

   if(!serie) return;
   else
      serie->Active = flag;
   }
//---------------------------------------------------------------------------
void __fastcall TformChart::RotuloX(AnsiString rotulox)
   {
   tChart->BottomAxis->Title->Caption = rotulox;
   }
//---------------------------------------------------------------------------
void __fastcall TformChart::RotuloY(AnsiString rotuloy)
   {
   tChart->LeftAxis->Title->Caption = rotuloy;
   }
//---------------------------------------------------------------------------
void __fastcall TformChart::Titulo(AnsiString titulo)
   {
   Caption = titulo;
   tChart->Title->Text->Clear();
   tChart->Title->Text->Add(titulo);
   }
//---------------------------------------------------------------------------

void __fastcall TformChart::tChartAfterDraw(TObject *Sender)
   {
   DrawlisPontos();
   DrawLinhaVertical();
   }
//---------------------------------------------------------------------------

void __fastcall TformChart::tChartResize(TObject *Sender)
   {
   DrawlisPontos();
   DrawLinhaVertical();
   }
//---------------------------------------------------------------------------

void __fastcall TformChart::tChartScroll(TObject *Sender)
   {
   DrawlisPontos();
   DrawLinhaVertical();
   }
//---------------------------------------------------------------------------

void __fastcall TformChart::tChartBeforeDrawSeries(TObject *Sender)
   {
   DrawlisPontos();
   DrawLinhaVertical();
   }
//---------------------------------------------------------------------------


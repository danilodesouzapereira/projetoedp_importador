//---------------------------------------------------------------------------

#ifndef TformChartH
#define TformChartH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
//---------------------------------------------------------------------------
class VTDados;
class VTHist;
//---------------------------------------------------------------------------
class TformChart : public TForm
{
__published:	// IDE-managed Components
   TChart *tChart;
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall tChartAfterDraw(TObject *Sender);
   void __fastcall tChartResize(TObject *Sender);
   void __fastcall tChartScroll(TObject *Sender);
   void __fastcall tChartBeforeDrawSeries(TObject *Sender);

private:	// User declarations
   bool mostraPontos;

public:		// User declarations
        __fastcall TformChart(TComponent* Owner);
   void __fastcall AddLinhaVertical(double X, AnsiString nome, TColor color = clNone, TPenStyle pst = psSolid);
   void __fastcall AddPontos(double X, double Y, AnsiString titulo, AnsiString nome);
   void __fastcall AddPontosBar(double X, double Y, AnsiString titulo, AnsiString nome);
   void __fastcall AddSerie(VTHist* hist, AnsiString titulo, AnsiString nome,
                            int tipo = 1, bool diferenca = true, bool f_abs = true, double f_mult = 1.0);
	void __fastcall AddSerieA(VTHist* hist, double passo, AnsiString titulo, AnsiString nome,
                            int tipo = 3, bool diferenca = true, bool f_abs = true, double f_mult = 1.0);
	void __fastcall AddSerieB(VTHist* hist, AnsiString titulo, AnsiString nome,
									 int tipo = 1, bool diferenca = true, bool f_abs = true, double f_mult = 1.0);
	void __fastcall AddSerie(int ninterv, double passo, double *lista, AnsiString titulo,
									 AnsiString nome = "Serie0", int noc =1, int tipo = 1, bool diferenca = true);
TChart* __fastcall Chart();
	void __fastcall DelSerie(AnsiString nome) ;
	void __fastcall DefineZoom(double valLeft, double valTop, double valRight, double valBottom);
   void __fastcall DefineZoom(double valLeft, double valTop, AnsiString nome);
   void __fastcall EixoMinMax(double aMin, double aMax);
   void __fastcall MostraPontos(bool mostra);
   void __fastcall MostraSerie(AnsiString nome, bool flag);
   void __fastcall LimpaLinhaVertical();
   void __fastcall LimpalisPontos();
   void __fastcall RotuloX(AnsiString rotulox);
   void __fastcall RotuloY(AnsiString rotuloy);
   void __fastcall Titulo(AnsiString titulo);

private:
   TChartSeries* __fastcall FindSeries(AnsiString nome);
            void __fastcall DrawLinhaVertical(TColor color = clNone, TPenStyle pst = psSolid);
            void __fastcall DrawlisPontos();

   TList* linhaVert;
	TList* lisPontos;

   struct strPoint
      {
      AnsiString serie;
      double X;
      double Y;
      };

};
//---------------------------------------------------------------------------
extern PACKAGE TformChart *formChart;
//---------------------------------------------------------------------------
#endif

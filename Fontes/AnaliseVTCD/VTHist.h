//---------------------------------------------------------------------------
#ifndef VTHistH
#define VTHistH
//---------------------------------------------------------------------------
#include <stdio.h>
//---------------------------------------------------------------------------
struct Ponto : public TObject
  {
  double pnovo;
  double px, py;
  //int    ifreq, ifreqAc;
  double ifreq, ifreqAc;
  //double fpond;
  };
struct Interv : public TObject
  {
  int    ni; //ordem do intervalo
  int    n;      //número de observações
  };
//---------------------------------------------------------------------------
class VTHist: public TObject
	{
	public:  //property
	  __property double      Media   = {read=PM_GetMedia};
	  __property double      DesvP   = {read=PM_GetDesvP};
	  __property double      NPtos   = {read=PM_GetNPtos};
	  __property double      VMax    = {read=PM_GetVMax};
	  __property double      VMin    = {read=PM_GetVMin};
	  __property double      VIni    = {read=PM_GetVIni, write=PM_SetVIni};
	  __property TList*      LisPtos = {read=PM_GetLisPtos};
	  __property TObject*    Obj     = {read=PM_GetObject, write=PM_SetObject};

	public:
					__fastcall VTHist(void) {};
	  virtual      __fastcall ~VTHist(void) {};
	  //virtual void    __fastcall AtualizaHist(double pnovo) = 0;
	  virtual double  __fastcall FuncaoInversaProb(double valor) = 0;
	  virtual double  __fastcall FuncaoProb(double d) = 0;
	  virtual void    __fastcall PontoNovo(double pnovo, double ifreq = 1) = 0;
	  virtual void    __fastcall ImprimeHistograma(FILE *fout) = 0;
	  virtual void    __fastcall Inicializa(void) = 0;
	  virtual void    __fastcall AdicionaInterv(double pnovo, double step) = 0;
	  virtual TList*  __fastcall GeraInterv(double step) = 0;
	  virtual TList*  __fastcall LisInterv(void) = 0;
	  virtual int     __fastcall NintervHist(void) = 0;

	protected:  //métodos acessados via property
		virtual double   __fastcall PM_GetMedia(void)   = 0;
		virtual TObject* __fastcall PM_GetObject(void)  = 0;
		virtual double   __fastcall PM_GetDesvP(void)   = 0;
		virtual double   __fastcall PM_GetNPtos(void)   = 0;
		virtual TList*   __fastcall PM_GetLisPtos(void) = 0;
		virtual double   __fastcall PM_GetVMax(void)    = 0;
		virtual double   __fastcall PM_GetVMin(void)    = 0;
		virtual double   __fastcall PM_GetVIni(void)    = 0;
		virtual void     __fastcall PM_SetObject(TObject* pobj) = 0;
		virtual void     __fastcall PM_SetVIni(double vIni) = 0;

	public:

	};
//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTHist
//---------------------------------------------------------------------------
VTHist* __fastcall NewObjHist(void);
//---------------------------------------------------------------------------
#endif
//eof



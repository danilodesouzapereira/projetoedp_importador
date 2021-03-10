//---------------------------------------------------------------------------
#ifndef VTBarraVTCDH
#define VTBarraVTCDH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;
//class VTBarCC;
class VTHist;
class VTLigacao;
class TRegiaoVTCD;
class VTBloco;
//---------------------------------------------------------------------------
class VTBarraVTCD : public TObject
	{
	public:  //property
	  __property VTBarra*        Barra      = {read=PM_GetBarra};
	  //__property VTBarCC*        BarCC      = {read=PM_GetBarCC, write=PM_SetBarCC};
	  __property VTBarraVTCD*    BarIni     = {read=PM_GetBarIni, write=PM_SetBarIni};
	  __property double          Dac        = {read=PM_GetDAc, write=PM_SetDAc};
	  __property VTHist*         Hist[int ndef]   = {read=PM_GetHist};
	  __property TList*          LisBar     = {read=PM_GetLisBar};
	  __property VTLigacao*      Ligacao    = {read=PM_GetLiga, write=PM_SetLiga};
	  __property double          Nc         = {read=PM_GetNCurto, write=PM_SetNCurto};
	  __property double          Na         = {read=PM_GetNAmCurto, write=PM_SetNAmCurto};
	  __property double          Naf[int ndef][double vaf] = {read=PM_GetNAf};
	  __property double          NafP[int ndef][double vaf] = {read=PM_GetNAfP};
	  __property bool            FlagExecuta[int iCurto] = {read=PM_GetFlagExecuta, write=PM_SetFlagExecuta};

	public:
										__fastcall VTBarraVTCD(void) {};
		virtual                 __fastcall ~VTBarraVTCD(void) {};

	protected:  //métodos acessados via property
		virtual double        __fastcall PM_GetNAf(int ndef, double vaf) = 0;
		virtual double        __fastcall PM_GetNAfP(int ndef, double vaf) = 0;
		virtual VTBarra*      __fastcall PM_GetBarra(void) = 0;
//		virtual VTBarCC*      __fastcall PM_GetBarCC(void) = 0;
//		virtual void          __fastcall PM_SetBarCC(VTBarCC* pbarCC) = 0;
		virtual VTBarraVTCD*  __fastcall PM_GetBarIni(void) = 0;
		virtual void          __fastcall PM_SetBarIni(VTBarraVTCD *pbarIni) = 0;
		virtual double        __fastcall PM_GetDAc(void) = 0;
		virtual void          __fastcall PM_SetDAc(double dac) = 0;
		virtual VTHist*       __fastcall PM_GetHist(int ndef) = 0;
		virtual TList*        __fastcall PM_GetLisBar(void) = 0;
		virtual VTLigacao*    __fastcall PM_GetLiga(void) = 0;
		virtual void          __fastcall PM_SetLiga(VTLigacao *pliga) = 0;
		virtual double        __fastcall PM_GetNCurto(void)= 0;
		virtual void          __fastcall PM_SetNCurto(double nc) = 0;
		virtual double        __fastcall PM_GetNAmCurto(void) = 0;
		virtual void          __fastcall PM_SetNAmCurto(double namc) = 0;
		virtual bool          __fastcall PM_GetFlagExecuta(int iCurto) = 0;
		virtual void          __fastcall PM_SetFlagExecuta(int iCurto, bool valor) = 0;

	public:
		double dacAux;
		double ntotDef;
		//Para uso do alocador
		double icc[6][3];   //corrente de curto por [tipoDef][faixa resFalta]
		TList *lisResVTCD;
		TRegiaoVTCD *regiaoVTCD;
        VTBloco    *bloco;
	};

//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
VTBarraVTCD* NewObjBarraVTCD(VTBarra* pbarra);

//---------------------------------------------------------------------------
#endif
//eof

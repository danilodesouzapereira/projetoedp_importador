//---------------------------------------------------------------------------
#ifndef TBarraVTCDH
#define TBarraVTCDH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBarraVTCD.h"
#include <PlataformaSinap\Fontes\RedeCC\ResCurto.h>
//---------------------------------------------------------------------------
class TBarraVTCD : public VTBarraVTCD
	{
	public:
			  __fastcall TBarraVTCD(VTBarra* pbarra);
			  __fastcall ~TBarraVTCD(void);
			

	private://métodos
			 void  __fastcall IniciaLisHist(void);

	protected:  //métodos acessados via property
		double        __fastcall PM_GetNAf(int ndef, double vaf);
		double        __fastcall PM_GetNAfP(int ndef, double vaf);
		VTBarra*      __fastcall PM_GetBarra(void);
		//VTBarCC*      __fastcall PM_GetBarCC(void);
		//void          __fastcall PM_SetBarCC(VTBarCC* pbarCC);
		VTBarraVTCD*  __fastcall PM_GetBarIni(void);
		void          __fastcall PM_SetBarIni(VTBarraVTCD *pbarIni);
		double        __fastcall PM_GetDAc(void);
		void          __fastcall PM_SetDAc(double dac);
		VTHist*       __fastcall PM_GetHist(int ndef);
		TList*        __fastcall PM_GetLisBar(void);
		VTLigacao*    __fastcall PM_GetLiga(void);
		void          __fastcall PM_SetLiga(VTLigacao *pliga);
		double        __fastcall PM_GetNCurto(void);
		void          __fastcall PM_SetNCurto(double nc);
		double        __fastcall PM_GetNAmCurto(void);
		void          __fastcall PM_SetNAmCurto(double namc);
		bool          __fastcall PM_GetFlagExecuta(int iCurto) {return(flagExecuta[iCurto]);};
		void          __fastcall PM_SetFlagExecuta(int iCurto, bool valor) {flagExecuta[iCurto] = valor;};

	private://Métodos

	private://dados
		double    nc, namc;
		double    dac;
		VTHist    *hist;
		VTBarra   *pbarra;
		//VTBarCC   *pbarCC;
		VTBarraVTCD *pbarIni;
		VTLigacao   *pliga;
		TList       *lisBar;
		TList       *lisHist;

		TList       *lisChavesProt; //hk2019
		bool        flagExecuta[NUM_MAX_DEF];


	public:

	};
//---------------------------------------------------------------------------
#endif

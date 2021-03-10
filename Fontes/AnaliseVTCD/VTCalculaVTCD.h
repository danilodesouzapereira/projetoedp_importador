//---------------------------------------------------------------------------
#ifndef VTCalculaVTCDH
#define VTCalculaVTCDH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTCarga;
class VTBarraVTCD;
class VTProgresso;
class VTRede;
class VTRedes;
class TRegiaoVTCD;
class TUcsv;
class VTParamConf;
//---------------------------------------------------------------------------
//struct str_UCSV : TObject
//		 {
//		 AnsiString  descricao;
//		 VTCarga     *carga;
//		 TRegiaoVTCD *regiaoVTCD;
//		 };
//---------------------------------------------------------------------------
struct str_ResUCSV : TObject
		 {
		 TUcsv       *ucsv;
		 double      vtcd[6][3];
		 bool        flagZonaReligamento;
		 bool        flagJusanteDefeito;

		 };
//---------------------------------------------------------------------------
struct DAT_REDE : TObject
		 {
		 VTRede      *rede;
		 double      taxa;
		 double      comp;//comprimento em km
		 };
//---------------------------------------------------------------------------
class VTCalculaVTCD : public TObject
	{
	public:  //property
	  __property int             TipoDef                   = {read=PM_GetTipoDef, write=PM_SetTipoDef};
	  __property int             Pat                       = {read=PM_GetPat, write=PM_SetPat};
	  __property TList*          LisBarraVTCD              = {read=PM_GetLisBarraVTCD};
	  __property TList*          LisDataRede               = {read=PM_GetLisDataRede};
	  __property TList*          LisPatamares              = {read=PM_GetLisPatamares};
	  __property VTBarraVTCD*    BarVTCD[VTBarra *pbarra]  = {read=PM_GetBarVTCD};
	  __property double          PDef[int tipoDef]         = {read=PM_GetPDef, write=PM_SetPDef};
	  __property double          RDef[int ifaixa][int ivalor]  = {read=PM_GetRDef, write=PM_SetRDef};
	  __property double          NivelAf                   = {read=PM_GetNivelAf, write=PM_SetNivelAf};
	  __property double          TaxaFalha[VTRede* rede]   = {read=PM_GetTaxaFalha, write=PM_SetTaxaFalha};
	  __property VTParamConf*    ParamConf                 = {read=PM_GetParamConf};
	  __property double          TaxaAmostra               = {read=PM_GetTaxaAmostra, write=PM_SetTaxaAmostra};

	public:
									 __fastcall VTCalculaVTCD(void) {};
		virtual               __fastcall ~VTCalculaVTCD(void) {};
		virtual       void    __fastcall IniciaFalhas(double distMed) = 0;
		virtual       void    __fastcall IniciaPadraoTiposCurto(void) = 0;
		virtual       void    __fastcall IniciaRedes(VTRedes * redes) = 0;
		virtual       void    __fastcall ExecutaCurtos(VTProgresso *prog) = 0;
		virtual       void    __fastcall ExecutaCurtos1(TList *lisBarVTCD, TList *lisUCSV) = 0;
		virtual       void    __fastcall IniciaObjetoCurto(void) = 0;
		virtual       void    __fastcall IniciaParamConf(VTParamConf *paramconfExt) = 0;

	protected:  //métodos acessados via property
		virtual TList*        __fastcall PM_GetLisBarraVTCD(void)= 0;
		virtual TList*        __fastcall PM_GetLisDataRede(void)= 0;
		virtual TList*        __fastcall PM_GetLisPatamares(void) = 0;
		virtual VTBarraVTCD*  __fastcall PM_GetBarVTCD(VTBarra *pbarra)= 0;
		virtual double        __fastcall PM_GetTaxaFalha(VTRede* rede) = 0;
		virtual void          __fastcall PM_SetTaxaFalha(VTRede* rede, double taxa) = 0;
		virtual int           __fastcall PM_GetTipoDef(void)= 0;
		virtual void          __fastcall PM_SetTipoDef(int ind_def) = 0;
		virtual int           __fastcall PM_GetPat(void) = 0;
		virtual void          __fastcall PM_SetPat(int ind_pat) = 0;
		virtual double        __fastcall PM_GetRDef(int ifaixa, int ivalor) = 0;
		virtual void          __fastcall PM_SetRDef(int ifaixa, int ivalor, double valor) = 0;
		virtual double        __fastcall PM_GetPDef(int tipoDef) = 0;
		virtual void          __fastcall PM_SetPDef(int tipoDef, double valor) = 0;
		virtual double        __fastcall PM_GetNivelAf(void) = 0;
		virtual void          __fastcall PM_SetNivelAf(double valor) = 0;
		virtual VTParamConf*  __fastcall PM_GetParamConf(void) = 0;
		virtual double        __fastcall PM_GetTaxaAmostra(void) = 0;
		virtual void          __fastcall PM_SetTaxaAmostra(double valor) = 0;

	public:
      double ntot;
	};

//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
VTCalculaVTCD* NewObjCalculaVTCD(VTApl* apl);

//---------------------------------------------------------------------------
#endif
//eof

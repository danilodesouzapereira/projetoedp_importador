//---------------------------------------------------------------------------
#ifndef TCalculaVTCDH
#define TCalculaVTCDH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTCalculaVTCD.h"
#include <PlataformaSinap\Fontes\RedeCC\ResCurto.h> //STR_ZDef
#include <PlataformaSinap\Fontes\Rede\Estrutura.h> //strIMP
//---------------------------------------------------------------------------
class VTApl;
class VTBarCC;
class VTCurtoCS;
class VTCurtocircuito;
class VTNet;
class VTRede;
class VTRedeCC;
class VTLigacao;

//---------------------------------------------------------------------------
class TCalculaVTCD : public VTCalculaVTCD
   {
   public:
              __fastcall TCalculaVTCD(VTApl* apl_owner);
              __fastcall ~TCalculaVTCD(void);
      void    __fastcall IniciaFalhas(double distMed);
		void    __fastcall IniciaPadraoTiposCurto(void);
		void    __fastcall IniciaRedes(VTRedes * redes);
		void    __fastcall ExecutaCurtos(VTProgresso *prog);
		void    __fastcall ExecutaCurtos1(TList *lisBarVTCD, TList *lisUCSV);
        void    __fastcall IniciaObjetoCurto(void);
		void    __fastcall IniciaParamConf(VTParamConf *paramconfExt);


   private://métodos
      //void    __fastcall AcumulaResultados(VTBarraVTCD *pbarV);
	  void    __fastcall AcumulaResultadosHP(VTBarraVTCD *pbarV, int tipoDef);
	  void    __fastcall AcumulaResultados(VTBarraVTCD *pbarV, int tipoDef,
									 TList *lisBarraVTCDAmostra, TList *lisTrechoAmostra);
	  double  __fastcall BuscaTaxa(VTRede* rede);
	  double  __fastcall BuscaTaxaParamConf(VTLigacao *pliga);
		bool  __fastcall ExecutaCurto(VTBarra *pbarra, int tipoDef);
		void  __fastcall ImprimeHistogramas(void);
		void  __fastcall ImprimeHistograma(VTBarraVTCD *pbarV, FILE *fout);
		void  __fastcall IniciaAmostra(void);
	  double  __fastcall Vfn_pu_min(VTBarCC *barCC, int ndef);
   //VTBarCC* __fastcall BuscaBarCC(VTBarra* pbarra);
	  //void  __fastcall IniciaCurtoHP(void);
	   double __fastcall CorrenteCurto(TList *lisLiga);
       double __fastcall ResVfnPuMin(VTBarra *pbarraF);

	   bool   __fastcall IniciaCurtoCS(void);

   protected:  //métodos acessados via property
      TList*        __fastcall PM_GetLisBarraVTCD(void);
      TList*        __fastcall PM_GetLisDataRede(void);
      TList*        __fastcall PM_GetLisPatamares(void);
		VTBarraVTCD*  __fastcall PM_GetBarVTCD(VTBarra *pbarra);
		double        __fastcall PM_GetTaxaFalha(VTRede* rede);
		void          __fastcall PM_SetTaxaFalha(VTRede* rede, double taxa);
		int           __fastcall PM_GetTipoDef(void);
		void          __fastcall PM_SetTipoDef(int ind_def);
      int           __fastcall PM_GetPat(void);
      void          __fastcall PM_SetPat(int ind_pat);
	  double        __fastcall PM_GetPDef(int tipoDef);
	  void          __fastcall PM_SetPDef(int tipoDef, double valor);
	  double        __fastcall PM_GetRDef(int ifaixa, int ivalor);
	  void          __fastcall PM_SetRDef(int ifaixa, int ivalor, double valor);
	  double        __fastcall PM_GetNivelAf(void);
	  void          __fastcall PM_SetNivelAf(double valor);
	  VTParamConf*  __fastcall PM_GetParamConf(void) {return(paramConf);};
	  double        __fastcall PM_GetTaxaAmostra(void) {return(tx_amostragem);};
	  void          __fastcall PM_SetTaxaAmostra(double valor) {tx_amostragem = valor;};

   private://Métodos

   private://dados
	  VTApl *apl;
	  VTCurtoCS       *curtoCS;
	  VTCurtocircuito *curtocircuito;
	  VTNet           *net;
	  VTRedeCC        *redeCC;
	  TList           *lisBarraVTCD, *lisBarRede, *lisDataRede, *lisPatamares;
	  int             ind_def;
	  int             ind_pat;
	  double          distMed;
	  double          *pDef; //vetor com proporção de defeitos por tipo
	  double          rDef[5][2]; //vetor com proporção de defeitos por resistência de falta
	  double          ind_af;// nível de afundamento
	  double          tx_def; //taxa de falhas default (falhas/km.ano)
	  double          tx_amostragem; //taxa de amostragem (metros/barra)
	  VTParamConf     *paramConf; //arquivo contendo taxas de falhas

	 double          Raiz3, INVR3;
			STR_ZDef        Zdef;               //impedâncias de defeito
			strIMP          Zdef_fase_ohm;      //impedâncias de defeito de fase
			strIMP          Zdef_terra_ohm;     //impedâncias de defeito de terra
	};
//---------------------------------------------------------------------------
#endif

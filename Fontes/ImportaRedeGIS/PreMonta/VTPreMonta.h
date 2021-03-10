//-----------------------------------------------------------------------------
#ifndef VTPreMonta_H
#define VTPreMonta_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "ModeloRede.h"

#define sttTRECHOCURTO 0x00200000

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18497403
class VTPreMonta : public TObject
   {
   public:  //property
      __property TDateTime  DataDominio         = {read=PD.date          , write=PD.date          };
      __property int        ConsA4              = {read=PD.consA4        , write=PD.consA4};
      __property AnsiString ArranjoRedePRI      = {read=PD.arranjoPRI    , write=PD.arranjoPRI    };
      __property AnsiString ArranjoRedeSEC      = {read=PD.arranjoSEC    , write=PD.arranjoSEC    };
      __property int        MesExtracao         = {read=PD.mesExtracao   , write=PD.mesExtracao};

   public:
						 __fastcall  VTPreMonta(void) {};
      virtual      __fastcall ~VTPreMonta(void) {};
		virtual bool __fastcall  InsereCfg_Arranjo(sdi_ARRANJO &sdi)=0;
		//virtual bool __fastcall  InsereCfg_Cabo(sdi_CABO &sdi)=0;
		virtual bool __fastcall  InsereCfg_Suporte(sdi_SUPORTE &sdi)=0;
		//etd
		virtual bool __fastcall  InsereEtd_Barra(sdi_BARRAMENTO &sdi)=0;
		virtual bool __fastcall  InsereEtd_Rede(sdi_SE &sdi)=0;
        virtual bool __fastcall  InsereEtd_Socorro(sdi_SOCORRO &sdi)=0;
		virtual bool __fastcall  InsereEtd_Trafo(sdi_TRAFO_SE &sdi)=0;
		virtual void __fastcall CompletaSuprimentoCircuito(sdi_SUPRIMENTO &sdi)=0;
		//pri
		virtual bool __fastcall  InserePri_Barra(sdi_BARRA &sdi)=0;
		virtual bool __fastcall  InserePri_Capacitor(sdi_CAPACITOR &sdi)=0;
		virtual bool __fastcall  InserePri_Chave(sdi_CHAVE &sdi)=0;
		virtual bool __fastcall  InserePri_Consumidor(sdi_CONS_MT &sdi)=0;
		virtual bool __fastcall  InserePri_Gerador(sdi_GERADOR &sdi)=0;
		virtual bool __fastcall  InserePri_Link(sdi_LINK &sdi)=0;
		virtual bool __fastcall  InserePri_Rede(sdi_CIRCUITO &sdi)=0;
		virtual bool __fastcall  InserePri_Regulador(sdi_REGULADOR &sdi)=0;
		virtual bool __fastcall  InserePri_Socorro(sdi_SOCORRO &sdi)=0;
		virtual bool __fastcall  InserePri_Trafo(sdi_TRAFO_LINHA &sdi)=0;
		virtual bool __fastcall  InserePri_Trecho(sdi_TRECHO &sdi)=0;
		//sec
		virtual bool __fastcall  InsereSec_Barra(sdi_BARRA_BT &sdi)=0;
		virtual bool __fastcall  InsereSec_Capacitor(sdi_CAPACITOR_BT &sdi)=0;
		virtual bool __fastcall  InsereSec_Consumidor(sdi_CONS_BT &sdi)=0;
		virtual bool __fastcall  InsereSec_ConsIP(sdi_CONS_IP &sdi)=0;
		virtual bool __fastcall  InsereSec_Gerador(sdi_GERADOR_BT &sdi)=0;
		virtual bool __fastcall  InsereSec_Link(sdi_LINK_BT &sdi)=0;
		virtual bool __fastcall  InsereSec_PtoCarga(sdi_PCARGA_BT &sdi)=0;
		virtual bool __fastcall  InsereSec_Rede(sdi_INSTAL_TRAFO &sdi)=0;
		virtual bool __fastcall  InsereSec_Trecho(sdi_TRECHO_BT &sdi)=0;
		virtual bool __fastcall  InsereSec_Utc(sdi_UTC_BT &sdi)=0;
		virtual bool __fastcall  Sucesso(void)=0;
		virtual bool __fastcall  TrataInicioDeDados(void)=0;
		virtual bool __fastcall  TrataFimDeDados(void)=0;

   protected:
      struct   {
               TDateTime  date;
               //DVK 2014.09.15
//               bool       consA4_energia;
               int        consA4, mesExtracao;
               AnsiString arranjoPRI;
               AnsiString arranjoSEC;
               }PD;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta1(VTApl *apl);
VTPreMonta* NewObjPreMonta2(VTApl *apl);
VTPreMonta* NewObjPreMonta3(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof

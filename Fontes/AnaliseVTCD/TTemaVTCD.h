//---------------------------------------------------------------------------
#ifndef TTemaVTCDH
#define TTemaVTCDH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\Fontes\MapaTematico\Basico\TTema.h>

//---------------------------------------------------------------------------
class VTCelula;
class VTResAlternativa;

//---------------------------------------------------------------------------
class TTemaVTCD : public TTema
   {
   public:  //m�todos
           __fastcall  TTemaVTCD(VTApl *apl);
           __fastcall ~TTemaVTCD(void);
      TColor __fastcall DegradeCinza(double valor);

   protected://m�todos a serem implementados nas classes derivadas
      void __fastcall CalculaValorProprioCelula(void);
      void __fastcall CalculaValorTotalCelula(void);
      void __fastcall DefineCorCelula(void);
      void __fastcall NormalizaValorTotalCelula(void);

   private: //m�todos locais
      double __fastcall Afundamentos(VTCelula *celula);
      //TColor __fastcall Degrade(double valor);

   public:
      VTResAlternativa *resAlt, *resAltRef;
      double vmaxExt, vminExt;
      bool   flagExt;

   private: //dados locais
      TList *lisEQP;
   };

//---------------------------------------------------------------------------
#endif
//eof

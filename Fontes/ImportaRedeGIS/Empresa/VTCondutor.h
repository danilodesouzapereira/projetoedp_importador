#ifndef VTCondutorH
#define VTCondutorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTArranjo;

//-----------------------------------------------------------------------------
class VTCondutor : public TObject
   {
   public:
		__fastcall  VTCondutor(void) {};
		__fastcall ~VTCondutor(void) {};

   public:  //objeos externos
      VTArranjo *arranjo;

   public:  //dados locais
		int          id;
		AnsiString   codigo;
		int          ativo;
		AnsiString   tipo;
		AnsiString   formacao;
		AnsiString   condutor_fase;
		AnsiString   condutor_neutro;
		double       iadm_a;
		double       r_ohm_km;
//		double       qcap_kvar_km;
		double       r0_ohm_km;
		double       x0_ohm_km;
		double       r1_ohm_km;
		double       x1_ohm_km;
		double       b0_s_km;
		double       b1_s_km;
	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTCondutores
//-----------------------------------------------------------------------------
VTCondutor* NewObjCondutor(void);

#endif
//-----------------------------------------------------------------------------
// eof

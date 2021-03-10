#ifndef VTRegionalH
#define VTRegionalH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTDadosEmpresa;

//-----------------------------------------------------------------------------
class VTRegional : public TObject
   {
	public:	//property
		__property int        Id      = {read=PD.id,      write=PD.id};
		__property AnsiString Nome    = {read=PD.nome,    write=PD.nome};
		__property VTDadosEmpresa* Empresa = {read=PD.empresa, write=PD.empresa};

	public:
		__fastcall  VTRegional(void) {};
		__fastcall ~VTRegional(void) {};

	protected:
		struct{
				int        id;
				AnsiString nome;
				VTDadosEmpresa *empresa;
				}PD;
	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTRegional
//-----------------------------------------------------------------------------
VTRegional* NewObjRegional(void);

#endif
//-----------------------------------------------------------------------------
// eof

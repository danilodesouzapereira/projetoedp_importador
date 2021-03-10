#ifndef VTLocalidadeH
#define VTLocalidadeH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTDadosEmpresa;
class VTRegional;

//-----------------------------------------------------------------------------
class VTLocalidade : public TObject
   {
	public:	//property
		__property int         Id       = {read=PD.id,       write=PD.id};
		__property AnsiString  Nome     = {read=PD.nome,     write=PD.nome};
		__property VTDadosEmpresa*  Empresa  = {read=PD.empresa,  write=PD.empresa};
		__property VTRegional* Regional = {read=PD.regional, write=PD.regional};

	public:
		__fastcall  VTLocalidade(void) {};
		__fastcall ~VTLocalidade(void) {};

	protected:
		struct{
				int        id;
				AnsiString nome;
				VTDadosEmpresa  *empresa;
				VTRegional *regional;
				}PD;
	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTLocalidade
//-----------------------------------------------------------------------------
VTLocalidade* NewObjLocalidade(void);

#endif
//-----------------------------------------------------------------------------
// eof

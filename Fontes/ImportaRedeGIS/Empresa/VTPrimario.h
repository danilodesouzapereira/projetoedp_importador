#ifndef VTPrimarioH
#define VTPrimarioH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTDadosEmpresa;
class VTRegional;
class VTLocalidade;
class VTSubestacao;

//-----------------------------------------------------------------------------
class VTPrimario : public TObject
   {
	public:	//property
		__property int           Id         = {read=PD.id,         write=PD.id        };
		__property AnsiString    Codigo     = {read=PD.codigo,     write=PD.codigo    };
		__property AnsiString    Nome       = {read=PD.nome,       write=PD.nome      };
        __property bool          Selecionado    = {read=PD.selecionado,     write=PD.selecionado     };
		__property VTDadosEmpresa*    Empresa    = {read=PD.empresa,    write=PD.empresa   };
		__property VTRegional*   Regional   = {read=PD.regional,   write=PD.regional  };
		__property VTLocalidade* Localidade = {read=PD.localidade, write=PD.localidade};
		__property VTSubestacao* Subestacao = {read=PD.subestacao, write=PD.subestacao};

	public:
		__fastcall  VTPrimario(void) {};
		__fastcall ~VTPrimario(void) {};

	protected:
		struct{
				int          id;
				AnsiString   codigo;
				AnsiString   nome;
				bool         selecionado;
				VTDadosEmpresa    *empresa;
				VTRegional   *regional;
				VTLocalidade *localidade;
				VTSubestacao *subestacao;
				}PD;
	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTPrimario
//-----------------------------------------------------------------------------
VTPrimario* NewObjPrimario(void);

#endif
//-----------------------------------------------------------------------------
// eof

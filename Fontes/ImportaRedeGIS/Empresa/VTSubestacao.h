#ifndef VTSubestacaoH
#define VTSubestacaoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTDadosEmpresa;
class VTPrimario;
class VTRegional;
class VTLocalidade;
class VTRede;
//-----------------------------------------------------------------------------
class VTSubestacao : public TObject
   {
	public:	//property
		__property int           Id         = {read=PD.id,         write=PD.id        };
		__property AnsiString    Codigo     = {read=PD.codigo,     write=PD.codigo    };
		__property AnsiString    Nome       = {read=PD.nome,       write=PD.nome      };
		__property VTDadosEmpresa*    Empresa    = {read=PD.empresa,    write=PD.empresa   };
		__property VTRegional*   Regional   = {read=PD.regional,   write=PD.regional  };
		__property VTLocalidade* Localidade = {read=PD.localidade, write=PD.localidade};
		__property TList*        LisPrimario = {read=PD.lisPRI, write=PD.lisPRI};
		__property bool          RodaFluxo =   {read=PD.rodafluxo, write=PD.rodafluxo};

	public:
		__fastcall  VTSubestacao(void) {};
		__fastcall ~VTSubestacao(void) {};
		virtual VTPrimario* __fastcall ExistePrimario(AnsiString cod_pri) = 0;

	protected:
		struct{
				int          id;
				AnsiString   codigo;
				AnsiString   nome;
				VTDadosEmpresa    *empresa;
				VTRegional   *regional;
				VTLocalidade *localidade;
				TList*       lisPRI;
				bool         rodafluxo;
				}PD;

	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTSubestacao
//-----------------------------------------------------------------------------
VTSubestacao* NewObjSubestacao(void);

#endif
//-----------------------------------------------------------------------------
// eof

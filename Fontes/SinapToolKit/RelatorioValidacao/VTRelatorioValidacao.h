#ifndef VTRelatorioValidacaoH
#define VTRelatorioValidacaoH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

// ---------------------------------------------------------------------------
class VTApl;

// -----------------------------------------------------------------------------
class VTRelatorioValidacao : public TObject
{
	public:
						__fastcall VTRelatorioValidacao(void){};
		virtual 		__fastcall ~VTRelatorioValidacao(void){};
		virtual bool 	__fastcall Executa(AnsiString diretorio);

};

// -----------------------------------------------------------------------------
// função global para criar objeto VTCondutores
// -----------------------------------------------------------------------------
VTRelatorioValidacao* NewObjRelatorioValidacao(VTApl *apl);
#endif
// -----------------------------------------------------------------------------
// eof

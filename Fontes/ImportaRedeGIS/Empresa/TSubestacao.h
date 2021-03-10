// ---------------------------------------------------------------------------
#ifndef TSubestacaoH
#define TSubestacaoH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSubestacao.h"

// ---------------------------------------------------------------------------
class TSubestacao : public VTSubestacao
{
public:
	__fastcall TSubestacao(void);
	__fastcall ~TSubestacao(void);
	VTPrimario* __fastcall ExistePrimario(AnsiString cod_pri);

private:
	TList *lisPRI;

};

#endif
// ---------------------------------------------------------------------------
// eof

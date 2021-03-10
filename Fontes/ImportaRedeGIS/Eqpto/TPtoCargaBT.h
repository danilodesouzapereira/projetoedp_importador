// ---------------------------------------------------------------------------
#ifndef TPtoCargaBTH
#define TPtoCargaBTH

// arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>

// ---------------------------------------------------------------------------
class VTBarra;

// ---------------------------------------------------------------------------
class TPtoCargaBT : public TObject
{
public:
	__fastcall TPtoCargaBT(void);
	__fastcall ~TPtoCargaBT(void);

	// dados
	int id;
	AnsiString codigo;
	VTBarra *barra;
};

// ---------------------------------------------------------------------------
// fun��o global
// ---------------------------------------------------------------------------
TPtoCargaBT* __fastcall NewObjPtoCargaBT(void);

// ---------------------------------------------------------------------------
#endif
// eof

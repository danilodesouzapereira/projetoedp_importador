// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCondutor.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// -----------------------------------------------------------------------------
VTCondutor* NewObjCondutor(void)
{
	return (new TCondutor());
}

// -----------------------------------------------------------------------------
__fastcall TCondutor::TCondutor(void)
{
	// inicia dados
	arranjo = NULL;
}

// -----------------------------------------------------------------------------
__fastcall TCondutor::~TCondutor(void)
{
	// nada a fazer
}

// -----------------------------------------------------------------------------
// eof

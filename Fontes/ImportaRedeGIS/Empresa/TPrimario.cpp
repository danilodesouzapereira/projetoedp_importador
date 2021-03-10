// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPrimario.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// -----------------------------------------------------------------------------
VTPrimario* NewObjPrimario(void)
{
	return (new TPrimario());
}

// -----------------------------------------------------------------------------
__fastcall TPrimario::TPrimario(void)
{
	// inicia variáveis
	PD.selecionado = false;//true;
	PD.empresa =NULL;
	PD.regional =NULL;
	PD.localidade =NULL;
	PD.subestacao =NULL;
}

// -----------------------------------------------------------------------------
__fastcall TPrimario::~TPrimario(void)
{
	// nada a fazer
}

// -----------------------------------------------------------------------------
// eof

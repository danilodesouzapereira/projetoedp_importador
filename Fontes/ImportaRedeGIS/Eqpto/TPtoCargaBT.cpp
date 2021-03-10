// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPtoCargaBT.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
TPtoCargaBT* __fastcall NewObjPtoCargaBT(void)
{
	try
	{
		return (new TPtoCargaBT());
	}
	catch (Exception &e)
	{
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TPtoCargaBT::TPtoCargaBT(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall TPtoCargaBT::~TPtoCargaBT(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
// eof

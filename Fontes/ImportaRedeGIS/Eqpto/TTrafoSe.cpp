// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TTrafose.h"
#include "TSe.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
TTrafose* __fastcall NewObjTrafose(void)
{
	try
	{
		return (new TTrafose());
	}
	catch (Exception &e)
	{
	}
	// erro na criação do objeto
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TTrafose::TTrafose(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall TTrafose::~TTrafose(void)
{
	// nada a fazer
}
/*
 //---------------------------------------------------------------------------
 AnsiString __fastcall TTrafose::CodigoBarraAT(void)
 {
 //assume que existe uma única barra AT
 return(se->cod + ".AT");
 }
 */

// ---------------------------------------------------------------------------
AnsiString __fastcall TTrafose::CodigoBarraMT(void)
{
	return (cod + ".MT");
}

// ---------------------------------------------------------------------------
void __fastcall TTrafose::DefineSe(TSe *se)
{
	this->se = se;
}

// ---------------------------------------------------------------------------
// eof

// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>

#include "TSocorro.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global
// ---------------------------------------------------------------------------
TSocorro* __fastcall NewObjSocorro(void)
{
	try
	{
		return (new TSocorro());
	}
	catch (Exception &e)
	{
	}
	// não conseguiu criar objeto
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TSocorro::TSocorro(void)
{
	chave_id = -1;
	se1_id = -1;
	se2_id = -1;
	circuito1_id = -1;
	circuito2_id = -1;
	barra1_id = -1;
	barra2_id = -1;
	telecomando = false;
	ativo= false;
}

// ---------------------------------------------------------------------------
__fastcall TSocorro::~TSocorro(void)
{
	
}

// ---------------------------------------------------------------------------
// eof

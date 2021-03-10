// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Constante\Fases.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include "TUtc.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
TUtc* __fastcall NewObjUtc(void)
{
	try
	{
		return (new TUtc());
	}
	catch (Exception &e)
	{
	}
	// erro na criação do objeto
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TUtc::TUtc(void)
{
	// assume que não é real
	real = false;
}

// ---------------------------------------------------------------------------
__fastcall TUtc::~TUtc(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TUtc::Codigo(void)
{
	switch (tipo_utc)
	{
	case utcTRI:
		return ("TRI");
	case utcLUZ:
		return ("LUZ");
	case utcF1:
		return ("F1");
	case utcF2:
		return ("F2");
	case utcIMP:
		return ("IMP");
	case utcPAR:
		return ("PAR");
	default:
		return ("Indefinida");
	}
}

// ---------------------------------------------------------------------------
double __fastcall TUtc::DefasagemPriSec(void)
{
	if ((pri.ligacao == lgTRIANG) && (sec.ligacao != lgTRIANG))
		return (30);
	if ((pri.ligacao != lgTRIANG) && (sec.ligacao == lgTRIANG))
		return (-30);
	return (0.);
}

// ---------------------------------------------------------------------------
int __fastcall TUtc::LigacaoPrimario(void)
{
	return (pri.ligacao);
}

// ---------------------------------------------------------------------------
int __fastcall TUtc::LigacaoSecundario(void)
{
	return (sec.ligacao);
}

// ---------------------------------------------------------------------------
// eof

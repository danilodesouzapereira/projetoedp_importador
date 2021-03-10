// ---------------------------------------------------------------------------
#ifndef TUtcH
#define TUtcH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

// ---------------------------------------------------------------------------
class TEt;

enum tipoUTC
{
	utcTRI = 1, utcLUZ, utcF1, utcF2, utcIMP, utcPAR, utcINDEF
};

// ---------------------------------------------------------------------------
class TUtc : public TObject
{
public:
	__fastcall TUtc(void);
	__fastcall ~TUtc(void);
	AnsiString __fastcall Codigo(void);
	double __fastcall DefasagemPriSec(void);
	int __fastcall LigacaoPrimario(void);
	int __fastcall LigacaoSecundario(void);

	// objetos externos
	TEt *et;

	// dados

	bool real;
	int id;
	int oid;
	int tipo_utc;
	int tipo_ligacao;
	// int          fases_mt;
	// int          fases_bt;
	double snom_kva;
	double perda_fe_perc;
	double r_pu;
	double x_pu;

	struct
	{
		int fases;
		int ligacao;
	}

	pri, sec;
};

// ---------------------------------------------------------------------------
// função global
// ---------------------------------------------------------------------------
TUtc* __fastcall NewObjUtc(void);

#endif
// ---------------------------------------------------------------------------
// eof

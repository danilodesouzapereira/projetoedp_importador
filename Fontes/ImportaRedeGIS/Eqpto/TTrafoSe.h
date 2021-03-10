// ---------------------------------------------------------------------------
#ifndef TTrafoseH
#define TTrafoseH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

// ---------------------------------------------------------------------------
class TSe;
class VTBarra;

// ---------------------------------------------------------------------------
class TTrafose : public TObject
{
public:
	__fastcall TTrafose(void);
	__fastcall ~TTrafose(void);
	void __fastcall DefineSe(TSe *se);
	// AnsiString __fastcall CodigoBarraAT(void);
	AnsiString __fastcall CodigoBarraMT(void);

	// objetos externos
	TSe *se;
	VTBarra *barra_at, *barra_mt;

	// dados
	int id;
	int oid;
	AnsiString cod;
	AnsiString tipo;
	double snom;
	double snomvent;
	double valta;
	double vmedia;
	double vop;
	double r0;
	double x0;
	double r1;
	double x1;
	double perda_fe_kw;
	double perda_cu_kw;
};

// ---------------------------------------------------------------------------
// função global
// ---------------------------------------------------------------------------
TTrafose* __fastcall NewObjTrafose(void);

#endif
// ---------------------------------------------------------------------------
// eof

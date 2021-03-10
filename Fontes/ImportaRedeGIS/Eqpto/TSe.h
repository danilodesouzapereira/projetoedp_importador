// ---------------------------------------------------------------------------
#ifndef TSeH
#define TSeH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

// ---------------------------------------------------------------------------
class TCircuito;
class VTBarra;
class VTRede;
class VTTrafo;
class VTTrafo3E;
class VTSuprimento;

// ---------------------------------------------------------------------------
class TSe : public TObject
{
public:
	__fastcall TSe(void);
	__fastcall ~TSe(void);
	AnsiString __fastcall CodigoBarraAT(void);
	VTBarra* __fastcall ExisteBarra(int barra_id);
	VTBarra* __fastcall ExisteBarra(double vnom_kv);
	TCircuito* __fastcall ExisteCircuito(AnsiString codigo);
	TCircuito* __fastcall ExisteCircuito(int id);
	VTSuprimento* __fastcall ExisteSuprimento(int id_circuito);
	VTTrafo* __fastcall ExisteTrafo(int id);
	VTTrafo* __fastcall ExisteTrafo(double vsec_kv);
	VTTrafo3E* __fastcall ExisteTrafo3E(int id);
	VTTrafo3E* __fastcall ExisteTrafo3E(double vbaixa_kv);
	bool __fastcall InsereBarra(VTBarra *barra);
	bool __fastcall InsereCircuito(TCircuito *circuito);
	bool __fastcall InsereTrafo(VTTrafo *trafo);
	bool __fastcall InsereTrafo3E(VTTrafo3E *trafo3E);

	// dados
	int id;
	AnsiString codigo;
	AnsiString nome;
	AnsiString reg;
	AnsiString loc_tec;
	double x_m;
	double y_m;
	VTRede *rede;
	VTSuprimento *suprimento;
	TList *lisBARRA;
	TList *lisCIRCUITO;
	TList *lisTRF;
	TList *lisTRF3E;
	//Teste
	TList *lisSUPRIMENTO;
};

// ---------------------------------------------------------------------------
TSe* __fastcall NewObjSe(void);

#endif
// ---------------------------------------------------------------------------
// eof

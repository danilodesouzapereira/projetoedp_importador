// ---------------------------------------------------------------------------
#ifndef TCircuitoH
#define TCircuitoH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

// ---------------------------------------------------------------------------
class TEt;
class TSe;
class VTBarra;
class VTRede;
class VTTrafo;
class VTTrafo3E;
class VTTrecho;

// ---------------------------------------------------------------------------
class TCircuito : public TObject
{
public:
	__fastcall TCircuito(void);
	__fastcall ~TCircuito(void);
	bool __fastcall DeleteTrecho(VTTrecho *trecho);
	VTBarra* __fastcall ExisteBarra(int barra_id);
	VTTrecho* __fastcall ExisteTrecho(int trecho_id);
	VTTrecho* __fastcall ExisteTrecho(int barra1_id, int barra2_id);
	TEt* __fastcall ExisteEp(int id);
	TEt* __fastcall ExisteEt(int id);
	bool __fastcall InsereBarra(VTBarra *barra);
	bool __fastcall InsereEp(TEt *et);
	bool __fastcall InsereEt(TEt *et);
	bool __fastcall InsereTrecho(VTTrecho *trecho);

	// objetos externos
	TSe *se;
	VTTrafo *trafo;
	VTTrafo3E *trafo3E;

	// dados
	int id;
	int GIS_bar_ini_id;
	AnsiString codigo;
	AnsiString nome;
	double vnom_kv;
	VTBarra *barra_ini;
	VTRede *rede;
	TList *lisEP;
	TList *lisET;
	TList *lisBAR;
	TList *lisTRE;

	//dados inventados FKM
	VTBarra *barra_id0_a;
    VTBarra *barra_id0_b;
};

// ---------------------------------------------------------------------------
// função global
// ---------------------------------------------------------------------------
TCircuito* __fastcall NewObjCircuito(void);

#endif
// ---------------------------------------------------------------------------
// eof

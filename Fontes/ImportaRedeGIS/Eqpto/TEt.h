// ---------------------------------------------------------------------------
#ifndef TEtH
#define TEtH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\Fontes\Constante\Const.h>
#include <PlataformaSinap\Fontes\Rede\VTFases.h>
#include "..\PreMonta\ModeloRede.h"

// ---------------------------------------------------------------------------
enum tipoET
{
	etMONOFASICA = 1, etBIFASICA = 2, etTRIFASICA = 3, etDELTA_ABERTO = 4, etDELTA_FECHADO = 5,
	etIP, etEP, etINDEFINIDA = -1
};

// ---------------------------------------------------------------------------
class TCircuito;
class TPtoCargaBT;
class TUtc;
class VTBarra;
class VTCarga;
class VTRede;
class VTTrecho;

// ---------------------------------------------------------------------------
class TEt : public TObject
{
public:
	__fastcall TEt(void);
	__fastcall ~TEt(void);
	int __fastcall ConverteBarraId(int barra_id);
	VTBarra* __fastcall BarraMaixProxima(int utm_x, int utm_y);
	void __fastcall DefBarraInicial(int barra_id);
	void __fastcall DefCircuito(TCircuito *circuito);
	void __fastcall DefTopologia(int tipo_trafo);
	VTBarra* __fastcall ExisteBarra(int barra_id);
	VTCarga* __fastcall ExisteCarga(int carga_id);
	TPtoCargaBT* __fastcall ExistePtoCargaBT(int id);
	TUtc* __fastcall ExisteUtc(int tipo_utc);
	TUtc* __fastcall ExisteUtcNaoReal(int fases_pri, int fases_sec);
	VTTrecho* __fastcall ExisteTrecho(int trecho_id);
	int __fastcall FasesPrimario(void);
	int __fastcall FasesSecundario(void);
	bool __fastcall InsereBarra(VTBarra *barra);
	bool __fastcall InsereCarga(VTCarga *carga);
	bool __fastcall InserePtoCargaBT(TPtoCargaBT *ptc);
	bool __fastcall InsereTrecho(VTTrecho *trecho);
	bool __fastcall InsereUtc(TUtc *utc);
	TList* __fastcall LisUtc(void);
	double __fastcall VnomPrimario(void);
	double __fastcall VnomSecundario(void);
	double __fastcall Vff_pri_kv(void);
	double __fastcall Vff_sec_kv(void);
	double __fastcall Vfn_pri_kv(void);
	double __fastcall Vfn_sec_kv(void);

private: // método
	TUtc* __fastcall ExisteUtc_FasesPrimario(TList *lisEXT, int fases_mt);
	TUtc* __fastcall ExisteUtc_FasesSecundario(TList *lisEXT, int fases_bt);
	TUtc* __fastcall ExisteUtcReal_FasesPrimario(int fases_mt);
	TUtc* __fastcall ExisteUtcReal_FasesSecundario(int fases_bt);
	// TUtc*       __fastcall ExisteUtc_Tipo(AnsiString tipo);
	void __fastcall IniciaLisUtc(void);
	bool __fastcall IniciaUtcDefault(void);
	TUtc* __fastcall IniciaUtcBifasica(double snom_kva, int tipo_utc, int fases_mt, int fases_bt);
	TUtc* __fastcall IniciaUtcMonofasica(double snom_kva, int tipo_utc, int fases_mt, int fases_bt);
	TUtc* __fastcall IniciaUtcTrifasica(double snom_kva);
	bool __fastcall InsereUtcEtBifasica(TUtc *utc);
	bool __fastcall InsereUtcEtDeltaAberto(TUtc *utc);
	bool __fastcall InsereUtcEtDeltaFechado(TUtc *utc);
	bool __fastcall InsereUtcEtIp(TUtc *utc);
	bool __fastcall InsereUtcEtMonofasica(TUtc *utc);
	bool __fastcall InsereUtcEtTrifasica(TUtc *utc);
	int __fastcall NumFases(int fases);
	void __fastcall RedefineFaseUtcFicticia(TUtc *utc1, TUtc *utc2, TUtc *utc3);
	void __fastcall VerificaFasesPrimario(TUtc *utc);
	void __fastcall VerificaFasesSecundario(TUtc *utc);
	bool __fastcall VerificaTipo(TUtc *utc);

public: // objetos externos

	TCircuito *circuito;
	VTRede *rede;

	// dados
	int id;
	int tipo_et;
	// {1=MONOFASICA, 2=BIFASICA, 3=TRIFASICA, 4=DELTA ABERTO, 5=DELTA FECHADO, -1=INDEFINIDO}
	AnsiString codigo;
	VTBarra *barra_mt; // int          id_barr;
	VTBarra *barra_bt; // int          id_barr_bt;
	VTCarga *carga;
	double snom_kva;
	double pri_vnom_kv;
	double sec_vnom_kv;
	int num_barra;
	int ativo;
	// bool         barra_ini_ok;
	int fases_mt;

private:
	TList *lisBAR;
	TList *lisCAR;
	TList *lisUTC;
	TList *lisPTC;
	TList *lisTRE;

	// barra inicial
	struct
	{
		int bar1_id;
		int bar2_id;
		int next_id;
	}

	barra_ini;

	// estrutura para UTCs
	struct
	{
		TUtc *utc;
	}

	et_trifasica;

	struct
	{
		TUtc *utc;
	}

	et_bifasica;

	struct
	{
		TUtc *utc;
	}

	et_monofasica;

	struct
	{
		TUtc *utc_luz;
		TUtc *utc_forca;
	}

	et_delta_aberto;

	struct
	{
		TUtc *utc_luz;
		TUtc *utc_forca1;
		TUtc *utc_forca2;
	}

	et_delta_fechado;

	struct
	{
		TUtc *utc_luz;
		TUtc *utc_forca1;
		TUtc *utc_forca2;
	}

	et_ip;
};

// ---------------------------------------------------------------------------
// função global
// ---------------------------------------------------------------------------
TEt* __fastcall NewObjEt(void);

#endif
// ---------------------------------------------------------------------------
// eof

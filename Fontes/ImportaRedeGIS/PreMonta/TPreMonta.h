// ---------------------------------------------------------------------------
#ifndef TPreMontaH
#define TPreMontaH

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\Fontes\Constante\Const.h>
#include <PlataformaSinap\Fontes\Curvas\VTCurva.h>
#include <PlataformaSinap\Fontes\Rede\Estrutura.h>
#include "VTPreMonta.h"

// ---------------------------------------------------------------------------
class TCircuito;
class TEt;
class TSe;
class TUtc;
class VTArranjo;
class VTArranjos;
class VTBarra;
class VTCabo;
class VTCarga;
class VTChave;
class VTClasses;
class VTDominio;
class VTDadosEmpresa;
class VTFases;
class VTLog;
class VTMonta;
class VTPatamares;
class VTSuporte;
class VTSuprimento;
class VTTipos;
struct strIMP;

class TLogThread;
class TLogManager;

// ---------------------------------------------------------------------------
class TPreMonta : public VTPreMonta
{
public:
	__fastcall TPreMonta(VTApl *apl);
	__fastcall ~TPreMonta(void);
	virtual bool __fastcall InsereCfg_Arranjo(sdi_ARRANJO &sdi);
	// virtual bool __fastcall  InsereCfg_Cabo(sdi_CABO &sdi);
	virtual bool __fastcall InsereCfg_Suporte(sdi_SUPORTE &sdi);
	// etd
	virtual bool __fastcall InsereEtd_Barra(sdi_BARRAMENTO &sdi);
	virtual bool __fastcall InsereEtd_Rede(sdi_SE &sdi);
	virtual bool __fastcall InsereEtd_Socorro(sdi_SOCORRO &sdi);
	virtual bool __fastcall InsereEtd_Trafo(sdi_TRAFO_SE &sdi);
	virtual void __fastcall CompletaSuprimentoCircuito(sdi_SUPRIMENTO &sdi);
	// pri
	virtual bool __fastcall InserePri_Barra(sdi_BARRA &sdi);
	virtual bool __fastcall InserePri_Capacitor(sdi_CAPACITOR &sdi);
	virtual bool __fastcall InserePri_Chave(sdi_CHAVE &sdi);
	virtual bool __fastcall InserePri_Consumidor(sdi_CONS_MT &sdi);
	virtual bool __fastcall InserePri_Gerador(sdi_GERADOR &sdi);
	virtual bool __fastcall InserePri_Link(sdi_LINK &sdi);
	virtual bool __fastcall InserePri_Rede(sdi_CIRCUITO &sdi);
	virtual bool __fastcall InserePri_Regulador(sdi_REGULADOR &sdi);
	virtual bool __fastcall InserePri_Socorro(sdi_SOCORRO &sdi);
	virtual bool __fastcall InserePri_Trafo(sdi_TRAFO_LINHA &sdi);
	virtual bool __fastcall InserePri_Trecho(sdi_TRECHO &sdi);

	 //sec
	 //virtual bool __fastcall  InsereSec_Consumidor(sdi_CONS_BT &sdi);
	 //virtual bool __fastcall  InsereSec_ConsIP(sdi_CONS_IP &sdi);
	 //virtual bool __fastcall  InsereSec_Gerador(sdi_GERADOR_BT &sdi);
	 //virtual bool __fastcall  InsereSec_Link(sdi_TRECHO_BT &sdi);
	 //virtual bool __fastcall  InsereSec_PtoCarga(sdi_PCARGA_BT &sdi);
	// virtual bool __fastcall  InsereSec_Rede(sdi_INSTAL_TRAFO &sdi);
	// virtual bool __fastcall  InsereSec_Trecho(sdi_TRECHO_BT &sdi);

	virtual bool __fastcall InsereSec_Utc(sdi_UTC_BT &sdi);
	virtual bool __fastcall Sucesso(void);
	virtual bool __fastcall TrataInicioDeDados(void);
	virtual bool __fastcall TrataFimDeDados(void);

protected: // métodos
	void __fastcall AtualizaCargaEquivalente(TEt *et, VTCarga *carga);
	VTArranjo* __fastcall ArranjoBTcerto(int fases_tag);
	VTArranjo* __fastcall ArranjoMTcerto(int fases_tag);
	VTBarra* __fastcall BarraSolta(VTChave *chave);
	bool __fastcall CalculaImpedancia(double z_pu, double fatorX_R, strIMP &imp_pu);

	bool __fastcall ConectaSocorro(void);
	int __fastcall ConverteClasseConsumidor(int classe);
	bool __fastcall ConverteTelecomando(int telecomando);
	int __fastcall ConverteTipoChave(AnsiString cod_chave);
    int __fastcall ConverteTipoGeracao(AnsiString tipo_geracao);
	AnsiString __fastcall ConverteCodigoTipoChave(AnsiString cod_chave);
//	int __fastcall ConverteTipoLigacao(int edp_lipo_ligacao);
	bool __fastcall ConverteZeqvToPotCurto(VTSuprimento *psup);
	bool __fastcall CriaArranjoTipicoBT(void);
	bool __fastcall CriaArranjoTipicoMT(void);
	VTCarga* __fastcall CriaCargaConsumidorBT(sdi_CONS_BT &sdi, int barra_id);
	VTCarga* __fastcall CriaCargaConsumidorIP(sdi_CONS_IP &sdi, int barra_id);
	TEt* __fastcall CriaEt(sdi_INSTAL_TRAFO &sdi, VTBarra *barra_mt, VTBarra *barra_bt);
	TColor __fastcall DefineCorRedePrimaria(void);
	// VTArranjo* __fastcall ExisteArranjo(int fases_tag, int cabo_fase_id, int cabo_neutro_id, int suporte_id);
	VTArranjo* __fastcall ExisteArranjo(int arranjo_id, int fases_tag, bool ramal = false);
	VTBarra*   __fastcall ExisteBarra(VTBarra *barra_ref,TList *lisBarrasIndesejaveis, VTRede *rede1= NULL, VTRede *rede2= NULL, double raio_m =1);
	VTBarra* __fastcall ExisteBarraCircuito(int bar_id, TCircuito *circ1, TCircuito *circ2);
	VTLigacao* __fastcall ExisteLigacaoCircuito(VTBarra* barra1, VTBarra* barra2, TCircuito *circ1, TCircuito *circ2);
	TSe* __fastcall ExisteSubestacao(int se_id);
	// int        __fastcall IdentificaTipoCabo(sdi_CABO &sdi);
	bool __fastcall IniciaDominio(void);
//	VTArranjo* __fastcall InsereCfg_ArranjoCaboZ0Z1(int fases_tag, VTCabo *cabo);
//	VTArranjo* __fastcall InsereCfg_ArranjoCABOxFASE(int fases_tag, VTSuporte *suporte,
//		VTCabo *cabo_fase, VTCabo *cabo_neutro);
	VTBarra* __fastcall InsereEtd_Barra(sdi_TRAFO_SE &sdi, int barra_id);
	bool __fastcall InsereEtd_Suprimento(sdi_TRAFO_SE &sdi);
	bool __fastcall InsereEtd_Suprimento(VTBarra *barra);
	bool __fastcall InsereEtd_Trafo2E(sdi_TRAFO_SE &sdi);
	bool __fastcall InsereEtd_Trafo3E(sdi_TRAFO_SE &sdi);
	VTBarra* __fastcall InserePri_BarraFicticia(VTBarra *bar_ref);
	VTBarra* __fastcall InserePri_BarraIniFicticia(double vnom, int id = -1);
	bool __fastcall InserePri_DisjuntorFicticio(void);
	VTBarra* __fastcall InsereSec_BarraTrafo(TEt *et, int barra_id);
	void __fastcall InsereSec_CargaEquivalente(TEt *et);
	bool __fastcall InsereSec_Trafo(TEt *et);
	bool __fastcall InsereSec_TrafoUtc(TEt *et, TUtc *utc);
//	bool __fastcall PossuiTrechoParalelo(TCircuito *circuito, VTTrecho *trecho);
	bool __fastcall IsBarraIsolada(VTBarra *barra);
	//void __fastcall RemoveTrechosCurtoParalelo(void);
	void __fastcall RemoveTrechosParalelo(void);
	void __fastcall TrataSESemTrafo(void);
    void __fastcall TrataTrafoZZ(void);
    void __fastcall TrataTrechoComArranjoMontante(void);
    VTTrecho* __fastcall TrechoPai(VTLigacao *ligacao);
//  void __fastcall TrataTrechosCurtos(VTTrecho *trecho);
	//funcoes para tratar Zcc
	void __fastcall TrataZcc(void);
	bool __fastcall ZpuEntreSupriAlimentador(VTSuprimento *sup, VTRede *redePri,
									complex<double> &z1, complex<double> &z0);


	// objetos externos
	VTApl *apl;
	VTDadosEmpresa *empresa;
	VTArranjos *arranjos;
	VTClasses *classes;
	VTDominio *dominio;
	VTFases *fases;
	VTLog *plog;
	VTMonta *monta;
	VTPatamares *patamares;
	VTTipos *tipos;

	// defines
#define   ERRO_DE_SISTEMA   AnsiString("Erro na criação do eqpto")
#define   POSBAR  50.
#define   DXBAR   2000
#define   DYBAR   2000
#define   ARRANJO_ID 1000
#define   INIPONTA 18
#define   FIMPONTA 22

	// dados locais
	TList *lisEQP;
	TList *lisSE;
	TList *lisSocorro;
	TSe *se;
	TCircuito *circuito;
	TEt *et;
    VTArranjo *arranjoBT;
	VTArranjo *arranjoMT;
    TList *lisTRECHO_ALT;

	struct
	{ // estrutura p/ armazenar hora inicial/final dos patamres da EDP
		strHM hm_ini;
		strHM hm_fim;
		bool ponta;
	}
	pat_edp[pat01HORA];

public:
	TLogThread *log_thread;
	TLogManager *log_manager;
	AnsiString msglog;
	const char *function;
};

#endif
// -----------------------------------------------------------------------------
// eof

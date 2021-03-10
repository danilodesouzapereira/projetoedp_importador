// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\Fontes\Constante\Fases.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTResFlowBar.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include "TEt.h"
#include "TCircuito.h"
#include "TPtoCargaBT.h"
#include "TUtc.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global
// ---------------------------------------------------------------------------
TEt* __fastcall NewObjEt(void)
{
	try
	{
		return (new TEt());
	}
	catch (Exception &e)
	{
	}
	// não conseguiu criar objeto
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TEt::TEt(void)
{
	// cria listas
	lisBAR = new TList();
	lisCAR = new TList();
	lisUTC = new TList();
	lisPTC = new TList();
	lisTRE = new TList();
	// inicia demais dados
	num_barra = 0;
}

// ---------------------------------------------------------------------------
__fastcall TEt::~TEt(void)
{
	// destrói lista sem destruir seus objetos
	if (lisBAR)
	{
		delete lisBAR;
		lisBAR = NULL;
	}
	if (lisCAR)
	{
		delete lisCAR;
		lisCAR = NULL;
	}
	// destrói lista e seus objetos
	if (lisPTC)
	{
		LimpaTList(lisPTC);
		delete lisPTC;
		lisPTC = NULL;
	}
	if (lisUTC)
	{
		LimpaTList(lisUTC);
		delete lisUTC;
		lisUTC = NULL;
	}
	if (lisTRE)
	{
		LimpaTList(lisTRE);
		delete lisTRE;
		lisTRE = NULL;
	}
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TEt::BarraMaixProxima(int utm_x, int utm_y)
{
	// variáveis locais
	int dist, dist_min;
	VTBarra *barra, *bar_sel;

	// identifica Barra com coordenadas mais próximas à indicada
	dist_min = 0;
	bar_sel = NULL;
	for (int n = 0; n < lisBAR->Count; n++)
	{
		barra = (VTBarra*)lisBAR->Items[n];
		if (bar_sel == NULL)
		{
			bar_sel = barra;
			dist_min = abs(barra->utm.x - utm_x) + abs(barra->utm.y - utm_y);
			if (dist_min == 0)
			{
				return (barra);
			}
		}
		else
		{
			dist = abs(barra->utm.x - utm_x) + abs(barra->utm.y - utm_y);
			if (dist == 0)
			{
				return (barra);
			}
			if (dist < dist_min)
			{
				dist_min = dist;
				bar_sel = barra;
			}
		}
	}
	return (bar_sel);
}

// ---------------------------------------------------------------------------
int __fastcall TEt::ConverteBarraId(int barra_id)
{
	// verifica se a ET é de IP
	if (!tipo_et == etIP)
		return (barra_id);
	// verifica se barra_id é igual ao da barra inicial
	if (barra_id == barra_ini.bar1_id)
	{ // redefine barra_id
		barra_id = barra_ini.next_id;
		// redefine barra_ini.next_id
		barra_ini.next_id = (barra_ini.next_id == barra_ini.bar1_id) ? barra_ini.bar2_id :
			barra_ini.bar1_id;
	}
	return (barra_id);
}

// ---------------------------------------------------------------------------
void __fastcall TEt::DefBarraInicial(int barra_id)
{
	// salva ID da Barra inicial
	barra_ini.bar1_id = barra_id;
	barra_ini.bar2_id = barra_id;
	barra_ini.next_id = barra_id;
	// verifica se a ET é uma IP c/ duas UTCs
	if ((tipo_et == etIP) && (lisUTC->Count == 2))
	{ // define ID p/ segunda barra inicial
		barra_ini.bar2_id = FalsoId();
		barra_ini.next_id = barra_ini.bar2_id;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEt::DefCircuito(TCircuito *circuito)
{
	// salva ponteiro
	this->circuito = circuito;
}

// ---------------------------------------------------------------------------
void __fastcall TEt::DefTopologia(int tipo_trafo)
{
	// assume que é uma ET: verifica tipo da ligação
	if (tipo_trafo == 1)
		tipo_et = etMONOFASICA;
	else if (tipo_trafo == 2)
		tipo_et = etBIFASICA;
	else if (tipo_trafo == 3)
		tipo_et = etTRIFASICA;
	else if (tipo_trafo == 4)
		tipo_et = etDELTA_ABERTO;
	else if (tipo_trafo == 5)
		tipo_et = etDELTA_FECHADO;
	else
		tipo_et = etTRIFASICA;
	// cria UTCs default de acordo com o tipo da ET
	IniciaUtcDefault();
}

// função original
// -----------------------------------------------------------------------------
VTBarra* __fastcall TEt::ExisteBarra(int barra_id)
{
	// variáveis locais
	VTBarra *barra;

	for (int nb = 0; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		if ((barra->Id == barra_id))
			return (barra);
	}
	return (NULL);
}

/* função escrita para o caso de barras com ID repetido
 // -----------------------------------------------------------------------------
 VTBarra* __fastcall TEt::ExisteBarra(int barra_id)
 {
 // variáveis locais
 VTBarra *barra;
 int externId;

 for (int nb = 0; nb < lisBAR->Count; nb++)
 { // localiza por ExternId
 barra = (VTBarra*)lisBAR->Items[nb];
 //procura somente barras com extern_id não nulo
 if(barra->Extern_id.IsEmpty())
 continue;
 externId = StrToInt(barra->Extern_id);
 if (externId == barra_id)
 return (barra);
 }
 return (NULL);
 }
 */
// -----------------------------------------------------------------------------
VTCarga* __fastcall TEt::ExisteCarga(int carga_id)
{
	// variáveis locais
	VTCarga *carga;

	for (int nb = 0; nb < lisCAR->Count; nb++)
	{
		carga = (VTCarga*)lisCAR->Items[nb];
		if ((carga->Id == carga_id))
			return (carga);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
TPtoCargaBT* __fastcall TEt::ExistePtoCargaBT(int id)
{
	// variáveis locais
	TPtoCargaBT *ptc;

	// procura PtoCargaBT em lisPTC
	for (int n = 0; n < lisPTC->Count; n++)
	{
		ptc = (TPtoCargaBT*)lisPTC->Items[n];
		if (ptc->id == id)
			return (ptc);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTTrecho* __fastcall TEt::ExisteTrecho(int trecho_id)
{
	// variáveis locais
	VTTrecho *trecho;

	// procura Barra em lisTRE
	for (int n = 0; n < lisTRE->Count; n++)
	{
		trecho = (VTTrecho*)lisTRE->Items[n];
		if (trecho->Id == trecho_id)
			return (trecho);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
TUtc* __fastcall TEt::ExisteUtc(int tipo_utc)
{
	// variáveis locais
	TUtc *utc;

	// procura utc do do tipo indicado
	for (int n = 0; n < lisUTC->Count; n++)
	{
		utc = (TUtc*)lisUTC->Items[n];
		if (utc->real)
		{
			if (utc->tipo_utc == tipo_utc)
				break;
		}
		utc = NULL;
	}
	return (utc);
}

/*
 //---------------------------------------------------------------------------
 TUtc* __fastcall TEt::ExisteUtcNaoReal(void)
 {
 //variáveis locais
 TUtc *utc;

 //procura utc não real
 for (int n = 0; n < lisUTC->Count; n++)
 {
 utc = (TUtc*)lisUTC->Items[n];
 if (! utc->real) return(utc);
 }
 return(NULL);
 }
 */
// ---------------------------------------------------------------------------
TUtc* __fastcall TEt::ExisteUtcNaoReal(int fases_pri, int fases_sec)
{
	// variáveis locais
	TUtc *utc, *utc_naoreal = NULL;

	// elimina faseNT das fases indicadas
	fases_pri = fases_pri & faseABC;
	fases_sec = fases_sec & faseABC;
	// procura utc não real
	for (int n = 0; n < lisUTC->Count; n++)
	{
		utc = (TUtc*)lisUTC->Items[n];
		if (utc->real)
			continue;
		// encontrou UTC não real
		if (utc_naoreal == NULL)
			utc_naoreal = utc;
		// verifica se a UTC tem as fases indicadas do primário
		if ((utc->pri.fases & faseABC) != fases_pri)
			continue;
		// encontrou UTC não real com as fases indicadas do primário
		utc_naoreal = utc;
		// verifica se a UTC tem as fases indicadas do secundário
		if ((utc->sec.fases & faseABC) == fases_sec)
			return (utc);
	}
	return (utc_naoreal);
}

// ---------------------------------------------------------------------------
TUtc* __fastcall TEt::ExisteUtc_FasesPrimario(TList *lisEXT, int fases_mt)
{
	// variáveis locais
	TUtc *utc;

	// verifica se já existe uma Utc real c/ as mesmas fases do primário (sem considerar neutro e terra)
	for (int n = 0; n < lisEXT->Count; n++)
	{
		utc = (TUtc*)lisEXT->Items[n];
		if ((fases_mt & faseABC) == (utc->pri.fases & faseABC))
			return (utc);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
TUtc* __fastcall TEt::ExisteUtc_FasesSecundario(TList *lisEXT, int fases_bt)
{
	// variáveis locais
	int fases_utc;
	TUtc *utc;

	// verifica se há uma Utc c/ as mesmas fases do secundário (sem considerar neutro e terra)
	for (int n = 0; n < lisEXT->Count; n++)
	{
		utc = (TUtc*)lisEXT->Items[n];
		if ((fases_bt & faseABC) == (utc->sec.fases & faseABC))
			return (utc);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
TUtc* __fastcall TEt::ExisteUtcReal_FasesPrimario(int fases_mt)
{
	// variáveis locais
	TUtc *utc;

	// verifica se já existe uma Utc real c/ as mesmas fases do primário (sem considerar neutro e terra)
	for (int n = 0; n < lisUTC->Count; n++)
	{
		utc = (TUtc*)lisUTC->Items[n];
		if (utc->real)
		{
			if ((fases_mt & faseABC) == (utc->pri.fases & faseABC))
				return (utc);
		}
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
TUtc* __fastcall TEt::ExisteUtcReal_FasesSecundario(int fases_bt)
{
	// variáveis locais
	int fases_utc;
	TUtc *utc;

	// verifica se há uma Utc c/ as mesmas fases do secundário (sem considerar neutro e terra)
	for (int n = 0; n < lisUTC->Count; n++)
	{
		utc = (TUtc*)lisUTC->Items[n];
		if (utc->real)
		{
			if ((fases_bt & faseABC) == (utc->sec.fases & faseABC))
				return (utc);
		}
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
int __fastcall TEt::FasesPrimario(void)
{
	// variáveis locais
	int fases_mt = 0;
	TUtc *utc;

	// loop p/ todas UTCs
	for (int n = 0; n < lisUTC->Count; n++)
	{
		utc = (TUtc*)lisUTC->Items[n];
		fases_mt |= utc->pri.fases;
	}
	return (fases_mt);
}

// ---------------------------------------------------------------------------
int __fastcall TEt::FasesSecundario(void)
{
	// variáveis locais
	int fases_bt = 0;
	TUtc *utc;

	// loop p/ todas UTCs
	for (int n = 0; n < lisUTC->Count; n++)
	{
		utc = (TUtc*)lisUTC->Items[n];
		fases_bt |= utc->sec.fases;
	}
	return (fases_bt);
}

// ---------------------------------------------------------------------------
void __fastcall TEt::IniciaLisUtc(void)
{
	// reinicia lisUTC
	lisUTC->Clear();
	// insere utcs em lisUTC, incluindo faseNT quando necessário
	switch (tipo_et)
	{
	case etEP:
		break;
	case etTRIFASICA:
		et_trifasica.utc->sec.fases |= faseNT;
		lisUTC->Add(et_trifasica.utc);
		break;
	case etMONOFASICA:
		et_monofasica.utc->pri.fases |= faseNT;
		et_monofasica.utc->sec.fases |= faseNT;
		lisUTC->Add(et_monofasica.utc);
		break;
	case etBIFASICA:
		et_monofasica.utc->pri.fases;
		et_monofasica.utc->sec.fases |= faseNT;
		lisUTC->Add(et_monofasica.utc);
		break;
	case etDELTA_ABERTO:
		// inclui neutro/terra na utc de luz: primário e secundário
		et_delta_aberto.utc_luz->pri.fases |= faseNT;
		et_delta_aberto.utc_luz->sec.fases |= faseNT;
		// inclui neutro/terra na utc de força: primário
		et_delta_aberto.utc_forca->pri.fases |= faseNT;
		// insere utc em lisUTC
		lisUTC->Add(et_delta_aberto.utc_luz);
		lisUTC->Add(et_delta_aberto.utc_forca);
		break;
	case etDELTA_FECHADO:
		// inclui neutro/terra na utc de luz: primário e secundário
		et_delta_fechado.utc_luz->pri.fases |= faseNT;
		et_delta_fechado.utc_luz->sec.fases |= faseNT;
		// inclui neutro/terra na utc de força1: primário
		et_delta_fechado.utc_forca1->pri.fases |= faseNT;
		// inclui neutro/terra na utc de força1: primário
		et_delta_fechado.utc_forca2->pri.fases |= faseNT;
		// insere utc em lisUTC
		lisUTC->Add(et_delta_fechado.utc_luz);
		lisUTC->Add(et_delta_fechado.utc_forca1);
		lisUTC->Add(et_delta_fechado.utc_forca2);
		break;
	case etIP:
		// inclui neutro/terra nas utc luz: primário e secundário
		et_ip.utc_luz->pri.fases |= faseNT;
		et_ip.utc_luz->sec.fases |= faseNT;
		// inclui neutro/terra na utc forca1: primário e secundário
		et_ip.utc_forca1->pri.fases |= faseNT;
		et_ip.utc_forca1->sec.fases |= faseNT;
		// inclui neutro/terra na utc forca2: primário e secundário
		et_ip.utc_forca2->pri.fases |= faseNT;
		et_ip.utc_forca2->sec.fases |= faseNT;
		// insere utc em lisUTC
		if (et_ip.utc_luz->real)
			lisUTC->Add(et_ip.utc_luz);
		if (et_ip.utc_forca1->real)
			lisUTC->Add(et_ip.utc_forca1);
		if (et_ip.utc_forca2->real)
			lisUTC->Add(et_ip.utc_forca2);
		// verifica se foi inserida pelo menos uma UTC
		if (lisUTC->Count == 0)
		{ // insere a utc luz fictícia
			lisUTC->Add(et_ip.utc_luz);
		}
		break;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TEt::IniciaUtcDefault(void)
{
	switch (tipo_et)
	{
	case etEP:
		et_trifasica.utc = IniciaUtcTrifasica(750);
		break;
	case etTRIFASICA:
		et_trifasica.utc = IniciaUtcTrifasica(500);
		break;
	case etMONOFASICA:
		et_monofasica.utc = IniciaUtcMonofasica(500, utcLUZ, faseA, faseAB);
		break;
	case etBIFASICA:
		et_bifasica.utc = IniciaUtcBifasica(500, utcLUZ, faseAB, faseAB);
		break;
	case etDELTA_ABERTO:
		et_delta_aberto.utc_luz = IniciaUtcMonofasica(100, utcLUZ, faseA, faseAB);
		et_delta_aberto.utc_forca = IniciaUtcMonofasica(50, utcF1, faseB, faseBC);
		break;
	case etDELTA_FECHADO:
		et_delta_fechado.utc_luz = IniciaUtcMonofasica(100, utcLUZ, faseA, faseAB);
		et_delta_fechado.utc_forca1 = IniciaUtcMonofasica(50, utcF1, faseB, faseBC);
		et_delta_fechado.utc_forca2 = IniciaUtcMonofasica(50, utcF2, faseC, faseCA);
		break;
	case etIP:
		et_ip.utc_luz = IniciaUtcMonofasica(10, utcLUZ, faseA, faseAB);
		et_ip.utc_forca1 = IniciaUtcMonofasica(10, utcF1, faseB, faseAB);
		et_ip.utc_forca2 = IniciaUtcMonofasica(10, utcF2, faseC, faseAB);
		break;
	}
	// inicia lisUTC
	IniciaLisUtc();
	return (true);
}

// ---------------------------------------------------------------------------
TUtc* __fastcall TEt::IniciaUtcBifasica(double snom_kva, int tipo_utc, int fases_mt, int fases_bt)
{
	// variáveis locais
	TUtc *utc;

	try
	{
		utc = NewObjUtc();
		utc->real = false;
		utc->tipo_utc = tipo_utc;
		utc->snom_kva = snom_kva;
		utc->r_pu = 0.015;
		utc->x_pu = 0.05;
		utc->perda_fe_perc = 0.5;
		utc->pri.ligacao = lgMONOFASICO;
		utc->pri.fases = fases_mt;
		utc->sec.ligacao = lgMONOFASICO;
		utc->sec.fases = fases_bt;
	}
	catch (Exception &)
	{
		utc = NULL;
	}
	return (utc);
}

// ---------------------------------------------------------------------------
TUtc* __fastcall TEt::IniciaUtcMonofasica(double snom_kva, int tipo_utc, int fases_mt, int fases_bt)
{
	// variáveis locais
	TUtc *utc;

	try
	{
		utc = NewObjUtc();
		utc->real = false;
		utc->tipo_utc = tipo_utc;
		utc->snom_kva = snom_kva;
		utc->r_pu = 0.015;
		utc->x_pu = 0.05;
		utc->perda_fe_perc = 0.5;
		utc->pri.ligacao = lgMONOFASICO;
		utc->pri.fases = fases_mt;
		utc->sec.ligacao = lgMONOFASICO;
		utc->sec.fases = fases_bt;
	}
	catch (Exception &)
	{
		utc = NULL;
	}
	return (utc);
}

// ---------------------------------------------------------------------------
TUtc* __fastcall TEt::IniciaUtcTrifasica(double snom_kva)
{
	// variáveis locais
	TUtc *utc;

	try
	{
		utc = NewObjUtc();
		utc->real = false;
		utc->tipo_utc = utcTRI;
		utc->snom_kva = snom_kva;
		utc->r_pu = 0.015;
		utc->x_pu = 0.05;
		utc->perda_fe_perc = 0.65;
		utc->pri.ligacao = lgTRIANG;
		utc->pri.fases = faseABC;
		utc->sec.ligacao = lgEST_AT;
		utc->sec.fases = faseABCN;
	}
	catch (Exception &)
	{
		utc = NULL;
	}
	return (utc);
}

// ---------------------------------------------------------------------------
bool __fastcall TEt::InsereBarra(VTBarra *barra)
{
	// proteção
	if (barra == NULL)
		return (false);
	// insere Barra na lista
	if (lisBAR->IndexOf(barra) < 0)
		lisBAR->Add(barra);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEt::InsereCarga(VTCarga *carga)
{
	// proteção
	if (carga == NULL)
		return (false);
	// insere Barra na lista
	if (lisCAR->IndexOf(carga) < 0)
		lisCAR->Add(carga);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEt::InserePtoCargaBT(TPtoCargaBT *ptc)
{
	// proteção
	if (ptc == NULL)
		return (false);
	// insere Ptocarga na lista
	lisPTC->Add(ptc);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEt::InsereTrecho(VTTrecho *trecho)
{
	// proeção
	if (trecho == NULL)
		return (false);
	// insere Barra em lisBAR
	lisTRE->Add(trecho);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEt::InsereUtc(TUtc *utc)
{
	bool sucesso = true;

	// if ((utc->pri.fases == faseINV)||(utc->sec.fases == faseINV))
	// {int a=0;}

	// proteção
	if (utc == NULL)
		return (false);
	// define utc como real
	utc->real = true;
	// verifica o tipo de ET
	switch (tipo_et)
	{
	case etTRIFASICA:
		sucesso = InsereUtcEtTrifasica(utc);
		break;
	case etMONOFASICA:
		sucesso = InsereUtcEtMonofasica(utc);
		break;
	case etBIFASICA:
		sucesso = InsereUtcEtBifasica(utc);
		break;
	case etDELTA_ABERTO:
		sucesso = InsereUtcEtDeltaAberto(utc);
		break;
	case etDELTA_FECHADO:
		sucesso = InsereUtcEtDeltaFechado(utc);
		break;
	case etIP:
		sucesso = InsereUtcEtIp(utc);
		break;
	case etEP:
		sucesso = false;
		break;
	default:
		sucesso = false;
		break;
	}
	// destrói a utc caso ela não tenha sido incluída
	if (!sucesso)
		delete utc;
	// reinicia lisUTC
	IniciaLisUtc();
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TEt::InsereUtcEtDeltaAberto(TUtc *utc)
{
	// variáveis locais
	int fases_pri;

	// define ligação
	utc->pri.ligacao = lgMONOFASICO;
	utc->sec.ligacao = lgMONOFASICO;
	// valida fases do primário
	switch (fases_pri = (utc->pri.fases & faseABC))
	{
	case faseA:
		utc->pri.fases = faseA;
		break;
	case faseB:
		utc->pri.fases = faseB;
		break;
	case faseC:
		utc->pri.fases = faseC;
		break;
	default:
		return (false);
	}
	// verifica se já existe uma utc com as fase do primário
	if (ExisteUtcReal_FasesPrimario(utc->pri.fases))
		return (false);
	// valida fases do secundário
	switch (utc->sec.fases & faseABC)
	{
	case faseAB:
		utc->sec.fases = faseAB;
		break;
	case faseBC:
		utc->sec.fases = faseBC;
		break;
	case faseCA:
		utc->sec.fases = faseCA;
		break;
	default:
		return (false);
	}
	// verifica se já existe uma utc com as fase do secundário
	if (ExisteUtcReal_FasesSecundario(utc->sec.fases))
		return (false);
	// verifica o tipo da utc
	switch (utc->tipo_utc)
	{
	case utcLUZ:
		// verifica se a utc luz já foi definida
		if (et_delta_aberto.utc_luz->real)
			return (false);
		// verifica se a fase do primário da UTC é disponível na Barra MT
		if ((barra_mt->resflow->Fases & fases_pri) == faseINV)
		{ // redefine fase do primário da UTC igual à fase da UTC fictícia
			utc->pri.fases = et_delta_aberto.utc_luz->pri.fases;
		}
		// destrói a utc luz pré-existente
		delete et_delta_aberto.utc_luz;
		// salva a utc luz
		et_delta_aberto.utc_luz = utc;
		break;
	case utcF1:
	case utcF2:
		// verifica se a utc força já foi definida
		if (et_delta_aberto.utc_forca->real)
			return (false);
		// verifica se a fase do primário da UTC é disponível na Barra MT
		if ((barra_mt->resflow->Fases & fases_pri) == faseINV)
		{ // redefine fase do primário da UTC igual à fase da UTC fictícia
			utc->pri.fases = et_delta_aberto.utc_forca->pri.fases;
		}
		// destrói a utc força pré-existente
		delete et_delta_aberto.utc_forca;
		// salva a utc força
		et_delta_aberto.utc_forca = utc;
		break;
	default:
		return (false);
	}
	// verifica necessidade de trocar fases das utc não reais devido à entrada de uma utc real
	RedefineFaseUtcFicticia(et_delta_aberto.utc_luz, et_delta_aberto.utc_forca, NULL);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEt::InsereUtcEtDeltaFechado(TUtc *utc)
{
	// redefine fases do secundário
	if (utc->tipo_utc == utcLUZ)
		utc->sec.fases = faseABN;
	else if (utc->tipo_utc == utcF1)
		utc->sec.fases = faseBC;
	else if (utc->tipo_utc == utcF2)
		utc->sec.fases = faseCA;
	// define ligação
	utc->pri.ligacao = lgMONOFASICO;
	utc->sec.ligacao = lgMONOFASICO;
	// valida fases do primário
	switch (utc->pri.fases & faseABC)
	{
	case faseA:
		utc->pri.fases = faseA;
		break;
	case faseB:
		utc->pri.fases = faseB;
		break;
	case faseC:
		utc->pri.fases = faseC;
		break;
	default:
		return (false);
	}
	// verifica se já existe uma utc com as fase do primário
	if (ExisteUtcReal_FasesPrimario(utc->pri.fases))
		return (false);
	// valida fases do secundário
	switch (utc->sec.fases & faseABC)
	{
	case faseAB:
		utc->sec.fases = faseAB;
		break;
	case faseBC:
		utc->sec.fases = faseBC;
		break;
	case faseCA:
		utc->sec.fases = faseCA;
		break;
	default:
		return (false);
	}
	// verifica se já existe uma utc com as fase do secundário
	if (ExisteUtcReal_FasesSecundario(utc->sec.fases))
		return (false);
	// verifica o tipo da utc
	switch (utc->tipo_utc)
	{
	case utcLUZ:
		// verifica se a utc luz já foi definida
		if (et_delta_fechado.utc_luz->real)
			return (false);
		// destrói a utc luz pré-existente
		delete et_delta_fechado.utc_luz;
		// salva a utc luz
		et_delta_fechado.utc_luz = utc;
		break;
	case utcF1:
		// verifica se a utc força1 já foi definida
		if (et_delta_fechado.utc_forca1->real)
			return (false);
		// destrói a utc força1 pré-existente
		delete et_delta_fechado.utc_forca1;
		// salva a utc força1
		et_delta_fechado.utc_forca1 = utc;
		break;
	case utcF2:
		// verifica se a utc força2 já foi definida
		if (et_delta_fechado.utc_forca2->real)
			return (false);
		// destrói a utc força2 pré-existente
		delete et_delta_fechado.utc_forca2;
		// salva a utc força2
		et_delta_fechado.utc_forca2 = utc;
		break;
	default:
		return (false);
	}
	// verifica necessidade de trocar fases das utc não reais devido à entrada de uma utc real
	RedefineFaseUtcFicticia(et_delta_fechado.utc_luz, et_delta_fechado.utc_forca1,
		et_delta_fechado.utc_forca2);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEt::InsereUtcEtIp(TUtc *utc)
{
	// define ligação
	utc->pri.ligacao = lgMONOFASICO;
	utc->sec.ligacao = lgMONOFASICO;
	// valida fases do primário
	switch (utc->pri.fases & faseABC)
	{
	case faseA:
		utc->pri.fases = faseA;
		break;
	case faseB:
		utc->pri.fases = faseB;
		break;
	case faseC:
		utc->pri.fases = faseC;
		break;
	default:
		return (false);
	}
	// define fases do secundário
	utc->sec.fases = faseAB;
	// verifica o tipo da utc
	switch (utc->tipo_utc)
	{
	case utcLUZ:
		// verifica se a utc luz já foi definida
		if (et_ip.utc_luz->real)
			return (false);
		// destrói a utc luz pré-existente
		delete et_ip.utc_luz;
		// salva a utc luz
		et_ip.utc_luz = utc;
		break;
	case utcF1:
		// verifica se a utc força1 já foi definida
		if (et_ip.utc_forca1->real)
			return (false);
		// destrói a utc força1 pré-existente
		if (et_ip.utc_forca1)
			delete et_ip.utc_forca1;
		// salva a utc força1
		et_ip.utc_forca1 = utc;
		break;
	case utcF2:
		// verifica se a utc força2 já foi definida
		if (et_ip.utc_forca2->real)
			return (false);
		// destrói a utc força2 pré-existente
		if (et_ip.utc_forca2)
			delete et_ip.utc_forca2;
		// salva a utc força1
		et_ip.utc_forca2 = utc;
		break;
	default:
		return (false);
	}
	// reinicia lisUTC
	IniciaLisUtc();
	// não há necessidade de verificas fases, pois se houver 2 utcs elas são independentes
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEt::InsereUtcEtBifasica(TUtc *utc)
{
	// define ligação
	utc->pri.ligacao = lgMONOFASICO;
	utc->sec.ligacao = lgMONOFASICO;
	// valida fases do primário
	switch (utc->pri.fases & faseABC)
	{
	case faseAB:
		utc->pri.fases = faseAB;
		break;
	case faseBC:
		utc->pri.fases = faseBC;
		break;
	case faseCA:
		utc->pri.fases = faseCA;
		break;
	default:
		return (false);
	}
	// impões fases do secundário
	utc->sec.fases = faseABNT;
	// verifica se a utc já foi definida
	if (et_bifasica.utc->real)
		return (false);
	// destrói a utc pré-existente
	delete et_bifasica.utc;
	// salva a utc
	et_bifasica.utc = utc;
	// reinicia lisUTC
	IniciaLisUtc();

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEt::InsereUtcEtMonofasica(TUtc *utc)
{
	// define ligação
	utc->pri.ligacao = lgMONOFASICO;
	utc->sec.ligacao = lgMONOFASICO;
	// valida fases do primário
	switch (utc->pri.fases & faseABC)
	{
	case faseA:
		utc->pri.fases = faseA;
		break;
	case faseB:
		utc->pri.fases = faseB;
		break;
	case faseC:
		utc->pri.fases = faseC;
		break;
	default:
		return (false);
	}
	// impões fases do secundário
	utc->sec.fases = faseABNT;
	// verifica se a utc já foi definida
	if (et_monofasica.utc->real)
		return (false);
	// destrói a utc pré-existente
	delete et_monofasica.utc;
	// salva a utc
	et_monofasica.utc = utc;
	// reinicia lisUTC
	IniciaLisUtc();

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEt::InsereUtcEtTrifasica(TUtc *utc)
{
	// define ligação
	utc->pri.ligacao = lgTRIANG;
	utc->sec.ligacao = lgEST_AT;
	// impõe fases do primário e do secundário
	utc->pri.fases = faseABC;
	utc->sec.fases = faseABCNT;
	// verifica se a utc já foi definida
	if (et_trifasica.utc->real)
		return (false);
	// destrói a utc pré-existente
	delete et_trifasica.utc;
	// salva a utc
	et_trifasica.utc = utc;
	// reinicia lisUTC
	IniciaLisUtc();

	return (true);
}

// ---------------------------------------------------------------------------
TList* __fastcall TEt::LisUtc(void)
{
	return (lisUTC);
}

/*
 //---------------------------------------------------------------------------
 int __fastcall TEt::LigacaoPrimario(void)
 {
 //variáveis locais
 int  ligacao;
 int  count;
 TUtc *utc;

 //verifica número de UTCs
 switch(lisUTC->Count)
 {
 case 1: //monofásica, bifásica ou trifásica: assume trifásica
 ligacao = lgTRIANG;
 break;
 default: //delta aberto ou ferchado: assume trifásica
 ligacao = lgTRIANG;
 break;
 }
 return(ligacao);
 }

 //---------------------------------------------------------------------------
 int __fastcall TEt::LigacaoSecundario(void)
 {
 return(lgEST_AT);
 }
 */
// ---------------------------------------------------------------------------
int __fastcall TEt::NumFases(int fases)
{
	// variáveis locais
	int num_fase = 0;

	// verifica número de fases do primário
	if (fases & faseA)
		num_fase++;
	if (fases & faseB)
		num_fase++;
	if (fases & faseC)
		num_fase++;

	return (num_fase);
}

// ---------------------------------------------------------------------------
void __fastcall TEt::RedefineFaseUtcFicticia(TUtc *utc1, TUtc *utc2, TUtc *utc3)
{
	// variáveis locais
	TUtc *utc;
	TList *lisUTCR = new TList();
	TList *lisUTCF = new TList();

	// inicia lisUTCR com as UTCs reais e lisUTCF com as UTCs fictícias
	if (utc1)
	{
		(utc1->real) ? lisUTCR->Add(utc1) : lisUTCF->Add(utc1);
	}
	if (utc2)
	{
		(utc2->real) ? lisUTCR->Add(utc2) : lisUTCF->Add(utc2);
	}
	if (utc3)
	{
		(utc3->real) ? lisUTCR->Add(utc3) : lisUTCF->Add(utc3);
	}
	// loop p/ todas UTCs fictícias
	for (int n = 0; n < lisUTCF->Count; n++)
	{
		utc = (TUtc*)lisUTCF->Items[n];
		// verifica se existe uma utc em lisUTCR com as fases do primário
		if (ExisteUtc_FasesPrimario(lisUTCR, utc->pri.fases))
		{ // redefine fases da do primário da utc
			if (!ExisteUtc_FasesPrimario(lisUTCR, faseA))
				(utc->pri.fases = faseA);
			else if (!ExisteUtc_FasesPrimario(lisUTCR, faseB))
				(utc->pri.fases = faseB);
			else if (!ExisteUtc_FasesPrimario(lisUTCR, faseC))
				(utc->pri.fases = faseC);
		}
		// verifica se existe uma utc ruma utc em lisUTCR com as fases do secundário
		if (ExisteUtc_FasesSecundario(lisUTCR, utc->sec.fases))
		{ // redefine fases da do primário da utc
			if (!ExisteUtc_FasesSecundario(lisUTCR, faseAB))
				(utc->sec.fases = faseAB);
			else if (!ExisteUtc_FasesSecundario(lisUTCR, faseBC))
				(utc->sec.fases = faseBC);
			else if (!ExisteUtc_FasesSecundario(lisUTCR, faseCA))
				(utc->sec.fases = faseCA);
		}
		// insere utc em lisUTR
		lisUTCR->Add(utc);
	}
	// destrói listas locais
	delete lisUTCF;
	delete lisUTCR;
}

// ---------------------------------------------------------------------------
double __fastcall TEt::Vff_pri_kv(void)
{
	return (pri_vnom_kv);
}

// ---------------------------------------------------------------------------
double __fastcall TEt::Vff_sec_kv(void)
{
	// verifica número de fases do primário
	return (sec_vnom_kv);
}

// ---------------------------------------------------------------------------
double __fastcall TEt::Vfn_pri_kv(void)
{
	// verifica número de fases do primário
	return (pri_vnom_kv / sqrt(3.));
}

// ---------------------------------------------------------------------------
double __fastcall TEt::Vfn_sec_kv(void)
{
	// variáveis locais
	int fases_bt = FasesSecundario();

	// verifica número de fases do primário
	if (NumFases(fases_bt) == 1)
		return (sec_vnom_kv);
	else if (NumFases(fases_bt) == 2)
		return (sec_vnom_kv / 2.);
	else
		return (pri_vnom_kv / sqrt(3.));
}

// ---------------------------------------------------------------------------
double __fastcall TEt::VnomPrimario(void)
{
	// verifica se foi definida sec_vnom_kv
	if (IsDoubleZero(pri_vnom_kv))
	{
		pri_vnom_kv = 13.8; // kV
	}
	return (pri_vnom_kv);
}

// ---------------------------------------------------------------------------
double __fastcall TEt::VnomSecundario(void)
{
	// variáveis locais
	int fases_bt = FasesSecundario();

	// verifica se foi definida sec_vnom_kv
	if (IsDoubleZero(sec_vnom_kv))
	{ // assume valor default
		if ((fases_bt & faseABC) == faseABC)
			sec_vnom_kv = 0.220; // trifásica
		else
			sec_vnom_kv = 0.230; // monofásica
	}
	return (sec_vnom_kv);
}

// ---------------------------------------------------------------------------
// eof

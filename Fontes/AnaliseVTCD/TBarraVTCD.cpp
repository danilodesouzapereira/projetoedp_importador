//---------------------------------------------------------------------------
#pragma hdrstop
#include "TBarraVTCD.h"
#include "VTHist.h"
//#include <PlataformaSinap\Fontes\RedeCC\VTBarCC.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTBarraVTCD* NewObjBarraVTCD(VTBarra* pbarra)
	{
	return(new TBarraVTCD(pbarra));
	}

//---------------------------------------------------------------------------
__fastcall TBarraVTCD::TBarraVTCD(VTBarra* pbarra)
	{
	this->pbarra = pbarra;
	//pbarCC = NULL;
	//Seta o número de curtos na barra
	nc = 0;  namc = 0;  dac = 0.;

	//Cria lista de histogramas
	lisHist = new TList();
	IniciaLisHist();

	//Lista de barras descendentes
	lisBar = new TList();

	pbarIni = NULL;
	pliga   = NULL;

	ntotDef = 0.0;

	lisChavesProt = new TList(); //hk2019

	lisResVTCD  = new TList();

	for(int n = 0; n < NUM_MAX_DEF; n++)
       flagExecuta[n] = false;
	}
//---------------------------------------------------------------------------
__fastcall TBarraVTCD::~TBarraVTCD(void)
	{
	if(lisHist) {LimpaTList(lisHist); delete lisHist; lisHist = NULL;}
	if(lisBar) {delete lisBar; lisBar = NULL;}
	if(lisChavesProt) {delete lisChavesProt; lisChavesProt = NULL;}
	if(lisResVTCD) {LimpaTList(lisResVTCD); delete lisResVTCD; lisResVTCD = NULL;}
	}
//---------------------------------------------------------------------------
void  __fastcall TBarraVTCD::IniciaLisHist(void)
	{
	LimpaTList(lisHist);
	for(int ndef = 0; ndef < NUM_MAX_DEF + 1; ndef++)
		lisHist->Add(NewObjHist());       //hk2019: acrescentar histResCurto
	}
//---------------------------------------------------------------------------
double  __fastcall TBarraVTCD::PM_GetNAf(int ndef, double vaf)
	{
	VTHist *hist;
	double probAf;
	double nAf;

	hist = PM_GetHist(ndef);
	if(hist == NULL) return(0.0);

	probAf = hist->FuncaoProb(vaf);
	nAf = probAf * ((double)hist->NPtos) / 1e5;
	return(nAf);
	}
//---------------------------------------------------------------------------
double  __fastcall TBarraVTCD::PM_GetNAfP(int ndef, double vaf)
	{
	VTHist *hist;
	double probAf;
	double nAf;

	hist = PM_GetHist(ndef);
	if(hist == NULL) return(0.0);
	probAf = hist->FuncaoProb(vaf);
	nAf = probAf * ((double)hist->NPtos) / 1e5;
	nAf /= ntotDef;
	nAf *= 1e5;
	return(nAf);
	}
//---------------------------------------------------------------------------
VTBarra*  __fastcall TBarraVTCD::PM_GetBarra(void)
	{
	return(pbarra);
	}
//---------------------------------------------------------------------------
//VTBarCC*  __fastcall TBarraVTCD::PM_GetBarCC(void)
//	{
//	return(pbarCC);
//	}
//---------------------------------------------------------------------------
//void  __fastcall TBarraVTCD::PM_SetBarCC(VTBarCC* pbarCC)
//	{
//	this->pbarCC = pbarCC;
//	}
//---------------------------------------------------------------------------
VTBarraVTCD*  __fastcall TBarraVTCD::PM_GetBarIni(void)
	{
	return(pbarIni);
	}
//---------------------------------------------------------------------------
void  __fastcall TBarraVTCD::PM_SetBarIni(VTBarraVTCD* pbarIni)
	{
	this->pbarIni = pbarIni;
	}
//---------------------------------------------------------------------------
double  __fastcall TBarraVTCD::PM_GetDAc(void)
	{
	return(dac);
	}
//---------------------------------------------------------------------------
void __fastcall TBarraVTCD::PM_SetDAc(double dac)
	{
	this->dac = dac;
	}
//---------------------------------------------------------------------------
VTHist*  __fastcall TBarraVTCD::PM_GetHist(int ndef)
	{
	VTHist *hist;
	if(ndef > NUM_MAX_DEF) return(NULL);
	hist = (VTHist*)lisHist->Items[ndef];
	return(hist);
	}
//---------------------------------------------------------------------------
TList*  __fastcall TBarraVTCD::PM_GetLisBar(void)
	{
	return(lisBar);
	}
//---------------------------------------------------------------------------
VTLigacao*  __fastcall TBarraVTCD::PM_GetLiga(void)
	{
	return(pliga);
	}
//---------------------------------------------------------------------------
void __fastcall TBarraVTCD::PM_SetLiga(VTLigacao *pliga)
	{
	this->pliga = pliga;
	}
//---------------------------------------------------------------------------
double  __fastcall TBarraVTCD::PM_GetNCurto(void)
	{
	return(nc);
	}
//---------------------------------------------------------------------------
void __fastcall TBarraVTCD::PM_SetNCurto(double nc)
	{
	this->nc = nc;
	}
//---------------------------------------------------------------------------
double  __fastcall TBarraVTCD::PM_GetNAmCurto(void)
	{
	return(namc);
	}
//---------------------------------------------------------------------------
void __fastcall TBarraVTCD::PM_SetNAmCurto(double namc)
	{
	this->namc = namc;
	}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "THist.h"
#include <PlataformaSinap\DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
VTHist* __fastcall NewObjHist(void)
	{
	return(new THist());
	}
//---------------------------------------------------------------------------
__fastcall THist::THist(void)
	{
	lisPontos = new TList();
	lisInterv = new TList(); //hkhk
	Inicializa();
	p = 1.;
	valIni = 0.0;
	pmax = -1;
	pmin = 999999999.;
	pobj = NULL;
	}
//---------------------------------------------------------------------------
__fastcall THist::~THist(void)
	{
	if(lisPontos) {LimpaTList(lisPontos); delete lisPontos; lisPontos = NULL;}
	if(lisInterv) {LimpaTList(lisInterv); delete lisInterv; lisInterv = NULL;}
	}
//---------------------------------------------------------------------------
//void __fastcall THist::PontoNovo(double pnovo, int ifreq)
void __fastcall THist::PontoNovo(double pnovo, double ifreq)
	{
	Ponto  *pponto, *ppontoN, *ppontoN1;
	int ind;
	double ifreqAux;
	double dif;
	bool   flagEq;

	if(ifreq == 0) return;

	if(pnovo > pmax) pmax = pnovo;
	if(pnovo < pmin) pmin = pnovo;

	//Seta valor mínimo da distribuição de frequência
	valIni = pmin;

	//Adiciona ponto a lista
	ind = 0;
	ifreqAux = 0;
	for(int n = 0; n < lisPontos->Count; n++)
		{
		pponto = (Ponto*)lisPontos->Items[n];
		dif = pponto->pnovo - pnovo;
		flagEq = IsDoubleZero(dif);

		if((dif > 0)||flagEq)
		  {
		  if(flagEq)
			  {ppontoN = pponto;
				ppontoN->ifreq += ifreq;
				ppontoN->ifreqAc += ifreq;
			  }
		  else
			  {lisPontos->Insert(n, (ppontoN = new Ponto));
				ppontoN->pnovo = pnovo;
				ppontoN->ifreq   = ifreq;
				ppontoN->ifreqAc = ifreq + ifreqAux;
			  }

		  for(int n1 = n + 1; n1 < lisPontos->Count; n1++)
			  {
			  ppontoN1 = (Ponto*)lisPontos->Items[n1];
			  ppontoN1->ifreqAc+= ifreq;
			  }
		  ind = 1;
		  break;
		  }
		ifreqAux = pponto->ifreqAc;
		}
	if(ind == 0)
		{lisPontos->Add(ppontoN = new Ponto);
		 ppontoN->pnovo   = pnovo;
		 ppontoN->ifreq   = ifreq;
		 ppontoN->ifreqAc = ifreq + ifreqAux;
		}

	AtualizaHist(ppontoN, ifreq);

	if((((Ponto*)lisPontos->Items[lisPontos->Count - 1])->ifreqAc-noc) != 0)
		int kkk = 0;


	}
//---------------------------------------------------------------------------
void __fastcall THist::AtualizaHist(Ponto* pponto, int ifreq)
	{
	double pnovo;
	//int    ifreq;

	pnovo = pponto->pnovo;
	//ifreq = pponto->ifreq;

	//calculo das medias
	m     = (m     * (double)noc + pnovo*ifreq) / ((double)(noc + ifreq));
	m2    = (m2    * (double)noc + (pnovo * pnovo)*ifreq) / ((double)(noc + ifreq));

	noc+=ifreq;//noc++;
	//calculo do desvio padrao
	if(noc > 1)
	 {
	 if((m2 - m * m) > 0)
		dp = sqrt((m2 - m * m) * ((double)noc/((double)noc - 1.)));
	 else
		dp = 0.;
	 }
	}
/*   //int iaux = 0;
	//calculo das medias
	m     = (m     * (double)noc + pnovo) / ((double)noc + 1);
	m2    = (m2    * (double)noc + pnovo * pnovo) / ((double)noc + 1);
	//histogramas acumulados
	//for(int i = ninterv - 1; i >= 0; i--)
	//			if(pnovo <= (double)(i + 1) * passo) hist[i] += 1;
	//			else break;
	//numero total de ocorrencias
	noc++;
	//calculo do desvio padrao
	if(noc > 1)
	 {
	 if((m2 - m * m) > 0)
		dp = sqrt((m2 - m * m) * ((double)noc/((double)noc - 1.)));
	 else
		dp = 0.;
	 }
*/
//---------------------------------------------------------------------------
double __fastcall THist::FuncaoInversaProb(double valor)
	{
	double px, py, aux;
	Ponto  *ponto;
	int    iaux, iconta;

	//Protecao
	if(lisPontos->Count == 0) return(-1);
	if((valor < 0)||(valor > 1)) return(-1);

	if(IsDoubleZero(valor - 1.0))
     {
	  ponto = (Ponto*)lisPontos->Last();
	  return(ponto->pnovo);
	  }

	iaux  = valor * noc;
	iconta= 0;
	do
	  {
	  ponto = (Ponto*)lisPontos->Items[iconta];
	  iconta++;
	  } while(ponto->ifreqAc <= iaux);

	return(ponto->pnovo);


	//iaux  = valor * (lisPontos->Count - 1);
	//ponto = (Ponto*)lisPontos->Items[iaux];
	//return(ponto->pnovo);

	}
//---------------------------------------------------------------------------
double __fastcall THist::FuncaoProb(double d)
	{
	double p0, p1;
	Ponto  *pto0, *pto1;
	double prob, paux;
	int    i0, iconta; //i1,

	//if(flag_hist) return(FuncaoProb0(d));
	if(lisPontos->Count == 0) return(0.);
	if(d < valIni) return(0.);
	if(d >= pmax) return(1.);

	//if(d < pmin)
	//	{
	//	if(pmin > valIni)
	//	  prob = ((d - valIni)/ (pmin - valIni)) * (1 / ((double)noc));
	//	else
	//	  prob = 0.;
	//	return(prob);
	//	}

	pto0 = (Ponto*)lisPontos->First();
	if(lisPontos->Count == 1)
	  {
	  if(d <= pto0->pnovo)
		  return(0.);
	  else
		  return(1.);
	  }

	for(int i = 1; i < lisPontos->Count; i++)
		{
		pto1 = (Ponto*)lisPontos->Items[i];
		if(pto1->pnovo >= d) break;
		pto0 = pto1;
		}

	double f0, f1;
	f0 = (double)pto0->ifreqAc / (double)noc;
	f1 = (double)pto1->ifreqAc / (double)noc;

	if(IsDoubleZero(pto1->pnovo - pto0->pnovo)) return(f0);
	else
     {
	  prob = f0 + (d - pto0->pnovo) * (f1 - f0) / (pto1->pnovo - pto0->pnovo);
	  return(prob);
     }

	}
/*double __fastcall THist::FuncaoProb(double d)
	{
	double p0, p1;
	double prob, paux;
	int i0, iconta; //i1,

	//if(flag_hist) return(FuncaoProb0(d));
	if(lisPontos->Count == 0) return(0.);
	if(d < valIni) return(0.);
	if(d >= pmax) return(1.);

	if(d < pmin)
		{
		if(pmin > valIni)
		  prob = ((d - valIni)/ (pmin - valIni)) * (1 / ((double)(lisPontos->Count)));
		else
		  prob = 0.;
		return(prob);
		}

	//p1 = ((Ponto*)lisPontos->Items[i])->pnovo;
	for(int i = 0; i < lisPontos->Count; i++)
		{
		//iconta = i;
		p1 = ((Ponto*)lisPontos->Items[i])->pnovo;
		iconta = i + 1;
		while(iconta < lisPontos->Count)
			 {
			 paux = ((Ponto*)lisPontos->Items[iconta])->pnovo;

			 if(IsDoubleZero(p1 - paux))
				 iconta++;
			 else
				 {
				 i = iconta - 1;
				 break;
				 }
			 }

		if(d <= p1)
		  {
		  if(IsDoubleZero(d - p1))
			 {i0 = i;}//i1 =
		  else
			 {
			 //i1 = i;
			 i0 = (i > 0) ? i - 1 : i;
			 }
		  p0 = ((Ponto*)lisPontos->Items[i0])->pnovo;
		  break;
		  }
		}

	prob = ((double)(i0 + 1))/((double)(lisPontos->Count));
	if(!IsDoubleZero(p1 - p0))
		 prob+= ((d - p0) / (p1 - p0)) * (1 / ((double)(lisPontos->Count)));
	return(prob);

	}
*/
//---------------------------------------------------------------------------
void __fastcall THist::ImprimeHistograma(FILE *fout)
	{
	Ponto  *pt, *ptAux;
	double prob;
	fprintf(fout, "prob\tvalor\tifreqAc\n");
	for(int n = 0; n <lisPontos->Count; n++)
		{
		pt = (Ponto*)lisPontos->Items[n];
		prob = FuncaoProb(pt->pnovo);
		fprintf(fout, "%12.6f\t%12.6f\t%12.6f\n", prob, pt->pnovo, pt->ifreqAc);
		}
	}
//---------------------------------------------------------------------------
void __fastcall THist::Inicializa(void)
   {
	if(lisPontos) LimpaTList(lisPontos);
	m = m2 = dp = 0.;
   noc = 0;
	}
//---------------------------------------------------------------------------
void __fastcall THist::AdicionaInterv(double pnovo, double step)
	{
	int     ninterv, ind;
	Interv* pinterv;

	ninterv = (int)(pnovo/step);
	ind = 0;
	for(int n = 0; n < lisInterv->Count; n++)
	  {
	  pinterv = (Interv*)lisInterv->Items[n];
	  if(pinterv->ni >= ninterv)
		{
		if(ninterv != pinterv->ni)
		  {
		  lisInterv->Insert(n, (pinterv = new Interv));
		  pinterv->ni = ninterv;
		  }
		ind = 1;
		break;
		}
		}
	if(ind == 0)
			 {lisInterv->Add(pinterv = new Interv);
			  pinterv->ni = ninterv;
			 }

	//Adiciona uma ocorrencia
	pinterv->n++;
	}
//---------------------------------------------------------------------------
TList* __fastcall THist::GeraInterv(double step)
	{
	Ponto* pponto;
	Interv* pinterv;
	double p, v, v0;

	if(pmin < 0)
	  p = ((int)(pmin/step) - 1) * step;
	else
	  p = ((int)(pmin/step)) * step;

	//Inicia lista de intervalos
	if(lisInterv) LimpaTList(lisInterv);
	//p = 0.0;
	v0 = FuncaoProb(p) * noc;
	while(p <= pmax)
		  {
		  v = FuncaoProb(p + step) * noc;
		  lisInterv->Add(pinterv = new Interv);
		  pinterv->n = v - v0;
		  pinterv->ni= lisInterv->Count - 1;

		  v0 = v;
		  p+=step;
		  }
	return(lisInterv);
	}
//---------------------------------------------------------------------------
double __fastcall THist::PM_GetMedia(void)
	{
	return(m);
	}
//---------------------------------------------------------------------------
double __fastcall THist::PM_GetDesvP(void)
	{
	return(dp);
	}
//---------------------------------------------------------------------------
double __fastcall THist::PM_GetNPtos(void)
	{
	return(noc);
	}
//---------------------------------------------------------------------------
TObject* __fastcall THist::PM_GetObject(void)
	{
	return(pobj);
	}
//---------------------------------------------------------------------------
TList* __fastcall THist::PM_GetLisPtos(void)
	{
	return(lisPontos);
	}
//---------------------------------------------------------------------------
double __fastcall THist::PM_GetVMax(void)
	{
	return(pmax);
	}
//---------------------------------------------------------------------------
double __fastcall THist::PM_GetVMin(void)
	{
	return(pmin);
	}
//---------------------------------------------------------------------------
double __fastcall THist::PM_GetVIni(void)
	{
	return(valIni);
	}
//---------------------------------------------------------------------------
void __fastcall THist::PM_SetObject(TObject* pobj)
	{
	this->pobj = pobj;
	}
//---------------------------------------------------------------------------
void __fastcall THist::PM_SetVIni(double vIni)
	{
	valIni = vIni;
	}
//---------------------------------------------------------------------------
//eof

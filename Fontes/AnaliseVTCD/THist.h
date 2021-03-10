//---------------------------------------------------------------------------

#ifndef THistH
#define THistH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTHist.h"
//---------------------------------------------------------------------------
class THist : public VTHist
	{
	public:
		__fastcall THist(void);
		__fastcall ~THist(void);
		double  __fastcall FuncaoInversaProb(double valor);
		double  __fastcall FuncaoProb(double d);
		//void    __fastcall PontoNovo(double pnovo, int ifreq);
		void    __fastcall PontoNovo(double pnovo, double ifreq);
		void    __fastcall ImprimeHistograma(FILE *fout);
		void    __fastcall Inicializa(void);
		void    __fastcall AdicionaInterv(double pnovo, double step);
		TList*  __fastcall GeraInterv(double step);
		TList*  __fastcall LisInterv(void) {return(lisInterv);};
		int     __fastcall NintervHist(void) {return(ninterv);};

	protected:  //métodos acessados via property
		double   __fastcall PM_GetMedia(void);
		TObject* __fastcall PM_GetObject(void);
		double   __fastcall PM_GetDesvP(void);
		//int      __fastcall PM_GetNPtos(void);
		double   __fastcall PM_GetNPtos(void);
		TList*   __fastcall PM_GetLisPtos(void);
		double   __fastcall PM_GetVMax(void);
		double   __fastcall PM_GetVMin(void);
		double   __fastcall PM_GetVIni(void);
		void     __fastcall PM_SetObject(TObject* pobj);
		void     __fastcall PM_SetVIni(double vIni);

	private:
		void    __fastcall AtualizaHist(Ponto* pponto, int ifreq);

	public:   //dados

	private: //métodos
	private:  //dados
		//int noc;//numero de ocorrencias
		double noc;//numero de ocorrencias
		double p;//passo
		double pmax, pmin;
		int ninterv;//numero de intervalos do histograma
		TList* lisPontos;//lista de pontos
		TList* lisInterv;//lista de intervalos
		double passo;//passo em minutos dos intervalos dos histogramas
		AnsiString titulo;
		double m, m2;//media e media dos quadrados
		double dp;   //desvio padrão
		double valIni; //valor inicial da função acumulada Prob(v<=valIni) = 0, vdef = 0
		TObject *pobj;
	};
#endif
//---------------------------------------------------------------------------
//eof


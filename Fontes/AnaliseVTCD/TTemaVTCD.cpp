//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
#include <Math.h>
#include <Math.hpp>
#include "TTemaVTCD.h"
#include "VTBarraVTCD.h"
#include "VTCalculaVTCD.h"
#include "VTResAlternativa.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\RedeCC\ResCurto.h>
#include <PlataformaSinap\Fontes\Redegraf\VTCelula.h>
#include <PlataformaSinap\Fontes\Redegraf\VTCelulas.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTemaVTCD::TTemaVTCD(VTApl *apl) : TTema(apl)
   {
   //define código do tema
   Codigo = "Afundamentos";
   //cria objetos
   lisEQP = new TList();

   resAltRef = NULL;

   flagExt = false;
   vmaxExt = 0.0;
   vminExt = 0.0;
   }

//---------------------------------------------------------------------------
__fastcall TTemaVTCD::~TTemaVTCD(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
double __fastcall TTemaVTCD::Afundamentos(VTCelula *celula)
   {
   //variáveis locais
   double af;
   double af_max = 0;
   VTBarra       *barra;
	VTBarraVTCD   *pbarV;
	VTCalculaVTCD *calcVTCD = (VTCalculaVTCD*)apl->GetObject(__classid(VTCalculaVTCD));

   //obtém lista de barras da celula
   lisEQP->Clear();
   celula->LisEqpto(lisEQP, eqptoBARRA);
   //obtem afundamentos p/ cada barra
	for (int n = 0; n < lisEQP->Count; n++)
      {
      barra = (VTBarra*)lisEQP->Items[n];
      if (resAlt == NULL) continue;
		pbarV = resAlt->BarVTCD[barra];
		if (pbarV == NULL) continue;
		af = pbarV->Naf[NUM_MAX_DEF][calcVTCD->NivelAf];
		if(!IsDoubleZero(calcVTCD->NivelAf - 0.1)) af -=pbarV->Naf[NUM_MAX_DEF][0.1];
		if(resAltRef != NULL)
			{
			pbarV = resAltRef->BarVTCD[barra];
			if (pbarV != NULL)
				{
				af -= pbarV->Naf[NUM_MAX_DEF][calcVTCD->NivelAf];
				if(!IsDoubleZero(calcVTCD->NivelAf - 0.1)) af -=pbarV->Naf[NUM_MAX_DEF][0.1];
            }
         }
      //verifica se primeira Barra
      if      (n == 0)      {af_max = af;}
      else if (af > af_max) {af_max = af;}
      }
   return(af_max);
   }

//---------------------------------------------------------------------------
void __fastcall TTemaVTCD::CalculaValorProprioCelula(void)
   {
   //variáveis locais
   VTCelula  *celula;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
	TList     *lisCEL  = celulas->LisCelula();
	FILE      *fout = NULL;

	//ALT000
	//VTPath   *path = (VTPath*)apl->GetObject(__classid(VTPath));
	//AnsiString arq;
	//arq = path->DirTmp() + "\\tempHP.txt";
	//fout = fopen(arq.c_str(), "wt");
	//ALT000
	//define consumo de energia em cada celula
	for (int i = 0; i < lisCEL->Count; i++)
		{
		celula = (VTCelula*)lisCEL->Items[i];
		celula->ValorProprio = Afundamentos(celula);
		if(fout) fprintf(fout, "%6.2f\n", celula->ValorProprio);
		}
	if(fout) fclose(fout);
	}

//---------------------------------------------------------------------------
void __fastcall TTemaVTCD::CalculaValorTotalCelula(void)
   {
	//variáveis locais
	int     dist;
	int     dist_max = 5;//8; //ALTERA
	double  fatpond;
	struct   {
				int       linha, coluna;
				VTCelula  *celula;
				}ref, viz;
	VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
	TList     *lisCEL  = celulas->LisCelula();
	TList     *lisBarras;


	lisBarras = new TList();

	//zera valor total e número de parcelas de todas as células
	for (int i = 0; i < lisCEL->Count; i++)
		{
		ref.celula = (VTCelula*)lisCEL->Items[i];
		ref.celula->ValorTotal = 0;
		ref.celula->NumParcela = 0;
		}
	//verifica quais células possuem valor próprio
	//valor total = valor próprio
	for (int i = 0; i < lisCEL->Count; i++)
		{
		ref.celula = (VTCelula*)lisCEL->Items[i];
		//if (IsDoubleZero(ref.celula->ValorProprio)) continue;
		lisBarras->Clear();
		ref.celula->LisEqpto(lisBarras, eqptoBARRA);
		if(lisBarras->Count == 0) continue;

		ref.celula->ValorTotal = ref.celula->ValorProprio;
		ref.celula->NumParcela = 1;
		}
	//loop p/ todas as células
	for (int i = 0; i < lisCEL->Count; i++)
		{
		ref.celula = (VTCelula*)lisCEL->Items[i];
		//verifica se a célula de referência tem valor próprio
		//if (IsDoubleZero(ref.celula->ValorProprio)) continue;
		lisBarras->Clear();
		ref.celula->LisEqpto(lisBarras, eqptoBARRA);
		if(lisBarras->Count == 0) continue;


		//determina posicionamento da célula de referência no grid
		if (! celulas->IndicesDaCelula(ref.celula, ref.linha, ref.coluna)) continue;
		//distribui seu valor na sua área de influência
		for (int j = 0; j < lisCEL->Count; j++)
			{
			viz.celula = (VTCelula*)lisCEL->Items[j];
			//verifica se a célula vizinha não tem valor próprio
			//if (! IsDoubleZero(viz.celula->ValorProprio)) continue;
			lisBarras->Clear();
			viz.celula->LisEqpto(lisBarras, eqptoBARRA);
			if(lisBarras->Count > 0) continue;



			//determina posicionamento da célula vizinha no grid
         if (! celulas->IndicesDaCelula(viz.celula, viz.linha, viz.coluna)) continue;
         //determinha distância entre célula de referência e célula vizinha
         dist = Max(abs(ref.linha - viz.linha), abs(ref.coluna - viz.coluna));
         //somente as células com dist_max da célula de referência são influenciadas
         if (dist >= dist_max) continue;
         //determina fator de ponderação
         fatpond = dist_max - dist;
         //totaliza contribuição da célula de referência na célula vizinha
         viz.celula->ValorTotal = viz.celula->ValorTotal + (fatpond * ref.celula->ValorProprio);
         //totaliza número de parcelas
         viz.celula->NumParcela = viz.celula->NumParcela + fatpond;
         }
      }
   //determina o valor total de cada célula como uma média ponderada
   for (int i = 0; i < lisCEL->Count; i++)
      {
      ref.celula = (VTCelula*)lisCEL->Items[i];
      //atualiza valor total
      if (ref.celula->NumParcela == 0) continue;
      ref.celula->ValorTotal = ref.celula->ValorTotal / ref.celula->NumParcela;
      }
	delete lisBarras;
	}

//---------------------------------------------------------------------------
void __fastcall TTemaVTCD::DefineCorCelula(void)
   {
   //variáveis locais
   VTCelula  *celula;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCEL  = celulas->LisCelula();

   //loop p/ todas celulas
	for (int n = 0; n < lisCEL->Count; n++)
      {
      celula = (VTCelula*)lisCEL->Items[n];

      //"não pinta" células com a condição
      if (celula->ValorTotal == 0)
         {
         celula->Color = clBlack;  //ALT000 celula->Color = clWhite;
         }
      //define cor da célula em função do seu valor
      else
         {
         celula->Color = DegradeCinza(celula->ValorTotal);
         }
      }
   }

//---------------------------------------------------------------------------
TColor __fastcall TTemaVTCD::DegradeCinza(double valor)
   {
   //variáveis locais
   unsigned valor_hexa = 0xff * valor;

   //retorna tom de cinza
   return(TColor(valor_hexa + (valor_hexa << 8) + (valor_hexa << 16)));
   }
   
//---------------------------------------------------------------------------
void __fastcall  TTemaVTCD::NormalizaValorTotalCelula(void)
   {
   //variáveis locais
   bool      first = true;
   double    valor_max, valor_min, dividendo;
   VTCelula  *celula;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
	TList     *lisCEL  = celulas->LisCelula();
	TList     *lisEQP;

   if(!flagExt)
   {
   //loop p/ todas celulas
   for (int n = 0; n < lisCEL->Count; n++)
      {
      celula = (VTCelula*)lisCEL->Items[n];
      if (celula->NumParcela == 0) continue;
      if (first) {valor_min = valor_max = celula->ValorTotal; first = false;}
      valor_max = Max(valor_max, fabs(celula->ValorTotal));
      valor_min = Min(valor_min, fabs(celula->ValorTotal));
      }
   }
   else
   {
   valor_max = vmaxExt;
   valor_min = vminExt;
   }
   //determina dividendo
   dividendo = valor_max;// - valor_min;
   //proteção
   if (IsDoubleZero(dividendo)) return;
   //calcula valor normalizado das celulas
	for (int n = 0; n < lisCEL->Count; n++)
		{
		celula = (VTCelula*)lisCEL->Items[n];
		celula->ValorTotal = celula->ValorTotal / dividendo;//(celula->ValorTotal - valor_min) / dividendo;
		}
	}

//---------------------------------------------------------------------------
//eof

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCalculaVTCD.h"
#include "TUcsv.h"
#include "VTBarraVTCD.h"
#include "VTHist.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\BDadoParam\Dados\VTParamConf.h>
#include <PlataformaSinap\Fontes\BDadoParam\Modela\VTGeraLisParamEqpto.h>
#include <PlataformaSinap\Fontes\Complexo\Complexo.h>
#include <PlataformaSinap\Fontes\Constante\Fases.h>
//#include <PlataformaSinap\Fontes\Curto\VTCurto.h>
#include <PlataformaSinap\Fontes\Curto\VTCurtoCS.h>
#include <PlataformaSinap\Fontes\Calculo\VTData.h>    //preciso usar funções de Data
#include <PlataformaSinap\Fontes\CurtoCircuito\VTCurtocircuito.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Esquematico\VTEsquematico.h>
#include <PlataformaSinap\Fontes\Geral\VTGeral.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Monta\VTMontaCC.h>
#include <PlataformaSinap\Fontes\Ordena\VTOrdena.h>
#include <PlataformaSinap\Fontes\Progresso\VTProgresso.h>
#include <PlataformaSinap\Fontes\Rede\VTNo.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTEqbar.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\RedeCC\VTBarCC.h>
#include <PlataformaSinap\Fontes\RedeCC\ResCurto.h>
#include <PlataformaSinap\Fontes\RedeCC\VTRedeCC.h>
#include <PlataformaSinap\Fontes\ResCC\VTResCC.h>
#include <PlataformaSinap\Fontes\Zona\VTZona.h>
#include <PlataformaSinap\Fontes\Zona\VTZonas.h>
#include <PlataformaSinap\DLL_Inc\Curto.h>
#include <PlataformaSinap\DLL_Inc\Curtocircuito.h>
#include <PlataformaSinap\DLL_Inc\Esquematico.h>
#include <PlataformaSinap\DLL_Inc\Ordena.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTCalculaVTCD* NewObjCalculaVTCD(VTApl *apl)
   {
   return(new TCalculaVTCD(apl));
   }

//---------------------------------------------------------------------------
__fastcall TCalculaVTCD::TCalculaVTCD(VTApl *apl_owner)
   {
   //Cria objeto para o cálculo do curto circuito
   //curtocircuito =  DLL_NewObjCurtocircuito(apl_owner);
   //apl = curtocircuito->Apl;
    this->apl = apl_owner;
	 //Lista de objetos com contagem de VTCDs por barra
	lisBarraVTCD = new TList();
	lisBarRede   = new TList();
	//Cria lista com taxas de falhas por rede
	lisDataRede = new TList();
	//lista com patamares de calculo
	lisPatamares= new TList();
	//Vetor com proporção de defeitos por tipo
	pDef = new double[NUM_MAX_DEF + 1];
	//Vetor com proporção por impedância de defeito
	//rDef = new double[5];
	IniciaPadraoTiposCurto();

   //redeCC    = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
   //Seta tipo de defeito
   ind_def = 0;
	ind_pat = 0;

	//Seta taxa de falhas default
	tx_def = 0.1;
	//seta taxa amostral de metros / barra
    tx_amostragem = 400;

	curtoCS = NULL;

	paramConf = NULL;
   }
//---------------------------------------------------------------------------
__fastcall TCalculaVTCD::~TCalculaVTCD(void)
   {
   if(lisBarRede) {delete lisBarRede; lisBarRede = NULL;}
   if(lisBarraVTCD) {LimpaTList(lisBarraVTCD); delete lisBarraVTCD; lisBarraVTCD = NULL;}
   if(lisDataRede) {LimpaTList(lisDataRede); delete lisDataRede; lisDataRede = NULL;}
   if(lisPatamares) {delete lisPatamares; lisPatamares = NULL;}
   if(pDef) {delete[] pDef; pDef = NULL;}
   //if(rDef) {delete[] rDef; rDef = NULL;}
   if(curtocircuito) {delete curtocircuito; curtocircuito = NULL;}
   if (net)          {delete net;           net = NULL;}
  }
//---------------------------------------------------------------------------
double __fastcall TCalculaVTCD::ResVfnPuMin(VTBarra *pbarraF)
	{
	double vmin, vfn_pu_min;
	complex<double>  vkv[MAX_FASE];
	double           r3 = 1.73205;


	curtoCS->Vfn_kv(pbarraF, vkv);
	vmin = 999.;
	if(pbarraF->zona->ExisteFase(faseA)) vmin = min(vmin, Abs(vkv[0]));
	if(pbarraF->zona->ExisteFase(faseB)) vmin = min(vmin, Abs(vkv[1]));
	if(pbarraF->zona->ExisteFase(faseC)) vmin = min(vmin, Abs(vkv[2]));
	vfn_pu_min = vmin * r3/pbarraF->vnom;
    return(vfn_pu_min);
	}
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::AcumulaResultadosHP(VTBarraVTCD *pbarV, int tipoDef)
	{
	VTBarraVTCD     *pbarVaux;
	VTBarra         *pbarraF;
	VTHist          *hist;
	complex<double>  vkv[MAX_FASE];
	double           vmin, vfn_pu_min;
	int              ifreq;
	FILE             *fout = NULL;
	double           r3 = 1.73205;

	ifreq = pbarV->Na * pDef[tipoDef];

	for(int n = 0; n < lisBarraVTCD->Count; n++)
	   {
	   pbarVaux = (VTBarraVTCD*)lisBarraVTCD->Items[n];
	   pbarraF = pbarVaux->Barra;
	   curtoCS->Vfn_kv(pbarraF, vkv);
	   vmin = 999.;
	   if(pbarraF->zona->ExisteFase(faseA)) vmin = min(vmin, Abs(vkv[0]));
	   if(pbarraF->zona->ExisteFase(faseB)) vmin = min(vmin, Abs(vkv[1]));
	   if(pbarraF->zona->ExisteFase(faseC)) vmin = min(vmin, Abs(vkv[2]));
	   vfn_pu_min = vmin * r3/pbarraF->vnom;
	   //Acumula em histograma por tipo de defeito
	   hist = pbarVaux->Hist[tipoDef];
	   hist->PontoNovo(vfn_pu_min, ifreq);
	   //Acumula em histograma total
	   hist = pbarVaux->Hist[NUM_MAX_DEF];
	   hist->PontoNovo(vfn_pu_min, ifreq);
	   //hk2019: Incluir dim.estado: resistencia de falta
	   if(fout) fprintf(fout, "%i\t%i\t%i\t%10.8f\t%10.8f\t%10.8f\t%10.8f\n", pbarV->Barra->Id, ifreq, pbarVaux->Barra->Id, vfn_pu_min, vkv[0], vkv[1], vkv[2]);
	   }
	if(fout) fclose(fout);
	}
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::IniciaParamConf(VTParamConf *paramconfExt)
   {
   VTGeraLisParamEqpto *geraLisParamEqpto = (VTGeraLisParamEqpto*)apl->GetObject(__classid(VTGeraLisParamEqpto));

   if(geraLisParamEqpto == NULL) return;


   geraLisParamEqpto->PreparaParamConfRede(paramconfExt);

   this->paramConf = paramconfExt;



   }
//---------------------------------------------------------------------------
double __fastcall TCalculaVTCD::BuscaTaxaParamConf(VTLigacao *pliga)
   {
   VTBloco *bloco;
   strDadoParamBloco *paramBloco;
   double taxaFalhasKm;

   if(paramConf == NULL)
	 {
	 taxaFalhasKm = BuscaTaxa(pliga->rede);
	 return(taxaFalhasKm);
	 }
   if(pliga->Obj == NULL) return(0.0);   //pliga->obj em TFormBDVTCD

   bloco = (VTBloco*)(pliga->Obj);

   paramBloco = paramConf->ParamBloco[bloco];

   taxaFalhasKm = 0;
   if(paramBloco != NULL)
	 {taxaFalhasKm = paramBloco->lambda;
	 }
   else
	 {taxaFalhasKm = BuscaTaxa(pliga->rede);
	  //altera para paramConf: paramConf->ParamRede[pliga->rede];
	 }

   return(taxaFalhasKm);
   }
//---------------------------------------------------------------------------
double __fastcall TCalculaVTCD::BuscaTaxa(VTRede* rede)
   {
   //PEND: nova funcao BuscaTaxa bloco - integrar com paramConf e geraLisParamEqpto->PreparaLisParamBlocoRede(

   double tx = 1.0;//0.0;
   return(tx);
   DAT_REDE  *datRede;

   for(int n = 0; n < lisDataRede->Count; n++)
	  {
	  datRede = (DAT_REDE*)lisDataRede->Items[n];
	  if(datRede->rede != rede) continue;
	  tx = datRede->taxa;
	  }

   return(tx);

   }
//---------------------------------------------------------------------------
bool __fastcall TCalculaVTCD::ExecutaCurto(VTBarra *pbarra, int tipoDef)
   {
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
   strIMP  zdef_fase, zdef_terra;
   complex<double> czero  = complex<double>(0., 0.);
   bool    resposta;


   zdef_terra.r = geral->CURTO.R_DEF * 100. / (pbarra->vnom * pbarra->vnom);
   zdef_terra.x = geral->CURTO.X_DEF * 100. / (pbarra->vnom * pbarra->vnom);

   zdef_fase.r  = 0.;
   zdef_fase.x  = 0.;

   resposta = curtoCS->Executa2(defeitoFT, Zdef_fase_ohm, Zdef_terra_ohm, pbarra);
   return(resposta);
	}
//---------------------------------------------------------------------------
bool __fastcall TCalculaVTCD::IniciaCurtoCS(void)
	{
	bool 		sucesso;
	VTData      *data = NULL;
	VTNet       *net;
	VTOrdena 	*ordena = DLL_NewObjOrdena(apl);
	VTRedes		*redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//inicialização do objeto net, preciso do Ordena e do Redes
	net = ordena->Executa(redes);

	//inicialização do objeto data sem pré falta
	data = DLL_NewObjData(apl);
	data->Inicia(apl, net);

	sucesso = curtoCS->Inicia(data, net, 0);

   return(sucesso);
   }
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::IniciaObjetoCurto(void)
   {
   if(curtoCS != NULL) {delete curtoCS; curtoCS = NULL;}
   curtoCS = DLL_NewObjCurtoCS(apl);

   //define constantes
   Raiz3      = sqrt(3.);
   INVR3      = 1. / Raiz3;
   // inicia impedâncias para o CurtoCS
   Zdef_fase_ohm.r = 0;
   Zdef_fase_ohm.x = 0;
   Zdef_terra_ohm.r = 0;
   Zdef_terra_ohm.x = 0;

   IniciaCurtoCS();
   }
//---------------------------------------------------------------------------
double __fastcall TCalculaVTCD::CorrenteCurto(TList *lisLiga)
   {
   VTLigacao *pliga;
   complex<double>	iamp[MAX_FASE+MAX_FASE];
   complex<double>	imax_amp[MAX_FASE+MAX_FASE];
   //variáveis locais
   //int         ind_fase, ind_pat;
   //VTBarCC     *barCC;
   //complex<double> ifaseA;
   //VTRedeCC    *redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
   double prec;
   double modI0,modI1,modI2, modIMax0, modIMax1, modIMax2, modMax;
   VTLigacao *pligaMax;

   prec = 0.1;
   modMax = -1;
   pligaMax = NULL;
   for(int nl = 0; nl < lisLiga->Count; nl++)
	  {
	  pliga = (VTLigacao*)lisLiga->Items[nl];
	  if(pliga->Tipo() == eqptoCHAVE) continue;
	  // zero o vetor de correntes medidas
	  for (int i = 0; i < MAX_FASE+MAX_FASE; i++)  iamp[i] = 0;

	  // zero o vetor de corrente máxima na chave
	  //for (int i = 0; i < MAX_FASE+MAX_FASE; i++)  imax_amp[i] = 0;

	  curtoCS->Iamp(pliga, iamp);
	  modI0 = Abs(iamp[0]);
	  modI1 = Abs(iamp[1]);
	  modI2 = Abs(iamp[2]);

	  //modI0 = iamp[0].real()*iamp[0].real()+iamp[0].imag()*iamp[0].imag();
	  //modI1 = iamp[1].real()*iamp[1].real()+iamp[1].imag()*iamp[1].imag();
	  //modI2 = iamp[2].real()*iamp[2].real()+iamp[2].imag()*iamp[2].imag();

	  if((modI0 + modI1 + modI2) > modMax)
		{
		modMax = (modI0 + modI1 + modI2);
		modIMax0 = modI0;
		modIMax1 = modI1;
		modIMax2 = modI2;
		pligaMax = pliga;
		}
	  }

   modMax = modIMax0;
   if(modMax < modIMax1) modMax = modIMax1;
   if(modMax < modIMax2) modMax = modIMax2;

   return(modMax);
   }
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::ExecutaCurtos1(TList *lisBarVTCD, TList *lisUCSV)
   {
   VTRedes		*redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTGrafico    *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));//ALT000
   VTBarraVTCD  *pbarV;
   TList        *lisLigaBarra;
   TUcsv *ucsv;//str_UCSV     *ucsv;
   str_ResUCSV  *res_ucsv;
   VTHist       *hist;
   double       ifreq;

   /*//IniciaCurtoHP();
   if(curtoCS != NULL) {delete curtoCS; curtoCS = NULL;}
   curtoCS = DLL_NewObjCurtoCS(apl);

   //define constantes
   Raiz3      = sqrt(3.);
   INVR3      = 1. / Raiz3;
   // inicia impedâncias para o CurtoCS
   Zdef_fase_ohm.r = 0;
   Zdef_fase_ohm.x = 0;
   Zdef_terra_ohm.r = 0;
   Zdef_terra_ohm.x = 0;

   IniciaCurtoCS();
   TList *lisAux;
   lisAux = new TList();
   for(int nb = 0; nb < lisBarVTCD->Count; nb++)
	  {//if(nb > lisBarraVTCD->Count - 5) continue;
	  pbarV = (VTBarraVTCD*)lisBarVTCD->Items[nb];
	  lisAux->Add(pbarV->Barra);
	  }
   */
   lisLigaBarra = new TList();
   grafico->Moldura();
   grafico->Refresh();
   for(int nb = lisBarVTCD->Count - 1; nb >= 0; nb--)
	  {//if(nb > lisBarraVTCD->Count - 5) continue;
	  pbarV = (VTBarraVTCD*)lisBarVTCD->Items[nb];

	  if(pbarV->Na == 0) continue;

	  //lisAux->Delete(nb); grafico->Moldura(lisAux); grafico->Refresh();
	  grafico->Moldura(pbarV->Barra); grafico->Refresh();
	  lisLigaBarra->Clear();
	  redes->LisLigacao(lisLigaBarra, pbarV->Barra);

	  LimpaTList(pbarV->lisResVTCD);

	  //para cada lisUCSV define flagJusante
	  //Guarda para o curto
	  for(int nres = 0; nres < lisUCSV->Count; nres++)
		 {
		 ucsv = (TUcsv*)lisUCSV->Items[nres];//(str_UCSV*)lisUCSV->Items[nres];
		 pbarV->lisResVTCD->Add(res_ucsv = new str_ResUCSV);
		 res_ucsv->ucsv = ucsv;
		 for(int ndef = 0; ndef < NUM_MAX_DEF; ndef++) res_ucsv->vtcd[ndef][0] = 0.0;

		 }

	  for(int ndef = 0; ndef < NUM_MAX_DEF; ndef++)
		 {
		 if(IsDoubleZero(pDef[ndef], 1e-2)) continue;
		 //enum tipoDEFEITO {defeito3F=0, defeito2F, defeitoFT, defeitoFTZ, defeito2FT};
		 curtoCS->Executa2(ndef, Zdef_fase_ohm, Zdef_terra_ohm, pbarV->Barra);

		 pbarV->FlagExecuta[ndef] = true;

		 //hk2019: Incluir dim.estado: resistencia de falta
		 //Guarda as correntes de curto em pbarV
		 pbarV->icc[ndef][0] = CorrenteCurto(lisLigaBarra);

		 //Guarda para o curto

		 for(int nres = 0; nres < pbarV->lisResVTCD->Count; nres++)
			{
			res_ucsv = (str_ResUCSV*)pbarV->lisResVTCD->Items[nres];
			res_ucsv->vtcd[ndef][0] = ResVfnPuMin(res_ucsv->ucsv->carga->pbarra);
			hist = res_ucsv->ucsv->barraVTCD->Hist[ndef];
			ifreq = pbarV->Na * pDef[ndef] * 1;//rDef[0];
			hist->PontoNovo(res_ucsv->vtcd[ndef][0], ifreq);
			hist = res_ucsv->ucsv->barraVTCD->Hist[NUM_MAX_DEF];
			hist->PontoNovo(res_ucsv->vtcd[ndef][0], ifreq);
			}


		 //if(!ExecutaCurto(pbarV->Barra, ndef)) continue;
		 //AcumulaResultadosHP(pbarV, ndef);
		 }
	  }

   //Imprime histogramas
   FILE *fout;
   AnsiString arq;
   VTPath   *path = (VTPath*)apl->GetObject(__classid(VTPath));
   for(int nres = 0; nres < lisUCSV->Count; nres++)
	  {
	  ucsv = (TUcsv*)lisUCSV->Items[nres];//(str_UCSV*)lisUCSV->Items[nres];
	  arq = path->DirExporta() + "\\hist_";
	  arq.cat_sprintf("%i.csv", nres);
	  fout = fopen(arq.c_str(), "wt");
	  hist = ucsv->barraVTCD->Hist[NUM_MAX_DEF];
	  hist->ImprimeHistograma(fout);
	  fclose(fout);
	  }


   //delete lisAux;
   delete lisLigaBarra;
   }
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::ExecutaCurtos(VTProgresso *prog)
   {
	VTBarraVTCD  *pbarV;
	AnsiString   aux;
	VTGrafico    *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));//ALT000
	TList *lisAux;
	//Atualiza barra de progresso
	prog->Start(progTEXTO);
   prog->Add("Executando curtos", true);

	lisAux = new TList();
	//IniciaCurtoHP();
   if(curtoCS != NULL) {delete curtoCS; curtoCS = NULL;}
   curtoCS = DLL_NewObjCurtoCS(apl);

   //define constantes
   Raiz3      = sqrt(3.);
   INVR3      = 1. / Raiz3;
   // inicia impedâncias para o CurtoCS
   Zdef_fase_ohm.r = 0;
   Zdef_fase_ohm.x = 0;
   Zdef_terra_ohm.r = 0;
   Zdef_terra_ohm.x = 0;

   IniciaCurtoCS();


	int icontaBar = 0;
	for(int nb = lisBarraVTCD->Count - 1; nb >= 0; nb--)
		{//if(nb > lisBarraVTCD->Count - 5) continue;
		pbarV = (VTBarraVTCD*)lisBarraVTCD->Items[nb];
		if(pbarV->Na == 0) continue;
		lisAux->Add(pbarV->Barra);
		grafico->Moldura(lisAux);
		aux.sprintf("CodBarra: %s IdBarra: %i", pbarV->Barra->Codigo, pbarV->Barra->Id);
		icontaBar++;
		prog->Add(aux);
		for(int ndef = 0; ndef < NUM_MAX_DEF; ndef++)
			{
			 if(IsDoubleZero(pDef[ndef], 1e-2)) continue;
			 //enum tipoDEFEITO {defeito3F=0, defeito2F, defeitoFT, defeitoFTZ, defeito2FT};
			 curtoCS->Executa2(ndef, Zdef_fase_ohm, Zdef_terra_ohm, pbarV->Barra);

			 //hk2019: Incluir dim.estado: resistencia de falta

			 //if(!ExecutaCurto(pbarV->Barra, ndef)) continue;
			 AcumulaResultadosHP(pbarV, ndef);
			}
		}
	//ImprimeHistogramas();
	prog->Stop();
	delete lisAux;
	}
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::ImprimeHistogramas(void)
	{
	VTPath   *path = (VTPath*)apl->GetObject(__classid(VTPath));
	FILE *fout;

	AnsiString arq;
	arq = path->DirTmp() + "\\tempHP.txt";
	fout = fopen(arq.c_str(), "wt");


	VTBarraVTCD  *pbarV;
	for(int nb = lisBarraVTCD->Count - 1; nb >= 0; nb--)
		{
		pbarV = (VTBarraVTCD*)lisBarraVTCD->Items[nb];
		fprintf(fout, "BARRA:\t%i\t%s\n", pbarV->Barra->Id, pbarV->Barra->Codigo);
		ImprimeHistograma(pbarV, fout);
		}
	fclose(fout);
	}
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::ImprimeHistograma(VTBarraVTCD *pbarV, FILE *fout)
	{
	VTHist *hist;
	double vaux;

	for(int ndef = 0; ndef < NUM_MAX_DEF; ndef++)
		{
		if(IsDoubleZero(pDef[ndef], 1e-2)) continue;
		hist = pbarV->Hist[ndef];
		fprintf(fout, "TIPODEF:\t%i\n", ndef);
		hist->ImprimeHistograma(fout);
		fprintf(fout, "Teste FProb\n");
		vaux = 0.0; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoProb(vaux));
		vaux = 0.1; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoProb(vaux));
		vaux = 0.2; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoProb(vaux));
		vaux = 0.3; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoProb(vaux));
		vaux = 0.4; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoProb(vaux));
		vaux = 0.5; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoProb(vaux));
		vaux = 0.6; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoProb(vaux));
		vaux = 0.7; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoProb(vaux));
		vaux = 0.8; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoProb(vaux));
		vaux = 0.9; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoProb(vaux));
		vaux = 1.0; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoProb(vaux));

		fprintf(fout, "Teste FInv\n");
		vaux = 0.0; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoInversaProb(vaux));
		vaux = 0.1; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoInversaProb(vaux));
		vaux = 0.2; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoInversaProb(vaux));
		vaux = 0.3; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoInversaProb(vaux));
		vaux = 0.4; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoInversaProb(vaux));
		vaux = 0.5; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoInversaProb(vaux));
		vaux = 0.6; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoInversaProb(vaux));
		vaux = 0.7; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoInversaProb(vaux));
		vaux = 0.8; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoInversaProb(vaux));
		vaux = 0.9; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoInversaProb(vaux));
		vaux = 1.0; fprintf(fout, "%6.2f\t%6.2f\n", vaux, hist->FuncaoInversaProb(vaux));
		}
	}
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::IniciaRedes(VTRedes *redes)
	{
	VTRede   *rede;
	TList    *lisRede;
	DAT_REDE *datRede;

   lisRede = redes->LisRede();
	LimpaTList(lisDataRede);
	for(int nr = 0; nr < lisRede->Count; nr++)
		{
		rede = (VTRede*)lisRede->Items[nr];
		lisDataRede->Add(datRede = new DAT_REDE);
		datRede->rede = rede;
		datRede->taxa = tx_def;
		}

	}
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::IniciaPadraoTiposCurto(void)
	{//Padrão EPRI
	pDef[defeito3F]  = 0.05;
	pDef[defeito2F]  = 0.11;
	pDef[defeitoFT]  = 0.80;
	pDef[defeitoFTZ] = 0.00;
	pDef[defeito2FT] = 0.04;
	pDef[NUM_MAX_DEF]= 1.00; //valor acumulado


	rDef[0][0] = 10; rDef[0][1] = 0.15;
	rDef[1][0] = 20; rDef[1][1] = 0.35;
	rDef[2][0] = 30; rDef[2][1] = 0.35;
	rDef[3][0] = 40; rDef[3][1] = 0.15;
	rDef[4][1] = 1.00;
	}
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::IniciaFalhas(double distMed)
	{
	VTRedes       *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTEsquematico *esquematico;
	TList        *lisRedes;
	VTBarra      *barra0, *barra1;
	VTEqbar      *psup;
	VTLigacao    *pliga;
	VTBarraVTCD *pbarV, *pbarV0;
	TList        *lisSup, *lisBarra, * lisBarraPerc, *lisLiga, *lisLigaPerc;
	double       tx;
	double       nt;
	VTHist       *hist;
	bool         flag_Esq;

	esquematico = DLL_NewObjEsquematico(apl);
	flag_Esq = esquematico->ExisteBarraSemCoordUtm(redes);
	delete esquematico;

	this->distMed = distMed; //distancia entre barras amostradas

	//Cria um objeto barraVTCD por barra da rede
	LimpaTList(lisBarraVTCD);
	lisBarRede->Clear();



	for(int nbar = 0; nbar < redes->LisBarra()->Count; nbar++)
		{
		barra0 = (VTBarra*)(redes->LisBarra())->Items[nbar];
		lisBarRede->Add(barra0);
		lisBarraVTCD->Add(pbarV = NewObjBarraVTCD(barra0));
		pbarV->Nc = 0;
		pbarV->Dac= 0.;
		barra0->Obj = pbarV;
		}

	//Monta arvore de barras partindo da barra de Suprimento
	lisSup       = new TList();
	lisBarra     = new TList();
	lisBarraPerc = new TList();
	lisLiga      = new TList();
	lisLigaPerc  = new TList();
	redes->LisEqbar(lisSup, eqptoSUPRIMENTO);
	redes->LisLigacao(lisLiga);
	//Calcula numero total de falhas nas redes
	nt = 0;
	for(int n = 0; n < lisLiga->Count; n++)
		{
		pliga = (VTLigacao*)lisLiga->Items[n];
		if(pliga->Tipo() != eqptoTRECHO)  continue;
		tx = BuscaTaxaParamConf(pliga);  //Altera BuscaTaxa(pliga->rede);
		if(flag_Esq)
			nt += tx * ((VTTrecho*)pliga)->Comprimento_m / 1e3;
		else
			nt += tx * pliga->ComprimentoUtm_cm() /1e5;
		}
	for(int n = 0; n < lisBarraVTCD->Count; n++)
		{
		pbarV = (VTBarraVTCD*)lisBarraVTCD->Items[n];
		pbarV->ntotDef = nt;
		}
	this->ntot = nt;


	lisLiga->Clear();
	while(lisSup->Count > 0)
		  {
		  psup = (VTEqbar*)lisSup->First();
		  lisSup->Delete(0);
		  lisBarra->Add(psup->pbarra);
		  while(lisBarra->Count > 0)
				{
				barra0 = (VTBarra*)lisBarra->First();
				pbarV0 = (VTBarraVTCD*)barra0->Obj;
				lisBarra->Delete(0);
				lisBarraPerc->Add(barra0);

				lisLiga->Clear();
				redes->LisLigacao(lisLiga, barra0);
				//lisBarra->Remove(barra0);
				for(int nl = 0; nl < lisLiga->Count; nl++)
					{
					pliga = (VTLigacao*)lisLiga->Items[nl];

					//Impede a ocorrencia de loopings em caso de rede não radial
					if(lisLigaPerc->IndexOf(pliga) != -1) continue;
					lisLigaPerc->Add(pliga);
					if((pliga->Tipo() == eqptoCHAVE))
						if(((VTChave*)pliga)->Aberta) continue;

					tx = BuscaTaxaParamConf(pliga);//Altera BuscaTaxa(pliga->rede);
					for(int nbar = 0; nbar < pliga->NumeroBarras(); nbar++)
						{
						barra1 = pliga->Barra(nbar);
						if(barra1 == barra0) continue;
						lisBarra->Add(barra1);
						//Acumula numero de falhas em TBarraFalha
						pbarV = (VTBarraVTCD*)(barra1->Obj);
						//Inicia a barra de origem
						pbarV->Ligacao = pliga;
						pbarV->BarIni =  pbarV0;
						pbarV0->LisBar->Add(pbarV);
						pbarV->Dac = pbarV0->Dac;

						if(pliga->Tipo() != eqptoTRECHO) continue;


						if(flag_Esq)
							{pbarV->Dac += (((VTTrecho*)pliga)->Comprimento_m / 1e3);
							 pbarV->Nc  += (tx * ((VTTrecho*)pliga)->Comprimento_m /1e3);
							}
						else
							{pbarV->Dac += pliga->ComprimentoUtm_cm() / 1e5;
							 pbarV->Nc  += (tx * pliga->ComprimentoUtm_cm() / 1e5);
							}

						}
					}
				}
		  }

	//for(int n = 0; n < lisBarraVTCD->Count; n++)
	//	{
	//	pbarV = (VTBarraVTCD*)lisBarraVTCD->Items[n];
	//	pbarV->Barra->Codigo = DoubleToStr("%10.2f", pbarV->Dac);
	//	}
	//Seleciona barras amostradas
	IniciaAmostra();

	delete lisBarraPerc;
	delete lisLigaPerc;
	delete lisSup;
   delete lisBarra;
   delete lisLiga;
   }
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::IniciaAmostra(void)
	{
	VTRedes      *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList        *lisSup;
	VTEqbar      *eqbar;
	VTBarraVTCD  *pbarV;
	double       dac, distMedKm;
	TList        *lisAux, *lisBarrasAm;

	lisSup       = new TList();
	lisAux       = new TList();
	lisBarrasAm  = new TList();
	redes->LisEqbar(lisSup, eqptoSUPRIMENTO);

	distMedKm = distMed/1e3;
	for(int ns = 0; ns < lisSup->Count; ns++)
		{
		pbarV = (VTBarraVTCD*)(((VTEqbar*)lisSup->Items[ns])->pbarra->Obj);
		lisAux->Add(pbarV);
		while(lisAux->Count > 0)
		  {
		  pbarV = (VTBarraVTCD*)lisAux->First();
		  pbarV->dacAux = (pbarV->BarIni != NULL) ? pbarV->BarIni->dacAux : 0.;
		  if((pbarV->Dac - pbarV->dacAux) >= distMedKm)
			  {
			  pbarV->dacAux = pbarV->Dac;
			  lisBarrasAm->Add(pbarV);
			  }
		  lisAux->Delete(0);
		  lisAux->Assign(pbarV->LisBar, laOr);
		  }
		}

	if(lisBarrasAm->Count == 0) return;

	//Cria lista de VTBarraVTCD finais
	lisAux->Clear();
	lisAux->Assign(lisBarraVTCD, laOr);
	for(int n = lisAux->Count - 1; n >= 0; n--)
		{
		pbarV = (VTBarraVTCD*)lisAux->Items[n];
		pbarV->Na = 0;
		if(pbarV->LisBar->Count > 0) lisAux->Delete(n);
		}

	VTBarraVTCD *barra0, *barIni;
	TList *lisBarraPerc;
	double   nac, nac0, nacN;
	bool  flag;
	lisBarraPerc= new TList();
	nac0=nacN=0;
	while(lisAux->Count > 0)
		  {
		  pbarV = (VTBarraVTCD*)lisAux->First();
		  lisAux->Delete(0);
		  barra0 = barIni = pbarV;
		  nac    = 0;
		  while(barra0 != NULL)
				 {
				 flag = false;
				 if(lisBarraPerc->IndexOf(barra0) == -1)
					 {nac += barra0->Nc; lisBarraPerc->Add(barra0);}
				 if(lisBarrasAm->IndexOf(barra0) != -1)
					{
					barra0->Na += nac;
					nac0+=nac;
					flag = true;
					nac = 0;
					}
				 barra0 = barra0->BarIni;
				 }
		  if(!flag)
			  {nacN += nac;}
		  }

	//int ndac = (int)((double)nacN / (double)lisBarrasAm->Count);
	double ndac = nacN / (double)lisBarrasAm->Count;
	for(int n = 0; n < lisBarrasAm->Count; n++)
		{
		barra0 = (VTBarraVTCD*)lisBarrasAm->Items[n];
		barra0->Na += ndac;
		}
	barra0->Na += (nacN - ndac * lisBarrasAm->Count);


	delete lisSup;
	delete lisAux;
	delete lisBarrasAm;
	delete lisBarraPerc;
	}
//---------------------------------------------------------------------------
TList* __fastcall TCalculaVTCD::PM_GetLisBarraVTCD(void)
   {
   return(lisBarraVTCD);
   }
//---------------------------------------------------------------------------
TList* __fastcall TCalculaVTCD::PM_GetLisDataRede(void)
   {
   return(lisDataRede);
   }
//---------------------------------------------------------------------------
TList* __fastcall TCalculaVTCD::PM_GetLisPatamares(void)
   {
   return(lisPatamares);
   }
//---------------------------------------------------------------------------
double __fastcall TCalculaVTCD::PM_GetNivelAf(void)
 {
 return(ind_af);
 }
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::PM_SetNivelAf(double ind_af)
 {
 this->ind_af = ind_af;
 }
//---------------------------------------------------------------------------
VTBarraVTCD*  __fastcall TCalculaVTCD::PM_GetBarVTCD(VTBarra *pbarra)
   {
   VTBarraVTCD* pbarV;

   pbarV = NULL;
   for(int n = 0; n < lisBarraVTCD->Count; n++)
      {
      pbarV = (VTBarraVTCD*)lisBarraVTCD->Items[n];
      if(pbarV->Barra == pbarra) break;
      pbarV = NULL;
      }
   return(pbarV);
   }
//---------------------------------------------------------------------------
double  __fastcall TCalculaVTCD::PM_GetTaxaFalha(VTRede* rede)
	{
	DAT_REDE *datRede;

	datRede = NULL;
	for(int nr = 0; nr < lisDataRede->Count; nr++)
		{
		datRede = (DAT_REDE*)lisDataRede->Items[nr];
		if(datRede->rede == rede) break;
		datRede = NULL;
		}
	if(datRede == NULL) return(0.0);

   return(datRede->taxa);
	}
//---------------------------------------------------------------------------
void  __fastcall TCalculaVTCD::PM_SetTaxaFalha(VTRede* rede, double taxa)
	{
	DAT_REDE *datRede;

	datRede = NULL;
	for(int nr = 0; nr < lisDataRede->Count; nr++)
		{
		datRede = (DAT_REDE*)lisDataRede->Items[nr];
		if(datRede->rede == rede) break;
		datRede = NULL;
		}
	if(datRede == NULL) return;
	datRede->taxa = taxa;
	}
//---------------------------------------------------------------------------
int  __fastcall TCalculaVTCD::PM_GetTipoDef(void)
   {
   return(ind_def);
   }
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::PM_SetTipoDef(int ind_def)
   {
   this->ind_def = ind_def;
   }
//---------------------------------------------------------------------------
int  __fastcall TCalculaVTCD::PM_GetPat(void)
   {
   return(ind_pat);
   }
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::PM_SetPat(int ind_pat)
   {
   this->ind_pat = ind_pat;
   }
//---------------------------------------------------------------------------
double  __fastcall TCalculaVTCD::PM_GetRDef(int ifaixa, int ivalor)
   {  //ALTERA
   if(ivalor > 1) return(0.0);
   if(ifaixa > 3) return(0.0);

   if(ivalor == 1)
	 {
	 if(IsDoubleZero(rDef[ifaixa][1], 1e-3)) return(0.0);
	 return(rDef[ifaixa][1]/rDef[4][1]);
	 }
   else
	 {
	 return(rDef[ifaixa][0]);
	 }
   }
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::PM_SetRDef(int ifaixa, int ivalor, double valor)
   {    //ALTERA
   if(ivalor > 1) return;

   if(ivalor == 1)
	 {
	 if(valor > 1.0) return;
	 if(ifaixa > 3) return;
	 //Atualiza valor acumulado
	 rDef[4][1] -= rDef[ifaixa][1];
	 rDef[4][1] += valor;
	 //Atualiza valor do tipo de defeito
	 rDef[ifaixa][1]    = valor;
	 }
   else
	 {
	 rDef[ifaixa][0]    = valor;
	 }
   }
//---------------------------------------------------------------------------
double  __fastcall TCalculaVTCD::PM_GetPDef(int tipoDef)
   {  //ALTERA
   if(tipoDef > (NUM_MAX_DEF - 1)) return(0.0);
   if(IsDoubleZero(pDef[NUM_MAX_DEF], 1e-3)) return(0.0);

   return(pDef[tipoDef]/pDef[NUM_MAX_DEF]);
   }
//---------------------------------------------------------------------------
void __fastcall TCalculaVTCD::PM_SetPDef(int tipoDef, double valor)
   {    //ALTERA
   if(valor > 1.0) return;
   if(tipoDef > (NUM_MAX_DEF - 1)) return;
   //Atualiza valor acumulado
   pDef[NUM_MAX_DEF] -= pDef[tipoDef];
   pDef[NUM_MAX_DEF] += valor;
   //Atualiza valor do tipo de defeito
   pDef[tipoDef]      = valor;
   }
//---------------------------------------------------------------------------
double __fastcall TCalculaVTCD::Vfn_pu_min(VTBarCC *barCC, int ndef)
   {
   //variáveis locais
   double vpu_min;

   vpu_min = Abs(barCC->Vfas_pu[ind_def][faseC]);
   //proteção
   if      (barCC->Barra->zona == NULL) return(0.);
	//inicia vpu_min c/ a tensão de uma fase existente
	for(int ndef = 0; ndef < NUM_MAX_DEF; ndef++){}
	if      (barCC->Barra->zona->ExisteFase(faseA)) vpu_min = Abs(barCC->Vfas_pu[ind_def][0]);
   else if (barCC->Barra->zona->ExisteFase(faseB)) vpu_min = Abs(barCC->Vfas_pu[ind_def][1]);
   else if (barCC->Barra->zona->ExisteFase(faseC)) vpu_min = Abs(barCC->Vfas_pu[ind_def][2]);
   else return(0.);
   //determina tensão de fase/neutro mínima em pu
   if (barCC->Barra->zona->ExisteFase(faseA)) vpu_min = min(vpu_min, Abs(barCC->Vfas_pu[ind_def][0]));
   if (barCC->Barra->zona->ExisteFase(faseB)) vpu_min = min(vpu_min, Abs(barCC->Vfas_pu[ind_def][1]));
   if (barCC->Barra->zona->ExisteFase(faseC)) vpu_min = min(vpu_min, Abs(barCC->Vfas_pu[ind_def][2]));
   return(vpu_min);
	}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/*void __fastcall TCalculaVTCD::IniciaCurtoHP(void)
	{
	//variáveis locais
	VTOrdena  *ordena  = (VTOrdena*)apl->GetObject(__classid(VTOrdena));
	VTZonas   *zonas   = (VTZonas*)apl->GetObject(__classid(VTZonas));
	VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTMontaCC *montaCC = (VTMontaCC*)apl->GetObject(__classid(VTMontaCC));

   //proteção: destrói evetnual Net criada anteriormente
   if (net) {delete net; net = NULL;}
   zonas->Executa();
	redeCC->Inicia(redes);
   net = montaCC->Executa(redeCC);
   ordena->Executa(net);
   if(curto) {delete curto; curto = NULL;}
   curto = DLL_NewObjCurtoCS(apl);
   curto->Inicia(false, net, ind_pat);
	}
*/


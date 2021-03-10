//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
#include "TFormBDVTCD.h"
#include "TFormRegioesVTCD.h"
#include "TRegiaoVTCD.h"
#include "TUcsv.h"
#include "VTCalculaVTCD.h"
#include "VTBarraVTCD.h"
#include <PlataformaSinap\Fontes\BDadoParam\Modela\VTGeraLisParamEqpto.h>
#include <PlataformaSinap\Fontes\BDadoParam\Dados\VTParamConf.h>
#include <PlataformaSinap\Fontes\CalcIndCont\VTBuscaProt.h>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Bloco\VTBlocos.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Progresso\VTProgresso.h>
#include <PlataformaSinap\Fontes\Rede\VTTipos.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\RedeCC\ResCurto.h>
#include <PlataformaSinap\DLL_Inc\BDadoParam.h>
#include <PlataformaSinap\DLL_Inc\CalcIndCont.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Progresso.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBDVTCD *FormBDVTCD;
//---------------------------------------------------------------------------
__fastcall TFormBDVTCD::TFormBDVTCD(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	: TForm(Owner)
   {
   this->Parent = parent;
   apl = NewObjApl(this, apl_owner);// = apl_owner;
   apl->Add(buscaProt = DLL_NewBuscaProt(apl));
   apl->Add(paramConf = DLL_NewObjParamConf());
   apl->Add(geraLisParamEqpto = DLL_NewObjGeraLisParamEqpto(apl));
   apl->Add(calcVTCD  = NewObjCalculaVTCD(apl));

   buscaProt->Inicia();
   VTBloco *bloco;
   TList   *lisLiga;
   TList   *lisBlocos;
   VTLigacao *pliga;
   lisBlocos = buscaProt->Blocos->LisBloco();
   for(int nb = 0; nb < lisBlocos->Count; nb++)
	  {
	  bloco = (VTBloco*)lisBlocos->Items[nb];
	  lisLiga = bloco->LisLigacao();
	  for(int nl = 0; nl < lisLiga->Count; nl++)
		 {
		 pliga = (VTLigacao*)lisLiga->Items[nl];
		 pliga->Obj = bloco;
		 }
	  }
   //Inicia paramConf
   paramConf->LeArquivosMdb(apl, "");
   calcVTCD->IniciaParamConf(paramConf);

   SgridTaxasDefeitoInicia();

   lisRegioes = new TList();
   lisUCSV    = new TList();

   formRegioesVTCD = NULL;
   }
//---------------------------------------------------------------------------
__fastcall TFormBDVTCD::~TFormBDVTCD(void)
   {
   if(apl) {delete apl; apl  = NULL;}
   if(lisRegioes) {LimpaTList(lisRegioes); delete lisRegioes; lisRegioes = NULL;}
   if(lisUCSV) {LimpaTList(lisUCSV); delete lisUCSV; lisUCSV = NULL;}

   }
//---------------------------------------------------------------------------
void __fastcall TFormBDVTCD::ActionDefLisRegioesExecute(TObject *Sender)
   {
   //Define lista de regiões entre elementos de proteção
   VTTipos *tipos =  (VTTipos*)apl->GetObject(__classid(VTTipos));
   VTTipoChave *tipoChave;
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));


   //Inicia lista de tipos de proteção
   buscaProt->LisTipoEqProt->Clear();
   buscaProt->LisTipoEqProt->Assign(tipos->LisTipoChave(), laCopy);

   for(int n = buscaProt->LisTipoEqProt->Count - 1; n >= 0; n--)
	  {
	  tipoChave = (VTTipoChave*)buscaProt->LisTipoEqProt->Items[n];
	  if (tipoChave->Tipo == chaveDJ)
			continue;
	  if (tipoChave->Tipo == chaveRA)
			continue;
	  if (tipoChave->Tipo == chaveBF)
			continue;
	  buscaProt->LisTipoEqProt->Delete(n);
	  }

   //Funcao que preenche lista de blocos entre chave e blocos a jusante até as proximas chaves com protecao
   LimpaTList(lisRegioes);
   TList *lisChavesRede;
   VTChave *chave;
   TRegiaoVTCD *regiaoVTCD, *regiaoVTCD1;
   //TList   *lisBlocosRegiao;

   lisChavesRede = new TList();
   redes->LisEqpto(lisChavesRede, eqptoCHAVE);
   IniciaRegioesVTCD(lisChavesRede, chaveDJ);
   IniciaRegioesVTCD(lisChavesRede, chaveRA);
   IniciaRegioesVTCD(lisChavesRede, chaveBF);


   //Define regiaoVTCD->flagReliga, regiaoVTCD->lisRegiaoJusante
   VTChave *eqProt;
   TList   *lisBlocosJusante;

   lisBlocosJusante = new TList();
   for(int nr = 0; nr < lisRegioes->Count; nr++)
	  {
	  regiaoVTCD = (TRegiaoVTCD*)lisRegioes->Items[nr];
	  //Identifica primeiro elemento religador a montante
	  chave = NULL;
	  for(int nc = 0; nc < regiaoVTCD->lisEqProt->Count; nc++)
		 {
		 chave = (VTChave*)regiaoVTCD->lisEqProt->Items[nc];
		 if((chave->TipoChave->Tipo == chaveRA)||(chave->TipoChave->Tipo == chaveDJ)) break;
		 chave = NULL;
		 }

	  if(chave != NULL)
		 {
		 //Busca regioes a jusante da proteçao
		 regiaoVTCD->flagReliga = true;
		 }
	  else
		 {
		 regiaoVTCD->flagReliga = false;
		 chave = (VTChave*)regiaoVTCD->lisEqProt->Items[0];
		 }
	  //Preenche lista de regioes a jusante
	  lisBlocosJusante->Clear();
	  buscaProt->LisBlJus(chave, lisBlocosJusante);
	  VTBloco *bloco;
	  for(int nb = 0; nb < lisBlocosJusante->Count; nb++)
		 {
		 bloco = (VTBloco*)lisBlocosJusante->Items[nb];
		 for(int nr1 = 0; nr1 < lisRegioes->Count; nr1++)
			{
			regiaoVTCD1 = (TRegiaoVTCD*)lisRegioes->Items[nr1];
			if(regiaoVTCD1->lisBlocosRegiao->IndexOf(bloco) != -1)
							regiaoVTCD->lisRegiaoJusante->Add(regiaoVTCD1);
			}

		 }

	  }
   delete lisChavesRede;
   delete lisBlocosJusante;

   IniciaUCSV();

   if(formRegioesVTCD) {delete formRegioesVTCD; formRegioesVTCD = NULL;}
   formRegioesVTCD = new TFormRegioesVTCD(this->Parent, apl, gbResultados);//this->Parent);
   formRegioesVTCD->MostraRegioes(lisRegioes);
   formRegioesVTCD->Align = alClient;
   //this->Hide();
   formRegioesVTCD->Show();

   PageControl1->TabIndex = 0;
   }
//---------------------------------------------------------------------------
void __fastcall TFormBDVTCD::LimpaTStringGrid(TStringGrid *StringGrid)
	{
   //limpa todos os valores da StringGrid
   for (int lin = 1; lin < StringGrid->RowCount; lin ++)
      {
      for (int col = 0; col < StringGrid->ColCount; col++)
         {
         StringGrid->Cells[col][lin] = "";
         }
      }
	}

//---------------------------------------------------------------------------
void __fastcall TFormBDVTCD::SgridTaxasDefeitoInicia(void)
	{
	//variáveis locais
	VTCalculaVTCD *calcVTCD = (VTCalculaVTCD*)apl->GetObject(__classid(VTCalculaVTCD));

	//nnhk
   LimpaTStringGrid(sgridTaxasDefeito);

   sgridTaxasDefeito->RowCount = 6;
   sgridTaxasDefeito->Cells[0][0] = "Defeito";
   sgridTaxasDefeito->Cells[1][0] = "%";

   sgridTaxasDefeito->Cells[0][1]   = "3F";
   sgridTaxasDefeito->Cells[1][1]   = DoubleToStr("%6.2f", calcVTCD->PDef[defeito3F]  *100.);

	sgridTaxasDefeito->Cells[0][2]   = "2F";
   sgridTaxasDefeito->Cells[1][2]   = DoubleToStr("%6.2f", calcVTCD->PDef[defeito2F]  *100.);

   sgridTaxasDefeito->Cells[0][3]   = "FT";
   sgridTaxasDefeito->Cells[1][3]   = DoubleToStr("%6.2f", calcVTCD->PDef[defeitoFT]  *100.);

   //sgridTaxasDefeito->Cells[0][4]   = "FTz";
   //sgridTaxasDefeito->Cells[1][4]   = DoubleToStr("%6.2f", calcVTCD->PDef[defeitoFTZ]  *100.);

   sgridTaxasDefeito->Cells[0][4]   = "2FT";
   sgridTaxasDefeito->Cells[1][4]   = DoubleToStr("%6.2f", calcVTCD->PDef[defeito2FT]  *100.);

   sgridTaxaAmostra->Cells[0][0]   = "Taxa(m/barra)";
   sgridTaxaAmostra->Cells[1][0]  = DoubleToStr("%6.2f", calcVTCD->TaxaAmostra);

   LimpaTStringGrid(sgridTaxasImpedancia);
   sgridTaxasImpedancia->RowCount = 6;
   sgridTaxasImpedancia->Cells[0][0]   = "R (ohms)";
   sgridTaxasImpedancia->Cells[1][0]   = "%";

   sgridTaxasImpedancia->Cells[0][1]   = calcVTCD->RDef[0][0];
   sgridTaxasImpedancia->Cells[1][1]   = calcVTCD->RDef[0][1];

   sgridTaxasImpedancia->Cells[0][2]   = calcVTCD->RDef[1][0];
   sgridTaxasImpedancia->Cells[1][2]   = calcVTCD->RDef[1][1];

   sgridTaxasImpedancia->Cells[0][3]   = calcVTCD->RDef[2][0];
   sgridTaxasImpedancia->Cells[1][3]   = calcVTCD->RDef[2][1];

   sgridTaxasImpedancia->Cells[0][4]   = calcVTCD->RDef[3][0];
   sgridTaxasImpedancia->Cells[1][4]   = calcVTCD->RDef[3][1];


   }

//---------------------------------------------------------------------------
void __fastcall TFormBDVTCD::IniciaRegioesVTCD(TList *lisChavesRede, int tipoChave)
   {
   VTChave *chave;
   TRegiaoVTCD *regiaoVTCD;
   for(int n = 0; n < lisChavesRede->Count; n++)
	  {
	  chave = (VTChave*)lisChavesRede->Items[n];
	  if(chave->Estado == chvABERTA) continue;
	  if(buscaProt->LisTipoEqProt->IndexOf(chave->TipoChave) < 0) continue;
	  if (chave->TipoChave->Tipo != tipoChave) continue;
	  //lisRegioes->Add(lisBlocosRegiao = new TList());
	  lisRegioes->Add(regiaoVTCD = new TRegiaoVTCD());
	  regiaoVTCD->chave = chave;
	  IniciaRegiaoVTCD(chave, regiaoVTCD);
//	  if(regiaoVTCD->lisBlocosRegiao->Count == 0)
//		int kkk = 0;
	  }

   }
//---------------------------------------------------------------------------
void __fastcall TFormBDVTCD::IniciaRegiaoVTCD(VTChave *chave, TRegiaoVTCD *regiaoVTCD)
   {
   //Lista blocos entre a chave de proteção e outras chaves de proteção a jusante
   buscaProt->ListaZonaProtJus(chave, regiaoVTCD->lisBlocosRegiao);
   VTBloco *bloco;
   for(int n = 0; n < regiaoVTCD->lisBlocosRegiao->Count; n++)
	  {
	  bloco = (VTBloco*)regiaoVTCD->lisBlocosRegiao->Items[n];
	  regiaoVTCD->lisBarrasRegiao->Assign(bloco->LisBarra(), laOr);
	  }

   //Define equipamentos de proteção até o suprimento
   buscaProt->IdentificaChvMont(chave, regiaoVTCD->lisEqProt, true);

   }
//---------------------------------------------------------------------------
void __fastcall TFormBDVTCD::IniciaAmostraRegiaoVTCD(TRegiaoVTCD *regiaoVTCD)
   {
   //Inicia amostra de curtos por regiaoVTCD
   VTBarraVTCD *pbarV;
   regiaoVTCD->lisBarrasCurto->Clear();
   for(int n = 0; n < calcVTCD->LisBarraVTCD->Count; n++)
	  {
	  pbarV  = (VTBarraVTCD*)calcVTCD->LisBarraVTCD->Items[n];

	  if(regiaoVTCD->lisBarrasRegiao->IndexOf(pbarV->Barra) == -1) continue;
	  pbarV->regiaoVTCD = regiaoVTCD;
	  if(pbarV->Na == 0) continue;

	  regiaoVTCD->lisBarrasCurto->Add(pbarV);
	  }
   }
//---------------------------------------------------------------------------
void __fastcall TFormBDVTCD::ActionDefAmostraExecute(TObject *Sender)
   {
   //Define amostra de pontos para realização de curto
   VTGrafico      *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   TRegiaoVTCD *regiaoVTCD;
   double distAmostra;
   TList  *lisBarras;

   //Inicia amostra em função da distância em metros
   //Saída lisBarraVTCD->pbarV->(barra, Na)
   distAmostra = StrToDouble(sgridTaxaAmostra->Cells[1][0]); //StrToDouble(rchDisAmostra->Lines->Strings[0]);
   calcVTCD->IniciaFalhas(distAmostra);  //

   lisBarras = new TList();
   for(int nr = 0; nr < lisRegioes->Count; nr++)
	  {
	  regiaoVTCD = (TRegiaoVTCD*)lisRegioes->Items[nr];
	  IniciaAmostraRegiaoVTCD(regiaoVTCD);
	  for(int nb = 0; nb < regiaoVTCD->lisBarrasCurto->Count; nb++)
		 lisBarras->Add(((VTBarraVTCD*)regiaoVTCD->lisBarrasCurto->Items[nb])->Barra);
	  }
   grafico->Moldura(lisBarras);
   delete lisBarras;
   PageControl1->TabIndex = 0;
   if(formRegioesVTCD) formRegioesVTCD->ActionMostraRegiao->Execute();
   }
//---------------------------------------------------------------------------
void __fastcall TFormBDVTCD::ActionGeraCurtosExecute(TObject *Sender)
   {
   //Gera curtos nas barras da amostra
   VTGrafico      *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   TRegiaoVTCD *regiaoVTCD;
   VTBarraVTCD  *pbarV;
   //Recria e inicia curtoCS
   calcVTCD->IniciaObjetoCurto();
   //Marca todas as barras a serem calculadas
   TList *lisAux;
   lisAux = new TList();
   for(int n = 0; n < lisRegioes->Count; n++)
	  {
	  regiaoVTCD = (TRegiaoVTCD*)lisRegioes->Items[n];
	  for(int nb = 0; nb < regiaoVTCD->lisBarrasCurto->Count; nb++)
		 {//if(nb > lisBarraVTCD->Count - 5) continue;
		 pbarV = (VTBarraVTCD*)regiaoVTCD->lisBarrasCurto->Items[nb];
		 lisAux->Add(pbarV->Barra);
		 }
	  }
   grafico->Moldura(lisAux);
   delete lisAux;
   for(int n = 0; n < lisRegioes->Count; n++)
	  {
	  regiaoVTCD = (TRegiaoVTCD*)lisRegioes->Items[n];
	  calcVTCD->ExecutaCurtos1(regiaoVTCD->lisBarrasCurto, lisUCSV);

	  }

   PageControl1->TabIndex = 0;
   }
//---------------------------------------------------------------------------
void __fastcall TFormBDVTCD::ActionGravaBDExecute(TObject *Sender)
   {
   //Grava resultados em BD
   //Versao 0: guarda CSV
   VTPath   *path = (VTPath*)apl->GetObject(__classid(VTPath));
   FILE *fout;
   AnsiString arq;
   TRegiaoVTCD *regiaoVTCD;
   VTBarraVTCD *pbarV;
   //str_UCSV     *ucsv;
   TUcsv        *ucsv;
   str_ResUCSV  *res_ucsv;


   arq = path->DirExporta() + "\\ResVTCD.csv";
   fout = fopen(arq.c_str(), "wt");
   /*
   TUcsv : public
		 {
		 AnsiString  descricao;
		 VTCarga     *carga;
		 TRegiaoVTCD *regiaoVTCD;
		 };
   struct str_ResUCSV : TObject
		 {
		 str_UCSV    *ucsv;
		 double      vtcd[6][3];
		 bool        flagZonaReligamento;
		 bool        flagJusanteDefeito;

		 };
   */
   VTChave* chave;
   for(int nreg = 0; nreg < lisRegioes->Count; nreg++)
	  {
	  regiaoVTCD = (TRegiaoVTCD*)lisRegioes->Items[nreg];
	  fprintf(fout, "RegiaoVTCD:; %s\n", regiaoVTCD->chave->Codigo);
	  fprintf(fout, "Religamento a montante:; %i\n", regiaoVTCD->flagReliga);
	  chave = (VTChave*)regiaoVTCD->lisEqProt->Items[0];
	  fprintf(fout, "%s", chave->Codigo);
	  for(int neq = 1; neq < regiaoVTCD->lisEqProt->Count; neq++)
		 {
		 chave = (VTChave*)regiaoVTCD->lisEqProt->Items[neq];
		 fprintf(fout, "; %s", chave->Codigo);
		 }
	  fprintf(fout, "\n");

	  for(int nb = 0; nb < regiaoVTCD->lisBarrasCurto->Count; nb++)
		 {
		 pbarV = (VTBarraVTCD*)regiaoVTCD->lisBarrasCurto->Items[nb];
		 fprintf(fout, "%s; ", pbarV->Barra->Codigo);
		 fprintf(fout, "%6.2f; ", pbarV->Na);
		 for(int ndef = 0; ndef < NUM_MAX_DEF; ndef++)
			{
			//TipoDef
			fprintf(fout, "%i; ", ndef);
			//Corrente de curto
			fprintf(fout, "%10.4f; ", pbarV->icc[ndef][0]);
			//Tensões nos UCSV
			for(int nres = 0; nres < pbarV->lisResVTCD->Count; nres++)
			   {
			   res_ucsv = (str_ResUCSV*)pbarV->lisResVTCD->Items[nres];
			   fprintf(fout, "%10.4f; ", res_ucsv->vtcd[ndef][0]);
			   }
			}
		 fprintf(fout, "\n");
		 }

	  }

   fclose(fout);
   }
//---------------------------------------------------------------------------
void __fastcall TFormBDVTCD::IniciaUCSV(void)
   {
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisCargas;
   VTCarga *carga;
   //str_UCSV *ucsv;
   TUcsv    *ucsv;
   TRegiaoVTCD *regiaoVTCD;

   LimpaTList(lisUCSV);
   lisCargas = new TList();
   redes->LisEqbar(lisCargas, eqptoCARGA);
   for(int nc = 0; nc < lisCargas->Count; nc++)
	  {
	  carga = (VTCarga*)lisCargas->Items[nc];
	  if((carga->Codigo.AnsiCompare("045EP005090539") == 0)||(carga->Codigo.AnsiCompare("045EP000099659") == 0))
		{
		lisUCSV->Add(ucsv = new TUcsv(carga));//(ucsv = new str_UCSV);
		ucsv->regiaoVTCD = NULL;
		//Busca regiao VTCD
		for(int nr = 0; nr < lisRegioes->Count; nr++)
		   {
		   regiaoVTCD = (TRegiaoVTCD*)lisRegioes->Items[nr];
		   if(regiaoVTCD->lisBarrasRegiao->IndexOf(ucsv->carga->pbarra) == -1) continue;
		   ucsv->regiaoVTCD = regiaoVTCD;
		   break;
		   }
		}

	  }
   delete lisCargas;
   }
//---------------------------------------------------------------------------
void __fastcall TFormBDVTCD::ActionDefUCSVExecute(TObject *Sender)
   {
   //Inicia lisUCSV
   VTGrafico      *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   TList   *lisCargas;
   lisCargas = new TList();
   lisCargas->Clear();
   for(int n = 0; n < lisUCSV->Count; n++)
	  {
	  lisCargas->Add(((TUcsv*)lisUCSV->Items[n])->carga);
	  }
   grafico->Moldura(lisCargas);
   delete lisCargas;
   }
//---------------------------------------------------------------------------


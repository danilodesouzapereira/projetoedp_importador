//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
//#include <stdio.h>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Esquematico\VTEsquematico.h>
#include <PlataformaSinap\Fontes\Geral\VTInfoset.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Obra\VTObra.h>
#include <PlataformaSinap\Fontes\Obra\VTObras.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamar.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamares.h>
#include <PlataformaSinap\Fontes\Planejamento\VTAlternativa.h>
#include <PlataformaSinap\Fontes\Planejamento\VTEstudo.h>
#include <PlataformaSinap\Fontes\Planejamento\VTPlanejamento.h>
#include <PlataformaSinap\Fontes\Progresso\VTProgresso.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\RedeCC\ResCurto.h>
#include <PlataformaSinap\DLL_Inc\Editor.h>
#include <PlataformaSinap\DLL_Inc\Progresso.h>
#include <PlataformaSinap\DLL_Inc\Esquematico.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TFormAnaliseVTCD.h"
#include "TFormResBar.h"
#include "TFormChart.h"
#include "VTBarraVTCD.h"
#include "VTCalculaVTCD.h"
#include "VTResAlternativa.h"
#include "VTHist.h"
#include "TTemaVTCD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormAnaliseVTCD::TFormAnaliseVTCD(TComponent *Owner, TForm* formEstudo, VTApl *apl_owner, TWinControl *parent)
   : TFormEdt(Owner, parent)
	{
	apl = NewObjApl(this, apl_owner);
	//Cria objeto para cálculo de distribuições de VTCDs
	//calcVTCD = NewObjCalculaVTCD(apl);
	apl->Add(NewObjCalculaVTCD(apl));
	//cria mapa temático para afundamentos
	apl->Add(new TTemaVTCD(apl));

	//posiciona o Form
	FormIniciaPosicao();


	//inicia CBoxPatamar
	//CBoxPatamarInicia();
	CLBoxPatamarInicia();
	//Inicia dados de entrada
	SgridTaxasRedeInicia();
	SgridTaxasDefeitoInicia();

	//Inicia alternativas de estudo
   LViewAltInicia();

   //Guarda formEstudo
   this->formEstudo = formEstudo;
   if(formEstudo == NULL)
		{
      gbAlt->Visible = false;
      this->Width -= gbAlt->Width;
		chkDif->Checked = false;
      chkDif->Enabled = false;
		LViewIndTotal->Column[1]->Width = 1;
		LViewIndRede->Column[7]->Width  = 1;
		LViewIndCarga->Column[7]->Width = 1;
		LViewIndCarga->Column[2]->Width += 20;
		}
   else
      {//esconde FormEstudo (em caso de Planejamento)
	   //formEstudo->Hide();
      }

	//pré-seleciona primeiro Tema
	if (CBoxAf->Items->Count > 0)
		{
		CBoxAf->ItemIndex = 0;
		}
	//Inicia tab
	pgcResult->TabIndex = 0;
	tbsIndicVTCD->Enabled = false;

	lisResAlt = new TList();

	tbsIndicVTCD->TabVisible = false;
	}

//---------------------------------------------------------------------------
__fastcall TFormAnaliseVTCD::~TFormAnaliseVTCD(void)
	{
	//variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTEstudo  *estudo  = ExisteEstudo();

	ConfigRede(estudo->AlternativaPadrao, NULL);
	//atualiza o gráfico: apagaa moldura e desabilita mapa temático
	grafico->Moldura();
	grafico->MapaTematicoEnabled = false;
	//reapresenta FormEstudo (em caso de Planejamento)
	//if(formEstudo != NULL) formEstudo->Show();
   //destrói objetos
   if(apl) {delete apl; apl = NULL;}
   //if(edt_busca) {delete edt_busca; edt_busca = NULL;}
   if(lisResAlt) {LimpaTList(lisResAlt); delete lisResAlt; lisResAlt = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::ActionDisablePatamarExecute(TObject *Sender)
   {
   //variáveis locais
	VTPatamar   *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //desabilita todo Patamar de Pataamres
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
		patamar = (VTPatamar*)patamares->Patamar[ind_pat];
		patamar->Enabled = false;
      }
	//reinicia CLBoxPatamar
   CLBoxPatamarInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::ActionDefaultTipoCurtoExecute(TObject *Sender)
	{
	VTCalculaVTCD *calcVTCD = (VTCalculaVTCD*)apl->GetObject(__classid(VTCalculaVTCD));
	//Reinicia taxas
	calcVTCD->IniciaPadraoTiposCurto();
	//Reinicia grid
	SgridTaxasDefeitoInicia();
	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::ActionEdtExecute(TObject *Sender)
	{
	try{//se necessário, cria um objeto TEdtMedidor
		if (edt_busca != NULL) delete edt_busca;
		//cria TEdtMedidor p/ controle do mouse
		edt_busca = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);//NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
		//atualiza indicação de mouse ativo e força botão Down
		AtualizaIndicacaoEditorOn(butEdt);
		} catch (Exception &e)
			{//nada a fazer
			}
	}

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::ActionEnablePatamarExecute(TObject *Sender)
	{
   //variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //desabilita todo Patamar de Pataamres
	for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
		{
		patamar = (VTPatamar*)patamares->Patamar[ind_pat];
		patamar->Enabled = true;
		}
	//reinicia CLBoxPatamar
	CLBoxPatamarInicia();
	}

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::IniciaChtRede(TList *lisAltF, TList *lisRedeF)
	{
	VTRedes       *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede        *rede;
	TChartSeries *serie90, *serie70, *serie40, *serieInt;
	TListItem    *item;
	TList        *lisRede;
	double  s90, s70, s40, sIn;
	VTResAlternativa *resAlt;
	AnsiString       nomeAlt, nomeRede;
	int irede, ialt;

	serie90 = chtEvRede->Series[0]; serie90->Clear();
	serie70 = chtEvRede->Series[1]; serie70->Clear();
	serie40 = chtEvRede->Series[2]; serie40->Clear();
	serieInt= chtEvRede->Series[3]; serieInt->Clear();

	for(int nr = 0; nr < LViewIndAuxTotal->Items->Count; nr++)
		{
		item = LViewIndAuxTotal->Items->Item[nr];
		resAlt = (VTResAlternativa*)item->SubItems->Objects[0];
		ialt   = lisResAlt->IndexOf(resAlt);
		if(lisAltF != NULL)
		  {if(lisAltF->IndexOf(resAlt) == -1) continue;
		  }
		s90 = StrToDouble(item->SubItems->Strings[1]);//SAIFI 90
		s70 = StrToDouble(item->SubItems->Strings[2]);//SAIFI 70
		s40 = StrToDouble(item->SubItems->Strings[3]);//SAIFI 40
		sIn = StrToDouble(item->SubItems->Strings[4]);//Interr.
		nomeAlt = "";
		nomeAlt = "A"+IntToStr(ialt);
		serie90->Add(s90, nomeAlt);
		serie70->Add(s70, nomeAlt);
		serie40->Add(s40, nomeAlt);
		serieInt->Add(sIn, nomeAlt);
		}


	lisRede = redes->LisRede();
	for(int nr = 0; nr < LViewIndAuxRede->Items->Count; nr++)
		{
		item = LViewIndAuxRede->Items->Item[nr];
		rede   = (VTRede*)item->SubItems->Objects[0];
		resAlt = (VTResAlternativa*)item->SubItems->Objects[1];
		irede  = lisRede->IndexOf(rede);
		ialt   = lisResAlt->IndexOf(resAlt);

		if(lisRedeF != NULL)
		  {if(lisRedeF->IndexOf(rede) == -1) continue;
			if(lisAltF != NULL)
			  {if((lisAltF->Count > 0)&&(lisAltF->IndexOf(resAlt) == -1)) continue;
			  }
		  }

		s90 = StrToDouble(item->SubItems->Strings[1]);//SAIFI 90
		s70 = StrToDouble(item->SubItems->Strings[2]);//SAIFI 70
		s40 = StrToDouble(item->SubItems->Strings[3]);//SAIFI 40
		sIn = StrToDouble(item->SubItems->Strings[4]);//Interr.

		nomeAlt = "";
		nomeAlt = "A"+IntToStr(ialt);
		nomeRede = "R"+IntToStr(irede);

		serie90->Add(s90, nomeAlt+"_"+nomeRede);
		serie70->Add(s70, nomeAlt+"_"+nomeRede);
		serie40->Add(s40, nomeAlt+"_"+nomeRede);
		serieInt->Add(sIn, nomeAlt+"_"+nomeRede);

		}

	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::IniciaChtRede1(void)
	{
	VTRedes       *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede        *rede;
	TChartSeries *serie90, *serie70, *serie40, *serieInt;
	TListItem    *item;
	TList        *lisRede;
	double  s90, s70, s40, sIn;
	VTResAlternativa *resAlt;
	AnsiString       nomeAlt, nomeRede;
	int irede, ialt;

	serie90 = chtEvRede->Series[0]; serie90->Clear();
	serie70 = chtEvRede->Series[1]; serie70->Clear();
	serie40 = chtEvRede->Series[2]; serie40->Clear();
	serieInt= chtEvRede->Series[3]; serieInt->Clear();

	for(int nr = 0; nr < LViewIndTotal->Items->Count; nr++)
		{
		item = LViewIndTotal->Items->Item[nr];
		if(!item->Checked) continue;
		resAlt = (VTResAlternativa*)item->SubItems->Objects[0];
		ialt   = lisResAlt->IndexOf(resAlt);
		s90 = StrToDouble(item->SubItems->Strings[1]);//SAIFI 90
		s70 = StrToDouble(item->SubItems->Strings[2]);//SAIFI 70
		s40 = StrToDouble(item->SubItems->Strings[3]);//SAIFI 40
		sIn = StrToDouble(item->SubItems->Strings[4]);//Interr.
		nomeAlt = "";
		nomeAlt = "A"+IntToStr(ialt);
		serie90->Add(s90, nomeAlt);
		serie70->Add(s70, nomeAlt);
		serie40->Add(s40, nomeAlt);
		serieInt->Add(sIn, nomeAlt);
		}


	lisRede = redes->LisRede();
	for(int nr = 0; nr < LViewIndRede->Items->Count; nr++)
		{
		item = LViewIndRede->Items->Item[nr];
		if(!item->Checked) continue;
		rede   = (VTRede*)item->SubItems->Objects[0];
		resAlt = (VTResAlternativa*)item->SubItems->Objects[1];
		irede  = lisRede->IndexOf(rede);
		ialt   = lisResAlt->IndexOf(resAlt);

		s90 = StrToDouble(item->SubItems->Strings[1]);//SAIFI 90
		s70 = StrToDouble(item->SubItems->Strings[2]);//SAIFI 70
		s40 = StrToDouble(item->SubItems->Strings[3]);//SAIFI 40
		sIn = StrToDouble(item->SubItems->Strings[4]);//Interr.

		nomeAlt = "";
		nomeAlt = "A"+IntToStr(ialt);
		nomeRede = "R"+IntToStr(irede);

		serie90->Add(s90, nomeAlt+"_"+nomeRede);
		serie70->Add(s70, nomeAlt+"_"+nomeRede);
		serie40->Add(s40, nomeAlt+"_"+nomeRede);
		serieInt->Add(sIn, nomeAlt+"_"+nomeRede);

		}

	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::ActionExibeAmostraExecute(TObject *Sender)
	{
	//variáveis locais
	VTBarraVTCD   *pbarV;
	VTCalculaVTCD *calcVTCD = (VTCalculaVTCD*)apl->GetObject(__classid(VTCalculaVTCD));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	double fatAux, fatAm, compRede;
	TList  *lisAux;

	//valor mínimo de 10m e máximo igual a 10% do compr. da rede
	fatAux = 1. - (double)(tbAmostra->Position) /(double)(tbAmostra->Max);
	//Atualiza o comprimento da rede e fator amostral
	compRede = CompRedes();
	fatAm  = 10. + fatAux * ((compRede /100)/ 50. - 10.);

	//Inicia lista de falhas por barra
	calcVTCD->IniciaFalhas(fatAm);//StrToDouble(edtAmostra->Text));

	lisAux = new TList();
	for(int n = 0; n < calcVTCD->LisBarraVTCD->Count; n++)
		{
		pbarV  = (VTBarraVTCD*)calcVTCD->LisBarraVTCD->Items[n];
		if(pbarV->Na == 0) continue;
		lisAux->Add(pbarV->Barra);
		}
	grafico->Moldura(lisAux);
	delete lisAux;

	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::ActionExecutarExecute(TObject *Sender)
	{
	//variáveis locais
	VTCalculaVTCD *calcVTCD = (VTCalculaVTCD*)apl->GetObject(__classid(VTCalculaVTCD));
   VTGrafico     *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TTemaVTCD *tema = (TTemaVTCD*)apl->GetObject(__classid(TTemaVTCD));

	//Reinicia listView
	LViewIndicZera();//ALTERA
	tbsIndicVTCD->Enabled = false;
	//Executa os curtos para as alternativas selecionadas
	if(!ExecutaAlternativas()) return;
	tbsIndicVTCD->TabVisible = true;

	tbsIndicVTCD->Enabled = true;

	//Inicia mapa temático
	if(CBoxAf->ItemIndex == -1) return;
	if(CBoxAf->ItemIndex == 0) calcVTCD->NivelAf = 0.4;
	else {if (CBoxAf->ItemIndex == 1) calcVTCD->NivelAf = 0.7;
			else {if(CBoxAf->ItemIndex == 2) calcVTCD->NivelAf = 0.9;
					else calcVTCD->NivelAf = 0.1;
				  }
		  }

	ChkMapa->Enabled = true;
	CBoxAf->Enabled  = true;
	tema->flagExt = IniciaVExtr(calcVTCD->NivelAf, tema->vmaxExt, tema->vminExt);
	lblMin->Visible = true;
	lblMin->Caption = "Min:"+ DoubleToStr("%6.2f", tema->vminExt) + " eventos/ano";
	pnMin->Visible = true;
	pnMin->Color    = tema->DegradeCinza(0.);
	lblMax->Visible = true;
	lblMax->Caption = "Max:"+ DoubleToStr("%6.2f", tema->vmaxExt) + " eventos/ano";
	pnMax->Visible = true;
	pnMax->Color    = tema->DegradeCinza(1.);
	//calcVTCD->NivelAf = StrToDouble(CBoxAf->Text);
	if (ChkMapa->Checked)
      {
		IniciaMapaTematico();
		}
	grafico->Moldura();

	if(formEstudo != NULL)
	  {
	  LViewAlt->ItemIndex = 0;
	  LViewAlt->ItemFocused = LViewAlt->Items->Item[0];
	  LViewAltClick(NULL);
	  }
	}

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::ActionSelEqptoExecute(TObject *Sender)
	{
	//variáveis locais
	TTemaVTCD *tema = (TTemaVTCD*)apl->GetObject(__classid(TTemaVTCD));
	VTResAlternativa *resAlt;
	TList         *lisEqpto = (TList*) Sender;
	VTEqpto       *eqpto;
	VTBarraVTCD   *pbarV;
	TformChart    *formChart;
	AnsiString    titulo, nome;
	TFormResBar *formResBar;


	resAlt = tema->resAlt;
	if(resAlt == NULL) return;

   //proteção
   if (lisEqpto == NULL) return;
   //Busca eqpto tipo barra
   eqpto = NULL;
   for(int n = 0; n < lisEqpto->Count; n++)
      {
      eqpto = (VTEqpto*)lisEqpto->Items[n];
      if(eqpto->Tipo() == eqptoBARRA) break;
      eqpto = NULL;
      }
	if(eqpto == NULL) return;


   //Busca a barra em lisBarraVTCD
	if((pbarV = resAlt->BarVTCD[(VTBarra*)eqpto]) == NULL) return;
	nome = "Rede carregada";
	if(resAlt->Alt != NULL) nome = resAlt->Alt->Codigo;
	formResBar = new TFormResBar(this, apl, pbarV, nome);
	formResBar->Show();
	}

//---------------------------------------------------------------------------
VTResAlternativa* __fastcall TFormAnaliseVTCD::BuscaAlternativa(VTAlternativa* alt, VTObra* obra)
   {
   VTResAlternativa* resAlt = NULL;
   for(int n = 0; n < lisResAlt->Count; n++)
      {
      resAlt = (VTResAlternativa*)lisResAlt->Items[n];
      if((resAlt->Alt == alt)&&(resAlt->Obra == obra)) break;

      resAlt = NULL;
      }
   return(resAlt);
   }
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::CBoxAfChange(TObject *Sender)
   {
	//variáveis locais
   VTCalculaVTCD *calcVTCD = (VTCalculaVTCD*)apl->GetObject(__classid(VTCalculaVTCD));
   TTemaVTCD *tema = (TTemaVTCD*)apl->GetObject(__classid(TTemaVTCD));

	//Inicia mapa temático
	if(CBoxAf->ItemIndex == -1) return;
	if(CBoxAf->ItemIndex == 0) calcVTCD->NivelAf = 0.4;
	else {if (CBoxAf->ItemIndex == 1) calcVTCD->NivelAf = 0.7;
			else {if(CBoxAf->ItemIndex == 2) calcVTCD->NivelAf = 0.9;
					else calcVTCD->NivelAf = 0.1;
				  }
		  }
	tema->flagExt = IniciaVExtr(calcVTCD->NivelAf, tema->vmaxExt, tema->vminExt);
	lblMin->Caption = "Min:"+ DoubleToStr("%6.2f", tema->vminExt) + " eventos/ano";
	pnMin->Color    = tema->DegradeCinza(0.);
	lblMax->Caption = "Max:"+ DoubleToStr("%6.2f", tema->vmaxExt) + " eventos/ano";
	pnMax->Color    = tema->DegradeCinza(1.);

   if (tema->resAlt != NULL)
      {
      if (ChkMapa->Checked)
         {
         IniciaMapaTematico();
         }
      }
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::CBoxPatamarInicia(void)
   {
	/*
	//variáveis locais
	VTPatamar   *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	//reinicia CBoxPatamar
	CBoxPatamar->Clear();
	//insere Patamares
	for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
		{
		patamar = patamares->Patamar[ind_pat];
		CBoxPatamar->Items->AddObject(patamar->Nome, patamar);
		}
	//pré-seleciona primeiro patamar
	if (CBoxPatamar->Items->Count > 0) CBoxPatamar->ItemIndex = 0;
	*/
	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::ChecaItens(void)
	{
	TListItem *item;

	for(int n = 0; n < LViewIndTotal->Items->Count; n++)
		{
		item = LViewIndTotal->Items->Item[n];
		item->Checked = true;
		}

	for(int n = 0; n < LViewIndRede->Items->Count; n++)
		{
		item = LViewIndRede->Items->Item[n];
		item->Checked = false;
		}

	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::ChkMapaClick(TObject *Sender)
   {
	//variáveis locais
   VTCalculaVTCD     *calcVTCD = (VTCalculaVTCD*)apl->GetObject(__classid(VTCalculaVTCD));
	TTemaVTCD *tema = (TTemaVTCD*)apl->GetObject(__classid(TTemaVTCD));
	VTGrafico         *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//Inicia mapa temático
	if(CBoxAf->ItemIndex == -1) return;
	if(CBoxAf->ItemIndex == 0) calcVTCD->NivelAf = 0.4;
	else {if (CBoxAf->ItemIndex == 1) calcVTCD->NivelAf = 0.7;
			else {if(CBoxAf->ItemIndex == 2) calcVTCD->NivelAf = 0.9;
					else calcVTCD->NivelAf = 0.1;
				  }
		  }

	//Inicia valores máximo e mínimo
	tema->flagExt = IniciaVExtr(calcVTCD->NivelAf, tema->vmaxExt, tema->vminExt);
	lblMin->Caption = "Min:"+ DoubleToStr("%6.2f", tema->vminExt) + " eventos/ano";
	pnMin->Color    = tema->DegradeCinza(0.);
	lblMax->Caption = "Max:"+ DoubleToStr("%6.2f", tema->vmaxExt) + " eventos/ano";
	pnMax->Color    = tema->DegradeCinza(1.);

	if (tema->resAlt != NULL)
		{
		if (ChkMapa->Checked == true)
			{
			IniciaMapaTematico();
			}
		else
			{
			grafico->MapaTematicoEnabled = false;
			}
		}
	}

//---------------------------------------------------------------------------
bool __fastcall TFormAnaliseVTCD::IniciaVExtr(double nivelAf, double &Vmax, double &Vmin)
	{
	VTResAlternativa* resAlt;
	VTBarraVTCD*  pbarV;
	double        v00, valor;
	bool          flagIni = true;

	for(int n = 0; n < lisResAlt->Count; n++)
		{
		resAlt = (VTResAlternativa*)lisResAlt->Items[n];
		for(int nb = 0; nb < resAlt->LisBarVTCD->Count; nb++)
			{
			pbarV = (VTBarraVTCD*)resAlt->LisBarVTCD->Items[nb];
			v00 = pbarV->Naf[NUM_MAX_DEF][0.1];
			if(IsDoubleZero(nivelAf - 0.1)) valor = v00;
			else valor = pbarV->Naf[NUM_MAX_DEF][nivelAf] - v00;
			if(flagIni) {Vmax = valor;
							 Vmin = valor;
							 flagIni = false;
							 continue;
							}
			if(valor > Vmax) Vmax = valor;
			if(valor < Vmin) Vmin = valor;
			}
		}
	return(true);
   }
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::CheckBox1Click(TObject *Sender)
	{
	VTGrafico         *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes      *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTResAlternativa *resAlt;
	TList        *lisBarra;
	VTBarra      *pbarra;
	double       af;
	TList        *lisAux;
	VTBarraVTCD   *pbarV;
	VTCalculaVTCD *calcVTCD = (VTCalculaVTCD*)apl->GetObject(__classid(VTCalculaVTCD));
	TTemaVTCD *tema = (TTemaVTCD*)apl->GetObject(__classid(TTemaVTCD));

	resAlt = tema->resAlt;
	if(resAlt == NULL) return;

	lisBarra = redes->LisBarra();

	lisAux = new TList();
	for(int nb = 0; nb < lisBarra->Count; nb++)
		{
		pbarra = (VTBarra*)lisBarra->Items[nb];
		pbarV = resAlt->BarVTCD[pbarra];
		if (pbarV == NULL) continue;
		af = pbarV->NafP[NUM_MAX_DEF][calcVTCD->NivelAf];
		if(af < 0.5) continue;
		lisAux->Add(pbarra);
		}

	grafico->DestacaEqpto(lisAux, clRed, 3);
	delete lisAux;
	
	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::CLBoxPatamarInicia(void)
	{
	//variáveis locais
	int         index;
	VTPatamar   *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	//reinicia CLBoxPatamar
	CLBoxPatamar->Clear();
   //exibe Patamar em CLBoxPatamar
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      patamar = (VTPatamar*)patamares->Patamar[ind_pat];
      index = CLBoxPatamar->Items->AddObject(patamar->Nome, patamar);
      CLBoxPatamar->Checked[index] = patamar->Enabled;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormAnaliseVTCD::ConfigRede(VTAlternativa *alt, VTObra *obra)
   {
   TList *lisAlt, *lisObras;
   VTEstudo       *estudo = ExisteEstudo();
   //Verifica se existe aternativa
   lisAlt = estudo->LisAlternativa();
	if(lisAlt->IndexOf(alt) == -1) return(false);
   if(obra != NULL)
     {lisObras = alt->Obras->LisObra();
      if(lisObras->IndexOf(obra) == -1) return(false);
     }

   estudo->DefineAlternativaAtiva(alt, obra);
   return(true);
   }
   
//---------------------------------------------------------------------------
double __fastcall TFormAnaliseVTCD::CompRedes(void)
	{
	VTRedes      *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede       *rede;
	TList        *lisRede;
	double       comp = 0.0;

	lisRede = redes->LisRede();
	for(int n = 0; n < lisRede->Count; n++)
		{
		rede = (VTRede*)lisRede->Items[n];
		comp+= CompRede(rede);
		}

	return(comp);
   }
//---------------------------------------------------------------------------
double __fastcall TFormAnaliseVTCD::CompRede(VTRede *rede)
	{
	VTEsquematico *esquematico;
	VTLigacao *pliga;
	TList     *lisLiga;
	double    comp;
	bool      flag_Esq;

	if(!rede->Carregada) return(0.0);
	//Verifica se a rede tem coordenadas utm
	esquematico = DLL_NewObjEsquematico(apl);
	flag_Esq = esquematico->ExisteBarraSemCoordUtm(rede->Redes);
	delete esquematico;
	lisLiga = rede->LisLigacao();
	comp    = 0.0;
	for(int nl = 0; nl < lisLiga->Count; nl++)
			{
			pliga = (VTLigacao*)lisLiga->Items[nl];
			if(pliga->Tipo() != eqptoTRECHO)  continue;
			if(flag_Esq)
			  comp += ((VTTrecho*)pliga)->Comprimento_m * 100;
			else
			  comp += pliga->ComprimentoUtm_cm();
			}

	return(comp);
	}
//---------------------------------------------------------------------------
bool __fastcall TFormAnaliseVTCD::ExecutaAlternativas(void)
	{
	//variáveis locais
	VTCalculaVTCD *calcVTCD = (VTCalculaVTCD*)apl->GetObject(__classid(VTCalculaVTCD));
	TListItem     *item;
	VTPatamares   *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	DAT_REDE      *datRede;
	double        compRede, fatAm, fatAux;
	VTProgresso   *prog;
	VTResAlternativa *resAlt;
	TTemaVTCD *tema = (TTemaVTCD*)apl->GetObject(__classid(TTemaVTCD));
	bool          flagExe = false;

	prog = DLL_NewObjProgresso(this, this->Parent);
	//Seta o patamar de cálculo
	//calcVTCD->Pat = CBoxPatamar->ItemIndex;
	calcVTCD->LisPatamares->Clear();
	for(int npat = 0; npat < CLBoxPatamar->Items->Count; npat++)
		{
		if(!CLBoxPatamar->Checked[npat]) continue;
		calcVTCD->LisPatamares->Add(CLBoxPatamar->Items->Objects[npat]);
		}
	if(calcVTCD->LisPatamares->Count == 0) return(false);
	calcVTCD->Pat = (patamares->LisPatamar())->IndexOf(calcVTCD->LisPatamares->First());

	//Seta o tipo de defeito
	//calcVTCD->TipoDef = CBoxDef->ItemIndex;
	for(int n = 1; n < sgridTaxasDefeito->RowCount; n++)
		calcVTCD->PDef[n - 1] = StrToDouble(sgridTaxasDefeito->Cells[1][n]) / 100.;


	//Gera distancia mínima entre barras amostradas
	//valor mínimo de 10m e máximo igual a 10% do compr. da rede
	fatAux = 1. - (double)(tbAmostra->Position) /(double)(tbAmostra->Max);


	//Reinicia lista de resultados de alternativas
	LimpaTList(lisResAlt);


	if(formEstudo != NULL)
	  {
	  for(int nalt = 0; nalt < LViewAlt->Items->Count; nalt++)
		  {
		  item = LViewAlt->Items->Item[nalt];
		  if(!item->Data) continue;
		  if(!item->Checked) continue;
		  altFoc = (VTAlternativa*)item->Data;
		  obraFoc= NULL;
		  if(item->SubItems->Objects[0] != NULL)
			  obraFoc = (VTObra*)item->SubItems->Objects[0];
		  if(!ConfigRede(altFoc, obraFoc)) continue;

		  //Atualiza o comprimento da rede e fator amostral
		  compRede = CompRedes();
		  fatAm  = 10. + fatAux * ((compRede /100)/ 50. - 10.);

		  //Inicia lista de falhas por barra
		  calcVTCD->IniciaFalhas(fatAm);//StrToDouble(edtAmostra->Text));
		  //Executa curtos por barra e acumula resultados
		  calcVTCD->ExecutaCurtos(prog);
		  flagExe = true;
		  //Cria nova classe para guardar resultados
		  lisResAlt->Add(resAlt = NewObjResAlternativa(altFoc, obraFoc));
		  resAlt->LisBarVTCD->Assign(calcVTCD->LisBarraVTCD, laOr);
		  //Mostra relatorio com resultados por barra
		  LViewBarrasInicia();
		  //Limpa a lista de resultados em calcVTCD
		  calcVTCD->LisBarraVTCD->Clear();
		  }
	  //Mostra relatório com indíces agregados, pontos de carga e cortes de redes equiv.
	  LViewIndAuxInicia();
	  }
	else
	  {
	  altFoc = NULL;
	  obraFoc= NULL;
	  //Inicia lista de falhas por barra
	  //Atualiza o comprimento da rede e fator amostral
	  compRede = CompRedes();
	  fatAm  = 10. + fatAux * ((compRede /100)/ 50. - 10.);
	  calcVTCD->IniciaFalhas(fatAm);//StrToDouble(edtAmostra->Text));
	  MostraBarrasAmostra();
	  //Executa curtos por barra e acumula resultados
	  calcVTCD->ExecutaCurtos(prog);
	  //Cria nova classe para guardar resultados
	  lisResAlt->Add(resAlt = NewObjResAlternativa(altFoc, obraFoc));
	  //resAlt->LisBarVTCD->Assign(calcVTCD->Lis)
	  resAlt->LisBarVTCD->Assign(calcVTCD->LisBarraVTCD, laOr);
	  //Mostra relatorio com resultados por barra
	  LViewBarrasInicia();
	  //Mostra relatório com indíces agregados, pontos de carga e cortes de redes equiv.
	  LViewIndAuxInicia();
	  //Limpa a lista de resultados em calcVTCD
	  calcVTCD->LisBarraVTCD->Clear();
	  flagExe = true;
	  }
	//Inicia Lview´s dos indicadores com todas as alternativas e redes
	LViewIndicInicia1(NULL, NULL, 0);
	LViewIndicInicia1(NULL, NULL, 1);
	LViewIndicInicia1(NULL, NULL, 2);
	//Checagem inicial dos itens
	ChecaItens();
   //Reinicia listView
	LViewIndRedeReinicia();

	//IniciaChtRede(NULL, NULL);

	tema->resAlt = resAlt;

	delete prog;
	return(flagExe);
	}

//---------------------------------------------------------------------------
VTEstudo* __fastcall TFormAnaliseVTCD::ExisteEstudo(void)
	{
	//variáveis locais
   VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

   return(planejamento->ExisteEstudo());
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::FormClose(TObject *Sender, TCloseAction &Action)
	{
   //destrói o Forms
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::FormIniciaPosicao(void)
	{
	ActionAlignLeftExecute(NULL);
	}

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::IniciaMapaTematico(void)
	{
	//variáveis locais
	int       ind_pat;
	VTPatamar *patamar;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTCalculaVTCD *calcVTCD = (VTCalculaVTCD*)apl->GetObject(__classid(VTCalculaVTCD));
	TTemaVTCD *tema = (TTemaVTCD*)apl->GetObject(__classid(TTemaVTCD));


	//determina Patamar selecionado
	if (calcVTCD->Pat < 0)
		{//define opção "todos patamares"
		ind_pat = -1;
		}
	else
		{
		ind_pat = calcVTCD->Pat;
		}

	//Define alternativa de referencia
	VTResAlternativa* resAlt;
	tema->resAltRef = NULL;
	if(chkDif->Checked)
	{
	for(int n = 0; n < lisResAlt->Count; n++)
		{
		resAlt = (VTResAlternativa*)lisResAlt->Items[n];
		if(!resAlt->Alt) continue;
		if(resAlt->Alt->Codigo.AnsiCompare("[Rede Original]") != 0) continue;
		tema->resAltRef = resAlt;
		break;
		}
	}

	//atualiza cores das celulas
	tema->Reinicia(ind_pat);
	//atualiza o gráfico: exibe mapa temático
	grafico->MapaTematicoEnabled = true;
	}

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::InsereAltObra(VTAlternativa *alt, VTObra *obra)
	{
	TListItem* item;
	for(int n = 0; n < LViewAlt->Items->Count; n++)
		{
		item = LViewAlt->Items->Item[n];
		if(item->Data != alt) continue;
		if(item->SubItems->Objects[0] != obra) continue;
		return;
		}

	LViewAlt->AddItem(alt->Codigo, alt);
	item =LViewAlt->Items->Item[LViewAlt->Items->Count -1];
	item->Checked = true;
   if(obra != NULL)
     {item->SubItems->AddObject(obra->Nome, obra);
      item->SubItems->Add(IntToStr(obra->AnoInicio));
     }
   else
     {item->SubItems->Add("");
      item->SubItems->Add("");
     }
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LimpaTStringGrid(TStringGrid *StringGrid)
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
void __fastcall TFormAnaliseVTCD::LViewAltInicia(void)
	{
	//variáveis locais
	TList          *lisALTERNATIVA;
	TListItem      *Item;
	VTAlternativa  *alternativa;
	VTAlternativa  *altPadrao;
	VTObra         *obra;
	VTEstudo       *estudo = ExisteEstudo();
	TList          *lisObras;

	//reinicia LView
	LViewAlt->Clear();
	//proteção
	if (estudo == NULL) return;
	//loop p/ todas Alternativas do Estudo
	lisALTERNATIVA = estudo->LisAlternativa();
	altPadrao = estudo->AlternativaPadrao;
	if(altPadrao != NULL) InsereAltObra(altPadrao, NULL);

	for (int n = 0; n < lisALTERNATIVA->Count; n++)
		{
		alternativa = (VTAlternativa*)lisALTERNATIVA->Items[n];
		if(alternativa == altPadrao) continue;
		InsereAltObra(alternativa, NULL);
		}
   }
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewAltClick(TObject *Sender)
	{
	//variáveis locais
   TTemaVTCD *tema = (TTemaVTCD*)apl->GetObject(__classid(TTemaVTCD));
   VTResAlternativa  *resAlt;
   TListItem         *item;
   VTAlternativa     *alt  = NULL;
   VTObra            *obra = NULL;
   int               itemIndex;

   itemIndex = LViewAlt->ItemIndex;
   if(itemIndex < 0) return;

   item = LViewAlt->ItemFocused;
   if(item->Data == NULL) return;

   alt = (VTAlternativa*)item->Data;
   if(item->SubItems->Objects[0] != NULL)
       obra = (VTObra*)item->SubItems->Objects[0];

   if(!ConfigRede(alt, obra)) item->Delete();

   for (int i = 0; i < lisResAlt->Count; i++)
		{
      resAlt = (VTResAlternativa*)lisResAlt->Items[i];
      if (resAlt->Alt != alt) continue;
      if (resAlt->Obra != obra) continue;

		tema->resAlt = resAlt;
      IniciaMapaTematico();
      }
   
   }
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewAltDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
   {
   VTEstudo       *estudo = ExisteEstudo();
   VTAlternativa  *alt;
   VTObra         *obra;
   TTreeNode *node;
   TTreeView *tviewAlt;
	AnsiString class_name;

   if(estudo == NULL) return;

   class_name = Source->ClassName();
   if(class_name.AnsiCompare("TTreeView") != 0) return;

   tviewAlt = (TTreeView*)Source;
	node = tviewAlt->Selected;

   alt = TreeViewAlternativaSelecionada(node);
   if(alt == NULL) return;
   obra= TreeViewObraSelecionada(node);
	InsereAltObra(alt, obra);
	}

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewAltDragOver(TObject *Sender,
		TObject *Source, int X, int Y, TDragState State, bool &Accept)
	{
	Accept = true;
	return;
	}

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewBarrasClick(TObject *Sender)
	{
	VTGrafico    *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TListItem    *item;
	VTBarra      *pbar;

	if(LViewBarras->ItemIndex <= 0) return;
	item = LViewBarras->Items->Item[LViewBarras->ItemIndex];
	pbar = (VTBarra*)item->SubItems->Objects[0];
	grafico->Moldura(pbar);
	}

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewBarrasInicia(void)
	{
	VTCalculaVTCD *calcVTCD = (VTCalculaVTCD*)apl->GetObject(__classid(VTCalculaVTCD));
	VTBarraVTCD  *pbarV;
	TListItem    *item;
	TList        *lisBarraVTCD = calcVTCD->LisBarraVTCD;


	LViewBarras->Clear();
	for(int n = 0; n < lisBarraVTCD->Count; n++)
		{
		pbarV = (VTBarraVTCD*)lisBarraVTCD->Items[n];
		item = LViewBarras->Items->Add();
		item->Caption = pbarV->Barra->Codigo;
		item->SubItems->Add(IntToStr(pbarV->Barra->Id));
		item->SubItems->Objects[0] = pbarV->Barra;

		item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[NUM_MAX_DEF][0.9]));
		item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Naf[NUM_MAX_DEF][0.1]));
		item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Hist[NUM_MAX_DEF]->VMin));
		item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Hist[NUM_MAX_DEF]->Media));
		item->SubItems->Add(DoubleToStr("%10.3f", pbarV->Hist[NUM_MAX_DEF]->VMax));
		}

	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndCargaClick(TObject *Sender)
	{
	VTGrafico    *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TListItem    *item;
	VTCarga      *pcarga;

	if(LViewIndCarga->ItemIndex < 0) return;
	item = LViewIndCarga->Items->Item[LViewIndCarga->ItemIndex];
	pcarga = (VTCarga*)item->SubItems->Objects[0];
	grafico->Moldura(pcarga);


	}

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndCargaDblClick(TObject *Sender)
	{
	//TTemaVTCD *tema = (TTemaVTCD*)apl->GetObject(__classid(TTemaVTCD));
	TListItem    *item;
	VTBarraVTCD   *pbarV;
	VTCarga      *pcarga;
	VTResAlternativa *resAlt;
	TFormResBar *formResBar;
	AnsiString    titulo, nome;

	//resAlt = tema->resAlt;

	if(LViewIndCarga->ItemIndex < 0) return;
	item = LViewIndCarga->Items->Item[LViewIndCarga->ItemIndex];
	pcarga = (VTCarga*)item->SubItems->Objects[0];
	resAlt = (VTResAlternativa*)item->SubItems->Objects[2];
	if(resAlt == NULL) return;

	//Busca a barra em lisBarraVTCD
	if((pbarV = resAlt->BarVTCD[pcarga->pbarra]) == NULL) return;

	nome = "Rede carregada";
	if(resAlt->Alt != NULL) nome = resAlt->Alt->Codigo;
	formResBar = new TFormResBar(this, apl, pbarV, nome);
	formResBar->Show();

	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndCargaColumnClick(TObject *Sender,
		TListColumn *Column)
	{
	//verifica se selecionou a mesma coluna
	if (lviewIndCarga.ColumnIndex == Column->Index)
		{//inverte ordem
		lviewIndCarga.OrdemCrescente  = ! lviewIndCarga.OrdemCrescente;
		}
	else
		{//redefine índice da coluna usada p/ ordenação
		lviewIndCarga.ColumnIndex = Column->Index;
		}
	//força reordenação
	((TCustomListView *)Sender)->AlphaSort();//LViewIndCarga->AlphaSort();
	}

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndCargaCompare(TObject *Sender,
		TListItem *Item1, TListItem *Item2, int Data, int &Compare)
	{
	//variáveis locais
	int  index = lviewIndCarga.ColumnIndex-1;
	AnsiString aux1, aux2;
	double val1, val2;

	switch(lviewIndCarga.ColumnIndex)
		{
		case 0:  //caption
			Compare = CompareText(Item1->Caption,Item2->Caption);
			break;               
		case 1:  //caption
			aux1    = Item1->SubItems->Strings[index];
			aux2    = Item2->SubItems->Strings[index];
			Compare = CompareText(aux1,aux2);
			break;
		case 7:  //caption
			aux1    = Item1->SubItems->Strings[index];
			aux2    = Item2->SubItems->Strings[index];
			Compare = CompareText(aux1,aux2);
			break;
		case 8:  //caption
			aux1    = Item1->SubItems->Strings[index];
			aux2    = Item2->SubItems->Strings[index];
			Compare = CompareText(aux1,aux2);
			break;
		default: //substring
			val1 = StrToDouble(Item1->SubItems->Strings[index]);
			val2 = StrToDouble(Item2->SubItems->Strings[index]);
			if(val1 < val2) Compare = -1;
			else if(val1 > val2) Compare = 1;
			else Compare = 0;
		}
	//verifica se ordem crescente/decrescente
	if (! lviewIndCarga.OrdemCrescente) Compare = - Compare;
	}

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndicZera(void)
	{
	LViewIndTotal->Clear();
	LViewIndCarga->Clear();
	LViewIndRede->Clear();
	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndAuxInicia(void)  //ALTERA
	{
	VTResAlternativa *resAlt;
	VTRedes       *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede        *rede;
	VTBarraVTCD   *pbarV;
	VTCarga       *pcarga;
	TListItem     *item;
	//TList         *lisBarraVTCD = calcVTCD->LisBarraVTCD;
	TList         *lisCargas;
	TList         *lisRede;
	double        v90, v70, v40, v00, vac90, vac70, vac40, vac00, vacc90, vacc70, vacc40, vacc00;
	int           ncacc;
	AnsiString    nomeAlt, nomeObra, anoObra;

	//Reinicia list views
	LViewIndAuxCarga->Clear();
	LViewIndAuxRede->Clear();
	LViewIndAuxTotal->Clear();

	lisCargas = new TList();
	for(int nalt = 0; nalt < lisResAlt->Count; nalt++)
		{
		resAlt = (VTResAlternativa*)lisResAlt->Items[nalt];
		ConfigRede(resAlt->Alt, resAlt->Obra);
		//Obra/ano
		nomeAlt = nomeObra = anoObra = "";
		if(resAlt->Alt != NULL) nomeAlt = resAlt->Alt->Codigo;
		if(resAlt->Obra != NULL)
			{
			nomeObra = resAlt->Obra->Nome;
			anoObra  = IntToStr(resAlt->Obra->AnoInicio);
			}
		vacc90= vacc70= vacc40= vacc00 = 0.0;
		ncacc = 0;
		lisRede = redes->LisRede();
		for(int nr = 0; nr < lisRede->Count; nr++)
			{
			rede = (VTRede*)lisRede->Items[nr];
			if(!rede->Carregada) continue;
			lisCargas->Clear();
			rede->LisEqbar(lisCargas, eqptoCARGA);
			if(lisCargas->Count == 0) continue;


			vac90 = vac70 = vac40 = vac00 = 0.0;
			for(int nc = 0; nc < lisCargas->Count; nc++)
				{
				pcarga = (VTCarga*)lisCargas->Items[nc];
				pbarV = resAlt->BarVTCD[pcarga->pbarra];
				vac00 += (v00 = pbarV->Naf[NUM_MAX_DEF][0.1]);
				vac90 += (v90 = pbarV->Naf[NUM_MAX_DEF][0.9] - v00);
				vac70 += (v70 = pbarV->Naf[NUM_MAX_DEF][0.7] - v00);
				vac40 += (v40 = pbarV->Naf[NUM_MAX_DEF][0.4] - v00);
				//Adiciona item de carga
				item = LViewIndAuxCarga->Items->Add();
				item->Caption = nomeAlt;
				item->SubItems->Add(nomeAlt);
				item->SubItems->Add(DoubleToStr("%10.3f", v90));
				item->SubItems->Add(DoubleToStr("%10.3f", v70));
				item->SubItems->Add(DoubleToStr("%10.3f", v40));
				item->SubItems->Add(DoubleToStr("%10.3f", v00));
				item->SubItems->Add(rede->Codigo);
				item->SubItems->Add(pcarga->Codigo);
				item->SubItems->Add(IntToStr(1));   //1 consumidor por ponto de carga
				item->SubItems->Objects[0] = pcarga;
				item->SubItems->Objects[1] = rede;
				item->SubItems->Objects[2] = resAlt;
				}

			//Adiciona item de rede
			item = LViewIndAuxRede->Items->Add();
			item->Data = rede;
			item->Caption =  "R"+IntToStr(nr);;//nomeAlt;
			item->SubItems->Add(nomeAlt);
			vacc90+= vac90;
			vacc70+= vac70;
			vacc40+= vac40;
			vacc00+= vac00;
			ncacc += lisCargas->Count;

			if(lisCargas->Count == 0) continue;
			vac90 /= (double)lisCargas->Count;
			vac70 /= (double)lisCargas->Count;
			vac40 /= (double)lisCargas->Count;
			vac00 /= (double)lisCargas->Count;

			item->SubItems->Add(DoubleToStr("%10.3f", vac90));
			item->SubItems->Add(DoubleToStr("%10.3f", vac70));
			item->SubItems->Add(DoubleToStr("%10.3f", vac40));
			item->SubItems->Add(DoubleToStr("%10.3f", vac00));
			item->SubItems->Add(rede->Codigo);

			item->SubItems->Add(IntToStr(lisCargas->Count));
			item->SubItems->Objects[0] = rede;
			item->SubItems->Objects[1] = resAlt;
			}

		//Adiciona item da alternativa
		item = LViewIndAuxTotal->Items->Add();
		item->Caption = "A"+IntToStr(nalt);//nomeAlt;
		item->SubItems->Add(nomeAlt);

		if(ncacc > 0)
			{vacc90 /= (double)ncacc;
			vacc70 /= (double)ncacc;
			vacc40 /= (double)ncacc;
			vacc00 /= (double)ncacc;
			}
		else
			{vacc90 = vacc70 = vacc40 = vac00 = 0.0;
			}

		item->SubItems->Add(DoubleToStr("%10.3f", vacc90));
		item->SubItems->Add(DoubleToStr("%10.3f", vacc70));
		item->SubItems->Add(DoubleToStr("%10.3f", vacc40));
		item->SubItems->Add(DoubleToStr("%10.3f", vacc00));
		item->SubItems->Add(IntToStr(ncacc));
		item->SubItems->Objects[0] = resAlt;
		}
	//força reordenação
	delete lisCargas;
	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndicInicia2(VTRede* rede, VTResAlternativa *resAlt)
	{
	VTRede *rede0;
	VTResAlternativa *resAlt0;
	TListItem *item, *itemN;

	if(rede == NULL) LViewIndCarga->Clear();
	for(int nc = 0; nc < LViewIndAuxCarga->Items->Count; nc++)
		{
		item = LViewIndAuxCarga->Items->Item[nc];
		rede0 = (VTRede*)item->SubItems->Objects[1];
		resAlt0 = (VTResAlternativa*)item->SubItems->Objects[2];
		if((rede != rede0)||(resAlt != resAlt0)) continue;
		itemN = LViewIndCarga->Items->Add();
		itemN->Caption = item->SubItems->Strings[0];
		itemN->SubItems->Add(item->SubItems->Strings[6]);
		itemN->SubItems->Add(item->SubItems->Strings[1]);
		itemN->SubItems->Add(item->SubItems->Strings[2]);
		itemN->SubItems->Add(item->SubItems->Strings[3]);
		itemN->SubItems->Add(item->SubItems->Strings[4]);
		itemN->SubItems->Add(item->SubItems->Strings[7]);
		itemN->SubItems->Add(item->SubItems->Strings[0]);
		itemN->SubItems->Add(item->SubItems->Strings[5]);


		itemN->SubItems->Objects[0] = item->SubItems->Objects[0];
		itemN->SubItems->Objects[1] = rede;
		itemN->SubItems->Objects[2] = resAlt;
		}
	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndicInicia1(TList *lisAltF, TList *lisRedeF, int iaux)
	{
	VTResAlternativa *resAlt;
	VTRede           *rede;
	TListItem *item, *itemN;
	AnsiString nomeAlt, nomeObra, anoObra, codRede, codCarga;
	AnsiString v90, v70, v40;
	VTEstudo       *estudo = ExisteEstudo();


	if(iaux == 0)
	{ LViewIndTotal->Clear();
	for(int n = 0; n < LViewIndAuxTotal->Items->Count; n++)
		{
		item = LViewIndAuxTotal->Items->Item[n];
		resAlt = (VTResAlternativa*)item->SubItems->Objects[0];
		//Adiciona item da alternativa
		itemN = LViewIndTotal->Items->Add();
		itemN->Caption = item->Caption;//item->SubItems->Strings[0];
		itemN->SubItems->Add(item->SubItems->Strings[0]);
		itemN->SubItems->Add(item->SubItems->Strings[1]);
		itemN->SubItems->Add(item->SubItems->Strings[2]);
		itemN->SubItems->Add(item->SubItems->Strings[3]);
		itemN->SubItems->Add(item->SubItems->Strings[4]);
		itemN->SubItems->Add(item->SubItems->Strings[5]);
		itemN->SubItems->Objects[0] = item->SubItems->Objects[0];
		}
	}
	if(iaux == 1)
	{ LViewIndRede->Clear();
	for(int nr = 0; nr < LViewIndAuxRede->Items->Count; nr++)
		{
		item = LViewIndAuxRede->Items->Item[nr];
		rede = (VTRede*)item->SubItems->Objects[0];
		resAlt = (VTResAlternativa*)item->SubItems->Objects[1];

		if(lisAltF != NULL)
		  {if(lisAltF->IndexOf(resAlt) == -1) continue;
		  }


		itemN = LViewIndRede->Items->Add();

		itemN->Checked = true;
		if(lisRedeF != NULL)
		  {itemN->Checked = !(lisRedeF->IndexOf(rede) == -1);
		  }




		itemN->Checked = false;
		itemN->Data = rede;
		itemN->Caption = item->Caption;//item->SubItems->Strings[0];
		itemN->SubItems->Add(item->SubItems->Strings[5]);
		itemN->SubItems->Add(item->SubItems->Strings[1]);
		itemN->SubItems->Add(item->SubItems->Strings[2]);
		itemN->SubItems->Add(item->SubItems->Strings[3]);
		itemN->SubItems->Add(item->SubItems->Strings[4]);
		itemN->SubItems->Add(item->SubItems->Strings[5]);
		itemN->SubItems->Add(item->SubItems->Strings[0]);


		itemN->SubItems->Objects[0] = rede;
		itemN->SubItems->Objects[1] = resAlt;
		}
	}

	if(iaux == 2)
	{
	LViewIndCarga->Clear();
	for(int nc = 0; nc < LViewIndAuxCarga->Items->Count; nc++)
		{
		item = LViewIndAuxCarga->Items->Item[nc];
		rede = (VTRede*)item->SubItems->Objects[1];
		resAlt = (VTResAlternativa*)item->SubItems->Objects[2];
		if(lisAltF != NULL)
		  {if(lisAltF->IndexOf(resAlt) == -1) continue;
		  }
		if(lisRedeF != NULL)
		  {if(lisRedeF->IndexOf(rede) == -1) continue;
		  }

		itemN = LViewIndCarga->Items->Add();
		itemN->Caption = item->SubItems->Strings[0];
		itemN->SubItems->Add(item->SubItems->Strings[6]);
		itemN->SubItems->Add(item->SubItems->Strings[1]);
		itemN->SubItems->Add(item->SubItems->Strings[2]);
		itemN->SubItems->Add(item->SubItems->Strings[3]);
		itemN->SubItems->Add(item->SubItems->Strings[4]);
		itemN->SubItems->Add(item->SubItems->Strings[7]);
		itemN->SubItems->Add(item->SubItems->Strings[0]);
		itemN->SubItems->Add(item->SubItems->Strings[5]);


		itemN->SubItems->Objects[0] = item->SubItems->Objects[0];
		itemN->SubItems->Objects[1] = rede;
		itemN->SubItems->Objects[2] = resAlt;
		}
	}
	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndRedeMouseUp(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
	{
	LViewIndRedeReinicia();
	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndRedeReinicia(void)
	{
	TList *lisAlt, *lisRede;
	TListItem *item, *itemTotal, *itemRede;


	itemTotal = LViewIndTotal->ItemFocused;
	itemRede  = LViewIndRede->ItemFocused;


	lisAlt = new TList();
	lisRede = new TList();

	for(int nalt = 0; nalt < LViewIndTotal->Items->Count; nalt++)
		{
		item = LViewIndTotal->Items->Item[nalt];
		if(!item->Checked) continue;
		lisAlt->Add(item->SubItems->Objects[0]);
		}
	//Cria lista de alternativas selecionadas
	VTRede *rede;
	VTResAlternativa *resAlt;
	LViewIndicInicia2(NULL, NULL);
	for(int nalt = 0; nalt < LViewIndRede->Items->Count; nalt++)
		{
		item = LViewIndRede->Items->Item[nalt];
		if(!item->Checked) continue;
		rede = (VTRede*)item->SubItems->Objects[0];
		resAlt = (VTResAlternativa*)item->SubItems->Objects[1];
		LViewIndicInicia2(rede, resAlt);
		lisRede->Add(item->SubItems->Objects[0]);
		}

	if(lisAlt->Count == 0)
	  {if(lisRede->Count == 0)
		  {
			LViewIndicInicia1(NULL, NULL, 1);
			//LViewIndicInicia1(NULL, NULL, 2);
		  }
		//else
		//  {
		//  LViewIndicInicia1(NULL, lisRede, 2);
		//  }
	  }
	else
	  {if(lisRede->Count == 0)
		  {LViewIndicInicia1(lisAlt, lisRede, 1);
			//LViewIndicInicia1(lisAlt, lisRede, 2);
		  }
	  //	else
		//  {
		//	LViewIndicInicia1(lisAlt, lisRede, 2);
		//  }
	  }


	LViewIndTotal->ItemFocused = itemTotal;
	if(itemRede != NULL)
	   {LViewIndRede->ItemFocused  = itemRede;
		LViewIndRede->ItemFocused->Selected = true;
		}

	IniciaChtRede1();
	delete lisAlt;
	delete lisRede;
	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndTotalMouseUp(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
	{
	TList *lisAlt, *lisRede;
	TListItem *item;


	lisAlt = new TList();
	lisRede = new TList();

	for(int nalt = 0; nalt < LViewIndTotal->Items->Count; nalt++)
		{
		item = LViewIndTotal->Items->Item[nalt];
		if(!item->Checked) continue;
		lisAlt->Add(item->SubItems->Objects[0]);
		}
	//Cria lista de alternativas selecionadas
	for(int nalt = 0; nalt < LViewIndRede->Items->Count; nalt++)
		{
		item = LViewIndRede->Items->Item[nalt];
//ATENÇÃO: código alterado na migração de Rev3.0 p/ Rev3.1
		//if(lisAlt->IndexOf(item->SubItems->Objects[1]) == -1)  continue;
		if(lisAlt->IndexOf(item->SubItems->Objects[1]) == NULL)  continue;
		//if(!item->Checked) continue;
		lisRede->Add(item->SubItems->Objects[0]);
		}
	LViewIndRedeReinicia();

	IniciaChtRede1();
	delete lisAlt;
	delete lisRede;
	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndTotalColumnClick(TObject *Sender,
		TListColumn *Column)
	{
	//verifica se selecionou a mesma coluna
	if (lviewIndTotal.ColumnIndex == Column->Index)
		{//inverte ordem
		lviewIndTotal.OrdemCrescente  = ! lviewIndTotal.OrdemCrescente;
		}
	else
		{//redefine índice da coluna usada p/ ordenação
		lviewIndTotal.ColumnIndex = Column->Index;
		}


	//força reordenação
	LViewIndTotal->AlphaSort();
   }
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndRedeColumnClick(TObject *Sender,
      TListColumn *Column)
   {
	//verifica se selecionou a mesma coluna
   if (lviewIndRede.ColumnIndex == Column->Index)
      {//inverte ordem
      lviewIndRede.OrdemCrescente  = ! lviewIndRede.OrdemCrescente;
      }
   else
      {//redefine índice da coluna usada p/ ordenação
		lviewIndRede.ColumnIndex = Column->Index;
		}
   //força reordenação
   LViewIndRede->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndRedeClick(TObject *Sender)
	{
	VTGrafico    *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TListItem    *item;
	VTRede       *rede;
	VTEqpto      *eqpto;
	TList        *lisTrafo;

	if(LViewIndRede->ItemIndex < 0) return;
	item = LViewIndRede->Items->Item[LViewIndRede->ItemIndex];
	if(item->SubItems->Objects[0] == NULL) return;

	rede = (VTRede*)item->SubItems->Objects[0];
	lisTrafo = new TList();
	rede->LisLigacao(lisTrafo, eqptoTRAFO);
	rede->LisLigacao(lisTrafo, eqptoTRAFO3E);
	rede->LisLigacao(lisTrafo, eqptoTRAFOZZ);

	grafico->Moldura(lisTrafo);
	delete lisTrafo;
	}
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndTotalCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
   {
   //variáveis locais
   int  index = lviewIndTotal.ColumnIndex-1;
	double val1, val2;
	AnsiString aux1, aux2;

	switch(lviewIndTotal.ColumnIndex)
      {
      case 0:  //caption
         Compare = CompareText(Item1->Caption,Item2->Caption);
         break;
		case 1:
			aux1    = Item1->SubItems->Strings[index];
			aux2    = Item2->SubItems->Strings[index];
			Compare = CompareText(aux1,aux2);
			break;
		case 5:
			aux1    = Item1->SubItems->Strings[index];
			aux2    = Item2->SubItems->Strings[index];
			Compare = CompareText(aux1,aux2);
			break;
		case 6:
			aux1    = Item1->SubItems->Strings[index];
			aux2    = Item2->SubItems->Strings[index];
			Compare = CompareText(aux1,aux2);
			break;
		default: //substring
         val1 = StrToDouble(Item1->SubItems->Strings[index]);
			val2 = StrToDouble(Item2->SubItems->Strings[index]);
         if(val1 < val2) Compare = -1;
         else if(val1 > val2) Compare = 1;
         else Compare = 0;
      }
   //verifica se ordem crescente/decrescente
   if (! lviewIndTotal.OrdemCrescente) Compare = - Compare;
   }
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::LViewIndRedeCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
   {
   //variáveis locais
	int  index = lviewIndRede.ColumnIndex-1;
	double val1, val2;
	AnsiString aux1, aux2;

   switch(lviewIndRede.ColumnIndex)
		{
      case 0:  //caption
			Compare = CompareText(Item1->Caption,Item2->Caption);
         break;
		case 1:
			aux1    = Item1->SubItems->Strings[index];
			aux2    = Item2->SubItems->Strings[index];
			Compare = CompareText(aux1,aux2);
			break;
		case 5:
			aux1    = Item1->SubItems->Strings[index];
			aux2    = Item2->SubItems->Strings[index];
			Compare = CompareText(aux1,aux2);
			break;
		case 6:
			aux1    = Item1->SubItems->Strings[index];
			aux2    = Item2->SubItems->Strings[index];
			Compare = CompareText(aux1,aux2);
			break;
		case 7:
			aux1    = Item1->SubItems->Strings[index];
			aux2    = Item2->SubItems->Strings[index];
			Compare = CompareText(aux1,aux2);
			break;
		default: //substring
         val1 = StrToDouble(Item1->SubItems->Strings[index]);
         val2 = StrToDouble(Item2->SubItems->Strings[index]);
         if(val1 < val2) Compare = -1;
         else if(val1 > val2) Compare = 1;
         else Compare = 0;
		}
   //verifica se ordem crescente/decrescente
   if (! lviewIndRede.OrdemCrescente) Compare = - Compare;
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::MostraBarrasAmostra(void)
	{
   //variáveis locais
   VTCalculaVTCD *calcVTCD = (VTCalculaVTCD*)apl->GetObject(__classid(VTCalculaVTCD));
   VTGrafico     *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTBarraVTCD   *pbarV;
   TList *lisAux = new TList();

   for(int n = 0; n < calcVTCD->LisBarraVTCD->Count; n++)
      {
      pbarV = (VTBarraVTCD*)calcVTCD->LisBarraVTCD->Items[n];
      if(pbarV->Na == 0) continue;
      lisAux->Add(pbarV->Barra);
      }
   grafico->Moldura(lisAux);
   delete lisAux;
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::tbAmostraEnter(TObject *Sender)
	{
	//variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	grafico->Moldura();
	}
//---------------------------------------------------------------------------
VTAlternativa* __fastcall TFormAnaliseVTCD::TreeViewAlternativaSelecionada(TTreeNode *node)
   {
   //proteção
   if (node == NULL) return(NULL);
   //determina TTreeNode de Level 0
   while (node->Level > 1) node = node->Parent;
   //retorna Alternativa associada ao TreeNode
   if (node->Level == 1) return((VTAlternativa*)node->Data);
   //não uma Alternativa selecionada
   return(NULL);
   }

//---------------------------------------------------------------------------
VTObra* __fastcall TFormAnaliseVTCD::TreeViewObraSelecionada(TTreeNode *node)
   {
   //proteção
   if (node == NULL) return(NULL);
   //verifica se o TTreeNode selecionada é de Level 2
   while (node->Level > 2) node = node->Parent;
   if (node->Level == 2) return((VTObra*)node->Data);
   //não uma Obra selecionada
   return(NULL);
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::SgridTaxasDefeitoInicia(void)
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

   sgridTaxasDefeito->Cells[0][4]   = "FTz";
   sgridTaxasDefeito->Cells[1][4]   = DoubleToStr("%6.2f", calcVTCD->PDef[defeitoFTZ]  *100.);

   sgridTaxasDefeito->Cells[0][5]   = "2FT";
   sgridTaxasDefeito->Cells[1][5]   = DoubleToStr("%6.2f", calcVTCD->PDef[defeito2FT]  *100.);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseVTCD::SgridTaxasRedeInicia(void)
   {
	//variáveis locais
	VTCalculaVTCD *calcVTCD = (VTCalculaVTCD*)apl->GetObject(__classid(VTCalculaVTCD));
	VTRedes       *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));  //nnhk
	VTRede        *rede;
	TList         *lisRede = redes->LisRede();
	double        taxa;

	//Inicia taxas de falha das redes
	calcVTCD->IniciaRedes(redes);

	LimpaTStringGrid(sgridTaxasRede);

	//inicia StringGrid
	sgridTaxasRede->Cells[0][0] = "Rede";
	sgridTaxasRede->Cells[1][0] = "Taxa(falhas/km/ano)";
	sgridTaxasRede->RowCount = lisRede->Count + 1;
	for(int n = 0; n < lisRede->Count; n++)
		{
		rede = (VTRede*)lisRede->Items[n];
		taxa = calcVTCD->TaxaFalha[rede];
		sgridTaxasRede->Cells[0][n+1]   = rede->Codigo;
		sgridTaxasRede->Cells[1][n+1]   = DoubleToStr("%6.2f", taxa);
      }
   }

//---------------------------------------------------------------------------
//eof









































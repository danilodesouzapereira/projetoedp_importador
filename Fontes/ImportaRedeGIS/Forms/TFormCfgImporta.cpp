// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TFormCfgImporta.h"
#include <PlataformaSinap\Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormCfgImporta::TFormCfgImporta(TComponent* Owner, strOPImporta *opcao) : TForm(Owner)
{
	// guarda opcao
	this->opcao = opcao;
	// lê opções
	LeOpcao();
}

// ---------------------------------------------------------------------------
__fastcall TFormCfgImporta::~TFormCfgImporta(void)
{
	// nada
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgImporta::ActionCancelaExecute(TObject *Sender)
{
	// não salva alterações nas opções
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgImporta::ActionConfirmaExecute(TObject *Sender)
{
	// altera opções
	opcao->sub.topologia = CheckBoxEtd->Checked ? opSUB_TROCAR : opSUB_GIS;
	opcao->pri.consA4 = RadioGroupConsPrimario->ItemIndex;
	opcao->fator_carga = 0.6;
	opcao->sec.topologia = RadioGroupRedeSec->ItemIndex;
	opcao->valida_fase = CheckBoxValidaFase->Checked;
	opcao->corrige_fase = CheckBoxCorrigeFase->Checked;
	opcao->corrige_perda_fe = CheckBoxCorrigePerdaFerro->Checked;
	opcao->valida_eqpto = true;
	opcao->gerarTXT = CheckBoxTXT->Checked;
//	opcao->corrige_malha_mts = CheckBoxCorrigeMalhaMTs->Checked;
	//malhas
	opcao->malha.abre_chave_ext = CheckBoxAbrirChaveExt->Checked;
	opcao->malha.abre_chave_int = CheckBoxAbrirChaveInt->Checked;
	opcao->malha.desloca_trecho_ext = CheckBoxDeslocarTrechoExt->Checked;
	opcao->malha.desloca_trecho_int = CheckBoxDeslocarTrechoInt->Checked;
    opcao->malha.somente_rede_pri = true;
	// opções de ihm
	opcao->ihm.log = false; // CheckBoxLog->Checked;
	opcao->ihm.progresso = true;
	// fecha o form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgImporta::CheckBoxTXTClick(TObject *Sender)
{
	// altera para rede secundária completa, se estiver marcado
	if (CheckBoxTXT->Checked)
		RadioGroupRedeSec->ItemIndex = opSEC_GIS;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgImporta::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destrói o Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgImporta::LeOpcao(void)
{
	// lê as opções
	CheckBoxEtd->Checked = (opcao->sub.topologia == opSUB_TROCAR);
	RadioGroupConsPrimario->ItemIndex = opcao->pri.consA4;
	RadioGroupRedeSec->ItemIndex = opcao->sec.topologia;
	CheckBoxValidaFase->Checked = opcao->valida_fase;
	CheckBoxCorrigeFase->Checked = opcao->corrige_fase;
	CheckBoxCorrigePerdaFerro->Checked = opcao->corrige_perda_fe;
	CheckBoxTXT->Checked = opcao->gerarTXT;
//	CheckBoxCorrigeMalhaMTs->Checked = opcao->corrige_malha_mts;
	CheckBoxAbrirChaveExt->Checked = opcao->malha.abre_chave_ext;
	CheckBoxAbrirChaveInt->Checked = opcao->malha.abre_chave_int;
	CheckBoxDeslocarTrechoExt->Checked = opcao->malha.desloca_trecho_ext;
	CheckBoxDeslocarTrechoInt->Checked = opcao->malha.desloca_trecho_int;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgImporta::RadioGroupRedeSecClick(TObject *Sender)
{
	// altera CheckBoxTXT conforme a opção Rede Secundária
	if (RadioGroupRedeSec->ItemIndex != opSEC_GIS)
		CheckBoxTXT->Checked = false;
}

// ---------------------------------------------------------------------------
// eof

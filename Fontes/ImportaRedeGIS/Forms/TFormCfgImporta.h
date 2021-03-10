// ---------------------------------------------------------------------------
#ifndef TFormCfgImportaH
#define TFormCfgImportaH

// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.ImageList.hpp>
#include <PlataformaSinap\Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>

// ---------------------------------------------------------------------------
class TFormCfgImporta : public TForm
{
__published: // IDE-managed Components

	TGroupBox *GroupBox1;
	TCheckBox *CheckBoxEtd;
	TRadioGroup *RadioGroupConsPrimario;
	TRadioGroup *RadioGroupRedeSec;
	TGroupBox *GroupBox2;
	TCheckBox *CheckBoxValidaFase;
	TCheckBox *CheckBoxCorrigeFase;
	TCheckBox *CheckBoxCorrigePerdaFerro;
	TToolBar *ToolBar;
	TToolButton *ButFecha;
	TToolButton *ToolButton4;
	TActionList *ActionList;
	TAction *ActionConfirma;
	TAction *ActionCancela;
	TToolButton *ToolButton1;
	TAction *ActionHelp;
	TImageList *ImageList1;
	TGroupBox *GBoxRedeBT;
	TCheckBox *CheckBoxTXT;
	TGroupBox *GBoxMalha;
	TCheckBox *CheckBoxAbrirChaveExt;
	TCheckBox *CheckBoxAbrirChaveInt;
	TCheckBox *CheckBoxDeslocarTrechoInt;
	TCheckBox *CheckBoxDeslocarTrechoExt;

	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall RadioGroupRedeSecClick(TObject *Sender);
	void __fastcall CheckBoxTXTClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private: // User declarations
	void __fastcall LeOpcao(void);

public: // User declarations
	__fastcall TFormCfgImporta(TComponent* Owner, strOPImporta *opcao);
	__fastcall ~TFormCfgImporta(void);

private: // externo

	strOPImporta *opcao;

};

// ---------------------------------------------------------------------------
#endif

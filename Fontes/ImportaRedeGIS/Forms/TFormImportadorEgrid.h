// --------------------------------------------------------------------------------------------- bof
#ifndef TFormImportadorEgridH
#define TFormImportadorEgridH

// ---------------------------------------------------------------------------------------- includes
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Dialogs.hpp>
#include <System.Actions.hpp>
#include <System.Classes.hpp>
#include <System.ImageList.hpp>

// Plataforma include
#include <PlataformaSinap\Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>

// Project include
#include <Egrid\TImportaArquivoEgrid.h>
#include <Importa\VTConversor.h>

// ------------------------------------------------------------------------------------ declarations

// Class declaration
class VTApl;
class VTDadosEmpresa;
class VTCronometro;
class VTMontaEmpresa;
class TLigaMaeFilha;

// --------------------------------------------------------------------------------------- namespace
//using namespace sinapsis::sinap::edp;

// ------------------------------------------------------------------------------------------- class
class TFormImportadorEgrid : public TForm
{
__published: // IDE-managed Components

	TSplitter *Splitter1;
	TToolBar *ToolBar;
	TToolButton *btnReiniciarRede;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TToolButton *ButFecha;
	TPanel *PanelOpcao;
	TRadioGroup *RadioGroupDestino;
	TRadioGroup *RadioGroupRedeSec;
	TRadioGroup *RadioGroupConsPrimario;
	TGroupBox *GBoxCondutor;
	TLabel *Label1;
	TLabel *Label2;
	TComboBox *CBoxCondutorMT;
	TComboBox *CBoxCondutorBT;
	TGroupBox *GBoxLog;
	TCheckBox *CheckBoxLog;
	TCheckBox *CheckBoxCronometro;
	TGroupBox *GBoxBaseDado;
	TPanel *PanelData;
	TLabel *LabelData;
	TDateTimePicker *DateTimePicker;
	TGroupBox *GBoxPastaPublica;
	TEdit *EditPastaPublica;
	TBitBtn *ButPastaPublica;
	TGroupBox *GroupBox2;
	TCheckBox *CheckBoxValidaFase;
	TCheckBox *CheckBoxCorrigeFase;
	TCheckBox *CheckBoxCorrigePerdaFerro;
	TGroupBox *GBoxSubestacaoAereo;
	TCheckListBox *CLBoxSubestacao;
	TToolBar *ToolBar2;
	TBitBtn *ButSubestacaoSelAll;
	TBitBtn *ButSubestacaoSelNone;
	TActionList *ActionList;
	TAction *ActionIniciarImportacao;
	TAction *ActionRedeClear;
	TAction *ActionSelPastaPublica;
	TAction *ActionSubestacaoSelAll;
	TAction *ActionSubestacaoSelNone;
	TAction *ActionPrimarioSelAll;
	TAction *ActionPrimarioSelNone;
	TAction *ActionHelp;
	TAction *ActionFecha;
	TImageList *ImageList;
	TToolButton *btnAbrir;
	TAction *ActionAbrirEgrid;
	TToolButton *btnImportar;
	TOpenDialog *dlgOpen;
	TPanel *pnlRedePri;
	TPanel *pnlRedeSec;
	TGroupBox *GBoxRegional;
	TCheckListBox *CLBoxRegional;
	TToolBar *ToolBarRede;
	TBitBtn *ButRegionalSelAll;
	TBitBtn *ButRegionalSelNone;
	TGroupBox *GBoxLocalidade;
	TToolBar *ToolBar3;
	TBitBtn *ButLocalidadeSelAll;
	TBitBtn *ButLocalidadeSelNone;
	TSplitter *Splitter2;
	TSplitter *Splitter3;
	TAction *ActionRegionalSelAll;
	TAction *ActionRegionalSelNone;
	TAction *ActionLocalidadeSelAll;
	TAction *ActionLocalidadeSelNone;
	TGroupBox *GBoxPrimario;
	TPanel *pnlCaboRamal;
	TLabel *Label3;
	TComboBox *CBoxCondutorRL;
	TPanel *pnlCaboIP;
	TLabel *Label4;
	TComboBox *CBoxCondutorIP;
	TPanel *PanelSelecao;
	TPanel *PanelAgrupamentos;
	TSplitter *Splitter4;
	TCheckListBox *CLBoxLocalidade;
	TCheckListBox *CLBoxPrimario;
	TToolBar *ToolBar1;
	TBitBtn *ButPrimarioSelAll;
	TBitBtn *ButPrimarioSelNone;
	TGroupBox *GBoxMalha;
	TCheckBox *CheckBoxAbrirChaveExt;
	TCheckBox *CheckBoxAbrirChaveInt;
	TCheckBox *CheckBoxDeslocarTrechoInt;
	TCheckBox *CheckBoxDeslocarTrechoExt;
	TAction *ActionRelValidacao;
	TGroupBox *GroupBox1;
	TCheckBox *CheckBoxSalvarRede;
	TCheckBox *CheckBoxRelEqpto;
	TEdit *EditDirRelEqpto;
	TBitBtn *BitBtnSelDirRelEqpto;
	TPanel *Panel1;
	TLabel *Label5;
	TAction *ActionSelPastaRelatorioEqpto;
    TCheckBox *CBoxArranjoMontante;
    TToolButton *ToolButton1;
    TAction *ActionAborta;

	void __fastcall ActionAbrirEgridExecute(TObject *Sender);
	void __fastcall ActionRedeClearExecute(TObject *Sender);
	void __fastcall ActionIniciarImportacaoExecute(TObject *Sender);
	void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall ActionHelpExecute(TObject *Sender);
	void __fastcall CLBoxSubestacaoClickCheck(TObject *Sender);
	void __fastcall ActionSelPastaPublicaExecute(TObject *Sender);
	void __fastcall ActionSubestacaoSelAllExecute(TObject *Sender);
	void __fastcall ActionSubestacaoSelNoneExecute(TObject *Sender);
	void __fastcall ActionPrimarioSelAllExecute(TObject *Sender);
	void __fastcall ActionPrimarioSelNoneExecute(TObject *Sender);
	void __fastcall CLBoxPrimarioClickCheck(TObject *Sender);
	void __fastcall CLBoxRegionalClickCheck(TObject *Sender);
	void __fastcall CLBoxLocalidadeClickCheck(TObject *Sender);
	void __fastcall ActionRegionalSelAllExecute(TObject *Sender);
	void __fastcall ActionRegionalSelNoneExecute(TObject *Sender);
	void __fastcall ActionLocalidadeSelAllExecute(TObject *Sender);
	void __fastcall ActionLocalidadeSelNoneExecute(TObject *Sender);
	void __fastcall ActionRelValidacaoExecute(TObject *Sender);
	void __fastcall CheckBoxRelEqptoExit(TObject *Sender);
	void __fastcall ActionSelPastaRelatorioEqptoExecute(TObject *Sender);
    void __fastcall ActionAbortaExecute(TObject *Sender);


private: // Variáveis locais
	TImportaArquivoEgrid* importadorArquivo;
	VTApl *apl;
	VTDadosEmpresa *empresa;
	VTCronometro *cronometro;
	VTConversor *conversor;
	VTMontaEmpresa *monta_empresa;
	TList *lisTMP;
	strOPImporta opcao;
	TList *lisRelMaeFilha;
	TLigaMaeFilha *ligaMaeFilha;
	VTProgresso *progresso;
	// métodos

	// void __fastcall CarregarConfiguracaoEmpresa();
	void __fastcall CBoxCondutorInicia(TComboBox *CBox, TList *lisCONDUTOR, int condutor_id);
	void __fastcall CLBoxLocalidadeInicia(void);
	void __fastcall CLBoxRegionalInicia(void);
	void __fastcall CheckAll(TCheckListBox *CLBox, bool checked);
	void __fastcall CLBoxSubestacaoInicia(void);
    void __fastcall EnableBut(bool enabled);
	void __fastcall EnableOpcaoRelEqpto(bool enable);
	void __fastcall FormIniciaPosicao(void);
	void __fastcall FormLeOpcoes(void);
	void __fastcall FormSalvaOpcoes(void);
	bool __fastcall ImportaSubestacao(void);
	void __fastcall CLBoxPrimarioInicia(void);
	void __fastcall CLBoxFilhasInicia(TList *lisFilhas);
  //	void __fastcall LBoxPrimarioInicia(void);
	int __fastcall LisObjectChecked(TCheckListBox *CLBox, TList *lisOBJ);
	bool __fastcall ReiniciaRede(bool aviso);
	bool __fastcall ValidaOpcoesDeImportacao(void);

	struct
	{
		int cabo_mt_id;
		int cabo_bt_id;
	}

	oracle;

public: // User declarations

	__fastcall TFormImportadorEgrid(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
	__fastcall ~TFormImportadorEgrid(void);
};

// extern PACKAGE TFormImportadorEgrid *FormImportadorEgrid;

#endif

// --------------------------------------------------------------------------------------------- eof

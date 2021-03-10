//---------------------------------------------------------------------------
#ifndef TFormAnaliseVTCDH
#define TFormAnaliseVTCDH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
//#include <complex.h>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TFormEdt.h"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <CheckLst.hpp>
#include <Grids.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAlternativa;
class VTBarra;
class VTBarraVTCD;
class VTCalculaVTCD;
class VTBarCC;
class VTEdt;
class VTEstudo;
class VTObra;
class VTRede;
class VTRedeCC;
class VTResAlternativa;
//---------------------------------------------------------------------------
struct ORDENACAO
		 {
		 int  ColumnIndex;
		 bool OrdemCrescente;
		 };
//---------------------------------------------------------------------------
class TFormAnaliseVTCD : public TFormEdt
{
__published:   // IDE-managed Components
	TActionList *ActionList1;
	TPageControl *pgcResult;
	TTabSheet *tbsDadosEntrada;
	TTabSheet *TabSheet2;
	TPageControl *PageControl2;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TToolBar *ToolBar2;
	TLabel *Label1;
	TComboBox *ComboBox1;
	TToolBar *ToolBar3;
	TLabel *Label2;
	TComboBox *ComboBox2;
	TChart *Chart2;
	TLineSeries *Series0;
	TLineSeries *LineSeries1;
	TTabSheet *TabSheet5;
	TSplitter *Splitter1;
	TGroupBox *GroupBox1;
	TListView *ListView2;
	TChart *Chart1;
	TBarSeries *Series1;
	TBarSeries *Series2;
	TBarSeries *Series3;
	TGroupBox *GroupBox2;
	TListView *ListView5;
	TChart *Chart3;
	TBarSeries *BarSeries1;
	TBarSeries *BarSeries2;
	TBarSeries *BarSeries3;
	TPanel *Panel1;
	TGroupBox *GroupBox4;
	TListView *ListView3;
	TSplitter *Splitter2;
	TGroupBox *GroupBox5;
	TListView *ListView6;
	TPanel *Panel2;
	TGroupBox *GroupBox6;
	TListView *ListView4;
	TGroupBox *GroupBox7;
	TListView *ListView7;
	TSplitter *Splitter3;
	TChart *Chart4;
	TBarSeries *BarSeries4;
	TBarSeries *BarSeries5;
	TBarSeries *BarSeries6;
	TChart *Chart5;
	TBarSeries *BarSeries7;
	TBarSeries *BarSeries8;
	TBarSeries *BarSeries9;
	TAction *ActionExecutar;
	TComboBox *CBoxDef;
	TAction *ActionSelEqpto;
	TTabSheet *TabSheet6;
	TTabSheet *TabSheet7;
	TListView *LViewBarras;
	TAction *ActionReinciaBarras;
	TAction *ActionMostraAfundamentos;
	TAction *ActionMostraFreqBarras;
	TPanel *panelAfundaBarra;
	TToolBar *ToolBar4;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TToolButton *ToolButton5;
	TPanel *panelCargas;
	TTabSheet *tbsIndicVTCD;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox8;
	TListView *LViewIndRede;
	TListView *LViewIndCarga;
	TSplitter *Splitter4;
	TSplitter *Splitter5;
	TToolButton *ToolButton6;
	TPanel *Panel3;
	TGroupBox *gbAlt;
	TListView *LViewAlt;
	TSplitter *Splitter6;
	TGroupBox *GroupBox10;
	TStringGrid *sgridTaxasRede;
	TStringGrid *sgridTaxasDefeito;
	TAction *ActionEnablePatamar;
	TAction *ActionDisablePatamar;
	TSplitter *Splitter7;
	TGroupBox *GroupBox12;
	TToolBar *ToolBarPatamar;
	TToolButton *ToolButton4;
	TToolButton *ToolButton7;
	TCheckListBox *CLBoxPatamar;
	TToolButton *ToolButton8;
	TCheckBox *ChkMapa;
	TComboBox *CBoxAf;
	TPanel *Panel4;
	TGroupBox *GroupBox11;
	TTrackBar *tbAmostra;
	TSplitter *Splitter8;
	TGroupBox *GroupBox13;
	TRichEdit *rchAno;
	TRichEdit *rchDura;
	TLabel *Label3;
	TSplitter *Splitter9;
	TGroupBox *GroupBox9;
	TListView *LViewIndTotal;
	TSplitter *Splitter10;
	TGroupBox *GroupBox14;
	TListView *LViewIndAuxCarga;
	TListView *LViewIndAuxRede;
	TListView *LViewIndAuxTotal;
	TCheckBox *chkDif;
	TTabSheet *TabSheet1;
	TGroupBox *GroupBox15;
	TShape *Shape1;
	TShape *Shape2;
	TShape *Shape3;
	TLabel *Label4;
	TShape *Shape4;
	TLabel *Label5;
	TShape *Shape5;
	TShape *Shape6;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TShape *Shape7;
	TLabel *Label10;
	TEdit *dec1;
	TEdit *dec2;
	TRadioButton *butDec;
	TRadioButton *butFec;
	TEdit *fec1;
	TEdit *fec2;
	TRadioButton *butEnd;
	TEdit *end1;
	TEdit *end2;
	TRadioButton *butPot;
	TEdit *pot1;
	TEdit *pot2;
	TPanel *Panel5;
	TListView *ListView1;
	TSplitter *Splitter11;
	TChart *chtEvRede;
	TBarSeries *Series4;
	TBarSeries *Series5;
	TBarSeries *Series6;
	TBarSeries *Series7;
	TPopupMenu *PopupMenu1;
	TPanel *pnMin;
	TLabel *lblMin;
	TLabel *lblMax;
	TPanel *pnMax;
	TAction *ActionExibeAmostra;
	TButton *Button1;
	TAction *ActionDefaultTipoCurto;
	TToolBar *ToolBar1;
	TToolButton *ToolButton9;
	TTabSheet *TabSheet8;
	void __fastcall ActionDisablePatamarExecute(TObject *Sender);
	void __fastcall ActionEdtExecute(TObject *Sender);
	void __fastcall ActionEnablePatamarExecute(TObject *Sender);
	void __fastcall ActionExecutarExecute(TObject *Sender);
	void __fastcall ActionSelEqptoExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall LViewAltDragDrop(TObject *Sender, TObject *Source, int X,
			 int Y);
	void __fastcall LViewAltDragOver(TObject *Sender, TObject *Source, int X,
			 int Y, TDragState State, bool &Accept);
	void __fastcall LViewBarrasClick(TObject *Sender);
	void __fastcall LViewIndCargaClick(TObject *Sender);
	void __fastcall LViewIndCargaColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall LViewIndCargaCompare(TObject *Sender, TListItem *Item1,
			 TListItem *Item2, int Data, int &Compare);
	void __fastcall LViewIndRedeColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall LViewIndRedeCompare(TObject *Sender, TListItem *Item1,
			 TListItem *Item2, int Data, int &Compare);
	void __fastcall LViewIndTotalColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall LViewIndTotalCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
	void __fastcall CBoxAfChange(TObject *Sender);
	void __fastcall ChkMapaClick(TObject *Sender);
	void __fastcall LViewAltClick(TObject *Sender);
	void __fastcall LViewIndTotalMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall LViewIndRedeMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall LViewIndCargaDblClick(TObject *Sender);
	void __fastcall LViewIndRedeClick(TObject *Sender);
	void __fastcall ActionExibeAmostraExecute(TObject *Sender);
	void __fastcall tbAmostraEnter(TObject *Sender);
	void __fastcall ActionDefaultTipoCurtoExecute(TObject *Sender);

private:   // User declarations

public:      // User declarations
	__fastcall  TFormAnaliseVTCD(TComponent *Owner, TForm* formEstudo, VTApl *apl_owner, TWinControl *parent);
	__fastcall ~TFormAnaliseVTCD(void);

private: //métodos
VTResAlternativa* __fastcall BuscaAlternativa(VTAlternativa* alt, VTObra* obra);
	  void         __fastcall CBoxPatamarInicia(void);
	  void         __fastcall ChecaItens(void);
	  void         __fastcall CLBoxPatamarInicia(void);
	  bool         __fastcall ConfigRede(VTAlternativa *alt, VTObra *obra);
	  double       __fastcall CompRede(VTRede *rede);
	  double       __fastcall CompRedes(void);
	  bool         __fastcall ExecutaAlternativas(void);
VTEstudo*         __fastcall ExisteEstudo(void);
	  void         __fastcall FormIniciaPosicao(void);
	  void         __fastcall IniciaChtRede(TList *lisAltF, TList *lisRedeF);
	  void         __fastcall IniciaChtRede1(void);
	  void         __fastcall IniciaMapaTematico(void);
	  bool         __fastcall IniciaVExtr(double nivelAf, double &Vmax, double &Vmin);
	  void         __fastcall InsereAltObra(VTAlternativa *alt, VTObra *obra);
	  void         __fastcall LimpaTStringGrid(TStringGrid *StringGrid);
	  void         __fastcall LViewAltInicia(void);
	  void         __fastcall LViewBarrasInicia(void);
	  void         __fastcall LViewIndAuxInicia(void);
	  void         __fastcall LViewIndicInicia1(TList *lisAltF, TList *lisRedeF, int iaux);
     void         __fastcall LViewIndicInicia2(VTRede* rede, VTResAlternativa *resAlt);
	  void         __fastcall LViewIndicZera(void);
	  void         __fastcall LViewIndRedeReinicia(void);
	  void         __fastcall MostraBarrasAmostra(void);
	  VTObra*      __fastcall TreeViewObraSelecionada(TTreeNode *node);
VTAlternativa*    __fastcall TreeViewAlternativaSelecionada(TTreeNode *node);
	  void         __fastcall SgridTaxasDefeitoInicia(void);
	  void         __fastcall SgridTaxasRedeInicia(void);

private: //objetos externos

private: //dados locais
	VTApl    *apl;
	TForm           *formEstudo;
	VTEdt           *edt_busca;
	ORDENACAO       lviewIndRede, lviewIndCarga, lviewIndTotal;
	VTAlternativa   *altFoc;
	VTObra          *obraFoc;
	TList           *lisResAlt;
};

//---------------------------------------------------------------------------
#endif
//eof

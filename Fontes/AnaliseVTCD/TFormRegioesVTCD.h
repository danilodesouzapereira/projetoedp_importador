//---------------------------------------------------------------------------

#ifndef TFormRegioesVTCDH
#define TFormRegioesVTCDH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class VTApl;
class TFormRegiaoCurtos;
//---------------------------------------------------------------------------
struct ORDENACAO
		{
		int  ColumnIndex;
		bool OrdemCrescente;
		};

//---------------------------------------------------------------------------
class TFormRegioesVTCD : public TForm
{
__published:	// IDE-managed Components
	TActionList *ActionList1;
	TAction *ActionMostraRegiao;
	TAction *ActionMostraEqProt;
	TAction *ActionMostraBarrasCurto;
	TToolBar *ToolBar2;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TSplitter *Splitter1;
	TGroupBox *gbBarrasCurto;
	TListView *lvRegioes;
	TSplitter *Splitter2;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ActionMostraRegiaoExecute(TObject *Sender);
	void __fastcall ActionMostraEqProtExecute(TObject *Sender);
	void __fastcall ActionMostraBarrasCurtoExecute(TObject *Sender);
	void __fastcall lvRegioesColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall lvRegioesCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);
private:	// User declarations
public:		// User declarations
	 __fastcall TFormRegioesVTCD(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
	 __fastcall ~TFormRegioesVTCD(void);
void __fastcall MostraRegioes(TList *lisRegioes);

private:
	VTApl* apl;
	TFormRegiaoCurtos *formRegiaoCurtos;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormRegioesVTCD *FormRegioesVTCD;
//---------------------------------------------------------------------------
#endif

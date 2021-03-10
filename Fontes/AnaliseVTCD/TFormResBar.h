//---------------------------------------------------------------------------

#ifndef TFormResBarH
#define TFormResBarH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <ExtCtrls.hpp>
#include <VclTee.TeeGDIPlus.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTBarraVTCD;
//---------------------------------------------------------------------------
class TFormResBar : public TForm
	{
	__published:	// IDE-managed Components
	TListView *LViewResBar;
	TChart *chtAfunda;
	TBarSeries *Series1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall LViewResBarSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
	private:	// User declarations
	public:  // User declarations
		__fastcall TFormResBar(TComponent* Owner, VTApl *apl, VTBarraVTCD *pbarV, AnsiString codAlt);
		__fastcall ~TFormResBar(void);
		void __fastcall IniciaLView(void);
		void __fastcall ReiniciaGraf(int idef, AnsiString tit);

	private:
      VTApl       *apl;
		VTBarraVTCD *pbarV;
		AnsiString  codAlt;

	};
//---------------------------------------------------------------------------
extern PACKAGE TFormResBar *FormResBar;
//---------------------------------------------------------------------------
#endif

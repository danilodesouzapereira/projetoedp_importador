//---------------------------------------------------------------------------

#ifndef TFormRegiaoCurtosH
#define TFormRegiaoCurtosH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
//---------------------------------------------------------------------------
class VTApl;
class TRegiaoVTCD;
//---------------------------------------------------------------------------
class TFormRegiaoCurtos : public TForm
{
__published:	// IDE-managed Components
	TListView *lvRegiaoCurtos;
	TActionList *ActionList1;
	TAction *ActionSelBarras;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ActionSelBarrasExecute(TObject *Sender);
private:	// User declarations
public:		// User declarations
	 __fastcall TFormRegiaoCurtos(TComponent *Owner, VTApl *apl_owner, TRegiaoVTCD *regiaoVTCD, TWinControl *parent);
	 __fastcall ~TFormRegiaoCurtos(void);
private:
	 void __fastcall ListaCurtos(void);


private:
     VTApl *apl;
	 TRegiaoVTCD *regiaoVTCD;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormRegiaoCurtos *FormRegiaoCurtos;
//---------------------------------------------------------------------------
#endif

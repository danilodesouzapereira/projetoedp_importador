// ---------------------------------------------------------------------------
#ifndef TFormStkMdiH
#define TFormStkMdiH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <System.Actions.hpp>

// ---------------------------------------------------------------------------
class VTApl;

// ---------------------------------------------------------------------------
class TFormStkMdi : public TForm
{
__published: // IDE-managed Components

	TActionList *ActionList;
	TAction *ActionImportaOracle;

	void __fastcall ActionImportaOracleExecute(TObject *Sender);

public: // User declarations
	__fastcall TFormStkMdi(TComponent *Owner, VTApl *apl_owner);
	__fastcall ~TFormStkMdi(void);
	void __fastcall Atualiza(void);

private: // métodos
	void __fastcall ConfiguraMainMenu(void);
	void __fastcall ConfiguraPopupMenuImporta(void);

	// objetos externos
	VTApl *apl;

	// dados locais
};

// ---------------------------------------------------------------------------
#endif
// eof

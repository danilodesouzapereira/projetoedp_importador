//---------------------------------------------------------------------------
#ifndef AnaliseVTCDH
#define AnaliseVTCDH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\DLL_Inc\Export.h>

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void   __fastcall DLL_CloseFormAnaliseVTCD(TComponent *owner);
EXPORT TForm* __fastcall DLL_NewFormAnaliseVTCD(TComponent *owner, TForm* formEstudo, VTApl *apl_owner, TWinControl *parent);
EXPORT TForm* __fastcall DLL_NewFormBDVTCD(TComponent *owner, VTApl *apl_owner, TWinControl *parent);

//---------------------------------------------------------------------------
#endif
//eof


 
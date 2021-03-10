#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <ProjetoEdp\DLL_Inc\AnaliseVTCD.h>
#pragma argsused

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormAnaliseVTCD(TComponent *owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
   {
   return 1;
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormAnaliseVTCD(TComponent *owner)
   {
   //variáveis locais
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAnaliseVTCD(TComponent *owner, TForm* formEstudo, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormBDVTCD(TComponent *owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   return(NULL);
   }
//---------------------------------------------------------------------------



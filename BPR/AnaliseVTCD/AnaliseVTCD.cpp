#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <ProjetoEdp\DLL_Inc\AnaliseVTCD.h>
#include <ProjetoEdp\Fontes\AnaliseVTCD\TFormAnaliseVTCD.h>
#include <ProjetoEdp\Fontes\AnaliseVTCD\TFormBDVTCD.h>
#pragma argsused

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormAnalise(TComponent *owner, AnsiString NomeForm);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
   {
   return 1;
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormAnaliseVTCD(TComponent *owner)
   {
   //vari�veis locais
   TForm *form;

   //destr�i todos os TFormAnaliseVTCD abertos
   while ((form = ExisteFormAnalise(owner, "TFormAnaliseVTCD")) != NULL) delete form;
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAnaliseVTCD(TComponent *owner, TForm* formEstudo, VTApl *apl_owner, TWinControl *parent)
   {
   //vari�veis locais
   TForm *form;

   try{//verifica se existe um TFormAnaliseVTCD aberto
	  if ((form = ExisteFormAnalise(owner, "TFormAnaliseVTCD")) == NULL)
         {//cria um novo TFormAnaliseVTCD e exibe como janela normal
         form = new TFormAnaliseVTCD(owner, formEstudo, apl_owner, parent);
         }
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(form);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormBDVTCD(TComponent *owner, VTApl *apl_owner, TWinControl *parent)
   {
   //vari�veis locais
   TForm *form;

   try{//verifica se existe um TFormBDVTCD aberto
	  if ((form = ExisteFormAnalise(owner, "TFormBDVTCD")) == NULL)
		 {//cria um novo TFormAnaliseVTCD e exibe como janela normal
		 form = new TFormBDVTCD(owner, apl_owner, parent);
		 }
	  }catch(Exception &e)
		 {
		 return(NULL);
		 }
   return(form);
   }

//---------------------------------------------------------------------------
//fun��es locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormAnalise(TComponent *owner, AnsiString NomeForm)
   {
   //vari�veis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormAnaliseVTCD aberto
   for (int n = 0; n < Screen->FormCount; n++)
	  {//verifica se o Form n � um TFormAnaliseVTCD
	  class_name = Screen->Forms[n]->ClassName();
   //	  if (class_name.AnsiCompareIC("TFormAnaliseVTCD") == 0)
	  if (class_name.AnsiCompareIC(NomeForm) == 0)
		 {
		 if (Screen->Forms[n]->Owner == owner) return(Screen->Forms[n]);
		 }
	  }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof



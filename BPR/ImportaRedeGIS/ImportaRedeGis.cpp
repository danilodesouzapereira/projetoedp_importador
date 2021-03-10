#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#pragma argsused
#include <PlataformaSinap\DLL_Inc\ImportaredeGIS.h>
#include <PlataformaSinap\Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>
#include <ProjetoEDP\Fontes\ImportaRedeGIS\Forms\TFormCfgImporta.h>
#include "Egrid\TDataEgrid.h"

#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "BDihm.a")
#pragma comment(lib, "Cronometro.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Esquematico.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Importa.a")
#pragma comment(lib, "InfoZcc.a")
#pragma comment(lib, "InterfaceTXT.a")
#pragma comment(lib, "LocalDB.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "MontaZ.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Potencia.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Redutor.a")
#pragma comment(lib, "Stk.a")

// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

// -----------------------------------------------------------------------------
EXPORT VTImportaRedeGIS* __fastcall DLL_NewObjImportaRedeGIS(TComponent *Owner, VTApl *apl_owner)
{
	return (NewObjImportaRedeGIS(Owner, apl_owner));
}
// -----------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewDataEgrid(VTApl *apl_owner, StrDadosEgrid* dadosEgrid)
{
   TDataEgrid *dataEgrid = new TDataEgrid(apl_owner);
	dataEgrid->ObtemDados(dadosEgrid);
	delete dataEgrid;
}
// ---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCfgImporta(TComponent *Owner, strOPImporta *opcao)
{
	return (new TFormCfgImporta(Owner, opcao));
}

// ---------------------------------------------------------------------------
// eof

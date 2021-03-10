//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TResAlternativa.h"
#include "VTBarraVTCD.h"
#include <PlataformaSinap\Fontes\Obra\VTObra.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTResAlternativa* NewObjResAlternativa(VTAlternativa* alt, VTObra* obra)
	{
	return(new TResAlternativa(alt, obra));
	}

//---------------------------------------------------------------------------
__fastcall TResAlternativa::TResAlternativa(VTAlternativa* alt, VTObra* obra)
	{
	this->alt = alt;
	this->obra= obra;
	lisBarVTCD = new TList();
	}
//---------------------------------------------------------------------------
__fastcall TResAlternativa::~TResAlternativa(void)
	{
	if(lisBarVTCD) {LimpaTList(lisBarVTCD); delete lisBarVTCD; lisBarVTCD = NULL;}
	}
//---------------------------------------------------------------------------
VTAlternativa*  __fastcall TResAlternativa::PM_GetAlternativa(void)
	{
	return(alt);
	}
//---------------------------------------------------------------------------
VTBarraVTCD*  __fastcall TResAlternativa::PM_GetBarVTCD(VTBarra *pbarra)
	{
	VTBarraVTCD* pbarV;

	pbarV = NULL;
	for(int n = 0; n < lisBarVTCD->Count; n++)
		{
		pbarV = (VTBarraVTCD*)lisBarVTCD->Items[n];
		if(pbarV->Barra == pbarra) break;
		pbarV = NULL;
		}
	return(pbarV);
	}
//---------------------------------------------------------------------------
VTObra*  __fastcall TResAlternativa::PM_GetObra(void)
	{
	return(obra);
	}
//---------------------------------------------------------------------------
TList*  __fastcall TResAlternativa::PM_GetLisBarraVTCD(void)
	{
	return(lisBarVTCD);
	}
//---------------------------------------------------------------------------


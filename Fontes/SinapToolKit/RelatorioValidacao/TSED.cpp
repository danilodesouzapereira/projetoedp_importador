//---------------------------------------------------------------------------

#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include "TAlimentador.h"
#include "TSED.h"
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TSED::TSED(VTApl *apl, VTRede *sed)
{
    this->apl = apl;
	lisALIM = new TList;
	redeSED =  sed;
	if (sed != NULL)
	{
		if (!sed->Codigo.IsEmpty())
		{
			codigo = sed->Codigo;
		}
		else
		{
			codigo = sed->Extern_id;
        }
	}
	else
	{
		codigo = "INDEFINIDA";
	}
}
//---------------------------------------------------------------------------
__fastcall TSED::~TSED(void)
{
	if(lisALIM)
	{
		LimpaTList(lisALIM);
		delete lisALIM;
        lisALIM = NULL;
    }
}
// ---------------------------------------------------------------------------
TAlimentador* __fastcall TSED::InsereAlimentador(VTRede *redePri)
{
	TAlimentador *alim;

	alim = new TAlimentador(apl,redePri);
	lisALIM->Add(alim);

	alim->redeSED = redeSED;

	return alim;
}
// ---------------------------------------------------------------------------
// eof

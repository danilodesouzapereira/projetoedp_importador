//---------------------------------------------------------------------------

#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Potencia\VTDemanda.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamares.h>
#include "TAlimentador.h"
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TAlimentador::TAlimentador(VTApl *apl, VTRede *primario)
{
	this->apl = apl;
	this->patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	this->demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

	redePri = primario;
	lisREDESSEC = new TList;
//	lisAUX = new TList;

}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16666485
__fastcall TAlimentador::~TAlimentador(void)
{
	DestroiTObject(lisREDESSEC);

//	DestroiTObject(lisAUX);
}
// ---------------------------------------------------------------------------
// eof

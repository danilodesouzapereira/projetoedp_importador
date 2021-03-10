//---------------------------------------------------------------------------
#pragma hdrstop
#include "TUcsv.h"
#include "VTBarraVTCD.h"
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TUcsv::TUcsv(VTCarga *carga)
	{
	this->carga = carga;

	barraVTCD = NewObjBarraVTCD(carga->pbarra);
	}
//---------------------------------------------------------------------------
__fastcall TUcsv::~TUcsv(void)
	{
	if(barraVTCD) {delete barraVTCD; barraVTCD = NULL;}
	}
//---------------------------------------------------------------------------

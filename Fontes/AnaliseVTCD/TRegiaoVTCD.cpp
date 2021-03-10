//---------------------------------------------------------------------------
#pragma hdrstop
#include "TRegiaoVTCD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TRegiaoVTCD::TRegiaoVTCD(void)
	{
	lisBarrasCurto = new TList();
	lisBlocosRegiao= new TList();
	lisEqProt      = new TList();
	lisRegiaoJusante = new TList();
	lisBarrasRegiao  = new TList();
	}
//---------------------------------------------------------------------------
__fastcall TRegiaoVTCD::~TRegiaoVTCD(void)
	{
	if(lisBarrasCurto) {delete lisBarrasCurto; lisBarrasCurto = NULL;}
	if(lisBlocosRegiao) {delete lisBlocosRegiao; lisBlocosRegiao = NULL;}
	if(lisEqProt) {delete lisEqProt; lisEqProt = NULL;}
	if(lisBarrasRegiao) {delete lisBarrasRegiao; lisBarrasRegiao = NULL;}
	if(lisRegiaoJusante) {delete lisRegiaoJusante; lisRegiaoJusante = NULL;}

	}
//---------------------------------------------------------------------------

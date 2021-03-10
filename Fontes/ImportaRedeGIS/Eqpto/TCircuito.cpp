// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Rede.h>
#include "TCircuito.h"
#include "TEt.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global
// ---------------------------------------------------------------------------
TCircuito* __fastcall NewObjCircuito(void)
{
	try
	{
		return (new TCircuito());
	}
	catch (Exception &e)
	{
	}
	// erro na criação do objeto
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TCircuito::TCircuito(void)
{
	// cria listas
	lisEP = new TList();
	lisET = new TList();
	lisBAR = new TList();
	lisTRE = new TList();
	barra_id0_a = NULL;
	barra_id0_b = NULL;
}

// ---------------------------------------------------------------------------
__fastcall TCircuito::~TCircuito(void)
{
	// destrói lista sem destruir seus objetos
	if (lisBAR)
	{
		delete lisBAR;
		lisBAR = NULL;
	}
	// destrói lista e seus objetos
	if (lisEP)
	{
		LimpaTList(lisEP);
		delete lisEP;
		lisEP = NULL;
	}
	if (lisET)
	{
		LimpaTList(lisET);
		delete lisET;
		lisET = NULL;
	}
	if (lisTRE)
	{
		LimpaTList(lisTRE);
		delete lisTRE;
		lisTRE = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TCircuito::DeleteTrecho(VTTrecho *trecho)
{
	// proteção
	if (trecho == NULL)
		return (false);
	lisTRE->Remove(trecho);
	DLL_DeleteEqpto(trecho);

	return (true);
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TCircuito::ExisteBarra(int barra_id)
{
	// variáveis locais
	VTBarra *barra;

	// procura Barra em lisBAR
	for (int n = 0; n < lisBAR->Count; n++)
	{
		barra = (VTBarra*)lisBAR->Items[n];
		if (barra->Id == barra_id)
			return (barra);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTTrecho* __fastcall TCircuito::ExisteTrecho(int trecho_id)
{
	// variáveis locais
	VTTrecho *trecho;

	// procura Barra em lisTRE
	for (int n = 0; n < lisTRE->Count; n++)
	{
		trecho = (VTTrecho*)lisTRE->Items[n];
		if (trecho->Id == trecho_id)
			return (trecho);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTTrecho* __fastcall TCircuito::ExisteTrecho(int barra1_id, int barra2_id)
{
	// variáveis locais
	VTTrecho *trecho;

	// proteção: troca o o ID das barras se for igual ao ID da Barra inicial indicada pelo GIS
	if (barra1_id == this->GIS_bar_ini_id)
		barra1_id = this->barra_ini->Id;
	else if (barra2_id == this->GIS_bar_ini_id)
		barra2_id = this->barra_ini->Id;
	// procura Barra em lisTRE
	for (int n = 0; n < lisTRE->Count; n++)
	{
		trecho = (VTTrecho*)lisTRE->Items[n];
		if ((trecho->pbarra1->Id == barra1_id) && (trecho->pbarra2->Id == barra2_id))
			return (trecho);
		if ((trecho->pbarra2->Id == barra1_id) && (trecho->pbarra1->Id == barra2_id))
			return (trecho);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
TEt* __fastcall TCircuito::ExisteEp(int id)
{
	// variáveis locais
	TEt *et;

	// procura EP em lisEP
	for (int n = 0; n < lisEP->Count; n++)
	{
		et = (TEt*)lisEP->Items[n];
		if (et->id == id)
			return (et);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
TEt* __fastcall TCircuito::ExisteEt(int id)
{
	// variáveis locais
	TEt *et;

	// procura ET em lisET
	for (int n = 0; n < lisET->Count; n++)
	{
		et = (TEt*)lisET->Items[n];
		if (et->id == id)
			return (et);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TCircuito::InsereBarra(VTBarra *barra)
{
	// proeção
	if (barra == NULL)
		return (false);
	// insere Barra em lisBAR
	lisBAR->Add(barra);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCircuito::InsereEp(TEt *et)
{
	// proteção
	if (et == NULL)
		return (false);
	// insere Et em lisEP e associa c/ este Circuito
	if (lisEP->IndexOf(et) < 0)
	{
		lisEP->Add(et);
		et->circuito = this;
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCircuito::InsereEt(TEt *et)
{
	// proteção
	if (et == NULL)
		return (false);
	// insere ET na lista e associa c/ este Circuito
	if (lisET->IndexOf(et) < 0)
	{
		lisET->Add(et);
		et->circuito = this;
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCircuito::InsereTrecho(VTTrecho *trecho)
{
	// proeção
	if (trecho == NULL)
		return (false);
	// insere Barra em lisBAR
	lisTRE->Add(trecho);
	return (true);
}

// ---------------------------------------------------------------------------
// eof

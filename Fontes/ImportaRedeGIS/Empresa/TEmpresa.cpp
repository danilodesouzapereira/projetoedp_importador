// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include "TEmpresa.h"
#include "VTCondutor.h"
#include "VTLocalidade.h"
#include "VTRegional.h"
#include "VTSubestacao.h"
#include "VTPrimario.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// -----------------------------------------------------------------------------
VTDadosEmpresa* NewObjDadosEmpresa(VTApl *apl)
{
	return (new TEmpresa(apl));
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18236926
__fastcall TEmpresa::TEmpresa(VTApl *apl)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	// cria listas
	lisCONDUTOR = new TList();
	lisREGIONAL = new TList();
	lisLOCALIDADE = new TList();
	lisSUBESTACAO = new TList();
	lisPRIMARIO = new TList();
    //default
    condutor_default.arranjo_montante = true;
}

// -----------------------------------------------------------------------------
__fastcall TEmpresa::~TEmpresa(void)
{
	// destrói lista e seus objetos
	if (lisCONDUTOR)
	{
		LimpaTList(lisCONDUTOR);
		delete lisCONDUTOR;
		lisCONDUTOR = NULL;
	}
	if (lisREGIONAL)
	{
		LimpaTList(lisREGIONAL);
		delete lisREGIONAL;
		lisREGIONAL = NULL;
	}
	if (lisLOCALIDADE)
	{
		LimpaTList(lisLOCALIDADE);
		delete lisLOCALIDADE;
		lisLOCALIDADE = NULL;
	}
	if (lisSUBESTACAO)
	{
		LimpaTList(lisSUBESTACAO);
		delete lisSUBESTACAO;
		lisSUBESTACAO = NULL;
	}
	if (lisPRIMARIO)
	{
		LimpaTList(lisPRIMARIO);
		delete lisPRIMARIO;
		lisPRIMARIO = NULL;
	}
}

// -----------------------------------------------------------------------------
bool __fastcall TEmpresa::Clear(void)
{
	// reinicia listas
	LimpaTList(lisCONDUTOR);
	LimpaTList(lisREGIONAL);
	LimpaTList(lisLOCALIDADE);
	LimpaTList(lisSUBESTACAO);
	LimpaTList(lisPRIMARIO);

	return (true);
}

// -----------------------------------------------------------------------------
VTCondutor* __fastcall TEmpresa::ExisteCondutor(int condutor_id)
{
	// variáveis locais
	VTCondutor *condutor;

	for (int n = 0; n < lisCONDUTOR->Count; n++)
	{
		condutor = (VTCondutor*)lisCONDUTOR->Items[n];
		if (condutor->id == condutor_id)
			return (condutor);
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
VTLocalidade* __fastcall TEmpresa::ExisteLocalidade(int localidade_id)
{
	// variáveis locais
	VTLocalidade *localidade;

	for (int n = 0; n < lisLOCALIDADE->Count; n++)
	{
		localidade = (VTLocalidade*)lisLOCALIDADE->Items[n];
		if (localidade->Id == localidade_id)
			return (localidade);
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
VTRegional* __fastcall TEmpresa::ExisteRegional(int regional_id)
{
	// variáveis locais
	VTRegional *regional;

	for (int n = 0; n < lisREGIONAL->Count; n++)
	{
		regional = (VTRegional*)lisREGIONAL->Items[n];
		if (regional->Id == regional_id)
			return (regional);
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
VTSubestacao* __fastcall TEmpresa::ExisteSubestacao(int subestacao_id)
{
	// variáveis locais
	VTSubestacao *subestacao;

	for (int n = 0; n < lisSUBESTACAO->Count; n++)
	{
		subestacao = (VTSubestacao*)lisSUBESTACAO->Items[n];
		if (subestacao->Id == subestacao_id)
			return (subestacao);
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
VTSubestacao* __fastcall TEmpresa::ExisteSubestacao(AnsiString cod)
{
	// variáveis locais
	VTSubestacao *subestacao;

	for (int n = 0; n < lisSUBESTACAO->Count; n++)
	{
		subestacao = (VTSubestacao*)lisSUBESTACAO->Items[n];
		if (subestacao->Codigo.AnsiCompareIC(cod) == 0)
			return (subestacao);
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
bool __fastcall TEmpresa::InsereCondutor(VTCondutor *condutor)
{
	// proteção
	if (condutor == NULL)
		return (false);
	// insere Concutor em lisCON
	if (lisCONDUTOR->IndexOf(condutor) < 0)
	{
		lisCONDUTOR->Add(condutor);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TEmpresa::InsereLocalidade(VTLocalidade *localidade)
{
	// proteção
	if (localidade == NULL)
		return (false);
	// insere na lista
	if (lisLOCALIDADE->IndexOf(localidade) < 0)
	{
		lisLOCALIDADE->Add(localidade);
		// associa com Empresa
		localidade->Empresa = this;
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TEmpresa::InsereRegional(VTRegional *regional)
{
	// proteção
	if (regional == NULL)
		return (false);
	// insere na lista
	if (lisREGIONAL->IndexOf(regional) < 0)
	{
		lisREGIONAL->Add(regional);
		// associa com Empresa
		regional->Empresa = this;
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TEmpresa::InsereSubestacao(VTSubestacao *subestacao)
{
	// proteção
	if (subestacao == NULL)
		return (false);
	// insere na lista
	if (lisSUBESTACAO->IndexOf(subestacao) < 0)
	{
		lisSUBESTACAO->Add(subestacao);
		// associa com Empresa
		subestacao->Empresa = this;
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TEmpresa::InserePrimario(VTPrimario *primario)
{
	// proteção
	if (primario == NULL)
		return (false);
	// insere na lista
	if (lisPRIMARIO->IndexOf(primario) < 0)
	{
		lisPRIMARIO->Add(primario);
		// associa com Empresa
		primario->Empresa = this;
	}
	return (true);
}

// -----------------------------------------------------------------------------
TList* __fastcall TEmpresa::LisCondutor(void)
{
	return (lisCONDUTOR);
}

// -----------------------------------------------------------------------------
TList* __fastcall TEmpresa::LisLocalidade(void)
{
	return (lisLOCALIDADE);
}

// -----------------------------------------------------------------------------
TList* __fastcall TEmpresa::LisPrimario(void)
{
	return (lisPRIMARIO);
}

// -----------------------------------------------------------------------------
TList* __fastcall TEmpresa::LisRegional(void)
{
	return (lisREGIONAL);
}

// -----------------------------------------------------------------------------
TList* __fastcall TEmpresa::LisSubestacao(void)
{
	return (lisSUBESTACAO);
}

// -----------------------------------------------------------------------------
// eof

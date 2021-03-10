// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TSubestacao.h"
#include "VTPrimario.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// -----------------------------------------------------------------------------
VTSubestacao* NewObjSubestacao(void)
{
	return (new TSubestacao());
}
// -----------------------------------------------------------------------------
__fastcall TSubestacao::TSubestacao(void)
{
	// cria lista
	PD.lisPRI = new TList();
}

// -----------------------------------------------------------------------------
__fastcall TSubestacao::~TSubestacao(void)
{
	// destroi lista sem destruir objetos
	if(PD.lisPRI) {delete PD.lisPRI; PD.lisPRI = NULL;}
}

// -----------------------------------------------------------------------------
VTPrimario*__fastcall TSubestacao::ExistePrimario(AnsiString cod_pri)
{
	// variáveis locais
	VTPrimario *primario;

	for (int n = 0; n < PD.lisPRI->Count; n++)
	{
		primario = (VTPrimario*)PD.lisPRI->Items[n];
		if (primario->Codigo.AnsiCompareIC(cod_pri) == 0)
			return (primario);
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
// eof

// ---------------------------------------------------------------------------
#ifndef TMontaEmpresaH
#define TMontaEmpresaH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMontaEmpresa.h"

// ---------------------------------------------------------------------------
class VTDadosEmpresa;

// ---------------------------------------------------------------------------
class TMontaEmpresa : public VTMontaEmpresa
{
public:
	__fastcall TMontaEmpresa(VTApl *apl);
	__fastcall ~TMontaEmpresa(void);
	bool __fastcall InsereCondutor(sdi_ARRANJO &str);
	bool __fastcall InsereLocalidade(strLOCALIDADE &str);
	bool __fastcall InserePrimario(strPRIMARIO &str);
	bool __fastcall InsereRegional(strREGIONAL &str);
	bool __fastcall InsereSubestacao(strSUBESTACAO &str);

private: // objetos externos
	AnsiString	__fastcall TraduzTipo(AnsiString tipo);
	AnsiString	__fastcall MontaCodigoCondutor(sdi_ARRANJO &str);

	VTApl *apl;
	VTDadosEmpresa *empresa;
};

#endif
// ---------------------------------------------------------------------------
// eof

// ---------------------------------------------------------------------------
#ifndef TEmpresaH
#define TEmpresaH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTDadosEmpresa.h"

// ---------------------------------------------------------------------------
class VTExtrator;
class VTLog;
class VTProgresso;

// ---------------------------------------------------------------------------
class TEmpresa : public VTDadosEmpresa
{
public:
	__fastcall TEmpresa(VTApl *apl);
	__fastcall ~TEmpresa(void);
	bool __fastcall Clear(void);
	VTCondutor* __fastcall ExisteCondutor(int condutor_id);
	VTLocalidade* __fastcall ExisteLocalidade(int localidade_id);
	VTRegional* __fastcall ExisteRegional(int regional_id);
	VTSubestacao* __fastcall ExisteSubestacao(int subestacao_id);
	VTSubestacao* __fastcall ExisteSubestacao(AnsiString cod);
	bool __fastcall InsereCondutor(VTCondutor *condutor);
	bool __fastcall InsereLocalidade(VTLocalidade *localidade);
	bool __fastcall InsereRegional(VTRegional *regional);
	bool __fastcall InsereSubestacao(VTSubestacao *subestacao);
	bool __fastcall InserePrimario(VTPrimario *primario);
	TList* __fastcall LisCondutor(void);
	TList* __fastcall LisLocalidade(void);
	TList* __fastcall LisPrimario(void);
	TList* __fastcall LisRegional(void);
	TList* __fastcall LisSubestacao(void);

private: // métodos

		private : // objetos externos

	VTApl *apl;

	// dados locais
	TList *lisCONDUTOR;
	TList *lisREGIONAL;
	TList *lisLOCALIDADE;
	TList *lisSUBESTACAO;
	TList *lisPRIMARIO;
};

#endif
// ---------------------------------------------------------------------------
// eof

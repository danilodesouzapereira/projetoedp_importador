// ---------------------------------------------------------------------------
#ifndef TImportaRedeGISH
#define TImportaRedeGISH

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>

// ---------------------------------------------------------------------------
class VTConversor;
class VTLog;
class VTDadosEmpresa;
class VTParse;
class VTSubestacao;

// ------------------------------------------------------------------------------------------- class
class TSub : public VTSub
{
public:
	__fastcall  TSub(void);
	__fastcall ~TSub(void);

};

// ---------------------------------------------------------------------------
class TImportaRedeGIS : public VTImportaRedeGIS
{
public:
	__fastcall TImportaRedeGIS(TComponent *Owner, VTApl *apl_owner);
	__fastcall ~TImportaRedeGIS(void);

	bool   __fastcall  ArquivoEgrid(void);
	bool   __fastcall  ImportaDadosEmpresa(AnsiString arq_egrid);
	bool   __fastcall  ImportaEgrid(strOPImporta *opcao, int destino);
	bool   __fastcall  InsereSubestacao(TList *lisSub);
	TList* __fastcall  LisAllSub(void);

	bool __fastcall ImportaLisArqPrimario(TStrings *files, strOPImporta *opcao);
	bool __fastcall ImportaLisArqSubestacao(TStrings *files, strOPImporta *opcao);
	void __fastcall SelecionaArqTxt(TOpenDialog *OpenDialog, TStrings *files);
	void __fastcall SelecionaLisArqTxt(TOpenDialog *OpenDialog, TStrings *files);
	bool __fastcall ShowModalFormImportaRedeGIS(void);
	bool __fastcall ShowModalFormImportaRedeGIS_SDBT(void);
	bool __fastcall ShowModalFormImportaRedeGIS_SDMT(void);

private: // métodos
	VTSub* __fastcall ExisteSub(VTSubestacao *subestacao);
	void   __fastcall IniciaLisAllSub(void);

private : // objetos externos
	TComponent *Owner;
	VTApl *apl_owner;

	// dados locais
	TList *lisSubestacao, *lisAllSub;
	VTConversor *conversor;
	VTDadosEmpresa *empresa;
};

#endif

// -----------------------------------------------------------------------------
// eof

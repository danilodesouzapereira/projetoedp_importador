// ---------------------------------------------------------------------------
#ifndef TImportaArquivoEgridH
#define TImportaArquivoEgridH

// ---------------------------------------------------------------------------------------- includes
#include <stdexcept>
#include <list>
#include <Classes.hpp>
#include <System.Classes.hpp>
#include <System.Zip.hpp>

// Plataforma include
#include <PlataformaSinap\Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>

// ---------------------------------------------------------------------------
class VTAborta;
class VTLog;
class VTProgresso;
class VTMontaEmpresa;
class VTSubestacao;
class VTPreMonta;
class EDPDAO;
class EImportaEgrid;
// --------------------------------------------------------------------------------------- namespace

// Namespace definition
//namespace sinapsis
//{
//	namespace sinap
//	{
//		namespace edp
//		{
//			class TImportaArquivoEgrid;
//		}
//
//	}
//}
//using sinapsis::sinap::edp::TImportaArquivoEgrid;

// Namespace using
using namespace std;

// ---------------------------------------------------------------------------
class TImportaArquivoEgrid : public TObject
{
public:  //property
	  __property bool BatchMode = {read=batch, write=batch};

protected:
	bool batch;
    int mes;
private:
	TZipFile *zip;
	UnicodeString arquivoConfiguracaoPath;
	UnicodeString diretorioTemporario;
	UnicodeString eGridPath;

	strOPImporta *opcao;
	TStringList *slisNomes;
    VTAborta *aborta;
	VTApl *apl;
	// VTCronometro *cronometro;
	VTMontaEmpresa *monta_empresa;
	VTPreMonta *pre_monta;
	VTProgresso *progresso;
	EDPDAO *dao;
	AnsiString nomeSe, nomeCircuito, nomeSec;

	DynamicArray<int>vet_redepri_id;
	DynamicArray<int>vet_redesec_id;
	// identificação dos relógios
	// Configuração
#define FILENAME_COMMONS	"commons.es"
#define RelogioCfgTipoChave "CFG_TipoChave"
	// SED
#define RelogioEtdChave     "SED_Chave"
#define RelogioEtdTrafo     "SED_Trafo"
#define RelogioEtdTrafo3E   "SED_Trafo3E"
	// SDMT
#define RelogioPriCapacitor  "SDMT_Capacitor"
#define RelogioPriCarga      "SDMT_Consumidor"
#define RelogioPriChave      "SDMT_Chave"
#define RelogioPriRede       "SDMT_Rede"
#define RelogioPriRegulador  "SDMT_Regulador"
#define RelogioPriTrecho     "SDMT_Trecho"
	// SDBT
#define RelogioSecCarga      "SDBT_Consumidor"
#define RelogioSecIP         "SDBT_IP"
#define RelogioSecRede       "SDBT_Rede"
#define RelogioSecTrafo      "SDBT_Trafo"
#define RelogioSecTrecho     "SDBT_Trecho"

	// versão
#define VERSAO_EGRID 1.18

public:
	// VTPreMonta *pre_monta;

	/**
	 *Construtor
	 *@param eGridPath O caminho completo até o arquivo .egrid.
	 *@throws runtime_error Lança uma exceção caso nao seja possivel abrir o arquivo.
	 */
	__fastcall TImportaArquivoEgrid(VTApl * apl, UnicodeString eGridPath) throw(runtime_error);

	/**
	 *Destrutor
	 */
	__fastcall ~TImportaArquivoEgrid(void);

	/**
	 *Fecha o importador.</br>
	 *Ao fechar o importador por este metodo, todos os arquivos temporarios criados por ele serao
	 *excluidos do disco do usuario.
	 */
	void __fastcall Close(void);
	void __fastcall CriaDAO(AnsiString nomeArq);
	void __fastcall DestroiDAO(void);
	void __fastcall Unzip(TList *lisTMP);
	bool __fastcall ExisteSubZip(AnsiString sigla_sub);

	TStringList* __fastcall GetSubestacaoList(void);
	//std::list<UnicodeString>__fastcall GetAlimentadorList(UnicodeString siglaSubestacao);

	//
	bool __fastcall InsereArranjo(void);
	//
	bool __fastcall LeCfgArranjo(void) throw(EImportaEgrid);
	bool __fastcall LeCfgLocalidade(void) throw(EImportaEgrid);
	bool __fastcall LeCfgPrimario(void)throw(EImportaEgrid);
	bool __fastcall LeCfgRegional(void) throw(EImportaEgrid);
	bool __fastcall LeCfgSubestacao(void) throw(EImportaEgrid);
	bool __fastcall LeRedes(TList *lisSUBESTACAO, strOPImporta *opcao);
	bool __fastcall LeRedes(VTSubestacao *subestacao, strOPImporta *opcao);

private: // métodos
	void __fastcall IniciaZip(void);
	bool __fastcall IniciaVetorRedePriId(int etd_id);
	bool __fastcall IniciaVetorRedeSecId(int pri_id);
    bool __fastcall LeMesExtracao(void);
	bool __fastcall LeUmaRedeEtd_TodasRedesPriSec(int etd_id) throw(EImportaEgrid);
	bool __fastcall LeUmaRedePri_TodasRedesSec(int pri_id) throw(EImportaEgrid);
	bool __fastcall LeUmaRedeSec_Completa(int sec_id);
	bool __fastcall LeUmaRedeSec_SomenteCarga(int sec_id);
	bool __fastcall LeUmaRedeSec_TrafoCarga(int sec_id);
	bool __fastcall PrimarioSelecionado(int id_pri);

	// métodos de leitura de tabelas
	// bool __fastcall ReadCfg_TabArranjo(void);
	bool __fastcall ReadCfg_TabImpedanciaCurto(int etd_id);
	// etd
	bool __fastcall ReadEtd_TabBarramento(int etd_id);
	bool __fastcall ReadEtd_TabSe(int etd_id);
	bool __fastcall ReadEtd_TabSocorro();
	bool __fastcall ReadEtd_TabTrafoSe(int etd_id);
	// rede primária
	bool __fastcall ReadPri_TabBarra(int pri_id);
	bool __fastcall ReadPri_TabCapacitorParalelo(int pri_id);
	bool __fastcall ReadPri_TabChave(int pri_id);
	bool __fastcall ReadPri_TabRede(int pri_id);
	bool __fastcall ReadPri_TabConsMT(int pri_id);
	bool __fastcall ReadPri_TabGerador(int pri_id);
	bool __fastcall ReadPri_TabLink(int pri_id);
	bool __fastcall ReadPri_TabRegulador(int pri_id);
	bool __fastcall ReadPri_TabSocorro(int pri_id);
	bool __fastcall ReadPri_TabTrecho(int pri_id);
	// rede secundária
	bool __fastcall ReadSec_TabBarraBT(int sec_id);
	bool __fastcall ReadSec_TabConsBT(int sec_id);
	bool __fastcall ReadSec_TabConsIP(int sec_id);
	bool __fastcall ReadSec_TabGeradorBT(int sec_id);
	bool __fastcall ReadSec_TabTrafoMTBT(int sec_id);
	bool __fastcall ReadSec_TabLinkBT(int sec_id);
   	bool __fastcall ReadSec_TabPCargaBT(int sec_id);
	bool __fastcall ReadSec_TabTrechoBT(int sec_id);
	bool __fastcall ReadSec_TabUtcBT(int sec_id);
	// função de teste
	int __fastcall FieldAsInteger(AnsiString fieldname);
	bool __fastcall VersaoCompativel(AnsiString &versao);

	void __fastcall DefinirDiretorioTemporario(void);
	UnicodeString GetPath(UnicodeString siglaSubestacao);
	void __fastcall Nomes(TStringList *slisNomes);

};

#endif
// ---------------------------------------------------------------------------
// eof

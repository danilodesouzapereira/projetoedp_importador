// --------------------------------------------------------------------------------------------- bof
#ifndef EDPDAOH
#define EDPDAOH

// ---------------------------------------------------------------------------------------- includes
#include <list>
#include <Classes.hpp>

// Project include
//#include <SQLite\SQLite.h>
#include <PlataformaSinap\Fontes\LocalDB\SQLite\VTSQLite.h>
class EImportaEgridDAO;

// --------------------------------------------------------------------------------------- namespace
/*namespace sinapsis
{
	namespace sinap
	{
		namespace edp
		{
			class EDPDAO;
		}

	}
}  */

using namespace std;
//using sinapsis::sinap::edp::EDPDAO;

// ------------------------------------------------------------------------------------------- class
class EDPDAO : public TObject
{

private:
	VTSQLiteDB *sqLite;

	VTSQLiteTable* __fastcall ExecuteSql(UnicodeString sql) throw (EImportaEgridDAO);
	int 		__fastcall ExecuteDML(UnicodeString sql) throw (EImportaEgridDAO);
public:
	__fastcall EDPDAO(UnicodeString filePathSED,UnicodeString filePathCOMMONS);
	__fastcall EDPDAO(UnicodeString filePath);
	__fastcall ~EDPDAO(void);
	void __fastcall Close(void);
	//GERAL
	VTSQLiteTable* __fastcall ExecuteSqlArranjo() throw (EImportaEgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlLocalidade() throw (EImportaEgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlRegional() throw (EImportaEgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlSub() throw (EImportaEgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlSub(int idSubestacao) throw (EImportaEgridDAO);
	// ETD
	VTSQLiteTable* __fastcall ExecuteSqlSubBarra(int idSubestacao)  throw (EImportaEgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlSubPri(void) throw (EImportaEgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlSubPri(int idSubestacao) throw (EImportaEgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlSubSocorro(AnsiString cmd_sql) throw (EImportaEgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlSubTrafo(int idSubestacao) throw (EImportaEgridDAO);
    VTSQLiteTable* __fastcall ExecuteSqlSubImpedancias(int idSubestacao) throw(EImportaEgridDAO);
	// PRIMÁRIO
	VTSQLiteTable* __fastcall ExecuteSqlPriBarra(int idAlimentador)throw (EImportaEgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlPriCapacitorParalelo(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlPriChave(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlPriRede(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlPriConsMT(int idAlimentador) throw (EImportaEgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlPriGerador(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlPriLink(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlPriRegulador(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlPriSocorro(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlPriTrecho(int idAlimentador);
    // SECUNDÁRIO
	VTSQLiteTable* __fastcall ExecuteSqlSecBarra(int idSec) throw(EImportaEgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlSecConsBT(int idSec);
	VTSQLiteTable* __fastcall ExecuteSqlSecConsIP(int idSec);
	VTSQLiteTable* __fastcall ExecuteSqlSecGeradorBT(int idSecundario);
	VTSQLiteTable* __fastcall ExecuteSqlSecLinkBT(int idSecundario);
	VTSQLiteTable* __fastcall ExecuteSqlSecPCargaBT(int idSecundario);
	VTSQLiteTable* __fastcall ExecuteSqlSecRede(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlSecTrafo(int idSec);
	VTSQLiteTable* __fastcall ExecuteSqlSecTrecho(int idSec);
	VTSQLiteTable* __fastcall ExecuteSqlSecUtcBT(int idSecundario);

	//VTSQLiteTable* __fastcall GetAlimentadorList(UnicodeString siglaSubestacao);

	VTSQLiteTable* __fastcall ExecuteSql_RedesMT();
	VTSQLiteTable* __fastcall ExecuteSql_ChavesSocRedesMT();
	VTSQLiteTable* __fastcall ExecuteSql_ChavesRedesMT();
};
#endif

// --------------------------------------------------------------------------------------------- eof

// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <list>

// Project include
// #include <SQLite\SQLite.h>
#include <PlataformaSinap\DLL_Inc\LocalDB.h>
#include <PlataformaSinap\Fontes\LocalDB\SQLite\VTSQLite.h>
#include <PlataformaSinap\Fontes\LocalDB\SQLite\ESQLite.h>
#include <Exceptions\EImportaEgrid.h>
#include <EDPDAO.h>

// --------------------------------------------------------------------------------------- namespace
using namespace std;
//using namespace sinapsis::sinap::edp;

// ------------------------------------------------------------------------------------------- class
__fastcall EDPDAO::EDPDAO(UnicodeString filePathSED, UnicodeString filePathCOMMONS)
{
	AnsiString sql;

	sqLite = DLL_NewObjSQLiteDB();
	sqLite->LoadAttach(AnsiString(filePathSED), AnsiString(filePathCOMMONS), "COMMONS");

}

// -------------------------------------------------------------------------------------------
__fastcall EDPDAO::EDPDAO(UnicodeString filePath)
{
	AnsiString sql;

	sqLite = DLL_NewObjSQLiteDB();
	sqLite->Load(AnsiString(filePath));
}

// -------------------------------------------------------------------------------------------------
__fastcall EDPDAO::~EDPDAO(void)
{
	if (sqLite != NULL)
	{
		delete sqLite;
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall EDPDAO::Close(void)
{
	sqLite->Close();                                                                                                    }

// -------------------------------------------------------------------------------------------------
int __fastcall EDPDAO::ExecuteDML(UnicodeString sql) throw(EImportaEgridDAO)
{
	// return(sqLite->execDML(AnsiString(sql).c_str()));
	try
	{
		return (sqLite->ExecDML(AnsiString(sql).c_str()));
	}
	catch (ESQLite &e)
	{
		throw EImportaEgridDAO("Erro na ExecuteDML.");
	}

}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSql(UnicodeString sql) throw(EImportaEgridDAO)
{
	// return(sqLite->getTable(AnsiString(sql).c_str()));
	try
	{
		return (sqLite->GetTable(AnsiString(sql).c_str()));
	}
	catch (ESQLite &e)
	{
		throw;
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlArranjo() throw(EImportaEgridDAO)
{
	try
	{
		return ExecuteSql("SELECT * FROM TB_ARRANJO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaEgridDAO("Erro na query de arrajo.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlLocalidade() throw(EImportaEgridDAO)
{
	try
	{
		return ExecuteSql("SELECT * FROM TB_LOCALIDADE;");
		// return ExecuteSql("SELECT * FROM TB_LOCASASALIDADE;"); //teste
	}
	catch (ESQLite &e)
	{
		throw EImportaEgridDAO("Erro na query de localidade.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlRegional() throw(EImportaEgridDAO)
{
	try
	{
		return ExecuteSql("SELECT * FROM TB_REGIONAL;");
	}
	catch (ESQLite &e)
	{
		throw EImportaEgridDAO("Erro na query de regional. ");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSub() throw(EImportaEgridDAO)
{
	try
	{
		return ExecuteSql
			("select * from TB_SED inner join TB_REDE on TB_REDE.ID_REDE = "
			" TB_SED.ID_SED ORDER BY NOME");
	}
	// catch(...)
	catch (ESQLite &e)
	{
		throw EImportaEgridDAO("Erro na query de subestações. ");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSub(int idSubestacao) throw(EImportaEgridDAO)
{
	try
	{
		return ExecuteSql
		("select SED.*, REDE.*, AUX.UTMX_M AS UTMX_M, AUX.UTMY_M AS UTMY_M from commons.TB_SED SED "
		 "inner join commons.TB_REDE REDE on REDE.ID_REDE = SED.ID_SED "
		 "inner join "
		 "("
		 "select distinct commons.TB_TRAFO_SED.ID_SED, TB_BARRAMENTO.UTMX_M, TB_BARRAMENTO.UTMY_M from TB_BARRAMENTO "
		 "inner join commons.TB_TRAFO_SED on commons.TB_TRAFO_SED.ID_INSTALACAO = TB_BARRAMENTO.ID_INSTALACAO "
		 ") AUX "
		 "ON aux.ID_SED = SED.ID_SED "
		 "WHERE SED.ID_SED=" + IntToStr(idSubestacao));
	}
	// catch(...)
	catch (ESQLite &e)
	{
		throw EImportaEgridDAO("Erro na query de subestações com where=" + IntToStr(idSubestacao)
			+ ". ");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSubBarra(int idSubestacao) throw(EImportaEgridDAO)
{
	try
	{
		return ExecuteSql
			("SELECT DISTINCT TB_BARRAMENTO.* from commons.TB_TRAFO_SED INNER JOIN"
			" TB_BARRAMENTO ON commons.TB_TRAFO_SED.ID_INSTALACAO = TB_BARRAMENTO.ID_INSTALACAO"
			" WHERE (commons.TB_TRAFO_SED.ID_SED = " + IntToStr(idSubestacao) + ")");
	}
	// catch(...)
	catch (ESQLite &e)
	{
		throw EImportaEgridDAO("Erro na query de barras da subestações id=" + IntToStr(idSubestacao)
			+ ". ");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSubImpedancias(int idSubestacao) throw(EImportaEgridDAO)
{
	try
	{
		return ExecuteSql
			("SELECT DISTINCT * FROM ("
			 "select commons.TB_TRAFO_SED.ID_INSTALACAO, commons.TB_TRAFO_SED.ID_BARRA_AT,"
			 " commons.TB_TRAFO_SED.RCC_AT, commons.TB_TRAFO_SED.XCC_AT, TB_REDE.CODIGO,"
			 " commons.TB_TRAFO_SED.VALTA_KV from commons.TB_TRAFO_SED"
			 " INNER JOIN commons.TB_REDE on commons.TB_REDE.ID_REDE = commons.TB_TRAFO_SED.ID_SED"
			 " WHERE commons.TB_TRAFO_SED.ID_SED = " + IntToStr(idSubestacao) + ")");
	}
	// catch(...)
	catch (ESQLite &e)
	{
		throw EImportaEgridDAO("Erro na query de impedancias de curto id=" + IntToStr(idSubestacao)
			+ ". ");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSubPri(void) throw(EImportaEgridDAO)
{
	try
	{
		return ExecuteSql
			("select distinct * from ("
			"select MT.*, TRAFO.ID_SED from TB_REDE_MT MT"
			" inner join TB_TRAFO_SED TRAFO on TRAFO.ID_INSTALACAO = MT.ID_INSTALACAO" " inner join TB_REDE rede on rede.id_rede = MT.id_rede_mt "
			" ORDER BY NOME)");
	}
	// catch(...)
	catch (ESQLite &e)
	{
		throw EImportaEgridDAO("Erro na query dos primários. ");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSubPri(int idSubestacao) throw(EImportaEgridDAO)
{
	try
	{
		return ExecuteSql("select distinct * from ("
			"select MT.*, TRAFO.ID_SED from commons.TB_REDE_MT MT"
			" inner join commons.TB_TRAFO_SED TRAFO on TRAFO.ID_INSTALACAO = MT.ID_INSTALACAO" " inner join commons.TB_REDE rede on rede.id_rede = MT.id_rede_mt"
			" where TRAFO.ID_SED = " + IntToStr(idSubestacao) + ")");
	}
	// catch(...)
	catch (ESQLite &e)
	{
		throw EImportaEgridDAO("Erro na query dos primários da subestacao id = " +
			IntToStr(idSubestacao));
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSubSocorro(AnsiString cmd_sql) throw(EImportaEgridDAO)
{
	try
	{
		return ExecuteSql("select * from TB_CHAVE_SOCORRO"
	      " where ID_REDE_MT1 IN" + cmd_sql);
	}
catch (ESQLite &e)
	{
		throw EImportaEgridDAO("Erro na query das chaves primárias. ");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSubTrafo(int idSubestacao) throw(EImportaEgridDAO)
{
	try
	{
		return ExecuteSql("select * from commons.TB_TRAFO_SED " "where ID_SED =" +
			IntToStr(idSubestacao));
	}
	// catch(...)
	catch (ESQLite &e)
	{
		throw EImportaEgridDAO("Erro na query dos transformadores de subestação. ");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlPriBarra(int idAlimentador) throw(EImportaEgridDAO)
{
	try
	{
		return ExecuteSql
			("select distinct * from("
			"select distinct tb_barra.* from tb_barra "
			"inner join tb_trecho on "
			"((tb_trecho.id_barra1 = tb_barra.ID_BARRA) or "
			"(tb_trecho.id_barra2 = tb_barra.ID_BARRA))"
			"where tb_trecho.ID_REDE = " + IntToStr(idAlimentador) +
			" union "
			"select distinct tb_barra.* from tb_barra "
			"inner join tb_link on "
			"((tb_link.id_barra1 = tb_barra.ID_BARRA) or "
			"(tb_link.id_barra2 = tb_barra.ID_BARRA)) "
			"where tb_link.ID_REDE = " + IntToStr(idAlimentador) +
			" union "
			"select distinct tb_barra.* from tb_barra "
			"inner join tb_trafo_mtbt on "
			"tb_trafo_mtbt.id_barra_mt = tb_barra.ID_BARRA "
			"where tb_trafo_mtbt.ID_REDE_MT = " + IntToStr(idAlimentador) +
//			" union "
//			"select distinct tb_barra.* from tb_barra "
//			"inner join tb_regulador_tensao on "
//			"((tb_regulador_tensao.id_barra1 = tb_barra.ID_BARRA) or "
//			"(tb_regulador_tensao.id_barra2 = tb_barra.ID_BARRA)) "
//			"where tb_regulador_tensao.ID_REDE_MT = " + IntToStr(idAlimentador) +
//			" union "
//			"select distinct tb_barra.* from tb_barra "
//			"inner join tb_chave_socorro on "
//			"((tb_chave_socorro.id_barra1 = tb_barra.ID_BARRA) or "
//			"(tb_chave_socorro.id_barra2 = tb_barra.ID_BARRA)) "
//			"where ((tb_chave_socorro.ID_REDE_MT1 = " + IntToStr(idAlimentador) +
//			") or (tb_chave_socorro.ID_REDE_MT2 = " + IntToStr(idAlimentador) +
//			")) union "
//			"select distinct tb_barra.* from tb_barra "
//			"inner join tb_chave on "
//			"((tb_chave.id_barra1 = tb_barra.ID_BARRA) or "
//			"(tb_chave.id_barra2 = tb_barra.ID_BARRA)) "
//			"where tb_chave.ID_REDE_MT = " + IntToStr(idAlimentador) +
//			" union "
//			"select distinct tb_barra.* from tb_barra "
//			"inner join tb_carga_mt on "
//			"(tb_carga_mt.id_barra = tb_barra.ID_BARRA)"
//			"where tb_carga_mt.ID_REDE_MT = " + IntToStr(idAlimentador) +
//			" union "
//			"select distinct tb_barra.* from tb_barra "
//			"inner join tb_capacitor on "
//			"(tb_capacitor.id_barra = tb_barra.ID_BARRA)"
//			"where tb_capacitor.ID_REDE = " + IntToStr(idAlimentador) +
//			" union "
//			"select distinct tb_barra.* from tb_barra "
//			"inner join tb_gerador on "
//			"((tb_gerador.id_barra1 = tb_barra.ID_BARRA) or "
//			"(tb_gerador.id_barra2 = tb_barra.ID_BARRA)) "
//			"where tb_gerador.ID_REDE = " + IntToStr(idAlimentador) +
			")");

	}
	// catch(...)
	catch (ESQLite &e)

	{
		throw EImportaEgridDAO("Erro na query das barras do primario. ");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlPriCapacitorParalelo(int idAlimentador)
{

	return ExecuteSql("select * from TB_CAPACITOR where ID_REDE = " + IntToStr(idAlimentador));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlPriRede(int idAlimentador)
{

	return ExecuteSql
		("select distinct MT.* from commons.TB_REDE_MT MT"
		" LEFT JOIN commons.TB_TRAFO_SED TRAFO ON  MT.ID_INSTALACAO = TRAFO.ID_INSTALACAO" " where ID_REDE_MT = " +
		IntToStr(idAlimentador));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSql_RedesMT()
{
	return ExecuteSql("select * from TB_REDE_MT");
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSql_ChavesSocRedesMT()
{
	return ExecuteSql("select * from TB_CHAVE_SOCORRO");
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSql_ChavesRedesMT()
{
	return ExecuteSql("select * from TB_CHAVE");
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlPriConsMT(int idAlimentador) throw(EImportaEgridDAO)
{
	try
	{
	return ExecuteSql("select * FROM TB_CARGA_MT "
					   "WHERE ID_REDE_MT = " + IntToStr(idAlimentador));
	}
    catch (ESQLite &e)
	{
		throw EImportaEgridDAO("Erro na query dos Consumidores Primários. ");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlPriChave(int idAlimentador)
{
	return ExecuteSql("select * from TB_CHAVE where ID_REDE_MT = " + IntToStr(idAlimentador));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlPriGerador(int idAlimentador)
{
	// Verificar que a tabela: novosit_mapping_eletricidade.elec_dist_primary$path
	// está sendo lida
	return ExecuteSql("select * from TB_GERADOR where ID_REDE = " + IntToStr(idAlimentador));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlPriLink(int idAlimentador)
{
	return ExecuteSql("select * from TB_LINK WHERE ID_REDE = " + IntToStr(idAlimentador));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlPriRegulador(int idAlimentador)
{
	return ExecuteSql("select * from TB_REGULADOR_TENSAO where ID_REDE_MT = " + IntToStr(idAlimentador));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlPriSocorro(int idAlimentador)
{
	return ExecuteSql("select * from TB_CHAVE_SOCORRO "
	"where  (ID_REDE_MT1 = " + IntToStr(idAlimentador) +
	" or ID_REDE_MT2 = " + IntToStr(idAlimentador) +
    ")");
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlPriTrecho(int idAlimentador)
{
	return ExecuteSql("select * from TB_TRECHO WHERE ID_REDE = " + IntToStr(idAlimentador));
}

// REDE SECUNDÁRIA
// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSecBarra(int idSecundario) throw(EImportaEgridDAO)
{
	try
	{
	return ExecuteSql("select distinct * from ( "
			"select distinct tb_barra.*, tb_trecho.ID_REDE as ID_REDE_BT from tb_barra "
			"inner join tb_trecho on "
			"((tb_trecho.id_barra1 = tb_barra.ID_BARRA) or "
			"(tb_trecho.id_barra2 = tb_barra.ID_BARRA)) "
			"where tb_trecho.ID_REDE = " + IntToStr(idSecundario) +
			" union "
			"select distinct tb_barra.*, tb_link.ID_REDE as ID_REDE_BT from tb_barra "
			"inner join tb_link on "
			"((tb_link.id_barra1 = tb_barra.ID_BARRA) or "
			"(tb_link.id_barra2 = tb_barra.ID_BARRA))"
			"where tb_link.ID_REDE = " + IntToStr(idSecundario) +
			" union "
			"select distinct tb_barra.*, tb_trafo_mtbt.ID_REDE_BT as ID_REDE_BT from tb_barra "
			"inner join tb_trafo_mtbt on "
			"(tb_trafo_mtbt.id_barra_bt = tb_barra.ID_BARRA) "
			"where tb_trafo_mtbt.ID_REDE_BT = " + IntToStr(idSecundario) +
			" union "
//			"select distinct tb_barra.*, tb_capacitor.ID_REDE as ID_REDE_BT from tb_barra "
//			"inner join tb_capacitor on "
//			"(tb_capacitor.id_barra = tb_barra.ID_BARRA) "
//			"where tb_capacitor.ID_REDE = " + IntToStr(idSecundario) +
//			" union "
//			"select distinct tb_barra.*, tb_gerador.ID_REDE as ID_REDE_BT from tb_barra "
//			"inner join tb_gerador on "
//			"((tb_gerador.id_barra1 = tb_barra.ID_BARRA) or "
//			"(tb_gerador.id_barra2 = tb_barra.ID_BARRA)) "
//			"where tb_gerador.ID_REDE = " + IntToStr(idSecundario) +
//			" union "
//			"select distinct tb_barra.*, tb_carga_ip.ID_REDE_BT as ID_REDE_BT from tb_barra "
//			"inner join tb_carga_ip on "
//			"(tb_carga_ip.id_barra = tb_barra.ID_BARRA) "
//			"where tb_carga_ip.ID_REDE_BT = " + IntToStr(idSecundario) +
//			" union "
			"select distinct tb_barra.*, tb_pcarga_bt.ID_REDE_BT as ID_REDE_BT from tb_barra "
			"inner join tb_pcarga_bt on "
			"(tb_pcarga_bt.id_barra = tb_barra.ID_BARRA) "
			"where tb_pcarga_bt.ID_REDE_BT = " + IntToStr(idSecundario) +
			")");
	}
// catch(...)
	catch (ESQLite &e)
	{
		throw EImportaEgridDAO("Erro na query de barras do secundário");
	}

	}


// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSecConsBT(int idSecundario)
{
	return ExecuteSql("select * FROM TB_CARGA_BT "
	       "where ID_REDE_BT = " + IntToStr(idSecundario));

}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSecConsIP(int idSecundario)
{
	return ExecuteSql
		("select * FROM TB_CARGA_IP "
		 "where ID_REDE_BT = " + IntToStr(idSecundario));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSecGeradorBT(int idSecundario)
{
	return ExecuteSql("select * from " "TB_GERADOR WHERE ID_REDE = " + IntToStr(idSecundario));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSecLinkBT(int idSecundario)
{
	return ExecuteSql("select * from TB_LINK WHERE ID_REDE = "
		+ IntToStr(idSecundario));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSecPCargaBT(int idSecundario)
{
	return ExecuteSql("select * FROM TB_PCARGA_BT "
	       "where ID_REDE_BT = " + IntToStr(idSecundario));

}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSecRede(int idAlimentador)
{
	return ExecuteSql
		("select distinct ID_REDE from commons.TB_REDE BT"
		" LEFT JOIN TB_TRAFO_MTBT TRAFO ON BT.ID_REDE = TRAFO.ID_REDE_BT"
		" where ID_REDE_MT = " + IntToStr(idAlimentador));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSecTrafo(int idSecundario)
{
	return ExecuteSql(
	"select TB_TRAFO_MTBT.*, commons.TB_REDE.CODIGO as COD_REDE "
	"from TB_TRAFO_MTBT LEFT JOIN commons.TB_REDE ON "
	"commons.TB_REDE.ID_REDE = TB_TRAFO_MTBT.ID_REDE_BT "
	"where ID_REDE_BT = " + IntToStr(idSecundario) + " ORDER BY SNOM_KVA DESC LIMIT 1");
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSecUtcBT(int idSecundario)
{
	return ExecuteSql(
	"select * from TB_TRAFO_MTBT "
	"where ID_REDE_BT = " + IntToStr(idSecundario) + " ORDER BY SNOM_KVA DESC");
}


// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall EDPDAO::ExecuteSqlSecTrecho(int idSecundario)
{
	return ExecuteSql("select * from TB_TRECHO " "where ID_REDE = " + IntToStr(idSecundario));
}


// --------------------------------------------------------------------------------------------- eof

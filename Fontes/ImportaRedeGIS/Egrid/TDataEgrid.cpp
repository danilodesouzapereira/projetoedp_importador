//---------------------------------------------------------------------------
#pragma hdrstop
#include "TDataEgrid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
#include <algorithm>
#include <inifiles.hpp>
#include <DateUtils.hpp>
#include <StrUtils.hpp>
//---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include "..\Egrid\EDPDAO.h"
#include "..\PreMonta\ModeloRede.h"
#include "..\Exceptions\EImportaEgrid.h"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
__fastcall TDataEgrid::TDataEgrid(VTApl *apl)
{
	this->apl = apl;
	dao = NULL;
	listaStrRedes = new TList;
}
//---------------------------------------------------------------------------
__fastcall TDataEgrid::~TDataEgrid(void)
{
	if (dao)       {dao->Close(); delete dao; dao = NULL;}
	if (slisNomes) {delete slisNomes; slisNomes = NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TDataEgrid::ObtemDados(StrDadosEgrid* dadosEgrid)
{
	eGridPath = dadosEgrid->pathArquivoEgrid;

	// Descompacta EGRID
	AnsiString msgErro = "";
	UnZip(dadosEgrid, msgErro);

	// Lê arquivo commons.ES
	LeCommons_es(msgErro);

	// Lê arquivos .ES específicos
	LeChavesFaltantes(dadosEgrid, msgErro);
}
//---------------------------------------------------------------------------
void __fastcall TDataEgrid::LeChavesFaltantes(StrDadosEgrid* dadosEgrid, AnsiString &msgErro)
{
	for(int i=0; i<dadosEgrid->lisStrDadosChvRede->Count; i++)
	{
		StrDadosChavesRede* dadosChvRede =
		(StrDadosChavesRede*)dadosEgrid->lisStrDadosChvRede->Items[i];

      // Pega o path do arquivo ES de uma subestação
		AnsiString codSub = dadosChvRede->codSub;

      // Pega os parâmetros das chaves que ainda não estão no MDB
		TStringList* lisCodChvExist = dadosChvRede->lisCodChvExistMDB;
		TList* lisStrChvAdic = dadosChvRede->lisStrChvAdicionar;
		LeChaves_es(codSub, lisCodChvExist, lisStrChvAdic, msgErro);
	}
}
//---------------------------------------------------------------------------
void __fastcall TDataEgrid::LeChaves_es(
AnsiString codSub, TStringList* lisCodChvExist,
TList* lisEXT, AnsiString &msgErro)
{
	AnsiString pathArqEs = ObtemPathArquivoSub_es(codSub);
	LeChavesSocorro_es(pathArqEs, lisCodChvExist, lisEXT, msgErro);
	LeChavesRede_es(pathArqEs, lisCodChvExist, lisEXT, msgErro);
}
//---------------------------------------------------------------------------
void __fastcall TDataEgrid::LeChavesRede_es(
AnsiString pathArqEs, TStringList* lisCodChvExist,
TList* lisEXT, AnsiString &msgErro)
{
	dao = new EDPDAO(pathArqEs);

	VTSQLiteTable *table;
	bool sucesso = true;
	StrChvAdicionar* strChvAdic;

	// Executa query solicitando o registro de todas as
	// chaves de uma determinada rede MT
	try
	{
		table = dao->ExecuteSql_ChavesRedesMT();
	}
	catch(EImportaEgridDAO &e)
	{
		table = NULL;
		msgErro += " Erro ao executar a query em TB_CHAVE.";
		sucesso = false;
		return;
	}

	// Pega os parâmetros das chaves
	if(sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);

				// Se a chave já está no MDB, pula.
				AnsiString codChave = table->GetStringField("CODIGO");
				if(lisCodChvExist->IndexOf(codChave) > -1) continue;

				strChvAdic = new StrChvAdicionar;
				strChvAdic->id_rede1 = table->GetIntField("ID_REDE_MT");
				strChvAdic->id_rede2 = table->GetIntField("ID_REDE_MT");
				strChvAdic->id_barra1 = table->GetIntField("ID_BARRA1");
				strChvAdic->id_barra2 = table->GetIntField("ID_BARRA2");
				strChvAdic->idChave = table->GetIntField("ID_CHAVE");
				strChvAdic->codigoChave = table->GetStringField("CODIGO");
				strChvAdic->telecomando = table->GetIntField("TELECOMANDO");
				strChvAdic->estado = table->GetIntField("ESTADO");
				strChvAdic->estado_normal = table->GetIntField("ESTADO_NORMAL");
				strChvAdic->inom_a = table->GetFloatField("INOM_A");
				lisEXT->Add(strChvAdic);
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			msgErro += " Coluna inexistente em TB_CHAVE.";
			sucesso = false;
			return;
		}
		catch (Exception &e)
		{ // erro inesperado
			msgErro += " Erro inesperado em TB_CHAVE.";
			sucesso = false;
         return;
		}
	}

	delete dao; dao = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TDataEgrid::LeChavesSocorro_es(
AnsiString pathArqEs, TStringList* lisCodChvExist,
TList* lisEXT, AnsiString &msgErro)
{
	dao = new EDPDAO(pathArqEs);

	VTSQLiteTable *table;
	bool sucesso = true;
	StrChvAdicionar* strChvAdic;

	// Executa query solicitando o registro de todas as
	// chaves de uma determinada rede MT
	try
	{
		table = dao->ExecuteSql_ChavesSocRedesMT();
	}
	catch(EImportaEgridDAO &e)
	{
		table = NULL;
		msgErro += " Erro ao executar a query em TB_CHAVE_SOCORRO.";
		sucesso = false;
		return;
	}

	// Pega os parâmetros das chaves
	if(sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);

				// Se a chave já está no MDB, pula.
				AnsiString codChave = table->GetStringField("CODIGO");
				if(lisCodChvExist->IndexOf(codChave) > -1) continue;

				strChvAdic = new StrChvAdicionar;
				strChvAdic->id_rede1 = table->GetIntField("ID_REDE_MT1");
				strChvAdic->id_rede2 = table->GetIntField("ID_REDE_MT2");
				strChvAdic->id_barra1 = table->GetIntField("ID_BARRA1");
				strChvAdic->id_barra2 = table->GetIntField("ID_BARRA2");
				strChvAdic->idChave = table->GetIntField("ID_CHAVE");
				strChvAdic->codigoChave = table->GetStringField("CODIGO");
				strChvAdic->telecomando = table->GetIntField("TELECOMANDO");
				strChvAdic->estado = table->GetIntField("ESTADO");
				strChvAdic->estado_normal = table->GetIntField("ESTADO_NORMAL");
				strChvAdic->inom_a = table->GetFloatField("INOM_A");
				lisEXT->Add(strChvAdic);
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			msgErro += " Coluna inexistente em TB_CHAVE_SOCORRO.";
			sucesso = false;
			return;
		}
		catch (Exception &e)
		{ // erro inesperado
			msgErro += " Erro inesperado em TB_CHAVE_SOCORRO.";
			sucesso = false;
         return;
		}
	}

	delete dao; dao = NULL;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TDataEgrid::ObtemPathArquivoSub_es(AnsiString codSub)
{
	AnsiString pathArq_es = diretorioTemporario + "\\Sub_" + codSub + ".es";
   return (pathArq_es);
}
//---------------------------------------------------------------------------
void __fastcall TDataEgrid::LeCommons_es(AnsiString &msgErro)
{
	AnsiString pathArqCommons = diretorioTemporario + "\\commons.es";
	dao = new EDPDAO(pathArqCommons);

	VTSQLiteTable *table;
	bool sucesso = true;
	sdi_REDE_MT* sdi;

	// Executa query solicitando o registro de todos as redes MT
	try
	{
		table = dao->ExecuteSql_RedesMT();
	}
	catch(EImportaEgridDAO &e)
	{
		table = NULL;
		msgErro += " Erro ao executar a query em ReadPri_TabRede.";
		sucesso = false;
		return;
	}

	// Pega os parâmetros de todas as redes MT
	if(sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi = new sdi_REDE_MT;
				sdi->id = table->GetIntField("ID_REDE_MT");
				sdi->codigo = table->GetStringField("NOME");
				sdi->nome = table->GetStringField("NOME");
				listaStrRedes->Add(sdi);
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			msgErro += " Coluna inexistente em ReadPri_TabRede.";
			sucesso = false;
			return;
		}
		catch (Exception &e)
		{ // erro inesperado
			msgErro += " Erro inesperado em ReadPri_TabRede.";
			sucesso = false;
         return;
		}
	}

	delete dao; dao = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TDataEgrid::UnZip(StrDadosEgrid* dadosEgrid, AnsiString &msgErro)
{
	// Cria diretório temporário
	AnsiString diretorioTmp = ExtractFilePath(Application->ExeName) + "\\tmp";
	if(!DirectoryExists(diretorioTmp)) {CreateDir(diretorioTmp);}

	// Abre o arquivo EGRID
	zip = new TZipFile();
	try
	{
		zip->Open(eGridPath, zmRead);
	}
	catch (const Exception & e)
	{
		msgErro += "Arquivo inválido ou corrompido! (" + e.Message + ")";
		return;
	}

	// Pega os nomes dos arquivos ES dentro do EGRID
	if(!slisNomes)
		slisNomes = new TStringList();
	slisNomes->Clear();
	NomesArquivosES(slisNomes);

   // Extrai os arquivos ES do ZIP
	for (int i=0; i<zip->FileCount; i++)
	{
		AnsiString fileName = zip->FileNames[i];
		if(!AnsiContainsStr(fileName, "Sub_")) continue;
		zip->Extract(zip->FileNames[i], diretorioTmp, true);
	}

	// Fecha o arquivo EGRID
	try
	{
		zip->Close();
	}
	catch (const Exception & e)
	{
		msgErro += " Erro ao fechar o arquivo! (" + e.Message + ")";
      return;
	}
}
// -------------------------------------------------------------------------------------------------
void __fastcall TDataEgrid::NomesArquivosES(TStringList *slisNomes)
{
	// variaveis locais
	AnsiString nome;
	AnsiString sub, subini, esfim;
	int pos_ini, pos_fim, length;
	int length_subini, length_esfim;

	subini = "Sub_";
	esfim = ".es";

	for (int i = 0; i < zip->FileCount; i++)
	{
		nome = zip->FileNames[i];
		pos_ini = nome.Pos("Sub_");
		length_subini = subini.Length();
		if (pos_ini <= 0)
		{
			continue;
		}
		pos_fim = nome.Pos(".es");
		if (pos_fim <= 0)
		{
			continue;
		}
		length = nome.Length();
		length_esfim = esfim.Length();
		sub = nome.SubString(pos_ini + length_subini, length - length_subini - length_esfim);
		if (slisNomes->IndexOf(sub) > 0)
		{
			continue;
		}
		slisNomes->Add(sub);
	}
}
//--------------------------------------------------------------------------

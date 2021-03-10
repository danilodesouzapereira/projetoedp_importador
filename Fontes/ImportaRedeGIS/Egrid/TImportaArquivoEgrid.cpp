// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <algorithm>
#include <inifiles.hpp>
#include <DateUtils.hpp>

// DLL Include
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\InfoZcc.h>
#include <PlataformaSinap\DLL_Inc\Progresso.h>

// Plataforma (fontes) Include
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\BD\Rede\BD_Inc.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Geral\VTGeral.h>
#include <PlataformaSinap\Fontes\Geral\VTInfoset.h>
#include <PlataformaSinap\Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>
#include <PlataformaSinap\Fontes\InterfaceNIX\VTInterfaceNIX.h>
#include <PlataformaSinap\Fontes\InfoZcc\VTInfoZccManager.h>
#include <PlataformaSinap\Fontes\LocalDB\SQLite\ESQLite.h>
#include <PlataformaSinap\Fontes\Log\VTLog.h>
#include <PlataformaSinap\Fontes\Progresso\VTAborta.h>
#include <PlataformaSinap\Fontes\Progresso\VTProgresso.h>

// Project Include
#include "..\Exceptions\EImportaEgrid.h"
#include "..\Empresa\VTCondutor.h"
#include "..\Empresa\VTDadosEmpresa.h"
#include "..\Empresa\VTMontaEmpresa.h"
#include "..\Empresa\VTPrimario.h"
#include "..\Empresa\VTRegional.h"
#include "..\Empresa\VTSubestacao.h"
#include "..\PreMonta\ModeloRede.h"
#include "..\PreMonta\VTPreMonta.h"
#include "..\Egrid\EDPDAO.h"

#include "TImportaArquivoEgrid.h"

// ---------------------------------------------------------------------------
using namespace std;
// using namespace sinapsis::sinap::edp;

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
__fastcall TImportaArquivoEgrid::TImportaArquivoEgrid(VTApl *apl, UnicodeString eGridPath) throw
	(runtime_error)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	// define o diretorio temporario
	DefinirDiretorioTemporario();
	// // adiciona um vtinfozccmanager no apl
	// apl->Add(DLL_NewObjInfoZccManager(apl));
	// obtem ponteiros
	aborta = (VTAborta*)apl->GetObject(__classid(VTAborta));
	progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	// guarda o path do arquivo .egrid
	this->eGridPath = eGridPath;
	// cria objeto zip e extrai arquivos
	IniciaZip();
	// inicia o parametro batch
	batch = false;
}

// -----------------------------------------------------------------------------
__fastcall TImportaArquivoEgrid::~TImportaArquivoEgrid(void)
{
	if (slisNomes != NULL)
	{
		delete slisNomes;
		slisNomes = NULL;
	}
	this->Close();
}

// -----------------------------------------------------------------------------
void __fastcall TImportaArquivoEgrid::Close(void)
{
	//
	// apaga os arquivos temporarios
	//
	try
	{
		if (DirectoryExists(diretorioTemporario))
			TDirectory::Delete(diretorioTemporario, true);
	}
	catch (Exception &e)
	{
		// OK, se nao conseguiu apagar sem problema, estah no diretorio de arquivos temporarios
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TImportaArquivoEgrid::CriaDAO(AnsiString nomeArq)
{
	if (dao == NULL)
	{ // verifica se não foi passado o nome do arquivo (caso contrario, sera arquivo de configuração)
		if (nomeArq.IsEmpty())
			dao = new EDPDAO(arquivoConfiguracaoPath);
		else
			dao = new EDPDAO(GetPath(nomeArq), arquivoConfiguracaoPath);
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TImportaArquivoEgrid::DestroiDAO(void)
{
	if (dao)
	{
		dao->Close();
		delete dao;
		dao = NULL;
	}
}

// -----------------------------------------------------------------------------
void __fastcall TImportaArquivoEgrid::DefinirDiretorioTemporario()
{
	//
	// cria o diretorio temporario para extrair os arquivos
	//
	// pega o diretorio temporario do windows(diretorio temporario do usuario)
	AnsiString tempWindowsDir = TPath::GetTempPathW();

	// cria um UID para cada importacao
	TGUID g;
	CreateGUID(g);

	// define o diretorio temporario
	this->diretorioTemporario = (tempWindowsDir + Sysutils::GUIDToString(g));
	try
	{ // cria o diretorio temporario
		TDirectory::CreateDirectory(diretorioTemporario);
	}
	catch (Exception &e)
	{ // se nao foi possivel criar um diretorio temporario no tmp do windows, cria no tmp do sinap
		VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

		// define o diretorio temporario
		this->diretorioTemporario = path->DirTmp() + "\\" + Sysutils::GUIDToString(g);
		// cria o diretorio temporario
		TDirectory::CreateDirectory(diretorioTemporario);
	}
}

/// -------------------------------------------------------------------------------------------------
/* std::list<UnicodeString>__fastcall TImportaArquivoEgrid::GetAlimentadorList
 (UnicodeString siglaSubestacao) {
 // arquivo a ser carregado
 AnsiString arquivo =
 (diretorioTemporario + "\\Sub_" + siglaSubestacao + ".es");
 std::list<UnicodeString>retorno;

 VTSQLiteTable *t = dao->GetAlimentadorList(arquivo);

 for (int row = 0; row < t->NumRows(); row++) {
 t->SetRow(row);
 // monta a representacao do alimentador
 // coloca o alimentador na lista de alimentadores
 retorno.push_back((siglaSubestacao + "-" + t->FieldValue(0)));
 }
 if (t != NULL) {
 delete t;
 }

 return retorno;
 } */

// -------------------------------------------------------------------------------------------------
UnicodeString TImportaArquivoEgrid::GetPath(UnicodeString siglaSubestacao)
{

	return (diretorioTemporario + "\\Sub_" + siglaSubestacao + ".es");

}

// -------------------------------------------------------------------------------------------------
TStringList* __fastcall TImportaArquivoEgrid::GetSubestacaoList()
{

	TStringList *retorno = new TStringList;
	TStringDynArray Files = TDirectory::GetFiles(diretorioTemporario);

	for (int i = 0; i < Files.Length; i++)
	{
		// se for um arquivo .es
		if ((Files[i].Pos("Sub_") > 0) && (Files[i].Pos(".es") > 0))
		{
			UnicodeString siglaSub = Files[i].SubString((Files[i].Length() - 5), 3);
			retorno->Add(siglaSub);
		}
	}
	return retorno;

}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaArquivoEgrid::InsereArranjo(void)
{
	// variáveis locais
	sdi_ARRANJO sdi;
	TList *lisCONDUTOR;
	VTCondutor *condutor;
	int num_arranjos = 0;
	VTDadosEmpresa *empresa = (VTDadosEmpresa*)apl->GetObject(__classid(VTDadosEmpresa));
	pre_monta = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));

	// insere Arranjo para Condutor
	lisCONDUTOR = empresa->LisCondutor();
	num_arranjos = lisCONDUTOR->Count;
	for (int n = 0; n < lisCONDUTOR->Count; n++)
	{
		condutor = (VTCondutor*)lisCONDUTOR->Items[n];
		sdi.id = condutor->id;
		sdi.codigo = condutor->codigo;
		sdi.iadm_a = condutor->iadm_a;
		sdi.r0_ohm_km = condutor->r0_ohm_km;
		sdi.x0_ohm_km = condutor->x0_ohm_km;
		sdi.r1_ohm_km = condutor->r1_ohm_km;
		sdi.x1_ohm_km = condutor->x1_ohm_km;
		sdi.tipo = condutor->tipo;
		sdi.formacao = condutor->formacao;
		sdi.condutor_fase = condutor->condutor_fase;
		sdi.condutor_neutro = condutor->condutor_neutro;
		sdi.r_ohm_km = condutor->r_ohm_km;
		sdi.b0_s_km = condutor->b0_s_km;
		sdi.b1_s_km = condutor->b1_s_km;
		// insere Arranjo em PreMonta
		pre_monta->InsereCfg_Arranjo(sdi);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoEgrid::IniciaVetorRedePriId(int etd_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_ARRANJO sdi;
	VTSQLiteTable *table;

	try
	{
		table = dao->ExecuteSqlSubPri(etd_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em IniciaVetorRedePriId.");
		sucesso = false;
	}
	// em caso de sucesso na query
	if (sucesso)
	{
		try
		{
			vet_redepri_id.Length = table->NumRows();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				vet_redepri_id[row] = table->GetIntField("ID_REDE_MT");
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaEgrid("Coluna inexistente em IniciaVetorRedePriId.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			throw EImportaEgrid("Erro inesperado em IniciaVetorRedePriId.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}

	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoEgrid::IniciaVetorRedeSecId(int pri_id)
{
	// variáveis locais
	AnsiString strlog;
	sdi_SE sdi;
	bool sucesso = true;
	VTSQLiteTable *table;

	try
	{
		table = dao->ExecuteSqlSecRede(pri_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em IniciaVetorRedeSecId.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			vet_redesec_id.Length = table->NumRows();
			for (int row1 = 0; row1 < table->NumRows(); row1++)
			{
				table->SetRow(row1);
				vet_redesec_id[row1] = table->GetIntField("ID_REDE");
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaEgrid("Coluna inexistente em IniciaVetorRedeSecId.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			throw EImportaEgrid("Erro inesperado em IniciaVetorRedeSecId.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}

	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#/tasks/15031223
//https://sinapsisenergia.teamwork.com/#/tasks/18497403
void __fastcall TImportaArquivoEgrid::IniciaZip(void)
{
	AnsiString versao;

	// cria zip
	zip = new TZipFile();
	try
	{ // abre o arquivo
		zip->Open(eGridPath, zmRead);
	}
	catch (const Exception & e)
	{
		AnsiString msg = ("Arquivo inválido ou corrompido! (" + e.Message + ")");
		throw std::runtime_error(msg.c_str());
	}
	try
	{ // verifica se a versão do Egrid é compativel
		if (!VersaoCompativel(versao))
		{
			AnsiString msg = ("Versão inválida da extração: " + versao);
			throw std::runtime_error(msg.c_str());
		}
		if(slisNomes == NULL)
		{
			slisNomes = new TStringList();
		}
		slisNomes->Clear();
		Nomes(slisNomes);
        //le mes da extração
        if (!LeMesExtracao())
		{
			AnsiString msg = ("Não foi possível definir data da extração");
			throw std::runtime_error(msg.c_str());
		}
	}
	catch (const Exception & e)
	{
		AnsiString msg = ("Erro ao abrir o arquivo! (" + e.Message + ")");
		throw std::runtime_error(msg.c_str());
	}
	//
	// define o arquivo .es que serah utilizado para obter as configuracoes da empresa(cabos)
	//
	if (zip->IndexOf(FILENAME_COMMONS) >= 0)
	{
		zip->Extract(FILENAME_COMMONS, diretorioTemporario, true);
		arquivoConfiguracaoPath = (diretorioTemporario + "\\" + FILENAME_COMMONS);
	}
	else
	{
		for (int i = 0; i < zip->FileCount; i++)
		{
			if (zip->FileNames[i].Pos("Sub_") > 0)
			{
				arquivoConfiguracaoPath = (diretorioTemporario + "\\" + zip->FileNames[i]);
				// extrai o arquivo de configuraco
				zip->Extract(zip->FileNames[i], diretorioTemporario, true);
				break;
			}
		}
	}
	//
	// fecha o arquivo
	//
	try
	{
		zip->Close();
	}
	catch (const Exception & e)
	{
		Erro("Erro ao fechar o arquivo! (" + e.Message + ")");
	}
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaArquivoEgrid::ExisteSubZip(AnsiString sigla_sub)
{
	// variaveis locais
	AnsiString nome;

	for (int i = 0; i < slisNomes->Count; i++)
	{
		nome = slisNomes->Strings[i];
		// if (nome.AnsiPos(sigla_sub) > 0)
		if (nome.AnsiCompareIC(sigla_sub) == 0)
		{
			return true;
		}
	}
	return false;
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::LeCfgArranjo(void) throw(EImportaEgrid)
{ // variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_ARRANJO sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
        {
			progresso->Add("Leitura de subestação");
        }
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}
	// obtém ponteiro do objeto MontaEmpresa
	monta_empresa = (VTMontaEmpresa*)apl->GetObject(__classid(VTMontaEmpresa));

	try
	{
		table = dao->ExecuteSqlArranjo();
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em LeCfgArranjo.");
		sucesso = false;
	}
	// em caso de sucesso na query
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_ARRANJO");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.tipo = System::UTF8String(table->GetStringField("TIPO"));
				sdi.formacao = System::UTF8String(table->GetStringField("FORMACAO"));
				sdi.condutor_fase = table->GetStringField("CONDUTOR_FASE");
				sdi.condutor_neutro = table->GetStringField("CONDUTOR_NEUTRO", "");
				sdi.iadm_a = table->GetFloatField("IADM_A");
				sdi.r_ohm_km = table->GetFloatField("R_OHM_KM");
				sdi.r0_ohm_km = table->GetFloatField("R0_OHM_KM");
				sdi.x0_ohm_km = table->GetFloatField("X0_OHM_KM");
				sdi.r1_ohm_km = table->GetFloatField("R1_OHM_KM");
				sdi.x1_ohm_km = table->GetFloatField("X1_OHM_KM");
				sdi.b0_s_km = table->GetFloatField("B0_S_KM");
				sdi.b1_s_km = table->GetFloatField("B1_S_KM");
				// sdi.qcap_kvar_km = table->GetIntField("qcap_kvar_km"); //SUMIU???
				// sdi.custo_novo = table->GetIntField("custo_novo");
				// sdi.custo_rec = table->GetIntField("custo_rec");
				// insere Cabo em MontaEmpresa
				monta_empresa->InsereCondutor(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaEgrid("Coluna inexistente em LeCfgArranjo.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			throw EImportaEgrid("Erro inesperado em LeCfgArranjo.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}

	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::LeCfgLocalidade(void) throw(EImportaEgrid)
{
	// variáveis locais
	AnsiString cmd_sql, strlog;
	strLOCALIDADE str;
	bool sucesso = true;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de localidade");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}
	// obtém ponteiro do objeto MontaEmpresa
	monta_empresa = (VTMontaEmpresa*)apl->GetObject(__classid(VTMontaEmpresa));

	try
	{
		table = dao->ExecuteSqlLocalidade();
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em LeCfgLocalidade.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				str.id = table->GetIntField("ID_LOCALIDADE");
				str.regional_id = table->GetIntField("ID_REGIONAL");
				str.nome = System::UTF8String(table->GetStringField("NOME"));
				// insere Localidade via MontaEmpresa
				monta_empresa->InsereLocalidade(str);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaEgrid("Coluna inexistente em LeCfgLocalidade.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			throw EImportaEgrid("Erro inesperado em LeCfgLocalidade.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return sucesso;
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::LeCfgPrimario(void) throw(EImportaEgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	strPRIMARIO str;
	VTRegional *regional;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de primário");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}
	// obtém ponteiro do objeto MontaEmpresa
	monta_empresa = (VTMontaEmpresa*)apl->GetObject(__classid(VTMontaEmpresa));

	try
	{
		table = dao->ExecuteSqlSubPri();
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em LeCfgPrimario.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{ // lê todos os registros
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				str.id = table->GetIntField("ID_REDE_MT");
				str.subestacao_id = table->GetIntField("ID_SED");
				str.codigo = System::UTF8String(table->GetStringField("NOME"));
				str.nome = System::UTF8String(table->GetStringField("NOME"));
				str.vnom_kv = table->GetFloatField("VNOM_KV", 0);
				str.vop_kv = table->GetFloatField("VOP_KV", 0);
				str.z0cc_pu.r = table->GetFloatField("R0", 0);
				str.z0cc_pu.x = table->GetFloatField("X0", 0);
				str.z1cc_pu.r = table->GetFloatField("R1", 0);
				str.z1cc_pu.x = table->GetFloatField("X1", 0);
				str.id_trafo = table->GetIntField("ID_INSTALACAO");
				// insere Primario via MontaEmpresa
				monta_empresa->InserePrimario(str);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaEgrid("Coluna inexistente em LeCfgPrimario.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			throw EImportaEgrid("Erro inesperado em LeCfgPrimario.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::LeCfgRegional(void) throw(EImportaEgrid)
{
	// variáveis locais
	AnsiString cmd_sql, strlog;
	strREGIONAL str;
	bool sucesso = true;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de regional");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}
	// obtém ponteiro do objeto MontaEmpresa
	monta_empresa = (VTMontaEmpresa*)apl->GetObject(__classid(VTMontaEmpresa));

	try
	{
		table = dao->ExecuteSqlRegional();
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em LeCfgRegional.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				str.id = table->GetIntField("ID_REGIONAL");
				// str.empresa_id = table->GetIntField("EMPRESA_ID");
				// str.empresa_id = empresa_id;
				str.nome = System::UTF8String(table->GetStringField("NOME"));
				// insere Regional via MontaEmpresa
				monta_empresa->InsereRegional(str);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaEgrid("Coluna inexistente em LeCfgRegional.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			throw EImportaEgrid("Erro inesperado em LeCfgRegional.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::LeCfgSubestacao(void) throw(EImportaEgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	strSUBESTACAO str;
	VTRegional *regional;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de subestação");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}
	// obtém ponteiro do objeto MontaEmpresa
	monta_empresa = (VTMontaEmpresa*)apl->GetObject(__classid(VTMontaEmpresa));

	try
	{
		table = dao->ExecuteSqlSub();
	}
	catch (EImportaEgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				str.id = table->GetIntField("ID_SED");
				str.localidade_id = table->GetIntField("ID_LOCALIDADE");
				str.codigo = System::UTF8String(table->GetStringField("CODIGO"));
				if (!ExisteSubZip(str.codigo))
					continue;
				str.nome = System::UTF8String(table->GetStringField("NOME"));
				str.sub_mae_id = table->GetIntField("ID_SED_MAE");
				str.rede_mt_pai_id = table->GetIntField("ID_REDE_MT_PAI");
				// insere Subestacao via MontaEmpresa
				monta_empresa->InsereSubestacao(str);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaEgrid("Coluna inexistente em LeCfgSubestacao.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			throw EImportaEgrid("Erro inesperado em LeCfgSubestacao.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}
// -------------------------------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18497403
bool __fastcall TImportaArquivoEgrid::LeMesExtracao(void)
{
    // variaveis locais
    AnsiString nome, arq_ini, key, section;
    TIniFile *ini = NULL;
    TDateTime datetime = Now();

    try
    {// inicia dados
        arq_ini = "meta-inf.ini";
        section = "META-INF";
        key = "DATA";
        //default
        mes = int(MonthOf(datetime));
        for (int i = 0; i < zip->FileCount; i++)
        {
            nome = zip->FileNames[i];
            if (nome.AnsiCompareIC(arq_ini) != 0)
                continue;
            // lê arquivo
            zip->Extract(nome, diretorioTemporario, true);
            ini = new TIniFile(diretorioTemporario + "\\" + nome);
            datetime = ini->ReadDate(section, key, datetime);
            mes = int(MonthOf(datetime));
            break;
        }
        if (ini != NULL)
            delete ini;
    }
    catch(...)
    {
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
//https://sinapsisenergia.teamwork.com/#/tasks/18497403
bool __fastcall TImportaArquivoEgrid::LeRedes(TList *lisSUBESTACAO, strOPImporta *opcao)
{
	// variáveis locais

	bool sucesso = true;
	VTSubestacao *subestacao;

	try
	{ // salva opções de importação
		this->opcao = opcao;
		// atualiza progresso
		if (!BatchMode)
		{
			if (progresso == NULL)
				progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
			if (progresso)
				progresso->Add("Iniciando importação");
            if((aborta) && (aborta->Requested))
            {
                return false;
            }
		}
		// obtém ponteiro para objeto PreMonta
		pre_monta = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));
		// inicia a importação
		pre_monta->TrataInicioDeDados();
        pre_monta->MesExtracao = mes;
		// faz a leitura dos Cabos
		// sucesso = ReadCfg_TabCabo();   //TESTE arrumar pra importar
		// faz a leitura dos Suportes, que a EDP chama de arranjo
		// if (sucesso) sucesso = ReadCfg_TabArranjo();
		// faz a leitura das Subestações e de todas suas redes primárias e secundárias
		for (int n = 0; (sucesso) & (n < lisSUBESTACAO->Count); n++)
		{
			nomeSe = IntToStr(n + 1) + " de " + IntToStr(lisSUBESTACAO->Count);
			subestacao = (VTSubestacao*)lisSUBESTACAO->Items[n];
			sucesso = LeUmaRedeEtd_TodasRedesPriSec(subestacao->Id);
            if((aborta) && (aborta->Requested))
            {
                sucesso = false;
                break;
            }
		}
		// atualiza progresso
		if (!BatchMode)
		{
			if (progresso == NULL)
				progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
			if (progresso)
				progresso->Add("Montagem das redes");
            if((aborta) && (aborta->Requested))
            {// finaliza a importação
                pre_monta->TrataFimDeDados();
                return false;
            }
		}
		// finaliza a importação
		pre_monta->TrataFimDeDados();
	}
	catch (Exception &e)
	{
		Erro(e.Message);
		sucesso = false;
	}
	return (sucesso);

}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
//https://sinapsisenergia.teamwork.com/#/tasks/18497403
bool __fastcall TImportaArquivoEgrid::LeRedes(VTSubestacao *subestacao, strOPImporta *opcao)
{
	// variáveis locais
	bool sucesso = true;

	try
	{ // salva opções de importação
		this->opcao = opcao;
		// atualiza progresso
		if (!BatchMode)
		{
			if (progresso == NULL)
				progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
			if (progresso)
			{
				progresso->Add("Iniciando importação");
			}
            if((aborta) && (aborta->Requested))
            {
                return false;
            }
		}
		// obtém ponteiro para objeto PreMonta
		pre_monta = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));
		// inicia a importação
		pre_monta->TrataInicioDeDados();
        pre_monta->MesExtracao = mes;
		// lê impedancias de curto circuito e salva em arquivo
		// ReadCfg_TabImpedanciaCurto(subestacao->Id); // DANIV: Implementar
		// sucesso = ReadCfg_TabArranjo();
		// faz a leitura da Subestacao e de todas suas redes primárias e secundárias
		if (sucesso)
		{
			nomeSe = subestacao->Codigo;
			sucesso = LeUmaRedeEtd_TodasRedesPriSec(subestacao->Id);
		}
		// atualiza progresso
		if (!BatchMode)
		{
			if (progresso == NULL)
				progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
			if (progresso)
				progresso->Add("Montagem das redes");
            if((aborta) && (aborta->Requested))
            {// finaliza a importação
                pre_monta->TrataFimDeDados();
                return false;
            }
		}
		// finaliza a importação
		pre_monta->TrataFimDeDados();
	}
	catch (Exception &e)
	{
		// log_geral->Add("LeRedes()");
		// log_geral->Add(e.Message);
		Erro(e.Message);
		sucesso = false;
		// Erro("");
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#/tasks/15031223
bool __fastcall TImportaArquivoEgrid::LeUmaRedeEtd_TodasRedesPriSec(int etd_id) throw(EImportaEgrid)
{
	int count_rede_ok = 0;
	try
	{ // lê dados da rede
		if (!ReadEtd_TabSe(etd_id))
			return (false);
		// lê dados dos Barramentos
		if (!ReadEtd_TabBarramento(etd_id))
			return (false);
		// lê dados dos transformadores
		if (!ReadEtd_TabTrafoSe(etd_id))
			return (false);
		if (!ReadCfg_TabImpedanciaCurto(etd_id))
			return (false);
		// lê ID de todas redes primárias da ETD
		if (!IniciaVetorRedePriId(etd_id))
		{
			return (false);
		}
		for (int n = 0; n < vet_redepri_id.Length; n++)
		{ // lê a rede primária e todas suas redes secundárias
			nomeCircuito = IntToStr(n + 1) + " de " + IntToStr(vet_redepri_id.Length);
			if (!PrimarioSelecionado(vet_redepri_id[n]))
				continue;
			if (!LeUmaRedePri_TodasRedesSec(vet_redepri_id[n]))
				break; //continue; // return(false);
			count_rede_ok++;
		}
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
		// lê socorro
		if(count_rede_ok > 0) ReadEtd_TabSocorro();
		else return false;
	}
	catch (Exception &e)
	{
		// log_geral->Add("LeUmaRedeEtd_TodasRedesPriSec()");
		// log_geral->Add(e.Message);
		throw EImportaEgrid(e.Message); ;
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::LeUmaRedePri_TodasRedesSec(int pri_id) throw(EImportaEgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString prog;

	try
	{ // lê dados da rede
		if (!ReadPri_TabRede(pri_id))
			return (false);
		if (!ReadPri_TabBarra(pri_id))
			return (false);
		if (!ReadPri_TabTrecho(pri_id))
			return (false);
		if (!ReadPri_TabLink(pri_id))
			return (false);
		if (!ReadPri_TabChave(pri_id))
			return (false);
		if (!ReadPri_TabRegulador(pri_id))
			return (false);
		if (!ReadPri_TabSocorro(pri_id))
			return (false);
		if (!ReadPri_TabCapacitorParalelo(pri_id))
			return (false);
		if (!ReadPri_TabGerador(pri_id))
			return (false);
		if (!ReadPri_TabConsMT(pri_id))
			return (false);
		// lê ID de todas redes secundárias da rede primária
		if (!IniciaVetorRedeSecId(pri_id))
			return (false);

        sucesso = true;
		for (int n = 0; (sucesso) && (n < vet_redesec_id.Length); n++)
		{
			nomeSec = IntToStr(n + 1) + " de " + IntToStr(vet_redesec_id.Length);
			// atualiza progresso
			if (!BatchMode)
			{
				if (progresso)
				{
					prog = "SED: " + nomeSe + " SDMT: " + nomeCircuito + " SDBT: " + nomeSec;
					progresso->Add(prog, true);
				}
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
			// verifica como dever ser lida a rede secundária
			switch (opcao->sec.topologia)
			{
			case opSEC_GIS:
				sucesso = LeUmaRedeSec_Completa(vet_redesec_id[n]);
				break;
			case opSEC_UTC:
				sucesso = LeUmaRedeSec_TrafoCarga(vet_redesec_id[n]);
				break;
			case opSEC_CARGA:
				sucesso = LeUmaRedeSec_SomenteCarga(vet_redesec_id[n]);
				break;
			}
		}
	}
	catch (Exception &e)
	{
		// log_geral->Add("LeUmaRedePri_TodasRedesSec()");
		// log_geral->Add(e.Message);
		throw EImportaEgrid(e.Message);
		sucesso = false;
		// Erro("");
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoEgrid::LeUmaRedeSec_Completa(int sec_id)
{

	try
	{
		// lê dados da rede
		if (!ReadSec_TabTrafoMTBT(sec_id))
			return (false);
		if (!ReadSec_TabUtcBT(sec_id))
			return (false);
		if (!ReadSec_TabBarraBT(sec_id))
			return (false);
		if (!ReadSec_TabTrechoBT(sec_id))
			return (false);
		if (!ReadSec_TabLinkBT(sec_id))
			return (false);
		if (!ReadSec_TabPCargaBT(sec_id))
			return (false);
		if (!ReadSec_TabConsIP(sec_id))
			return (false);
		if (!ReadSec_TabConsBT(sec_id))
			return (false);
		if (!ReadSec_TabGeradorBT(sec_id))
			return (false);
	}
	catch (Exception &e)
	{
		// log_geral->Add("LeUmaRedeEtd_TodasRedesPriSec()");
		// log_geral->Add(e.Message);
		throw EImportaEgrid(e.Message); ;
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoEgrid::LeUmaRedeSec_SomenteCarga(int sec_id)
{
	try
	{
		// lê dados da rede
		if (!ReadSec_TabTrafoMTBT(sec_id))
			return (false);
		if (!ReadSec_TabConsIP(sec_id))
			return (false);
		if (!ReadSec_TabConsBT(sec_id))
			return (false);
		if (!ReadSec_TabGeradorBT(sec_id))
			return (false);
	}
	catch (Exception &e)
	{
		throw EImportaEgrid(e.Message); ;
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoEgrid::LeUmaRedeSec_TrafoCarga(int sec_id)
{
	try
	{
		// lê dados da rede
		if (!ReadSec_TabTrafoMTBT(sec_id))
			return (false);
		if (!ReadSec_TabUtcBT(sec_id))
			return (false);
		if (!ReadSec_TabBarraBT(sec_id)) // somente para inserir trafos
				return (false);
		if (!ReadSec_TabConsIP(sec_id))
			return (false);
		if (!ReadSec_TabConsBT(sec_id))
			return (false);
		if (!ReadSec_TabGeradorBT(sec_id))
			return (false);
	}
	catch (Exception &e)
	{
		// log_geral->Add("LeUmaRedeEtd_TodasRedesPriSec()");
		// log_geral->Add(e.Message);
		throw EImportaEgrid(e.Message); ;
		return (false);
	}
	return (true);
}

// -------------------------------------------------------------------------------------------------
void __fastcall TImportaArquivoEgrid::Nomes(TStringList *slisNomes)
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

// ---------------------------------------------------------------------------------------------
bool __fastcall TImportaArquivoEgrid::PrimarioSelecionado(int id_pri)
{
	// variáveis locais

	VTDadosEmpresa *empresa = (VTDadosEmpresa*)apl->GetObject(__classid(VTDadosEmpresa));
	VTPrimario *primario;
	TList *lisPRIMARIO = empresa->LisPrimario();

	for (int n = 0; n < lisPRIMARIO->Count; n++)
	{
		primario = (VTPrimario*)lisPRIMARIO->Items[n];
		if (primario->Id == id_pri)
		{
			return (primario->Selecionado);
		}
	}
	return (false);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadCfg_TabImpedanciaCurto(int etd_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	// strInfoZcc *sdi;
	sdi_SUPRIMENTO sdi;
	// variáveis locais
	VTSQLiteTable *table;

	// // proteção
	// if (!infoZccManager) {
	// return (false);
	// }
	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso)
			progresso->Add("Leitura de impedâncias de curto circuito");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}
	//
	try
	{
		table = dao->ExecuteSqlSubImpedancias(etd_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadCfg_TabImpedanciaCurto.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_INSTALACAO");
				sdi.id_barra_at = table->GetIntField("ID_BARRA_AT");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.r1_pu = table->GetFloatField("RCC_AT") / 100.;
				sdi.x1_pu = table->GetFloatField("XCC_AT") / 100.;
				// sdi.r0_pu = table->GetFloatField("R0_PU")/100.;
				// sdi.x0_pu = table->GetFloatField("X0_PU")/100.;
				sdi.vnom_kv = table->GetFloatField("VALTA_KV");
				// insere Suprimento em PreMonta
				pre_monta->CompletaSuprimentoCircuito(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Coluna inexistente em ReadCfg_TabImpedanciaCurto.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Erro inesperado em ReadCfg_TabImpedanciaCurto.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}

	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadEtd_TabBarramento(int etd_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_BARRAMENTO sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de barramento SED");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlSubBarra(etd_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadEtd_TabBarramento.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_INSTALACAO");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.vnom_kv = table->GetFloatField("VNOM_KV");
				sdi.r_aterr_ohms = table->GetFloatField("R_ATERRAMENTO_OHM");
				sdi.x_aterr_ohms = table->GetFloatField("X_ATERRAMENTO_OHM");
				sdi.x_utm_m = table->GetFloatField("UTMX_M");
				sdi.y_utm_m = table->GetFloatField("UTMY_M");
				sdi.id_instalacao = table->GetIntField("ID_INSTALACAO");
				// sdi.trafose_id = table->GetIntField("ID_TRAFO_SED");
				// insere Barra em PreMonta
				pre_monta->InsereEtd_Barra(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Coluna inexistente em ReadEtd_TabSe.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Erro inesperado em ReadEtd_TabSe.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}

	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadEtd_TabSocorro()
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_SOCORRO sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura das Chaves de Socorro");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	// verifica se foram lidos os primários

	if (vet_redepri_id.Length == 0)
		return (true);
	cmd_sql = "(";
	for (int n = 0; n < vet_redepri_id.Length; n++)
	{
		cmd_sql += IntToStr(vet_redepri_id[n]);
		if (n < (vet_redepri_id.Length - 1))
			cmd_sql += ", ";
		else
			cmd_sql += ") ";
	}
	try
	{
		table = dao->ExecuteSqlSubSocorro(cmd_sql);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadEtd_TabSocorro.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_CHAVE_SOCORRO");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.ativo = table->GetIntField("ATIVO");
				sdi.telecomando = table->GetIntField("TELECOMANDO");
				sdi.inom_a = table->GetFloatField("INOM_A");
				sdi.se1_id = table->GetIntField("ID_SED1");
				sdi.se2_id = table->GetIntField("ID_SED2");
				sdi.circ1_id = table->GetIntField("ID_REDE_MT1");
				sdi.circ2_id = table->GetIntField("ID_REDE_MT2");
				sdi.barra1_id = table->GetIntField("ID_BARRA1");
				sdi.barra2_id = table->GetIntField("ID_BARRA2");
				// insere Chave em PreMonta
				pre_monta->InsereEtd_Socorro(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Coluna inexistente em ReadEtd_TabSocorro.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Erro inesperado em ReadEtd_TabSocorro.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}

	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadEtd_TabSe(int etd_id)
{
	// variáveis locais
	AnsiString cmd_sql, strlog;
	sdi_SE sdi;
	bool sucesso = true;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de rede SED", true);
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}
	try
	{
		table = dao->ExecuteSqlSub(etd_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadEtd_TabSe.");
		sucesso = false;
	}
	// em caso de sucesso na query
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_SED");
				sdi.codigo = System::UTF8String(table->GetStringField("CODIGO"));
				sdi.nome = System::UTF8String(table->GetStringField("NOME"));
				sdi.semae_id = table->GetIntField("ID_SED_MAE");
				sdi.circpai_id = table->GetIntField("ID_REDE_MT_PAI");
				sdi.x_utm_m = table->GetFloatField("UTMX_M");
				sdi.y_utm_m = table->GetFloatField("UTMY_M");
				// insere Etd em PreMonta
				pre_monta->InsereEtd_Rede(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Coluna inexistente em ReadEtd_TabSe.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Erro inesperado em ReadEtd_TabSe.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadEtd_TabTrafoSe(int etd_id)
{
	// variáveis locais
	AnsiString cmd_sql, strlog;
	sdi_TRAFO_SE sdi;
	bool sucesso = true;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de transformador SED");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlSubTrafo(etd_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadEtd_TabTrafoSe.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_INSTALACAO");
				sdi.extern_id = table->GetIntField("ID_TRAFO_SED");
				sdi.se_id = table->GetIntField("ID_SED");
				sdi.barra_mt1 = table->GetIntField("ID_BARRA1");
				sdi.barra_mt2 = table->GetIntField("ID_BARRA2");
				sdi.barra_at = table->GetIntField("ID_BARRA_AT");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.ativo = table->GetIntField("ATIVO");
				// sdi.tipo = table->GetIntField("tipo");
				sdi.tipo_lig = table->GetIntField("TP_LIGACAO");
				sdi.snom_mva = table->GetFloatField("SNOM_MVA");
				sdi.snom_vent_mva = table->GetFloatField("SNOM_VENT_MVA");
				sdi.valta_kv = table->GetFloatField("VALTA_KV");
				sdi.vbaixa_kv = table->GetFloatField("VBAIXA_KV");
				sdi.vbaixa_op_kv = table->GetFloatField("VBAIXA_OP_KV");
				sdi.vbaixa2_kv = table->GetFloatField("VBAIXA2_KV");
				sdi.vbaixa2_op_kv = table->GetFloatField("VBAIXA2_OP_KV");
				sdi.r0_pu = table->GetFloatField("R0_PU") / 100.;
				sdi.x0_pu = table->GetFloatField("X0_PU") / 100.;
				sdi.r1_pu = table->GetFloatField("R1_PU") / 100.;
				sdi.x1_pu = table->GetFloatField("X1_PU") / 100.;
				sdi.z12 = table->GetFloatField("Z12");
				sdi.z23 = table->GetFloatField("Z23");
				// SERA ALTERADO NO EGRID PARA VIR
				sdi.z13 = table->GetFloatField("Z13");
				// SERA ALTERADO NO EGRID PARA VIR
				sdi.xr12 = table->GetFloatField("XR12");
				// SERA ALTERADO NO EGRID PARA VIR
				sdi.xr23 = table->GetFloatField("XR23");
				// SERA ALTERADO NO EGRID PARA VIR
				sdi.xr13 = table->GetFloatField("XR13");
				// SERA ALTERADO NO EGRID PARA VIR
				sdi.z012 = table->GetFloatField("Z012");
				// SERA ALTERADO NO EGRID PARA VIR
				sdi.z023 = table->GetFloatField("Z023");
				// SERA ALTERADO NO EGRID PARA VIR
				sdi.z013 = table->GetFloatField("Z013");
				// SERA ALTERADO NO EGRID PARA VIR
				sdi.xr012 = table->GetFloatField("XR012");
				// SERA ALTERADO NO EGRID PARA VIR
				sdi.xr023 = table->GetFloatField("XR023");
				// SERA ALTERADO NO EGRID PARA VIR
				sdi.xr013 = table->GetFloatField("XR013");
				// SERA ALTERADO NO EGRID PARA VIR
				sdi.perda_fe_kw = table->GetFloatField("PERDA_FERRO_KW");
				sdi.perda_cu_kw = table->GetFloatField("PERDA_COBRE_KW");

				// insere Trafo em PreMonta
				pre_monta->InsereEtd_Trafo(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Coluna inexistente em ReadEtd_TabTrafoSe.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Erro inesperado em ReadEtd_TabTrafoSe.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadPri_TabBarra(int pri_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_BARRA sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de barra SDMT");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}
	try
	{
		table = dao->ExecuteSqlPriBarra(pri_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadPri_TabBarra.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_BARRA");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.x_utm_m = table->GetFloatField("UTMX_M");
				sdi.y_utm_m = table->GetFloatField("UTMY_M");
				// insere Barra em PreMonta
				pre_monta->InserePri_Barra(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Coluna inexistente em ReadPri_TabBarra.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Erro inesperado em ReadPri_TabBarra.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadPri_TabCapacitorParalelo(int pri_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_CAPACITOR sdi;
	VTSQLiteTable *table;
	//
	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de barra SDMT");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}
	try
	{
		table = dao->ExecuteSqlPriCapacitorParalelo(pri_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadPri_TabCapacitorParalelo.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_CAPACITOR");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.vnom_kv = table->GetFloatField("VNOM_KV");
				sdi.qnon_kvar = table->GetFloatField("QNOM_KVAR");
				sdi.barra_id = table->GetIntField("ID_BARRA");
				sdi.circ_id = table->GetIntField("ID_REDE");
				// insere Capacitor em PreMonta
				pre_monta->InserePri_Capacitor(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Coluna inexistente em ReadPri_TabCapacitorParalelo.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Erro inesperado em ReadPri_TabCapacitorParalelo.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}

	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadPri_TabChave(int pri_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_CHAVE sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de chave SDMT");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlPriChave(pri_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadPri_TabChave.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_CHAVE");
				sdi.id_rede_mt = table->GetIntField("ID_REDE_MT");
				sdi.id_barra1 = table->GetIntField("ID_BARRA1");
				sdi.id_barra2 = table->GetIntField("ID_BARRA2");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.ativo = table->GetIntField("ATIVO");
				sdi.telecomando = table->GetIntField("TELECOMANDO");
				sdi.estado = table->GetIntField("ESTADO");
				sdi.estado_normal = table->GetIntField("ESTADO_NORMAL");
				sdi.inom_a = table->GetFloatField("INOM_A");
				sdi.vnom_kv = table->GetFloatField("VNOM_KV");
				sdi.tipo_elo = table->GetStringField("TP_ELO");
				sdi.elo_a = table->GetFloatField("I_ELO_A");
				// insere Chave em PreMonta
				pre_monta->InserePri_Chave(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Coluna inexistente em ReadPri_TabChave.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Erro inesperado em ReadPri_TabChave.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadPri_TabRede(int pri_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_CIRCUITO sdi;
	AnsiString prog;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
		{
			prog = "SED: " + nomeSe + " SDMT: " + nomeCircuito;
			progresso->Add(prog, true);
			progresso->Add("Leitura de rede SDMT");
		}
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}
	try
	{
		table = dao->ExecuteSqlPriRede(pri_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadPri_TabRede.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_REDE_MT");
				sdi.codigo = table->GetStringField("NOME");
				sdi.nome = table->GetStringField("NOME");
				sdi.vnom_kv = table->GetFloatField("VNOM_KV");
				sdi.vop_kv = table->GetFloatField("VOP_KV");
				sdi.trafo_se_id = table->GetIntField("ID_INSTALACAO");
				sdi.barra_mt_id = table->GetIntField("ID_BARRA_MT");
				sdi.z0_pu.r = table->GetFloatField("R0", 0);
				sdi.z0_pu.x = table->GetFloatField("X0", 0);
				sdi.z1_pu.r = table->GetFloatField("R1", 0);
				sdi.z1_pu.x = table->GetFloatField("X1", 0);
				sdi.pcc3f = table->GetFloatField("PCC_3F", 0);
				sdi.pcc1f = table->GetFloatField("PCC_1F", 0);
				sdi.acc3f = table->GetFloatField("ACC_3F", 0);
				sdi.acc1f = table->GetFloatField("ACC_1F", 0);
				// insere Circuito em PreMonta
				sucesso = pre_monta->InserePri_Rede(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaEgrid("Coluna inexistente em ReadPri_TabRede.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			throw EImportaEgrid("Erro inesperado em ReadPri_TabRede.");
			sucesso = false;
		}
	}

	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadPri_TabConsMT(int pri_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_CONS_MT sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de consumidor SDMT");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlPriConsMT(pri_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadPri_TabConsMT.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_CARGA_MT");
				sdi.barra_id = table->GetIntField("ID_BARRA");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.ativo = table->GetIntField("ATIVO");
				sdi.tipo_contrato = table->GetIntField("TP_CONTRATO");
				sdi.tipo_fat = table->GetIntField("TP_FAT");
				sdi.tipo_medicao = table->GetIntField("TP_MEDICAO");
				sdi.classe_cons = table->GetIntField("CLASSE");
				sdi.nome = table->GetStringField("NOME");
				sdi.cod_ativ = table->GetStringField("CODIGO_ATIVIDADE");
				sdi.snom_kva = table->GetFloatField("SNOM_KVA");
				sdi.consumo_kwh_mes = table->GetFloatField("CONSUMO_MENSAL_KWH");
				sdi.p_ponta_kw = table->GetFloatField("DEMANDA_ATIVA_PONTA_KW");
				sdi.q_ponta_kvar = table->GetFloatField("DEMANDA_REATIVA_PONTA_KVAR");
				sdi.p_fponta_kw = table->GetFloatField("DEMANDA_ATIVA_FORA_PONTA_KW");
				sdi.q_fponta_kvar = table->GetFloatField("DEMANDA_REATIVA_FORA_PONTA_KVAR");
				sdi.fases = table->GetStringField("FASES");
				// insere Consumidor MT em PreMonta
				pre_monta->InserePri_Consumidor(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Coluna inexistente em ReadPri_TabConsMT.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Erro inesperado em ReadPri_TabConsMT.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
//https://sinapsisenergia.teamwork.com/#/tasks/18497403
bool __fastcall TImportaArquivoEgrid::ReadPri_TabGerador(int pri_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_GERADOR sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Gerador SDMT");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlPriGerador(pri_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadPri_TabGerador.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_GERADOR");
				sdi.circ_id = table->GetIntField("ID_REDE");
				sdi.barra_id1 = table->GetIntField("ID_BARRA1");
				sdi.barra_id2 = table->GetIntField("ID_BARRA2");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.ativo = table->GetIntField("ATIVO");
				sdi.snom_kva = table->GetFloatField("SNOM_KVA");
				sdi.vnom_kv = table->GetFloatField("VNOM_KV");
				sdi.r0_pu = table->GetFloatField("R0_PU");
				sdi.x0_pu = table->GetFloatField("X0_PU");
				sdi.r1_pu = table->GetFloatField("R1_PU");
				sdi.x1_pu = table->GetFloatField("X1_PU");
				sdi.cdc = table->GetStringField("CDC");
				sdi.tipo_geracao = table->GetStringField("TIPO_GERACAO");
				// insere Gerador em PreMonta
				pre_monta->InserePri_Gerador(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Coluna inexistente em ReadPri_TabGerador.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Erro inesperado em ReadPri_TabGerador.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// --------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadPri_TabLink(int pri_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_LINK sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de link SDMT");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlPriLink(pri_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadPri_TabLink.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.link_id = table->GetIntField("ID_LINK");
				sdi.trecho_id = table->GetIntField("ID_TRECHO");
				sdi.barra_id1 = table->GetIntField("ID_BARRA1");
				sdi.barra_id2 = table->GetIntField("ID_BARRA2");
				// insere Link em PreMonta
				pre_monta->InserePri_Link(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaEgrid("Coluna inexistente em ReadPri_TabLink.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			throw EImportaEgrid("Erro inesperado em ReadPri_TabLink.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadPri_TabRegulador(int pri_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_REGULADOR sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura dos reguladores SDMT");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlPriRegulador(pri_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadPri_TabRegulador.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_REGULADOR_TENSAO");
				sdi.id_rede_mt = table->GetIntField("ID_REDE_MT");
				sdi.id_barra1 = table->GetIntField("ID_BARRA1");
				sdi.id_barra2 = table->GetIntField("ID_BARRA2");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.ativo = table->GetIntField("ATIVO");
				sdi.tipo_lig = table->GetIntField("TP_LIGACAO");
				sdi.bypass = table->GetIntField("BYPASS");
				sdi.inversao = table->GetIntField("INVERSAO");
				sdi.tap_pu = table->GetFloatField("TAP_PU");
				sdi.faixa_pu = table->GetFloatField("FAIXA_PU");
				sdi.n_passos = table->GetIntField("NUMERO_PASSOS");
				sdi.snom_kva = table->GetFloatField("SNOM_KVA");
				sdi.vnom_kv = table->GetFloatField("VNOM_KV");
				sdi.inom_a = table->GetFloatField("INOM_A");
				// insere Regulador em PreMonta
				pre_monta->InserePri_Regulador(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Coluna inexistente em ReadPri_TabRegulador.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Erro inesperado em ReadPri_TabRegulador.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadPri_TabSocorro(int pri_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_SOCORRO sdi;
	VTSQLiteTable *table;
	//
	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura das Chaves de Socorro");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlPriSocorro(pri_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadPri_TabSocorro.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{

			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_CHAVE_SOCORRO");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.ativo = table->GetIntField("ATIVO");
				sdi.telecomando = table->GetIntField("TELECOMANDO");
				sdi.inom_a = table->GetFloatField("INOM_A");
				sdi.se1_id = table->GetIntField("ID_SED1");
				sdi.se2_id = table->GetIntField("ID_SED2");
				sdi.circ1_id = table->GetIntField("ID_REDE_MT1");
				sdi.circ2_id = table->GetIntField("ID_REDE_MT2");
				sdi.barra1_id = table->GetIntField("ID_BARRA1");
				sdi.barra2_id = table->GetIntField("ID_BARRA2");
				// insere Chave em PreMonta
				pre_monta->InserePri_Socorro(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Coluna inexistente em ReadPri_TabSocorro.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Erro inesperado em ReadPri_TabSocorro.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}

	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadPri_TabTrecho(int pri_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_TRECHO sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de trecho SDMT");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlPriTrecho(pri_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadPri_TabTrecho.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_TRECHO");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.ativo = table->GetIntField("ATIVO");
				sdi.tipo_local = table->GetIntField("TP_LOCAL");
				sdi.propriedade = table->GetIntField("PROPRIEDADE");
				sdi.mult = table->GetIntField("MULT");
				sdi.fases = table->GetStringField("FASES");
				sdi.comp_m = table->GetFloatField("COMPRIMENTO_M");
				sdi.arranjo_id = table->GetIntField("ID_ARRANJO");
				sdi.cabo_id = table->GetIntField("ID_CABO_FASE");
				sdi.barra1_id = table->GetIntField("ID_BARRA1");
				sdi.barra2_id = table->GetIntField("ID_BARRA2");
				sdi.circ_id = table->GetIntField("ID_REDE");
				// insere Trecho em PreMonta
				sucesso = pre_monta->InserePri_Trecho(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}

		}

		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaEgrid("Coluna inexistente em ReadPri_TabTrecho.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			throw EImportaEgrid("Erro inesperado em ReadPri_TabTrecho.");
			sucesso = false;
		}
	}

	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadSec_TabBarraBT(int sec_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_BARRA_BT sdi;
	int count_barra = 0;
	VTSQLiteTable *table;
	int numero = 0;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de barra SDBT");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlSecBarra(sec_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadSec_TabTrafoMTBT.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			numero = table->NumRows();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_BARRA");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.x_utm_m = table->GetFloatField("UTMX_M");
				sdi.y_utm_m = table->GetFloatField("UTMY_M");
				sdi.redebt_id = table->GetIntField("ID_REDE_BT");
				// insere BarraBT em PreMonta
				pre_monta->InsereSec_Barra(sdi);
				count_barra++;
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}

			// insere BarraBT em PreMonta (somente para inserir os trafos)
			if (count_barra == 0)
			{
				sdi.id = -1;
				pre_monta->InsereSec_Barra(sdi);
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Coluna inexistente em ReadSec_TabBarraBT.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add("ReadEtd_TabSe");
			// log_geral->Add(e.Message);
			throw EImportaEgrid("Erro inesperado em ReadSec_TabBarraBT.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadSec_TabConsBT(int sec_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_CONS_BT sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de consumidor SDBT");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlSecConsBT(sec_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadSec_TabConsBT.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_CARGA_BT");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.ativo = table->GetIntField("ATIVO");
				sdi.classe_cons = table->GetIntField("CLASSE");
				sdi.nome = table->GetStringField("NOME");
				sdi.cod_ativ = table->GetStringField("CODIGO_ATIVIDADE");
				sdi.consumo_kwh_mes = table->GetFloatField("CONSUMO_MENSAL_KWH");
				sdi.q_dias = table->GetIntField("QT_DIAS");
				sdi.fases = table->GetStringField("FASES");
				sdi.pcarga_id = table->GetIntField("ID_PCARGA_BT");
				sdi.redebt_id = table->GetIntField("ID_REDE_BT");
				// insere ConsBT em PreMonta
				pre_monta->InsereSec_Consumidor(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add(" ReadEtd_TabSe ");
			// log_geral->Add(e.Message);
			throw EImportaEgrid(" Coluna inexistente em ReadSec_TabConsBT.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add(" ReadEtd_TabSe ");
			// log_geral->Add(e.Message);
			throw EImportaEgrid(" Erro inesperado em ReadSec_TabConsBT.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadSec_TabConsIP(int sec_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_CONS_IP sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de iluminação pública SDBT ");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlSecConsIP(sec_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid(" Erro ao executar a query em ReadSec_TabConsIP.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_CARGA_IP");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.n_lamp = table->GetIntField("NUMERO_LAMPADAS");
				sdi.lamp_dem_w = table->GetFloatField("LAMPADA_DEM_W");
				sdi.lamp_dem_var = table->GetFloatField("LAMPADA_DEM_VAR");
				sdi.fases = table->GetStringField("FASES");
				sdi.barra_id = table->GetIntField("ID_BARRA");
				// insere ConsIP em PreMonta
				pre_monta->InsereSec_ConsIP(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add(" ReadEtd_TabSe ");
			// log_geral->Add(e.Message);
			throw EImportaEgrid(" Coluna inexistente em ReadSec_TabConsIP.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add(" ReadEtd_TabSe ");
			// log_geral->Add(e.Message);
			throw EImportaEgrid(" Erro inesperado em ReadSec_TabConsIP.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadSec_TabGeradorBT(int sec_id)
{
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_GERADOR_BT sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de gerador SDBT");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlSecGeradorBT(sec_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid("Erro ao executar a query em ReadSec_TabGeradorBT.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_GERADOR");
				sdi.circ_id = table->GetIntField("ID_REDE");
				sdi.barra_id = table->GetIntField("ID_BARRA1");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.ativo = table->GetIntField("ATIVO");
				sdi.snom_kva = table->GetFloatField("SNOM_KVA");
				sdi.vnom_kv = table->GetFloatField("VNOM_KV");
				sdi.r0_pu = table->GetFloatField("R0_PU");
				sdi.x0_pu = table->GetFloatField("X0_PU");
				sdi.r1_pu = table->GetFloatField("R1_PU");
				sdi.x1_pu = table->GetFloatField("X1_PU");
				// insere Gerador em PreMonta
				pre_monta->InsereSec_Gerador(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaEgrid("Coluna inexistente em ReadSec_TabGeradorBT.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			throw EImportaEgrid("Erro inesperado em ReadSec_TabGeradorBT.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadSec_TabTrafoMTBT(int sec_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_INSTAL_TRAFO sdi;
	AnsiString prog;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de rede SDBT '" + IntToStr(sec_id) + "' ");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}
	try
	{
		table = dao->ExecuteSqlSecTrafo(sec_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid(" Erro ao executar a query em ReadSec_TabTrafoMTBT.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				// sdi.id = table->GetIntField("ID_TRAFO_MTBT");
				sdi.id = table->GetIntField("ID_REDE_BT");
				sdi.circ_id = table->GetIntField("ID_REDE_MT");
				sdi.id_rede_bt = table->GetIntField("ID_REDE_BT");
				sdi.barra_mt_id = table->GetIntField("ID_BARRA_MT");
				sdi.barra_bt_id = table->GetIntField("ID_BARRA_BT");
				sdi.codigo = table->GetStringField("COD_REDE");
				sdi.ativo = table->GetIntField("ATIVO");
				// sdi.tipo = table->GetIntField("TIPO");
				sdi.tipo_lig = table->GetIntField("TP_LIGACAO");
				sdi.tipo_local = table->GetStringField("TP_LOCAL");
				sdi.propriedade = table->GetIntField("PROPRIEDADE");
				sdi.snom_kva = table->GetFloatField("SNOM_KVA");
				sdi.fases_mt = table->GetStringField("FASES_INSTAL_MT");
				sdi.fases_bt = table->GetStringField("FASES_INSTAL_BT");
				sdi.valta_kv = table->GetFloatField("VALTA_KV");
				sdi.vbaixa_v = table->GetFloatField("VBAIXA_V");
				// insere InstalTrafo em PreMonta
				sucesso = pre_monta->InsereSec_Rede(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add(" ReadEtd_TabSe ");
			// log_geral->Add(e.Message);
			throw EImportaEgrid(" Coluna inexistente em ReadSec_TabTrafoMTBT.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add(" ReadEtd_TabSe ");
			// log_geral->Add(e.Message);
			throw EImportaEgrid(" Erro inesperado em ReadSec_TabTrafoMTBT.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadSec_TabLinkBT(int sec_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_LINK_BT sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de link SDBT");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlSecLinkBT(sec_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid(" Erro ao executar a query em ReadSec_TabLinkBT.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.link_id = table->GetIntField("ID_LINK");
				sdi.trecho_id = table->GetIntField("ID_TRECHO");
				sdi.barra_id1 = table->GetIntField("ID_BARRA1");
				sdi.barra_id2 = table->GetIntField("ID_BARRA2");
				// insere LinkBT em PreMonta
				sucesso = pre_monta->InsereSec_Link(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaEgrid(" Coluna inexistente em ReadSec_TabLinkBT.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			throw EImportaEgrid(" Erro inesperado em ReadSec_TabLinkBT.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadSec_TabPCargaBT(int sec_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_PCARGA_BT sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Pontos de Carga SDMT ");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlSecPCargaBT(sec_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid(" Erro ao executar a query em ReadSec_TabPCargaBT.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_PCARGA_BT");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.barra_id = table->GetIntField("ID_BARRA");
				sdi.redebt_id = table->GetIntField("ID_REDE_BT");
				// insere PCargaBT em PreMonta
				pre_monta->InsereSec_PtoCarga(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add(" ReadEtd_TabSe ");
			// log_geral->Add(e.Message);
			throw EImportaEgrid(" Coluna inexistente em ReadSec_TabPCargaBT.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add(" ReadEtd_TabSe ");
			// log_geral->Add(e.Message);
			throw EImportaEgrid(" Erro inesperado em ReadSec_TabPCargaBT.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadSec_TabTrechoBT(int sec_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_TRECHO_BT sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de trecho SDBT ");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlSecTrecho(sec_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid(" Erro ao executar a query em ReadSec_TabTrechoBT.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_TRECHO");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.ativo = table->GetIntField("ATIVO");
				sdi.tipo = table->GetIntField("TIPO");
				sdi.fases = table->GetStringField("FASES");
				sdi.comp_m = table->GetFloatField("COMPRIMENTO_M");
				sdi.arranjo_id = table->GetIntField("ID_ARRANJO");
				sdi.cabo_fase_id = table->GetIntField("ID_CABO_FASE");
				// sdi.cabo_neutro_id = table->GetIntField(" cabo_neutro_id ");
				sdi.barra1_id = table->GetIntField("ID_BARRA1");
				sdi.barra2_id = table->GetIntField("ID_BARRA2");
				sdi.redebt_id = table->GetIntField("ID_REDE");
				// insere TrechoBT em PreMonta
				pre_monta->InsereSec_Trecho(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add(" ReadEtd_TabSe ");
			// log_geral->Add(e.Message);
			throw EImportaEgrid(" Coluna inexistente em ReadSec_TabTrechoBT.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add(" ReadEtd_TabSe ");
			// log_geral->Add(e.Message);
			throw EImportaEgrid(" Erro inesperado em ReadSec_TabTrechoBT.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TImportaArquivoEgrid::ReadSec_TabUtcBT(int sec_id)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	sdi_UTC_BT sdi;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de transformador SDBT");
        if((aborta) && (aborta->Requested))
        {
            return false;
        }
	}

	try
	{
		table = dao->ExecuteSqlSecUtcBT(sec_id);
	}
	catch (EImportaEgridDAO &e)
	{
		table = NULL;
		throw EImportaEgrid(" Erro ao executar a query em ReadSec_TabUtcBT.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				sdi.id = table->GetIntField("ID_TRAFO_MTBT");
				sdi.codigo = table->GetStringField("CODIGO");
				sdi.ativo = table->GetIntField("ATIVO");
				sdi.tipo = table->GetIntField("TIPO");
				sdi.snom_kva = table->GetFloatField("SNOM_KVA");
				sdi.valta_kv = table->GetFloatField("VALTA_KV");
				sdi.vbaixa_v = table->GetFloatField("VBAIXA_V");
				sdi.tap_v = table->GetFloatField("TAP_V");
				sdi.fases_mt = table->GetStringField("FASES_MT");
				sdi.fases_bt = table->GetStringField("FASES_BT");
				sdi.r0_pu = table->GetFloatField("R0_PU");
				sdi.x0_pu = table->GetFloatField("X0_PU");
				sdi.r1_pu = table->GetFloatField("R1_PU");
				sdi.x1_pu = table->GetFloatField("X1_PU");
				sdi.perda_fe_w = table->GetFloatField("PERDA_FERRO_W");
				sdi.perda_cu_w = table->GetFloatField("PERDA_COBRE_W");
				sdi.et_id = table->GetIntField("ID_REDE_BT");
				// insere TrechoBT em PreMonta
				pre_monta->InsereSec_Utc(sdi);
                if((aborta) && (aborta->Requested))
                {
                    sucesso = false;
                    break;
                }
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add(" ReadEtd_TabSe ");
			// log_geral->Add(e.Message);
			throw EImportaEgrid(" Coluna inexistente em ReadSec_TabUtcBT.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			// log_geral->Add(" ReadEtd_TabSe ");
			// log_geral->Add(e.Message);
			throw EImportaEgrid(" Erro inesperado em ReadSec_TabUtcBT.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#/tasks/15031223
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
void __fastcall TImportaArquivoEgrid::Unzip(TList *lisTMP)
{

	// Inicia progresso
	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add(" Carregando subestações : ", true);
        if((aborta) && (aborta->Requested))
        {
            return;
        }
	}
	//verifica se o zip ja foi destruido
	if(zip == NULL)
	{
		IniciaZip();
    }
	// abre o zip
	zip->Open(eGridPath, zmRead);
	// loop p/ todas Subestacoes selecionadas
	for (int i = 0; i < slisNomes->Count; i++)
	{
		AnsiString nome = (AnsiString)slisNomes->Strings[i];
		for (int j = 0; j < lisTMP->Count; j++)
		{
			VTSubestacao* subestacao = (VTSubestacao*)lisTMP->Items[j];
			if (nome.AnsiCompareIC(subestacao->Codigo) == 0)
			{
				if (progresso)
					progresso->Add(nome);
				zip->Extract("Sub_" + nome + ".es", diretorioTemporario, true);
                if((aborta) && (aborta->Requested))
                {
                    break;
                }
			}
		}
	}
	try
	{
		zip->Close();
	}
	catch (const Exception & e)
	{
		Erro(" Erro ao fechar o arquivo !(" + e.Message + ")");
	}
	delete zip;
    zip = NULL;
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaArquivoEgrid::VersaoCompativel(AnsiString &versao)
{
	// variaveis locais
	// variaveis locais
	AnsiString nome, arq_ini, key, section;
	double valor = 1.0; // default
	TIniFile *ini = NULL;
	bool compativel = false;

	// inicia dados
	arq_ini = "meta-inf.ini";
	section = "META-INF";
	key = "VERSION";
	versao = DoubleToStr("%3.1f", valor);

	for (int i = 0; i < zip->FileCount; i++)
	{
		nome = zip->FileNames[i];
		if (nome.AnsiCompareIC(arq_ini) != 0)
			continue;
		// lê arquivo
		zip->Extract(nome, diretorioTemporario, true);
		ini = new TIniFile(diretorioTemporario + "\\" + nome);
		versao = ini->ReadString(section, key, versao);
		valor = StrToDouble(versao);
		if (valor < VERSAO_EGRID)
		{
			break;
		}
		else
		{
			compativel = true;
			break;
		}
	}
	if (ini != NULL)
		delete ini;
	return compativel;
}

// -----------------------------------------------------------------------------
// eof

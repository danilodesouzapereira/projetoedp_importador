// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
// ---------------------------------------------------------------------------
// DLL include
#include <PlataformaSinap\DLL_Inc\BDihm.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Importa.h>
#include <PlataformaSinap\DLL_Inc\Log.h>
#include <PlataformaSinap\DLL_Inc\MontaZ.h>
#include <PlataformaSinap\DLL_Inc\Progresso.h>
#include <PlataformaSinap\DLL_Inc\Radial.h>
#include <PlataformaSinap\DLL_Inc\Rede.h>
#include <PlataformaSinap\DLL_Inc\Redutor.h>
// ---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Arranjo\VTArranjos.h>
#include <PlataformaSinap\Fontes\BDihm\VTSalva.h>
#include <PlataformaSinap\Fontes\Diretorio\VTDir.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Edita\VTEdita.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Importa\IntegraEtd\VTIntegraEtd.h>
#include <PlataformaSinap\Fontes\Importa\Monta\VTMonta.h>
#include <PlataformaSinap\Fontes\Importa\Valfases\VTValfases.h>
#include <PlataformaSinap\Fontes\Importa\ValMalhas\VTValMalhas.h>
#include <PlataformaSinap\Fontes\Importa\ValPerdafe\VTValPerdafe.h>
#include <PlataformaSinap\Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>
#include <PlataformaSinap\Fontes\Log\VTLog.h>
#include <PlataformaSinap\Fontes\MontaZ\VTMontaZ.h>
#include <PlataformaSinap\Fontes\Progresso\VTProgresso.h>
#include <PlataformaSinap\Fontes\Radial\VTRadial.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTDominio.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoRede.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Redutor\VTRedutor.h>
#include <PlataformaSinap\Fontes\Sinap\VTSinapChild.h>
// ---------------------------------------------------------------------------
#include "TConversor.h"
#include "..\Egrid\TImportaArquivoEgrid.h"
#include <ModulosOpcionais\Fontes\ImportaRedesBatch\VTBatchImportaRedesTask.h>
#include "..\Empresa\VTDadosEmpresa.h"
#include "..\Empresa\VTMontaEmpresa.h"
#include "..\Empresa\VTSubestacao.h"
#include "..\PreMonta\VTPreMonta.h"

#include "TRelatorioValidacao.h"
// #ifdef _DEBUG
#include <TLogThread.h>
#include <TLogManager.h>
// #endif

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTConversor* NewObjConversor(VTApl *apl)
{
	return (new TConversor(apl));
}

// ---------------------------------------------------------------------------
__fastcall TConversor::TConversor(VTApl *apl_owner)
{
	// salva ponteiros
	this->apl_owner = apl_owner;
	// cria objeto Apl com objetos necessários p/ a importação
	apl = NewObjApl(NULL, apl_owner);
	LogGeral();
}

// ---------------------------------------------------------------------------
__fastcall TConversor::~TConversor(void)
{
	// destrói objetos
	if (importador)
	{
		delete importador;
		importador = NULL;
	}
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
}

//// ---------------------------------------------------------------------------
// bool __fastcall TConversor::Conecta(void)
// {
// // variáveis locais
// bool sucesso = false;
// VTBDOracle *bd_oracle;
//
// try
// { // cria objeto VTBDOracle
// apl->Add(bd_oracle = NewObjBDOracle(apl));
// // passa log_geral para BDoracle colocar erros, se houverem
// bd_oracle->LogGeral = log_geral;
// // inicia conexão com base de dados
// sucesso = bd_oracle->Conecta(TNSname, Username, Password);
// }
// catch (Exception &e)
// {
// log_geral->Add(e.Message);
// }
// return (sucesso);
// }

// ---------------------------------------------------------------------------
bool __fastcall TConversor::CriaDominioGlobal(void)
{
	// variáveis locais
	AnsiString txt;
	TDateTime DataDominio = Now();
	VTBatchImportaRedesTask *batchImporta = (VTBatchImportaRedesTask*)apl->GetObject(__classid(VTBatchImportaRedesTask));

	try
	{ // define código do dominio com a data atual
		txt.sprintf("Rede_%04d.%02d.%02d", YearOf(DataDominio), MonthOf(DataDominio),
			DayOf(DataDominio));
		dominio_global = DLL_NewObjDominio();
		dominio_global->Id = FalsoId();
		dominio_global->Codigo = txt;
		dominio_global->DataCriacao = DataDominio;
		dominio_global->Obra = NULL;
	}
	catch (Exception &e)
	{
		if (batchImporta != NULL)
		{
			batchImporta->LogAdd("Erro ao criar domínio global");
		}
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TConversor::CriaNovoArqRede(AnsiString dirname, AnsiString cod_base)
{
	// variáveis locais
	AnsiString arq_rede;
	VTDir * dir = (VTDir*)apl->GetObject(__classid(VTDir));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// #ifdef _DEBUG
	function = AnsiString(__FUNC__).c_str();
	msglog = "Inicio de " + AnsiString(function);
	if (log_thread)
		log_thread->AddLog(msglog, function, false, 33);
	// #endif
	// verifica se foi definido cod_base
	if (cod_base.IsEmpty())
		// arq_rede = dirname + "\\BaseUnica.mdb";
		if (!NomeArquivoBaseUnica.IsEmpty())
		{
			arq_rede = dirname + "\\" + NomeArquivoBaseUnica + ".mdb";
		}
		else
		{
			int numero_base = 1;
			arq_rede = dirname + "\\BASE_UNICA.mdb";
			while (FileExists(arq_rede))
			{
				numero_base++;
				arq_rede = dirname + "\\BASE_UNICA_" + IntToStr(numero_base) + ".mdb";
			}

		}
	else
		arq_rede = dirname + "\\Rede_" + cod_base + ".mdb";
	// cria uma nova base de dados vazia sem consultar o usuário
	if (!dir->CriaCopiaRedeSinap(arq_rede, false))
	{
		// #ifdef _DEBUG
		function = AnsiString(__FUNC__).c_str();
		msglog = "Não criou base de dados ";
		msglog += "arq_rede=" + arq_rede;
		if (log_thread)
			log_thread->AddLog(msglog, function);
		// #endif
		return (false);
	}
	// #ifdef _DEBUG
	function = AnsiString(__FUNC__).c_str();
	msglog = "Criou base de dados ";
	msglog += "arq_rede=" + arq_rede;
	if (log_thread)
		log_thread->AddLog(msglog, function, false, 34);
	// #endif
	// atualiza objeto Path p/ apontar p/ nova base de dados
	path->ArqRede = arq_rede;

	// DEIXA O ARQ REDE OCULTO durante a importação
	int iSize = arq_rede.WideCharBufSize();
	wchar_t *wTemp = new wchar_t[iSize];
	arq_rede.WideChar(wTemp, iSize);
	// use wTemp as needed...
	int attr = GetFileAttributes(wTemp);
	if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0)
	{
		attr = attr | FILE_ATTRIBUTE_HIDDEN;
		SetFileAttributes(wTemp, attr);
	}
	delete[]wTemp;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TConversor::CriaNovoDiretorio(AnsiString &dirname)
{
	// variáveis locais
	WideString pathanme;
	AnsiString pasta = "";
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// verifica se usuário definiu a pasta destino
	if (!Pasta.IsEmpty())
	{
		if (DirectoryExists(Pasta))
		{
			dirname.sprintf("%s\\Redes_%04d.%02d.%02d", Pasta.c_str(), YearOf(Now()),
				MonthOf(Now()), DayOf(Now())).c_str();
		}
	}
	else
	{
		pasta = path->DirBase + "\\Redes";
		dirname.sprintf("%s\\Redes_%04d.%02d.%02d", pasta.c_str(), YearOf(Now()), MonthOf(Now()),
			DayOf(Now()));
		Aviso("Destino default das redes no diretório base: " + pasta);
	}
	// define nome do diretório
	// dirname.sprintf("%s\\Redes_%04d.%02d.%02d", path->DirBase.c_str(), YearOf(Now()),
	// MonthOf(Now()), DayOf(Now()));
	// verifica se o diretório já existe
	if (DirectoryExists(dirname))
	{
		return (true);
	}
	// cria o diretório
	pathanme = dirname;
	return (CreateDir(pathanme.c_bstr()));
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TConversor::DefineDiretorio(AnsiString &dirname)
{
	// variáveis locais
	WideString pathanme;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// verifica se usuário definiu a pasta destino
	if (!Pasta.IsEmpty())
	{
		if (DirectoryExists(Pasta))
		{
			dirname = Pasta;
			return (true);
		}
	}
	// define nome do diretório
	dirname.sprintf("%s\\Redes_%04d.%02d.%02d", path->DirBase.c_str(), YearOf(Now()),
		MonthOf(Now()), DayOf(Now()));
	// verifica se o diretório já existe
	if (DirectoryExists(dirname))
	{
		return (true);
	}
	// cria o diretório
	pathanme = dirname;
	return (CreateDir(pathanme.c_bstr()));
}

// ---------------------------------------------------------------------------
void __fastcall TConversor::ExibeGrafico(void)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// exibe o gráfico
	grafico->AreaRedeDefine();
	Application->ProcessMessages();
}
// ---------------------------------------------------------------------------
void __fastcall TConversor::GeraRelatorioEqpto(void)
{
	TRelatorioValidacao * rel = NULL;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	if (opcao->rel_eqpto.gera_relatorio_eqpto)
	{

		try
		{
			rel  = new TRelatorioValidacao(apl);
			rel->Executa(opcao->rel_eqpto.dir_rel_eqpto);
		}
		catch (Exception &e)
		{
			LogErro(nome_arq_log, e.Message);
        }
		catch (...)
		{
			LogErro(nome_arq_log, "Erro inesperado ao gerar relatorios de eqptos.");
		}
		if (rel)
		{
			delete rel;
			rel = NULL;
		}
	}
}
// ---------------------------------------------------------------------------
bool __fastcall TConversor::ImportaDadosEmpresa(int empresa_id)
	// bool __fastcall TConversor::ImportaDadosEmpresa(AnsiString fileName)
{
	// // variáveis locais
	// bool sucesso = false;
	// VTMontaEmpresa* me;
	// VTProgresso *progresso;
	//
	// try
	// { // cria objetos necessários p/ a importação
	// apl->Add(me = NewObjMontaEmpresa(apl));
	// // verifica se o objeto já foi criado
	// if (importador != NULL)
	// { // destroi para recriar
	// delete importador;
	// importador = NULL;
	// }
	// apl->Add(progresso = DLL_NewObjProgresso(NULL, NULL));
	// // exibe progresso
	// progresso->Start(progTEXTO);
	// // cria objeto importador e passa pra ele o nome do arquivo selecionado
	// // FALTA PASSAR O FILENAME E NÃO EMPRESA_ID
	// importador = new TImportaArquivoEgrid(this->apl, fileName);
	// // lê conjunto de Regionais, Localidades e Subestações
	// if (sucesso)
	// sucesso = importador->LeCfgRegional(empresa_id);
	// if (sucesso)
	// sucesso = importador->LeCfgLocalidade();
	// if (sucesso)
	// sucesso = importador->LeCfgSubestacao();
	// if (sucesso)
	// sucesso = importador->LeCfgPrimario();
	// if (sucesso)
	// sucesso = importador->LeCfgCabo();
	// // fecha o progresso
	// progresso->Stop();
	// // fecha log geral
	// log_geral->Close();
	// }
	// catch (Exception &e)
	// {
	// log_geral->Add(e.Message);
	// // fecha log geral
	// log_geral->Close();
	// sucesso = false;
	// }
	//
	// return (sucesso);
	return false;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18497425
bool __fastcall TConversor::ImportaDadosEmpresa(AnsiString fileName) throw(EImportaEgrid)
{
	// variáveis locais
	bool sucesso = true;
	VTMontaEmpresa* me;
	VTProgresso *progresso;

	try
	{ // cria objetos necessários p/ a importação
		apl->Add(me = NewObjMontaEmpresa(apl));
		// verifica se o objeto já foi criado
		if (importador != NULL)
		{ // destroi para recriar
			delete importador;
			importador = NULL;
		}
		progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso == NULL)
		{
			apl->Add(progresso = DLL_NewObjProgresso(apl));
		}
		// exibe progresso
		progresso->Start(progTEXTO);
		// cria objeto importador e passa pra ele o nome do arquivo selecionado
		importador = new TImportaArquivoEgrid(this->apl, fileName);
		importador->BatchMode = this->BatchMode;
		importador->CriaDAO("");
		// lê conjunto de Regionais, Localidades e Subestações
		if (sucesso)
			sucesso = importador->LeCfgRegional();
		if (sucesso)
			sucesso = importador->LeCfgLocalidade();
		if (sucesso)
			sucesso = importador->LeCfgSubestacao();
		if (sucesso)
			sucesso = importador->LeCfgPrimario();
		if (sucesso)
			sucesso = importador->LeCfgArranjo();
		// fecha o progresso
		progresso->Stop();
		// fecha log geral
		log_geral->Close();
		importador->DestroiDAO();
	}
	catch (Exception &e)
	{
		log_geral->Add(e.Message);
		// fecha log geral
		log_geral->Close();
		throw;
		sucesso = false;
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#/tasks/15031223
bool __fastcall TConversor::ImportaRedes(TList *lisSUBESTACAO, strOPImporta *opcao, int destino)
{
	// variáveis locais
	bool sucesso = false;
	VTLog *log_completo;
	VTPreMonta *pre_monta;
	VTProgresso *progresso;
	VTRadial *radial;
	VTRedes *redes;
	VTSubestacao *subestacao;
	VTDadosEmpresa *empresa = (VTDadosEmpresa*)apl->GetObject(__classid(VTDadosEmpresa));

	try
	{ // salva opção

		this->opcao = opcao;

		// protecao
		if (lisSUBESTACAO == NULL)
		{
			if (empresa != NULL)
			{
				lisSUBESTACAO = empresa->LisSubestacao();
			}
		}
		if (lisSUBESTACAO == NULL)
		{
			return (false);
		}
		importador->Unzip(lisSUBESTACAO);
		// salva subestações selecionadas para importação
		empresa->LisSubSelecionada = lisSUBESTACAO;
		// cria objeto Apl e todos os objetos necessários p/ a importação
		if (!IniciaObjApl())
		{
			return (false);
		}
		// obtém objetos criados
		progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		pre_monta = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));

		// define opção de consumidor A4
		pre_monta->DataDominio = opcao->date;
		pre_monta->ConsA4 = opcao->pri.consA4;
		// exibe progresso

		if (!BatchMode)
		{
			if (progresso)
			{
				progresso->Start(progTEXTO);
			}
		}
		// inicia log
		// LogInicia();
		// verifica destino das redes importadas
		switch (destino)
		{
		case 0:
			// case destINDEX_MEMORIA: // carregar redes em memória  (destinoINDEX VTConversor)
			sucesso = ImportaToMemoria(lisSUBESTACAO);
			break;
		case 1:
			// case destINDEX_BASEUNICA: // salvar redes em uma única base Acces
			sucesso = ImportaToBaseUnica(lisSUBESTACAO);
			break;
		case 2:
			// case destINDEX_BASESEPARADA: // salvar cada rede em uma base Acces
			sucesso = ImportaToBaseMultipla(lisSUBESTACAO);
			break;
		default:
			sucesso = false;
		}
		// fecha progresso
		if (progresso)
			progresso->Stop();
		// fecha log e exibe o relatório
		// LogFecha();
		importador->Close();
		LogShow();
		// destrói objeto VTApl e todos seus objetos
        //destrutor ira destruir apl
//		delete apl;
//		apl = NULL;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TConversor::ImportaToBaseMultipla(TList *lisSUBESTACAO)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString dirname, arq_log;
	TDateTime start, start_tudo;
	VTPreMonta *pre_monta;
	VTRadial *radial;
	VTRedes *redes;
	VTSubestacao *subestacao;
	VTSalva *salva;
	VTBatchImportaRedesTask *batchImporta;
	VTDir *dir;

	try
	{ // obtém objetos criados
		dir = (VTDir*)apl->GetObject(__classid(VTDir));
		batchImporta = (VTBatchImportaRedesTask*)apl->GetObject(__classid(VTBatchImportaRedesTask));
		pre_monta = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));
		radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
		redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
		salva = (VTSalva*)apl->GetObject(__classid(VTSalva));

		start_tudo = Now();

		// habilita Progresso na gravação
		if (!BatchMode)
		{
			salva->Progresso = true;
		}
		else
		{
			salva->Progresso = false;
			if (batchImporta != NULL)
			{
				batchImporta->LogAdd
					("\n--------------------------------------------------------", false);
				batchImporta->LogAdd
					("-----DESCRIÇÃO DO PROCESSO DE IMPORTAÇÃO DAS REDES------", false);
				batchImporta->LogAdd
					("--------------------------------------------------------\n", false);
				batchImporta->LogAdd("Inicio do processo de Importação Geral\n");
			}
		}

		// define diretório para armazenas as redes
		if (!BatchMode)
		{
			if (!DefineDiretorio(dirname))
			{
				return (false);
			}
			// inicia log
			nome_arq_log = "LogRedes";
			LogInicia();
		}
		else
		{ // cria diretório para armazenar as redes
			if (!CriaNovoDiretorio(dirname))
				return (false);
		}
		// loop p/ todas as Subestacoes
		for (int n = 0; (sucesso) && (n < lisSUBESTACAO->Count); n++)
		{
			subestacao = (VTSubestacao*)lisSUBESTACAO->Items[n];
			// salva instante inicial
			start = Now();
			// reinicia Redes e Arranjos
			ReiniciaRedes(true);
			// inicia log
			nome_arq_log = subestacao->Codigo;
			if (!BatchMode)
			{
				LogInicia();
			}
			else
			{
				if (batchImporta != NULL)
				{
					batchImporta->LogAdd("Inicio da Importação da rede " + subestacao->Codigo);
				}
			}
			// importa a subestação
			importador->CriaDAO(subestacao->Codigo);
			importador->InsereArranjo();
			if (batchImporta != NULL)
			{
				batchImporta->LogAdd("Leitura das redes");
			}
			sucesso = importador->LeRedes(subestacao, opcao);
			importador->DestroiDAO();

			// inicia objeto Radial
			if (sucesso)
			{
				radial->Inicia(redes);
				// verifica existência de malhas
				if (batchImporta != NULL)
				{
					batchImporta->LogAdd("Verificação de Malhas");
				}
				VerificaMalhas();
				// verifica consistência das fases dos equipamentos
				if (opcao->valida_fase)
				{
					if (batchImporta != NULL)
					{
						batchImporta->LogAdd("Verificação de Fases");
					}
					VerificaFases();
				}
				// verifica consistência das perdas no ferro dos trafos
				if (opcao->corrige_perda_fe)
				{
					if (batchImporta != NULL)
					{
						batchImporta->LogAdd
							("Verificação da consistência das perdas no ferro dos transformadores");
					}
					VerificaPerdaFerro();
				}

				// verifica existência de malhas
				// VerificaMalhasBT();
				// integração com ETD completa
				//IntegraETDcompleta();
				// atualiza o gráfico
				//ExibeGrafico();
			}
            //gera relatorios de eqptos se opcao true
			GeraRelatorioEqpto();
			//não salva caso tenha sido escolhido
			if (opcao->rel_eqpto.gera_relatorio_eqpto && !opcao->rel_eqpto.salva_rede)
			{
                //faz nada
			}
			else
			{
			// cria nova base de dados para a rede
				sucesso = CriaNovoArqRede(dirname, subestacao->Codigo);
				//salva rede na base
				if (sucesso)
				{
					if (BatchMode)
					{
						if (batchImporta != NULL)
						{
							batchImporta->LogAdd("Fim da importação da rede " + subestacao->Codigo);
							batchImporta->SalvaRede(subestacao->Codigo);
						}
					}
					else
					{
						SalvaRede();
					}
				}
			}
			LogSucesso(nome_arq_log, start, Now());
		}
		if (!BatchMode)
		{
			// fecha log
			LogFecha();
			// salva tempo total no log resumo
			LogSucesso("", start_tudo, Now());
		}
		else
		{
			dir->DeleteDirectory(dirname);
		}

	}
	catch (Exception &e)
	{
		LogErro(nome_arq_log, e.Message);
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TConversor::ImportaToBaseUnica(TList *lisSUBESTACAO)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString dirname;
	TDateTime start_rede, start_tudo;
	VTBatchImportaRedesTask *batchImporta;
	VTPreMonta *pre_monta;
	VTRadial *radial;
	VTRedes *redes;
	VTSubestacao *subestacao;
	VTSalva *salva;
	VTDir *dir;

	try
	{ // obtém objetos criados
		dir = (VTDir*)apl->GetObject(__classid(VTDir));
		pre_monta = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));
		batchImporta = (VTBatchImportaRedesTask*)apl->GetObject(__classid(VTBatchImportaRedesTask));
		radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
		redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
		salva = (VTSalva*)apl->GetObject(__classid(VTSalva));
		// VTSinapChild *sinap_child = (VTSinapChild*) apl->GetObject(__classid(VTSinapChild));

		// salva instante inicial
		start_tudo = Now();
		// habilita Progresso na gravação
		if (!BatchMode)
		{
			salva->Progresso = true;
		}
		else
		{
			salva->Progresso = false;
		}
        if (batchImporta != NULL)
		{
			batchImporta->LogAdd
				("\n--------------------------------------------------------", false);
			batchImporta->LogAdd("-----DESCRIÇÃO DO PROCESSO DE IMPORTAÇÃO DAS REDES------", false);
			batchImporta->LogAdd
				("--------------------------------------------------------\n", false);
			batchImporta->LogAdd("Inicio do processo de Importação Geral\n");
		}
		if (!BatchMode)
		{ // cria diretório para armazenar as redes
			if (!DefineDiretorio(dirname))
			{
				return (false);
			}
		}
		else
		{ // cria diretorio para armazenar as redes // BATCHMODE
			if (!CriaNovoDiretorio(dirname))
				return (false);
		}
		// cria nova base de dados única para todas subestações
		if (!CriaNovoArqRede(dirname))
		{
			return (false);
		}
		// reinicia Redes e Arranjos
		ReiniciaRedes(true);
		// cria dominio global
		if (!CriaDominioGlobal())
			return false;
		// loop p/ todas as Subestacoes
		for (int n = 0; (sucesso) && (n < lisSUBESTACAO->Count); n++)
		{
			subestacao = (VTSubestacao*)lisSUBESTACAO->Items[n];
			// salva instante inicial
			start_rede = Now();
			if (!BatchMode)
			{// reinicia Redes e Arranjos menos dominio
				ReiniciaRedes(false);
				// inicia log
				nome_arq_log = subestacao->Codigo;
				LogInicia();
			}
			// importa uma nova subestação
			importador->CriaDAO(subestacao->Codigo);
			importador->InsereArranjo();
			if (batchImporta != NULL)
			{
				batchImporta->LogAdd("Leitura das redes: " + subestacao->Codigo);
			}
			sucesso = importador->LeRedes(subestacao, opcao);
			importador->DestroiDAO();
			// inicia objeto Radial
			if (sucesso)
			{
				radial->Inicia(redes);
				// verifica existência de malhas
				if (batchImporta != NULL)
				{
					batchImporta->LogAdd("Verificação de Malhas");
				}
				VerificaMalhas();
				// verifica consistência das fases dos equipamentos
				if (opcao->valida_fase)
				{
					if (batchImporta != NULL)
					{
						batchImporta->LogAdd("Verificação de Fases - Rede: " + subestacao->Codigo);
					}
					VerificaFases();
				}
				// verifica consistência das perdas no ferro dos trafos
				if (opcao->corrige_perda_fe)
				{
					if (batchImporta != NULL)
					{
						batchImporta->LogAdd
							("Verificação da consistência das perdas no ferro dos transformadores - Rede: " +
							subestacao->Codigo);

					}
					VerificaPerdaFerro();
				}

				// integração com ETD completa
				IntegraETDcompleta();
				// if (sinap_child != NULL)
				// {
				// if (sinap_child->ActionRedeCarregada != NULL)
				// { // executa Action externa
				// sinap_child->ActionRedeCarregada->Execute();
				// }
				// }
				// atualiza o gráfico
				//ExibeGrafico();
				//gera relatorios de eqptos se opcao true
				GeraRelatorioEqpto();
				// altera o dominio das redes para dominio global
				if (!RedefineDominio())
				{
					sucesso = false;
					continue;
				}
				//não salva caso tenha sido escolhido
				if (opcao->rel_eqpto.gera_relatorio_eqpto && !opcao->rel_eqpto.salva_rede)
				{
					//faz nada
				}
				else
				{
					if (batchImporta != NULL)
					{
						batchImporta->LogAdd("Fim da importação da rede (ou conjunto de redes)");
						batchImporta->SalvaRede("BASE_UNICA");
					}
					else
					{ // salva rede na base
						SalvaRede();
					}
                }
                if (!BatchMode)
				{ // fecha log
					LogFecha();
				}
				else
				{
					dir->DeleteDirectory(dirname);
				}
			}
			//reinicia redes
			redes->Clear(false);
		}
        if (batchImporta != NULL)
		{
			batchImporta->LogAdd("Finalização");
			batchImporta->Finaliza(true);
		}
		LogSucesso(nome_arq_log, start_rede, Now());
	}
	catch (Exception &e)
	{
		LogErro(nome_arq_log, e.Message);
		sucesso = false;
	}
	// destroi dominio global
	redes->RemoveDominio(dominio_global);
	if (dominio_global)
	{
		delete dominio_global;
		dominio_global = NULL;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TConversor::ImportaToGateway(TList *lisSUBESTACAO)
{
	return (false);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#/tasks/15031223
bool __fastcall TConversor::ImportaToMemoria(TList *lisSUBESTACAO)
{
	// variáveis locais
	bool sucesso = true;
	TDateTime start;
	VTRadial *radial;
	VTRedes *redes;
	VTSubestacao *subestacao;
	VTSinapChild *sinap_child = (VTSinapChild*) apl->GetObject(__classid(VTSinapChild));

	try
	{ // obtém objetos criados

		radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
		redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
		// reinicia Redes e Arranjos
		ReiniciaRedes(true);
		importador->InsereArranjo();

		// loop p/ todas as Subestacoes
		for (int n = 0; (sucesso) && (n < lisSUBESTACAO->Count); n++)
		{
			subestacao = (VTSubestacao*)lisSUBESTACAO->Items[n];
			// salva instante inicial
			start = Now();
			// inicia log
			nome_arq_log = subestacao->Codigo;
			importador->InsereArranjo();
			LogInicia();
			importador->CriaDAO(subestacao->Codigo);
			sucesso = importador->LeRedes(subestacao, opcao);
			importador->DestroiDAO();
			// fecha log
			if (sucesso)
				LogSucesso(nome_arq_log, start, Now());
			// LogFecha();
		}

		if (sucesso)
		{
			// inicia objeto Radial
			radial->Inicia(redes);
			// Verifica existencia de malhas
			VerificaMalhas();
			// verifica consistência das fases dos equipamentos
			if (opcao->valida_fase)
				VerificaFases();
			// verifica consistência das perdas no ferro dos trafos
			if (opcao->corrige_perda_fe)
				VerificaPerdaFerro();
			// verifica existência de malhas
			// VerificaMalhas();
			// integração com ETD completa
			IntegraETDcompleta();
			// verifica se foi definida Action externa para ajustar rede carregada
			if (sinap_child != NULL)
			{
				if (sinap_child->ActionRedeCarregada != NULL)
				{ // executa Action externa
					sinap_child->ActionRedeCarregada->Execute();
				}
			}
			//gera relatorio
			GeraRelatorioEqpto();
		}
        // fecha log
		LogFecha();
	}
	catch (Exception &e)
	{
		LogErro(nome_arq_log, e.Message);
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TConversor::IniciaObjApl(void)
{
	VTProgresso *progresso;
	try
	{ // cria objetos opcionais e insere em Apl
		if (!opcao->ihm.progresso)
		{
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
			if (progresso != NULL)
			{
				apl->Remove(progresso);
			}
			delete progresso;
		}

		if (opcao->ihm.log)
		{
			apl->Add(DLL_NewObjLog(apl));
			log_resumo = DLL_NewObjLog(apl);
		}
		// cria objetos do importador
		apl->Add(DLL_NewObjMontaZ(apl));
		apl->Add(DLL_NewObjMontaRede(apl));
		apl->Add(DLL_NewObjRadial(apl));
		apl->Add(DLL_NewObjSalva(NULL, apl));
		// cria objeto PreMonta conforme a opção da rede secundária
		switch (opcao->sec.topologia)
		{
		case opSEC_CARGA:
			apl->Add(NewObjPreMonta1(apl));
			break;
		case opSEC_UTC:
			apl->Add(NewObjPreMonta2(apl));
			break;
		case opSEC_GIS:
			apl->Add(NewObjPreMonta3(apl));
			break;
		default:
			apl->Add(NewObjPreMonta1(apl));
			break;
		}
	}
	catch (Exception &e)
	{
		log_geral->Add(e.Message);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TConversor::IntegraETDcompleta(void)
{
	// variáveis locais
	bool sucesso;
	VTIntegraEtd *integra_etd;

	// verifica se deve integrar ETD
	if (opcao->sub.topologia != opSUB_TROCAR)
		return (true);
	try
	{ // cria objeto p/ fazer integração
		integra_etd = DLL_NewObjIntegraEtd(apl);
		sucesso = integra_etd->InsereETDcompleta();
		delete integra_etd;
	}
	catch (Exception &e)
	{
		if (integra_etd)
			delete integra_etd;
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TConversor::LogErro(AnsiString arq_txt, AnsiString msg_erro)
{
	// vsriáveis locais
	AnsiString msg;

	// insere mensagem de log
	if (log_resumo)
	{
		log_resumo->Add(msg.sprintf("Erro:    %-20s, Diagnóstico: %s", arq_txt.c_str(),
			msg_erro.c_str()));
	}

}

// ---------------------------------------------------------------------------
void __fastcall TConversor::LogFecha(void)
{
	// variáveis locais
	VTLog *plog = (VTLog*)apl->GetObject(__classid(VTLog));

	if (plog)
	{ // fecha Log completo
		plog->Close();
		// fecha Log resumo
		if (log_resumo)
		{
			log_resumo->Close();
		}
		// exibe o relatório
		// plog->ShowModal("Log de Importação");
	}
	if (log_geral)
		log_geral->Close();
}

// ---------------------------------------------------------------------------
void __fastcall TConversor::LogGeral(void)
{
	// variáveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString directory;

	// cria log geral, com informações antes de importar redes
	log_geral = DLL_NewObjLog(apl);
	if (log_geral)
	{
		directory = path->DirTmp() + "\\Logs\\";
		ForceDirectories(directory);
		log_geral->Open(path->DirTmp() + "\\Logs\\_Log_Geral.log");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TConversor::LogInicia(void)
{
	// variáveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTLog *plog = (VTLog*)apl->GetObject(__classid(VTLog));
	AnsiString directory;

	if (plog)
	{ // abre arquivo
		// plog->Open(path->DirTmp() + "\\ImportaEO.log");
		if (log_resumo)
		{
			directory = path->DirTmp() + "\\LogsResumo\\";
			ForceDirectories(directory);
			log_resumo->Open(path->DirTmp() + "\\LogsResumo\\" + nome_arq_log + "_Resumo.log");
		}
		directory = path->DirTmp() + "\\Logs\\";
		ForceDirectories(directory);
		plog->Open(path->DirTmp() + "\\Logs\\" + nome_arq_log + ".log");
		// inicia campos do Log completo
		plog->InsereCampo("Rede");
		plog->InsereCampo("Eqpto");
		plog->InsereCampo("Código");
		plog->InsereCampo("Descrição");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TConversor::LogShow(void)
{
	// variáveis locais
	VTLog *plog = (VTLog*)apl->GetObject(__classid(VTLog));

	if (!BatchMode)
	{
		if (plog)
		{ // exibe o relatório
			plog->ShowModal("Log de Importação");
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TConversor::LogSucesso(AnsiString arq_txt, TDateTime start, TDateTime stop)
{
	// variáveis locais
	AnsiString msg;
	int t_mseg;
	unsigned short hour, min, sec, msec;
	TDateTime dif;

	// determina intervalo de tempo em mseg
	dif = stop - start;
	dif.DecodeTime(&hour, &min, &sec, &msec);
	t_mseg = (1000 * ((60 * ((60 * hour) + min)) + sec)) + msec;
	msg = "Sucesso: " + arq_txt + ", Tempo: " + hour + "h" + min + "min" + sec + "s" + msec + "ms";
	// insere mensagem de log
	// log_resumo->Add(msg.sprintf("Sucesso: %-20s, Tempo: %d ms", arq_txt, t_mseg));
	if (log_resumo)
	{
		log_resumo->Add(msg);
	}
}

//
// ---------------------------------------------------------------------------
void __fastcall TConversor::MoveLigacaoDeParaNovaBarra(VTLigacao *lig, VTBarra *de)
{
	VTBarra *barraClone;
	VTLigacao *depois;

	if (de != NULL)
	{
		barraClone = de->Clone();
		barraClone->Codigo = de->Codigo + "_2";
		barraClone->Id = FalsoId();
		barraClone->Obj = de;
		de->Obj = barraClone;
		// insere a barra nova na lista de barras à serem criadas
		lisNOVASBARRAS->Add(barraClone);
		// agora move a ligacao
		depois = lig->Clone();
		lig->Obj = depois;
		// redefine Barra da Ligacao
		if (de == depois->pbarra1)
			depois->pbarra1 = barraClone;
		else if (de == depois->pbarra2)
			depois->pbarra2 = barraClone;
		else if (de == depois->pbarra3)
			depois->pbarra3 = barraClone;
		// adiciona a ligacao à lista de eqptos alterados
		lisEQPTOSALTERADOS->Add(lig);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TConversor::RedefineDominio(void)
{
	// variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisDOMINIO, *lisREDE;
	VTBatchImportaRedesTask *batchImporta = (VTBatchImportaRedesTask*)apl->GetObject(__classid(VTBatchImportaRedesTask));

	try
	{ // remove redes anteriores do dominio
		dominio_global->LisRede()->Clear();
		lisREDE = redes->LisRede();
		for (int n = 0; n < lisREDE->Count; n++)
		{ // insere todas as redes no dominio global
			rede = (VTRede*)lisREDE->Items[n];
			dominio_global->InsereRede(rede);
		}
		// destroi demais dominios do VTRedes
		lisDOMINIO = redes->LisDominio();
		// retira dominio_global, se houver
		if (lisDOMINIO->IndexOf(dominio_global) >= 0)
		{
			lisDOMINIO->Remove(dominio_global);
		}
		LimpaTList(lisDOMINIO);
		// insere dominio global no VTRedes
		redes->InsereDominio(dominio_global);
	}
	catch (Exception &e)
	{
		if (batchImporta != NULL)
		{
			batchImporta->LogAdd("Erro na redefinição dos domínios: " + e.Message);
		}
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TConversor::ReiniciaRedes(bool inclusive_dominio)
{
	// variáveis locais
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// reinicia Redes
	redes->Clear(inclusive_dominio);
	// reinicia Arranjos
	arranjos->Clear();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TConversor::SalvaRede(void)
{
	// variáveis locais
	VTRedutor *redutor;
	VTSalva *salva = (VTSalva*)apl->GetObject(__classid(VTSalva));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// cria objeto VTRedutor
	if ((redutor = DLL_NewObjRedutor(apl)) == NULL)
	{
		return;
	}
	// desabilita mensagens do redutor
	redutor->AvisoEnabled = false;
	redutor->ProgressoEnabled = false;
	// monta redes reduzidas
	redutor->ExecutaReducaoMNet();
	// atualiza base de dados
	salva->SalvaRede();
	// destroi objeto
	delete redutor;

	// tIRA ATRIBUTO OCULTO
	int iSize = path->ArqRede.WideCharBufSize();
	wchar_t *wTemp = new wchar_t[iSize];
	path->ArqRede.WideChar(wTemp, iSize);
	int attr = GetFileAttributes(wTemp);
	if ((attr & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN)
	{
		attr = attr&~FILE_ATTRIBUTE_HIDDEN;
		SetFileAttributes(wTemp, attr);
	}
	delete[]wTemp;
}

// -----------------------------------------------
void __fastcall TConversor::VerificaBarrasFronteira(VTRede *redeSec)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisLIGSEC;
	TList *lisBARRASEC;
	TList *lisAUXLIG;
	VTLigacao *ligacao;
	VTBarra *barra1, *barra2, *barra3, *barra;

	// protecao
	if (redeSec == NULL)
	{
		return;
	}
	// lisLIGSEC   = new TList;
	// lisBARRASEC = new TList;
	lisBARRASEC = redeSec->LisBarra();
	// zera todas tags
	for (int nb = 0; nb < lisBARRASEC->Count; nb++)
	{
		barra = (VTBarra*) lisBARRASEC->Items[nb];
		barra->Tag = 0;
	}
	// percorre todas ligacoes incrementando as tags
	lisLIGSEC = redeSec->LisLigacao();
	for (int nl = 0; nl < lisLIGSEC->Count; nl++)
	{
		ligacao = (VTLigacao*) lisLIGSEC->Items[nl];
		if (ligacao->pbarra1 != NULL)
		{
			ligacao->pbarra1->Tag++;
		}
		if (ligacao->pbarra2 != NULL)
		{
			ligacao->pbarra2->Tag++;
		}
		if (ligacao->pbarra3 != NULL)
		{
			ligacao->pbarra3->Tag++;
		}
	}
	// percorre as barras novamente procurando as barras que ficam com tag 1
	lisAUXLIG = new TList;
	for (int nb = 0; nb < lisBARRASEC->Count; nb++)
	{
		barra = (VTBarra*) lisBARRASEC->Items[nb];
		if (barra->Tag == 1)
		{ // como a barra tem tag 1, quer dizer que eh ponta solta ou fronteira
			// verifica as ligacoes dessa barra
			lisAUXLIG->Clear();
			redes->LisLigacao(lisAUXLIG, barra);
			if (lisAUXLIG->Count > 1)
			{ // percorre a lista de ligacoes à procura de ligacao de outra rede bt
				for (int nr = 0; nr < lisAUXLIG->Count; nr++)
				{
					ligacao = (VTLigacao*) lisAUXLIG->Items[nr];
					// verifica se a ligacao pertence à rede analizada
					if (redeSec != ligacao->rede)
					{ // verifica o tipo de segmento
						if (ligacao->rede->TipoRede->Segmento == redeSEC)
						{
							MoveLigacaoDeParaNovaBarra(ligacao, barra);
						}
					}
				}
			}
		}
	}

	DestroiTObject(lisAUXLIG);
	// DestroiTObject(lisLIGSEC);
	// DestroiTObject(lisBARRASEC);
}

// ---------------------------------------------------------------------------
void __fastcall TConversor::VerificaFases(void)
{
	// variáveis locais
	int num_eqpto;
	TList *lisEXT;
	VTEqpto *eqpto;
	VTValFases *valfases;

	VTLog *plog = (VTLog*)apl->GetObject(__classid(VTLog));

	// verifica a opção
	if ((!opcao->valida_fase) && (!opcao->corrige_fase))
		return;
	try
	{ // cria objeto VTValfase
		valfases = DLL_NewObjValFases(apl);
		valfases->CorrigeFases = opcao->corrige_fase;
		valfases->Executa();
		if ((num_eqpto = valfases->ExisteEqptoComFaseInvalida()) > 0)
		{ // atualiza campos do Log
			if (plog)
				plog->DefineValor("Rede", "");
			// preenche log
			lisEXT = valfases->LisEqptoComFaseInvalida();
			for (int n = 0; n < lisEXT->Count; n++)
			{
				eqpto = (VTEqpto*)lisEXT->Items[n];
				if (plog)
					plog->DefineValor("Eqpto", eqpto->TipoAsString());
				if (plog)
					plog->Add("Fase Incompatível");
				// altera código do eqpto p/ facilitar localização
				eqpto->Codigo = eqpto->Codigo + "Fase Incompatível";
			}
			/*
			 if (plog)
			 {
			 Aviso("Existem " + IntToStr(num_eqpto) + " eqptos com fases incompatíveis\n" +
			 "Estes eqptos receberam o código 'Fase Incompatível'\n\n" +
			 "IMPORTANTE: Este tipo de problema inviabiliza o cálculo de fluxo desequilibrado");
			 }
			 */
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói objeto criado
	if (valfases)
	{
		delete valfases;
	}
}

/// -------------------------------------------------------------------------------------------------
void __fastcall TConversor::VerificaMalhas(void)
{
	// variáveis locais
	VTValMalhas *valmalhas;
	int num_opc = 0;

	// #ifdef _DEBUG
	function = __FUNC__;
	msglog = "Inicio de " + AnsiString(function);
	if (log_thread)
		log_thread->AddLog(msglog, function);
	// #endif

	try
	{ // verifica se deve verificar malhas
		// if(! opcao->corrige_malha_mts) return;

		if (opcao->malha.abre_chave_ext)
			num_opc++;
		if (opcao->malha.abre_chave_int)
			num_opc++;
		if (opcao->malha.desloca_trecho_ext)
			num_opc++;
		if (opcao->malha.desloca_trecho_int)
			num_opc++;

		if (num_opc == 0)
			return;

		// cria objeto VTValMalhas
		if ((valmalhas = DLL_NewObjValMalhas(apl)) == NULL)
		{
			// #ifdef _DEBUG
			function = __FUNC__;
			msglog = "Erro ao criar o valmalhas em ";
			msglog += AnsiString(function);
			if (log_thread)
				log_thread->AddLog(msglog, function, true);
			// #endif
			return;
		}
		// executa verificação de malhas
		// #ifdef _DEBUG
		function = __FUNC__;
		msglog = "Executa o valmalhas" + AnsiString(function);
		if (log_thread)
			log_thread->AddLog(msglog, function);
		// #endif

		// guarda opções de abertura de malha
		valmalhas->AbreChaveExt = opcao->malha.abre_chave_ext;
		valmalhas->AbreChaveInt = opcao->malha.abre_chave_int;
		valmalhas->DeslocaTrechoExt = opcao->malha.desloca_trecho_ext;
		valmalhas->DeslocaTrechoInt = opcao->malha.desloca_trecho_int;
		  valmalhas->SomenteRedePri = opcao->malha.somente_rede_pri;
		// executa
		valmalhas->Executa();

		// #ifdef _DEBUG
		function = __FUNC__;
		msglog = "Valmalhas executado" + AnsiString(function);
		if (log_thread)
			log_thread->AddLog(msglog, function);
		// #endif
		// destrói onbjeto
		delete valmalhas;
	}
	catch (Exception &e)
	{ // nada a fazer

		Erro(e.Message);
		// #ifdef _DEBUG
		function = __FUNC__;
		msglog = "Exception caught em ";
		msglog += AnsiString(function);
		msglog += " " + e.Message;
		if (log_thread)
			log_thread->AddLog(msglog, function, true);
		// #endif
	}
}
/*
 // ---------------------------------------------------------------------------
 void __fastcall TConversor::VerificaMalhas(void)
 {
 // variáveis locais
 VTValMalhas *valmalhas;

 try
 { // cria objeto VTValMalhas
 if ((valmalhas = DLL_NewObjValMalhas(apl)) == NULL)
 return;
 // executa verificação de malhas
 valmalhas->Executa();
 // destrói onbjeto
 delete valmalhas;
 }
 catch (Exception &e)
 { // nada a fazer
 }
 }

 // ---------------------------------------------------------------------------
 void __fastcall TConversor::VerificaMalhasBT(void)
 {
 VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
 VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
 TList *lisREDEBT;
 VTRede *rede;

 lisREDEBT = new TList;
 lisEQPTOSALTERADOS->Clear();
 lisNOVASBARRAS->Clear();

 //filtra as redes para ter só bt
 redes->LisRedeCarregada(lisREDEBT);
 //remove as redes que pertencem à clusters
 //	RemoveRedesDeCluster(lisREDEBT);
 for(int nr = (lisREDEBT->Count - 1); nr >= 0; nr--)
 {
 rede = (VTRede*) lisREDEBT->Items[nr];
 if (rede->TipoRede->Segmento != redeSEC)
 {lisREDEBT->Remove(rede);}
 }
 //verifica as bts
 for (int nrb = 0 ; nrb < lisREDEBT->Count; nrb++)
 {
 rede = (VTRede*) lisREDEBT->Items[nrb];
 VerificaBarrasFronteira(rede);
 }
 //insere as novas barras
 edita->InsereLisEqpto(lisNOVASBARRAS);
 //altera as ligacoes para as novas barras
 edita->AlteraLisEqpto(lisEQPTOSALTERADOS);

 DestroiTObject(lisREDEBT);
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TConversor::VerificaPerdaFerro(void)
{
	// variáveis locais
	int num_eqpto;
	TList *lisEXT;
	VTEqpto *eqpto;
	VTValPerdafe *valperdafe;
	VTLog *plog = (VTLog*)apl->GetObject(__classid(VTLog));

	// verifica a opção
	if (!opcao->corrige_perda_fe)
		return;
	try
	{ // cria objeto VTValPerdafe
		valperdafe = DLL_NewObjValPerdafe(apl);
		valperdafe->Executa();
		if ((num_eqpto = valperdafe->ExisteTrafoComPerdaFeInvalida()) > 0)
		{ // atualiza campos do Log completo
			if (plog)
				plog->DefineValor("Bloco", "Validação");
			if (plog)
				plog->DefineValor("Linha", -1);
			if (plog)
				plog->DefineValor("Rede", "");
			// preenche log
			lisEXT = valperdafe->LisTrafoComPerdaFeInvalida();
			for (int n = 0; n < lisEXT->Count; n++)
			{
				eqpto = (VTEqpto*)lisEXT->Items[n];
				if (plog)
					plog->DefineValor("Eqpto", eqpto->TipoAsString());
				if (plog)
					plog->Add("Perda no ferro inválida");
				// altera código do eqpto p/ facilitar localização
				// eqpto->Codigo = eqpto->Codigo + "Perda ferro";
			}
			/*
			 if (plog)
			 {
			 Aviso("Existem " + IntToStr(num_eqpto) + " trafos com perda no ferro inválida\n" +
			 "Estes eqptos receberam o código 'PerdaFe'");
			 }
			 */
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói objeto criado
	if (valperdafe)
	{
		delete valperdafe;
		valperdafe = NULL;
	}
}

// ---------------------------------------------------------------------------
// eof

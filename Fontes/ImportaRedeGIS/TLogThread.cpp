// --------------------------------------------------------------------------------------------- bof
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <iostream>
#include <fstream>

// Plataforma include
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>

// Project include
#include <TLogThread.h>

// --------------------------------------------------------------------------------------- namespace
using namespace std;
//using namespace sinapsis::sinap::neoenergia;

// ------------------------------------------------------------------------------------------- class

// -------------------------------------------------------------------------------------------------
__fastcall TLogThread::TLogThread(VTApl *apl, AnsiString nome, AnsiString dir)
	// __fastcall TLogThread::TLogThread(VTApl *apl, AnsiString nome) : TThread(true)
{
	this->apl = apl;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString filename;
	char *file;
	bool sucesso = false;
	AnsiString last;

	// guarda o nome do log
	this->codigo = StringReplace(nome, "::", "_", TReplaceFlags() << rfReplaceAll);

	if (dir.IsEmpty())
	{
		filename = ExtractFilePath(path->ArqRede);
		filename = filename + "Logs\\" + StringReplace(nome, "::", "_",
			TReplaceFlags() << rfReplaceAll) + "\\";
	}
	else
	{
		if ((last = dir.AnsiLastChar()).AnsiCompare("\\") == 0)
		{
			filename = dir;
		}
		else
		{
			filename = dir + "\\";
		}
		filename = filename + "Logs\\" + StringReplace(nome, "::", "_",
			TReplaceFlags() << rfReplaceAll) + "\\";
		// filename = StringReplace(filename,"\\","\\\\",TReplaceFlags() << rfReplaceAll);
	}
	// caso nao exista o diretorio
	if (!DirectoryExists(filename))
	{
		if (ForceDirectories(filename))
		{
			sucesso = true;
		}
	}
	else
	{
		sucesso = true;
	}
	// caso nao tenha  criado a pasta
	if (!sucesso)
	{
		filename = path->DirTmp();
		filename = filename + "\\Logs\\";
		// caso nao exista o diretorio
		if (!DirectoryExists(filename))
		{
			if (!ForceDirectories(filename))
			{
				throw Exception("Cannot create " + filename);
			}
		}
	}
	name = nome;
	// abre arquivo
	filename += FormatDateTime("yyyy.mm.dd-hh.mm.ss.zzz", Now());
	filename = filename + "_Log_DEBUG-";
	filename = filename + StringReplace(name, "::", "_", TReplaceFlags() << rfReplaceAll) + ".log";
	file = filename.c_str();
	// pFile = fopen(filename, "w");
	nome_arq = file;
	// myfile.open (file, std::ofstream::out | std::ofstream::app);
	myfile = NULL;
	myfile = fopen(file, "a");
	// //configura opções
	// FreeOnTerminate = false;
	// Priority        = tpHighest;
	// //cria objeto Event para bloquear processamento
	// //event = new TEvent(NULL, false /*ManualReset*/, false /*InitialState*/, "", false /*UseCOMWait*/);
	// name = nome;

}

// -------------------------------------------------------------------------------------------------
__fastcall TLogThread::~TLogThread(void)
{
	// destrói objetos
	// if (event) {delete event; event = NULL;}
	// if (pFile)
	// {
	// fclose(pFile);
	// }
	// if (myfile != NULL)
	// {
	// myfile.close();
	// }
	// fecha arquivo
	if (myfile != NULL)
	{
		fclose(myfile);
		myfile = NULL;
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TLogThread::AddLog(AnsiString string, const char *nome_funcao, bool isErro, int ordinal)
{ // variaveislocais

	char *txt;
	char *filename;

	AnsiString data;
	AnsiString Separador = ";";
	AnsiString msg;
	AnsiString funcao = nome_funcao;

	// o formato ficará
	// data;funcao;tipoerro;string;
	data = FormatDateTime("yyyy.mm.dd-hh.mm.ss.zzz", Now()) + Separador;
	// data = data + FormatDateTime("tt", Now())+ Separador;
	msg = data;
	msg += IntToStr(ordinal) + Separador;
	if (isErro)
	{
		msg += ltstr_ERRO + Separador;
	}
	else
	{
		msg += ltstr_WARN + Separador;
	}
	msg += funcao + Separador;
	msg += string + Separador;
	txt = msg.c_str();

	if (myfile != NULL)
	{ // inclui 'new line' e grava mensagem
		msg += "\n";
		fputs(msg.c_str(), myfile);
		fflush(myfile);
	}
	// if(myfile.is_open())
	// {
	// //myfile << txt;
	// myfile << txt << endl;
	// //myfile.close();
	// }
	// else
	// {
	// filename = nome_arq.c_str();
	// myfile.open (filename, std::ofstream::out | std::ofstream::app);
	// myfile << txt << endl;
	// //myfile.close();
	// }
}

// -------------------------------------------------------------------------------------------------
AnsiString __fastcall TLogThread::PM_GetCodigo(void)
{
	return (codigo);
}

// -------------------------------------------------------------------------------------------------
void __fastcall TLogThread::PM_SetCodigo(AnsiString codigo)
{
	this->codigo = codigo;
}

// --------------------------------------------------------------------------------------------- eof
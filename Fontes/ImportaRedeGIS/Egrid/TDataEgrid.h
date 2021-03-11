//---------------------------------------------------------------------------
#ifndef TDataEgridH
#define TDataEgridH
// --------------------------------------------------------------------------
//#include <Classes.hpp>
//#include <System.Classes.hpp>
//#include <Vcl.Controls.hpp>
//#include <Vcl.StdCtrls.hpp>
//#include <Vcl.ComCtrls.hpp>
//#include <System.Zip.hpp>
#include <stdexcept>
#include <list>
#include <Classes.hpp>
#include <System.Classes.hpp>
#include <System.Zip.hpp>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
class VTApl;
class EDPDAO;
// ------------------------------------------------------------------------------------------- class
struct StrChvAdicionar : public TObject
{
   int idChave;
	AnsiString codigoChave;
	int id_barra1_egrid;
	int id_barra2_egrid;
	int id_rede1_egrid;
	int id_rede2_egrid;
	int id_barra1_mdb;
	int id_barra2_mdb;
	int id_rede1_mdb;
	int id_rede2_mdb;
	int telecomando;
	int estado;
	int estado_normal;
	double inom_a;
};
// ------------------------------------------------------------------------------------------- class
struct StrDadosChavesRede : public TObject
{
	AnsiString codSub;
	TStringList* lisIdsRedesMt;
	TStringList* lisCodChvExistMDB;
	TList* lisStrChvAdicionar;

	__fastcall StrDadosChavesRede()
	{
		lisCodChvExistMDB = new TStringList;
		lisStrChvAdicionar = new TList;
		lisIdsRedesMt = new TStringList;
	}
	__fastcall ~StrDadosChavesRede()
	{
		delete lisCodChvExistMDB;
		delete lisStrChvAdicionar;
		delete lisIdsRedesMt;
	}
};
// ------------------------------------------------------------------------------------------- class
struct StrDadosEgrid : public TObject
{
	AnsiString selecaoDados;
   AnsiString pathArquivoEgrid;
	AnsiString StrJsonResposta;
	TList* lisStrDadosChvRede;

	__fastcall StrDadosEgrid() {lisStrDadosChvRede = new TList;}
	__fastcall ~StrDadosEgrid() {delete lisStrDadosChvRede;}
};
// ------------------------------------------------------------------------------------------- class
class TDataEgrid : public TObject
{
public:
	__fastcall TDataEgrid(VTApl *apl);
	__fastcall ~TDataEgrid(void);

	void __fastcall ObtemDados(StrDadosEgrid* dadosEgrid);

	AnsiString __fastcall ObtemPathArquivoSub_es(AnsiString codSub);
	void __fastcall LeCommons_es(AnsiString &msgErro);
	void __fastcall LeChaves_es(AnsiString codSub, TStringList* lisCodChvExist, TList* lisEXT, AnsiString &msgErro);
	void __fastcall LeChavesSocorro_es(AnsiString pathArqEs, TStringList* lisCodChvExist, TList* lisEXT, AnsiString &msgErro);
	void __fastcall LeChavesRede_es(AnsiString pathArqEs, TStringList* lisCodChvExist, TList* lisEXT, AnsiString &msgErro);
	void __fastcall LeChavesFaltantes(StrDadosEgrid* dadosEgrid, AnsiString &msgErro);
	void __fastcall NomesArquivosES(TStringList *slisNomes);
   void __fastcall UnZip(StrDadosEgrid* dadosEgrid, AnsiString &msgErro);

	VTApl *apl;
	EDPDAO *dao;
	DynamicArray<int>vet_redepri_id;
	TZipFile *zip;
	AnsiString eGridPath;
	AnsiString diretorioTmp;
	TStringList *slisNomes;

	TList* listaStrRedes;
};
//---------------------------------------------------------------------------
#endif

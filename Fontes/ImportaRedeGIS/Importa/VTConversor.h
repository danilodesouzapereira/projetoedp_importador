//---------------------------------------------------------------------------
#ifndef VTConversorH
#define VTConversorH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
struct strOPImporta;
//---------------------------------------------------------------------------
enum destinoINDEX
{
	destINDEX_MEMORIA = 0,
	//destINDEX_GATEWAY,
	destINDEX_BASEUNICA,
	destINDEX_BASESEPARADA
};
//---------------------------------------------------------------------------
class VTConversor : public TObject
   {
   public:  //property
      __property AnsiString TNSname  = {read=PD.TNSname,  write=PD.TNSname};
      __property AnsiString Username = {read=PD.username, write=PD.username};
	  __property AnsiString Password = {read=PD.password, write=PD.password};
	  __property AnsiString NomeArquivoBaseUnica = {read = PD.nomeArquivoBaseUnica, write = PD.nomeArquivoBaseUnica};
	  __property AnsiString Pasta = {read = PD.pasta, write = PD.pasta};
	  __property bool BatchMode = {read = PD.batchMode, write = PD.batchMode, default = false};

   public:
                   __fastcall  VTConversor(void){};
	  virtual      __fastcall ~VTConversor(void){};
//	  virtual bool __fastcall  Conecta(void) = 0;
	  virtual bool __fastcall  ImportaDadosEmpresa(int empresa_id) = 0;
	  virtual bool __fastcall  ImportaDadosEmpresa(AnsiString fileName) = 0;
      virtual bool __fastcall  ImportaRedes(TList *lisSUBESTACAO,  strOPImporta *opcao, int destino) = 0;

   protected:
      struct{
            AnsiString TNSname;
            AnsiString username;
			AnsiString password;
			AnsiString nomeArquivoBaseUnica;
			AnsiString pasta;
			bool batchMode;
            }PD;
   };

//-----------------------------------------------------------------------------
// função global para criarobjeto VTConversor
//-----------------------------------------------------------------------------
VTConversor* NewObjConversor(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif

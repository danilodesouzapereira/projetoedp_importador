#ifndef VTDadosEmpresaH
#define VTDadosEmpresaH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTCondutor;
class VTLocalidade;
class VTRegional;
class VTSubestacao;
class VTPrimario;

//-----------------------------------------------------------------------------
enum enumEMPRESA_ID {ORACLE_MASTER=0, ORACLE_BANDEIRANTE_ID=1, ORACLE_ESCELSA_ID=2};

//-----------------------------------------------------------------------------
class VTDadosEmpresa : public TObject
   {
	public:	//property
		__property int         Id                = {read=empresa.id,          write=empresa.id};
		__property AnsiString  Nome              = {read=empresa.nome,        write=empresa.nome};
      //
		__property AnsiString  TNSname           = {read=oracle.tnsname,      write=oracle.tnsname};
		__property AnsiString  Username          = {read=oracle.username,     write=oracle.username};
		__property AnsiString  Password          = {read=oracle.password,     write=oracle.password};
      //
      __property VTCondutor* CondutorDefaultMT = {read=condutor_default.mt, write=condutor_default.mt};
      __property VTCondutor* CondutorDefaultBT = {read=condutor_default.bt, write=condutor_default.bt};
      __property VTCondutor* CondutorDefaultRL = {read=condutor_default.rl, write=condutor_default.rl};
      __property VTCondutor* CondutorDefaultIP = {read=condutor_default.ip, write=condutor_default.ip};
      __property bool        ArranjoMontante = {read=condutor_default.arranjo_montante, write=condutor_default.arranjo_montante};

	  __property TList*      LisSubSelecionada = {read=lisSUB_SEL, write=lisSUB_SEL};

	public:
							__fastcall  VTDadosEmpresa(void) {};
							__fastcall ~VTDadosEmpresa(void) {};
	  virtual bool          __fastcall  Clear(void) = 0;
	  virtual VTCondutor*   __fastcall  ExisteCondutor(int condutor_id) = 0;
	  virtual VTLocalidade* __fastcall  ExisteLocalidade(int localidade_id) = 0;
	  virtual VTRegional*   __fastcall  ExisteRegional(int regional_id) = 0;
	  virtual VTSubestacao* __fastcall  ExisteSubestacao(int subestacao_id) = 0;
      virtual VTSubestacao* __fastcall  ExisteSubestacao(AnsiString cod) = 0;
	  virtual bool          __fastcall  InsereCondutor(VTCondutor *condutor) = 0;
	  virtual bool          __fastcall  InsereLocalidade(VTLocalidade *localidade) = 0;
	  virtual bool          __fastcall  InsereRegional(VTRegional *regional) = 0;
	  virtual bool          __fastcall  InsereSubestacao(VTSubestacao *subestacao) = 0;
	  virtual bool          __fastcall  InserePrimario(VTPrimario *primario) = 0;
	  virtual TList*        __fastcall  LisCondutor(void) = 0;
	  virtual TList*        __fastcall  LisLocalidade(void) = 0;
	  virtual TList*        __fastcall  LisPrimario(void) = 0;
	  virtual TList*        __fastcall  LisRegional(void) = 0;
	  virtual TList*        __fastcall  LisSubestacao(void) = 0;

	protected:
		struct{
				int        id;
				AnsiString nome;
				}empresa;
		struct{
            AnsiString  tnsname;
            AnsiString  username;
            AnsiString  password;
				}oracle;
		struct{
			VTCondutor *mt;
			VTCondutor *bt;
			VTCondutor *rl;
			VTCondutor *ip;
            bool arranjo_montante;
			}condutor_default;
			TList *lisSUB_SEL;
	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTDadosEmpresa
//-----------------------------------------------------------------------------
VTDadosEmpresa* NewObjDadosEmpresa(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof

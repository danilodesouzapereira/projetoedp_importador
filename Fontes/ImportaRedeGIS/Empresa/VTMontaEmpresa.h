#ifndef VTMontaEmpresaH
#define VTMontaEmpresaH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\Fontes\Rede\Estrutura.h>
//-----------------------------------------------------------------------------
struct sdi_ARRANJO;

//-----------------------------------------------------------------------------
struct strLOCALIDADE	{
							int        id;
							int        regional_id;
							AnsiString nome;
							};
//-----------------------------------------------------------------------------
struct strREGIONAL	{
							int        id;
							int        empresa_id;
							AnsiString nome;
							};
//-----------------------------------------------------------------------------
struct strSUBESTACAO	{
							int        id;
							int        empresa_id;
							int        regional_id;
							int        localidade_id;
							int		   sub_mae_id;
							int		   rede_mt_pai_id;
							AnsiString codigo;
							AnsiString nome;
//							double     x_utm_m;
//							double     y_utm_m;
							};
//-----------------------------------------------------------------------------
struct strPRIMARIO	{
							int        id;
							int        subestacao_id;
							AnsiString codigo;
							AnsiString nome;
							int		   id_trafo;
							double	   vnom_kv;
							double     vop_kv;
							strIMP	   z0cc_pu;
                            strIMP	   z1cc_pu;
							};


//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTMontaEmpresa : public TObject
   {
	public:
						 __fastcall  VTMontaEmpresa(void) {};
						 __fastcall ~VTMontaEmpresa(void) {};
		virtual bool __fastcall  InsereCondutor(sdi_ARRANJO &str) = 0;
		virtual bool __fastcall  InsereLocalidade(strLOCALIDADE &str) = 0;
		virtual bool __fastcall  InserePrimario(strPRIMARIO &str) = 0;
		virtual bool __fastcall  InsereRegional(strREGIONAL &str) = 0;
		virtual bool __fastcall  InsereSubestacao(strSUBESTACAO &str) = 0;
	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTMontaEmpresa
//-----------------------------------------------------------------------------
VTMontaEmpresa* NewObjMontaEmpresa(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof

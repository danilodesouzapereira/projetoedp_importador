#ifndef ModeloRedeH
#define ModeloRedeH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "Estrutura.h"

//-----------------------------------------------------------------------------
struct sdi_ARRANJO     {
								int          id;
								AnsiString   codigo;
//                        		int          ativo;
								AnsiString   tipo;
								AnsiString   formacao;
								AnsiString   condutor_fase;
								AnsiString   condutor_neutro;
								double       iadm_a;
								double       r_ohm_km;
//								double       qcap_kvar_km;
								double       r0_ohm_km;
								double       x0_ohm_km;
								double       r1_ohm_km;
								double       x1_ohm_km;
								double       b0_s_km;
								double       b1_s_km;
//								double       custo_novo;
//								double       custo_rec;
								};
struct sdi_BARRA        {
								int          id;
								AnsiString   codigo;
								double       x_utm_m;
								double       y_utm_m;
								};
struct sdi_BARRA_BT     {
								int          id;
								AnsiString   codigo;
								double       x_utm_m;
								double       y_utm_m;
								int          redebt_id;
								};
struct sdi_BARRAMENTO   {
								int          id;
								AnsiString   codigo;
								double       vnom_kv;
								double       r_aterr_ohms;
								double       x_aterr_ohms;
								double       x_utm_m;
								double       y_utm_m;
								int          id_instalacao;
								};
/*
struct sdi_CABO         {
								int          id;
								AnsiString   extern_id;
								AnsiString   codigo;
								int          tipo;          //{0=MT, 1=BT}
								AnsiString   desc_cabo;
								double       iadm_a;
								double       grm_m;
								double       r_ohm_km;
								double       qcap_kvar_km;
								double       r0_ohm_km;
                        double       x0_ohm_km;
                        double       r1_ohm_km;
								double       x1_ohm_km;
								double       b0_s_km;
								double       b1_s_km;
								double       custo_novo;
								double       custo_rec;
								};
*/
struct sdi_CAPACITOR    {
								int          id;
								AnsiString   codigo;
								double       vnom_kv;
								double       qnon_kvar;
								int          barra_id;
								int          circ_id;
								};
struct sdi_CAPACITOR_BT {
								int          id;
								AnsiString   extern_id;
								AnsiString   codigo;
								int          ativo;        //{0=Não, 1=Sim}
								//int          tipo;
								int          tipo_lig;
								double       vnom_kv;
								double       qnon_kvar;
								int          barra_id;
								int          redebt_id;
								};
struct sdi_CHAVE        {
								int          id;
								AnsiString   codigo;
								int          ativo;         //{0=Não, 1=Sim}
								int          telecomando;
								int          estado;        //{0=aberto, 1=fechado}
								int          estado_normal; //{0=aberto, 1=fechado}
								double       inom_a;
								double       vnom_kv;
								AnsiString   tipo_elo;
								double       elo_a;
								int          id_rede_mt;
								int          id_barra1;
								int          id_barra2;
								};

struct sdi_CHAVE_RedeMT : public TObject
								{
								int          id;
								AnsiString   codigo;
								int          ativo;         //{0=Não, 1=Sim}
								int          telecomando;
								int          estado;        //{0=aberto, 1=fechado}
								int          estado_normal; //{0=aberto, 1=fechado}
								double       inom_a;
								int          id_rede_mt;
								int          id_barra1;
								int          id_barra2;
								};

struct sdi_CIRCUITO     {
								int          id;
								AnsiString   codigo;
								AnsiString   nome;
								double       vnom_kv;
								double       vop_kv;
								int          trafo_se_id;
								int          barra_mt_id;
								strIMP       z0_pu;
								strIMP       z1_pu;
								double       pcc3f;
								double       pcc1f;
								double       acc3f;
								double       acc1f;
								};

struct sdi_REDE_MT : public TObject
								{
								int          id;
								AnsiString   codigo;
								AnsiString   nome;
								};

struct sdi_CONS_BT      {
								int          id;
								AnsiString   codigo;
								int          ativo;
								int          classe_cons;
								AnsiString   nome;
								AnsiString   cod_ativ;
								double       consumo_kwh_mes;
								int          q_dias;
								AnsiString   fases;
								int          pcarga_id;
								int          redebt_id;
								};
struct sdi_CONS_IP      {
								int          id;
								AnsiString   codigo;
								int          n_lamp;
								double       lamp_dem_w;
								double       lamp_dem_var;
								AnsiString   fases;
								int          barra_id;
								};
struct sdi_CONS_MT      {
								int          id;
								AnsiString   codigo;
								int          ativo;           //{0=Não, 1=Sim}
								int          tipo_contrato;	//{0=convencional, 1: horosazonal
								int          tipo_fat;        //{0=MT, 1=BT, -1=Outro
								int          tipo_medicao;    //{0=MT, 1=BT, -1=Outro
								int          classe_cons;     //{1=res, 2=com, 3=ind, 4=rur, 5=out=4}
								AnsiString   nome;
								AnsiString   cod_ativ;
								double       snom_kva;
                                double       consumo_kwh_mes;
								double       p_ponta_kw;
								double       q_ponta_kvar;
								double       p_fponta_kw;
								double       q_fponta_kvar;
								AnsiString   fases;
								int          barra_id;
								};
struct sdi_GERADOR      {
								int          id;
								AnsiString   codigo;
								int          ativo;
								double       snom_kva;
								double       vnom_kv;
								double       r0_pu;
								double       x0_pu;
								double       r1_pu;
								double       x1_pu;
								int          barra_id1;
								int          barra_id2;
								int          circ_id;
								AnsiString   cdc;
								AnsiString   tipo_geracao;
								};
struct sdi_GERADOR_BT   {
								int          id;
								//AnsiString   extern_id;
								AnsiString   codigo;
								int          ativo;
								//int          tipo;
								//int          tipo_lig;
								//int          tipo_sup;     //{0=PQ, 1=PV}
								double       snom_kva;
								double       vnom_kv;
								double       r0_pu;
								double       x0_pu;
								double       r1_pu;
								double       x1_pu;
								int          barra_id;
								int          circ_id;
								};
struct sdi_INSTAL_TRAFO {
								int          id;
								AnsiString   codigo;
								int          circ_id;
								int          id_rede_bt;
								int          barra_mt_id;
								int          barra_bt_id;
								int          ativo;
							//	int          tipo;
								int          tipo_lig;         //{1=monofasica, 2=bifasica, 3=trifasica,
																		 // 4=delta aberto,5=delta fechado, -1=indefinido}
								AnsiString   tipo_local;       //{0=indefinido, 1=urbano, 2=rural}
								int          propriedade;      //{0=empresa, 1=particular}
								double       snom_kva;
								double       valta_kv;
								double       vbaixa_v;
//								double       tap_v;
								AnsiString   fases_mt;
								AnsiString   fases_bt;
//								double       r0_pu;
//								double       x0_pu;
//								double       r1_pu;
//								double       x1_pu;
//								double       perda_fe_w;
//								double       perda_cu_w;
//								int          et_id;
                        };
struct sdi_LINK         {
								int          link_id;
								int          trecho_id;
								int          barra_id1;
								int          barra_id2;
								};
struct sdi_LINK_BT      {
                                int          link_id;
								int          trecho_id;
								int          barra_id1;
								int          barra_id2;
								};
struct sdi_PCARGA_BT    {
								int          id;
								AnsiString   extern_id;
								AnsiString   codigo;
								int          ativo;
								int          barra_id;
								int          redebt_id;
								};
struct sdi_PTOSERVICO_MT{
								int          id;
								AnsiString   cod;
								int          ativado;
								int          coord_x_cm;
                        int          coord_y_cm;
                        int          id_trafo;
                        AnsiString   cod_se;
                        AnsiString   cod_circuito;
                        };
struct sdi_REGULADOR    {
								int          id;
								AnsiString   codigo;
								int          ativo;    //{0=Não, 1=Sim}
								int          tipo_lig;
								int          bypass;
								int          inversao;
								double       tap_pu;
								double       faixa_pu;
								int       n_passos;
								double       snom_kva;
								double       vnom_kv;
								double       inom_a;
								int          id_barra1;
								int          id_barra2;
								int          id_rede_mt;
								};
struct sdi_SE           {
								int          id;
								AnsiString   codigo;
								int          ativo;    //{0=Não, 1=Sim}
								AnsiString   nome;
								int          empresa_id;
								int          regional_id;
								int          localidade_id;
								int          semae_id;
								int          circpai_id;
								float        x_utm_m;
								float        y_utm_m;
								};
struct sdi_SOCORRO      {
								int          id;
								AnsiString   codigo;
								int          ativo;         //{0=Não, 1=Sim}
								int          telecomando;
								double       inom_a;
								int          se1_id;
								int          se2_id;
								int          circ1_id;
								int          circ2_id;
								int          barra1_id;
								int          barra2_id;
								};
struct sdi_SUPORTE      {
								int          id;
								AnsiString   extern_id;
								AnsiString   codigo;
								AnsiString   subestacao_codigo;
								int          tipo;          //{0=MT, 1=BT}
								AnsiString   desc_arranjo;
								double       fase1_x_m;
								double       fase1_y_m;
								double       fase2_x_m;
								double       fase2_y_m;
								double       fase3_x_m;
								double       fase3_y_m;
								double       fase4_x_m;
								double       fase4_y_m;
								};

struct sdi_SUPRIMENTO      {
								int          id;
								int          id_barra_at;
								AnsiString   codigo;
								double       r1_pu;
								double       x1_pu;
								double       r0_pu;
								double       x0_pu;
								double       vnom_kv;

};

struct sdi_TRAFO_SE     {
								int          id;
								int          extern_id;
								AnsiString   codigo;
								int          ativo;    //{0=Não, 1=Sim}
								int          tipo_lig;
								double       snom_mva;
								double       snom_vent_mva;
								double       valta_kv;
								double       vbaixa_kv;
								double       vbaixa2_kv;
								double       vbaixa_op_kv;
								double       vbaixa2_op_kv;
								double       r0_pu;
								double       x0_pu;
								double       r1_pu;
								double       x1_pu;
								double       z12;
								double       z23;
								double       z13;
								double       xr12;
								double       xr23;
								double       xr13;
								double       z012;
								double       z023;
								double       z013;
								double       xr012;
								double       xr023;
								double       xr013;
								double       perda_fe_kw;
								double       perda_cu_kw;
								int          se_id;
								int          barra_mt1;
								int          barra_mt2;
								int          barra_at;
								};
struct sdi_TRAFO_LINHA  {
								int          id;
								AnsiString   extern_id;
								AnsiString   codigo;
								int          ativo;    //{0=Não, 1=Sim}
								int          tipo;     //2 ou 3 enrolamentos
								int          tipo_lig;
								double       snom_kva;
								double       valta_kv;
								double       vbaixa_kv;
								double       tap_alta_pu;
								double       tap_baixa_pu;
								double       r0_pu;
								double       x0_pu;
								double       r1_pu;
								double       x1_pu;
								double       perda_fe_kw;
								double       perda_cu_kw;
								int          trecho_id;
								int          circ_id;
								};
struct sdi_TRECHO       {
								int          id;
								AnsiString   codigo;
								int          ativo;    //{0=Não, 1=Sim}
								int          tipo_local;		//{0=indefinido, 1=urbano, 2=rural}
								int          propriedade;
								int          mult;
								AnsiString   fases;
								double       comp_m;
								int          arranjo_id;
								int          cabo_id;
								int          barra1_id;
								int          barra2_id;
								int          circ_id;
								};
struct sdi_TRECHO_BT       {
								int          id;
								AnsiString   codigo;
								int          ativo;    //{0=Não, 1=Sim}
								int          tipo;
								AnsiString   fases;
								double       comp_m;
								int          arranjo_id;
								int          cabo_fase_id;
								int          cabo_neutro_id;
								int          barra1_id;
								int          barra2_id;
								int          redebt_id;
								};
struct sdi_UTC_BT       {
								int          id;
								AnsiString   extern_id;
								AnsiString   codigo;
								int          ativo;
								int          tipo;
								//int          tipo_lig;
								double       snom_kva;
								double       valta_kv;
								double       vbaixa_v;
								double       tap_v;
								AnsiString   fases_mt;
								AnsiString   fases_bt;
								double       r0_pu;
								double       x0_pu;
								double       r1_pu;
								double       x1_pu;
								double       perda_fe_w;
								double       perda_cu_w;
								int          et_id;
								};
//struct sdi_ZCC       {
//								int          id_circuito;
//								double		 pcc_3f; 		//futuro
//								double		 tensao_em_imax;//futuro
//								double		 fp_em_imax;    //futuro
//								double		 acc_3f;        //futuro
//								double		 imax_medida;   //futuro
//								double		 r0_pu;
//								double		 r1_pu;
//								double		 x0_pu;
//								double		 x1_pu;
//								double		 pcc_1f;       //futuro
//								double	     demanda_max;  //futuro
//								double		 acc_1f;       //futuro
//								};
#endif
//-----------------------------------------------------------------------------
// eof

// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include "TMontaEmpresa.h"
#include "VTDadosEmpresa.h"
#include "VTCondutor.h"
#include "VTLocalidade.h"
#include "VTPrimario.h"
#include "VTRegional.h"
#include "VTSubestacao.h"
#include "PreMonta\ModeloRede.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// -----------------------------------------------------------------------------
VTMontaEmpresa* NewObjMontaEmpresa(VTApl *apl)
{
	return (new TMontaEmpresa(apl));
}

// -----------------------------------------------------------------------------
__fastcall TMontaEmpresa::TMontaEmpresa(VTApl *apl)
{
	// salva ponteiro para objetos
	this->apl = apl;
	empresa = (VTDadosEmpresa*)apl->GetObject(__classid(VTDadosEmpresa));
}

// -----------------------------------------------------------------------------
__fastcall TMontaEmpresa::~TMontaEmpresa(void)
{
	// nada a fazer
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaEmpresa::InsereCondutor(sdi_ARRANJO &str)
{
	// variáveis locais
	VTCondutor *condutor;

	// cria uma nova Localidade
	condutor = NewObjCondutor();
	condutor->id = str.id;
	condutor->codigo = MontaCodigoCondutor(str);
	condutor->tipo = str.tipo;
	condutor->formacao = str.formacao;
	condutor->condutor_fase = str.condutor_fase;
	condutor->condutor_neutro = str.condutor_neutro;
	condutor->iadm_a = str.iadm_a;
	condutor->r_ohm_km = str.r_ohm_km;
	condutor->r0_ohm_km = str.r0_ohm_km;
	condutor->x0_ohm_km = str.x0_ohm_km;
	condutor->r1_ohm_km = str.r1_ohm_km;
	condutor->x1_ohm_km = str.x1_ohm_km;
	condutor->b0_s_km = str.b0_s_km;
	condutor->b1_s_km = str.b1_s_km;

	// insere Condutor em Empresa
	empresa->InsereCondutor(condutor);
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaEmpresa::InsereLocalidade(strLOCALIDADE &str)
{
	// variáveis locais
	VTLocalidade *localidade;

	// cria uma nova Localidade
	localidade = NewObjLocalidade();
	localidade->Id = str.id;
	localidade->Nome = str.nome;
	localidade->Regional = empresa->ExisteRegional(str.regional_id);
	// insere Localidade em Empresa
	empresa->InsereLocalidade(localidade);
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaEmpresa::InserePrimario(strPRIMARIO &str)
{
	// variáveis locais
	VTPrimario *primario;
	VTSubestacao *subestacao;

	//verifica se existe a subestação
	subestacao = empresa->ExisteSubestacao(str.subestacao_id);
	if(subestacao != NULL)
	{//verifica se existe o primario
		if(subestacao->ExistePrimario(str.codigo))
			return (true);
    }
	// cria uma novo Primario
	primario = NewObjPrimario();
	primario->Id = str.id;
	primario->Codigo = str.codigo;
	primario->Nome = str.nome;
	primario->Subestacao = subestacao;
	if (primario->Subestacao != NULL)
	{
		primario->Regional = primario->Subestacao->Regional;
		primario->Localidade = primario->Subestacao->Localidade;
		// insere Primario em Empresa
		empresa->InserePrimario(primario);
		// insere tambem em subestacao
		primario->Subestacao->LisPrimario->Add(primario);
		return (true);
	}
	return (false);
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaEmpresa::InsereRegional(strREGIONAL &str)
{
	// variáveis locais
	VTRegional *regional;

	// cria uma nova Regional
	regional = NewObjRegional();
	regional->Id = str.id;
	regional->Nome = str.nome;
	// insere Regional em Empresa
	empresa->InsereRegional(regional);
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaEmpresa::InsereSubestacao(strSUBESTACAO &str)
{
	// variáveis locais
	VTSubestacao *subestacao;

	// cria uma nova Subestacao
	subestacao = NewObjSubestacao();
	subestacao->Id = str.id;
	subestacao->Codigo = str.codigo;
	subestacao->Nome = str.nome;
	subestacao->Localidade = empresa->ExisteLocalidade(str.localidade_id);
	subestacao->Regional = subestacao->Localidade->Regional;
	// insere Subestacao em Empresa
	empresa->InsereSubestacao(subestacao);
	return (true);
}

// -----------------------------------------------------------------------------
AnsiString __fastcall TMontaEmpresa::MontaCodigoCondutor(sdi_ARRANJO &str)
{
	AnsiString codigo = "";

	codigo = TraduzTipo(str.tipo);
	if (!str.condutor_fase.IsEmpty())
	{
		codigo += "[" + str.condutor_fase + "]";
	}
	if (!str.condutor_neutro.IsEmpty())
	{
		codigo += "[" + str.condutor_neutro + "]";
	}

	return codigo;
}

// -----------------------------------------------------------------------------
AnsiString __fastcall TMontaEmpresa::TraduzTipo(AnsiString tipo)
{
	AnsiString tipoAbreviado = "";
	AnsiString tipoExtenso = "";

	tipoExtenso = tipo.UpperCase();

	if (tipoExtenso.Pos("HORIZONTAL") > 0)
	{
		tipoAbreviado = "[AH]";
	}
	else if (tipoExtenso.Pos("VERTICAL") > 0)
	{
		tipoAbreviado = "[AV]";
	}
	else if (tipoExtenso.Pos("AÉREO") > 0)
	{
		tipoAbreviado = "[A]";
	}
	else if (tipoExtenso.Pos("SUBTERRÂNEO") > 0)
	{
		tipoAbreviado = "[S]";
	}
	else
	{
		tipoAbreviado = "[-]";
	}
	return tipoAbreviado;
}
// -----------------------------------------------------------------------------
// eof

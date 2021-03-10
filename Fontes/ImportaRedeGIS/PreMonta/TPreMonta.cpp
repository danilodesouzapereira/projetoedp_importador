// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
// #include <complexo.h>
#include <Math.hpp>
#include <DateUtils.hpp>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Arranjo\VTArranjo.h>
#include <PlataformaSinap\Fontes\Arranjo\VTArranjos.h>
#include <PlataformaSinap\Fontes\Arranjo\VTCabo.h>
#include <PlataformaSinap\Fontes\Arranjo\VTSuporte.h>
#include <PlataformaSinap\Fontes\Distancia\VTCaminho.h>
#include <PlataformaSinap\Fontes\Classes\VTClasse.h>
#include <PlataformaSinap\Fontes\Classes\VTClasses.h>
#include <PlataformaSinap\Fontes\Complexo\Complexo.h>
#include <PlataformaSinap\Fontes\Constante\Fases.h>
#include <PlataformaSinap\Fontes\Curvas\VTCurva.h>
#include <PlataformaSinap\Fontes\Curvas\VTCurvas.h>
#include <PlataformaSinap\Fontes\Importa\Monta\VTMonta.h>
#include <PlataformaSinap\Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>
// #include <PlataformaSinap\Fontes\InfoZcc\VTInfoZccManager.h>
// #include <PlataformaSinap\Fontes\InfoZcc\VTCalculaZcc.h>
#include <PlataformaSinap\Fontes\Log\VTLog.h>
#include <PlataformaSinap\Fontes\Ordena\VTOrdena.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamar.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamares.h>
#include <PlataformaSinap\Fontes\Potencia\VTDemanda.h>
#include <PlataformaSinap\Fontes\Radial\VTRadial.h>
#include <PlataformaSinap\Fontes\Radial\VTPrimario.h>
#include <PlataformaSinap\Fontes\Radial\VTSubestacao.h>
#include <PlataformaSinap\Fontes\Rede\Estrutura.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTDominio.h>
#include <PlataformaSinap\Fontes\Rede\VTFases.h>
#include <PlataformaSinap\Fontes\Rede\VTGerador.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTNet.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTRegulador.h>
#include <PlataformaSinap\Fontes\Rede\VTSuprimento.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoRede.h>
#include <PlataformaSinap\Fontes\Rede\VTTipos.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo3E.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Util\TInsereTrafoZZ.h>

#include <PlataformaSinap\DLL_Inc\Distancia.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// #include <PlataformaSinap\DLL_Inc\InfoZcc.h>
#include <PlataformaSinap\DLL_Inc\Ordena.h>
#include <PlataformaSinap\DLL_Inc\Radial.h>
#include <PlataformaSinap\DLL_Inc\Rede.h>

#include "TPreMonta.h"
#include "..\Empresa\VTCondutor.h"
#include "..\Empresa\VTDadosEmpresa.h"
#include "..\Eqpto\TCircuito.h"
#include "..\Eqpto\TEt.h"
#include "..\Eqpto\TSe.h"
#include "..\Eqpto\TSocorro.h"
#include "..\Eqpto\TTrafose.h"
#include "..\Eqpto\TUtc.h"

#include <TLogThread.h>
#include <TLogManager.h>

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18497403
__fastcall TPreMonta::TPreMonta(VTApl *apl)
{
    // salva ponteiros p/ objetos
    this->apl = apl;
    empresa = (VTDadosEmpresa*)apl->GetObject(__classid(VTDadosEmpresa));
    arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
    classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
    fases = (VTFases*)apl->GetObject(__classid(VTFases));
    plog = (VTLog*)apl->GetObject(__classid(VTLog));
    monta = (VTMonta*)apl->GetObject(__classid(VTMonta));
    patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
    tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
    // cria listas
    lisEQP = new TList();
    lisSE = new TList();
    lisSocorro = new TList();
    lisTRECHO_ALT = new TList();
    // inicia horários dos patamares da EDP
    for (int ind_pat = 0; ind_pat < pat01HORA; ind_pat++)
    {
        pat_edp[ind_pat].ponta = (ind_pat >= INIPONTA) && (ind_pat <= FIMPONTA);
        pat_edp[ind_pat].hm_ini.hora = 0;
        pat_edp[ind_pat].hm_ini.minuto = 0;
        pat_edp[ind_pat].hm_fim.hora = ind_pat + 1;
        pat_edp[ind_pat].hm_fim.minuto = 0;
    }
    // inicia data para Domínio
    DataDominio = Now();
    MesExtracao = int(MonthOf(DataDominio));
    // cria arranjos default
    CriaArranjoTipicoMT();
    CriaArranjoTipicoBT();

    function = __FUNC__;
    log_manager = (TLogManager*)apl->GetObject(__classid(TLogManager));
    // if(log_manager == NULL){
    // apl->Add(log_manager = new TLogManager(apl));
    // }
    if (log_manager)
    {
        log_thread = log_manager->NewLogThread(function);
    }
}

// ---------------------------------------------------------------------------
__fastcall TPreMonta::~TPreMonta(void)
{
    // destrói lista sem destruir seus objetos
    if (lisEQP)
    {
        delete lisEQP;
        lisEQP = NULL;
    }
    if (lisSocorro)
    {
        delete lisSocorro;
        lisSocorro = NULL;
    }
    if (lisTRECHO_ALT)
    {
        delete lisTRECHO_ALT;
        lisTRECHO_ALT = NULL;
    }
    // destrói listas e seus objetos
    if (lisSE)
    {
        LimpaTList(lisSE);
        delete lisSE;
        lisSE = NULL;
    }
}

// ---------------------------------------------------------------------------
VTArranjo* __fastcall TPreMonta::ArranjoBTcerto(int fases_tag)
{
    // variáveis locais
    AnsiString codigo;
    VTArranjo *arranjo;

    // o Arranjo BT tem fases diferentes da indicada:
    // verifica se existe um Arranjo cujo código é igual ao do ArranjoBT acrescido com string das fases
    codigo = arranjoBT->Codigo + "_" + fases->AsStrABCN[fases_tag];
    if ((arranjo = arranjos->ExisteArranjo(codigo)) == NULL)
    { // cria uma cópia do Arranjo original e redefine seu código e fases
        if ((arranjo = arranjoBT->Clone()) != NULL)
        {
            arranjo->Id = FalsoId();
            arranjo->Codigo = codigo;
            arranjo->Fases = fases_tag;
            arranjo->Status[sttNOVO] = true;
            arranjo->ramal = false;
            // insere o novo Arranjo em Arranjos
            arranjos->InsereArranjo(arranjo);
        }
    }

    return (arranjo);
}

// ---------------------------------------------------------------------------
VTArranjo* __fastcall TPreMonta::ArranjoMTcerto(int fases_tag)
{
    // variáveis locais
    AnsiString codigo;
    VTArranjo *arranjo;

    // o Arranjo MT tem fases diferentes da indicada:
    // verifica se existe um Arranjo cujo código é igual ao do ArranjoMT acrescido com string das fases
    codigo = arranjoMT->Codigo + "_" + fases->AsStrABCN[fases_tag];
    if ((arranjo = arranjos->ExisteArranjo(codigo)) == NULL)
    { // cria uma cópia do Arranjo original e redefine seu código e fases
        if ((arranjo = arranjoMT->Clone()) != NULL)
        {
            arranjo->Id = FalsoId();
            arranjo->Codigo = codigo;
            arranjo->Fases = fases_tag;
            arranjo->Status[sttNOVO] = true;
            arranjo->ramal = false;
            // insere o novo Arranjo em Arranjos
            arranjos->InsereArranjo(arranjo);
        }
    }

    return (arranjo);
}

// -----------------------------------------------------------------------------
void __fastcall TPreMonta::AtualizaCargaEquivalente(TEt *et, VTCarga *carga)
{
    // variáveis locais
    complex<double>s_mva;
    VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

    try
    { // incorpora demanda da Carga na demanda da Carga equivalente
        for (int np = 0; np < patamares->NumPatamar(); np++)
        { // determina demanda da Carga equivalente da Et no patamar
            s_mva = demanda->S_mva[et->carga][np];
            // soma demanda da Carga no patamar
            s_mva += demanda->S_mva[carga][np];
            // atualiza demanda da Carga equivalente da Et no patamar
            demanda->S_mva[et->carga][np] = s_mva;
        }
        // incrementa quantidade de consumidores da classe (DVK 2013.08.21)
        switch (carga->classe->Tag)
        {
        case consRESIDENCIAL:
            et->carga->NumConsResidencial++;
            break;
        case consCOMERCIAL:
            et->carga->NumConsComercial++;
            break;
        case consINDUSTRIAL:
            et->carga->NumConsIndustrial++;
            break;
        case consRURAL:
            et->carga->NumConsRural++;
            break;
        case consOUTROS:
            et->carga->NumConsOutros++;
            break;
        case consA4:
            et->carga->NumConsA4++;
            break;
        case consIP: // DVK 2014.10.06 não conta consIP
            break;
        default:
            et->carga->NumConsOutros++;
        }
        // altera a definição de Classe da Carga equivalente   //DVK 2014.05.27
        if (et->carga->NumConsTotal == 1)
        { // assume mesma classe da Carga parcial
            et->carga->DefineClasse(carga->classe);
        }
        else
        { // assume Equivalente
            et->carga->DefineClasse(classes->ExisteClasseTag(consEQUIVALENTE));
        }
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add("Erro ao atualizar carga equivalente");
    }
}

/*
 // ---------------------------------------------------------------------------
 VTBarra* __fastcall TPreMonta::BarraSolta(VTChave *chave)
 {
 VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
 VTRede *rede;
 VTBarra *barra1, *barra2;
 TList *lisAuxiliar = new TList;
 VTBarra *barraEscolhida = NULL;

 __try
 {
 try
 {
 if (chave == NULL)
 {
 throw new Exception("Chave Invalida");
 }
 if ((rede = chave->rede) == NULL)
 {
 throw new Exception("Rede Invalida");
 }
 barra1 = chave->pbarra1;
 barra2 = chave->pbarra2;
 if ((barra1 == NULL) || (barra2 == NULL))
 {
 throw new Exception("Barra Invalida");
 }
 // verifica cada barra da chave
 redes->LisLigacao(lisAuxiliar, barra1);
 if (lisAuxiliar->Count == 1)
 {
 barraEscolhida = barra1;
 }
 else
 {
 lisAuxiliar->Clear();
 redes->LisLigacao(lisAuxiliar, barra2);
 if (lisAuxiliar->Count == 1)
 {
 barraEscolhida = barra2;
 }
 }
 }
 catch (Exception &e)
 {
 barraEscolhida = NULL;
 }
 }
 __finally
 {
 DestroiTObject(lisAuxiliar);
 }
 return barraEscolhida;
 }
 */
// ---------------------------------------------------------------------------
VTBarra* __fastcall TPreMonta::BarraSolta(VTChave *chave)
{
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    VTRede *rede;
    VTBarra *barra1, *barra2;
    TList *lisAuxiliar;
    VTBarra *barraEscolhida = NULL;

    try
    { // proteção
        if (chave == NULL)
        {
            return (NULL);
        }
        if ((rede = chave->rede) == NULL)
        {
            return (NULL);
        }
        barra1 = chave->pbarra1;
        barra2 = chave->pbarra2;
        if ((barra1 == NULL) || (barra2 == NULL))
        {
            return (NULL);
        }
        // verifica cada barra da chave
        lisAuxiliar = new TList;
        redes->LisLigacao(lisAuxiliar, barra1);
        if (lisAuxiliar->Count == 1)
        {
            barraEscolhida = barra1;
        }
        else
        {
            lisAuxiliar->Clear();
            redes->LisLigacao(lisAuxiliar, barra2);
            if (lisAuxiliar->Count == 1)
            {
                barraEscolhida = barra2;
            }
        }
        DestroiTObject(lisAuxiliar);
        return barraEscolhida;
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add("Erro ao localizar barra de chave de socorro");
        DestroiTObject(lisAuxiliar);
        barraEscolhida = NULL;
    }
    return barraEscolhida;
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::CalculaImpedancia(double z_pu, double fatorX_R, strIMP &imp_pu)
{
    // variáveis locais
    double r_pu, x_pu;
    double k = fatorX_R;

    // (z*z) = (r*r) + (x*x)
    // k = fatorX_R = x/r
    // x = k*r
    // (z* z) = (r*r) + ((k*r)* (k*r))
    // (z*z) = (1+(k*k)) * (r*r);
    // r*r   = (z*z)/(1+(k*k))
    // r     = z / sqrt((1+(k*k))

    try
    { // proteção contra valores nulos
        if (IsDoubleZero(z_pu) || IsDoubleZero(fatorX_R))
        { // assume valores default
            r_pu = 0.001;
            x_pu = 0.03;
        }
        else
        {
            r_pu = z_pu / sqrt(1. + (k * k));
            x_pu = sqrt((z_pu * z_pu) - (r_pu * r_pu));
        }
        imp_pu.r = r_pu;
        imp_pu.x = x_pu;
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add("Erro ao calcular impedância de trafo com 3 enrolamentos");
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TPreMonta::CompletaSuprimentoCircuito(sdi_SUPRIMENTO &sdi)
{
    // variáveis locais
    VTSuprimento *suprimento;
    TCircuito *circ;
    complex<double>zeq, zsup, zcir;

    try
    { // proteção
        if (se == NULL)
            return;
        // verifica se existe um suprimento associado ao circuito
        suprimento = se->ExisteSuprimento(sdi.id_barra_at);
        if (suprimento == NULL)
            return;
        suprimento->Codigo = sdi.codigo;
        // verifica se todas as impedancias estão zeradas
        // if (IsDoubleZero(sdi.r0_pu) && IsDoubleZero(sdi.x0_pu))
        // return;
        if (IsDoubleZero(sdi.r1_pu) && IsDoubleZero(sdi.x1_pu))
            return;
        // verifica se a impedância Z0 do suprimento está zerada
        zsup = complex<double>(suprimento->zeq0.r, suprimento->zeq0.x);
        zcir = complex<double>(1.0, 1.0);
        if (IsComplexZero(zsup))
        { // temporariamente assume mesmo valor
            suprimento->zeq0.r = float(zcir.real());
            suprimento->zeq0.x = float(zcir.imag());
        }
        else
        {
            if (IsDoubleZero(suprimento->zeq0.r) || (suprimento->zeq0.r < 0.))
                suprimento->zeq0.r = fabs(1.e-5);
        }
        // verifica se a impedância Z1 do suprimento está zerada
        zsup = complex<double>(suprimento->zeq1.r, suprimento->zeq1.x);
        zcir = complex<double>(sdi.r1_pu, sdi.x1_pu);
        if (IsComplexZero(zsup))
        { // temporariamente assume mesmo valor
            suprimento->zeq1.r = float(zcir.real());
            suprimento->zeq1.x = float(zcir.imag());
        }
        else
        {
            // proteção para valores negativos ou "zero negativo"
            if (IsDoubleZero(suprimento->zeq1.r) || (suprimento->zeq1.r < 0.))
                suprimento->zeq1.r = fabs(1.e-5);
        }
        // converte valores para potência de curto
        suprimento->pcc_3f.p = suprimento->pcc_3f.q = 0.;
        suprimento->pcc_ft.p = suprimento->pcc_ft.q = 0.;
        ConverteZeqvToPotCurto(suprimento);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add("Erro ao completar dados de impedância ou potência de curto no suprimento");
    }
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::ConectaSocorro(void)
{
    TSe *se1, *se2;
    TCircuito *circ1, *circ2;
    VTBarra *barra1, *barra2;
    VTChave *chave, *chaveAux;
    VTRede *rede1, *rede2;
    TList *lisAuxiliar;
    bool sucesso = false;
    TSocorro *socorro;
    VTBarra *barraRef, *barraProx, *outraBarra;
    VTRede *redeChave, *redeConexao;
    double multiplicador;
    TList *lisBarrasIndesej;

    __try
    {
        try
        {
            lisAuxiliar = new TList;
            lisBarrasIndesej = new TList;
            // percorre a lista de socorros
            for (int ns = 0; ns < lisSocorro->Count; ns++)
            {
                se1 = se2 = NULL;
                circ1 = circ2 = NULL;
                barra1 = barra2 = NULL;
                chave = chaveAux = NULL;
                rede1 = rede2 = NULL;
                barraRef = barraProx = NULL;
                redeChave = redeConexao = NULL;
                socorro = (TSocorro*) lisSocorro->Items[ns];
                // se socorro inativo
                if (socorro->ativo == false)
                    continue;
                // acha as SEs
                se1 = ExisteSubestacao(socorro->se1_id);
                se2 = ExisteSubestacao(socorro->se2_id);
                // caso nao existam
                if ((se1 == NULL) || (se2 == NULL))
                    continue;
                // verifica os circuitos envolvidos
                circ1 = se1->ExisteCircuito(socorro->circuito1_id);
                circ2 = se2->ExisteCircuito(socorro->circuito2_id);
                // caso existam
                if ((circ1 == NULL) || (circ2 == NULL))
                    continue;
                rede1 = circ1->rede;
                rede2 = circ2->rede;
                lisAuxiliar->Clear();
                // encontra a chave
                rede1->LisLigacao(lisAuxiliar, eqptoCHAVE);
                // percorre a lista
                for (int nc = 0; nc < lisAuxiliar->Count; nc++)
                {
                    chaveAux = (VTChave*)lisAuxiliar->Items[nc];
                    if (chaveAux->Id == socorro->chave_id)
                    {
                        chave = chaveAux;
                        break;
                    }
                }
                // se nao achar na primeira, procura na 2a
                if (chave == NULL)
                {
                    lisAuxiliar->Clear();
                    // encontra a chave
                    rede2->LisLigacao(lisAuxiliar, eqptoCHAVE);
                    // percorre a lista
                    for (int nc = 0; nc < lisAuxiliar->Count; nc++)
                    {
                        chaveAux = (VTChave*)lisAuxiliar->Items[nc];
                        if (chaveAux->Id == socorro->chave_id)
                        {
                            chave = chaveAux;
                            break;
                        }
                    }
                }
                // se nao encontrar...
                if (chave == NULL)
                    continue;
                lisBarrasIndesej->Clear();
                // verifica qual das barras da chave está solta
                barraRef = BarraSolta(chave);
                // se nao encontrar...
                if (barraRef == NULL)
                    continue;
                if (barraRef == chave->pbarra1)
                    outraBarra = chave->pbarra2;
                else
                    outraBarra = chave->pbarra1;
                lisBarrasIndesej->Add(barraRef);
                lisBarrasIndesej->Add(outraBarra);
                // verifica qual eh a rede da conexao
                redeChave = chave->rede;
                if (rede1 == redeChave)
                    redeConexao = rede2;
                else
                    redeConexao = rede1;
                // encontra a barra proxima
                // 0.0056506043007377
                for (int retry = 0; retry < 3; retry++)
                {
                    barraProx = NULL;
                    // por quantas vezes o range sera multiplicado
                    multiplicador = pow((double)10, retry);
                    barraProx = ExisteBarra(barraRef, lisBarrasIndesej, redeConexao, redeChave,
                        multiplicador * 1);
                    if (barraProx != NULL)
                        break;
                }
                // se nao encontrar...
                if (barraProx == NULL)
                    continue;
                chave->DefineObjBarra(outraBarra, barraProx);
            }
        }
        catch (Exception &e)
        {
            if (plog)
                plog->Add(ERRO_DE_SISTEMA);
            sucesso = false;
        }
    }
    __finally
    {
        DestroiTObject(lisAuxiliar);
        DestroiTObject(lisBarrasIndesej);
    }
    return (sucesso);
}

// ---------------------------------------------------------------------------
int __fastcall TPreMonta::ConverteClasseConsumidor(int classe)
{
    // retorna código da classe
    switch (classe)
    {
    case 1:
        return (consRESIDENCIAL);
    case 2:
        return (consCOMERCIAL);
    case 3:
        return (consINDUSTRIAL);
    case 4:
        return (consRURAL);
    case 5:
        return (consOUTROS);
    default:
        // return (consRESIDENCIAL);
        return (consOUTROS);
    }
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::ConverteTelecomando(int telecomando)
{
    switch (telecomando)
    { // 0=não tem; 1=tem
    case 1:
        return (true);
    default:
        return (false);
    }
}

/*
 // -----------------------------------------------------------------------------
 int __fastcall TPreMonta::ConverteTipoChave(AnsiString cod_chave)
 {
 // variáveis locais
 int length = cod_chave.Length();
 VTTipoChave *tipo_chave;

 // verifica dimensão
 if (length < 12)
 return (chaveINDEFINIDA);
 // extrai letras  que definem o tipo da Chave
 cod_chave = cod_chave.SubString(4, 2);
 // define código do tipo da Chave
 if (cod_chave.AnsiCompareIC("BF") == 0)
 cod_chave = "base fusível";
 else if (cod_chave.AnsiCompareIC("CZ") == 0)
 //cod_chave = "chave motorizada";
 cod_chave = "chave tripolar";
 else if (cod_chave.AnsiCompareIC("CF") == 0)
 cod_chave = "faca";
 else if (cod_chave.AnsiCompareIC("TD") == 0)
 cod_chave = "tandem";
 else if (cod_chave.AnsiCompareIC("CS") == 0)
 cod_chave = "seccionalizadora de alta";
 else if (cod_chave.AnsiCompareIC("MD") == 0)
 cod_chave = "medidor de fronteira";
 else if (cod_chave.AnsiCompareIC("RL") == 0)
 cod_chave = "religador";
 else if (cod_chave.AnsiCompareIC("FF") == 0)
 //cod_chave = "fusível com lâmina";
 cod_chave = "fusível faca";
 else if (cod_chave.AnsiCompareIC("BS") == 0)
 cod_chave = "fusível com seccionalizadora";
 else if (cod_chave.AnsiCompareIC("BR") == 0)
 //cod_chave = "fusível com religadora";
 cod_chave = "base fusível religadora";
 else if (cod_chave.AnsiCompareIC("DJ") == 0)
 cod_chave = "disjuntor";
 else if (cod_chave.AnsiCompareIC("SZ") == 0)
 cod_chave = "seccionalizador";
 else
 cod_chave = "indefinida";
 // determina tipo da chave
 tipo_chave = tipos->ExisteTipoChave(cod_chave);
 // retorna ID do tipo da chave
 return ((tipo_chave != NULL) ? tipo_chave->Tipo : chaveINDEFINIDA);
 }
 */

// -----------------------------------------------------------------------------
int __fastcall TPreMonta::ConverteTipoChave(AnsiString cod_chave)
{ // DVK 2017.07.26 considera os tipoChave default do sinap
    // variáveis locais
    int length = cod_chave.Length();
    VTTipoChave *tipo_chave;

    // verifica dimensão
    if (length < 12)
        return (chaveINDEFINIDA);
    // extrai letras que definem o tipo da Chave
    cod_chave = cod_chave.SubString(4, 2);
    // define código do tipo da Chave
    if (cod_chave.AnsiCompareIC("BF") == 0)
        cod_chave = "base fusível";
    else if (cod_chave.AnsiCompareIC("CZ") == 0)
        // cod_chave = "chave motorizada";
            cod_chave = "faca";
    else if (cod_chave.AnsiCompareIC("CF") == 0)
        cod_chave = "faca";
    else if (cod_chave.AnsiCompareIC("TD") == 0)
        cod_chave = "faca";
    else if (cod_chave.AnsiCompareIC("CS") == 0)
        cod_chave = "seccionadora";
    else if (cod_chave.AnsiCompareIC("MD") == 0)
        cod_chave = "faca";
    else if (cod_chave.AnsiCompareIC("RL") == 0)
        cod_chave = "religador";
    else if (cod_chave.AnsiCompareIC("FF") == 0)
        // cod_chave = "fusível com lâmina";
            cod_chave = "base fusível";
    else if (cod_chave.AnsiCompareIC("BS") == 0)
        cod_chave = "base fusível";
    else if (cod_chave.AnsiCompareIC("BR") == 0)
        // cod_chave = "fusível com religadora";
            cod_chave = "base fusível";
    else if (cod_chave.AnsiCompareIC("DJ") == 0)
        cod_chave = "disjuntor";
    else if (cod_chave.AnsiCompareIC("SZ") == 0)
        cod_chave = "seccionadora";
    else
        cod_chave = "indefinida";
    // determina tipo da chave
    tipo_chave = tipos->ExisteTipoChave(cod_chave);
    // retorna ID do tipo da chave
    if (tipo_chave != NULL)
        return (tipo_chave->Tipo);
    // tipo nao encontrado: default indefinida
    return (chaveINDEFINIDA);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18497403
int __fastcall TPreMonta::ConverteTipoGeracao(AnsiString tipo_geracao)
{
    //traduz tipo conforme especificação da EDP
    if (tipo_geracao.AnsiCompareIC("Fotovoltáica") == 0)
        return gerSOLAR;
    else if (tipo_geracao.AnsiCompareIC("Eólica") == 0)
        return gerEOLICA;
    else if (tipo_geracao.AnsiCompareIC("Microturbina") == 0)
        return gerVAPOR;
    else if (tipo_geracao.AnsiPos("Gerador") > 0)
        return gerPCH;
    else if (tipo_geracao.AnsiCompareIC("Célula Combustível") == 0)
        return gerDIESEL;
    else
        return gerDESCONHECIDO;
}

// ----------------------------------------------------------------------------
AnsiString __fastcall TPreMonta::ConverteCodigoTipoChave(AnsiString cod_chave)
{ // DVK 2017.07.26 retorna o texto que define cada tipo de Chave do GIS
    // variáveis locais
    int length = cod_chave.Length();

    // verifica dimensão
    if (length < 12)
        return ("indefinida");
    // extrai letras que definem o tipo da Chave
    cod_chave = cod_chave.SubString(4, 2);
    // define código do tipo da Chave
    if (cod_chave.AnsiCompareIC("BF") == 0)
        return "base fusível";
    else if (cod_chave.AnsiCompareIC("CZ") == 0)
        // return "chave motorizada";
            return "chave tripolar";
    else if (cod_chave.AnsiCompareIC("CF") == 0)
        return "faca";
    else if (cod_chave.AnsiCompareIC("TD") == 0)
        return "tandem";
    else if (cod_chave.AnsiCompareIC("CS") == 0)
        // return "seccionalizadora de alta";
            return "seccionalizadora AT";
    else if (cod_chave.AnsiCompareIC("MD") == 0)
        return "medidor de fronteira";
    else if (cod_chave.AnsiCompareIC("RL") == 0)
        return "religador";
    else if (cod_chave.AnsiCompareIC("FF") == 0)
        // return "fusível com lâmina";
            return "fusível faca";
    else if (cod_chave.AnsiCompareIC("BS") == 0)
        // return "fusível com seccionalizadora";
            return "fus.seccionalizadora";
    else if (cod_chave.AnsiCompareIC("BR") == 0)
        // return "fusível com religadora";
        // return "base fusível religadora";
            return "fusível religadora";
    else if (cod_chave.AnsiCompareIC("DJ") == 0)
        return "disjuntor";
    else if (cod_chave.AnsiCompareIC("SZ") == 0)
        return "seccionalizador";
    else
        return "indefinida";
}

/*
 // -----------------------------------------------------------------------------
 int __fastcall TPreMonta::ConverteTipoChave(AnsiString cod_chave)
 {
 //pendência: documento da EDP não define os possíveis tipos
 return(chaveINDEFINIDA);
 }
 */
/*
 // -----------------------------------------------------------------------------
 int __fastcall TPreMonta::ConverteTipoLigacao(int edp_lipo_ligacao)
 {
 // pendência: documento da EDP não define os possíveis tipos
 return (lgDELTA_FECHADO);
 }
 */

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::ConverteZeqvToPotCurto(VTSuprimento *psup)
{
    // variáveis locais
    complex<double>scc_3f_mva, scc_ft_mva, z0_pu, z1_pu;
    double Sbase_mva = 100;

    try
    { // determina valores complexos de Zeq e Zeq1 em pu
        z0_pu = complex<double>(psup->zeq0.r, psup->zeq0.x);
        z1_pu = complex<double>(psup->zeq1.r, psup->zeq1.x);
        // verifica se deu zero
        if (IsComplexZero(z0_pu) || IsComplexZero(z1_pu))
        {
            return (false);
        }
        // calcula PotCC, em MVA
        scc_3f_mva = (1. / conj(z1_pu)) * Sbase_mva;
        scc_ft_mva = (3. / ((2. * conj(z1_pu)) + conj(z0_pu))) * Sbase_mva;
        // atualiza pot.curto do Suprimento
        psup->pcc_3f.p = RoundTo(scc_3f_mva.real(), -3);
        psup->pcc_3f.q = RoundTo(scc_3f_mva.imag(), -3);
        psup->pcc_ft.p = RoundTo(scc_ft_mva.real(), -3);
        psup->pcc_ft.q = RoundTo(scc_ft_mva.imag(), -3);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add
                ("Erro ao converter impedância equivalente em potência de curto no suprimento");
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------------------------------
bool __fastcall TPreMonta::CriaArranjoTipicoBT(void)
{
    // variáveis locais
    strARRANJO2 str;
    // verifica se já existe o Arranjo BT default
    if (arranjoBT != NULL)
    {
        // #ifdef _DEBUG
        function = __FUNC__;
        msglog = "Já existe Arranjo BT default em " + AnsiString(function);
        if (log_thread)
            log_thread->AddLog(msglog, function);
        // #endif
        return (true);
    }
    try
    { // inicia atributos comuns
        str.tipico = true;
        str.data = DateOf(Now());
        str.arranjo_id = FalsoId();
        str.tipo_cfg = arranjoZ0Z1;
        str.fases = faseABCN;
        str.ramal = false;
        // define código do Arranjo compondo código dos cabos de fase e neutro
        str.codigo = "Arranjo_BT";
        str.extern_id = "";
        str.status = sttNOVO;
        // define atributo específicos do arranjo Z0Z1 (baseado no 4 CAA)
        str.cfg_arranjoZ0Z1.iadm_amp = 153;
        str.cfg_arranjoZ0Z1.z0.r = 1.8046;
        str.cfg_arranjoZ0Z1.z0.x = 0.5495;
        str.cfg_arranjoZ0Z1.z1.r = 1.8046;
        str.cfg_arranjoZ0Z1.z1.x = 0.5495;
        // converte capacitância para nF/km
        str.cfg_arranjoZ0Z1.c0 = 0.;
        str.cfg_arranjoZ0Z1.c1 = 0.;
        str.cfg_arranjoZ0Z1.suporte_id = ID_NULO;
        // insere Arranjo
        arranjoBT = monta->InsereArranjo(str);
        // #ifdef _DEBUG
        if (arranjoBT)
        {
            // function = __FUNC__;
            // msglog = "Inseriu arranjo com sucesso em " + AnsiString(function);
            // if(log_thread)log_thread->AddLog(msglog,function);
        }
        else
        {
            function = __FUNC__;
            msglog = "Inserção falhou em " + AnsiString(function);
            if (log_thread)
                log_thread->AddLog(msglog, function, true);
        }
        // #endif
    }
    catch (Exception &e)
    {
        // #ifdef _DEBUG
        function = __FUNC__;
        msglog = "Exception caught em ";
        msglog += AnsiString(function);
        msglog += " " + e.Message;
        if (log_thread)
            log_thread->AddLog(msglog, function, true);
        // #endif
        return (false);
    }
    return (arranjoBT != NULL);
}

// ---------------------------------------------------------------------------------------------------
bool __fastcall TPreMonta::CriaArranjoTipicoMT(void)
{
    // variáveis locais
    strARRANJO2 str;
    // verifica se já existe o Arranjo MT default
    if (arranjoMT != NULL)
    {
        // #ifdef _DEBUG
        function = __FUNC__;
        msglog = "Já existe Arranjo MT default em " + AnsiString(function);
        if (log_thread)
            log_thread->AddLog(msglog, function);
        // #endif
        return (true);
    }
    try
    { // inicia atributos comuns
        str.tipico = true;
        str.data = DateOf(Now());
        str.arranjo_id = FalsoId();
        str.tipo_cfg = arranjoZ0Z1;
        str.fases = faseABCN;
        str.ramal = false;
        // define código do Arranjo compondo código dos cabos de fase e neutro
        str.codigo = "Arranjo_MT";
        str.extern_id = "";
        str.status = sttNOVO;
        // define atributo específicos do arranjo Z0Z1 (baseado no 336 CAA)
        str.cfg_arranjoZ0Z1.iadm_amp = 591;
        str.cfg_arranjoZ0Z1.z0.r = 0.2240;
        str.cfg_arranjoZ0Z1.z0.x = 0.4165;
        str.cfg_arranjoZ0Z1.z1.r = 0.2240;
        str.cfg_arranjoZ0Z1.z1.x = 0.4165;
        // converte capacitância para nF/km
        str.cfg_arranjoZ0Z1.c0 = 0.;
        str.cfg_arranjoZ0Z1.c1 = 0.;
        str.cfg_arranjoZ0Z1.suporte_id = ID_NULO;
        // insere Arranjo
        arranjoMT = monta->InsereArranjo(str);
        // #ifdef _DEBUG
        if (arranjoMT)
        {
            // function = __FUNC__;
            // msglog = "Inseriu arranjo com sucesso em " + AnsiString(function);
            // if(log_thread)log_thread->AddLog(msglog,function);
        }
        else
        {
            function = __FUNC__;
            msglog = "Inserção falhou em " + AnsiString(function);
            if (log_thread)
                log_thread->AddLog(msglog, function, true);
        }
        // #endif
    }
    catch (Exception &e)
    {
        // #ifdef _DEBUG
        function = __FUNC__;
        msglog = "Exception caught em ";
        msglog += AnsiString(function);
        msglog += " " + e.Message;
        if (log_thread)
            log_thread->AddLog(msglog, function, true);
        // #endif
        return (false);
    }
    return (arranjoMT != NULL);
}

// ---------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta::CriaCargaConsumidorBT(sdi_CONS_BT &sdi, int barra_id)
{
    // variáveis locais
    double kwhmes_max;
    int fases_car, fases_et;
    strCARGA str;
    VTCarga *carga;

    try
    { // configura Log
        if (plog)
            plog->DefineValor("Eqpto", "Cons.BT");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // determina fases da Et
        fases_et = et->FasesSecundario();
        // verifica se as fases do Consumidor estão definidas
        if ((sdi.fases.IsEmpty()) || (sdi.fases.AnsiCompareIC("N") == 0))
        { // assume fases da ET sem terra
            if (plog)
                plog->Add("Fases indefinidas");
            fases_car = fases_et & faseABCN;
        }
        else
        {
            fases_car = fases->ExisteFase(sdi.fases);
            // verifica se as fases da carga são compatíveis com as fases da ET
            if (!fases->Fase1ContemFase2(fases_et, fases_car))
            { // assume fases da ET, sem terra
                if (plog)
                    plog->Add("Fases incompatíveis com a ET");
                fases_car = fases_et & faseABCN;
            }
        }
        // garante consumidor com neutro
        if ((fases_car & faseN) == 0)
        {
            if (plog)
                plog->Add("Fases não incluem neutro");
            fases_car |= faseN;
        }
        // verifica consumo muito grande
        switch (fases->NumeroDeFases(fases_car))
        {
        case 1:
            kwhmes_max = 1000;
            break;
        case 2:
            kwhmes_max = 5000;
            break;
        case 3:
            kwhmes_max = 15000;
            break;
        default:
            kwhmes_max = 1000;
            break;
        }
        if (sdi.consumo_kwh_mes > kwhmes_max)
        {
            if (plog)
                plog->Add("Consumo de " + DoubleToStr("%2.1f kWh/mês", sdi.consumo_kwh_mes));
            // reduz valor de consumo
            sdi.consumo_kwh_mes = kwhmes_max;
        }
        // preenche estrutura strCARGA
        str.car_id = sdi.id;
        str.bar_id = barra_id;
        str.codigo = sdi.codigo;
        // define classe de consumidor/energia
        str.classe_cons = ConverteClasseConsumidor(sdi.classe_cons);
        str.grupoA = false;
        // define modelo de carga Icte
        str.modelo.icte_pu = 1;
        str.modelo.scte_pu = 0;
        str.modelo.zcte_pu = 0;
        str.fases = fases_car;
        str.energia_mwhmes = sdi.consumo_kwh_mes * 0.001;
        // cancela vetor de demanda p/ curva própria
        str.curva.id = -1;
        str.curva.tipica = true;
        str.curva.num_pat = 0;
        str.curva.num_valor = 2;
        // cancela dados de IP
        str.ip.num_lamp = 0;
        str.ip.pot_kw = 0.;
        str.ip.fat_pot = 1.;
        // esquemático
        str.esq.posbar = 50.;
        str.esq.dxbar = DXBAR;
        str.esq.dybar = -DYBAR;
        // insere Carga em VTMonta
        carga = (VTCarga*)(monta->InsereCarga(str));
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        carga = NULL;
    }
    return (carga);
}

// -----------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta::CriaCargaConsumidorIP(sdi_CONS_IP &sdi, int barra_id)
{
    // variáveis locais
    int fases_et, fases_car;
    double p_kw, q_kvar, s_kva;
    strCARGA str;
    VTCarga *carga;

    try
    { // configura Log
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // determina fases do secundário da Et
        fases_et = et->FasesSecundario();
        // preenche estrutura strCARGA
        str.car_id = sdi.id;
        str.bar_id = barra_id;
        str.codigo = sdi.codigo;
        str.classe_cons = consIP;
        str.grupoA = false;
        // define modelo Scte
        str.modelo.icte_pu = 0.;
        str.modelo.scte_pu = 1.;
        str.modelo.zcte_pu = 0.;
        // determina fases
        fases_car = fases->ExisteFase(sdi.fases);
        // verifica se as fases da carga são compatíveis com as fases da ET
        if (!fases->Fase1ContemFase2(fases_et, fases_car))
        { // assume fases da ET, sem neutro e terra
            if (plog)
                plog->Add("Fases incompatíveis com a ET");
            fases_car = fases->Fase1IntersecaoFase2(faseABC, fases_et);
            // não permite as 3 fases p/ IP
            if ((fases_car & faseABC) == faseABC)
            { // assume fases AB
                fases_car = faseAB;
            }
            // verifca se há apenas uma fase
            if ((fases_car == faseA) || (fases_car == faseB) || (fases_car == faseC))
            { // inclui neutro
                fases_car = fases_car + faseN;
            }
        }
        str.fases = fases_car;
        // cancela energia
        str.energia_mwhmes = 0.;
        // define curva típica
        str.curva.id = -1;
        str.curva.tipica = true;
        str.curva.num_pat = 0;
        str.curva.num_valor = 2;
        // define dados de IP
        str.ip.num_lamp = sdi.n_lamp;
        // proteção: verifica potência ativa com valor nulo
        p_kw = sdi.lamp_dem_w * 0.001;
        q_kvar = sdi.lamp_dem_var * 0.001;
        if (IsDoubleZero(p_kw))
        {
            if (plog)
                plog->Add("Potência ativa nula");
            p_kw = 0.2;
        }
        // proteção: verifica potência ativa com valor muito alto
        else if (p_kw > 1.0)
        {
            if (plog)
                plog->Add(DoubleToStr("Potência ativa de %4.3f kW", p_kw));
            while (p_kw > 1.0)
                p_kw *= 0.1;
        }
        // determina fator de potência
        s_kva = sqrt((p_kw * p_kw) + (q_kvar * q_kvar));
        str.ip.pot_kw = p_kw;
        str.ip.fat_pot = p_kw / s_kva;
        // esquemático
        str.esq.posbar = 50.;
        str.esq.dxbar = 0;
        str.esq.dybar = -DYBAR;
        // insere Carga em VTMonta
        carga = (VTCarga*)monta->InsereCarga(str);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        carga = NULL;
    }
    return (carga);
}

// -----------------------------------------------------------------------------
TEt* __fastcall TPreMonta::CriaEt(sdi_INSTAL_TRAFO &sdi, VTBarra *barra_mt, VTBarra *barra_bt)
{
    // variáveis locais
    int fases_tag_et;
    TEt *et;

    try
    { // configura Log
        if (plog)
            plog->DefineValor("Eqpto", "ET");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // determina fase do primário da ET
        fases_tag_et = fases->ExisteFase(sdi.fases_mt);
        // verifica tipo da ligação
        switch (sdi.tipo_lig)
        {
        case 1: // monofásica
            if ((fases_tag_et & faseA) == faseA)
                fases_tag_et = faseA;
            else if ((fases_tag_et & faseB) == faseB)
                fases_tag_et = faseB;
            else if ((fases_tag_et & faseC) == faseC)
                fases_tag_et = faseC;
            else
            {
                if (plog)
                    plog->Add("ET monofásica com fase inválida: " + sdi.fases_mt);
                fases_tag_et = faseA;
            }
            break;
        case 2: // bifásico
            if ((fases_tag_et & faseAB) == faseAB)
                fases_tag_et = faseAB;
            else if ((fases_tag_et & faseBC) == faseBC)
                fases_tag_et = faseBC;
            else if ((fases_tag_et & faseCA) == faseCA)
                fases_tag_et = faseCA;
            else
            {
                if (plog)
                    plog->Add("ET bifásica com fase inválida: " + sdi.fases_mt);
                fases_tag_et = faseAB;
            }
            break;
        case 3: // trifásico
            if ((fases_tag_et & faseABC) == faseABC)
                fases_tag_et = faseABC;
            else
            {
                if (plog)
                    plog->Add("ET trifásica com fase inválida: " + sdi.fases_mt);
                fases_tag_et = faseABC;
            }
            break;
        case 4: // delta aberto
            if ((fases_tag_et & faseAB) == faseAB)
                fases_tag_et = faseAB;
            else if ((fases_tag_et & faseBC) == faseBC)
                fases_tag_et = faseBC;
            else if ((fases_tag_et & faseCA) == faseCA)
                fases_tag_et = faseCA;
            else
            {
                if (plog)
                    plog->Add("ET delta aberto com fase inválida: " + sdi.fases_mt);
                fases_tag_et = faseAB;
            }
            break;
        case 5: // delta fechado
            if ((fases_tag_et & faseABC) == faseABC)
                fases_tag_et = faseABC;
            else
            {
                if (plog)
                    plog->Add("ET delta fechado com fase inválida: " + sdi.fases_mt);
                fases_tag_et = faseABC;
            }
            break;
        default:
            if (plog)
                plog->Add("Tipo de ligação inválido");
            // assume tipo em função das fases
            switch (fases_tag_et)
            {
            case faseA: // monofásico
            case faseB:
            case faseC:
                sdi.tipo_lig = 1;
                break;
            case faseAB: // bifásico
            case faseBC:
            case faseCA:
                sdi.tipo_lig = 2;
                break; // bifásico
            case faseABC: // trifásico
                sdi.tipo_lig = 3;
                break;
            default: // assume trifásico
                sdi.tipo_lig = 3;
                fases_tag_et = faseABC;
            }
        }
        // verifica Snom nula
        if (IsDoubleZero(sdi.snom_kva))
        {
            if (plog)
                plog->Add("Potência nominal nula");
            if (sdi.tipo_lig == 1)
                sdi.snom_kva = 30;
            else if (sdi.tipo_lig == 2)
                sdi.snom_kva = 50;
            else
                sdi.snom_kva = 100;
        }
        // verifica tensão nula ou acima de 380V
        if (IsDoubleZero(sdi.vbaixa_v) || (sdi.vbaixa_v > 380))
        {
            if (IsDoubleZero(sdi.vbaixa_v))
            {
                if (plog)
                    plog->Add("Tensão secundário nula");
            }
            else if (sdi.vbaixa_v > 380)
            {
                if (plog)
                    plog->Add("Tensão secundário inválida: " + DoubleToStr("%2.1f V",
                sdi.vbaixa_v));
            }
            // assume tensão do secundário em função do tipo
            if (sdi.tipo_lig == 1)
                sdi.vbaixa_v = 220;
            else if (sdi.tipo_lig == 2)
                sdi.vbaixa_v = 230;
            else
                sdi.vbaixa_v = 220;
        }
        // verifica se código está definido
        if (sdi.codigo.IsEmpty())
            sdi.codigo = "id=" + IntToStr(sdi.id_rede_bt);
        // cria um objeto Et e inicia seus dados
        if ((et = NewObjEt()) == NULL)
        {
            if (plog)
                plog->Add("Não foi possível alocar memória");
            return (NULL);
        }
        // define tipo da Et e cria as UTCs apropriadas
        et->DefTopologia(sdi.tipo_lig);
        et->id = sdi.id_rede_bt;
        et->codigo = sdi.codigo;
        et->ativo = sdi.ativo;
        et->barra_mt = barra_mt;
        et->barra_bt = barra_bt;
        // dados do Trafo
        et->snom_kva = sdi.snom_kva;
        et->pri_vnom_kv = circuito->vnom_kv;
        et->sec_vnom_kv = (sdi.vbaixa_v) * 0.001; // V => kV
        et->fases_mt = fases_tag_et;
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (NULL);
    }
    return (et);
}

// ---------------------------------------------------------------------------
TColor __fastcall TPreMonta::DefineCorRedePrimaria(void)
{
    // variáveis locais
    int vet_size, index;
    TColor vet_color[] =
    {clBlue, clRed, clGreen, clYellow, clSkyBlue, clFuchsia, clOlive, clPurple, clNavy, clMaroon,
        clTeal, clLime};

    // proteção
    if ((se == NULL) || (circuito == NULL))
        return (clBlue);
    // determina dimensão do vetor de cores
    vet_size = sizeof(vet_color) / sizeof(vet_color[0]);
    index = se->lisCIRCUITO->IndexOf(circuito);
    while (index >= vet_size)
        index -= vet_size;

    return (vet_color[index]);
}

/*
 // ---------------------------------------------------------------------------
 VTArranjo* __fastcall TPreMonta::ExisteArranjo(int fases_tag, int cabo_fase_id, int cabo_neutro_id, int suporte_id)
 {
 //variáveis locais
 VTArranjo  *arranjo;
 VTCabo     *cabo_fase, *cabo_neutro;
 VTCabo     *caboA, *caboB, *caboC, *caboN;
 VTSuporte  *suporte;

 //determina Cabo de fase
 if ((cabo_fase = arranjos->ExisteCabo(cabo_fase_id)) == NULL)
 {
 if (plog) plog->Add("Não existe cabo da fase");
 return(NULL);
 }
 //verifica se o Cabo da fase é do tipo caboZ0Z1
 if (cabo_fase->TipoCfg == caboZ0Z1)
 {//determina Arranjo definido por caboZ0Z1
 if ((arranjo = arranjos->ExisteArranjoCABO_Z0Z1(cabo_fase)) == NULL)
 {//insere Arranjo definido por caboZ0Z1
 arranjo = InsereCfg_ArranjoCaboZ0Z1(fases_tag, cabo_fase);
 }
 return(arranjo);
 }
 //arranjo definido por cabo de fase, cabo de neutro e suporte
 if ((cabo_neutro = arranjos->ExisteCabo(cabo_neutro_id)) == NULL)
 {
 if (plog) plog->Add("Não existe cabo da neutro");
 return(NULL);
 }
 if ((suporte = arranjos->ExisteSuporte(suporte_id)) == NULL)
 {
 if (plog) plog->Add("Não existe suporte");
 return(NULL);
 }
 //verifica se existe o ArranjoCABOxFASE
 caboA = (fases_tag & faseA) ? cabo_fase   : NULL;
 caboB = (fases_tag & faseB) ? cabo_fase   : NULL;
 caboC = (fases_tag & faseC) ? cabo_fase   : NULL;
 caboN = (fases_tag & faseN) ? cabo_neutro : NULL;
 if ((arranjo = arranjos->ExisteArranjoCABOxFASE(suporte, caboA, caboB, caboC, caboN)) == NULL)
 {//insere Arranjo definido por cabos de fase/neutro e suporte
 arranjo = InsereCfg_ArranjoCABOxFASE(fases_tag, suporte, cabo_fase, cabo_neutro);
 }

 return(arranjo);
 }
 */
/*
 // ---------------------------------------------------------------------------
 VTArranjo* __fastcall TPreMonta::ExisteArranjo(int arranjo_id, int fases_tag, bool ramal)
 {
 // variáveis locais
 AnsiString codigo;
 VTArranjo *arranjo_edp, *arranjo;

 // procura Arranjo original da EDP pelo seu ID
 if ((arranjo_edp = arranjos->ExisteArranjo(arranjo_id)) == NULL)
 {
 return (NULL);
 }
 // verifica se o Arranjo tem as fases indicadas
 if (arranjo_edp->Fases == fases_tag)
 {
 arranjo = arranjo_edp;
 }
 else
 { // o Arranjo original da EDP tem fases diferents da indicada:
 // verifica se existe um Arranjo cujo código é igual ao do Arranjo da EDP acrescido com string das fases
 codigo = arranjo_edp->Codigo + fases->AsStrABCN[fases_tag];
 if ((arranjo = arranjos->ExisteArranjo(codigo)) == NULL)
 { // cria uma cópia do Arranjo original e redefine seu código e fases
 if ((arranjo = arranjo_edp->Clone()) != NULL)
 {
 arranjo->Id = FalsoId();
 arranjo->Codigo = codigo;
 arranjo->Fases = fases_tag;
 // insere o novo Arranjo em Arranjos
 arranjos->InsereArranjo(arranjo);
 }
 }
 }
 // define Arranjo como ramal ou não
 arranjo->ramal = ramal;

 return (arranjo);
 }
 */

// ---------------------------------------------------------------------------
VTArranjo* __fastcall TPreMonta::ExisteArranjo(int arranjo_id, int fases_tag, bool ramal)
{
    // variáveis locais
    AnsiString codigo;
    VTArranjo *arranjo_edp, *arranjo;

    // proteção
    if (arranjo_id == 0)
        return (NULL);
    // corrige id do arranjo
    arranjo_id += ARRANJO_ID;
    // procura Arranjo original da EDP pelo seu ID
    if ((arranjo_edp = arranjos->ExisteArranjo(arranjo_id)) == NULL)
        return (NULL);
    // verifica se o Arranjo tem as fases indicadas
    if (arranjo_edp->Fases == fases_tag)
        arranjo = arranjo_edp;
    else
    { // o Arranjo original da EDP tem fases diferentes da indicada:
        // verifica se existe um Arranjo cujo código é igual ao do Arranjo da EDP acrescido com string das fases
        codigo = arranjo_edp->Codigo + "_" + fases->AsStrABCN[fases_tag];
        if ((arranjo = arranjos->ExisteArranjo(codigo)) == NULL)
        { // cria uma cópia do Arranjo original e redefine seu código e fases
            if ((arranjo = arranjo_edp->Clone()) != NULL)
            {
                arranjo->Id = FalsoId();
                arranjo->Codigo = codigo;
                arranjo->Fases = fases_tag;
                arranjo->Status[sttNOVO] = true;
                // insere o novo Arranjo em Arranjos
                arranjos->InsereArranjo(arranjo);
            }
        }
    }
    // define Arranjo como ramal ou não
    arranjo->ramal = ramal;

    return (arranjo);
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TPreMonta::ExisteBarra(VTBarra *barra_ref, TList *lisBarrasIndesejaveis,
    VTRede *rede1, VTRede *rede2, double raio_m)
{
    // variáveis locais
    double menor_dist, dist;
    int x, y;
    int coord_x, coord_y;
    int delta_x = 0;
    int delta_y = 0;
    VTBarra *barra, *barraIndesej;
    VTBarra *barra_escolhida = NULL;
    TList *lisBarra;
    TList *lisCandidatas;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

    // proteção
    if (barra_ref == NULL)
        return NULL;
    // inicia x e y
    x = barra_ref->utm.x;
    y = barra_ref->utm.y;
    // cria listas
    lisBarra = new TList;
    lisCandidatas = new TList;
    // verifica se foi indicada a Rede para busca da Barra
    if (rede1 != NULL)
        rede1->LisBarra(lisBarra);
    else
        lisBarra = redes->LisBarra();
    if (rede2 != NULL)
        rede2->LisBarra(lisBarra);
    if (IsDoubleZero(raio_m, 0.01))
    {
        delta_x = 100;
        delta_y = 100;
    }
    else
    {
        delta_x = raio_m * 100;
        delta_y = raio_m * 100;
    }

    coord_x = x;
    coord_y = y;
    // determina a lista de candidatas
    for (int nb = 0; nb < lisBarra->Count; nb++)
    {
        barra = (VTBarra*) lisBarra->Items[nb];
        // verifica se esta dentro do quadrado
        if ((barra->utm.x > (coord_x - delta_x)) && (barra->utm.x < (coord_x + delta_x)))
        {
            if ((barra->utm.y > (coord_y - delta_y)) && (barra->utm.y < (coord_y + delta_y)))
            {
                if (barra_ref != barra)
                {
                    if (lisBarrasIndesejaveis != NULL)
                    {
                        if (lisBarrasIndesejaveis->IndexOf(barra) < 0)
                        {
                            lisCandidatas->Add(barra);
                        }
                    }
                    else
                    {
                        lisCandidatas->Add(barra);
                    }
                }
            }
        }
    }
    // verifica menor distancia
    for (int nb = 0; nb < lisCandidatas->Count; nb++)
    {
        barra = (VTBarra*) lisCandidatas->Items[nb];
        if (nb == 0)
        {
            menor_dist =
                sqrt(fabs((long double)coord_x - barra->utm.x) * fabs((long double)coord_x -
                barra->utm.x) + fabs((long double)coord_y - barra->utm.y) * fabs
                ((long double)coord_y - barra->utm.y));
            barra_escolhida = barra;
            continue;
        }
        dist = sqrt(fabs((long double)coord_x - barra->utm.x) * fabs((long double)coord_x -
            barra->utm.x) + fabs((long double)coord_y - barra->utm.y) * fabs((long double)coord_y -
            barra->utm.y));
        if (dist < menor_dist)
        {
            barra_escolhida = barra;
            menor_dist = dist;
        }
    }
    // delete
    DestroiTObject(lisCandidatas);
    DestroiTObject(lisBarra);

    return (barra_escolhida);
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TPreMonta::ExisteBarraCircuito(int bar_id, TCircuito *circ1, TCircuito *circ2)
{
    // variaveis locais
    VTBarra *barra = NULL;

    // localiza barra no circuito que nao for nulo
    if (circ1 != NULL)
        barra = circ1->ExisteBarra(bar_id);
    if ((barra == NULL) && (circ2 != NULL))
        barra = circ2->ExisteBarra(bar_id);

    return (barra);
}

// ---------------------------------------------------------------------------
VTLigacao* __fastcall TPreMonta::ExisteLigacaoCircuito(VTBarra* barra1, VTBarra* barra2,
    TCircuito *circ1, TCircuito *circ2)
{
    // variaveis locais
    VTLigacao *ligacao = NULL;

    // localiza ligacao no circuito que nao for nulo
    if (circ1 != NULL)
        ligacao = circ1->rede->ExisteLigacao(barra1, barra2);
    if ((ligacao == NULL) && (circ2 != NULL))
        ligacao = circ2->rede->ExisteLigacao(barra1, barra2);

    return (ligacao);
}

// ---------------------------------------------------------------------------
TSe* __fastcall TPreMonta::ExisteSubestacao(int se_id)
{
    // variáveis locais
    TSe *se;

    // procura TSe
    for (int n = 0; n < lisSE->Count; n++)
    {
        se = (TSe*)lisSE->Items[n];
        if (se->id == se_id)
            return (se);
    }
    return (NULL);
}
/*
 //---------------------------------------------------------------------------
 int __fastcall TPreMonta::IdentificaTipoCabo(sdi_CABO &sdi)
 {
 //verifica se Z0 e Z1 tem valores não nulos
 if ((! IsDoubleZero(sdi.r0_ohm_km))&&
 (! IsDoubleZero(sdi.x0_ohm_km))&&
 (! IsDoubleZero(sdi.r1_ohm_km))&&
 (! IsDoubleZero(sdi.x1_ohm_km))) return(caboZ0Z1);
 //verifica se resistência e raio médio tem valores não nulos
 if ((! IsDoubleZero(sdi.r_ohm_km))&&
 (! IsDoubleZero(sdi.grm_m))) return(caboR_GMR);
 //cabo inválido
 return(-1);
 }
 */

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::IniciaDominio(void)
{
    // variáveis locais
    AnsiString txt;
    strDOMINIO str_dominio;

    try
    { // insere um Dominio em VTMonta
        txt.sprintf("Rede_%04d.%02d.%02d", YearOf(DataDominio), MonthOf(DataDominio),
            DayOf(DataDominio));
        str_dominio.obra = false;
        str_dominio.dominio_id = FalsoId();
        str_dominio.codigo = txt;
        str_dominio.data_criacao = DataDominio;
        dominio = monta->InsereDominio(str_dominio);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add("Erro na inserção de domínio");
        return (false);
    }
    return (dominio != NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCfg_Arranjo(sdi_ARRANJO &sdi)
{
    // variáveis locais
    strARRANJO2 str;
    AnsiString type;
    AnsiString formation;
    double freq = 60.; // hertz
    double w = 2. * M_PI * freq;

    try
    { // inicia atributos comuns
        str.tipico = true;
        str.data = DateOf(Now());
        str.arranjo_id = ARRANJO_ID + sdi.id;
        str.tipo_cfg = arranjoZ0Z1;
        str.fases = faseABCN;
        str.ramal = false;
        str.codigo = sdi.codigo;
        str.extern_id = "";
        str.status = sttNOVO;
        // define atributo específicos do arranjo Z0Z1
        str.cfg_arranjoZ0Z1.iadm_amp = sdi.iadm_a;
        str.cfg_arranjoZ0Z1.z0.r = sdi.r0_ohm_km;
        str.cfg_arranjoZ0Z1.z0.x = sdi.x0_ohm_km;
        str.cfg_arranjoZ0Z1.z1.r = sdi.r1_ohm_km;
        str.cfg_arranjoZ0Z1.z1.x = sdi.x1_ohm_km;
        // converte capacitância para nF/km
        str.cfg_arranjoZ0Z1.c0 = (sdi.b0_s_km / w) * 1e-9;
        str.cfg_arranjoZ0Z1.c1 = (sdi.b1_s_km / w) * 1e-9;
        str.cfg_arranjoZ0Z1.suporte_id = ID_NULO;
        // insere Arranjo
        if (monta->InsereArranjo(str) == NULL)
            return (false);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add("Erro na inserção de cabo: " + sdi.codigo);
        return (false);
    }
    return (true);
}

/*
 // ---------------------------------------------------------------------------
 VTArranjo* __fastcall TPreMonta::InsereCfg_ArranjoCaboZ0Z1(int fases_tag, VTCabo *cabo)
 {
 // variáveis locais
 strARRANJO2 str;
 VTArranjo *arranjo;

 try
 { // cofigura Log
 if (plog)
 plog->DefineValor("Eqpto", "Arranjo");
 // preenche estrutura strARRANJO2
 str.tipico = true;
 str.data = DateOf(Now());
 str.arranjo_id = FalsoId();
 str.codigo = fases->AsStrABCN[fases_tag] + "[" + cabo->Codigo + "]";
 str.extern_id = "";
 str.tipo_cfg = arranjoCABO_Z0Z1;
 str.fases = fases_tag;
 str.res_terra = 100.;
 str.vmin_kv = 0;
 str.vmax_kv = 0;
 str.color = clBlue;
 str.status = sttNOVO;
 str.cfg_caboZOZ1.cabo_id = cabo->Id;
 str.cfg_caboZOZ1.suporte_id = ID_NULO;
 // insere Arranjo em VTMonta
 arranjo = (VTArranjo*)monta->InsereArranjo(str);
 }
 catch (Exception &e)
 {
 if (plog)
 plog->Add(ERRO_DE_SISTEMA);
 arranjo = NULL;
 }
 return (arranjo);
 }

 // ---------------------------------------------------------------------------
 VTArranjo* __fastcall TPreMonta::InsereCfg_ArranjoCABOxFASE(int fases_tag, VTSuporte *suporte,
 VTCabo *cabo_fase, VTCabo *cabo_neutro)
 {
 // variáveis locais
 VTArranjo *arranjo;
 AnsiString codigo;
 strARRANJO2 str;
 int vet_cabo_id[MAX_FASE];

 try
 { // cofigura Log
 if (plog)
 plog->DefineValor("Eqpto", "Arranjo");
 // define código do Arranjo
 codigo = fases->AsStrABCN[fases_tag];
 if (cabo_fase)
 codigo = codigo + "[" + cabo_fase->Codigo + "]";
 if (cabo_neutro)
 codigo = codigo + "[" + cabo_neutro->Codigo + "]";
 // preenche estrutura strARRANJO2
 str.tipico = true;
 str.data = DateOf(Now());
 str.arranjo_id = FalsoId();
 str.codigo = codigo;
 str.extern_id = "";
 str.tipo_cfg = arranjoCABOxFASE;
 str.fases = fases_tag;
 str.res_terra = 100.;
 str.vmin_kv = 0;
 str.vmax_kv = 0;
 str.color = clBlue;
 str.status = sttNOVO;
 // define o suporte
 str.cfg_suporte_cabo.suporte_id = suporte->Id;
 // preenche vetor com IDs dos cabos de cada fase
 vet_cabo_id[indFASE_A] = (fases_tag & faseA) ? cabo_fase->Id : ID_NULO;
 vet_cabo_id[indFASE_B] = (fases_tag & faseB) ? cabo_fase->Id : ID_NULO;
 vet_cabo_id[indFASE_C] = (fases_tag & faseC) ? cabo_fase->Id : ID_NULO;
 vet_cabo_id[indNEUTRO] = (fases_tag & faseN) ? cabo_neutro->Id : ID_NULO;
 for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
 {
 str.cfg_suporte_cabo.fase[ind_fase].cabo_id = vet_cabo_id[ind_fase];
 str.cfg_suporte_cabo.fase[ind_fase].pto_fixacao_id = ind_fase + 1;
 str.cfg_suporte_cabo.fase[ind_fase].multiplicidade = 1;
 }
 // insere Arranjo em VTMonta
 arranjo = (VTArranjo*)monta->InsereArranjo(str);
 }
 catch (Exception &e)
 {
 if (plog)
 plog->Add(ERRO_DE_SISTEMA);
 arranjo = NULL;
 }
 return (arranjo);
 }
 */
/*
 //---------------------------------------------------------------------------
 bool __fastcall TPreMonta::InsereCfg_Cabo(sdi_CABO &sdi)
 {
 //variáveis locais
 strCABO str;
 double  freq = 60.; //hertz
 double  w    =  2. * M_PI * freq;

 try{//inicia atributos comuns
 str.id        = sdi.id;
 str.codigo    = sdi.codigo;
 str.extern_id = sdi.extern_id;
 str.status    = sttNOVO;
 str.iadm      = sdi.iadm_a;
 str.vmin      = 0.;
 str.vmax      = 0.;
 //verifica se é um caboR_GMR ou um caboZ0Z1
 switch(IdentificaTipoCabo(sdi))
 {
 case caboR_GMR:
 str.tipo_cfg  = caboR_GMR;
 str.r         = sdi.r_ohm_km;
 str.raio      = 0.;
 str.gmr       = sdi.grm_m;
 break;
 case caboZ0Z1:
 str.tipo_cfg  = caboZ0Z1;
 str.z0.r      = sdi.r0_ohm_km;
 str.z0.x      = sdi.x0_ohm_km;
 str.z1.r      = sdi.r1_ohm_km;
 str.z1.x      = sdi.x1_ohm_km;
 //converte capacitância para nF/km
 str.c0        = (sdi.b1_s_km / w)  * 1e9;
 str.c1        = (sdi.b1_s_km / w)  * 1e9;
 break;
 default:
 if (plog) plog->Add("Cabo inválido: " + sdi.codigo);
 return(false);
 }
 //insere Cabo
 if (monta->InsereCabo(str) == NULL) return(false);
 }catch(Exception &e)
 {
 if (plog) plog->Add("Erro na inserção de cabo: " + sdi.codigo);
 return(false);
 }
 return(true);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCfg_Suporte(sdi_SUPORTE &sdi)
{
    // variáveis locais
    double x_m, y_m;
    strSUPORTE str;

    try
    { // inicia strSUPORTE (apesar de a estrutura ter nome de "arranjo"
        str.id = sdi.id;
        str.codigo = sdi.codigo;
        str.extern_id = sdi.extern_id;
        str.status = sttNOVO;
        str.tipo = suporteAEREO;
        str.vmin = 0.;
        str.vmax = 0.;
        str.num_pto = 4;
        // define coordenadas dos 4 pontos
        for (int index = 0; index < 4; index++)
        {
            str.pto_fixacao[index].id = index + 1;
            str.pto_fixacao[index].codigo = "Ponto" + IntToStr(index + 1);
            switch (index + 1)
            {
            case 1:
                x_m = sdi.fase1_x_m;
                y_m = sdi.fase1_y_m;
                break;
            case 2:
                x_m = sdi.fase2_x_m;
                y_m = sdi.fase2_y_m;
                break;
            case 3:
                x_m = sdi.fase3_x_m;
                y_m = sdi.fase3_y_m;
                break;
            case 4:
                x_m = sdi.fase4_x_m;
                y_m = sdi.fase4_y_m;
                break;
            }
            str.pto_fixacao[index].coord_x = x_m;
            str.pto_fixacao[index].coord_y = y_m;
        }
        // insere Suporte
        if (!monta->InsereSuporte(str) == NULL)
            return (false);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add("Erro na inserção de arranjo: " + sdi.codigo);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereEtd_Barra(sdi_BARRAMENTO &sdi)
{
    // variáveis locais
    strBARRA str;
    VTBarra *barra;

    try
    { // cofigura Log
        if (plog)
            plog->DefineValor("Eqpto", "Barramento");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // proteção
        if (se == NULL)
        {
            if (plog)
                plog->Add("Não está associado a ETD");
            return (true);
        }
        // insere uma nova Barra
        str.bar_id = sdi.id;
        str.codigo = sdi.codigo;
        str.extern_id = "";
        str.vnom = sdi.vnom_kv;
        // ATENÇÃO: as coordenadas do barramento estão em cm mas a unidade indicada é metro
        str.utm.x = sdi.x_utm_m; // * 100;
        str.utm.y = sdi.y_utm_m; // * 100;
        // define coordenadas do esquemático
        str.esq.x1 = str.utm.x;
        str.esq.y1 = str.utm.y;
        str.esq.x2 = str.utm.x;
        str.esq.y2 = str.utm.y;
        // insere Barra em VTMonta
        if ((barra = (VTBarra*)monta->InsereBarraEqpto(str)) == NULL)
            return (false); ;
        // salva Barra na Se
        se->InsereBarra(barra);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TPreMonta::InsereEtd_Barra(sdi_TRAFO_SE &sdi, int barra_id)
{
    // variáveis locais
    AnsiString codigo;
    double vnom_kv;
    strBARRA str;
    VTBarra *barra;

    try
    { // determina tensão nominal da Barra
        if (barra_id == sdi.barra_at)
        {
            vnom_kv = sdi.valta_kv;
            codigo = sdi.codigo + ".at";
        }
        else if (barra_id == sdi.barra_mt1)
        {
            vnom_kv = sdi.vbaixa_kv;
            codigo = sdi.codigo + ".mt1";
        }
        else if (barra_id == sdi.barra_mt2)
        {
            vnom_kv = sdi.vbaixa2_kv;
            codigo = sdi.codigo + ".mt2";
        }
        // cofigura Log
        if (plog)
            plog->DefineValor("Eqpto", "Barra");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // insere uma nova Barra
        str.bar_id = barra_id;
        str.codigo = codigo;
        str.extern_id = "";
        str.vnom = vnom_kv;
        str.utm.x = se->x_m;
        str.utm.y = se->y_m;
        // define coordenadas do esquemático
        str.esq.x1 = str.utm.x;
        str.esq.y1 = str.utm.y;
        str.esq.x2 = str.utm.x;
        str.esq.y2 = str.utm.y;
        // insere Barra em VTMonta
        if ((barra = (VTBarra*)monta->InsereBarraEqpto(str)) != NULL)
        { // salva Barra na Se
            se->InsereBarra(barra);
        }
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        barra = NULL;
    }
    return (barra);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereEtd_Rede(sdi_SE &sdi)
{
    // variáveis locais
    strREDE str_rede;

    try
    { // configura Log
        if (plog)
            plog->DefineValor("Rede", sdi.codigo);
        if (plog)
            plog->DefineValor("Eqpto", "Rede");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // reinicia ponteiro p/ Se e Circuitos correntes
        se = NULL;
        circuito = NULL;
        // reinicia controle de Barra do Monta
        monta->InsereBarraStop();
        monta->InsereBarraStart();
        // proteção: verifica se já existe a Subestacao
        if ((se = ExisteSubestacao(sdi.id)) != NULL)
        {
            if (plog)
                plog->Add("Já existe a subestação");
            return (true);
        }
        // cria um objeto TSe e inicia seus dados
        if ((se = NewObjSe()) == NULL)
        {
            if (plog)
                plog->Add("Impossível criar subestação");
            return (false);
        }
        // inicia objeto Se
        se->id = sdi.id;
        se->codigo = sdi.codigo;
        se->nome = sdi.nome;
        se->x_m = sdi.x_utm_m;
        se->y_m = sdi.y_utm_m;
        // armazena Subestacao em lisSE
        lisSE->Add(se);
        // proteção: verifica se já existe a Rede
        if ((se->rede = (VTRede*)monta->ExisteRede(se->codigo)) != NULL)
        {
            if (plog)
                plog->Add("Já existe a rede da subestação");
            return (true);
        }
        // insere Rede em VTMonta
        str_rede.dominio_id = dominio->Id;
        str_rede.rede_id = sdi.id;
        str_rede.mrede_id = -1;
        str_rede.cluster_id = -1;
        str_rede.barini_id = -1;
        str_rede.color = clBlack;
        str_rede.codigo = sdi.codigo;
        str_rede.tiporede.id = ID_NULO;
        str_rede.tiporede.segmento = redeETD;
        str_rede.tiporede.codigo = "";
        str_rede.estatica = true;
        str_rede.radial = false;
        if ((se->rede = (VTRede*)monta->InsereRede(str_rede)) == NULL)
        {
            if (plog)
                plog->Add("Não foi possível criar a subestação");
            return (false);
        }
    }
    catch (Exception &e)
    { // cancela última Subestacao criada
        se = NULL;
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18236926
bool __fastcall TPreMonta::InsereEtd_Socorro(sdi_SOCORRO &sdi)
{
    // variáveis locais
    strCHAVE str;
    TCircuito *circuito1, *circuito2;
    VTBarra *barra1, *barra2;
    VTLigacao *ligacao;
    VTTrecho *trecho;
    VTRede *rede;

    try
    { // configura log
        if (plog)
            plog->DefineValor("Eqpto", "Socorro");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // proteção
        if (se == NULL)
        {
            if ((se = ExisteSubestacao(sdi.se1_id)) == NULL)
            {
                if ((se = ExisteSubestacao(sdi.se2_id)) == NULL)
                {
                    if (plog)
                        plog->Add("Não existe a subestação das redes primárias");
                    return (true);
                }
            }
        }
        // determina Redes primárias conectadas pelo socorro
        circuito1 = se->ExisteCircuito(sdi.circ1_id);
        circuito2 = se->ExisteCircuito(sdi.circ2_id);
        if ((circuito1 == NULL) && (circuito2 == NULL))
        {
            if (plog)
                plog->Add("Não existem as redes primárias");
            return (true);
        }
        // determina Barras conectadas pelo Socorro
        barra1 = ExisteBarraCircuito(sdi.barra1_id, circuito1, circuito2);
        barra2 = ExisteBarraCircuito(sdi.barra2_id, circuito1, circuito2);

        // barra1 = circuito1->ExisteBarra(sdi.barra1_id);
        // barra2 = circuito1->ExisteBarra(sdi.barra2_id);
        // // verifica se encontrou pelo menos uma Barra no circuito1
        // if ((barra1 != NULL) || (barra2 != NULL))
        // { // assume que a Chave será associada ao circuito1
        // rede = circuito1->rede;
        // }
        // else
        // { // assume que a Chave será associada ao circuito2
        // rede = circuito2->rede;
        // }
        // // tenta localizar as Barras no circuito2
        // if (barra1 == NULL)
        // barra1 = circuito2->ExisteBarra(sdi.barra1_id);
        // if (barra2 == NULL)
        // barra2 = circuito2->ExisteBarra(sdi.barra2_id);

        if ((barra1 == NULL) || (barra2 == NULL))
        {
            if (plog)
                plog->Add("Não existem as barras");
            return (true);
        }

        // assume rede do circuito nao nulo
        if (circuito1 != NULL)
            rede = circuito1->rede;
        else if (circuito2 != NULL)
            rede = circuito2->rede;

        // determina Trecho que interliga as Barras
        ligacao = ExisteLigacaoCircuito(barra1, barra2, circuito1, circuito2);

        // verifica se a chave ja tinha sido inserida
        if ((ligacao != NULL) && (ligacao->Tipo() == eqptoCHAVE))
            return (true);

        if (ligacao == NULL)
        {
            if (plog)
                plog->Add("Não existe o trecho");
        }

        // preenche estrutura strCHAVE
        str.chv_id = sdi.id;
        str.rede_id = rede->Id;
        str.codigo = sdi.codigo;
        // str.extern_id = ConverteCodigoTipoChave(sdi.codigo);//"";
        str.tipo_gis = ConverteCodigoTipoChave(sdi.codigo); // sdi.extern_id;
        str.elo_fusivel = ""; // sdi.tipo_elo;
        str.bar1_id = barra1->Id;
        str.bar2_id = barra2->Id;
        str.estado = chvABERTA;
        str.corn = sdi.inom_a;
        str.telecomandada = ConverteTelecomando(sdi.telecomando);
        str.tecno_comunicacao = comDESCONHECIDO;
        str.tipochave.id = -1;
        str.tipochave.tipo = ConverteTipoChave(sdi.codigo);
        str.tipochave.codigo = "";
        str.esq.posbar1 = 50.;
        str.esq.posbar2 = 50.;
        // insere Chave em VTMonta
        if (monta->InsereChave(str) == NULL)
            return (true);
        // retira Ligacao da rede e destrói o objeto
        if (ligacao != NULL)
        {
            ligacao->rede->RemoveLigacao(ligacao);
            // retira da lista de trechos com arranjo errado, se estiver lá
            lisTRECHO_ALT->Remove(ligacao);
            DLL_DeleteEqpto(ligacao);
        }
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereEtd_Suprimento(sdi_TRAFO_SE &sdi)
{
    // variáveis locais
    int ind_pat;
    strSUPRIMENTO str;
    VTSuprimento *suprimento;

    try
    { // configura Resumo
        if (plog)
            plog->DefineValor("Eqpto", "Suprimento");
        str.sup_id = FalsoId();
        str.bar_id = sdi.barra_at;
        str.codigo = "";
        str.extern_id = "";
        str.tiposup = supVF;
        str.smax = 4 * sdi.snom_mva;
        str.vnom_kv = sdi.valta_kv;
        str.zeq0.r = 0;
        str.zeq0.x = 0;
        str.zeq1.r = 0;
        str.zeq1.x = 0;
        str.pcc_3f.p = 0;
        str.pcc_3f.q = 600;
        str.pcc_ft.p = 0;
        str.pcc_ft.q = 400;
        // define um patamar único
        str.curva.id = -1;
        str.curva.tipica = false;
        str.curva.num_pat = 1;
        str.curva.num_valor = nvCURVA_SUP;
        ind_pat = 0;
        str.curva.patamar[ind_pat].hm_ini.hora = 0;
        str.curva.patamar[ind_pat].hm_ini.minuto = 0;
        str.curva.patamar[ind_pat].hm_fim.hora = 24;
        str.curva.patamar[ind_pat].hm_fim.minuto = 0;
        str.curva.patamar[ind_pat].valor[indP] = 0;
        str.curva.patamar[ind_pat].valor[indQ] = 0;
        str.curva.patamar[ind_pat].valor[indV] = 1;
        str.curva.patamar[ind_pat].valor[indT] = 0;
        // define posição na Barra
        str.esq.posbar = POSBAR;
        str.esq.dxbar = DXBAR;
        str.esq.dybar = DYBAR;
        // insere Suprimento em Monta
        if ((suprimento = (VTSuprimento*)monta->InsereSuprimento(str)) == NULL)
            return (false);
        // associa o Suprimento com a Se
        // se->suprimento = suprimento;

    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereEtd_Suprimento(VTBarra *barra)
{
    // variáveis locais
    int ind_pat;
    strSUPRIMENTO str;
    VTSuprimento *suprimento;

    try
    { // configura Resumo
        if (plog)
            plog->DefineValor("Eqpto", "Suprimento");
        str.sup_id = FalsoId();
        str.bar_id = barra->Id;
        str.codigo = "";
        str.extern_id = "";
        str.tiposup = supVF;
        str.smax = 100;
        str.vnom_kv = barra->vnom;
        str.zeq0.r = 0;
        str.zeq0.x = 0;
        str.zeq1.r = 0;
        str.zeq1.x = 0;
        str.pcc_3f.p = 0;
        str.pcc_3f.q = 600;
        str.pcc_ft.p = 0;
        str.pcc_ft.q = 400;
        // define um patamar único
        str.curva.id = -1;
        str.curva.tipica = false;
        str.curva.num_pat = 1;
        str.curva.num_valor = nvCURVA_SUP;
        ind_pat = 0;
        str.curva.patamar[ind_pat].hm_ini.hora = 0;
        str.curva.patamar[ind_pat].hm_ini.minuto = 0;
        str.curva.patamar[ind_pat].hm_fim.hora = 24;
        str.curva.patamar[ind_pat].hm_fim.minuto = 0;
        str.curva.patamar[ind_pat].valor[indP] = 0;
        str.curva.patamar[ind_pat].valor[indQ] = 0;
        str.curva.patamar[ind_pat].valor[indV] = 1;
        str.curva.patamar[ind_pat].valor[indT] = 0;
        // define posição na Barra
        str.esq.posbar = POSBAR;
        str.esq.dxbar = DXBAR;
        str.esq.dybar = DYBAR;
        // insere Suprimento em Monta
        if ((suprimento = (VTSuprimento*)monta->InsereSuprimento(str)) == NULL)
            return (false);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereEtd_Trafo(sdi_TRAFO_SE &sdi)
{
    // variáveis locais
    bool sucesso;
    VTBarra *bar_at;

    try
    { // configura Log
        if (plog)
            plog->DefineValor("Eqpto", "Trafo");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // proteção: verifica se existe a Se
        if ((sdi.se_id == NULL) || (se->id != sdi.se_id))
        {
            if (plog)
                plog->Add("Trafo não está associado a ETD");
            return (true);
        }
        // verifica potência nominal
        if (IsDoubleZero(sdi.snom_vent_mva))
        {
            if (plog)
                plog->Add("Trafo de ETD c/ potência ventilada nula");
            if (IsDoubleZero(sdi.snom_mva))
            {
                if (plog)
                    plog->Add("Trafo de ETD c/ potência nominal nula");
                sdi.snom_mva = 100.;
            }
            else
            {
                sdi.snom_mva = sdi.snom_mva / 1000.;
            }
        }
        else
        {
            sdi.snom_mva = sdi.snom_vent_mva;
        }

        // verifica tensão do primário
        if (IsDoubleZero(sdi.valta_kv))
        {
            if (plog)
                plog->Add("Tensão nominal primária nula");
            sdi.valta_kv = 88.0;
        }
        // verifica tensão do secundário
        if (IsDoubleZero(sdi.vbaixa_kv))
        {
            if (plog)
                plog->Add("Tensão nominal secundária nula");
            sdi.vbaixa_kv = 13.8;
        }
        // verifica tensão do terciário, caso tenha sido definida a Barra
        if (sdi.barra_mt2 > 0)
        {
            if (IsDoubleZero(sdi.vbaixa2_kv))
            {
                if (plog)
                    plog->Add("Tensão nominal terciária nula");
                sdi.vbaixa2_kv = sdi.vbaixa_kv;
            }
        }
        // verifica se existe a Barra do primário
        if (se->ExisteBarra(sdi.barra_at) == NULL)
        { // verifica se existe uma Barra com mesma tensão
            // if ((bar_at = se->ExisteBarra(sdi.valta_kv)) != NULL)
            // { // redefine sdi.barra_at
            // sdi.barra_at = bar_at->Id;
            // }
            // else
            // { // cria Barra para o primário
            if (InsereEtd_Barra(sdi, sdi.barra_at) == NULL)
                return (false);
            // insere Suprimento na Barra do primário
            InsereEtd_Suprimento(sdi);
            // }
        }
        // verifica se existe a Barra do secundário
        if (se->ExisteBarra(sdi.barra_mt1) == NULL)
        { // cria Barra
            if (!InsereEtd_Barra(sdi, sdi.barra_mt1))
                return (false);
        }
        // verifica se foi definida Barra do terciário
        if (sdi.barra_mt2 > 0)
        { // verifica se existe a Barra do terciário
            if (se->ExisteBarra(sdi.barra_mt2) == NULL)
            { // cria Barra
                if (!InsereEtd_Barra(sdi, sdi.barra_mt2))
                    return (false);
            }
        }
        // verifica se é um trafo de 2 ou 3 enrolamentos
        if (sdi.barra_mt2 > 0)
            sucesso = InsereEtd_Trafo3E(sdi);
        else
            sucesso = InsereEtd_Trafo2E(sdi);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        sucesso = false;
    }
    return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereEtd_Trafo2E(sdi_TRAFO_SE &sdi)
{
    // variáveis locais
    bool sucesso;
    strTRAFO str;
    VTTrafo *trafo;
    double zconversao = 0;

    try
    { // configura Log
        if (plog)
            plog->DefineValor("Eqpto", "Trafo");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // Converte de base as impedâncias (elas estão calculadas
        // na baixa do trafo e 100 MVA
        zconversao = sdi.snom_mva / 100;
        sdi.r0_pu = sdi.r0_pu * zconversao;
        sdi.x0_pu = sdi.x0_pu * zconversao;
        sdi.r1_pu = sdi.r1_pu * zconversao;
        sdi.x1_pu = sdi.x1_pu * zconversao;
        // Proteção no caso da impedância ser zero ou resistência ter um valor não admissível
        // valida z0
        if ((sdi.r0_pu < 0.) || (sdi.x0_pu <= 0.))
        {
            if (plog)
                plog->Add("Trafo de ETD c/ Z0 fora da faixa admissível");
            sdi.r0_pu = 0.0;
            sdi.x0_pu = 0.1;
        }
        // valida z1
        if ((sdi.r1_pu < 0.) || (sdi.x1_pu <= 0.))
        {
            if (plog)
                plog->Add("Trafo de ETD c/ Z1 fora da faixa admissível");
            sdi.r1_pu = 0.0;
            sdi.x1_pu = 0.1;
        }
        // // valida z0
        // if ((sdi.r0_pu > 0.08) || (sdi.x0_pu > 0.1)) {
        // if (plog)
        // plog->Add("Trafo de ETD c/ Z0 fora da faixa");
        // sdi.r0_pu = 0.002;
        // sdi.x0_pu = 0.03;
        // }
        // // valida z1
        // if ((sdi.r1_pu > 0.08) || (sdi.x1_pu > 0.1)) {
        // if (plog)
        // plog->Add("Trafo de ETD c/ Z1 fora da faixa");
        // sdi.r1_pu = 0.002;
        // sdi.x1_pu = 0.03;
        // }

        // insere Trafo na Rede
        str.trf_id = sdi.id;
        str.rede_id = se->rede->Id;
        str.bar1_id = sdi.barra_at;
        str.bar2_id = sdi.barra_mt1;
        str.codigo = sdi.codigo;
        str.extern_id = sdi.extern_id;
        str.snom_mva = sdi.snom_mva;
        str.z0.r = sdi.r0_pu;
        str.z0.x = sdi.x0_pu;
        str.z1.r = sdi.r1_pu;
        str.z1.x = sdi.x1_pu;
        str.perda_ferro = ((sdi.perda_fe_kw * 0.001) / sdi.snom_mva) * 100.;
        // primário
        str.pri.fases = faseABC;
        str.pri.ligacao = lgTRIANG;
        str.pri.vnom = sdi.valta_kv;
        str.pri.tap = sdi.valta_kv;
        str.pri.defasagem = 0;
        str.pri.zterra.r = 0;
        str.pri.zterra.x = 0;
        // secundário
        str.sec.fases = faseABCNT;
        str.sec.ligacao = lgEST_AT;
        str.sec.vnom = sdi.vbaixa_kv;
        str.sec.tap = sdi.vbaixa_kv;
        str.sec.defasagem = -30.;
        str.sec.zterra.r = 0;
        str.sec.zterra.x = 0;
        // dados de LTC
        str.ltc.tipo = ltcFIXO;
        str.ltc.bar_id = ID_NULO;
        str.ltc.v_pu = 1.0;
        // posição nas barras
        str.esq.posbar1 = 50.;
        str.esq.posbar2 = 50.;
        if ((trafo = (VTTrafo*)monta->InsereTrafo(str)) == NULL)
            return (false);
        // salva Trafo na Se
        se->InsereTrafo(trafo);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereEtd_Trafo3E(sdi_TRAFO_SE &sdi)
{
    // variáveis locais
    bool sucesso;
    double z_pu, r_pu, x_pu, k;
    strTRAFO3E str;
    VTTrafo3E *trafo3E;

    try
    { // configura Log
        if (plog)
            plog->DefineValor("Eqpto", "Trafo3E");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // calcula impedâncias de seq1 entre os enrolamentos
        CalculaImpedancia(sdi.z12, sdi.xr12, str.zps1);
        CalculaImpedancia(sdi.z13, sdi.xr13, str.zpt1);
        CalculaImpedancia(sdi.z23, sdi.xr23, str.zst1);
        // calcula impedâncias de seq0 entre os enrolamentos
        CalculaImpedancia(sdi.z012, sdi.xr012, str.zps0);
        CalculaImpedancia(sdi.z013, sdi.xr013, str.zpt0);
        CalculaImpedancia(sdi.z023, sdi.xr023, str.zst0);
        // insere Trafo3E na Rede
        str.trf_id = sdi.id;
        str.rede_id = se->rede->Id;
        str.bar1_id = sdi.barra_at;
        str.bar2_id = sdi.barra_mt1;
        str.bar3_id = sdi.barra_mt2;
        str.codigo = sdi.codigo;
        str.extern_id = sdi.extern_id;
        str.perda_ferro = ((sdi.perda_fe_kw * 0.001) / sdi.snom_mva) * 100.;
        // primário
        str.pri.fases = faseABC;
        str.pri.ligacao = lgTRIANG;
        str.pri.snom = sdi.snom_mva;
        str.pri.vnom = sdi.valta_kv;
        str.pri.tap = sdi.valta_kv;
        str.pri.defasagem = 0;
        str.pri.zterra.r = 0;
        str.pri.zterra.x = 0;
        // secundário
        str.sec.fases = faseABCNT;
        str.sec.ligacao = lgEST_AT;
        str.sec.snom = sdi.snom_mva * 0.5;
        str.sec.vnom = sdi.vbaixa_kv;
        str.sec.tap = sdi.vbaixa_kv;
        str.sec.defasagem = -30.;
        str.sec.zterra.r = 0;
        str.sec.zterra.x = 0;
        // terciário
        str.ter.fases = faseABCNT;
        str.ter.ligacao = lgEST_AT;
        str.ter.snom = sdi.snom_mva * 0.5;
        str.ter.vnom = sdi.vbaixa2_kv;
        str.ter.tap = sdi.vbaixa2_kv;
        str.ter.defasagem = -30.;
        str.ter.zterra.r = 0;
        str.ter.zterra.x = 0;
        // posição nas barras
        str.esq.posbar1 = 50.;
        str.esq.posbar2 = 50.;
        str.esq.posbar3 = 50.;
        if ((trafo3E = (VTTrafo3E*)monta->InsereTrafo3E(str)) == NULL)
            return (false);
        // salva Trafo3E na Se
        se->InsereTrafo3E(trafo3E);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_Barra(sdi_BARRA &sdi)
{
    // variáveis locais
    strBARRA str;
    VTBarra *barra = NULL;

    try
    { // atualiza log
        if (plog)
            plog->DefineValor("Eqpto", "Barra");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // proteção
        if (circuito == NULL)
        {
            if (plog)
                plog->Add("Não existe rede primária ativa");
            return (false);
        }
        // cria uma nova Barra
        str.bar_id = sdi.id;
        str.codigo = sdi.codigo;
        str.vnom = circuito->vnom_kv;
        str.utm.x = sdi.x_utm_m * 100;
        str.utm.y = sdi.y_utm_m * 100;
        // esquemático
        str.esq.x1 = str.utm.x;
        str.esq.y1 = str.utm.y;
        str.esq.x2 = str.utm.x;
        str.esq.y2 = str.utm.y;
        // insere Barra em VTMonta
        if ((barra = (VTBarra*)monta->InsereBarraEqpto(str)) == NULL)
            return (false);
        // salva Barra no Circuito
        circuito->InsereBarra(barra);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TPreMonta::InserePri_BarraFicticia(VTBarra *bar_ref)
{
    // variáveis locais
    strBARRA str;
    VTBarra *barra;

    try
    { // cria uma nova Barra
        str.bar_id = FalsoId();
        str.codigo = "";
        str.extern_id = "";
        str.vnom = bar_ref->vnom;
        str.utm.x = bar_ref->utm.x;
        str.utm.y = bar_ref->utm.y + 500;
        // esquemático
        str.esq.x1 = str.utm.x;
        str.esq.y1 = str.utm.y;
        str.esq.x2 = str.utm.x;
        str.esq.y2 = str.utm.y;
        // insere Barra em VTMonta
        // monta->InsereBarraStart();
        barra = (VTBarra*)monta->InsereBarraEqpto(str);
        // monta->InsereBarraStop();
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        barra = NULL;
    }
    return (barra);
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TPreMonta::InserePri_BarraIniFicticia(double vnom, int id)
{
    // variáveis locais
    strBARRA str;
    VTBarra *barra;

    try
    { // cria uma nova Barra
        if (id < 0)
        {
            str.bar_id = FalsoId();
            str.codigo = "";
        }
        else
        {
            str.bar_id = id;
            str.codigo = IntToStr(id);
        }
        str.extern_id = "";
        str.vnom = vnom;
        str.utm.x = se->x_m;
        str.utm.y = se->y_m;
        // esquemático
        str.esq.x1 = str.utm.x;
        str.esq.y1 = str.utm.y;
        str.esq.x2 = str.utm.x;
        str.esq.y2 = str.utm.y;
        // insere Barra em VTMonta
        // monta->InsereBarraStart();
        barra = (VTBarra*)monta->InsereBarraEqpto(str);
        // monta->InsereBarraStop();
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        barra = NULL;
    }
    return (barra);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_Capacitor(sdi_CAPACITOR &sdi)
{
    // variáveis locais
    strCAPACITOR str;

    try
    { // cofigura Log
        if (plog)
            plog->DefineValor("Eqpto", "Capacitor");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // verifica se o Capacitor série pertence ao Circuito selecionado
        if ((circuito == NULL) || (circuito->rede->Id != sdi.circ_id))
        {
            if (plog)
                plog->Add("Não está associado a rede primária");
            return (true);
        }
        // preenche estrutura strCAPACITOR
        str.cap_id = sdi.id;
        str.codigo = sdi.codigo;
        str.bar_id = sdi.barra_id;
        str.ligacao = lgEST_ISO; // lgTRIANG;
        str.vnom = circuito->vnom_kv;
        str.q_mvar = sdi.qnon_kvar * 0.001;
        str.esq.posbar = 50.;
        str.esq.dxbar = 0;
        str.esq.dybar = -DYBAR;
        // insere Capacitor série em VTMonta
        if (monta->InsereCapacitor(str) == NULL)
            return (false);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18236926
bool __fastcall TPreMonta::InserePri_Chave(sdi_CHAVE &sdi)
{
    // variáveis locais
    strCHAVE str;
    VTTrecho *trecho, *link;

    try
    { // cofigura Log
        if (plog)
            plog->DefineValor("Eqpto", "Chave");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // verifica se a Chave pertence ao Circuito selecionado
        if ((circuito == NULL) || (circuito->rede->Id != sdi.id_rede_mt))
        {
            if (plog)
                plog->Add("Não está associada a rede primária");
            return (true);
        }
        // determina Trecho correspondente à Chave
        if ((trecho = circuito->ExisteTrecho(sdi.id_barra1, sdi.id_barra2)) == NULL)
        {
            if (plog)
                plog->Add("Não existe  trecho associado");
            return (true);
        }
        // verifica se o Trecho possui um Link associado
        link = NULL;
        if (trecho->Obj != NULL)
        { // a Chave deve substituir o Link do Trecho original
            link = (VTTrecho*)trecho->Obj;
        }

        // preenche estrutura strCHAVE
        str.chv_id = sdi.id;
        str.rede_id = circuito->rede->Id;
        str.codigo = sdi.codigo;
        // str.extern_id = ConverteCodigoTipoChave(sdi.codigo);//sdi.extern_id;
        str.tipo_gis = ConverteCodigoTipoChave(sdi.codigo);
        // sdi.extern_id;
        str.elo_fusivel = sdi.tipo_elo;
        str.bar1_id = (link != NULL) ? link->pbarra1->Id : trecho->pbarra1->Id;
        str.bar2_id = (link != NULL) ? link->pbarra2->Id : trecho->pbarra2->Id;
        str.estado = sdi.estado;
        str.corn = sdi.inom_a;
        str.tipochave.id = -1;
        str.tipochave.tipo = ConverteTipoChave(sdi.codigo);
        str.tipochave.codigo = "";
        str.telecomandada = ConverteTelecomando(sdi.telecomando);
        str.tecno_comunicacao = comDESCONHECIDO;
        str.esq.posbar1 = 50.;
        str.esq.posbar2 = 50.;
        // insere Chave em VTMonta
        if (monta->InsereChave(str) == NULL)
            return (true);
        if (link)
        {
            // retira o link da rede
            circuito->rede->RemoveLigacao(link);
            DLL_DeleteEqpto(link);
        }
        // retira da lista de trechos com arranjo errado, se estiver lá
        lisTRECHO_ALT->Remove(trecho);
        // destrói Trecho
        circuito->DeleteTrecho(trecho);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18497315
bool __fastcall TPreMonta::InserePri_Consumidor(sdi_CONS_MT &sdi)
{
    // variáveis locais
    int fases_tag;
    strCARGA str;
    VTBarra *barra;

    try
    { // configura Log
        if (plog)
            plog->DefineValor("Eqpto", "Cons.MT");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // proteção
        // if ((circuito == NULL)||(circuito->id != sdi.circ_id))
        if (circuito == NULL)
        {
            if (plog)
                plog->Add("Não está associado a circuito");
            return (true);
        }
        // verifica se as fases do Consumidor estão definidas
        if (sdi.fases.IsEmpty())
        { // assume fases ABC (trafo em delta)
            if (plog)
                plog->Add("Fases indefinidas");
            sdi.fases = "ABC";
        }
        // determina barra
        barra = monta->ExisteBarra(sdi.barra_id);
        if(barra != NULL)
        {//assume código da EP
            barra->Codigo = sdi.codigo;
        }
        // preenche estrutura strCARGA
        str.car_id = sdi.id;
        str.bar_id = sdi.barra_id;
        str.codigo = IntToStr(sdi.id);//sdi.codigo;
        str.classe_cons = ConverteClasseConsumidor(sdi.classe_cons);
        // consA4;
        str.grupoA = true;
        str.fases = fases->ExisteFase(sdi.fases);
        // modelo de carga Icte
        str.modelo.icte_pu = 1;
        str.modelo.scte_pu = 0;
        str.modelo.zcte_pu = 0;
        // consumidor definido por energia
        str.energia_mwhmes = sdi.consumo_kwh_mes * 0.001;
        str.curva.id = -1;
        str.curva.tipica = true;
        // preenche a curva própria também
        str.curva.num_pat = pat01HORA;
        str.curva.num_valor = nvCURVA_PQ;
        // define valores de demanda em cada ponto da curva
        for (int np = 0; np < pat01HORA; np++)
        { // define hora inicial e final do patamar
            str.curva.patamar[np].hm_ini = pat_edp[np].hm_ini;
            str.curva.patamar[np].hm_fim = pat_edp[np].hm_fim;
            // define valore de ponta/fora de ponta
            if (pat_edp[np].ponta)
            { // demanda de ponta
                str.curva.patamar[np].valor[indP] = sdi.p_ponta_kw * 0.001;
                str.curva.patamar[np].valor[indQ] = sdi.q_ponta_kvar * 0.001;
            }
            else
            { // demanda fora de ponta
                str.curva.patamar[np].valor[indP] = sdi.p_fponta_kw * 0.001;
                str.curva.patamar[np].valor[indQ] = sdi.q_fponta_kvar * 0.001;
            }
        }
        // cancela dados de IP
        str.ip.num_lamp = 0.;
        str.ip.pot_kw = 0.;
        str.ip.fat_pot = 1.;
        // esquemático
        str.esq.posbar = 50.;
        str.esq.dxbar = 0;
        str.esq.dybar = -DYBAR;
        // insere Carga em VTMonta
        if (!monta->InsereCarga(str))
            return (false);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_DisjuntorFicticio(void)
{
    // variáveis locais
    strCHAVE str;
    TCircuito *circuito;
    TSe *se;
    VTChave *chave;
    VTLigacao *ligacao;
    VTBarra *bar_ini;
    VTBarra *bar_dj;

    // loop para todas Se
    for (int i = 0; i < lisSE->Count; i++)
    {
        se = (TSe*)lisSE->Items[i];
        // loop para todos Circuitos da Se
        for (int j = 0; j < se->lisCIRCUITO->Count; j++)
        {
            circuito = (TCircuito*)se->lisCIRCUITO->Items[j];
            // determina Barra inicial da Rede
            if ((bar_ini = circuito->rede->BarraInicial()) == NULL)
                continue;
            // cria uma nova Barra conectar o disjuntor
            if ((bar_dj = InserePri_BarraFicticia(bar_ini)) == NULL)
                continue;
            // cria uma Chave do tipo DJ
            str.chv_id = FalsoId();
            str.rede_id = circuito->rede->Id;
            str.codigo = "DJ." + circuito->rede->Codigo;
            str.extern_id = "";
            str.bar1_id = bar_ini->Id;
            str.bar2_id = bar_dj->Id;
            str.estado = chvFECHADA;
            str.corn = 600;
            str.tipochave.id = -1;
            str.tipochave.tipo = chaveDJ;
            str.tipochave.codigo = "";
            str.esq.posbar1 = 50.;
            str.esq.posbar2 = 50.;
            // insere Chave em VTMonta
            if ((chave = (VTChave*)monta->InsereChave(str)) == NULL)
                continue;
            // redefine barra das Ligacoes do primário que conectavam bar_ini
            lisEQP->Clear();
            circuito->rede->LisLigacao(lisEQP, bar_ini);
            for (int k = 0; k < lisEQP->Count; k++)
            {
                ligacao = (VTLigacao*)lisEQP->Items[k];
                // proteção: não altera as barras do disjuntor que foi criado agora
                if (ligacao == chave)
                    continue;
                if (ligacao->pbarra1 == bar_ini)
                    ligacao->pbarra1 = bar_dj;
                else if (ligacao->pbarra2 == bar_ini)
                    ligacao->pbarra2 = bar_dj;
                else if (ligacao->pbarra3 == bar_ini)
                    ligacao->pbarra3 = bar_dj;
            }
        }
    }
    return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18497403
bool __fastcall TPreMonta::InserePri_Gerador(sdi_GERADOR &sdi)
{
    // variáveis locais
    double s_mva, p_mw, q_mvar;
    strGERADOR str;
    VTBarra *barra1, *barra2, *barraEscolhida;
    VTCurva *curva;
    VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

    barraEscolhida = NULL;
    try
    { // configura Resumo
        if (plog)
            plog->DefineValor("Eqpto", "Gerador");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // proteção
        if ((circuito == NULL) || (circuito->id != sdi.circ_id))
        {
            if (plog)
                plog->Add("Não está associado a circuito");
            return (true);
        }
        // inicia estrutura strGerador
        str.obra = false;
        str.ger_id = sdi.id;
        // str.bar_id = sdi.barra_id;
        // teste
        // verifica as barras sugeridas
        barra1 = monta->ExisteBarra(sdi.barra_id1);
        barra2 = monta->ExisteBarra(sdi.barra_id2);
        if (barra1 != NULL)
        { // caso exista e nao seja isolada
            if (!IsBarraIsolada(barra1))
            {
                str.bar_id = sdi.barra_id1;
                barra1->Codigo = sdi.cdc;
            }
        }
        if (barra2 != NULL)
        { // caso exista e nao seja isolada
            if (!IsBarraIsolada(barra2))
            {
                str.bar_id = sdi.barra_id2;
                barra2->Codigo = sdi.cdc;
            }
        }
        // caso ambas sejam nulas
        if ((barra1 == NULL) && (barra2 == NULL))
        { // deixa o monta lidar com a barra inexistente
            str.bar_id = sdi.barra_id1;
        }
        str.codigo = sdi.codigo;
        str.extern_id = "";
        str.tiposup = supPV; // (sdi.tipo_sup == 0) ? supPQ : supPV;
        str.tipo_geracao = ConverteTipoGeracao(sdi.tipo_geracao);//gerPCH;
        str.ligacao = lgEST_AT;
        str.fases = faseABC;
        str.vnom = circuito->vnom_kv;
        str.snom_mva = sdi.snom_kva * 0.001;
        str.smax_mva = str.snom_mva * 1.2;
        str.fplim = 0.9;
        str.z0.r = sdi.r0_pu;
        str.z0.x = sdi.x0_pu;
        str.z1.r = sdi.r1_pu;
        str.z1.x = sdi.x1_pu;
        // define pot.ativa e reativa a partir de Snom e considerando fatpot = 0.92
        s_mva = str.snom_mva;
        p_mw = s_mva * 0.92;
        q_mvar = sqrt((s_mva * s_mva) - (p_mw * p_mw));
        //se for eolica ou fotovoltaica, adota curva tipica
        if(str.tipo_geracao == gerSOLAR)
        {//determina curva do mes de extração
            curva = curvas->ExisteCurvaTipicaGeradorFotovoltaicoMes(MesExtracao);
            if(curva != NULL)
            {
                str.curva.id = curva->Id;
                str.curva.tipica = true;
            }
        }
        else if(str.tipo_geracao == gerEOLICA)
        {//determina curva eolica de SC
            curva = curvas->ExisteCurvaTipicaGeradorFotovoltaicoId(275);
            if(curva != NULL)
            {
                str.curva.id = curva->Id;
                str.curva.tipica = true;
            }
        }
        else
        {// define curva de geração própria
            str.curva.id = -1;
            str.curva.tipica = false;
            str.curva.num_pat = pat01HORA;
            str.curva.num_valor = nvCURVA_PQVT;

            for (int np = 0; np < pat01HORA; np++)
            { // define hora inicial e final do patamar
                str.curva.patamar[np].hm_ini = pat_edp[np].hm_ini;
                str.curva.patamar[np].hm_fim = pat_edp[np].hm_fim;
                // define valores de ponta/fora de ponta
                if (pat_edp[np].ponta)
                { // demanda de ponta
                    str.curva.patamar[np].valor[indP] = p_mw;
                    str.curva.patamar[np].valor[indQ] = q_mvar;
                    str.curva.patamar[np].valor[indV] = 1.;
                    str.curva.patamar[np].valor[indT] = 0.;
                }
                else
                { // demanda fora de ponta
                    str.curva.patamar[np].valor[indP] = p_mw;
                    str.curva.patamar[np].valor[indQ] = q_mvar;
                    str.curva.patamar[np].valor[indV] = 1.;
                    str.curva.patamar[np].valor[indT] = 0.;
                }
            }
        }
        // define coordenadas p/ esquemático
        str.esq.dxbar = -2000;
        str.esq.dybar = 0;
        // insere Gerador em VTMonta
        if (!monta->InsereGerador(str))
            return (false);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_Link(sdi_LINK &sdi)
{
    // variáveis locais
    AnsiString codigo;
    strTRECHO str_tre;
    VTArranjo *arranjo;
    VTBarra *barra1, *barra2;
    VTTrecho *trecho, *link;
    VTSuporte *suporte;
    bool trechoCurto = false;

    try
    { // configura Log
        if (plog)
            plog->DefineValor("Eqpto", "Link MT");
        if (plog)
            plog->DefineValor("Código", "trecho_id=" + IntToStr(sdi.trecho_id));
        // verifica se o Trecho pertence ao Circuito selecionado
        if (circuito == NULL)
        {
            if (plog)
                plog->Add("Não está associado a rede primária");
            return (true);
        }
        // determina Trecho associado
        if ((trecho = circuito->ExisteTrecho(sdi.trecho_id)) == NULL)
        {
            if (plog)
                plog->Add("Não está associado a um Trecho");
            return (true);
        }
        // verifica se Barras iguais

        if (sdi.barra_id1 == sdi.barra_id2)
        { // não cria trecho com barras iguais
            if (plog)
                plog->Add("Barra inicial igual à barra final");
            return (true);
        }
        // proteção: troca o o ID das barras se for igual ao ID da Barra inicial indicada pelo GIS
        if (sdi.barra_id1 == circuito->GIS_bar_ini_id)
            sdi.barra_id1 = circuito->barra_ini->Id;
        else if (sdi.barra_id2 == circuito->GIS_bar_ini_id)
            sdi.barra_id2 = circuito->barra_ini->Id;
        // verifica se existem as Barras
        barra1 = monta->ExisteBarra(sdi.barra_id1);
        barra2 = monta->ExisteBarra(sdi.barra_id2);
        if ((barra1 == NULL) || (barra2 == NULL))
        {
            if (plog)
                plog->Add("Não existem as barras");
            return (true);
        }
        // preenche estrutura strTRECHO
        str_tre.tre_id = trecho->Id;
        str_tre.rede_id = circuito->rede->Id;
        str_tre.extern_id = "";
        str_tre.codigo = trecho->Codigo;
        str_tre.bar1_id = barra1->Id;
        str_tre.bar2_id = barra2->Id;
        str_tre.fases = trecho->arranjo->Fases;
        str_tre.comp_km = Distancia_cm(barra1->utm.x, barra1->utm.y, barra2->utm.x,
            barra2->utm.y) * 1e-5;

        if (IsDoubleZero(str_tre.comp_km))
        { // verifica se comprimento nulo
            trechoCurto = true;
            str_tre.comp_km = 0.001;
        }
        else if (str_tre.comp_km < 0.001)
        { // verifica se comprimento é curto
            trechoCurto = true;
            str_tre.comp_km = 0.001;
        }

        str_tre.esq.posbar1 = 50.;
        str_tre.esq.posbar2 = 50.;
        // insere Trecho com o Arranjo definido
        link = (VTTrecho*)monta->InsereTrecho(str_tre, barra1, barra2, trecho->arranjo);
        // proteção
        if (link == NULL)
        {
            return (true); // return(false);
        }
        else
        {
            link->Status[sttTRECHOCURTO] = trechoCurto;
        }
        // associa Trecho original com o Trecho criado a partir do seu primeiro Link
        if (trecho->Obj == NULL)
            trecho->Obj = link;
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/index.cfm#tasks/15031223
bool __fastcall TPreMonta::InserePri_Rede(sdi_CIRCUITO &sdi)
{
    // variáveis locais
    strREDE str;
    VTRede *rede;
    VTTrafo *trafo = NULL;
    VTTrafo *trafo_aux = NULL;
    VTTrafo3E *trafo3E = NULL;
    VTBarra *barra, *barraAux;

    try
    { // configura Log
        if (plog)
            plog->DefineValor("Rede", sdi.codigo);
        if (plog)
            plog->DefineValor("Eqpto", "Rede MT");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // reinicia ponteiro p/ Circuito corrente
        circuito = NULL;
        // reinicia controle de Barra do Monta
        monta->InsereBarraStop();
        monta->InsereBarraStart();
        barra = NULL;
        // proteção: verifica se existe uma Subestacao
        if (se == NULL)
        {
            if (plog)
                plog->Add("Primário não está associado a ETD");
            return (false);
        }
        // proteção: verifica se já existe um Circuito c/ mesmo ID
        if (se->ExisteCircuito(sdi.id))
        {
            if (plog)
                plog->Add("ID repetido");
            return (false);
        }
        // proteção: verifica se existe uma Rede com o mesmo código
        if (monta->ExisteRede(sdi.codigo) != NULL)
        {
            if (plog)
                plog->Add("Primário já existente");
            return (false);
        }
        // cria um objeto Circuito e inicia seus dados
        if ((circuito = NewObjCircuito()) == NULL)
        {
            if (plog)
                plog->Add(ERRO_DE_SISTEMA);
            return (false);
        }
        // atualiza Circuito
        circuito->id = sdi.id;
        circuito->codigo = sdi.codigo;
        circuito->nome = sdi.nome;
        circuito->vnom_kv = sdi.vnom_kv;
        // proteção: verifica se existe Trafo ou Trafo3E associado ao Circuito
        if ((trafo = se->ExisteTrafo(sdi.trafo_se_id)) == NULL)
        {
            if ((trafo3E = se->ExisteTrafo3E(sdi.trafo_se_id)) == NULL)
            {
                if (plog)
                    plog->Add("Primário não está associado a trafo");
                // assume um Trafo qualquer desde que com mesma tensão
                if ((trafo = se->ExisteTrafo(sdi.vnom_kv)) != NULL)
                {
                    if (plog)
                        plog->Add("Primário transferido p/ trafo " + trafo->Codigo);
                }
                else if ((trafo3E = se->ExisteTrafo3E(sdi.vnom_kv)) != NULL)
                {
                    if (plog)
                        plog->Add("Primário transferido p/ trafo3E " + trafo3E->Codigo);
                }
                else
                {
                    // if (plog)
                    // plog->Add("Primário descartado");
                    // return (false);
                    if ((barra = se->ExisteBarra(sdi.vnom_kv)) == NULL)
                    {
                        barra = InserePri_BarraIniFicticia(sdi.vnom_kv, sdi.barra_mt_id);
                        se->InsereBarra(barra);
                    }

                }
            }
        }
        else
        {
            if (!IsDoubleZero(trafo->sec.vnom - circuito->vnom_kv))
            { // assume um Trafo qualquer desde que com mesma tensão
                trafo_aux = trafo;
                if ((trafo = se->ExisteTrafo(sdi.vnom_kv)) != NULL)
                {
                    if (plog)
                        plog->Add("Primário transferido p/ trafo " + trafo->Codigo);
                }
                else
                { // solução proposta: altera tensão do circuito
                    trafo = trafo_aux;
                    // if (plog)
                    // plog->Add("Tensão do primário alterada p/ " + DoubleToStr("%3.2f", trafo->sec.vnom) + " kV");
                    //
                    // circuito->vnom_kv = trafo->sec.vnom;
                    // interromper importação
                    if (plog)
                        plog->Add("Tensão do primário " + DoubleToStr("%3.2f", circuito->vnom_kv) +
                        " kV incompatível " "com a tensão do secundário do transformador " +
                        DoubleToStr("%3.2f", trafo->sec.vnom) + " kV");
                    return false;
                }
            }
        }

        // associa trafo ao circuito
        if (trafo != NULL)
            circuito->trafo = trafo;
        if (trafo3E != NULL)
            circuito->trafo3E = trafo3E;
        // salva bar_ini_id recebido no GIS
        circuito->GIS_bar_ini_id = sdi.barra_mt_id;
        // determina Barra da Se que conecta o Circuito
        if ((circuito->barra_ini = se->ExisteBarra(sdi.barra_mt_id)) == NULL)
        { // Não existe a Barra indicada em sdi.barra_mt_id. Assume barra do Trafo
            if (plog)
                plog->Add("Não existe a barra inicial");
            if (trafo)
                circuito->barra_ini = trafo->BarraSecundario();
            else if (trafo3E)
            {
                if (IsDoubleZero(trafo3E->ter.vnom - circuito->vnom_kv))
                {
                    circuito->barra_ini = trafo3E->pbarra3;
                }
                else if (IsDoubleZero(trafo3E->sec.vnom - circuito->vnom_kv))
                {
                    circuito->barra_ini = trafo3E->pbarra2;
                }
                else if (IsDoubleZero(trafo3E->pri.vnom - circuito->vnom_kv))
                {
                    circuito->barra_ini = trafo3E->pbarra1;
                }
                else
                {
                    if ((barra = se->ExisteBarra(sdi.vnom_kv)) == NULL)
                    {
                        barra = InserePri_BarraIniFicticia(sdi.vnom_kv, sdi.barra_mt_id);
                        se->InsereBarra(barra);
                    }
                }
            }
            else
            {
                circuito->barra_ini = barra;
            }
        }
        // verifica tensão nominal
        if (IsDoubleZero(circuito->vnom_kv))
        { // gera mensagem de erro
            if (plog)
                plog->Add("Tensão nominal nula");
            circuito->vnom_kv = circuito->barra_ini->vnom;
        }
        // verifica se tensão nominal diferente da Vnom da Barra inicial
        if (!IsDoubleZero(circuito->vnom_kv - circuito->barra_ini->vnom))
        { // define tensão nominal igual à da Barra
            if (plog)
                plog->Add("Tensão nominal incompatível com trafo");
            circuito->vnom_kv = circuito->barra_ini->vnom;
        }
        // insere Circuito na SE
        se->InsereCircuito(circuito);
        // complementa dados de curto no suprimento associado ao circuito
        // CompletaSuprimentoCircuito(se->id);
        // insere Rede em VTMonta
        str.dominio_id = dominio->Id;
        str.rede_id = sdi.id;
        str.mrede_id = ID_NULO;
        str.cluster_id = ID_NULO;
        str.barini_id = circuito->barra_ini->Id;
        str.color = DefineCorRedePrimaria();
        str.codigo = sdi.codigo;
        str.extern_id = IntToStr(sdi.id);
        str.estatica = false;
        str.radial = true;
        str.tiporede.id = ID_NULO;
        str.tiporede.segmento = redePRI;
        str.tiporede.codigo = "";
        // insere Rede em Monta
        if ((rede = (VTRede*)monta->InsereRede(str)) == NULL)
            return (false);
        // salva ponteiro da Rede no Circuito
        circuito->rede = rede;
    }
    catch (Exception &e)
    {
        circuito = NULL;
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18236926
bool __fastcall TPreMonta::InserePri_Regulador(sdi_REGULADOR &sdi)
{
    // variáveis locais
    int reg_max_percent;
    double reg_max_pu;
    strREGULADOR str;
    VTLigacao *ligacao;
    VTTrecho *trecho;

    try
    { // configura Log
        if (plog)
            plog->DefineValor("Eqpto", "Regulador");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // verifica se o Regulador pertence ao Circuito selecionado
        if ((circuito == NULL) || (circuito->id != sdi.id_rede_mt))
        {
            if (plog)
                plog->Add("Não está associado a rede primária");
            return (true);
        }
        if ((trecho = circuito->ExisteTrecho(sdi.id_barra1, sdi.id_barra2)) == NULL)
        {
            return (true);
        }
        if ((ligacao = circuito->rede->ExisteLigacao(eqptoTRECHO, trecho->Id)) == NULL)
        {
            if (plog)
                plog->Add("Não está associada a um trecho");
            return (true);
        }
        if (IsDoubleZero(sdi.snom_kva))
        {
            if (plog)
                plog->Add("Potência nominal nula");
            sdi.snom_kva = 300.0;
        }
        // proteção p/ número de passos
        if ((sdi.n_passos <= 2) || (sdi.n_passos > 40))
        {
            if (plog)
                plog->Add("Número de passos inválido");
            sdi.n_passos = 32;
        }
        // proteção p/ faixa de variação

        reg_max_percent = 10;
        // proteção p/ tap_pu
        reg_max_pu = reg_max_percent * 0.01;
        if ((sdi.tap_pu < (1.0 - reg_max_pu)) || (sdi.tap_pu > (1.0 + reg_max_pu)))
        {
            if (plog)
                plog->Add("Valor de tap_pu inválido");
            if (IsDoubleZero(sdi.tap_pu))
                sdi.tap_pu = 1.0;
            else if (sdi.tap_pu < (1. - reg_max_pu))
                sdi.tap_pu = 1.0 - reg_max_pu;
            else
                sdi.tap_pu = 1.0 + reg_max_pu;
        }
        // preenche estrutura strREGULADOR
        str.obra = false;
        str.id = sdi.id;
        str.rede_id = circuito->rede->Id;
        // define Barras do Regulador a partir das Barras do Trecho
        str.bar1_id = ligacao->pbarra1->Id;
        str.bar2_id = ligacao->pbarra2->Id;
        str.modelo = reguAUTO_BI; // reguPADRAO;
        str.codigo = sdi.codigo;
        str.by_pass = (sdi.bypass == 1);
        str.ligacao = lgEST_AT; // ConverteTipoLigacao(sdi.tipo_lig);
        str.fases = faseABC;
        str.snom_mva = sdi.snom_kva / 1000.; // kVA =>MVA
        str.var_tensao = reg_max_percent;
        str.num_passo = sdi.n_passos / 2;
        str.z0.r = 0.0;
        str.z0.x = 0.01;
        str.z1.r = 0.0;
        str.z1.x = 0.01;
        // str.sentido.reversivel = (sdi.inversao == 1);
        // tipo de ajuste: tap fixo com passo p/ obter o mesmo valor de sdi.tap_pu
        // DVK: alterado para automático, devido a solicitações
        // str.ajuste_auto = true; // false;
        // DVK: a barra de referencia está sendo adotada como qualquer uma, pois nao tem essa
        // informação no GIS e o ValFases vai alterar, conforme ordenação dos primários
        // str.cfg_auto.bar_ref_id = -1;
        str.cfg_auto.bar_ref_id = str.bar1_id;
        str.cfg_auto.v_pu = sdi.tap_pu;
        str.cfg_auto.bar_ref_inv_id = str.bar2_id;
        str.cfg_auto.vinv_pu = sdi.tap_pu;
        str.cfg_fixo.passo = ((sdi.tap_pu - 1) * str.num_passo) / reg_max_pu;
        // esquemático
        str.esq.posbar1 = 50.;
        str.esq.posbar2 = 50.;
        // retira Ligacao da sua Rede e destrói objeto
        circuito->rede->RemoveLigacao(ligacao);
        // retira da lista de trechos com arranjo errado, se estiver lá
        lisTRECHO_ALT->Remove(ligacao);
        DLL_DeleteEqpto(ligacao);
        // insere Regulador em VTMonta
        if (monta->InsereRegulador(str) == NULL)
            return (false);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_Socorro(sdi_SOCORRO &sdi)
{
    TSocorro *socorro;
    bool sucesso = false;

    try
    {
        socorro = NewObjSocorro();
        socorro->chave_id = sdi.id;
        socorro->circuito1_id = sdi.circ1_id;
        socorro->circuito2_id = sdi.circ2_id;
        socorro->se1_id = sdi.se1_id;
        socorro->se2_id = sdi.se2_id;
        socorro->barra1_id = sdi.barra1_id;
        socorro->barra2_id = sdi.barra2_id;
        socorro->telecomando = (sdi.telecomando != 0);
        socorro->ativo = (sdi.ativo != 0);
        lisSocorro->Add(socorro);
        sucesso = true;
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        sucesso = false;
        if (socorro != NULL)
            delete socorro;
    }
    return (sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18236926
bool __fastcall TPreMonta::InserePri_Trafo(sdi_TRAFO_LINHA &sdi)
{
    // variáveis locais
    strTRAFO str;
    VTLigacao *ligacao;

    try
    { // configura Log
        if (plog)
            plog->DefineValor("Eqpto", "Trafo");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // verifica se o Regulador pertence ao Circuito selecionado
        if ((circuito == NULL) || (circuito->id != sdi.circ_id))
        {
            if (plog)
                plog->Add("Não está associado a rede primária");
            return (true);
        }
        // determina Trecho correspondente ao Tafo
        if ((ligacao = circuito->rede->ExisteLigacao(eqptoTRECHO, sdi.trecho_id)) == NULL)
        {
            if (plog)
                plog->Add("Não está associado a um trecho");
            return (true);
        }
        // verifica potência nominal
        if (IsDoubleZero(sdi.snom_kva))
        {
            if (plog)
                plog->Add("Potência nominal nula");
            sdi.snom_kva = 5000;
        }
        // verifica tensão do primário
        if (IsDoubleZero(sdi.valta_kv))
        {
            if (plog)
                plog->Add("Tensão nominal primária nula");
            sdi.valta_kv = circuito->vnom_kv;
        }
        else if (!IsDoubleZero(sdi.valta_kv - circuito->vnom_kv))
        {
            if (plog)
                plog->Add("Tensão nominal primária diferente do circuito");
            sdi.valta_kv = circuito->vnom_kv;
        }
        // verifica tensão do secundário
        if (IsDoubleZero(sdi.vbaixa_kv))
        {
            if (plog)
                plog->Add("Tensão nominal secundária nula");
            sdi.vbaixa_kv = 13.8;
        }
        // insere Trafo na Rede
        str.trf_id = sdi.id;
        str.rede_id = circuito->rede->Id;
        str.bar1_id = ligacao->pbarra1->Id;
        str.bar2_id = ligacao->pbarra2->Id;
        str.codigo = sdi.codigo;
        str.extern_id = sdi.extern_id;
        str.snom_mva = sdi.snom_kva * 0.001;
        str.z0.r = sdi.r0_pu;
        str.z0.x = sdi.x0_pu;
        str.z1.r = sdi.r1_pu;
        str.z1.x = sdi.x1_pu;
        str.perda_ferro = (sdi.perda_fe_kw / sdi.snom_kva) * 100.;
        // primário
        str.pri.fases = faseABC;
        str.pri.ligacao = lgTRIANG;
        str.pri.vnom = sdi.valta_kv;
        str.pri.tap = sdi.valta_kv;
        str.pri.defasagem = 0;
        str.pri.zterra.r = 0;
        str.pri.zterra.x = 0;
        // secundário
        str.sec.fases = faseABCNT;
        str.sec.ligacao = lgEST_AT;
        str.sec.vnom = sdi.vbaixa_kv;
        str.sec.tap = sdi.vbaixa_kv;
        str.sec.defasagem = -30.;
        str.sec.zterra.r = 0;
        str.sec.zterra.x = 0;
        // dados de LTC
        str.ltc.tipo = ltcFIXO;
        str.ltc.bar_id = ID_NULO;
        str.ltc.v_pu = 1.0;
        // posição nas barras
        str.esq.posbar1 = 50.;
        str.esq.posbar2 = 50.;
        // retira Ligacao da sua Rede e destrói objeto
        circuito->rede->RemoveLigacao(ligacao);
        // retira da lista de trechos com arranjo errado, se estiver lá
        lisTRECHO_ALT->Remove(ligacao);
        DLL_DeleteEqpto(ligacao);
        if (monta->InsereTrafo(str) == NULL)
            return (false);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18236926
bool __fastcall TPreMonta::InserePri_Trecho(sdi_TRECHO &sdi)
{
    // variáveis locais
    int fases_tag;
    AnsiString codigo;
    strTRECHO str_tre;
    VTArranjo *arranjo;
    VTBarra *barra1, *barra2;
    VTTrecho *trecho;
    VTSuporte *suporte;
    bool trechoCurto = false;
    bool arranjo_invalido = false;

    try
    { // configura Log
        if (plog)
            plog->DefineValor("Eqpto", "Trecho MT");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);
        // verifica se o Trecho pertence ao Circuito selecionado
        if ((circuito == NULL) || (circuito->id != sdi.circ_id))
        {
            if (plog)
                plog->Add("Não está associado a rede primária");
            return (true);
        }
        // verifica se as barras vieram nulas
        if ((sdi.barra1_id == 0) && (sdi.barra1_id == 0))
        {
            // verifica se as barras auxiliares foram criadas senão as cria
            if ((circuito->barra_id0_a == NULL) && (circuito->barra_ini != NULL))
            {
                circuito->barra_id0_a = InserePri_BarraFicticia(circuito->barra_ini);
            }
            if ((circuito->barra_id0_b == NULL) && (circuito->barra_ini != NULL))
            {
                circuito->barra_id0_b = InserePri_BarraFicticia(circuito->barra_ini);
            }
        }
        else
        { // verifica se Barras iguais
            if (sdi.barra1_id == sdi.barra2_id)
            { // não cria trecho com barras iguais
                if (plog)
                    plog->Add("Barra inicial igual à barra final");
                return (true);
            }
        }
        // proteção: troca o o ID das barras se for igual ao ID da Barra inicial indicada pelo GIS
        if (sdi.barra1_id == circuito->GIS_bar_ini_id)
            sdi.barra1_id = circuito->barra_ini->Id;
        else if (sdi.barra2_id == circuito->GIS_bar_ini_id)
            sdi.barra2_id = circuito->barra_ini->Id;
        // verifica se existem as Barras
        barra1 = monta->ExisteBarra(sdi.barra1_id);
        barra2 = monta->ExisteBarra(sdi.barra2_id);
        // caso seja id = 0...
        if ((sdi.barra1_id == 0) && (sdi.barra1_id == 0))
        {
            barra1 = circuito->barra_id0_a;
            barra2 = circuito->barra_id0_b;
        }
        if ((barra1 == NULL) && (barra2 == NULL))
        {
            if (plog)
                plog->Add("Não existem as duas barras");
            return (true);
        }
        if ((barra1 != NULL) && (barra2 == NULL))
        {
            if (plog)
                plog->Add("Não existe a barra " + IntToStr(sdi.barra2_id));
            // cria uma nova Barra baseada na Barra 1
            // InserePri_BarraNaoRecebidaGis(sdi.barra2_id, barra1);
            return (true);
        }
        else if ((barra1 == NULL) && (barra2 != NULL))
        {
            if (plog)
                plog->Add("Não existe a barra " + IntToStr(sdi.barra1_id));
            // cria uma nova Barra baseada na Barra 2
            // InserePri_BarraNaoRecebidaGis(sdi.barra1_id, barra2);
            return (true);
        }
        // if ((trecho = circuito->ExisteTrecho(barra1->Id, barra2->Id)) != NULL)
        // {
        // if (plog)
        // plog->Add("Esse trecho já existe");
        // return (true);
        // }
        // determina fases do Trecho e inclui o neutro
        if (sdi.fases.IsEmpty())
        {
            if (plog)
                plog->Add("Fases indefinidas");
            // assume fases ABCN
            sdi.fases = "ABCN";
        }
        fases_tag = fases->ExisteFase(sdi.fases) | faseN;
        // verifica se cabo_id está definido
        if ((sdi.cabo_id <= 0) || (sdi.cabo_id == 999))
        {
            if (plog)
                plog->Add("Cabo não definido");
            // assume Cabo default
            if (empresa->CondutorDefaultMT != NULL)
            {
                sdi.cabo_id = empresa->CondutorDefaultMT->id;
            }
            arranjo_invalido = true;
        }
        // verifica se existe Arranjo (criando um novo Arranjo se for necessário)
        if ((arranjo = ExisteArranjo(sdi.cabo_id, fases_tag)) == NULL)
        {
            if (plog)
                plog->Add("Não existe cabo");
            // assume default
            arranjo = arranjoMT;
            // verifica se precisa trocar as fases do arranjoMT
            if (arranjo->Fases != fases_tag)
            {
                arranjo = ArranjoMTcerto(fases_tag);
            }
            arranjo_invalido = true;
            // return (true);
        }

        if (IsDoubleZero(sdi.comp_m))
        { // verifica se comprimento nulo
            trechoCurto = true;
            sdi.comp_m = 1.;
        }
        else if (sdi.comp_m < 1.0)
        { // verifica se comprimento é curto
            trechoCurto = true;
            sdi.comp_m = 1.;
        }

        // determina tag p/ fases do trecho e inclui o neutro
        fases_tag = fases->ExisteFase(sdi.fases) | faseN;
        // preenche estrutura strTRECHO
        str_tre.tre_id = sdi.id;
        str_tre.rede_id = circuito->rede->Id;
        // str_tre.rede_id = sdi.circ_id;
        str_tre.codigo = sdi.codigo;
        str_tre.bar1_id = sdi.barra1_id;
        str_tre.bar2_id = sdi.barra2_id;
        str_tre.fases = fases_tag;
        str_tre.comp_km = sdi.comp_m * 0.001;
        str_tre.esq.posbar1 = 50.;
        str_tre.esq.posbar2 = 50.;
        // insere Trecho com o Arranjo definido
        if ((trecho = (VTTrecho*)monta->InsereTrecho(str_tre, barra1, barra2, arranjo)) != NULL)
        { // remove Trecho da Rede e salva em Circuito
            circuito->rede->RemoveLigacao(trecho);
            circuito->InsereTrecho(trecho);
            trecho->Status[sttTRECHOCURTO] = trechoCurto;
            if (arranjo_invalido)
            {
                lisTRECHO_ALT->Add(trecho);
            }
            return (true);
        }
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TPreMonta::InsereSec_Barra(sdi_BARRA_BT &sdi)
 {
 // variáveis locais
 strBARRA str;
 VTBarra *barra = NULL;

 try{//configura log
 if (plog) plog->DefineValor("Eqpto", "Barra");
 if (plog) plog->DefineValor("Código", sdi.codigo);
 //proteção
 if (et == NULL)
 {
 if (plog) plog->Add("Não existe rede secundária ativa");
 return(false);
 }
 // cria uma nova Barra
 str.bar_id    = sdi.id;
 str.codigo    = sdi.codigo;
 str.extern_id = "";
 str.vnom      = et->sec_vnom_kv;
 str.utm.x     = sdi.x_utm_m * 100;
 str.utm.y     = sdi.y_utm_m * 100;
 //esquemático
 str.esq.x1 = str.utm.x;
 str.esq.y1 = str.utm.y;
 str.esq.x2 = str.utm.x;
 str.esq.y2 = str.utm.y;
 // insere Barra em VTMonta
 if ((barra = (VTBarra*)monta->InsereBarraEqpto(str)) != NULL)
 {//salva Barra na Et
 et->InsereBarra(barra);
 }
 }catch(Exception &e)
 {
 if (plog) plog->Add(ERRO_DE_SISTEMA);
 return(false);
 }
 return(true);
 }
 */

// ---------------------------------------------------------------------------
VTBarra* __fastcall TPreMonta::InsereSec_BarraTrafo(TEt *et, int barra_id)
{
    // variáveis locais
    strBARRA str;
    VTBarra *barra = NULL;

    try
    { // configura log
        if (plog)
            plog->DefineValor("Eqpto", "Barra");
        if (plog)
            plog->DefineValor("Código", et->codigo);
        // proteção
        if (et->barra_bt != NULL)
            return (NULL);
        // cria uma nova Barra
        str.bar_id = barra_id;
        str.codigo = "";
        str.extern_id = IntToStr(barra_id);

        str.vnom = et->sec_vnom_kv;
        str.utm.x = et->barra_mt->utm.x;
        str.utm.y = et->barra_mt->utm.y - DYBAR;
        // esquemático
        str.esq.x1 = str.utm.x;
        str.esq.y1 = str.utm.y;
        str.esq.x2 = str.utm.x;
        str.esq.y2 = str.utm.y;
        // insere Barra em VTMonta
        if ((barra = (VTBarra*)monta->InsereBarraEqpto(str)) != NULL)
        { // salva Barra na Et
            et->InsereBarra(barra);
        }
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        barra = NULL;
    }
    return (barra);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TPreMonta::InsereSec_Capacitor(sdi_CAPACITOR_BT &sdi)
 {
 // variáveis locais
 strCAPACITOR  str;

 try {//cofigura Log
 if (plog) plog->DefineValor("Eqpto", "Capacitor");
 if (plog) plog->DefineValor("Código", sdi.codigo);
 // verifica se o Capacitor série pertence à Rede secundária selecionada
 if ((et == NULL) ||(et->rede->Id != sdi.redebt_id))
 {
 if (plog) plog->Add("Não está associado a rede secundária");
 return (true);
 }
 // preenche estrutura strCAPACITOR
 str.cap_id       = sdi.id;
 str.codigo       = sdi.codigo;
 str.extern_id    = sdi.extern_id;
 str.bar_id       = sdi.barra_id;
 str.ligacao      = lgTRIANG;
 str.vnom         = et->sec_vnom_kv;
 str.q_mvar       = sdi.qnon_kvar * 0.001;
 str.esq.posbar   = 50.;
 str.esq.dxbar    = 0;
 str.esq.dybar    = -DYBAR;
 // insere Capacitor série em VTMonta
 if (monta->InsereCapacitor(str) == NULL) return(false);
 }catch (Exception &e)
 {
 if (plog) plog->Add(ERRO_DE_SISTEMA);
 return (false);
 }
 return(true);
 }

 //---------------------------------------------------------------------------
 bool __fastcall TPreMonta::InsereSec_Consumidor(sdi_CONS_BT &sdi)
 {
 //variáveis locais
 TPtoCargaBT *ptc;
 VTCarga     *carga;

 try{//configura Log
 if (plog) plog->DefineValor("Eqpto", "Carga");
 // verifica se o Consumidor  pertence à Rede secundária selecionada
 if ((et == NULL) ||(et->rede->Id != sdi.redebt_id))
 {
 if (plog) plog->Add("Não está associado a rede secundária");
 return (true);
 }
 // determina PtoServicoBT do consumidor
 if ((ptc = et->ExistePtoCargaBT(sdi.pcarga_id)) == NULL)
 {
 if (plog) plog->Add("Não existe pto.carga");
 return (true);
 }
 // incrementa número de Trechos da ET e verifica se é a primeira
 et->num_trecho++;
 if (et->num_trecho == 1)
 {//cria Trafos da ET e insere em VTMonta
 InsereSec_Trafo(et);
 }
 // cria Carga do conumidor BT
 if ((carga = CriaCargaConsumidorBT(sdi, ptc->barra->Id)) != NULL)
 {//insere Carga na Et
 et->InsereCarga(carga);
 }
 }catch(Exception &e)
 {
 if (plog) plog->Add(ERRO_DE_SISTEMA);
 return(false);
 }
 return(true);
 }

 // ---------------------------------------------------------------------------
 bool __fastcall TPreMonta::InsereSec_ConsIP(sdi_CONS_IP &sdi)
 {
 // variáveis locais
 VTBarra *barra;
 VTCarga *carga = NULL;

 try
 { // configura Log
 if (plog)
 plog->DefineValor("Eqpto", "Carga IP");
 if (plog)
 plog->DefineValor("Código", IntToStr(sdi.id));
 // verifica se o Consumidor  pertence à Rede secundária selecionada
 if (et == NULL)
 {
 if (plog)
 plog->Add("Não está associado a rede secundária");
 return (true);
 }
 // incrementa número de Trechos da ET e verifica se é a primeira
 et->num_trecho++;
 if (et->num_trecho == 1)
 { // cria Trafos da ET e insere em VTMonta
 InsereSec_Trafo(et);
 }
 // cria Carga de IP
 CriaCargaConsumidorIP(sdi, sdi.barra_id);
 }
 catch (Exception &e)
 {
 if (plog)
 plog->Add(ERRO_DE_SISTEMA);
 return (false);
 }
 return (true);
 }


 //---------------------------------------------------------------------------
 bool __fastcall TPreMonta::InsereSec_Gerador(sdi_GERADOR_BT &sdi)
 {
 // variáveis locais
 double     s_mva, p_mw, q_mvar;
 strGERADOR str;
 VTCarga    *carga;

 try{//configura Resumo
 if (plog) plog->DefineValor("Eqpto", "Gerador");
 if (plog) plog->DefineValor("Código", sdi.codigo);
 //proteção
 if ((et == NULL)||(et->id != sdi.circ_id))
 {
 if (plog) plog->Add("Não está associado a rede secundária");
 return (true);
 }
 // inicia estrutura strGerador
 str.obra         = false;
 str.ger_id       = sdi.id;
 str.bar_id       = sdi.barra_id;
 str.codigo       = sdi.codigo;
 str.extern_id    = "";
 str.tiposup      = supPV;
 str.tipo_geracao = gerPCH;
 str.ligacao      = lgEST_AT;
 str.vnom         = circuito->vnom_kv;
 str.snom_mva     = sdi.snom_kva * 0.001;
 str.smax_mva     = str.snom_mva * 1.2;
 str.fplim        = 0.9;
 str.z0.r         = 0.;
 str.z0.x         = 0.05;
 str.z1.r         = 0.;
 str.z1.x         = 0.05;
 // define curva de geração própria
 str.curva.id        = -1;
 str.curva.tipica    = false;
 str.curva.num_pat   = pat01HORA;
 str.curva.num_valor = nvCURVA_PQVT;
 //define pot.ativa e reativa a partir de Snom e considerando fatpot = 0.92
 s_mva   = str.snom_mva;
 p_mw    = s_mva * 0.92;
 q_mvar  = sqrt((s_mva * s_mva) - (p_mw * p_mw));
 for (int np = 0; np < pat01HORA; np++)
 {// define hora inicial e final do patamar
 str.curva.patamar[np].hm_ini = pat_edp[np].hm_ini;
 str.curva.patamar[np].hm_fim = pat_edp[np].hm_fim;
 //define valores de ponta/fora de ponta
 if (pat_edp[np].ponta)
 {//demanda de ponta
 str.curva.patamar[np].valor[indP] = p_mw;
 str.curva.patamar[np].valor[indQ] = q_mvar;
 str.curva.patamar[np].valor[indV] = 1.;
 str.curva.patamar[np].valor[indT] = 0.;
 }
 else
 {//demanda fora de ponta
 str.curva.patamar[np].valor[indP] = p_mw;
 str.curva.patamar[np].valor[indQ] = q_mvar;
 str.curva.patamar[np].valor[indV] = 1.;
 str.curva.patamar[np].valor[indT] = 0.;
 }
 }
 // define coordenadas p/ esquemático
 str.esq.dxbar = -2000;
 str.esq.dybar = 0;
 // insere Gerador em VTMonta
 if (!monta->InsereGerador(str)) return (false);
 }catch(Exception &e)
 {
 if (plog) plog->Add(ERRO_DE_SISTEMA);
 return(false);
 }
 return(true);
 }

 // ---------------------------------------------------------------------------
 bool __fastcall TPreMonta::InsereSec_Link(sdi_TRECHO_BT &sdi)
 {
 // variáveis locais
 int fases_tag;
 AnsiString codigo;
 strTRECHO str_tre;
 VTArranjo *arranjo;
 VTBarra *barra1, *barra2;
 VTTrecho *trecho;

 try
 { // configura Log
 if (plog)
 plog->DefineValor("Eqpto", "Link BT");
 if (plog)
 plog->DefineValor("Código", "trecho_id=" + IntToStr(sdi.id));
 // verifica se há uma Et para o Link
 if (et == NULL)
 {
 if (plog)
 plog->Add("Não está associado a rede secundária");
 return (true);
 }
 // determina Trecho associado
 if ((trecho = et->ExisteTrecho(sdi.id)) == NULL)
 {
 if (plog)
 plog->Add("Não está associado a um trecho");
 return (true);
 }
 // verifica se Barras iguais
 if (sdi.barra_id1 == sdi.barra_id2)
 { // não cria trecho com barras iguais
 if (plog)
 plog->Add("Barra inicial igual à barra final");
 return (true);
 }
 // verifica se existem as Barras
 barra1 = monta->ExisteBarra(sdi.barra1_id);
 barra2 = monta->ExisteBarra(sdi.barra2_id);
 if ((barra1 == NULL) || (barra2 == NULL))
 {
 if (plog)
 plog->Add("Não existem as barras");
 return (true);
 }
 // preenche estrutura strTRECHO
 str_tre.tre_id = FalsoId();
 str_tre.rede_id = et->rede->Id;
 str_tre.extern_id = "";
 str_tre.codigo = trecho->Codigo;
 str_tre.bar1_id = barra1->Id;
 str_tre.bar2_id = barra2->Id;
 str_tre.fases = fases_tag;
 str_tre.comp_km = Distancia_cm(barra1->utm.x, barra1->utm.y, barra2->utm.x,
 barra2->utm.y) * 1e-5;
 str_tre.esq.posbar1 = 50.;
 str_tre.esq.posbar2 = 50.;
 // insere Trecho com o Arranjo definido
 if ((trecho = (VTTrecho*)monta->InsereTrecho(str_tre, barra1, barra2, trecho->arranjo))
 == NULL)
 { // remove Trecho da Rede e salva em Circuito
 circuito->rede->RemoveLigacao(trecho);
 circuito->InsereTrecho(trecho);
 return (true);
 }
 }
 catch (Exception &e)
 {
 if (plog)
 plog->Add(ERRO_DE_SISTEMA);
 return (false);
 }
 return (true);
 }

 //---------------------------------------------------------------------------
 bool __fastcall TPreMonta::InsereSec_PtoCarga(sdi_PCARGA_BT &sdi)
 {
 // variáveis locais
 TPtoCargaBT *ptc;
 VTBarra     *barra;

 try{//configura Resumo
 if (plog) plog->DefineValor("Eqpto", "Pto.Carga BT");
 if (plog) plog->DefineValor("Código", sdi.codigo);
 //proteção
 if ((et == NULL)||(et->id != sdi.redebt_id))
 {
 if (plog) plog->Add("Não está associado a rede secundária");
 return (true);
 }
 //determina Barra do PtoServico
 if ((barra = et->ExisteBarra(sdi.barra_id)) == NULL)
 {
 if (plog) plog->Add("Barra inexistente");
 return(true);
 }
 // cria um objeto PtoCargaBT
 if ((ptc = NewObjPtoCargaBT()) == NULL)
 {
 if (plog) plog->Add(ERRO_DE_SISTEMA);
 return (false);
 }
 ptc->id        = sdi.id;
 ptc->codigo    = sdi.codigo;
 ptc->extern_id = sdi.extern_id;
 ptc->ativo     = sdi.ativo;
 ptc->barra     = barra;
 // insere PtoCargaBT na ET
 et->InserePtoCargaBT(ptc);
 }catch(Exception &e)
 {
 if (plog) plog->Add(ERRO_DE_SISTEMA);
 return(false);
 }
 return(true);
 }


 // ---------------------------------------------------------------------------
 bool __fastcall TPreMonta::InsereSec_Rede(sdi_INSTAL_TRAFO &sdi)
 {
 // variáveis locais
 strREDE str_rede;
 VTBarra *barra_mt, *barra_bt;

 try
 { // configura Log
 if (plog)
 plog->DefineValor("Rede", sdi.codigo);
 if (plog)
 plog->DefineValor("Eqpto", "Rede BT");
 if (plog)
 plog->DefineValor("Código", sdi.codigo);
 // reinicia ponteiro p/ ET corrente
 et = NULL;
 // verifica se existe Circuito selecionado
 if ((circuito == NULL) || (circuito->rede->Id != sdi.circ_id))
 {
 if (plog)
 plog->Add("Não está associado a rede primária");
 return (false);
 }
 // verifica se é uma EP
 // não há este tipo de informação
 // proteção: verifica se já existe uma ET c/ mesmo ID
 if (circuito->ExisteEt(sdi.id_rede_bt))
 {
 if (plog)
 plog->Add("ID repetido");
 return (false);
 }
 // verifica se a tensão de baixa é válida
 if (IsDoubleZero(sdi.vbaixa_v))
 {
 if (plog)
 plog->Add("Tensão nominal do secundário nula");
 // define tensão em função do tipo da ET
 sdi.vbaixa_v = 220;
 }
 // determina Barra do primário
 if ((barra_mt = circuito->ExisteBarra(sdi.barra_mt_id)) == NULL)
 {
 if (plog)
 plog->Add("Não existe barra de conexão c/ rede primária");
 return (true);
 }
 // redefine código da Barra com o código da instalação
 barra_mt->Codigo = sdi.codigo;
 // cria objeto Et e insere na lista de ET do circuito
 if ((et = CriaEt(sdi, barra_mt, NULL)) == NULL)
 return (false);
 // cria Barra BT p/ ligar os Trafos da ET
 barra_bt = InsereSec_Barra(et, sdi.barra_bt_id);
 // define Barra BT da Et
 et->barra_bt = barra_bt;
 // insere Et no Circuito
 circuito->InsereEt(et);
 // insere Barra BT na Et
 // et->InsereBarra(barra_bt);
 // insere Rede secundária em VTMonta
 str_rede.dominio_id = dominio->Id;
 str_rede.rede_id = et->id;
 str_rede.mrede_id = -1;
 str_rede.cluster_id = -1;
 str_rede.barini_id = barra_mt->Id;
 str_rede.color = clBlack;
 str_rede.codigo = sdi.codigo;
 str_rede.tiporede.id = ID_NULO;
 str_rede.tiporede.segmento = redeSEC;
 str_rede.tiporede.codigo = "";
 // str_rede.estatica          = false;
 str_rede.estatica = true;
 str_rede.radial = true;
 if ((et->rede = (VTRede*)monta->InsereRede(str_rede)) == NULL)
 return (false);
 }
 catch (Exception &e)
 {
 if (plog)
 plog->Add(ERRO_DE_SISTEMA);
 return (false);
 }
 return (true);
 }
 */
// -----------------------------------------------------------------------------
void __fastcall TPreMonta::InsereSec_CargaEquivalente(TEt *et)
{
    // variáveis locais
    strCARGA str_carga;
    VTPatamar *patamar;

    // cria Carga equivalente com fases ABC e demanda nula
    str_carga.car_id = et->id;
    str_carga.bar_id = (et->barra_bt != NULL) ? et->barra_bt->Id : et->barra_mt->Id;
    str_carga.codigo = et->codigo;
    str_carga.classe_cons = consEQUIVALENTE;
    str_carga.modelo.icte_pu = 1;
    str_carga.modelo.scte_pu = 0;
    str_carga.modelo.zcte_pu = 0;
    str_carga.fases = (et->FasesPrimario() & faseABCN);
    str_carga.energia_mwhmes = 0;
    // cancela dados de IP
    str_carga.ip.num_lamp = 0.;
    str_carga.ip.pot_kw = 0.;
    str_carga.ip.fat_pot = 1.;
    // define curva própria
    str_carga.curva.id = -1;
    str_carga.curva.tipica = false;
    str_carga.curva.num_pat = patamares->NumPatamar();
    str_carga.curva.num_valor = nvCURVA_PQ;
    // define curva de demanda própria nula
    for (int np = 0; np < patamares->NumPatamar(); np++)
    { // define hora inicial e final do patamar
        patamar = patamares->Patamar[np];
        str_carga.curva.patamar[np].hm_ini.hora = patamar->HoraIni;
        str_carga.curva.patamar[np].hm_ini.minuto = patamar->MinutoIni;
        str_carga.curva.patamar[np].hm_fim.hora = patamar->HoraFim;
        str_carga.curva.patamar[np].hm_fim.minuto = patamar->MinutoFim;
        str_carga.curva.patamar[np].valor[indP] = 0;
        str_carga.curva.patamar[np].valor[indQ] = 0;
    }
    // define posição da carga na Barra
    str_carga.esq.posbar = 50.;
    str_carga.esq.dxbar = 2000;
    str_carga.esq.dybar = -2000;
    // insere Carga em VTMonta
    et->carga = (VTCarga*)(monta->InsereCarga(str_carga));
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereSec_Trafo(TEt *et)
{
    // variáveis locais
    AnsiString txt;
    strTRAFO str_trafo;
    TUtc *utc;
    TList *lisUTC = et->LisUtc();

    try
    { // configura Log
        if (plog)
            plog->DefineValor("Rede", et->codigo);
        if (plog)
            plog->DefineValor("Eqpto", "Rede SEC");
        if (plog)
            plog->DefineValor("Código", et->codigo);
        if (lisUTC->Count == 0)
        {
            if (plog)
                plog->Add("Não possui UTCs");
            return (true);
        }
        // verifica se as fases estão definidas
        if ((et->FasesPrimario() == 0) || (et->FasesSecundario() == 0))
        {
            if (plog)
                plog->Add("Fases indefinidas");
            return (true);
        }
        // insere um Trafo p/ cada UTC da ET
        for (int n = 0; n < lisUTC->Count; n++)
        {
            utc = (TUtc*)lisUTC->Items[n];
            if (plog)
                plog->DefineValor("Eqpto", "Trafo UTC");
            if (plog)
                plog->DefineValor("Código", utc->Codigo());
            InsereSec_TrafoUtc(et, utc);
        }
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereSec_TrafoUtc(TEt *et, TUtc *utc)
{
    // variáveis locais
    int num_fase;
    double vpri_kv, vsec_kv;
    strTRAFO str;

    // determina tensões nominal do primário
    if (fases->NumeroDeFases(utc->pri.fases) == 1)
    { // tensão nominal igual à tensão fase/neutro
        vpri_kv = et->Vfn_pri_kv();
    }
    else
    { // tensão nominal igual à tensão fase/fase
        vpri_kv = et->Vff_pri_kv();
    }
    // determina tensões nominal do secundário
    if (fases->NumeroDeFases(utc->sec.fases) == 1)
    { // tensão nominal igual à tensão fase/fase
        vsec_kv = et->Vfn_sec_kv();
    }
    else
    { // tensão nominal igual à tensão de fase
        vsec_kv = et->Vff_sec_kv();
    }
    // insere Trafo na Rede (secundária ou primária)
    str.trf_id = utc->id;
    str.rede_id = (et->rede != NULL) ? et->rede->Id : circuito->rede->Id;
    str.bar1_id = et->barra_mt->Id;
    str.bar2_id = et->barra_bt->Id;
    str.codigo = utc->Codigo();
    str.extern_id = utc->oid;
    str.snom_mva = utc->snom_kva * 0.001;
    str.z0.r = utc->r_pu;
    str.z0.x = utc->x_pu;
    str.z1.r = utc->r_pu;
    str.z1.x = utc->x_pu;
    str.perda_ferro = utc->perda_fe_perc;
    str.pri.fases = utc->pri.fases;
    str.pri.ligacao = utc->LigacaoPrimario();
    str.pri.vnom = vpri_kv;
    str.pri.tap = vpri_kv;
    str.pri.defasagem = 0;
    str.pri.zterra.r = 0;
    str.pri.zterra.x = 0;
    str.sec.fases = utc->sec.fases;
    str.sec.ligacao = utc->LigacaoSecundario();
    str.sec.vnom = vsec_kv;
    str.sec.tap = vsec_kv;
    str.sec.defasagem = utc->DefasagemPriSec();
    str.sec.zterra.r = 0;
    str.sec.zterra.x = 0;
    // dados de LTC
    str.ltc.tipo = ltcFIXO;
    str.ltc.bar_id = -1;
    str.ltc.v_pu = 1.0;
    // posição nas barras
    str.esq.posbar1 = 50.;
    str.esq.posbar2 = 50.;
    if (monta->InsereTrafo(str) == NULL)
        return (false);

    return (true);
}

//// ---------------------------------------------------------------------------
// bool __fastcall TPreMonta::InsereSec_Trecho(sdi_TRECHO_BT &sdi)
// {
// // variáveis locais
// int fases_tag;
// strTRECHO str_tre;
// VTArranjo *arranjo;
// VTBarra *barra1, *barra2;
// VTTrecho *trecho;
// VTSuporte *suporte;
//
// try
// { // configura Log
// if (plog)
// plog->DefineValor("Eqpto", "Trecho");
// if (plog)
// plog->DefineValor("Código", sdi.codigo);
// // verifica se o Trecho pertence à Et selecionado
// if ((et == NULL) || (et->id != sdi.redebt_id))
// {
// if (plog)
// plog->Add("Trecho não está associado a rede secundária");
// return (true);
// }
// // verifica se Barras iguais
// if (sdi.barra1_id == sdi.barra2_id)
// { // não cria trecho com barras iguais
// if (plog)
// plog->Add("Barra inicial igual à barra final");
// return (true);
// }
// // verifica se existem as Barras
// barra1 = monta->ExisteBarra(sdi.barra1_id);
// barra2 = monta->ExisteBarra(sdi.barra2_id);
// if ((barra1 == NULL) || (barra2 == NULL))
// {
// if (plog)
// plog->Add("Não existem as barras");
// return (true);
// }
// // determina Arranjo do Trecho
// // fases_tag = fases->ExisteFase(sdi.fases);
// // SEMPRE assume p/ o Trecho as mesmas fases da ET (sem terra). exlcuindo neutro p/ IP
// fases_tag = et->FasesSecundario() & faseABCN;
// // verifica se existe Arranjo
// if (sdi.cabo_fase_id == 0)
// {
// if (plog)
// plog->Add("ID do cabo nulo");
// return (true);
// }
// // verifica se existe Arranjo (criando um novo Arrano se for necessário)
// if ((arranjo = ExisteArranjo(sdi.cabo_fase_id, fases_tag)) == NULL)
// {
// if (plog)
// plog->Add("Não existe arranjo");
// return (true);
// }
// // determina tag p/ fases do trecho e inclui o neutro
// fases_tag = fases->ExisteFase(sdi.fases) | faseN;
// // preenche estrutura strTRECHO
// str_tre.tre_id = sdi.id;
// str_tre.rede_id = et->rede->Id;
// str_tre.extern_id = "";
// str_tre.codigo = sdi.codigo;
// str_tre.bar1_id = sdi.barra1_id;
// str_tre.bar2_id = sdi.barra2_id;
// str_tre.fases = fases_tag;
// str_tre.comp_km = sdi.comp_m * 0.001;
// str_tre.esq.posbar1 = 50.;
// str_tre.esq.posbar2 = 50.;
// // insere Trecho com o Arranjo definido
// if ((trecho = (VTTrecho*)monta->InsereTrecho(str_tre, barra1, barra2, arranjo)) != NULL)
// { // remove Trecho da sua Rede e insere em TEt
// et->rede->RemoveLigacao(trecho);
// et->InsereTrecho(trecho);
// InsereSec_Link(sdi);
// }
// }
// catch (Exception &e)
// {
// if (plog)
// plog->Add(ERRO_DE_SISTEMA);
// return (false);
// }
// return (true);
// }
//
// *

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereSec_Utc(sdi_UTC_BT &sdi)
{
    // variáveis locais
    int tipo_utc, fases_bt, fases_mt;
    double r_pu_default = 0.03;
    double x_pu_default = 0.05;
    double perda_fe_perc_default = 0.5;
    TUtc *utc;

    try
    { // configura log
        if (plog)
            plog->DefineValor("Eqpto", "UTC");
        if (plog)
            plog->DefineValor("Código", sdi.codigo);

        // verifica se existe Circuito selecionado
        if ((et == NULL) || (et->id != sdi.et_id))
        {
            if (plog)
                plog->Add("Não está associada a uma ET");
            return (true);
        }
        // proteção: verifica se fases indefinidas
        if ((sdi.fases_mt.IsEmpty()) || (sdi.fases_bt.IsEmpty()))
        {
            if (plog)
                plog->Add("Fases indefinidas");
        }
        // determina fases da UTC
        fases_mt = fases->ExisteFase(sdi.fases_mt);
        fases_bt = fases->ExisteFase(sdi.fases_bt);
        // determina tipo da UTC em função da Et e de suas fases
        switch (et->tipo_et)
        {
        case 1: // MONOFASICA
            tipo_utc = utcLUZ;
            fases_mt = et->fases_mt | faseN;
            fases_bt = faseABNT;
            break;
        case 2: // BIFASICA
            tipo_utc = utcLUZ;
            fases_mt = et->fases_mt;
            fases_bt = faseABNT;
            break;
        case 3: // TRIFASICA
            tipo_utc = utcTRI;
            fases_mt = faseABC;
            fases_bt = faseABCNT;
            break;
        case 4: // DELTA ABERTO
            if ((utc = et->ExisteUtcNaoReal(fases_mt, fases_bt)) == NULL)
                return (true);
            // assume dados da UTC
            tipo_utc = utc->tipo_utc;
            fases_mt = utc->pri.fases;
            fases_bt = utc->sec.fases;
            break;
        case 5: // DELTA FECHADO
            if ((utc = et->ExisteUtcNaoReal(fases_mt, fases_bt)) == NULL)
                return (true);
            // assume dados da UTC
            tipo_utc = utc->tipo_utc;
            fases_mt = utc->pri.fases;
            fases_bt = utc->sec.fases;
            break;
        default:
            return (true);
        }
        // proteção
        if (IsDoubleZero(sdi.snom_kva))
        {
            if (plog)
                plog->Add("Potência nominal nula");
            // assume valor default
            sdi.snom_kva = 100;
        }
        // cria um objeto Utc
        if ((utc = NewObjUtc()) == NULL)
        {
            if (plog)
                plog->Add("Não foi possível alocar memória p/ dados do UTC");
            return (false);
        }
        utc->id = sdi.id;
        utc->tipo_utc = tipo_utc;
        utc->snom_kva = sdi.snom_kva;
        utc->pri.fases = fases_mt;
        utc->sec.fases = fases_bt;
        utc->perda_fe_perc = ((sdi.perda_fe_w * 0.001) / utc->snom_kva) * 100;
        utc->r_pu = (sdi.perda_cu_w * 0.001) / utc->snom_kva; // W => pu
        utc->x_pu = sdi.x1_pu;
        // proteção: verifica resistência nula
        if (IsDoubleZero(utc->r_pu))
        {
            if (plog)
                plog->Add("Resistência nula");
            // assume valor default
            utc->r_pu = r_pu_default;
        }
        else if (utc->r_pu > r_pu_default)
        {
            if (plog)
                plog->Add("Resistência acima de " + DoubleToStr("%4.3f", r_pu_default));
            utc->r_pu = r_pu_default;
        }
        // proteção: verifica reatância nula
        if (IsDoubleZero(utc->x_pu))
        {
            if (plog)
                plog->Add("Reatância nula");
            // assume valor default
            utc->x_pu = x_pu_default;
        }
        // proteção: verifica reatância alta
        else if (utc->x_pu > x_pu_default)
        {
            if (plog)
                plog->Add("Reatância acima de " + DoubleToStr("%4.3f", x_pu_default));
            utc->x_pu = x_pu_default;
        }
        // proteção
        if (IsDoubleZero(utc->perda_fe_perc))
        {
            if (plog)
                plog->Add("Perda no ferro nula");
            // assume valor default
            utc->perda_fe_perc = perda_fe_perc_default;
        }
        else if (utc->perda_fe_perc > 1.5)
        { // gera mensagem de erro
            if (plog)
                plog->Add("UTC com perda no ferro acima de 1.5 %");
            // assume valor default
            utc->perda_fe_perc = perda_fe_perc_default;
        }
        // insere Utc na Et
        if (!et->InsereUtc(utc))
            return (true);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::IsBarraIsolada(VTBarra *barra)
{
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    TList *lisLig = new TList;
    bool isolada = false;

    try
    {
        // protecao
        if (redes == NULL)
        {
            return false;
        }
        redes->LisLigacao(lisLig, barra);
        if (lisLig->Count == 0)
        {
            isolada = true;
        }
    }
    catch (...)
    {
        // nao sei haha
    }
    DestroiTObject(lisLig);
    return isolada;
}
/*
 // ---------------------------------------------------------------------------
 bool __fastcall TPreMonta::PossuiTrechoParalelo(TCircuito *circuito, VTTrecho *trecho)
 {
 VTBarra *barra1 = NULL;
 VTBarra *barra2 = NULL;
 VTBarra *barraAux = NULL;
 TList *lisTrechoBarraComum, *lisLIG;
 // VTTrecho *trechoAux;
 bool possuiTrechoParalelo = false;
 bool possuiBarra1 = false;
 bool possuiBarra2 = false;
 VTLigacao *ligacao;
 // VTRede *rede;

 try
 { // protecao
 if ((circuito == NULL) || (trecho == NULL))
 return(false);
 // seta a barra referencia
 barra1 = trecho->pbarra1;
 barra2 = trecho->pbarra2;
 // protecao
 if (barra1 == barra2)
 return (false);
 // percorre todos trechos do circuito procurando os que possuem barra em comum
 lisLIG = circuito->rede->LisLigacao();
 // rede = circuito->rede;
 for (int nt = 0; nt < lisLIG->Count; nt++)
 {
 ligacao = (VTLigacao*)lisLIG->Items[nt];
 if (ligacao == trecho)
 { // verifica as barras1
 for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
 {
 if ((barraAux = ligacao->Barra(ind_bar)) == barra1)
 {
 possuiBarra1 = true;
 break;
 }
 }
 }
 // se possui a barra 1, verifica se possui a barra 2
 if (possuiBarra1 == true)
 {
 // verifica as barras2
 for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
 {
 if ((barraAux = ligacao->Barra(ind_bar)) == barra2)
 { // possuindo as 2 barras, tem paralelo
 possuiTrechoParalelo = true;
 break;
 }
 }
 }
 // reinicia o bool
 possuiBarra1 = false;
 // caso tenha as duas barras, pode parar de buscar
 if (possuiTrechoParalelo == true)
 break;
 }
 }
 catch (Exception &e)
 {
 int a = 0;
 }
 return possuiTrechoParalelo;
 }
 */
// ---------------------------------------------------------------------------
// void __fastcall TPreMonta::RemoveTrechosCurtoParalelo(void)
// {
// VTLigacao *ligacao;
// TCircuito *circuito;
// TSe *se;
// VTRede *rede;
// VTTrecho *trecho;
// int debug = 0;
//
// try
// {
// // loop para todas Se
// for (int i = 0; i < lisSE->Count; i++)
// {
// se = (TSe*)lisSE->Items[i];
// // loop para todos Circuitos da Se
// for (int j = 0; j < se->lisCIRCUITO->Count; j++)
// {
// circuito = (TCircuito*)se->lisCIRCUITO->Items[j];
// rede = circuito->rede;
// //percorre todos trechos do circuito
// for(int nt = (rede->LisLigacao()->Count - 1); nt >= 0; nt--)
// {
// ligacao = (VTLigacao*)rede->LisLigacao()->Items[nt];
// if(ligacao->Tipo() != eqptoTRECHO)
// }
// else
// }
// //caso o trecho seja curto
// if (ligacao->Status[sttTRECHOCURTO] == true)
// {
// if(PossuiTrechoParalelo(circuito, trecho) == true)
// {
// rede->RemoveLigacao(ligacao);
// debug++;
// }
// else
// {   //caso o trecho tenha comprimento "nulo"
// TrataTrechosCurtos(trecho);
// }
// }
// }
// }
// }
// }
// catch(Exception &e)
// {
// int a =0;
// }
// }

// ---------------------------------------------------------------------------
void __fastcall TPreMonta::RemoveTrechosParalelo(void)
{
    VTLigacao *ligacao, *ligacaoAux;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    TList *lisLig;
    VTRede *rede;
    TList *lisLigComum;
    VTBarra *barra1, *barra2, *barraAux1, *barraAux2;

    try
    { // cria listas
        lisLig = new TList;
        lisLigComum = new TList;
        for (int nr = 0; nr < redes->LisRede()->Count; nr++)
        {
            rede = (VTRede*)redes->LisRede()->Items[nr];
            // limpa lista
            lisLig->Clear();
            rede->LisLigacao(lisLig, eqptoTRECHO);
            // percorre todas ligacoes
            for (int nl = (lisLig->Count - 1); nl >= 0; nl--)
            {
                ligacao = (VTLigacao*) lisLig->Items[nl];
                if (ligacao->Tipo() != eqptoTRECHO)
                {
                    continue;
                }
                barra1 = ligacao->pbarra1;
                barra2 = ligacao->pbarra2;
                // compara essa ligacao com todas as outras
                for (int nla = (nl - 1); nla >= 0; nla--)
                {
                    ligacaoAux = (VTLigacao*) lisLig->Items[nla];
                    if (ligacao == ligacaoAux)
                    {
                        continue;
                    }
                    barraAux1 = ligacaoAux->pbarra1;
                    barraAux2 = ligacaoAux->pbarra2;
                    // verifica se as barras são iguais
                    if ((barra1 == barraAux1) && (barra2 == barraAux2))
                    {
                        rede->RemoveLigacao(ligacao);
                        lisLig->Remove(ligacao);
                        // se estiver na lista de trechos com arranjo errado, tira
                        lisTRECHO_ALT->Remove(ligacao);
                    }
                    else if ((barra1 == barraAux2) && (barra2 == barraAux1))
                    {
                        rede->RemoveLigacao(ligacao);
                        lisLig->Remove(ligacao);
                        // se estiver na lista de trechos com arranjo errado, tira
                        lisTRECHO_ALT->Remove(ligacao);
                    }
                }
            }
        }
        // destroi listas
        DestroiTObject(lisLig);
        DestroiTObject(lisLigComum);
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
    }
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::Sucesso(void)
{
    return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TPreMonta::TrataInicioDeDados(void)
{
    try
    { // reinicia objetos locais
        se = NULL;
        circuito = NULL;
        et = NULL;
        // destrói objetos Se
        LimpaTList(lisSE);
        // inicia listas
        lisEQP->Clear();
        lisTRECHO_ALT->Clear();
        lisSocorro->Clear();
        // se ainda não existe, cria um Dominio
        IniciaDominio();
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18417177
bool __fastcall TPreMonta::TrataFimDeDados(void)
{
    try
    { // cria disjuntor no início de cada primário
        // arranjos->LisArranjo()->Count;
        InserePri_DisjuntorFicticio();
        // remove os trechos em paralelo
        RemoveTrechosParalelo();
        // Conecta chaves vis
        ConectaSocorro();
        // Insere suprimento nos alimentadores sem trafo
        TrataSESemTrafo();
        //insere trafo zigzag
        TrataTrafoZZ();
        // reinicia objetos locais
        se = NULL;
        circuito = NULL;
        et = NULL;

        // obs.: a ordem aqui é importante, pois para tratar os trechos com arranjo do trecho a montante,
        // precisa passar pelo monta, depois trata e por fim destroi lista de SE

        // executa tratamento final do MontaRede
        monta->TrataFimDeDados();
        // adota arranjo do trecho a montante, se definido
        TrataTrechoComArranjoMontante();
        // destrói objetos Se
        LimpaTList(lisSE);

        // teste!
        // TrataZcc();
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(ERRO_DE_SISTEMA);
        return (false);
    }
    return (true);
}

// -----------------------------------------------------------------------------
void __fastcall TPreMonta::TrataSESemTrafo(void)
{ // variáveis locais
    TSe *se;
    TCircuito *circuito;
    VTBarra *barraIni;

    // procura TSe
    for (int n = 0; n < lisSE->Count; n++)
    {
        se = (TSe*)lisSE->Items[n];
        // verifica se a se possui trafos
        if ((se->lisTRF->Count == 0) && (se->lisTRF3E->Count == 0))
        {
            // percorre todos circuitos
            for (int nc = 0; nc < se->lisCIRCUITO->Count; nc++)
            {
                circuito = (TCircuito*)se->lisCIRCUITO->Items[nc];
                barraIni = circuito->rede->BarraInicial();
                if (barraIni == NULL)
                {
                    continue;
                }
                // se nao existir suprimento
                if (!barraIni->ExisteGeracao())
                {
                    InsereEtd_Suprimento(barraIni);
                }
            }
        }
    }
}
// -------------------------------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18417177
void __fastcall TPreMonta::TrataTrafoZZ(void)
{
    // variáveis locais
    TSe *sub;
    TInsereTrafoZZ *insere = new TInsereTrafoZZ(apl);

    for(int n = 0; n < lisSE->Count; n++)
    {
        sub = (TSe*)lisSE->Items[n];
        insere->Executa(sub->lisTRF);
    }

    delete insere;
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18236926
void __fastcall TPreMonta::TrataTrechoComArranjoMontante(void)
{
    // variáveis locais
    VTArranjo *arranjo_novo;
    VTTrecho *trecho, *original, *pai;
    VTNet *net;
    VTOrdena *ordena;
    int fase_tag, id_arranjo;
    bool ramal;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

    // verifica se a opção estava marcada
    if (!empresa->ArranjoMontante)
        return;
    // verifica se há trechos a serem alterados
    if (lisTRECHO_ALT->Count == 0)
        return;
    // cria objeto Ordena, para preencher ligaPai de cada ligação
    try
    {
        ordena = DLL_NewObjOrdena(apl);
        // ordena->TramoEnabled = true;
        net = ordena->Executa(redes);

		for (int n = 0; n < lisTRECHO_ALT->Count; n++)
        {
            original = (VTTrecho*)lisTRECHO_ALT->Items[n];
            trecho = (VTTrecho*)redes->ExisteEqpto(eqptoTRECHO, IntToStr(original->Id));
            if (trecho == NULL)
                continue;
            pai = TrechoPai(trecho);
            if (pai == NULL)
                continue;
            // determina dados do arranjo anterior
            fase_tag = trecho->arranjo->Fases;
            // determina arranjo do pai
            id_arranjo = pai->arranjo->Id - ARRANJO_ID;
            ramal = pai->arranjo->ramal;
            arranjo_novo = ExisteArranjo(id_arranjo, fase_tag, ramal);
            if (arranjo_novo != NULL)
            {
                trecho->DefineObjArranjo(arranjo_novo);
            }
        }
        lisTRECHO_ALT->Clear();
        // destrói objetos
		DLL_DeleteEqpto(net);
        delete ordena;
    }
    catch (...)
    { // destrói objetos
        if (net)
            DLL_DeleteEqpto(net);
        if (ordena)
            delete ordena;
        return;
    }
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18236926
VTTrecho* __fastcall TPreMonta::TrechoPai(VTLigacao *ligacao)
{
    // determina trecho pai
    if (ligacao == NULL)
        return NULL;
    if (ligacao->ligaPai == NULL)
        return NULL;
    if (ligacao->ligaPai->Tipo() == eqptoTRECHO)
        return (VTTrecho*)ligacao->ligaPai;
    // chamada recursiva
    return TrechoPai(ligacao->ligaPai);
}

// -----------------------------------------------------------------------------
// eof

// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------------------- includes
#include <algorithm>
#include <DateUtils.hpp>
#include <Classes.hpp>
#include <TypInfo.hpp>

// Plataforma include
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Diretorio\VTDir.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Flow\VTFlow.h>
#include <PlataformaSinap\Fontes\Cronometro\VTCronometro.h>
#include <PlataformaSinap\Fontes\Progresso\VTAborta.h>
#include <PlataformaSinap\Fontes\Progresso\VTProgresso.h>
#include <PlataformaSinap\Fontes\Cronometro\VTCronometro.h>
#include <PlataformaSinap\Fontes\Geral\VTGeral.h>
#include <PlataformaSinap\Fontes\Geral\VTInfoset.h>
#include <PlataformaSinap\Fontes\Sinap\VTSinapChild.h>
#include <PlataformaSinap\DLL_Inc\Cronometro.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Help.h>
#include <PlataformaSinap\DLL_Inc\Progresso.h>

// Project include
#include <Egrid\TImportaArquivoEgrid.h>
#include <Exceptions\EImportaEgrid.h>
#include <Empresa\VTCondutor.h>
#include <Empresa\VTSubestacao.h>
#include <Importa\VTConversor.h>
#include <Empresa\VTDadosEmpresa.h>
#include <Empresa\VTLocalidade.h>
#include <Empresa\VTRegional.h>
#include <Empresa\VTPrimario.h>
// #include <Importa\TLigaMaeFilha.h>

#include <TFormImportadorEgrid.h>
#include <TRelatorioValidacao.h>

// --------------------------------------------------------------------------------------- namespace
//using namespace sinapsis::sinap::edp;

// ------------------------------------------------------------------------------------------- class

// -------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18496139
//https://sinapsisenergia.teamwork.com/#/tasks/18497425
__fastcall TFormImportadorEgrid::TFormImportadorEgrid(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
                                :TForm(Owner)
{
    // variáveis locais
    VTSinapChild    *sinap_child = (VTSinapChild*)apl_owner->GetObject(__classid(VTSinapChild));
    VTPath          *path;

    //TImportaRedeGis já criou um VTDadosEmpresa
    empresa = (VTDadosEmpresa*)apl_owner->GetObject(__classid(VTDadosEmpresa));
    // cria objeto Apl local
    apl = NewObjApl(this, apl_owner);
    // // cria objeto Progresso
    progresso = (VTProgresso*)apl_owner->GetObject(__classid(VTProgresso));
    if (progresso != NULL)
    {
		apl->Add(progresso = DLL_NewObjProgresso(apl));
    }
    // cria objeto Empresa
    //	apl->Add(empresa = NewObjDadosEmpresa(apl));
    // cria o conversor
    apl->Add(conversor = NewObjConversor(apl));
    // cria ligaMaeFilha
    // ligaMaeFilha = (TLigaMaeFilha*)apl->GetObject(__classid(TLigaMaeFilha));
    // if (ligaMaeFilha == NULL)
    // {
    // apl->Add(ligaMaeFilha = new TLigaMaeFilha(apl)); // FKM
    // }
    // cria listas
    lisTMP = new TList();

    // inicia DateTimePicker c/ data atual
    DateTimePicker->Date = DateOf(Now());

    // exibe o gráfico de topologia
    if (sinap_child)
        sinap_child->ActionTopo->Execute();

    // posiciona Form
    FormIniciaPosicao();
    FormLeOpcoes();

    // adiciona a pasta exporta para os relatorios de eqptos
    // e também a pasta para exportar as redes
    path = (VTPath*)apl->GetObject(__classid(VTPath));
    EditDirRelEqpto->Text  = path->DirExporta();
    EditPastaPublica->Text = ExtractFilePath(path->ArqRede);

    // define IDs dos cabos default
    oracle.cabo_mt_id = 0;
    oracle.cabo_bt_id = 0;

    // cria objeto VTCronometo
    cronometro = DLL_NewObjCronometro(apl);
}

// -------------------------------------------------------------------------------------------------
__fastcall TFormImportadorEgrid::~TFormImportadorEgrid(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva informações da tela
	if (geral)
	{ // salva parâmetros do Form
		geral->Infoset->SaveToFile(this);
		FormSalvaOpcoes();
	}

	// destroi listas sem destruir objetos
	if (lisTMP)
	{
		delete lisTMP;
		lisTMP = NULL;
	}

	// remove objeto Cronometro do Apl
	if (apl)
		apl->Remove(cronometro);

	// destrói objetos
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
	if (cronometro)
	{
		delete cronometro;
		cronometro = NULL;
	}

}

// -------------------------------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
void __fastcall TFormImportadorEgrid::ActionAbortaExecute(TObject *Sender)
{
    // variáveis locais
    VTAborta *aborta = (VTAborta*)apl->GetObject(__classid(VTAborta));

    if(aborta != NULL)
    {
        if(Confirma("A importação das redes será interrompida.", "Deseja mesmo interromper ?") == idYes)
        {
            aborta->Requested = true;
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::ActionAbrirEgridExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString fileName;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// oculta o form para exibir a caixa de dialogo de abrir arquivo
	Hide();
	// solicita o arquivo a ser importado
	// TOpenDialog *dlgOpen = new TOpenDialog(this);
	dlgOpen->Filter = "extracted grid (*.egrid)|*.EGRID|";
	if (dlgOpen->InitialDir.IsEmpty())
	{
		dlgOpen->InitialDir = path->DirImporta();
	}
	if (dlgOpen->Execute())
	{
		fileName = dlgOpen->FileName;
	}
	// verifica se o usuario selecionou o arquivo
	if (fileName.IsEmpty())
	{
		Aviso("Selecione o arquivo a ser importado!");
		Show();
		return;
	}
	//
	// abre o arquivo
	//
	try
	{
		//
		// carrega as relacoes mae->filhas e os cabos
		//
		empresa->LisRegional()->Clear();
		empresa->LisLocalidade()->Clear();
		empresa->LisSubestacao()->Clear();
		empresa->LisPrimario()->Clear();
		// empresa->LisCondutor()->Clear();
		conversor->ImportaDadosEmpresa(fileName);
		CLBoxRegionalInicia();
		CLBoxLocalidadeInicia();
		// inicia CheckListBox de Subestação
		CLBoxSubestacaoInicia();
		// inicia LBoxPrimarioAEreo e Subterraneo
		CLBoxPrimarioInicia();
		// LBoxPrimarioInicia();
		// inicia CBox dos condutores
		// CBoxCondutorInicia(CBoxCondutorMT, empresa->LisCondutorMT(), oracle.cabo_mt_id);
		// CBoxCondutorInicia(CBoxCondutorBT, empresa->LisCondutorBT(), oracle.cabo_bt_id);
		CBoxCondutorInicia(CBoxCondutorBT, empresa->LisCondutor(), 47);
		CBoxCondutorInicia(CBoxCondutorMT, empresa->LisCondutor(), 261);
		CBoxCondutorInicia(CBoxCondutorIP, empresa->LisCondutor(), 358);
		CBoxCondutorInicia(CBoxCondutorRL, empresa->LisCondutor(), 201);
		// verifica se há alguma opção de "Não utilizar" um Condutor
		if ((CBoxCondutorMT->ItemIndex == 0) || (CBoxCondutorBT->ItemIndex == 0))
		{
			Aviso("Está selecionada opção de não utilizar condutor default\n"
				"em caso de trecho com condutor não definido");
		}
		// habilita o botao para importar as subestacoes
		ActionIniciarImportacao->Enabled = true;
		ActionSubestacaoSelAll->Enabled = true;
		ActionSubestacaoSelNone->Enabled = true;
		ActionSubestacaoSelAll->Visible = true;
	}
	catch (EImportaEgrid &e)
	{
		Erro("Erro ao importar dados da empresa, revise o egrid utilizado.");
		Show();
		if (progresso)
		{
			progresso->Stop();
		}
		return;
	}
	catch (const runtime_error &e)
	{
		Erro((AnsiString)e.what());
		Show();
		if (progresso)
		{
			progresso->Stop();
		}
		return;
	}
	// exibe o form
	Show();

}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::ActionFechaExecute(TObject *Sender)
{
	Close();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::ActionHelpExecute(TObject *Sender)
{
	// variáveis locais
	TForm *FormHelp;

	// exibe arquivo de ajuda do módulo
	if ((FormHelp = DLL_NewFormHelp(this, apl, "Importador_de_redes_GSE")) != NULL)
	{
		FormHelp->Show();
	}
}

// -------------------------------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
void __fastcall TFormImportadorEgrid::ActionIniciarImportacaoExecute(TObject *Sender)
{
	// variáveis locais
    VTAborta *aborta = (VTAborta*)apl->GetObject(__classid(VTAborta));
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	int count_rede = 0;

	lisTMP->Clear();
	// preenche lisTMP com as subestações selecionadas
	if (LisObjectChecked(CLBoxSubestacao, lisTMP) == 0)
		return;

	// this->Unzip();

	// valida opções selecionadas
	if (!ValidaOpcoesDeImportacao())
		return;
	// btnImportar->Enabled = false;

    //ativa botão de abortar
    ActionAborta->Enabled = true;
    //desativa outros botoes
    EnableBut(false);

	if (ImportaSubestacao())
	{ // guarda quantidade de redes importadas
		count_rede = redes->LisRede()->Count;
		// verifica se a opção de importação é só pra salvar as redes
		if (RadioGroupDestino->ItemIndex > 0)
		{ // limpa redes da memória
			Aviso("Importação concluída com sucesso");
			redes->Clear();
			count_rede = 0;
		}
		// copia bases das redes para pasta publica
		// CopiaRedesParaPastaPublica();
		// exibe o gráfico de topologia
		grafico->Show(grafTOPO);
		// atualiza o gráfico
		grafico->AreaRedeDefine();
		// marca que o fluxo não está atualziado
		flow->UpToDate = false;
		// exibe resultado do Cronometro
		if (CheckBoxCronometro->Checked)
			cronometro->Show();
		// fecha o Form, indicando se há redes carregadas ou não
		if (count_rede > 0)
			ModalResult = mrOk;
		else
			ModalResult = mrCancel;
	}
    if((aborta) && (aborta->Requested))
    {
        aborta->Requested = false;
        //limpa redes se foi abortado
        ReiniciaRede(false);
        //desativa botão de abortar
        ActionAborta->Enabled = false;
        //ativa outros botoes
        EnableBut(true);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::ActionLocalidadeSelAllExecute(TObject *Sender)

{
	// seleciona todas as Localidades
	CheckAll(CLBoxLocalidade, true);
	// reinicia CLBoxSubestacao
	CLBoxSubestacaoInicia();
	CLBoxPrimarioInicia();
}
// ---------------------------------------------------------------------------

void __fastcall TFormImportadorEgrid::ActionLocalidadeSelNoneExecute(TObject *Sender)

{
	// cancela seleção de todas as Localidades
	CheckAll(CLBoxLocalidade, false);
	// reinicia CLBoxSubestacao
	CLBoxSubestacaoInicia();
	CLBoxPrimarioInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::ActionPrimarioSelAllExecute(TObject *Sender)
{
	// variáveis locais
	VTPrimario *primario;

	// seleciona todos os Primarios
	for (int n = 0; n < CLBoxPrimario->Items->Count; n++)
	{
		CLBoxPrimario->Checked[n] = true;
		primario = (VTPrimario*)CLBoxPrimario->Items->Objects[n];
		primario->Selecionado = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::ActionPrimarioSelNoneExecute(TObject *Sender)
{
	// variáveis locais
	VTPrimario *primario;

	// cancela seleção de todos os Primarios
	for (int n = 0; n < CLBoxPrimario->Items->Count; n++)
	{
		CLBoxPrimario->Checked[n] = false;
		primario = (VTPrimario*)CLBoxPrimario->Items->Objects[n];
		primario->Selecionado = false;
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::ActionRedeClearExecute(TObject *Sender)
{
	// var local
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// solicita aviso p/ apagar redes
	if (!ReiniciaRede(true))
		return;
	// atualiza o gráfico
	grafico->AreaRedeDefine();

}

// ---------------------------------------------------------------------------

void __fastcall TFormImportadorEgrid::ActionRegionalSelAllExecute(TObject *Sender)

{
	// seleciona todas as Regionais
	CheckAll(CLBoxRegional, true);
	// reinicia CLBoxLocalidade e CLBoxSubestacao
	CLBoxLocalidadeInicia();
	CLBoxSubestacaoInicia();
	CLBoxPrimarioInicia();
}
// ---------------------------------------------------------------------------

void __fastcall TFormImportadorEgrid::ActionRegionalSelNoneExecute(TObject *Sender)

{
	// cancela seleção de todas as Regionais
	CheckAll(CLBoxRegional, false);
	// reinicia CLBoxLocalidade e CLBoxSubestacao
	CLBoxLocalidadeInicia();
	CLBoxSubestacaoInicia();
	CLBoxPrimarioInicia();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::ActionSelPastaRelatorioEqptoExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString dirname;
	VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// esconde este Form
	Visible = false;
	// verifica se existe uma pasta selecionada
	dirname = EditDirRelEqpto->Text;
	if (dirname.IsEmpty())
	{ // assume pasta default: Redes
		dirname = path->DirExporta();
	}
	// verifica se é uma pasta válida
	if (!DirectoryExists(dirname))
		dirname = "";
	// exibe janela para selecionar a pasta
	if (dir->SelDir(dirname))
		EditDirRelEqpto->Text = dirname;
	else
		EditDirRelEqpto->Text = path->DirExporta();
	// torna a exibir este Form
	Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::ActionSelPastaPublicaExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString dirname;
	VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// esconde este Form
	Visible = false;
	// verifica se existe uma pasta selecionada
	dirname = EditPastaPublica->Text;
	if (dirname.IsEmpty())
	{ // assume pasta default: Redes
		dirname = path->DirRedes();
	}
	// verifica se é uma pasta válida
	if (!DirectoryExists(dirname))
		dirname = "";
	// exibe janela para selecionar a pasta
	if (dir->SelDir(dirname))
		EditPastaPublica->Text = dirname;
	else
		EditPastaPublica->Text = "";
	// torna a exibir este Form
	Visible = true;
}

// --------------------------------------------------------------------------------------------- eof
void __fastcall TFormImportadorEgrid::ActionSubestacaoSelAllExecute(TObject *Sender)
{
	// seleciona todas as Subestacoes
	CheckAll(CLBoxSubestacao, true);
	// reinicia CLBoxPrimario
	// LBoxPrimarioInicia();
	CLBoxPrimarioInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::ActionSubestacaoSelNoneExecute(TObject *Sender)
{
	// cancela seleção de todas as Subestacoes
	CheckAll(CLBoxSubestacao, false);
	// reinicia CLBoxPrimario
	// LBoxPrimarioInicia();
	CLBoxPrimarioInicia();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::CBoxCondutorInicia(TComboBox *CBox, TList *lisCONDUTOR,
	int condutor_id)
{
	// variáveis locais
	int index;
	// AnsiString codigo;
	VTCondutor *condutor;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	AnsiString nome_arq;
	int valor;

	// reinicia CBox
	CBox->Clear();
	// insere opção de não assumir um Condutor default
	CBox->Items->AddObject("Não utilizar", NULL);
	for (int n = 0; n < lisCONDUTOR->Count; n++)
	{
		condutor = (VTCondutor*)lisCONDUTOR->Items[n];
		// define o código, verificando se existe o cabo de neutro
		// codigo = IntToStr(condutor->id) + " = " + condutor->codigo;
		index = CBox->Items->AddObject(condutor->codigo, condutor);
		// index = CBox->Items->AddObject(codigo, condutor);
		// seleciona Condutor indicado
		if (condutor->id == condutor_id)
			CBox->ItemIndex = index;
		continue;
	}
	// verifica se foi selecionado um Condutor
	if (CBox->ItemIndex < 0)
	{ // verifica se condutor_id é nulo, o que indica opção "Não utilizar"
		if (condutor_id == 0)
			CBox->ItemIndex = 0;
	}
	try
	{ // define nome do arquivo
		nome_arq = "OpcoesImportadorEgrid";
		if (geral)
		{ // lê parâmetros do Form
			if (!geral->Infoset->LoadFromFile(nome_arq))
				return;
			geral->Infoset->GetInfo(CBox->Name, valor);
			// verifica se o index existe
			if ((valor < CBox->Items->Count) && (valor >= 0))
				CBox->ItemIndex = valor;
		}
	}
	catch (Exception &e)
	{
		Aviso("Erro ao carregar opções de condutores");
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::CheckAll(TCheckListBox *CLBox, bool checked)
{
	for (int n = 0; n < CLBox->Items->Count; n++)
	{
		CLBox->Checked[n] = checked;
	}

}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::CLBoxFilhasInicia(TList *lisFilhas)
{
	// // Variáveis locais
	// VTSubestacao *subfilha;
	// VTPrimario *primfilha;
	// TList *lisPrimFilha;
	// int indexf;
	//
	// // Proteçao
	// if (ligaMaeFilha == NULL)
	// return;
	// // verifica se existe filhas
	// if (ligaMaeFilha->lisRelMaeFilha->Count != 0)
	// {
	// // Verifica se lisFilhas é NULL
	// if (lisFilhas != NULL)
	// {
	// // insere todas as filhas no CLBox
	// for (int kf = 0; kf < lisFilhas->Count; kf++)
	// {
	// // analisa a kf-ésima filha em lisFilhas
	// subfilha = (VTSubestacao*)lisFilhas->Items[kf];
	// // proteção inserida pois nem todas as SE's da lista foram extraídas para COELBA #DSF
	// if (subfilha == NULL)
	// {
	// continue;
	// }
	// // percorre todas as subestações do CLBoxSubestacao
	// for (int ic = 0; ic < CLBoxSubestacao->Items->Count; ic++)
	// {
	// // Encontra a subestação filha analisada e dá um check no CLBoxSubestacao
	// if (CLBoxSubestacao->Items->Objects[ic] == subfilha)
	// {
	// CLBoxSubestacao->Checked[ic] = True;
	// }
	// }
	// // analisa os primários da filha analisada
	// lisPrimFilha = subfilha->LisPrimario();
	// // percorre os primários da filha
	// for (int jf = 0; jf < lisPrimFilha->Count; jf++)
	// {
	// primfilha = (VTPrimario*)lisPrimFilha->Items[jf];
	// // verifica se essa rede filha já existe no CLBox - DVC
	// if (CLBoxPrimario->Items->IndexOfObject(primfilha) >= 0)
	// continue;
	// // insere o primário da SE filha em CLBoxPrimario
	// indexf = CLBoxPrimario->Items->AddObject(primfilha->Codigo, primfilha);
	// CLBoxPrimario->Checked[indexf] = primfilha->Selecionado;
	// //
	// }
	// }
	//
	// }
	//
	// }
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::CLBoxPrimarioClickCheck(TObject *Sender)
{
	// variáveis locais
	VTPrimario *primario;
	TCheckListBox *CLBox = CLBoxPrimario;
	bool checked = false;
	int item_check;

	//verifica selecao
	item_check = CLBoxPrimario->ItemIndex;
	if (item_check < 0) {
		return;
	}
	// marca/desmarca seleção
	if (CLBox->Checked[item_check] == false) 
	{
		checked = true;
	}
	else
	{
		checked = false;
	}
	CLBox->Checked[item_check] = checked;

	// determina primario
	primario = (VTPrimario*)CLBoxPrimario->Items->Objects
		[item_check];
	// marca/desmarca seleção
	primario->Selecionado = CLBoxPrimario->Checked[item_check];
}

// -------------------------------------------------------------------------------------------------

void __fastcall TFormImportadorEgrid::CLBoxPrimarioInicia(void)
{
	// variáveis locais
	VTPrimario *primario;
	TList *lisEXT = empresa->LisPrimario();
	int index;

	// reinicia CLBoxPrimario
	CLBoxPrimario->Clear();
	// monta lista com as Subestações selecionadas
	if (LisObjectChecked(CLBoxSubestacao, lisTMP) == 0)
		return;
	// insere em CLBoxPrimario os Primarios associados a Subestaçoes selecionadas
	for (int n = 0; n < lisEXT->Count; n++)
	{
		primario = (VTPrimario*)lisEXT->Items[n];
		// verifica se a Subestacao do Primario está na lista lisTMP
		if ((primario->Subestacao != NULL) && (lisTMP->IndexOf(primario->Subestacao) >= 0))
		{ // insere Primario em CLBoxPrimario
			index = CLBoxPrimario->Items->AddObject(primario->Nome, primario);
			// Verifica se estava com check
			CLBoxPrimario->Checked[index] = primario->Selecionado;
		}
	}
}

// ---------------------------------------------------------------------------

void __fastcall TFormImportadorEgrid::CLBoxRegionalClickCheck(TObject *Sender)
{
	TCheckListBox *CLBox = CLBoxRegional;
	bool checked = false;
	int item_check;

	//verifica selecao
	item_check = CLBoxRegional->ItemIndex;
	if (item_check < 0) {
		return;
	}
	// marca/desmarca seleção
	if (CLBox->Checked[item_check] == false) {
		checked = true;
	}
	else
	{
		checked = false;
	}
	CLBox->Checked[item_check] = checked;
	// exibe Localidades das Regionais selecionadas
	CLBoxLocalidadeInicia();
	// exibe Subestações  das Localidades selecionadas
	CLBoxSubestacaoInicia();
	// exibe Primários das Subestações selecionadas
	CLBoxPrimarioInicia();
	// LBoxPrimarioInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::CLBoxRegionalInicia(void)
{
	// variáveis locais
	VTRegional *regional;
	TList *lisEXT = empresa->LisRegional();

	// reinicia CLBoxRegional
	CLBoxRegional->Clear();
	// CLBoxRegional->Items->BeginUpdate();
	// insere todas Regionais em CLBoxRegional
	for (int n = 0; n < lisEXT->Count; n++)
	{
		regional = (VTRegional*)lisEXT->Items[n];
		CLBoxRegional->Items->AddObject(regional->Nome, regional);
	}
	CLBoxRegional->Sorted = true;
	// CLBoxRegional->Items->EndUpdate();
}
// ---------------------------------------------------------------------------

void __fastcall TFormImportadorEgrid::CLBoxLocalidadeClickCheck(TObject *Sender)
{
	TCheckListBox *CLBox = CLBoxLocalidade;
	bool checked = false;
	int item_check;

	//verifica selecao
	item_check = CLBoxLocalidade->ItemIndex;
	if (item_check < 0) {
		return;
	}

	// marca/desmarca seleção
	if (CLBox->Checked[item_check] == false) {
		checked = true;
	}
	else
	{
		checked = false;
	}
	CLBox->Checked[item_check] = checked;
	// exibe Subestações  das Localidades selecionadas
	CLBoxSubestacaoInicia();
	// exibe Primários das Subestações selecionadas
	CLBoxPrimarioInicia();
	// LBoxPrimarioInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::CLBoxLocalidadeInicia(void)
{
	// variáveis locais
	VTLocalidade *localidade;
	TList *lisEXT = empresa->LisLocalidade();

	// reinicia CLBoxLocalidade
	CLBoxLocalidade->Clear();
	// CLBoxLocalidade->Items->BeginUpdate();
	// lisTMP->Clear();
	// monta lista com as Regionais selecionadas
	if (LisObjectChecked(CLBoxRegional, lisTMP) == 0)
		return;
	// insere em CLBoxLocalidade as Localidades associadas a Regionais selecionadas
	for (int n = 0; n < lisEXT->Count; n++)
	{
		localidade = (VTLocalidade*)lisEXT->Items[n];
		// verifica se a Regional da Localidade está na lista lisTMP
		if (lisTMP->IndexOf(localidade->Regional) >= 0)
		{ // insere Localidade em CLBoxLocalidade
			CLBoxLocalidade->Items->AddObject(localidade->Nome, localidade);
		}
	}
	CLBoxLocalidade->Sorted = true;
	CLBoxLocalidade->Visible = true;
	// CLBoxLocalidade->Items->EndUpdate();
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::CLBoxSubestacaoClickCheck(TObject *Sender)
{
	TCheckListBox *CLBox = CLBoxSubestacao;
	VTSubestacao *sub;
	bool checked = false;
	int item_check;

	//verifica selecao
	item_check = CLBoxSubestacao->ItemIndex;
	if (item_check < 0) {
		return;
	}
	// marca/desmarca seleção
	if (CLBox->Checked[item_check] == false) {
		checked = true;
	}
	else
	{
		checked = false;
	}
	CLBox->Checked[item_check] = checked;
	CLBoxPrimarioInicia();
	// LBoxPrimarioInicia();

}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::CLBoxSubestacaoInicia(void)
{
	// variáveis locais
	VTSubestacao *subestacao;
	TList *lisEXT = empresa->LisSubestacao();

	// reinicia CLBoxSubestacao
	CLBoxSubestacao->Clear();
	// CLBoxSubestacao->Items->BeginUpdate();
	// lisTMP->Clear();
	// monta lista com as Localidades selecionadas
	if (LisObjectChecked(CLBoxLocalidade, lisTMP) == 0)
		return;
	// insere em CLBoxSubestacao as Subestaçoes associadas a Localidades selecionadas
	for (int n = 0; n < lisEXT->Count; n++)
	{
		subestacao = (VTSubestacao*)lisEXT->Items[n];
		// verifica se a Localidade da Subestacao está na lista lisTMP
		if ((subestacao->Localidade == NULL) || (lisTMP->IndexOf(subestacao->Localidade) >= 0))
		{ // insere Localidade em CLBoxLocalidade
			CLBoxSubestacao->Items->AddObject(subestacao->Nome, subestacao);
		}
	}
	CLBoxSubestacao->Sorted = true;
	// CLBoxSubestacao->Items->EndUpdate();
	CLBoxSubestacao->Visible = true;
}
// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::CheckBoxRelEqptoExit(TObject *Sender)
{
    EnableOpcaoRelEqpto(CheckBoxRelEqpto->Checked);
}

// -------------------------------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
void __fastcall TFormImportadorEgrid::EnableBut(bool enabled)
{
    //atualiza enabled de cada botao, exceto abortar
    ActionAbrirEgrid->Enabled = enabled;
    ActionFecha->Enabled = enabled;
    ActionHelp->Enabled = enabled;
    ActionIniciarImportacao->Enabled = enabled;
    ActionLocalidadeSelAll->Enabled = enabled;
    ActionLocalidadeSelNone->Enabled = enabled;
    ActionPrimarioSelAll->Enabled = enabled;
    ActionPrimarioSelNone->Enabled = enabled;
    ActionRedeClear->Enabled = enabled;
    ActionRegionalSelAll->Enabled = enabled;
    ActionRegionalSelNone->Enabled = enabled;
    ActionSelPastaPublica->Enabled = enabled;
    ActionSelPastaRelatorioEqpto->Enabled = enabled;
    ActionSubestacaoSelAll->Enabled = enabled;
    ActionSubestacaoSelNone->Enabled = enabled;
}

//---------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::EnableOpcaoRelEqpto(bool enable)
{
	CheckBoxSalvarRede->Enabled = enable;
	BitBtnSelDirRelEqpto->Enabled = enable;
	EditDirRelEqpto->Enabled = enable;
}
// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorEgrid::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
}

// -------------------------------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18236926
void __fastcall TFormImportadorEgrid::FormLeOpcoes(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	AnsiString nome_arq;
	int valor;
	AnsiString texto;

	try
	{ // define nome do arquivo
		nome_arq = "OpcoesImportadorEgrid";
		if (geral)
		{ // lê parâmetros do Form
			if (!geral->Infoset->LoadFromFile(nome_arq))
				return;
			geral->Infoset->GetInfo("RadioGroupDestino", valor);
			RadioGroupDestino->ItemIndex = valor;

			geral->Infoset->GetInfo("EditPastaPublica", texto);
			EditPastaPublica->Text = texto;

			geral->Infoset->GetInfo("RadioGroupConsPrimario", valor);
			RadioGroupConsPrimario->ItemIndex = valor;

			geral->Infoset->GetInfo("CheckBoxLog", valor);
			CheckBoxLog->Checked = (valor == 1);

			geral->Infoset->GetInfo("CheckBoxCronometro", valor);
			CheckBoxCronometro->Checked = (valor == 1);

			geral->Infoset->GetInfo("RadioGroupRedeSec", valor);
			RadioGroupRedeSec->ItemIndex = valor;

			geral->Infoset->GetInfo("CheckBoxValidaFase", valor);
			CheckBoxValidaFase->Checked = (valor == 1);

			geral->Infoset->GetInfo("CheckBoxCorrigeFase", valor);
			CheckBoxCorrigeFase->Checked = (valor == 1);

			geral->Infoset->GetInfo("CheckBoxCorrigePerdaFerro", valor);
			CheckBoxCorrigePerdaFerro->Checked = (valor == 1);

//			geral->Infoset->GetInfo("CheckBoxCorrigeMalhaMTs", valor);
//			CheckBoxCorrigeMalhaMTs->Checked = (valor == 1);

			geral->Infoset->GetInfo("CheckBoxAbrirChaveExt", valor);
			CheckBoxAbrirChaveExt->Checked = (valor == 1);

			geral->Infoset->GetInfo("CheckBoxAbrirChaveInt", valor);
			CheckBoxAbrirChaveInt->Checked = (valor == 1);

			geral->Infoset->GetInfo("CheckBoxDeslocarTrechoExt", valor);
			CheckBoxDeslocarTrechoExt->Checked = (valor == 1);

			geral->Infoset->GetInfo("CheckBoxDeslocarTrechoInt", valor);
			CheckBoxDeslocarTrechoInt->Checked = (valor == 1);

            //arranjo do trecho a montante
			geral->Infoset->GetInfo("CBoxArranjoMontante", valor);
            CBoxArranjoMontante->Checked = (valor == 1);

			//infos do relatorio de eqpto
			geral->Infoset->GetInfo("CheckBoxRelEqpto", valor);
			CheckBoxRelEqpto->Checked = (valor == 1);
            geral->Infoset->GetInfo("CheckBoxSalvarRede", valor);
			CheckBoxSalvarRede->Checked = (valor == 1);
			//atualiza o form
			EnableOpcaoRelEqpto(CheckBoxRelEqpto->Checked);

		}
	}
	catch (Exception &e)
	{
		Aviso("Erro ao ler opções de importação");
	}

}

// -------------------------------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18236926
void __fastcall TFormImportadorEgrid::FormSalvaOpcoes(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	AnsiString nome_arq;

	try
	{ // define nome do arquivo
		nome_arq = "OpcoesImportadorEgrid";
		if (geral)
		{ // salva parâmetros do Form
			geral->Infoset->AddInfo("RadioGroupDestino", RadioGroupDestino->ItemIndex);
			geral->Infoset->AddInfo("EditPastaPublica", EditPastaPublica->Text);
			geral->Infoset->AddInfo("RadioGroupConsPrimario", RadioGroupConsPrimario->ItemIndex);
			geral->Infoset->AddInfo("CheckBoxLog", (CheckBoxLog->Checked ? 1 : 0));
			geral->Infoset->AddInfo("CheckBoxCronometro", (CheckBoxCronometro->Checked ? 1 : 0));
			geral->Infoset->AddInfo("RadioGroupRedeSec", RadioGroupRedeSec->ItemIndex);
			geral->Infoset->AddInfo("CheckBoxValidaFase", (CheckBoxValidaFase->Checked ? 1 : 0));
			geral->Infoset->AddInfo("CheckBoxCorrigeFase", (CheckBoxCorrigeFase->Checked ? 1 : 0));
			geral->Infoset->AddInfo("CheckBoxCorrigePerdaFerro",
				(CheckBoxCorrigePerdaFerro->Checked ? 1 : 0));
//			geral->Infoset->AddInfo("CheckBoxCorrigeMalhaMTs",
//				(CheckBoxCorrigeMalhaMTs->Checked ? 1 : 0));

			geral->Infoset->AddInfo("CheckBoxAbrirChaveExt", (CheckBoxAbrirChaveExt->Checked ? 1 : 0));
			geral->Infoset->AddInfo("CheckBoxAbrirChaveInt", (CheckBoxAbrirChaveInt->Checked ? 1 : 0));
			geral->Infoset->AddInfo("CheckBoxDeslocarTrechoExt", (CheckBoxDeslocarTrechoExt->Checked ? 1 : 0));
			geral->Infoset->AddInfo("CheckBoxDeslocarTrechoInt", (CheckBoxDeslocarTrechoInt->Checked ? 1 : 0));

			// condutores
			geral->Infoset->AddInfo("CBoxCondutorMT", CBoxCondutorMT->ItemIndex);
			geral->Infoset->AddInfo("CBoxCondutorBT", CBoxCondutorBT->ItemIndex);
			geral->Infoset->AddInfo("CBoxCondutorIP", CBoxCondutorIP->ItemIndex);
			geral->Infoset->AddInfo("CBoxCondutorRL", CBoxCondutorRL->ItemIndex);

            //arranjo do trecho a montante
			geral->Infoset->AddInfo("CBoxArranjoMontante", (CBoxArranjoMontante->Checked ? 1 : 0));

			//infos do relatorio de eqpto
			geral->Infoset->AddInfo("CheckBoxRelEqpto", (CheckBoxRelEqpto->Checked ? 1 : 0));
			geral->Infoset->AddInfo("CheckBoxSalvarRede", (CheckBoxSalvarRede->Checked ? 1 : 0));

            //salva
            geral->Infoset->SaveToFile(nome_arq);
		}
	}
	catch (Exception &e)
	{
		Aviso("Erro ao salvar opções de importação");
	}
}

// -------------------------------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
bool __fastcall TFormImportadorEgrid::ImportaSubestacao(void)
{
	// variáveis locais
	bool sucesso = false;

	// define pasta para o caso de salvar as redes
	conversor->Pasta = EditPastaPublica->Text;
	// esconde este Form  (não esconde mais por causa do botão que interrompe importação)
//	Visible = false;
	// faz a leitura da base Oracle e montagem das redes
	sucesso = conversor->ImportaRedes(lisTMP, &opcao, RadioGroupDestino->ItemIndex);
	// exibe este Form
//	Visible = true;

	return (sucesso);
}

// ---------------------------------------------------------------------------
// void __fastcall TFormImportadorEgrid::LBoxPrimarioInicia(void)
// {
// // variáveis locais
// VTPrimario *primario;
// TList *lisEXT = empresa->LisPrimario();
//
// // reinicia LBoxPrimario
// LBoxPrimario->Clear();
// LBoxPrimario->Sorted = false;
// // lisTMP->Clear();
// // monta lista com as Subestações selecionadas
// if (LisObjectChecked(CLBoxSubestacao, lisTMP) == 0)
// return;
// // insere em LBoxPrimario os Primarios associados a Subestaçoes selecionadas
// for (int n = 0; n < lisEXT->Count; n++)
// {
// primario = (VTPrimario*)lisEXT->Items[n];
// // verifica se a Subestacao do Primario está na lista lisTMP
// if ((primario->Subestacao != NULL) && (lisTMP->IndexOf(primario->Subestacao) >= 0))
// { // insere Primario em LBoxPrimario
// LBoxPrimario->Items->AddObject(primario->Nome, primario);
// }
// }
// LBoxPrimario->Sorted = true;
// }

// -------------------------------------------------------------------------------------------------
int __fastcall TFormImportadorEgrid::LisObjectChecked(TCheckListBox *CLBox, TList *lisOBJ)
{
	// variáveis locais
	TObject *object;

	lisOBJ->Clear();
	// insere em lisOBJ os objetos associados aos Items checked
	for (int n = 0; n < CLBox->Items->Count; n++)
	{
		if (CLBox->Checked[n])
		{
			if ((object = CLBox->Items->Objects[n]) == NULL)
				continue;
			if (lisOBJ->IndexOf(object) < 0)
				lisOBJ->Add(object);
		}
	}

	return (lisOBJ->Count);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TFormImportadorEgrid::ReiniciaRede(bool aviso)
{
	// var local
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisRedes;

	// proteção
	if (!redes)
		return false;
	lisRedes = redes->LisRede();
	if (lisRedes->Count == 0)
		return true;

	// aviso
	if (aviso)
		if (Confirma("As redes serão apagadas da memória.\n" "Deseja realmente continuar ?",
			"") != ID_YES)
		{
			return false;
		}

	// remove redes da memoria
	if (redes)
		redes->Clear();

	return true;
}

// -------------------------------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18236926
bool __fastcall TFormImportadorEgrid::ValidaOpcoesDeImportacao(void)
{
	// variáveis locais
	AnsiString dirname;
	VTConversor *conversor;

	// verifica destino das redes importadas
	if (RadioGroupDestino->ItemIndex < 0)
	{
		Aviso("Selecione opção de como serão tratadas as redes importadas");
		return (false);
	}
	lisTMP->Clear();
	// inicia lista com Subestações selecionadas
	if (LisObjectChecked(CLBoxSubestacao, lisTMP) == 0)
	{
		Aviso("Selecione as subestações a serem importadas");
		return (false);
	}

	// verifica seleção de Condutores default
	if ((CBoxCondutorMT->ItemIndex < 0) || (CBoxCondutorBT->ItemIndex < 0) ||
		(CBoxCondutorRL->ItemIndex < 0) || (CBoxCondutorIP->ItemIndex < 0))
	{
		Aviso("Selecione opções de condutor default");
		return (false);
	}
	empresa->CondutorDefaultMT =
		(VTCondutor*)CBoxCondutorMT->Items->Objects[CBoxCondutorMT->ItemIndex];
	empresa->CondutorDefaultBT =
		(VTCondutor*)CBoxCondutorBT->Items->Objects[CBoxCondutorBT->ItemIndex];
	empresa->CondutorDefaultRL =
		(VTCondutor*)CBoxCondutorRL->Items->Objects[CBoxCondutorRL->ItemIndex];
	empresa->CondutorDefaultIP =
		(VTCondutor*)CBoxCondutorIP->Items->Objects[CBoxCondutorIP->ItemIndex];
    //arranjo do trecho a montante
    empresa->ArranjoMontante = CBoxArranjoMontante->Checked;

	// verifica se foi definida uma pasta publica válida
	dirname = EditPastaPublica->Text.Trim();
	if ((RadioGroupDestino->ItemIndex > 0) && (!dirname.IsEmpty()))
	{ // verifica se é uma pasta válida
		if (!DirectoryExists(dirname))
		{
			Aviso("Não existe a pasta pública para cópia das redes");
			return (false);
		}
	}
	// define as demais opções selecionadas
	opcao.date = DateTimePicker->Date;
	opcao.sub.topologia = opSUB_GIS;
	opcao.pri.consA4 = RadioGroupConsPrimario->ItemIndex; // == 0;
	opcao.sec.topologia = RadioGroupRedeSec->ItemIndex;
	opcao.valida_fase = CheckBoxValidaFase->Checked; // true;
	opcao.corrige_fase = CheckBoxCorrigeFase->Checked; // true;
	opcao.corrige_perda_fe = CheckBoxCorrigePerdaFerro->Checked; // true;
	opcao.gerarTXT = false;
	// malhas
	opcao.malha.abre_chave_ext = CheckBoxAbrirChaveExt->Checked;
	opcao.malha.abre_chave_int = CheckBoxAbrirChaveInt->Checked;
	opcao.malha.desloca_trecho_ext = CheckBoxDeslocarTrechoExt->Checked;
	opcao.malha.desloca_trecho_int = CheckBoxDeslocarTrechoInt->Checked;
    opcao.malha.somente_rede_pri = true;
	// opções de ihm
	opcao.ihm.log = CheckBoxLog->Checked;
	opcao.ihm.progresso = true;
	//opcoes relatorio eqpto
	opcao.rel_eqpto.gera_relatorio_eqpto = CheckBoxRelEqpto->Checked;
	opcao.rel_eqpto.salva_rede = !CheckBoxSalvarRede->Checked; //não salva, inverso
    // verifica se foi definida uma pasta publica válida
	dirname = EditDirRelEqpto->Text.Trim();
	if (opcao.rel_eqpto.gera_relatorio_eqpto)
	{ // verifica se é uma pasta válida
		if (!DirectoryExists(dirname))
		{
			Aviso("Não existe a pasta " + dirname + " para salvar os relatórios de equipamentos");
			return (false);
		}
		else
		{
			opcao.rel_eqpto.dir_rel_eqpto = dirname;
		}

	}
	// verifica se deve cronometrar tempos
	apl->Remove(cronometro);
	if (CheckBoxCronometro->Checked)
	{
		apl->Add(cronometro);
	}
	return (true);
}

// --------------------eof-----------------------------------------------------
void __fastcall TFormImportadorEgrid::ActionRelValidacaoExecute(TObject *Sender)
{
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	TRelatorioValidacao *rel;

	rel = new TRelatorioValidacao(apl);
	if (rel->Executa(path->DirExporta()))
	{
		Aviso("Relatórios exportados com sucesso para " + path->DirExporta());
	}
	else
	{
		Aviso("FALHA.");
    }

	delete rel;
}
// --------------------------------------------------------------------------------------------- eof




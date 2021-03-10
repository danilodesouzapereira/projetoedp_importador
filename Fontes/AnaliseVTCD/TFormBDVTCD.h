//---------------------------------------------------------------------------

#ifndef TFormBDVTCDH
#define TFormBDVTCDH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTCalculaVTCD;
class VTChave;
class VTBuscaProt;
class TRegiaoVTCD;
class VTParamConf;
class VTGeraLisParamEqpto;
class TFormRegioesVTCD;
//---------------------------------------------------------------------------
class TFormBDVTCD : public TForm
{
__published:	// IDE-managed Components
	TActionList *ActionList1;
	TAction *ActionDefLisRegioes;
	TAction *ActionDefAmostra;
	TAction *ActionGeraCurtos;
	TAction *ActionGravaBD;
	TAction *ActionDefUCSV;
	TGroupBox *GroupBox1;
	TButton *Button1;
	TButton *Button5;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TGroupBox *GroupBox2;
	TSplitter *Splitter1;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TGroupBox *GroupBox3;
	TStringGrid *sgridTaxasImpedancia;
	TGroupBox *GroupBox4;
	TStringGrid *sgridTaxasDefeito;
	TTabSheet *TabSheet2;
	TGroupBox *gbResultados;
	TGroupBox *GroupBox5;
	TStringGrid *sgridTaxaAmostra;
	void __fastcall ActionDefLisRegioesExecute(TObject *Sender);
	void __fastcall ActionDefAmostraExecute(TObject *Sender);
	void __fastcall ActionGeraCurtosExecute(TObject *Sender);
	void __fastcall ActionGravaBDExecute(TObject *Sender);
	void __fastcall ActionDefUCSVExecute(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormBDVTCD(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
	__fastcall ~TFormBDVTCD(void);

private:
    void __fastcall IniciaRegioesVTCD(TList *lisChavesRede, int tipoChave);
	void __fastcall IniciaRegiaoVTCD(VTChave *chave, TRegiaoVTCD *regiaoVTCD);
    void __fastcall IniciaAmostraRegiaoVTCD(TRegiaoVTCD *regiaoVTCD);
    void __fastcall IniciaUCSV(void);
	void __fastcall SgridTaxasDefeitoInicia(void);
    void __fastcall LimpaTStringGrid(TStringGrid *StringGrid);

private:
	VTApl *apl;
	VTBuscaProt *buscaProt;
	VTCalculaVTCD *calcVTCD;
	VTParamConf   *paramConf;
	VTGeraLisParamEqpto  *geraLisParamEqpto;
	TList *lisRegioes, *lisUCSV;
	TFormRegioesVTCD *formRegioesVTCD;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormBDVTCD *FormBDVTCD;
//---------------------------------------------------------------------------
#endif

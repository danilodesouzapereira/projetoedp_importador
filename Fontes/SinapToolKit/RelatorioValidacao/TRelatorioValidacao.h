// ---------------------------------------------------------------------------
#ifndef TRelatorioValidacaoH
#define TRelatorioValidacaoH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <map>
//#include "VTRelatorioValidacao.h"
// ---------------------------------------------------------------------------
class VTApl;
class VTArvore;
class VTCarga;
class VTCapacitor;
class VTChave;
class VTTrafo;
class VTRegulador;
class VTRede;
class VTTrecho;
class VTArranjo;
class TAlimentador;
class TSED;
class VTNode;
class VTEqpto;
class VTBarra;
class VTGerador;
// ---------------------------------------------------------------------------
class TRelatorioValidacao : public TObject //public VTRelatorioValidacao
{
public:
	__fastcall TRelatorioValidacao(VTApl *apl);
	__fastcall ~TRelatorioValidacao(void);


	bool __fastcall Executa(AnsiString diretorio);

private:
	void       __fastcall AgregaEqpto(TAlimentador *alim, TList *lisEXT, int tipo_eqpto);
	void       __fastcall MontaArvore(void);
	bool       __fastcall CriaRelatorio(AnsiString dir, TSED *sub);
	bool       __fastcall CriaRelatorioEqpto(TStrings *strings, TList *lisEqpto,  AnsiString adicional);
	AnsiString __fastcall MontaQuantidadesRede(VTRede *rede);
	AnsiString __fastcall MontaLinhaEqpto(VTEqpto *eqpto);
	AnsiString __fastcall MontaLinhaCarga(VTCarga *carga);
	AnsiString __fastcall MontaLinhaCapacitor(VTCapacitor *capacitor);
	AnsiString __fastcall MontaLinhaChave(VTChave *chave);
	AnsiString __fastcall MontaLinhaTrafo(VTTrafo *trafo);
	AnsiString __fastcall MontaLinhaRegulador(VTRegulador *regulador);
	AnsiString __fastcall MontaLinhaTrecho(VTTrecho *trecho);
	AnsiString __fastcall MontaLinhaGerador(VTGerador *gerador);
//	AnsiString __fastcall MontaLinhaArranjo(VTArranjo *arranjo);
	///
    bool __fastcall ExisteGD(void);
	void __fastcall RemoveGD(void);
	void __fastcall InsereGD(void);
	void __fastcall ListaRedesSec(VTNode *primario, TList *lisREDESEC);
	TSED*	__fastcall InsereSubestacao(VTRede *redeSed);
	void __fastcall AnalisaNodeRede(VTNode *node);
	VTRede* __fastcall RedeSEDmae(VTNode *node);
	TSED* __fastcall ExisteSub(VTRede *redeSub);
	AnsiString __fastcall TipoDeLigacao(int tipo_lig);
	AnsiString __fastcall TipoDeSuprimento(int suprimento);
	AnsiString __fastcall TraduzModeloRegulador(int tipo);
	VTRede* __fastcall ExisteRede(VTBarra *pbarra);

	VTApl *apl;
	VTArvore *arvore_;
	TList *lisSED_;
	std::map<VTRede*, TSED*> mapSub;
	TSED* subestacaoResto;
};

#endif
// ---------------------------------------------------------------------------
// eof

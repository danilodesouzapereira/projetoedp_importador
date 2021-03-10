// ---------------------------------------------------------------------------
#ifndef TSocorroH
#define TSocorroH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\Fontes\Constante\Const.h>
#include <PlataformaSinap\Fontes\Rede\VTFases.h>
#include "..\PreMonta\ModeloRede.h"

// ---------------------------------------------------------------------------
class TSocorro : public TObject
{
public:
	__fastcall TSocorro(void);
	__fastcall ~TSocorro(void);
	

public:
	int chave_id;
	int se1_id;
	int se2_id;
	int circuito1_id;
	int circuito2_id;
	int barra1_id;
	int barra2_id;
	bool telecomando;
	bool ativo;

};

// ---------------------------------------------------------------------------
// função global
// ---------------------------------------------------------------------------
TSocorro* __fastcall NewObjSocorro(void);

#endif
// ---------------------------------------------------------------------------
// eof

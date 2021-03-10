//---------------------------------------------------------------------------

#ifndef TRegiaoVTCDH
#define TRegiaoVTCDH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTChave;
//---------------------------------------------------------------------------
class TRegiaoVTCD : public TObject
	{
	public:
			  __fastcall TRegiaoVTCD(void);
			  __fastcall ~TRegiaoVTCD(void);


	private://métodos
			 void  __fastcall IniciaLisHist(void);

	public:
			 VTChave *chave;
			 TList *lisBlocosRegiao;
             TList *lisEqProt;
			 TList *lisBarrasCurto;
			 TList *lisRegiaoJusante;
			 TList *lisBarrasRegiao;
			 bool  flagReliga;
	};
//---------------------------------------------------------------------------
#endif

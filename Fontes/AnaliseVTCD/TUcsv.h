//---------------------------------------------------------------------------

#ifndef TUcsvH
#define TUcsvH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTCarga;
class TRegiaoVTCD;
class VTBarraVTCD;
//---------------------------------------------------------------------------
class TUcsv : public TObject
	{
	public:
			  __fastcall TUcsv(VTCarga *carga);
			  __fastcall ~TUcsv(void);


	private://métodos

	public:
		 AnsiString  descricao;
		 VTCarga     *carga;
		 TRegiaoVTCD *regiaoVTCD;
		 VTBarraVTCD *barraVTCD;
	};
//---------------------------------------------------------------------------
#endif

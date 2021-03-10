//---------------------------------------------------------------------------
#ifndef TResAlternativaH
#define TResAlternativaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTResAlternativa.h"
//---------------------------------------------------------------------------
class TResAlternativa : public VTResAlternativa
	{
	public:
			  __fastcall TResAlternativa(VTAlternativa* alt, VTObra* obra);
			  __fastcall ~TResAlternativa(void);


	private://m�todos

	protected:  //m�todos acessados via property
		VTAlternativa*   __fastcall PM_GetAlternativa(void);
		VTObra*          __fastcall PM_GetObra(void);
		TList*           __fastcall PM_GetLisBarraVTCD(void);
		VTBarraVTCD*     __fastcall PM_GetBarVTCD(VTBarra *pbarra);

	private://M�todos

	private://dados
      TList         *lisBarVTCD;
		VTAlternativa *alt;
		VTObra        *obra;

	public:

	};
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#ifndef VTResAlternativaH
#define VTResAlternativaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTAlternativa;
class VTObra;
class VTBarra;
class VTBarraVTCD;
//---------------------------------------------------------------------------
class VTResAlternativa : public TObject
	{
	public:  //property
	  __property TList*          LisBarVTCD = {read=PM_GetLisBarraVTCD};
	  __property VTAlternativa*  Alt        = {read=PM_GetAlternativa};
	  __property VTObra*         Obra       = {read=PM_GetObra};
	  __property VTBarraVTCD*    BarVTCD[VTBarra *pbarra]  = {read=PM_GetBarVTCD};

	public:
										__fastcall VTResAlternativa(void) {};
		virtual                 __fastcall ~VTResAlternativa(void) {};

	protected:  //métodos acessados via property
		virtual VTAlternativa*   __fastcall PM_GetAlternativa(void) = 0;
		virtual VTObra*          __fastcall PM_GetObra(void) = 0;
		virtual TList*           __fastcall PM_GetLisBarraVTCD(void) = 0;
		virtual VTBarraVTCD*     __fastcall PM_GetBarVTCD(VTBarra *pbarra) = 0;
	};

//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
VTResAlternativa* NewObjResAlternativa(VTAlternativa* alt, VTObra* obra);

//---------------------------------------------------------------------------
#endif
//eof

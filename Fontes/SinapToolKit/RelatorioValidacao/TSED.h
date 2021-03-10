//---------------------------------------------------------------------------

#ifndef TSEDH
#define TSEDH
//---------------------------------------------------------------------------
class TAlimentador;
// ---------------------------------------------------------------------------
class TSED : public TObject
{
public:
		__fastcall TSED(VTApl *apl, VTRede *sed);
		__fastcall ~TSED(void);
TAlimentador* __fastcall InsereAlimentador(VTRede *redePri);


public:
	VTRede *redeSED;
	TList *lisALIM;
    AnsiString codigo;

private : //dados externos
	VTApl *apl;

};
#endif

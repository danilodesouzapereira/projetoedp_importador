//---------------------------------------------------------------------------

#ifndef TAlimentadorH
#define TAlimentadorH

//---------------------------------------------------------------------------
class VTApl;
class VTRede;
class VTRedes;
class TPosto;
class VTDemanda;
class VTPatamares;
class VTTrafo;

// ---------------------------------------------------------------------------
class TAlimentador : public TObject
{
public:
			__fastcall TAlimentador(VTApl *apl, VTRede *primario);
			__fastcall ~TAlimentador(void);


public:
	VTRede *redeSED;
	VTRede *redePri;
    TList *lisREDESSEC;

private : //dados externos
	VTApl *apl;
	VTDemanda *demanda;
	VTPatamares *patamares;
	VTRedes *redes;
};
#endif

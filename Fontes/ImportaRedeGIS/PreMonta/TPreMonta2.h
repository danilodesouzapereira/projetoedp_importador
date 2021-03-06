// ---------------------------------------------------------------------------
#ifndef TPreMonta2H
#define TPreMonta2H

// arquivos inclu?dos---------------------------------------------------------
#include <Classes.hpp>
#include "TPreMonta.h"

// ---------------------------------------------------------------------------
class TPreMonta2 : public TPreMonta
{
public:
	__fastcall TPreMonta2(VTApl *apl);
	__fastcall ~TPreMonta2(void);
	bool __fastcall InsereSec_Barra(sdi_BARRA_BT &sdi);
	bool __fastcall InsereSec_Capacitor(sdi_CAPACITOR_BT &sdi);
	bool __fastcall InsereSec_Consumidor(sdi_CONS_BT &sdi);
	bool __fastcall InsereSec_ConsIP(sdi_CONS_IP &sdi);
	bool __fastcall InsereSec_Gerador(sdi_GERADOR_BT &sdi);
	bool __fastcall InsereSec_Link(sdi_LINK_BT &sdi);
	bool __fastcall InsereSec_PtoCarga(sdi_PCARGA_BT &sdi);
	bool __fastcall InsereSec_Rede(sdi_INSTAL_TRAFO &sdi);
	bool __fastcall InsereSec_Trecho(sdi_TRECHO_BT &sdi);
};

#endif
// -----------------------------------------------------------------------------
// eof

//---------------------------------------------------------------------------

#pragma hdrstop

#include "EImportaEgrid.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall EImportaEgrid::EImportaEgrid(AnsiString msg) : Exception(msg)
{

}
//---------------------------------------------------------------------------
__fastcall EImportaEgridDAO::EImportaEgridDAO(AnsiString msg) : EImportaEgrid(msg)
{

}
//---------------------------------------------------------------------------


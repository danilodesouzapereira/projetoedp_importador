//---------------------------------------------------------------------------

#ifndef EImportaEgridH
#define EImportaEgridH
// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
// ----------------------------------------------------------------------------------------
class EImportaEgrid : public Sysutils::Exception
{
	public:

		//Construtor e Destrutor
		__fastcall  EImportaEgrid(AnsiString msg);

};
class EImportaEgridDAO : public EImportaEgrid
{
	public:

		//Construtor e Destrutor
		__fastcall  EImportaEgridDAO(AnsiString msg);

};
#endif
// --------------------------------------------------------------------------------------------- eof


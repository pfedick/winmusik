#ifndef WM_EXCEPTIONS_H
#define WM_EXCEPTIONS_H

#include "ppl7.h"

namespace de {
namespace pfp {
namespace winmusik {


PPL7EXCEPTION(InvalidDatabaseFileException,Exception)
PPL7EXCEPTION(DatabaseNotOpenException,Exception)
PPL7EXCEPTION(UnknownDataChunkException,Exception)
PPL7EXCEPTION(DatabaseRecordWasModifiedException,Exception)
PPL7EXCEPTION(CouldNotReadDatabaseRecordException,Exception)

}}}	// EOF Namespace de.pfp.winmusik
#endif // WM_EXCEPTIONS_H

/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2022 Patrick Fedick
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef WM_EXCEPTIONS_H
#define WM_EXCEPTIONS_H

#include <ppl7.h>

namespace de {
namespace pfp {
namespace winmusik {


PPL7EXCEPTION(InvalidDatabaseFileException, Exception);
PPL7EXCEPTION(DatabaseFileNotOpenException, Exception);
PPL7EXCEPTION(InvalidDatabaseChunkException, Exception);
PPL7EXCEPTION(DatabaseFileCorruptException, Exception);
PPL7EXCEPTION(CouldNotReadDatabaseFileException, Exception);
PPL7EXCEPTION(DatabaseModifiedException, Exception);
PPL7EXCEPTION(StorageClassNotInitializedException, Exception);
PPL7EXCEPTION(InvalidStorageClassException, Exception);
PPL7EXCEPTION(UnknownRecordVersionException, Exception);
PPL7EXCEPTION(InvalidRecordException, Exception);
PPL7EXCEPTION(RecordDoesNotExistException, Exception);
PPL7EXCEPTION(InvalidTrackNumberException, Exception);
PPL7EXCEPTION(TrackDoesNotExistException, Exception);
PPL7EXCEPTION(DeviceAlreadyExistsException, Exception);



PPL7EXCEPTION(DatabaseRecordWasModifiedException, Exception)
PPL7EXCEPTION(CouldNotReadDatabaseRecordException, Exception)

}
}
}	// EOF Namespace de.pfp.winmusik
#endif // WM_EXCEPTIONS_H

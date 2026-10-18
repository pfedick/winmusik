/*******************************************************************************
 * This file is part of WinMusik 4 by Patrick Fedick
 * Web: https://github.com/pfedick/winmusik
 *
 * Copyright (c) 2026, Patrick Fedick
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
 *******************************************************************************/

#ifndef WM_EXCEPTIONS_H
#define WM_EXCEPTIONS_H

#include <pplib.h>

namespace de
{
namespace pfp
{
namespace winmusik
{

PPLIBEXCEPTION(InvalidDatabaseFileException, Exception);
PPLIBEXCEPTION(DatabaseFileNotOpenException, Exception);
PPLIBEXCEPTION(InvalidDatabaseChunkException, Exception);
PPLIBEXCEPTION(DatabaseFileCorruptException, Exception);
PPLIBEXCEPTION(CouldNotReadDatabaseFileException, Exception);
PPLIBEXCEPTION(DatabaseModifiedException, Exception);
PPLIBEXCEPTION(StorageClassNotInitializedException, Exception);
PPLIBEXCEPTION(InvalidStorageClassException, Exception);
PPLIBEXCEPTION(UnknownRecordVersionException, Exception);
PPLIBEXCEPTION(InvalidRecordException, Exception);
PPLIBEXCEPTION(RecordDoesNotExistException, Exception);
PPLIBEXCEPTION(InvalidTrackNumberException, Exception);
PPLIBEXCEPTION(TrackDoesNotExistException, Exception);
PPLIBEXCEPTION(DeviceAlreadyExistsException, Exception);

PPLIBEXCEPTION(DatabaseRecordWasModifiedException, Exception)
PPLIBEXCEPTION(CouldNotReadDatabaseRecordException, Exception)

} // namespace winmusik
} // namespace pfp
} // namespace de
#endif // WM_EXCEPTIONS_H

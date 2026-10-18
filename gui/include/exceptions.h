/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2024, Patrick Fedick
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

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_
#include <pplib.h>
#include <QString>
#include <QByteArray>

pplib::String ToString(const QString& fmt, ...);

#define WMEXCEPTION(name, inherit)                                                                                                         \
    class name : public inherit                                                                                                            \
    {                                                                                                                                      \
    public:                                                                                                                                \
        name() noexcept                                                                                                                    \
        {                                                                                                                                  \
        }                                                                                                                                  \
        name(const char* msg, ...) noexcept                                                                                                \
        {                                                                                                                                  \
            va_list args;                                                                                                                  \
            va_start(args, msg);                                                                                                           \
            copyText(msg, args);                                                                                                           \
            va_end(args);                                                                                                                  \
        }                                                                                                                                  \
        name(const pplib::String& msg) noexcept                                                                                            \
        {                                                                                                                                  \
            copyText((const char*)msg);                                                                                                    \
        }                                                                                                                                  \
        virtual const char* what() const noexcept                                                                                          \
        {                                                                                                                                  \
            return (STR_VALUE(name));                                                                                                      \
        }                                                                                                                                  \
        name(const QString& format, ...) noexcept                                                                                          \
        {                                                                                                                                  \
            QByteArray a = format.toUtf8();                                                                                                \
            va_list args;                                                                                                                  \
            va_start(args, format);                                                                                                        \
            copyText((const char*)a, args);                                                                                                \
            va_end(args);                                                                                                                  \
        }                                                                                                                                  \
    };

WMEXCEPTION(InvalidConfigurationFile, pplib::Exception);
WMEXCEPTION(InvalidDatabaseFile, pplib::Exception);
WMEXCEPTION(InvalidDatabaseChunk, pplib::Exception);
WMEXCEPTION(DatabaseFileNotOpen, pplib::Exception);
WMEXCEPTION(DatabaseFileCorrupt, pplib::Exception);
WMEXCEPTION(CouldNotReadDatabaseFile, pplib::Exception);
WMEXCEPTION(DatabaseModified, pplib::Exception);
WMEXCEPTION(StorageClassNotInitialized, pplib::Exception);
WMEXCEPTION(InvalidStorageClass, pplib::Exception);
WMEXCEPTION(UnknownRecordVersion, pplib::Exception);
WMEXCEPTION(InvalidRecord, pplib::Exception);
WMEXCEPTION(RecordDoesNotExist, pplib::Exception);

void ShowException(const pplib::Exception& exp, const QString& msg = QString());
void ShowError(const QString& msg = QString());

#endif // EOF EXCEPTIONS_H_

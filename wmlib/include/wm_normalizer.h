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

#ifndef WM_NORMALIZER_H
#define WM_NORMALIZER_H

#include <stdlib.h>
#include <stdio.h>

#include <map>
#include <ppl7.h>

namespace de
{
namespace pfp
{
namespace winmusik
{

class WMNormalizer
{
private:
    std::map<wchar_t, wchar_t> letterReplacements;
    std::map<wchar_t, wchar_t> filenameLetterReplacements;

    void initLetterReplacements();
    void initFilenameLetterReplacements();
    void addLetterReplacement(wchar_t letter, wchar_t replacement);
    void addLetterReplacement(const pplib::WideString& letters, wchar_t replacement);
    void addFilenameLetterReplacement(const pplib::WideString& letters, wchar_t replacement);
    void NormalizeLetters(const std::map<wchar_t, wchar_t>& letters, pplib::WideString& term) const;

public:
    WMNormalizer();

    void NormalizeTerm(pplib::String& term) const;
    void NormalizeFilename(pplib::String& term) const;

    size_t GetWords(const pplib::String& str, pplib::Array& words) const;
};

const WMNormalizer& GetNormalizer();

} // namespace winmusik
} // namespace pfp
} // namespace de

#endif /* WM_NORMALIZER_H */

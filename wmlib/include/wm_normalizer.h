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

#ifndef WM_NORMALIZER_H
#define WM_NORMALIZER_H

#include <stdlib.h>
#include <stdio.h>

#include <map>
#include <ppl7.h>


namespace de {
namespace pfp {
namespace winmusik {


class WMNormalizer
{
private:
	std::map<wchar_t, wchar_t> letterReplacements;
	std::map<wchar_t, wchar_t> filenameLetterReplacements;

	void initLetterReplacements();
	void initFilenameLetterReplacements();
	void addLetterReplacement(wchar_t letter, wchar_t replacement);
	void addLetterReplacement(const ppl7::WideString& letters, wchar_t replacement);
	void addFilenameLetterReplacement(const ppl7::WideString& letters, wchar_t replacement);
	void NormalizeLetters(const std::map<wchar_t, wchar_t>& letters, ppl7::WideString& term) const;


public:
	WMNormalizer();

	void NormalizeTerm(ppl7::String& term) const;
	void NormalizeFilename(ppl7::String& term) const;

	size_t GetWords(const ppl7::String& str, ppl7::Array& words) const;



};


const WMNormalizer& GetNormalizer();


}
}
}	// EOF Namespace de.pfp.winmusik

#endif /* WM_NORMALIZER_H */

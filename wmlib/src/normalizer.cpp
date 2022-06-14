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

#include "wm_normalizer.h"

namespace de {
namespace pfp {
namespace winmusik {

const WMNormalizer& GetNormalizer()
{
	static WMNormalizer n;
	return n;
}

static void ReplaceIfExists(ppl7::WideString& s, const wchar_t* search, const ppl7::WideString& replace)
{
	if (s.isEmpty()) return;
	const wchar_t* buffer=(wchar_t*)s.getPtr();
	if (!buffer) return;
	if (wcsstr(buffer, search)) {
		ppl7::WideString ss;
		ss.set(search);
		s.replace(ss, replace);
	}
}

WMNormalizer::WMNormalizer()
{
	initLetterReplacements();
	initFilenameLetterReplacements();
}

void WMNormalizer::initLetterReplacements()
{
	addLetterReplacement(ppl7::WideString(L"&+()_,!?/"), L' ');
	addLetterReplacement(ppl7::WideString(L".:''`"), 0);
	addLetterReplacement(ppl7::WideString(L"°"), L'o');
	addLetterReplacement(ppl7::WideString(L"àáâãäåāăąæ"), L'a');
	addLetterReplacement(ppl7::WideString(L"þ"), L'b');
	addLetterReplacement(ppl7::WideString(L"çćĉċč"), L'c');
	addLetterReplacement(ppl7::WideString(L"ďđ"), L'd');
	addLetterReplacement(ppl7::WideString(L"èéêëēĕėęě"), L'e');
	addLetterReplacement(ppl7::WideString(L"ĝğġģ"), L'g');
	addLetterReplacement(ppl7::WideString(L"ĥħ"), L'h');
	addLetterReplacement(ppl7::WideString(L"ìíîïĩīĭįı"), L'i');
	addLetterReplacement(ppl7::WideString(L"ĵ"), L'j');
	addLetterReplacement(ppl7::WideString(L"ķĸ"), L'k');
	addLetterReplacement(ppl7::WideString(L"ĺļľŀł"), L'l');
	addLetterReplacement(ppl7::WideString(L"ñńņňŉŋ"), L'n');
	addLetterReplacement(ppl7::WideString(L"òóôõöøōŏőœ"), L'o');
	addLetterReplacement(ppl7::WideString(L"ŕŗř"), L'r');
	addLetterReplacement(ppl7::WideString(L"śŝşš"), L's');
	addLetterReplacement(ppl7::WideString(L"ţťŧ"), L't');
	addLetterReplacement(ppl7::WideString(L"ùúûüũūŭůűų"), L'u');
	addLetterReplacement(ppl7::WideString(L"ŵẃẁ"), L'w');
	addLetterReplacement(ppl7::WideString(L"ýÿŷ"), L'y');
	addLetterReplacement(ppl7::WideString(L"źżž"), L'z');
	addLetterReplacement(ppl7::WideString(L"–"), L' ');
}

void WMNormalizer::initFilenameLetterReplacements()
{
	addFilenameLetterReplacement(ppl7::WideString(L"&"), L'+');
	addFilenameLetterReplacement(ppl7::WideString(L"\"´`"), L'\'');
	addFilenameLetterReplacement(ppl7::WideString(L"{"), L'(');
	addFilenameLetterReplacement(ppl7::WideString(L"}"), L')');
	addFilenameLetterReplacement(ppl7::WideString(L"|%#;:<>*?\\/"), L' ');
	addFilenameLetterReplacement(ppl7::WideString(L"$"), L'S');
	addFilenameLetterReplacement(ppl7::WideString(L"°"), L'o');
	addFilenameLetterReplacement(ppl7::WideString(L"þ"), L'b');

}

void WMNormalizer::addLetterReplacement(wchar_t letter, wchar_t replacement)
{
	letterReplacements[letter]=replacement;
}

void WMNormalizer::addLetterReplacement(const ppl7::WideString& letters, wchar_t replacement)
{
	for (size_t i=0;i < letters.size();i++) {
		addLetterReplacement(letters[i], replacement);
	}
}

void WMNormalizer::addFilenameLetterReplacement(const ppl7::WideString& letters, wchar_t replacement)
{
	//printf ("Adding Letters: %ls (%i letters)",(const wchar_t*)letters,letters.Len());
	for (size_t i=0;i < letters.size();i++) {
		filenameLetterReplacements[letters[i]]=replacement;
	}
}



void WMNormalizer::NormalizeLetters(const std::map<wchar_t, wchar_t>& letters, ppl7::WideString& term) const
{
	wchar_t* buffer;
	std::map<wchar_t, wchar_t>::const_iterator it;
	size_t ss=term.size();
	size_t target=0;
	buffer=(wchar_t*)term.getPtr();
	wchar_t c;
	for (size_t i=0;i < ss;i++) {
		c=buffer[i];
		if (c < L'A' || c>L'z' || (c > L'Z' && c < L'a')) {
			it=letters.find(c);
			if (it != letters.end()) {
				if (it->second != (wchar_t)0) {
					buffer[target++]=it->second;
				}
			} else {
				buffer[target++]=c;
			}
		} else {
			buffer[target++]=c;
		}
	}
	term.cut(target);
}


void WMNormalizer::NormalizeTerm(ppl7::String& term) const
{
	if (term.isEmpty()) return;
	ppl7::WideString s=term;
	ppl7::WideString search, replace;
	s.lowerCase();
	replace.set(L" ");
	ReplaceIfExists(s, L" versus ", replace);
	ReplaceIfExists(s, L" pres. ", replace);
	ReplaceIfExists(s, L" presents ", replace);
	ReplaceIfExists(s, L" vs. ", replace);
	ReplaceIfExists(s, L" vs ", replace);
	ReplaceIfExists(s, L" ft. ", replace);
	ReplaceIfExists(s, L" ft ", replace);
	ReplaceIfExists(s, L" feat. ", replace);
	ReplaceIfExists(s, L" featuring ", replace);
	ReplaceIfExists(s, L" und ", replace);
	ReplaceIfExists(s, L" and ", replace);
	ReplaceIfExists(s, L" - ", replace);
	ReplaceIfExists(s, L" x ", replace);
	ReplaceIfExists(s, L" with ", replace);
	ReplaceIfExists(s, L" /\\ ", replace);
	ReplaceIfExists(s, L"DJ ", replace);
	ReplaceIfExists(s, L" ", replace);		// U+00A0, c2 a0, NO-BREAK SPACE
	s.trim();
	s.replace(L"  ", L" ");
	NormalizeLetters(letterReplacements, s);
	term=s;
}

void WMNormalizer::NormalizeFilename(ppl7::String& term) const
{
	if (term.isEmpty()) return;
	ppl7::WideString s=term;
	s.replace(L"ß", L"ss");
	s.trim();
	NormalizeLetters(filenameLetterReplacements, s);
	term=s;
}

size_t WMNormalizer::GetWords(const ppl7::String& str, ppl7::Array& words) const
{
	words.clear();
	ppl7::String s=str;
	s.trim();
	if (s.isEmpty()) return 0;
	// Bestimmte Zeichen filtern wir raus
	NormalizeTerm(s);
	words.explode(s, " ", 0, true);
	return words.size();
}




}
}
}	// EOF Namespace de.pfp.winmusik

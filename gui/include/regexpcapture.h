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


#ifndef REGEXPCAPTURE_H_
#define REGEXPCAPTURE_H_

#include <wm_regexpcapture.h>


void copyFromClipboard(de::pfp::winmusik::RegExpClipboard& clip);



class RegularExpressionCapture
{
private:
	std::vector<de::pfp::winmusik::RegExpPattern> patterns;
	void copyToMatch(const de::pfp::winmusik::RegExpPattern& p, const ppl7::Array& res, de::pfp::winmusik::RegExpMatch& match) const;
	void addDefaultPatterns();

public:
	RegularExpressionCapture();
	~RegularExpressionCapture();

	void load();
	void save();
	void addPattern(const de::pfp::winmusik::RegExpPattern& pattern);
	void insertPattern(size_t pos, const de::pfp::winmusik::RegExpPattern& pattern);
	void setPattern(size_t pos, const de::pfp::winmusik::RegExpPattern& pattern);
	void deletePattern(size_t pos);
	const de::pfp::winmusik::RegExpPattern& getPattern(size_t pos) const;
	size_t size() const;

	bool match(const ppl7::String& data, de::pfp::winmusik::RegExpMatch& match) const;
	bool match(const de::pfp::winmusik::RegExpClipboard& data, de::pfp::winmusik::RegExpMatch& match) const;
	bool testMatch(const ppl7::String& data, de::pfp::winmusik::RegExpMatch& match, const de::pfp::winmusik::RegExpPattern& pattern) const;




};


#endif /* REGEXPCAPTURE_H_ */

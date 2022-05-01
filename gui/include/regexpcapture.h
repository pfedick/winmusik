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


#include <vector>
#include <list>
class RegExpPattern
{
public:
	RegExpPattern();

	void copyFrom(const RegExpPattern& other);

	ppl7::String Name;
	ppl7::String Pattern;
	int artist;
	int title;
	int version;
	int genre;
	int label;
	int bpm;
	int album;
	int hours;
	int minutes;
	int seconds;
	int releasedate;
	bool isHTML;
};

class RegExpMatch
{
public:
	RegExpMatch() {
		Length=0;
	}
	ppl7::String	Artist;
	ppl7::String	Title;
	ppl7::String	Version;
	ppl7::String	Genre;
	ppl7::String	Label;
	ppl7::String	Bpm;
	ppl7::String	Album;
	ppl7::String	ReleaseDate;
	int				Length;		// in Sekunden

};

class RegExpClipboard
{
public:
	void copyFromClipboard();
	ppl7::String PlainText;
	ppl7::String Html;
};


class RegularExpressionCapture
{
private:
	std::vector<RegExpPattern> patterns;
#ifdef HAVE_PYTHON
	std::list<PythonModule> python_modules;
#endif
	void copyToMatch(const RegExpPattern& p, const ppl7::Array& res, RegExpMatch& match) const;
	void addDefaultPatterns();
	void loadScripts();

public:
	RegularExpressionCapture();
	~RegularExpressionCapture();

	void load();
	void save();
	void addPattern(const RegExpPattern& pattern);
	void insertPattern(size_t pos, const RegExpPattern& pattern);
	void setPattern(size_t pos, const RegExpPattern& pattern);
	void deletePattern(size_t pos);
	const RegExpPattern& getPattern(size_t pos) const;
	size_t size() const;

	bool match(const ppl7::String& data, RegExpMatch& match) const;
	bool match(const RegExpClipboard& data, RegExpMatch& match) const;
	bool testMatch(const ppl7::String& data, RegExpMatch& match, const RegExpPattern& pattern) const;
	bool buildinMatch(const ppl7::String& data, RegExpMatch& match) const;




};


#endif /* REGEXPCAPTURE_H_ */

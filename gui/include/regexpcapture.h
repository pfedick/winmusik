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
#ifdef HAVE_PYTHON
#include <Python.h>
#endif
class RegExpPattern
{
	public:
		RegExpPattern();

		void copyFrom(const RegExpPattern &other);

		ppl6::CString Name;
		ppl6::CString Pattern;
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
		ppl6::CString	Artist;
		ppl6::CString	Title;
		ppl6::CString	Version;
		ppl6::CString	Genre;
		ppl6::CString	Label;
		ppl6::CString	Bpm;
		ppl6::CString	Album;
		ppl6::CString	ReleaseDate;
		int				Length;		// in Sekunden

};

class RegExpClipboard
{
	public:
		void copyFromClipboard();
		ppl6::CString PlainText;
		ppl6::CString Html;
};

#ifdef HAVE_PYTHON
class PythonModule
{
	public:
		PythonModule(const ppl6::CString &name, PyObject *pModule) {
			this->name=name;
			this->pModule=pModule;
		}
		ppl6::CString	name;
		PyObject 		*pModule;
};

#endif

class RegularExpressionCapture
{
	private:
		std::vector<RegExpPattern> patterns;
#ifdef HAVE_PYTHON
		std::list<PythonModule> python_modules;
#endif
		void copyToMatch(const RegExpPattern &p, const ppl6::CArray &res, RegExpMatch &match) const;
		void addDefaultPatterns();
		void loadScripts();
#ifdef HAVE_PYTHON
		bool matchAgainstScripts(const RegExpClipboard &data, RegExpMatch &match) const;
		bool matchScript(const PythonModule &module, const RegExpClipboard &data, RegExpMatch &match) const;
#endif

	public:
		RegularExpressionCapture();
		~RegularExpressionCapture();

		void load();
		void save();
		void addPattern(const RegExpPattern &pattern);
		void insertPattern(size_t pos, const RegExpPattern &pattern);
		void setPattern(size_t pos, const RegExpPattern &pattern);
		void deletePattern(size_t pos);
		const RegExpPattern &getPattern(size_t pos) const;
		size_t size() const;

		bool match(const ppl6::CString &data, RegExpMatch &match) const;
		bool match(const RegExpClipboard &data, RegExpMatch &match) const;
		bool testMatch(const ppl6::CString &data, RegExpMatch &match, const RegExpPattern &pattern) const;
		bool buildinMatch(const ppl6::CString &data, RegExpMatch &match) const;




};


#endif /* REGEXPCAPTURE_H_ */

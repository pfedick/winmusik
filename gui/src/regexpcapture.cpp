/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.5 $
 * $Date: 2010/10/10 10:31:01 $
 * $Id: properties.cpp,v 1.5 2010/10/10 10:31:01 pafe Exp $
 *
 *
 * Copyright (c) 2010 Patrick Fedick
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

#include "winmusik3.h"
#include "regexpcapture.h"
#include <QSettings>
#include <QClipboard>
#include <exception>
#include <stdexcept>
#ifdef HAVE_PYTHON
#include <Python.h>
#endif

#include "ppl7.h"
#include "ppl7-types.h"
#include "ppl7-ppl6compat.h"

void RegExpClipboard::copyFromClipboard()
{
	QString subtype="html";
	Html = QApplication::clipboard()->text(subtype);
	PlainText = QApplication::clipboard()->text();
}

RegExpPattern::RegExpPattern()
{
	artist=0;
	title=0;
	version=0;
	genre=0;
	label=0;
	bpm=0;
	album=0;
	hours=0;
	minutes=0;
	seconds=0;
	releasedate=0;
	isHTML=false;
}

void RegExpPattern::copyFrom(const RegExpPattern &other)
{
	Name=other.Name;
	Pattern=other.Pattern;
	artist=other.artist;
	title=other.title;
	version=other.version;
	genre=other.genre;
	label=other.label;
	bpm=other.bpm;
	album=other.album;
	hours=other.hours;
	minutes=other.minutes;
	seconds=other.seconds;
	releasedate=other.releasedate;
	isHTML=other.isHTML;
}


RegularExpressionCapture::RegularExpressionCapture()
{

}

RegularExpressionCapture::~RegularExpressionCapture()
{

}

void RegularExpressionCapture::load()
{
	//loadScripts();
	patterns.clear();
	if (!wm_main) return;
	ppl6::CString File=wm_main->conf.DataPath+"/regexp.conf";
	QSettings settings(File,QSettings::IniFormat);
	size_t pos=0;
	ppl6::CString Group;
	while (1) {
		Group.Setf("%i",(int)pos);
		settings.beginGroup(Group);
		RegExpPattern p;
		p.Name=settings.value("Name").toString();
		if (p.Name.IsEmpty()) break;
		p.Pattern=settings.value("Pattern").toString();
		p.artist=settings.value("artist").toInt();
		p.title=settings.value("title").toInt();
		p.version=settings.value("version").toInt();
		p.genre=settings.value("genre").toInt();
		p.label=settings.value("label").toInt();
		p.bpm=settings.value("bpm").toInt();
		p.album=settings.value("album").toInt();
		p.releasedate=settings.value("releasedate").toInt();
		p.hours=settings.value("hours").toInt();
		p.minutes=settings.value("minutes").toInt();
		p.seconds=settings.value("seconds").toInt();
		p.isHTML=settings.value("isHTML", false).toBool();
		patterns.push_back(p);

		settings.endGroup();
		pos++;
	}
	if (patterns.empty()) addDefaultPatterns();

}

void RegularExpressionCapture::addDefaultPatterns()
{
	RegExpPattern p;
	//Tonny Nesse - Memories (Fady & Mina Remix, 7:56 min, Trance) [MP3 572 A-5]
	p.Name="WinMusik Editor";
	p.Pattern="^(.*?)\\s-\\s(.*)\\s\\((.*),\\s([0-9]+):([0-9]+)\\smin,\\s(.*?)\\)\\s\\[.*?\\]$";
	p.artist=1;
	p.title=2;
	p.version=3;
	p.minutes=4;
	p.seconds=5;
	p.genre=6;

	patterns.push_back(p);
}

void RegularExpressionCapture::save()
{
	if (!wm_main) return;
	ppl6::CString File=wm_main->conf.DataPath+"/regexp.conf";
	QSettings settings(File,QSettings::IniFormat);
	std::vector<RegExpPattern>::const_iterator it;

	size_t pos=0;
	ppl6::CString Group;
	for (it = patterns.begin() ; it != patterns.end(); ++it) {
		Group.Setf("%i",(int)pos);
		settings.beginGroup(Group);
		settings.setValue("Name",(*it).Name);
		settings.setValue("Pattern",(*it).Pattern);
		settings.setValue("artist",(*it).artist);
		settings.setValue("title",(*it).title);
		settings.setValue("version",(*it).version);
		settings.setValue("genre",(*it).genre);
		settings.setValue("label",(*it).label);
		settings.setValue("bpm",(*it).bpm);
		settings.setValue("album",(*it).album);
		settings.setValue("releasedate",(*it).releasedate);
		settings.setValue("hours",(*it).hours);
		settings.setValue("minutes",(*it).minutes);
		settings.setValue("seconds",(*it).seconds);
		settings.setValue("isHTML",(*it).isHTML);
		settings.endGroup();

		pos++;
	}
}

void RegularExpressionCapture::addPattern(const RegExpPattern &pattern)
{
	patterns.push_back(pattern);
}

void RegularExpressionCapture::insertPattern(size_t pos, const RegExpPattern &pattern)
{
	if (pos<patterns.size()) {
		std::vector<RegExpPattern>::iterator it;
		it = patterns.begin();
		try {
			it+=pos;
			patterns.insert(it,pattern);
		} catch ( const std::out_of_range & oor) {
			patterns.push_back(pattern);
		}
	} else {
		patterns.push_back(pattern);
	}
}

void RegularExpressionCapture::deletePattern(size_t pos)
{
	if (pos<patterns.size()) {
		std::vector<RegExpPattern>::iterator it;
		it = patterns.begin();
		try {
			it+=pos;
			patterns.erase(it);
		} catch ( const std::out_of_range & oor) {

		}
	}
}

void RegularExpressionCapture::setPattern(size_t pos, const RegExpPattern &pattern)
{
	if (pos<patterns.size()) {
		RegExpPattern &p=patterns.at(pos);
		p.copyFrom(pattern);
	}
}


const RegExpPattern &RegularExpressionCapture::getPattern(size_t pos) const
{
	return patterns.at(pos);
}

size_t RegularExpressionCapture::size() const
{
	return patterns.size();
}

void RegularExpressionCapture::copyToMatch(const RegExpPattern &p, const ppl6::CArray &res, RegExpMatch &match) const
{
	if (p.artist) match.Artist=ppl6::Trim(res[p.artist]);
	if (p.title) match.Title=ppl6::Trim(res[p.title]);
	if (p.version) match.Version=ppl6::Trim(res[p.version]);
	if (p.genre) match.Genre=ppl6::Trim(res[p.genre]);
	if (p.label) match.Label=ppl6::Trim(res[p.label]);
	if (p.bpm) match.Bpm=ppl6::Trim(res[p.bpm]);
	if (p.album) match.Album=ppl6::Trim(res[p.album]);
	if (p.hours) match.Length+=res.GetString(p.hours).ToInt()*60*60;
	if (p.minutes) match.Length+=res.GetString(p.minutes).ToInt()*60;
	if (p.seconds) match.Length+=res.GetString(p.seconds).ToInt();
	if (p.releasedate) match.ReleaseDate=ppl6::Trim(res[p.releasedate]);
}


static bool matchBeatPortPro100_getArtist(const ppl6::CString &html, RegExpMatch &match)
{
	ppl6::CArray Lines, List, matches;
	if (!html.PregMatch("/<p class=\"buk-track-artists\".*?>(.*?)<\\/p>/is",matches)) return false;
	Lines.Explode(matches.GetString(1),"\n");
	List.Clear();
	for (size_t i=0;i<Lines.Size();i++) {
		ppl6::CString Line=Lines.GetString(i);
		if (Line.PregMatch("/<a href=\".*?\" data-artist=\"[0-9]+\".*?>(.*?)<\\/a>/is",matches)) {
			List.Add(matches.GetString(1));
		}
	}
	if (List.Size()==0) return false;
	match.Artist=ppl6::UnescapeHTMLTags(List.Implode(", "));
	return true;
}

static bool matchBeatPortPro100_getLabels(const ppl6::CString &html, RegExpMatch &match)
{
	ppl6::CArray Lines, List, matches;
	if (!html.PregMatch("/<p class=\"buk-track-labels\".*?>(.*?)<\\/p>/is",matches)) return false;
	Lines.Explode(matches.GetString(1),"\n");
	List.Clear();
	for (size_t i=0;i<Lines.Size();i++) {
		ppl6::CString Line=Lines.GetString(i);
		if (Line.PregMatch("/<a href=\".*?\" data-label=\"[0-9]+\".*?>(.*?)<\\/a>/is",matches)) {
			List.Add(matches.GetString(1));
		}
	}
	if (List.Size()==0) return false;
	match.Label=ppl6::UnescapeHTMLTags(List.Implode(", "));
	return true;
}

static bool matchBeatPortPro100_getGenres(const ppl6::CString &html, RegExpMatch &match)
{
	ppl6::CArray Lines, List, matches;
	if (!html.PregMatch("/<p class=\"buk-track-genre\".*?>(.*?)<\\/p>/is",matches)) return false;
	Lines.Explode(matches.GetString(1),"\n");
	List.Clear();
	for (size_t i=0;i<Lines.Size();i++) {
		ppl6::CString Line=Lines.GetString(i);
		if (Line.PregMatch("/<a href=\".*?\" data-genre=\"[0-9]+\".*?>(.*?)<\\/a>/is",matches)) {
			List.Add(matches.GetString(1));
		}
	}
	if (List.Size()==0) return false;
	match.Genre=ppl6::UnescapeHTMLTags(List.Implode(", "));
	return true;
}


static void fixArtistAndTitle(const ppl6::CString &prefix, RegExpMatch &match, const ppl6::CString prefix_replace)
{
	ppl6::CString regex1="/(.*?)\\s"+prefix+"\\s+(.*)$/i";
	ppl6::CString regex2="/(.*?)\\("+prefix+"\\s+(.*)\\)$/i";
	ppl6::CArray Matches;
	if (match.Title.PregMatch(regex1, Matches)>0 || match.Title.PregMatch(regex2, Matches)>0) {
		ppl6::CString feat=Matches.GetString(2);
		feat.Trim();
		match.Title=Matches[1];
		match.Artist.Replace(feat,"");
		match.Artist.Trim();
		match.Artist.Trim(",");
		match.Artist.Replace(",,",",");
		match.Artist+=" "+prefix_replace+" "+Matches.GetString(2);
	}
}

static void fixIt(RegExpMatch &match)
{
	//ppl6::CArray Matches;
	fixArtistAndTitle("feat\\.",match, "feat.");
	fixArtistAndTitle("featuring",match, "feat.");
	fixArtistAndTitle("ft",match, "feat.");
	fixArtistAndTitle("ft.",match, "feat.");
	fixArtistAndTitle("pres\\.",match, "pres.");
	fixArtistAndTitle("presents",match, "pres.");
}

static void fixHTML(ppl6::CString &text)
{
	text=ppl6::UnescapeHTMLTags(text);
}

static void fixHTML(RegExpMatch &match)
{
	fixHTML(match.Artist);
	fixHTML(match.Title);
	fixHTML(match.Version);
	fixHTML(match.Genre);
	fixHTML(match.Label);
	fixHTML(match.Bpm);
	fixHTML(match.Album);
	fixHTML(match.ReleaseDate);
}

static bool matchBeatPortPro100(const ppl6::CString &html, RegExpMatch &match)
{
	ppl6::CArray matches;
	printf ("Try match: %s\n\n",(const char*)html);
	//<span class="buk-track-primary-title">Access</span>
	if (!html.PregMatch("/<span class=\"buk-track-primary-title\".*?>(.*?)<\\/span>/is",matches)) return false;
	printf ("ok\n");
	match.Title=ppl6::UnescapeHTMLTags(matches.GetString(1));
	if (!html.PregMatch("/<span class=\"buk-track-remixed\".*?>(.*?)<\\/span>/is",matches)) return false;
	match.Version=ppl6::UnescapeHTMLTags(matches.GetString(1));
	if (html.PregMatch("/<p class=\"buk-track-released\".*?>([0-9\\-]+)<\\/p>/is",matches))
		match.ReleaseDate=ppl6::UnescapeHTMLTags(matches.GetString(1));
	if (!matchBeatPortPro100_getArtist(html,match)) return false;
	matchBeatPortPro100_getLabels(html,match);
	matchBeatPortPro100_getGenres(html,match);
	fixIt(match);
	return true;
}

static bool matchBeatPortProReleases(const ppl6::CString &html6, RegExpMatch &match)
{
	ppl7::String html=ppl7::to7(html6);
	ppl7::Array matches;
	ppl7::String Artists,Title,Version,Genre,Released,Label;
	int found=0;
	if (html.pregMatch("/<p class=\"buk-track-artists\".*?>(.*?)<\\/p>/i",matches)) {
		ppl7::String todo=matches[1];
		while (todo.pregMatch("/^(.*?)<a href=.*?>(.*?)<\\/a>(.*?)$/i",matches)) {
			todo=matches[1]+matches[3];
			Artists=Artists+", "+matches[2];
		}
		Artists.trim(" ,");
		found++;
	}
	if (html.pregMatch("/<p class=\"buk-track-title\".*?>(.*?)<\\/p>/i",matches)) {
		ppl7::String todo=matches[1];
		if (todo.pregMatch("/<span class=\"buk-track-primary-title\".*?>(.*?)<\\/span>/i",matches)) {
			Title=matches[1];
		}
		if (todo.pregMatch("/<span class=\"buk-track-remixed\".*?>(.*?)<\\/span>/i",matches)) {
			Version=matches[1];
		}
		found++;

	}
	if (html.pregMatch("/<p class=\"buk-track-genre\".*?>(.*?)<\\/p>/i",matches)) {
		ppl7::String todo=matches[1];
		//printf ("Match1\n");
		//todo.printnl();
		if (todo.pregMatch("/<a href=.*?>(.*?)<\\/a>/i",matches)) {
			Genre=matches[1];
		}
		found++;

	}
	if (html.pregMatch("/<p class=\"buk-track-released\".*?>(.*?)<\\/p>/i",matches)) {
		ppl7::String todo=matches[1];
		Released=matches[1];
		found++;
	}
	if (html.pregMatch("/<p class=\"buk-track-labels\".*?>(.*?)<\\/p>/i",matches)) {
		ppl7::String todo=matches[1];
		if (todo.pregMatch("/<a href=.*?>(.*?)<\\/a>/i",matches)) {
			Label=matches[1];
		}
		found++;

	}
	match.Artist=ppl7::to6(Artists);
	match.Title=ppl7::to6(Title);
	match.Genre=ppl7::to6(Genre);
	match.Label=ppl7::to6(Label);
	match.ReleaseDate=ppl7::to6(Released);
	match.Version=ppl7::to6(Version);
	fixHTML(match);
	fixIt(match);
	//printf ("matches: %d\n",found);
	if (found>1) return true;
	return false;
}


bool RegularExpressionCapture::match(const RegExpClipboard &data, RegExpMatch &match) const
{
	//if (matchAgainstScripts(data,match)) return true;
	//if (this->match(data.Html,match)) return true;
	//if (matchBeatPortPro100(data.Html,match)) return true;
	if (matchBeatPortProReleases(data.Html,match)) return true;

	if (patterns.empty()) return false;
	std::vector<RegExpPattern>::const_iterator it;
	for (it = patterns.begin() ; it != patterns.end(); ++it) {
		if ((*it).isHTML) {
			if(testMatch(data.Html,match,*it)) {
				fixHTML(match);
				fixIt(match);
				return true;
			}
		} else {
			if(testMatch(data.PlainText,match,*it)) {
				fixIt(match);
				return true;
			}
		}
	}
	if (buildinMatch(data.PlainText,match)) return true;
	return false;
	//return this->match(data.PlainText,match);
}

bool RegularExpressionCapture::match(const ppl6::CString &data, RegExpMatch &match) const
{
	if (matchBeatPortPro100(data,match)) return true;

	if (patterns.empty()) return false;
	std::vector<RegExpPattern>::const_iterator it;
	for (it = patterns.begin() ; it != patterns.end(); ++it) {
		if(testMatch(data,match,*it)) {
			fixIt(match);
			return true;
		}
	}
	if (buildinMatch(data,match)) return true;
	return false;
}

bool RegularExpressionCapture::buildinMatch(const ppl6::CString &data, RegExpMatch &match) const
{
	ppl6::CArray Matches;
	if (data.PregMatch("/^(.*?)\\s+\\-\\s+(.*?)\\t(.*?)\\t.*$/",Matches)) {
		//printf ("Match ASOT\n");
		match.Artist=ppl6::Trim(Matches[1]);
		match.Title=ppl6::Trim(Matches[2]);
		match.Version=ppl6::Trim(Matches[3]);
	} else if (data.PregMatch("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)\\t/",Matches)) {
		match.Artist=ppl6::Trim(Matches[3]);
		match.Title=ppl6::Trim(Matches[1]);
		match.Version=ppl6::Trim(Matches[2]);
	} else if (data.PregMatch("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)$/",Matches)) {
		match.Artist=ppl6::Trim(Matches[3]);
		match.Title=ppl6::Trim(Matches[1]);
		match.Version=ppl6::Trim(Matches[2]);
	} else if (data.PregMatch("/^(.*?)\\-(.*?)\\((.*?)\\).*$/",Matches)) {
		match.Artist=ppl6::Trim(Matches[1]);
		match.Title=ppl6::Trim(Matches[2]);
		match.Version=ppl6::Trim(Matches[3]);
	} else if (data.PregMatch("/^(.*?)\\-(.*?)$/",Matches)) {
		match.Artist=ppl6::Trim(Matches[1]);
		match.Title=ppl6::Trim(Matches[2]);
	} else {
		return false;
	}
	if (match.Title.PregMatch("/(.*?)(\\sfeat\\..*)$/i", Matches)) {
		match.Title=Matches[1];
		match.Artist+=Matches[2];
	} else if (match.Title.PregMatch("/(.*?)(\\sfeaturing.*)$/i", Matches)) {
		match.Title=Matches[1];
		match.Artist+=Matches[2];
	} else if (match.Title.PregMatch("/(.*?)(\\spres\\..*)$/i", Matches)) {
		match.Title=Matches[1];
		match.Artist+=Matches[2];
	} else if (match.Title.PregMatch("/(.*?)(\\spresents.*)$/i", Matches)) {
		match.Title=Matches[1];
		match.Artist+=Matches[2];
	}
	fixIt(match);
	return true;
}

bool RegularExpressionCapture::testMatch(const ppl6::CString &data, RegExpMatch &match, const RegExpPattern &pattern) const
{
	ppl6::CString Pattern;
	ppl6::CArray res;
	Pattern="/"+(pattern).Pattern+"/ism";
	if (data.PregMatch(Pattern,res)) {
		copyToMatch(pattern,res,match);
		return true;
	}
	return false;
}


void RegularExpressionCapture::loadScripts()
{
#ifdef HAVE_PYTHON
	ppl6::CString Path=wm_main->conf.DataPath+"/scripts";
	ppl6::CDir Dir;
	printf ("RegularExpressionCapture::loadScripts\n");

	if (!Dir.Open(Path)) return;

	PyObject *sysPath = PySys_GetObject("path");
	PyObject *path = PyString_FromString(Path);
	int result = PyList_Insert(sysPath, 0, path);
	Py_DECREF(path);


	Dir.Reset();
	const ppl6::CDirEntry *entry;
	while ((entry=Dir.GetNextPattern("*.py"))) {
		ppl6::CArray matches;
		if (entry->Filename.PregMatch("/^(.*?)\\.py$/i",matches)) {
			ppl6::CString ModuleName=matches.GetString(1);
			printf ("Loading Module: %s\n",(const char*)ModuleName);
			PyObject *pName = PyString_FromString(ModuleName);
			PyObject *pModule = PyImport_Import(pName);
		    Py_DECREF(pName);
		    if (pModule) {
		    	printf ("Module loaded\n");
		    	PyObject* pFunc = PyObject_GetAttrString(pModule, "winmusik_init");
		    	if (pFunc) {
		    		printf ("Rufe Init auf\n");
		    		PyObject *pReturn = PyObject_CallObject(pFunc,NULL);
		    		if (pReturn) Py_DECREF(pReturn);
		    		Py_XDECREF(pFunc);
		    	}
				python_modules.push_back(PythonModule(ModuleName,pModule));
		    }
			//PyRun_SimpleString("import "+ModuleName+"\n");
		}
	}
#endif
}

#ifdef HAVE_PYTHON
bool RegularExpressionCapture::matchScript(const PythonModule &module, const RegExpClipboard &data, RegExpMatch &match) const
{
	PyObject* pFunc = PyObject_GetAttrString(module.pModule, "winmusik_parse_clipboard");
	if (pFunc && PyCallable_Check(pFunc)) {
		PyObject *pArgs = PyTuple_New(2);
		PyObject * pValueText = PyString_FromString(data.PlainText);
		PyObject * pValueHtml = PyString_FromString(data.Html);
		PyTuple_SetItem(pArgs, 0, pValueText);
		PyTuple_SetItem(pArgs, 1, pValueHtml);
		PyObject *pReturn = PyObject_CallObject(pFunc, pArgs);
		Py_DECREF(pArgs);
		Py_DECREF(pValueText);
		Py_DECREF(pValueHtml);

		if (pReturn!=NULL && PyObject_IsTrue(pReturn)==1) {
			printf("Result of call: %s\n", PyString_AsString(pReturn));
			Py_DECREF(pReturn);
		} else {
			printf ("Return not ok\n");
		}
		//Py_XDECREF(pFunc);
	}
	Py_XDECREF(pFunc);
	//Py_DECREF(pModule);
	return false;
}

bool RegularExpressionCapture::matchAgainstScripts(const RegExpClipboard &data, RegExpMatch &match) const
{
	std::list<PythonModule>::const_iterator it;
	for (it=python_modules.begin(); it != python_modules.end(); ++it) {
		printf ("Matching against module: %s\n",(const char*)(*it).name);
		if (matchScript((*it),data,match)) return true;
	}
	return false;
}
#endif


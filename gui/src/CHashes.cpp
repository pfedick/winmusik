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


#include "winmusik3.h"
#include "src/asynchronousMessage/asynchronousMessage.h"
#include "resultfilter.h"


CHashes::CHashes()
{
	wm=NULL;
	log=NULL;
}

CHashes::~CHashes()
{
}

void CHashes::Clear()
{
	Mutex.lock();
	Artist.clear();
	Title.clear();
	Version.clear();
	Genre.clear();
	Tags.clear();
	Remarks.clear();
	Album.clear();
	//Global.Clear();
	Mutex.unlock();
}

int CHashes::GetTags(const ppl7::String& str, ppl7::Array& words)
{
	words.clear();
	ppl7::String s=str;
	s.trim();
	if (s.isEmpty()) return 0;
	// Wir nehmen nur Kleinbuchstaben
	s.lowerCase();
	s.replace(",", ";");
	words.explode(s, ";", 0, true);
	return 1;
}

void CHashes::AddWords(WordTree& Tree, const ppl7::Array& words, const DataTitle* title)
{
	for (size_t i=0;i < words.size();i++) {
		const ppl7::String& key=words[i];
		Tree[key].insert(title->TitleId);
	}
}

void CHashes::Copy(TitleTree& Result, const TitleTree& src)
{
	Result.clear();
	TitleTree::const_iterator it;
	for (it=src.begin();it != src.end();it++) {
		Result.insert(*it);
	}
}

uint32_t CHashes::Add(TitleTree& Result, const TitleTree& src, const ResultFilter& filter)
{
	uint32_t counter=0;
	TitleTree::const_iterator it;
	for (it=src.begin();it != src.end();it++) {
		if (filter.pass(*it)) {
			Result.insert(*it);
			counter++;
		}
	}
	return counter;
}

uint32_t CHashes::Add(TitleTree& Result, const TitleTree& src)
{
	uint32_t counter=0;
	TitleTree::const_iterator it;
	for (it=src.begin();it != src.end();it++) {
		Result.insert(*it);
		counter++;
	}
	return counter;
}

void CHashes::Union(TitleTree& Result, const TitleTree& Tree1, const TitleTree& Tree2)
{
	Result.clear();
	// Nur was in Tree1 und Tree2 vorhanden ist, wandert in Result
	const TitleTree* small, * big;
	if (Tree1.size() < Tree2.size()) {
		small=&Tree1;
		big=&Tree2;
	} else {
		small=&Tree2;
		big=&Tree1;
	}
	TitleTree::const_iterator it;
	for (it=small->begin();it != small->end();it++) {
		if (big->find(*it) != big->end()) Result.insert(*it);
	}
}

void CHashes::FindWords(const WordTree& Tree, const ppl7::Array& words, TitleTree& Result)
{
	int wordnum=0;
	for (size_t i=0;i < words.size();i++) {
		const ppl7::String& key=words[i];
		//printf ("Word: %s",(char*)key);
		if (key.instr("*") >= 0) {
			ppl7::String key2=key;
			key2.replace("*", ".*");
			ppl7::String expression;
			expression="/^" + key2 + "$/i";

			if (log) log->printf(ppl7::Logger::DEBUG, 10, "CHashes", "FindWords", __FILE__, __LINE__, "Wildcard-Search: %s", (const char*)expression);
			// Wildcard Suche
			bool found=false;
			WordTree::const_iterator it;
			for (it=Tree.begin();it != Tree.end();it++) {
				if (ppl7::RegEx::match(expression, it->first)) {
					found=true;
					if (!wordnum) Copy(Result, it->second);
					else Add(Result, it->second);
					wordnum++;
				}
			}
			if (!found) {
				Result.clear();
				return;
			}

		} else {
			WordTree::const_iterator it=Tree.find(key);
			if (it != Tree.end()) {
				if (!wordnum) Copy(Result, it->second);
				else {
					TitleTree res2;
					Union(res2, Result, it->second);
					Copy(Result, res2);
				}
			} else {
				Result.clear();
				return;
			}
			wordnum++;
		}
	}
}


void CHashes::FindSingleWord(const WordTree& Tree, const ppl7::String& Word, TitleTree& Result, const ResultFilter& filter)
{
	if (Word.instr("*") >= 0) {
		ppl7::String Tmp=Word;
		Tmp.replace("*", ".*");
		ppl7::String expression;
		expression="/^" + Tmp + "$/i";
		// Wildcard Suche
		bool found=false;
		WordTree::const_iterator it;
		for (it=Tree.begin();it != Tree.end();it++) {
			if (ppl7::RegEx::match(expression, it->first)) {
				if (Add(Result, it->second, filter)) found=true;
			}
		}
		if (!found) {
			return;
		}

	} else {
		WordTree::const_iterator it=Tree.find(Word);
		if (it != Tree.end()) {
			Add(Result, it->second, filter);
		} else {
			return;
		}
	}
}


void CHashes::RemoveWords(WordTree& Tree, const ppl7::Array& words, const DataTitle* title)
{
	for (size_t i=0;i < words.size();i++) {
		const ppl7::String& key=words[i];
		Tree[key].erase(title->TitleId);
	}
}

int CHashes::AddTitleInternal(uint32_t TitleId, const DataTitle* title)
{
	ppl7::String Tmp;
	ppl7::Array words;
	if (!title) title=wm->GetTitle(TitleId);
	if (!title) return 0;
	if (wm->normalizer.GetWords(title->Artist, words)) {
		AddWords(Artist, words, title);
		//AddWords(Global,words,title);
	}
	if (wm->normalizer.GetWords(title->Title, words)) {
		AddWords(Title, words, title);
		//AddWords(Global,words,title);
	}
	if (wm->normalizer.GetWords(title->Album, words)) {
		AddWords(Album, words, title);
		//AddWords(Global,words,title);
	}
	if (wm->normalizer.GetWords(title->Remarks, words)) {
		AddWords(Remarks, words, title);
		//AddWords(Global,words,title);
	}

	ppl7::String MyTags;
	if (title->ReleaseDate > 0) {
		Tmp.setf("%u", title->ReleaseDate);
		int year=Tmp.mid(0, 4).toInt();
		MyTags.setf("%i;", year);
	}
	const char* tmp;
	tmp=wm->GetVersionText(title->VersionId);
	if (tmp) {
		if (wm->normalizer.GetWords(tmp, words)) {
			AddWords(Version, words, title);
			//AddWords(Global,words,title);
		}
	}
	tmp=wm->GetGenreText(title->GenreId);
	if (tmp) {
		if (wm->normalizer.GetWords(tmp, words)) {
			AddWords(Genre, words, title);
			//AddWords(Global,words,title);
		}
		MyTags.appendf("%s;", tmp);
	}
	tmp=wm->GetLabelText(title->LabelId);
	if (tmp) {
		if (wm->normalizer.GetWords(tmp, words)) {
			AddWords(Label, words, title);
			//AddWords(Global,words,title);
		}
	}

	MyTags+=title->Tags;
	if (GetTags(MyTags, words)) {
		AddWords(Tags, words, title);
		//AddWords(Global,words,title);
	}

	MusicKeys[title->Key].insert(title->TitleId);

	//printf ("%u Einträge in Words-Tree\n",Words.Num());

	return 1;
}

int CHashes::AddTitle(uint32_t TitleId, const DataTitle* title)
{
	if (!title) title=wm->GetTitle(TitleId);
	if (!title) return 0;
	Mutex.lock();
	AddTitleInternal(TitleId, title);
	Mutex.unlock();
	//printf ("%u Einträge in Words-Tree\n",Words.Num());

	return 1;
}

int CHashes::RemoveTitle(uint32_t TitleId, const DataTitle* title)
{
	ppl7::String Tmp;
	ppl7::Array words;
	if (!title) title=wm->GetTitle(TitleId);
	if (!title) return 0;
	Mutex.lock();
	if (wm->normalizer.GetWords(title->Artist, words)) {
		RemoveWords(Artist, words, title);
	}
	if (wm->normalizer.GetWords(title->Title, words)) {
		RemoveWords(Title, words, title);
	}
	if (wm->normalizer.GetWords(title->Album, words)) {
		RemoveWords(Album, words, title);
	}
	if (wm->normalizer.GetWords(title->Remarks, words)) {
		RemoveWords(Remarks, words, title);
	}

	ppl7::String MyTags;
	if (title->ReleaseDate > 0) {
		Tmp.setf("%u", title->ReleaseDate);
		int year=Tmp.mid(0, 4).toInt();
		MyTags.setf("%i;", year);
	}
	const char* tmp;
	tmp=wm->GetVersionText(title->VersionId);
	if (tmp) {
		if (wm->normalizer.GetWords(tmp, words)) {
			RemoveWords(Version, words, title);
		}
	}
	tmp=wm->GetLabelText(title->LabelId);
	if (tmp) {
		if (wm->normalizer.GetWords(tmp, words)) {
			RemoveWords(Label, words, title);
		}
	}
	tmp=wm->GetGenreText(title->GenreId);
	if (tmp) {
		if (wm->normalizer.GetWords(tmp, words)) {
			RemoveWords(Genre, words, title);
		}
		MyTags.appendf("%s;", tmp);
	}
	MyTags+=title->Tags;
	if (GetTags(MyTags, words)) {
		RemoveWords(Tags, words, title);
	}

	MusicKeys[title->Key].erase(title->TitleId);

	Mutex.unlock();
	return 1;
}

void CHashes::run()
{
	if (!wm) return;
	Mutex.lock();
	if (wmlog) wmlog->printf(ppl7::Logger::DEBUG, 1, "CHashes", "ThreadMain", __FILE__, __LINE__, "Erstelle interne Suchtrees");

	Artist.clear();
	Title.clear();
	Version.clear();
	Genre.clear();
	Tags.clear();
	Remarks.clear();
	Album.clear();
	Label.clear();

	CTitleStore* store=&wm->TitleStore;
	if (!store) {
		wmlog->printf(ppl7::Logger::ERR, 1, "CHashes", "ThreadMain", __FILE__, __LINE__, "Kein Titlestore");
		return;
	}
	const DataTitle* ti;
	uint32_t max=store->MaxId();
	for (uint32_t id=0;id <= max;id++) {
		ti=store->GetPtr(id);
		if (ti) {
			AddTitleInternal(id, ti);
		}
	}

	if (wmlog) {
		wmlog->printf(ppl7::Logger::DEBUG, 1, "CHashes", "ThreadMain", __FILE__, __LINE__, "Interne Suchtrees fertig erstellt");
		wmlog->printf(ppl7::Logger::DEBUG, 2, "CHashes", "ThreadMain", __FILE__, __LINE__, "Anzahl Worte in Artist: %zu", Artist.size());
		wmlog->printf(ppl7::Logger::DEBUG, 2, "CHashes", "ThreadMain", __FILE__, __LINE__, "Anzahl Worte in Title: %zu", Title.size());
		wmlog->printf(ppl7::Logger::DEBUG, 2, "CHashes", "ThreadMain", __FILE__, __LINE__, "Anzahl Worte in Version: %zu", Version.size());
		wmlog->printf(ppl7::Logger::DEBUG, 2, "CHashes", "ThreadMain", __FILE__, __LINE__, "Anzahl Worte in Genre: %zu", Genre.size());
		wmlog->printf(ppl7::Logger::DEBUG, 2, "CHashes", "ThreadMain", __FILE__, __LINE__, "Anzahl Worte in Tags: %zu", Tags.size());
		wmlog->printf(ppl7::Logger::DEBUG, 2, "CHashes", "ThreadMain", __FILE__, __LINE__, "Anzahl Worte in Remarks: %zu", Remarks.size());
		wmlog->printf(ppl7::Logger::DEBUG, 2, "CHashes", "ThreadMain", __FILE__, __LINE__, "Anzahl Worte in Album: %zu", Album.size());
		wmlog->printf(ppl7::Logger::DEBUG, 2, "CHashes", "ThreadMain", __FILE__, __LINE__, "Anzahl Worte in Label: %zu", Label.size());
		wmlog->printf(ppl7::Logger::DEBUG, 2, "CHashes", "ThreadMain", __FILE__, __LINE__, "Anzahl MusicKeys: %zu", MusicKeys.size());
		log=wmlog;
	}
	Mutex.unlock();
}

int CHashes::Find(const ppl7::String& Artist, const ppl7::String& Title, TitleTree& Result)
{
	Result.clear();
	// Zuerst die Wortlisten erstellen
	ppl7::Array WordsArtist;
	ppl7::Array WordsTitle;
	wm->normalizer.GetWords(Artist, WordsArtist);
	wm->normalizer.GetWords(Title, WordsTitle);
	if (WordsArtist.size() == 0 && WordsTitle.size() == 0) {
		//ppl6::SetError(20028);
		return 0;
	}
	Mutex.lock();
	//WordsArtist.List("WordsArtist");
	//WordsTitle.List("WordsTitle");
	TitleTree res1, res2, res;
	if (WordsArtist.size()) {
		FindWords(this->Artist, WordsArtist, res1);
		//printf ("Result enthält %i Titel\n",res1.Num());
	}
	if (WordsTitle.size()) {
		FindWords(this->Title, WordsTitle, res2);
		//printf ("Result enthält %i Titel\n",res2.Num());
	}
	Mutex.unlock();

	if (WordsArtist.size() > 0 && WordsTitle.size() > 0) {
		Union(Result, res1, res2);
	} else if (WordsArtist.size() > 0) {
		Copy(Result, res1);
	} else {
		Copy(Result, res2);
	}

	return 1;
}

int CHashes::Find(const ppl7::String& Artist, const ppl7::String& Title, const ppl7::String& Version, const ppl7::String& Genre, const ppl7::String& Tags, const ppl7::String& Label, TitleTree& Result)
{
	Result.clear();
	// Zuerst die Wortlisten erstellen
	ppl7::Array WordsArtist;
	ppl7::Array WordsTitle;
	ppl7::Array WordsVersion;
	ppl7::Array WordsGenre;
	ppl7::Array WordsTags;
	ppl7::Array WordsLabel;
	wm->normalizer.GetWords(Artist, WordsArtist);
	wm->normalizer.GetWords(Title, WordsTitle);
	wm->normalizer.GetWords(Version, WordsVersion);
	wm->normalizer.GetWords(Label, WordsLabel);
	wm->normalizer.GetWords(Genre, WordsGenre);
	GetTags(Tags, WordsTags);
	if (WordsArtist.size() == 0
		&& WordsTitle.size() == 0
		&& WordsVersion.size() == 0
		&& WordsLabel.size() == 0
		&& WordsGenre.size() == 0
		&& WordsTags.size() == 0) {
		//ppl6::SetError(20028);
		return 0;
	}
	Mutex.lock();
	//WordsArtist.List("WordsArtist");
	//WordsTitle.List("WordsTitle");
	TitleTree res1, res2, res3, res4, res5, res6, res;
	if (WordsArtist.size()) {
		FindWords(this->Artist, WordsArtist, res1);
		//printf ("Result enthält %i Titel\n",res1.Num());
	}
	if (WordsTitle.size()) {
		FindWords(this->Title, WordsTitle, res2);
		//printf ("Result enthält %i Titel\n",res2.Num());
	}
	if (WordsVersion.size()) {
		FindWords(this->Version, WordsVersion, res3);
	}
	if (WordsGenre.size()) {
		FindWords(this->Genre, WordsGenre, res4);
	}
	if (WordsTags.size()) {
		FindWords(this->Tags, WordsTags, res5);
	}
	if (WordsLabel.size()) {
		FindWords(this->Label, WordsLabel, res6);
	}

	Mutex.unlock();
	if (WordsArtist.size() > 0) Copy(Result, res1);
	else if (WordsTitle.size() > 0) Copy(Result, res2);
	else if (WordsVersion.size() > 0) Copy(Result, res3);
	else if (WordsGenre.size() > 0) Copy(Result, res4);
	else if (WordsTags.size() > 0) Copy(Result, res5);
	else if (WordsLabel.size() > 0) Copy(Result, res6);

	if (WordsTitle.size() > 0) {
		Copy(res, Result);
		Union(Result, res, res2);
	}
	if (WordsVersion.size() > 0) {
		Copy(res, Result);
		Union(Result, res, res3);
	}
	if (WordsGenre.size() > 0) {
		Copy(res, Result);
		Union(Result, res, res4);
	}
	if (WordsTags.size() > 0) {
		Copy(res, Result);
		Union(Result, res, res5);
	}
	if (WordsLabel.size() > 0) {
		Copy(res, Result);
		Union(Result, res, res6);
	}
	return 1;
}


int CHashes::CheckDupes(const ppl7::String& Artist, const ppl7::String& Title, uint32_t Version, uint32_t Ignore)
{
	TitleTree Result;
	if (!Find(Artist, Title, Result)) return 0;
	if (Result.size() == 0) return 0;
	if (Ignore) {		// Den zu ignorierenden Titel löschen, falls er im Ergebnis vorhanden ist
		Result.erase(Ignore);
	}
	if (Version) {
		const DataTitle* Ti;
		TitleTree::const_iterator it;
		for (it=Result.begin();it != Result.end();it++) {
			Ti=wm->GetTitle(*it);
			if (Ti->VersionId == Version) return 1;
		}
		return 0;
	}
	return Result.size();
}

int CHashes::FindGlobal(const ppl7::String& Query, TitleTree& Result, int Flags, const ResultFilter& filter)
{
	Result.clear();
	if (log) log->printf(ppl7::Logger::DEBUG, 10, "CHashes", "FindGlobal", __FILE__, __LINE__, "Query=%s", (const char*)Query);
	// Zuerst die Wortliste erstellen
	ppl7::Array WordsQuery;
	wm->normalizer.GetWords(Query, WordsQuery);
	/*
	if (WordsQuery.Num()==0) {
		ppl6::SetError(20028);
		return 0;
	}
	*/
	if (!Mutex.tryLock()) {
		if (log) log->printf(ppl7::Logger::DEBUG, 10, "CHashes", "FindGlobal", __FILE__, __LINE__, "Search Trees are not ready yet, waiting...");
		asynchronousMessage message;
		message.setMessageText(asynchronousMessage::tr("search trees are created, please wait..."));
		message.show();
		message.raise();
		qApp->processEvents();
		while (!Mutex.tryLock()) {
			qApp->processEvents();
			ppl7::USleep(100);
		}
	}
	if (WordsQuery.size() > 0) {
		TitleTree TmpResult;
		int wordnum=0;
		for (size_t i=0;i < WordsQuery.size();i++) {
			const ppl7::String& key=WordsQuery[i];
			TmpResult.clear();
			if (Flags & SearchArtist) FindSingleWord(Artist, key, TmpResult, filter);
			if (Flags & SearchTitle) FindSingleWord(Title, key, TmpResult, filter);
			if (Flags & SearchVersion) FindSingleWord(Version, key, TmpResult, filter);
			if (Flags & SearchGenre) FindSingleWord(Genre, key, TmpResult, filter);
			if (Flags & SearchTags) FindSingleWord(Tags, key, TmpResult, filter);
			if (Flags & SearchRemarks) FindSingleWord(Remarks, key, TmpResult, filter);
			if (Flags & SearchAlbum) FindSingleWord(Album, key, TmpResult, filter);
			if (Flags & SearchLabel) FindSingleWord(Label, key, TmpResult, filter);
			if (TmpResult.size() > 0) {
				if (wordnum == 0) Copy(Result, TmpResult);
				else {
					TitleTree LastResult;
					Copy(LastResult, Result);
					Union(Result, LastResult, TmpResult);
				}
				wordnum++;
			} else {
				Result.clear();
				break;
			}
		}
	} else {
		// Leerer Query, alle Titel kommen ins Result-Set, sofern sie den Filter bestehen
		uint32_t max=wm_main->TitleStore.MaxId();
		for (uint32_t i=0;i <= max;i++) {
			const DataTitle* ti=wm_main->TitleStore.GetPtr(i);
			if (ti) {
				if (filter.pass(*ti)) Result.insert(i);
			}
		}
	}
	Mutex.unlock();
	return 1;
}

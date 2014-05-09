/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.10 $
 * $Date: 2011/05/15 09:39:40 $
 * $Id: CHashes.cpp,v 1.10 2011/05/15 09:39:40 pafe Exp $
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
#include "asynchronousMessage.h"

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
	Mutex.Lock();
	Artist.clear();
	Title.clear();
	Version.clear();
	Genre.clear();
	Tags.clear();
	Remarks.clear();
	Album.clear();
	//Global.Clear();
	Mutex.Unlock();
}

int CHashes::GetTags(const ppl6::CString &str, ppl6::CArray &words)
{
	words.Clear();
	ppl6::CString s=str;
	s.Trim();
	if (s.IsEmpty()) return 0;
	// Wir nehmen nur Kleinbuchstaben
	s.LCase();
	ppl6::CArray a;
	s.Replace(",",";");
	words.Explode(s,";",0,true);
	return 1;
}

void CHashes::AddWords(WordTree &Tree, ppl6::CArray &words, const DataTitle *title)
{
	const char *tmp;
	ppl6::CString key;
	words.Reset();
	while ((tmp=words.GetNext())) {
		key=tmp;
		Tree[key].insert(title->TitleId);
	}
}

void CHashes::Copy(TitleTree &Result, const TitleTree &src)
{
	Result.clear();
	TitleTree::const_iterator it;
	for (it=src.begin();it!=src.end();it++) {
		Result.insert(*it);
	}
}

void CHashes::Add(TitleTree &Result, const TitleTree &src)
{
	TitleTree::const_iterator it;
	for (it=src.begin();it!=src.end();it++) {
		Result.insert(*it);
	}
}

void CHashes::Union(TitleTree &Result, const TitleTree &Tree1, const TitleTree &Tree2)
{
	Result.clear();
	// Nur was in Tree1 und Tree2 vorhanden ist, wandert in Result
	const TitleTree *small, *big;
	if (Tree1.size()<Tree2.size()) {
		small=&Tree1;
		big=&Tree2;
	} else {
		small=&Tree2;
		big=&Tree1;
	}
	TitleTree::const_iterator it;
	for (it=small->begin();it!=small->end();it++) {
		if (big->find(*it)!=big->end()) Result.insert(*it);
	}
}

void CHashes::FindWords(const WordTree &Tree, ppl6::CArray &words, TitleTree &Result)
{
	const char *tmp;
	ppl6::CString key;
	words.Reset();
	int wordnum=0;
	while ((tmp=words.GetNext())) {
		key=tmp;
		//printf ("Word: %s",(char*)key);
		if (key.Instr("*")>=0) {
			key.Replace("*",".*");
			ppl6::CString expression;
			expression="/^"+key+"$/i";

			if (log) log->Printf(ppl6::LOG::DEBUG,10,"CHashes","FindWords",__FILE__,__LINE__,"Wildcard-Search: %s",(const char*)expression);
			// Wildcard Suche
			bool found=false;
			WordTree::const_iterator it;
			for (it=Tree.begin();it!=Tree.end();it++) {
				if(ppl6::PregMatch(expression,it->first)) {
					found=true;
					if (!wordnum) Copy(Result,it->second);
					else Add(Result,it->second);
					wordnum++;
				}
			}
			if (!found) {
				Result.clear();
				return;
			}

		} else {
			WordTree::const_iterator it=Tree.find(key);
			if (it!=Tree.end()) {
				if (!wordnum) Copy(Result,it->second);
				else {
					TitleTree res2;
					Union(res2,Result,it->second);
					Copy(Result,res2);
				}
			} else {
				Result.clear();
				return;
			}
			wordnum++;
		}
	}
}


void CHashes::FindSingleWord(const WordTree &Tree, const ppl6::CString &Word, TitleTree &Result)
{
	if (Word.Instr("*")>=0) {
		ppl6::CString Tmp=Word;
		Tmp.Replace("*",".*");
		ppl6::CString expression;
		expression="/^"+Tmp+"$/i";
		// Wildcard Suche
		bool found=false;
		WordTree::const_iterator it;
		for (it=Tree.begin();it!=Tree.end();it++) {
			if(ppl6::PregMatch(expression,it->first)) {
				found=true;
				Add(Result,it->second);
			}
		}
		if (!found) {
			return;
		}

	} else {
		WordTree::const_iterator it=Tree.find(Word);
		if (it!=Tree.end()) {
			Add(Result,it->second);
		} else {
			return;
		}
	}
}


void CHashes::RemoveWords(WordTree &Tree, ppl6::CArray &words, const DataTitle *title)
{
	const char *tmp;
	ppl6::CString key;
	words.Reset();
	while ((tmp=words.GetNext())) {
		key=tmp;
		Tree[key].erase(title->TitleId);
	}
}

int CHashes::AddTitleInternal(ppluint32 TitleId, const DataTitle *title)
{
	ppl6::CString Tmp;
	ppl6::CArray words;
	if (!title) title=wm->GetTitle(TitleId);
	if (!title) return 0;
	if (wm->GetWords(title->Artist,words)) {
		AddWords(Artist,words,title);
		//AddWords(Global,words,title);
	}
	if (wm->GetWords(title->Title,words)) {
		AddWords(Title,words,title);
		//AddWords(Global,words,title);
	}
	if (wm->GetWords(title->Album,words)) {
		AddWords(Album,words,title);
		//AddWords(Global,words,title);
	}
	if (wm->GetWords(title->Remarks,words)) {
		AddWords(Remarks,words,title);
		//AddWords(Global,words,title);
	}

	ppl6::CString MyTags;
	if (title->ReleaseDate>0) {
		Tmp.Setf("%u",title->ReleaseDate);
		int year=Tmp.Mid(0,4).ToInt();
		MyTags.Setf("%i;",year);
	}
	const char *tmp;
	tmp=wm->GetVersionText(title->VersionId);
	if (tmp) {
		if (wm->GetWords(tmp,words)) {
			AddWords(Version,words,title);
			//AddWords(Global,words,title);
		}
	}
	tmp=wm->GetGenreText(title->GenreId);
	if (tmp) {
		if (wm->GetWords(tmp,words)) {
			AddWords(Genre,words,title);
			//AddWords(Global,words,title);
		}
		MyTags.Concatf("%s;",tmp);
	}
	tmp=wm->GetLabelText(title->LabelId);
	if (tmp) {
		if (wm->GetWords(tmp,words)) {
			AddWords(Label,words,title);
			//AddWords(Global,words,title);
		}
	}

	MyTags+=title->Tags;
	if (GetTags(MyTags,words)) {
		AddWords(Tags,words,title);
		//AddWords(Global,words,title);
	}

	MusicKeys[title->Key].insert(title->TitleId);

	//printf ("%u Einträge in Words-Tree\n",Words.Num());

	return 1;
}

int CHashes::AddTitle(ppluint32 TitleId, const DataTitle *title)
{
	ppl6::CAssocArray words;
	if (!title) title=wm->GetTitle(TitleId);
	if (!title) return 0;
	Mutex.Lock();
	AddTitleInternal(TitleId,title);
	Mutex.Unlock();
	//printf ("%u Einträge in Words-Tree\n",Words.Num());

	return 1;
}

int CHashes::RemoveTitle(ppluint32 TitleId, const DataTitle *title)
{
	ppl6::CString Tmp;
	ppl6::CArray words;
	if (!title) title=wm->GetTitle(TitleId);
	if (!title) return 0;
	Mutex.Lock();
	if (wm->GetWords(title->Artist,words)) {
		RemoveWords(Artist,words,title);
	}
	if (wm->GetWords(title->Title,words)) {
		RemoveWords(Title,words,title);
	}
	if (wm->GetWords(title->Album,words)) {
		RemoveWords(Album,words,title);
	}
	if (wm->GetWords(title->Remarks,words)) {
		RemoveWords(Remarks,words,title);
	}

	ppl6::CString MyTags;
	if (title->ReleaseDate>0) {
		Tmp.Setf("%u",title->ReleaseDate);
		int year=Tmp.Mid(0,4).ToInt();
		MyTags.Setf("%i;",year);
	}
	const char *tmp;
	tmp=wm->GetVersionText(title->VersionId);
	if (tmp) {
		if (wm->GetWords(tmp,words)) {
			RemoveWords(Version,words,title);
		}
	}
	tmp=wm->GetLabelText(title->LabelId);
	if (tmp) {
		if (wm->GetWords(tmp,words)) {
			RemoveWords(Label,words,title);
		}
	}
	tmp=wm->GetGenreText(title->GenreId);
	if (tmp) {
		if (wm->GetWords(tmp,words)) {
			RemoveWords(Genre,words,title);
		}
		MyTags.Concatf("%s;",tmp);
	}
	MyTags+=title->Tags;
	if (GetTags(MyTags,words)) {
		RemoveWords(Tags,words,title);
	}

	MusicKeys[title->Key].erase(title->TitleId);

	Mutex.Unlock();
	return 1;
}

void CHashes::ThreadMain(void *)
{
	if (!wm) return;
	Mutex.Lock();
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CHashes","ThreadMain",__FILE__,__LINE__,"Erstelle interne Suchtrees");

	Artist.clear();
	Title.clear();
	Version.clear();
	Genre.clear();
	Tags.clear();
	Remarks.clear();
	Album.clear();
	Label.clear();

	CTitleStore *store=&wm->TitleStore;
	if (!store) {
		wmlog->Printf(ppl6::LOG::ERROR,1,"CHashes","ThreadMain",__FILE__,__LINE__,"Kein Titlestore");
		return;
	}
	DataTitle *ti;
	ppluint32 max=store->MaxId();
	for (ppluint32 id=0;id<=max;id++) {
		ti=store->Get(id);
		if (ti) {
			AddTitleInternal(id,ti);
		}
	}

	if (wmlog) {
		wmlog->Printf(ppl6::LOG::DEBUG,1,"CHashes","ThreadMain",__FILE__,__LINE__,"Interne Suchtrees fertig erstellt");
		wmlog->Printf(ppl6::LOG::DEBUG,2,"CHashes","ThreadMain",__FILE__,__LINE__,"Anzahl Worte in Artist: %u",Artist.size());
		wmlog->Printf(ppl6::LOG::DEBUG,2,"CHashes","ThreadMain",__FILE__,__LINE__,"Anzahl Worte in Title: %u",Title.size());
		wmlog->Printf(ppl6::LOG::DEBUG,2,"CHashes","ThreadMain",__FILE__,__LINE__,"Anzahl Worte in Version: %u",Version.size());
		wmlog->Printf(ppl6::LOG::DEBUG,2,"CHashes","ThreadMain",__FILE__,__LINE__,"Anzahl Worte in Genre: %u",Genre.size());
		wmlog->Printf(ppl6::LOG::DEBUG,2,"CHashes","ThreadMain",__FILE__,__LINE__,"Anzahl Worte in Tags: %u",Tags.size());
		wmlog->Printf(ppl6::LOG::DEBUG,2,"CHashes","ThreadMain",__FILE__,__LINE__,"Anzahl Worte in Remarks: %u",Remarks.size());
		wmlog->Printf(ppl6::LOG::DEBUG,2,"CHashes","ThreadMain",__FILE__,__LINE__,"Anzahl Worte in Album: %u",Album.size());
		wmlog->Printf(ppl6::LOG::DEBUG,2,"CHashes","ThreadMain",__FILE__,__LINE__,"Anzahl Worte in Label: %u",Label.size());
		wmlog->Printf(ppl6::LOG::DEBUG,2,"CHashes","ThreadMain",__FILE__,__LINE__,"Anzahl MusicKeys: %u",MusicKeys.size());
		log=wmlog;
	}
	Mutex.Unlock();

	/*
	ppl6::CString Artist="Pet Shop Boys", Title;
	CTitleHashTree Result;
	Find(Artist,Title,Result);
	Result.PrintNodes();
	*/

}

int CHashes::Find(const ppl6::CString &Artist, const ppl6::CString &Title, TitleTree &Result)
{
	Result.clear();
	// Zuerst die Wortlisten erstellen
	ppl6::CArray WordsArtist;
	ppl6::CArray WordsTitle;
	wm->GetWords(Artist,WordsArtist);
	wm->GetWords(Title,WordsTitle);
	if (WordsArtist.Num()==0 && WordsTitle.Num()==0) {
		ppl6::SetError(20028);
		return 0;
	}
	Mutex.Lock();
	//WordsArtist.List("WordsArtist");
	//WordsTitle.List("WordsTitle");
	TitleTree res1, res2, res;
	if (WordsArtist.Num()) {
		FindWords(this->Artist,WordsArtist,res1);
		//printf ("Result enthält %i Titel\n",res1.Num());
	}
	if (WordsTitle.Num()) {
		FindWords(this->Title,WordsTitle,res2);
		//printf ("Result enthält %i Titel\n",res2.Num());
	}
	Mutex.Unlock();

	if (WordsArtist.Num()>0 && WordsTitle.Num()>0) {
		Union(Result,res1,res2);
	} else if (WordsArtist.Num()>0) {
		Copy(Result,res1);
	} else {
		Copy(Result,res2);
	}

	return 1;
}

int CHashes::Find(const ppl6::CString &Artist, const ppl6::CString &Title, const ppl6::CString &Version, const ppl6::CString &Genre, const ppl6::CString &Tags, const ppl6::CString &Label, TitleTree &Result)
{
	Result.clear();
	// Zuerst die Wortlisten erstellen
	ppl6::CArray WordsArtist;
	ppl6::CArray WordsTitle;
	ppl6::CArray WordsVersion;
	ppl6::CArray WordsGenre;
	ppl6::CArray WordsTags;
	ppl6::CArray WordsLabel;
	wm->GetWords(Artist,WordsArtist);
	wm->GetWords(Title,WordsTitle);
	wm->GetWords(Version,WordsVersion);
	wm->GetWords(Label,WordsLabel);
	wm->GetWords(Genre,WordsGenre);
	GetTags(Tags,WordsTags);
	if (WordsArtist.Num()==0
			&& WordsTitle.Num()==0
			&& WordsVersion.Num()==0
			&& WordsLabel.Num()==0
			&& WordsGenre.Num()==0
			&& WordsTags.Num()==0) {
		ppl6::SetError(20028);
		return 0;
	}
	Mutex.Lock();
	//WordsArtist.List("WordsArtist");
	//WordsTitle.List("WordsTitle");
	TitleTree res1, res2, res3, res4, res5, res6, res;
	if (WordsArtist.Num()) {
		FindWords(this->Artist,WordsArtist,res1);
		//printf ("Result enthält %i Titel\n",res1.Num());
	}
	if (WordsTitle.Num()) {
		FindWords(this->Title,WordsTitle,res2);
		//printf ("Result enthält %i Titel\n",res2.Num());
	}
	if (WordsVersion.Num()) {
		FindWords(this->Version,WordsVersion,res3);
	}
	if (WordsGenre.Num()) {
		FindWords(this->Genre,WordsGenre,res4);
	}
	if (WordsTags.Num()) {
		FindWords(this->Tags,WordsTags,res5);
	}
	if (WordsLabel.Num()) {
		FindWords(this->Label,WordsLabel,res6);
	}

	Mutex.Unlock();
	if (WordsArtist.Num()>0) Copy(Result,res1);
	else if (WordsTitle.Num()>0) Copy(Result,res2);
	else if (WordsVersion.Num()>0) Copy(Result,res3);
	else if (WordsGenre.Num()>0) Copy(Result,res4);
	else if (WordsTags.Num()>0) Copy(Result,res5);
	else if (WordsLabel.Num()>0) Copy(Result,res6);

	if (WordsTitle.Num()>0 ) {
		Copy(res,Result);
		Union(Result,res,res2);
	}
	if (WordsVersion.Num()>0 ) {
		Copy(res,Result);
		Union(Result,res,res3);
	}
	if (WordsGenre.Num()>0 ) {
		Copy(res,Result);
		Union(Result,res,res4);
	}
	if (WordsTags.Num()>0 ) {
		Copy(res,Result);
		Union(Result,res,res5);
	}
	if (WordsLabel.Num()>0 ) {
		Copy(res,Result);
		Union(Result,res,res6);
	}
	return 1;
}


int CHashes::CheckDupes(const ppl6::CString &Artist, const ppl6::CString &Title, ppluint32 Version, ppluint32 Ignore)
{
	TitleTree Result;
	if (!Find(Artist,Title,Result)) return 0;
	if (Result.size()==0) return 0;
	if (Ignore) {		// Den zu ignorierenden Titel löschen, falls er im Ergebnis vorhanden ist
		Result.erase(Ignore);
	}
	if (Version) {
		DataTitle *Ti;
		TitleTree::const_iterator it;
		for (it=Result.begin();it!=Result.end();it++) {
			Ti=wm->GetTitle(*it);
			if (Ti->VersionId==Version) return 1;
		}
		return 0;
	}
	return Result.size();
}

int CHashes::FindGlobal(const ppl6::CString &Query, TitleTree &Result, int Flags)
{
	Result.clear();
	if (log) log->Printf(ppl6::LOG::DEBUG,10,"CHashes","FindGlobal",__FILE__,__LINE__,"Query=%s",(const char*)Query);
	// Zuerst die Wortliste erstellen
	ppl6::CArray WordsQuery;
	wm->GetWords(Query,WordsQuery);
	if (WordsQuery.Num()==0) {
		ppl6::SetError(20028);
		return 0;
	}
	if (!Mutex.TryLock()) {
		if (log) log->Printf(ppl6::LOG::DEBUG,10,"CHashes","FindGlobal",__FILE__,__LINE__,"Search Trees are not ready yet, waiting...");
		asynchronousMessage message;
		message.setMessageText(asynchronousMessage::tr("search trees are created, please wait..."));
		message.show();
		message.raise();
		qApp->processEvents();
		while (!Mutex.TryLock()) {
			qApp->processEvents();
			ppl6::USleep(100);
		}
	}
	TitleTree TmpResult;
	WordsQuery.Reset();
	ppl6::CString key;
	int wordnum=0;
	const char*tmp;
	while ((tmp=WordsQuery.GetNext())) {
		key=tmp;
		TmpResult.clear();
		if (Flags&SearchArtist) FindSingleWord(Artist,key,TmpResult);
		if (Flags&SearchTitle) FindSingleWord(Title,key,TmpResult);
		if (Flags&SearchVersion) FindSingleWord(Version,key,TmpResult);
		if (Flags&SearchGenre) FindSingleWord(Genre,key,TmpResult);
		if (Flags&SearchTags) FindSingleWord(Tags,key,TmpResult);
		if (Flags&SearchRemarks) FindSingleWord(Remarks,key,TmpResult);
		if (Flags&SearchAlbum) FindSingleWord(Album,key,TmpResult);
		if (Flags&SearchLabel) FindSingleWord(Label,key,TmpResult);
		if (TmpResult.size()>0) {
			if (wordnum==0) Copy(Result,TmpResult);
			else {
				TitleTree LastResult;
				Copy(LastResult,Result);
				Union(Result,LastResult,TmpResult);
			}
			wordnum++;
		} else {
			Result.clear();
			break;
		}

	}
	Mutex.Unlock();
	return 1;
}

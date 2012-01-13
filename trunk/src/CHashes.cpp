/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: CHashes.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
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

CHashes::CHashes()
{
	wm=NULL;
	Artist.Name="Artist";
	Title.Name="Title";
	Global.Name="Global";
	log=NULL;
}

CHashes::~CHashes()
{
	Artist.Clear();
	Title.Clear();
	Global.Clear();
}

void CHashes::Clear()
{
	Mutex.Lock();
	Artist.Clear();
	Title.Clear();
	Global.Clear();
	Mutex.Unlock();
}

int CHashes::GetWords(const ppl6::CString &str, ppl6::CAssocArray &words)
{
	words.Clear();
	ppl6::CString s=str;
	s.Trim();
	if (s.IsEmpty()) return 0;
	// Wir nehmen nur Kleinbuchstaben
	s.LCase();
	// Bestimmte Zeichen filtern wir raus
	wm->NormalizeTerm(s);
	/*
	s.Replace(" feat.","");
	s.Replace(" pres.","");
	s.Replace(" vs.","");
	s.Replace(" featuring ","");
	s.Replace(" versus ","");
	s.Replace(" presents ","");
	s.Replace(" and ","");
	s.Replace(".","");
	s.Replace("&","");
	s.Replace("+","");
	s.Replace(",","");
	s.Replace("-"," ");
	*/

	ppl6::CArray a;
	a.Explode(s," ",0,true);
	a.Reset();
	const char *tmp;
	while ((tmp=a.GetNext())) {
		words.Set(tmp,"1");
	}
	return 1;
}

void CHashes::AddWords(CWordTree &Tree, ppl6::CAssocArray &words, const DataTitle *title)
{
	ppl6::CString key,value;
	CWord find, *item;
	if (log) log->Printf(ppl6::LOG::DEBUG,10,"CHashes","AddWords",__FILE__,__LINE__,"Hash=%s, Title=%i",(const char*)Tree.Name,title->TitleId);
	words.Reset();
	while (words.GetNext(&key,&value)) {
		//printf ("Word: %s",(char*)key);
		if (log) log->Printf(ppl6::LOG::DEBUG,10,"CHashes","AddWords",__FILE__,__LINE__,"Searching word: %s",(const char*)key);
		find.Word=key;
		item=(CWord*)Tree.Find(&find);
		if (item) {
			item->Titles.Add(title);
			if (log) log->Printf(ppl6::LOG::DEBUG,10,"CHashes","AddWords",__FILE__,__LINE__,"Word found, adding Title %i, %i references",title->TitleId, item->Titles.Num());
			//printf (", gefunden\n");

		} else {
			//printf (", neu\n");
			item=new CWord;
			item->Word=key;
			item->Titles.Add(title);
			Tree.Add(item);
			if (log) log->Printf(ppl6::LOG::DEBUG,10,"CHashes","AddWords",__FILE__,__LINE__,"New word, adding Title %i to tree, %i references",title->TitleId, item->Titles.Num());
		}
	}
}

void CHashes::Copy(CTitleHashTree &Result, CTitleHashTree &src)
{
	Result.Clear();
	src.Reset();
	void *v;
	while ((v=src.GetNext())) Result.Add(v);
}

void CHashes::FindWords(CWordTree &Tree, ppl6::CAssocArray &words, CTitleHashTree &Result)
{
	ppl6::CString key,value;
	CWord find, *item;
	words.Reset();
	int wordnum=0;
	while (words.GetNext(&key,&value)) {
		//printf ("Word: %s",(char*)key);
		if (log) log->Printf(ppl6::LOG::DEBUG,10,"CHashes","FindWords",__FILE__,__LINE__,"Search Word: %s",(const char*)key);
		find.Word=key;
		item=(CWord*)Tree.Find(&find);
		if (item) {
			if (log) log->Printf(ppl6::LOG::DEBUG,10,"CHashes","FindWords",__FILE__,__LINE__,"%i Tracks found",item->Titles.Num());
			//printf (", gefunden, %i Titel\n",item->Titles.Num());
			if (!wordnum) Copy(Result,item->Titles);
			else {
				CTitleHashTree res2;
				Union(res2,Result,item->Titles);
				Copy(Result,res2);
				if (log) log->Printf(ppl6::LOG::DEBUG,10,"CHashes","FindWords",__FILE__,__LINE__,"%i Tracks after union",Result.Num());
			}
		} else {
			if (log) log->Printf(ppl6::LOG::DEBUG,10,"CHashes","FindWords",__FILE__,__LINE__,"No Tracks found, clearing Result");
			Result.Clear();
			return;
		}
		wordnum++;
	}
}

void CHashes::RemoveWords(CWordTree &Tree, ppl6::CAssocArray &words, const DataTitle *title)
{
	ppl6::CString key,value;
	CWord find, *item;

	words.Reset();
	while (words.GetNext(&key,&value)) {
		//printf ("Word: %s",(char*)key);
		find.Word=key;
		item=(CWord*)Tree.Find(&find);
		if (item) {
			//printf (", gefunden\n");
			item->Titles.Delete(title);
		}
	}
}

int CHashes::AddTitleInternal(ppluint32 TitleId, const DataTitle *title)
{
	ppl6::CAssocArray words;
	if (!title) title=wm->GetTitle(TitleId);
	if (!title) return 0;
	if (GetWords(title->Artist,words)) {
		AddWords(Artist,words,title);
		AddWords(Global,words,title);
	}
	if (GetWords(title->Title,words)) {
		AddWords(Title,words,title);
		AddWords(Global,words,title);
	}

	//printf ("%u Einträge in Words-Tree\n",Words.Num());

	return 1;
}

int CHashes::AddTitle(ppluint32 TitleId, const DataTitle *title)
{
	ppl6::CAssocArray words;
	if (!title) title=wm->GetTitle(TitleId);
	if (!title) return 0;
	Mutex.Lock();
	if (GetWords(title->Artist,words)) {
		AddWords(Artist,words,title);
		AddWords(Global,words,title);
	}
	if (GetWords(title->Title,words)) {
		AddWords(Title,words,title);
		AddWords(Global,words,title);
	}
	Mutex.Unlock();
	//printf ("%u Einträge in Words-Tree\n",Words.Num());

	return 1;
}

int CHashes::RemoveTitle(ppluint32 TitleId, const DataTitle *title)
{
	ppl6::CAssocArray words;
	if (!title) title=wm->GetTitle(TitleId);
	if (!title) return 0;
	Mutex.Lock();
	if (GetWords(title->Artist,words)) {
		RemoveWords(Artist,words,title);
		RemoveWords(Global,words,title);
	}
	if (GetWords(title->Title,words)) {
		RemoveWords(Title,words,title);
		RemoveWords(Global,words,title);
	}
	Mutex.Unlock();
	return 1;
}

void CHashes::ThreadMain(void *param)
{
	if (!wm) return;
	Mutex.Lock();
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CHashes","ThreadMain",__FILE__,__LINE__,"Erstelle interne Suchtrees");

	Artist.Clear();
	Title.Clear();
	Global.Clear();
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
		wmlog->Printf(ppl6::LOG::DEBUG,2,"CHashes","ThreadMain",__FILE__,__LINE__,"Anzahl Worte in Artist: %u",Artist.Num());
		wmlog->Printf(ppl6::LOG::DEBUG,2,"CHashes","ThreadMain",__FILE__,__LINE__,"Anzahl Worte in Title: %u",Title.Num());
		wmlog->Printf(ppl6::LOG::DEBUG,2,"CHashes","ThreadMain",__FILE__,__LINE__,"Anzahl Worte global: %u",Global.Num());
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

void CHashes::Union(CTitleHashTree &Result, CTitleHashTree &Tree1,CTitleHashTree &Tree2)
{
	Result.Clear();
	// Nur was in Tree1 und Tree2 vorhanden ist, wandert in Result
	CTitleHashTree *small, *big;
	if (Tree1.Num()<Tree2.Num()) {
		small=&Tree1;
		big=&Tree2;
	} else {
		small=&Tree2;
		big=&Tree1;
	}
	small->Reset();
	void *v;
	while ((v=small->GetNext())) {
		if (big->Find(v)) Result.Add(v);
	}
}

int CHashes::Find(const ppl6::CString &Artist, const ppl6::CString &Title, CTitleHashTree &Result)
{
	Result.Clear();
	// Zuerst die Wortlisten erstellen
	ppl6::CAssocArray WordsArtist;
	ppl6::CAssocArray WordsTitle;
	GetWords(Artist,WordsArtist);
	GetWords(Title,WordsTitle);
	if (WordsArtist.Count()==0 && WordsTitle.Count()==0) {
		ppl6::SetError(20028);
		return 0;
	}
	Mutex.Lock();
	//WordsArtist.List("WordsArtist");
	//WordsTitle.List("WordsTitle");
	CTitleHashTree res1, res2, res;
	if (WordsArtist.Count()) {
		FindWords(this->Artist,WordsArtist,res1);
		//printf ("Result enthält %i Titel\n",res1.Num());
	}
	if (WordsTitle.Count()) {
		FindWords(this->Title,WordsTitle,res2);
		//printf ("Result enthält %i Titel\n",res2.Num());
	}
	Mutex.Unlock();

	if (WordsArtist.Count()>0 && WordsTitle.Count()>0) {
		Union(Result,res1,res2);
	} else if (WordsArtist.Count()>0) {
		Copy(Result,res1);
	} else {
		Copy(Result,res2);
	}

	return 1;
}

int CHashes::CheckDupes(const ppl6::CString &Artist, const ppl6::CString &Title, ppluint32 Version, ppluint32 Ignore)
{
	CTitleHashTree Result;
	if (!Find(Artist,Title,Result)) return 0;
	if (Result.Num()==0) return 0;
	if (Ignore) {		// Den zu ignorierenden Titel löschen, falls er im Ergebnis vorhanden ist
		DataTitle Ti;
		Ti.TitleId=Ignore;
		Result.Delete(&Ti);
	}
	if (Version) {
		DataTitle *Ti;
		Result.Reset();
		while ((Ti=(DataTitle*)Result.GetNext())) {
			if (Ti->VersionId==Version) return 1;
		}
		return 0;
	}
	return Result.Num();
}

int CHashes::FindGlobal(const ppl6::CString &Query, CTitleHashTree &Result)
{
	Result.Clear();
	if (log) log->Printf(ppl6::LOG::DEBUG,10,"CHashes","FindGlobal",__FILE__,__LINE__,"Query=%s",(const char*)Query);
	// Zuerst die Wortliste erstellen
	ppl6::CAssocArray WordsQuery;
	GetWords(Query,WordsQuery);
	if (WordsQuery.Count()==0) {
		ppl6::SetError(20028);
		return 0;
	}
	Mutex.Lock();
	if (log) log->PrintArraySingleLine(ppl6::LOG::DEBUG,10,"CHashes","FindGlobal",__FILE__,__LINE__,&WordsQuery,"Words: ");
	//WordsQuery.List("words");
	FindWords(Global,WordsQuery,Result);
	//printf ("Gefundene Tracks: %u\n",Result.Num());
	if (log) log->Printf(ppl6::LOG::DEBUG,10,"CHashes","FindGlobal",__FILE__,__LINE__,"Gefundene Tracks: %i",Result.Num());
	Mutex.Unlock();
	return 1;
}

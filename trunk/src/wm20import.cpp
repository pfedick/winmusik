/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/05/16 12:23:30 $
 * $Id: wm20import.cpp,v 1.1 2010/05/16 12:23:30 pafe Exp $
 *
 *
 * Copyright (c) 2009 Patrick Fedick
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "winmusik3.h"
#include "wm20import.h"


ImportProgress::ImportProgress()
{
	progress=0;
	current=0;
	import_titeldatei=false;
	import_traegertitel=false;
	import_versionen=false;
	import_musikarten=false;
	import_quellen=false;
	import_kaufquellen=false;
	import_label=false;
	import_geraete=false;
	import_kassetten=false;
	import_cds=false;
	import_platten=false;
	import_mp3=false;
	import_daten=false;
	import_video=false;
	import_band=false;
	import_sonstige=false;
}

ImportProgress::~ImportProgress()
{

}

void ImportProgress::Update()
{

}



int CWmClient::ImportDatabaseWM20()
{
	wm20::Cwm20Import Import;
	Storage.DeleteDatabase();
	if (!Import.Load("/home/patrick/WMusik32/daten",&Storage,NULL)) {
		return 0;
	}
	return 1;

}




namespace wm20 {


Cwm20TraegerTitel::Cwm20TraegerTitel()
{
	titel=NULL;
	num=0;
}

int Cwm20TraegerTitel::Load(const char *path)
{
	if (!ff.Open("%s/traeger.tit","rb",path)) {
		ppl6::SetError(10012,"%s/traeger.tit",path);
		return 0;
	}
	num=ff.Size()/sizeof(TraegerTitel);
	WMLOG(ppl6::LOG::DEBUG,2) "Import: %i Traeger-Titel in Database, loading...",num);
	titel=(TraegerTitel*) ((ppl6::CFileObject)ff).Load();
	if (!titel) return 0;

	return 1;
}

int Cwm20TraegerTitel::Get(int index, ppl6::CString *Titel1, ppl6::CString *Titel2)
{
	if (!index) {
		ppl6::SetError(194,"int Cwm20TraegerTitel::Get( ==> int index <== , ppl6::CpplString *Titel1, ppl6::CpplString *Titel2)");
		return 0;
	}
	if (!Titel1) {
		ppl6::SetError(194,"int Cwm20TraegerTitel::Get(int index, ==> ppl6::CpplString *Titel1 <== , ppl6::CpplString *Titel2)");
		return 0;
	}
	if (!Titel2) {
		ppl6::SetError(194,"int Cwm20TraegerTitel::Get(int index, ppl6::CpplString *Titel1, ==> ppl6::CpplString *Titel2 <==)");
		return 0;
	}
	index--;
	if (index<num) {
		Titel1->Set(titel[index].Titel1,80);
		Titel2->Set(titel[index].Titel2,80);
		Titel1->Trim();
		Titel2->Trim();
		return 1;
	}
	ppl6::SetError(10014);
	return 0;

}


Cwm20TraegerVerzeichnis::Cwm20TraegerVerzeichnis()
{
	vz=NULL;
	num=0;
}

int Cwm20TraegerVerzeichnis::Load(const char *path, const char *file)
{
	if (!ff.Open("%s/%s","rb",path,file)) {
		WMLOG(ppl6::LOG::DEBUG,2) "Import: %i Traeger in %s...",0, file);
		return 1;
		ppl6::SetError(10012,"%s/%s",path,file);
		return 0;
	}
	num=ff.Size()/sizeof(TraegerVerzeichnis);
	WMLOG(ppl6::LOG::DEBUG,2) "Import: %i Traeger in %s...",num, file);
	vz=(TraegerVerzeichnis*)ff.Load();
	if (!vz) return 0;
	return 1;
}

TraegerVerzeichnis *Cwm20TraegerVerzeichnis::Get(int index)
{
	if (!index) {
		ppl6::SetError(194,"TraegerVerzeichnis20 *Cwm20TraegerTitel::Get(int index)");
		return 0;
	}
	index--;
	if (index<num) return &vz[index];
	ppl6::SetError(10014);
	return 0;
}

Cwm20TraegerDaten::Cwm20TraegerDaten()
{
	td=NULL;
	num=0;
}

int Cwm20TraegerDaten::Load(const char *path, const char *file)
{
	if (!ff.Open("%s/%s","rb",path,file)) {
		WMLOG(ppl6::LOG::DEBUG,2) "Import: %i Tracks in TraegerDaten of %s...",0, file);
		return 1;
		ppl6::SetError(10012,"%s/%s",path,file);
		return 0;
	}
	num=ff.Size()/sizeof(TraegerDatensatz);
	WMLOG(ppl6::LOG::DEBUG,2) "Import: %i Tracks in TraegerDaten of %s...",num, file);
	td=(TraegerDatensatz*)ff.Load();
	if (!td) return 0;
	return 1;
}

TraegerDatensatz *Cwm20TraegerDaten::Get(int index)
{
	if (!index) {
		ppl6::SetError(194,"TraegerVerzeichnis20 *Cwm20TraegerTitel::Get(int index)");
		return 0;
	}
	index--;
	if (index<num) return &td[index];
	ppl6::SetError(10014);
	return 0;
}





/*******************************************************************
 * class Cwm20Import
 ******************************************************************/
Cwm20Import::Cwm20Import()
{
	progress=NULL;
	charset="ISO-8859-1";
}

Cwm20Import::~Cwm20Import()
{
}


void Cwm20Import::SetCharset(const char *charset)
{
	this->charset=charset;
}

int Cwm20Import::Load(const char *path, CStorage *storage, ImportProgress *progress)
{
	if (!path) {
		ppl6::SetError(194,"int CwmServer::Import(char *path)");
		return 0;
	}
	Storage=storage;
	this->progress=progress;
	if (progress) progress->progress=0;
	if (strlen(path)==0) path=".";
	if (!ppl6::IsDir(path)) {
		ppl6::SetError(10013,"%s",path);
		return 0;
	}
	WMLOG(ppl6::LOG::INFO,1) "Import: Importing Data from WinMusik 2.x from %s",path);
	//printf ("Importing Data from WinMusik 2.x from %s\n",path);

	storage->StartImport();
	ImportShortcuts(path);
	if (progress) {
		progress->progress=2;
		progress->import_shortcuts=true;
		progress->Update();
	}

	ImportTable(path,"version.bez",80,"VERS");
	if (progress) {
		progress->progress=5;
		progress->import_versionen=true;
		progress->Update();
	}
	ImportTable(path,"aufnahme.bez",80,"RDEV");
	if (progress) {
		progress->progress=10;
		progress->import_geraete=true;
		progress->Update();
	}
	ImportTable(path,"kauf.bez",80,"PCHS");
	if (progress) {
		progress->progress=15;
		progress->import_kaufquellen=true;
		progress->Update();
	}

	ImportTable(path,"label.bez",80,"LABL");
	if (progress) {
		progress->progress=20;
		progress->import_label=true;
		progress->Update();
	}

	ImportTable(path,"musikart.bez",80,"GENR");
	if (progress) {
		progress->progress=25;
		progress->import_musikarten=true;
		progress->Update();
	}

	ImportTable(path,"quelle.bez",80,"RSRC");
	if (progress) {
		progress->progress=30;
		progress->import_quellen=true;
		progress->Update();
	}
	ImportTitle(path);
	if (progress) {
		progress->progress=70;
		progress->import_titeldatei=true;
		progress->Update();
	}

	ImportDevices(path,"kassette",1);
	if (progress) {
		progress->progress=75;
		progress->import_kassetten=true;
		progress->Update();
	}

	ImportDevices(path,"cd",2);
	if (progress) {
		progress->progress=80;
		progress->import_cds=true;
		progress->Update();
	}
	ImportDevices(path,"daten",3);
	if (progress) {
		progress->progress=82;
		progress->import_daten=true;
		progress->Update();
	}
	ImportDevices(path,"platte",4);
	if (progress) {
		progress->progress=85;
		progress->import_platten=true;
		progress->Update();
	}

	ImportDevices(path,"video",5);
	if (progress) {
		progress->progress=88;
		progress->import_video=true;
		progress->Update();
	}

	ImportDevices(path,"sonstige",6);
	if (progress) {
		progress->progress=90;
		progress->import_sonstige=true;
		progress->Update();
	}

	ImportDevices(path,"mpeg",7);
	if (progress) {
		progress->progress=98;
		progress->import_mp3=true;
		progress->Update();
	}

	ImportDevices(path,"band",8);
	if (progress) {
		progress->progress=100;
		progress->import_band=true;
		progress->Update();
	}
	storage->FinishImport();
	return 1;
}


int Cwm20Import::ImportTable(const char *path, const char *file, int width, const char *chunktype)
{
	CTableStore *s;
	CSimpleTable v;
	ppl6::CString Text;
	s=(CTableStore*)Storage->FindStorageType(chunktype);
	if (!s) return 0;
	ppl6::CFile ff;
	if (!ff.Open("%s/%s","rb",path,file)) {
		ppl6::SetError(10012,"%s/%s",path,file);
		return 0;
	}
	int num=ff.Size()/width;
	if (progress) {
		progress->current=0;
		progress->Update();
	}
	int p=0;
	for (int i=0;i<num;i++) {
		if (i*100/num!=p) {
			p=i*100/num;
			if (progress) {
				progress->current=p;
				progress->Update();
			}
		}
		ff.Read(Text,width);
		Text.Trim();
		if (Text.Len()>0) {
			Text.Transcode(charset,"UTF-8");
			//printf ("Tabelle: %i => >>%s<<\n",i,(char*)Text);
			v.Clear();
			v.SetValue(Text);
			v.Id=i+1;
			if (!s->Put(&v)) {
				return 0;
			}
		}
	}
	return 1;
}

int Cwm20Import::ImportShortcuts(const char *path)
{
	ppl6::CFile ff;
	if (!ff.Open("%s/abk.dat","rb",path)) {
		ppl6::SetError(10012,"%s/abk.dat",path);
		return 0;
	}
	CShortcutStore *s;
	DataShortcut v;
	s=(CShortcutStore*)Storage->FindStorageType("SHRT");
	if (!s) return 0;

	Abkuerzungen abk;
	ppl6::CString Abk,Artist;
	int width=sizeof(abk);
	int num=ff.Size()/width;
	WMLOG(ppl6::LOG::DEBUG,2) "Import: %i shortcuts from abk.dat...",num);
	if (progress) {
		progress->current=0;
		progress->Update();
	}
	int p=0;

	for (int i=0;i<num;i++) {
		if (i*100/num!=p) {
			p=i*100/num;
			if (progress) {
				progress->current=p;
				progress->Update();
			}
		}
		ff.Read(&abk,sizeof(abk));
		Abk.Strncpy(abk.abk,10);
		Artist.Strncpy(abk.interpret,80);
		Abk.Trim();
		Artist.Trim();
		Abk.Transcode(charset,"UTF-8");
		Abk.LCase();
		Artist.Transcode(charset,"UTF-8");
		//printf ("Abkuerzung: >>%s<< => >>%s<<\n",(char*)Abk,(char*)Artist);
		v.Clear();
		v.SetValue(Abk,Artist);
		if (!s->Put(&v)) {
			return 0;
		}
	}
	return 1;
}

int Cwm20Import::ImportTitle(const char *path)
{
	CTitleStore *s;
	DataTitle v;
	ppl6::CFile ff,bff;
	ppl6::CString Artist, Title, Remarks;
	TITELDATEI ti;
	s=(CTitleStore*)Storage->FindStorageType("TITL");
	if (!s) return 0;
	// Titeldatei
	if (!ff.Open("%s/titel20.dat","rb",path)) {
		ppl6::SetError(10012,"%s/titel20.dat",path);
		return 0;
	}
	// Bemerkungen
	if (!bff.Open("%s/bemerk.dat","rb",path)) {
		ppl6::SetError(10012,"%s/bemerk.dat",path);
		return 0;
	}
	if (progress) {
		progress->current=0;
		progress->Update();
	}
	ppluint32 p=0;

	ppluint32 num=ff.Size()/sizeof(TITELDATEI);
	for (ppluint32 i=0;i<num;i++) {
		if (i*100/num!=p) {
			p=i*100/num;
			if (progress) {
				progress->current=p;
				progress->Update();
			}
		}

		ff.Read(&ti,sizeof(ti));
		if (ti.Nummer==(i+1) && (ti.InfoByte&256)==256) {
			v.Clear();
			/* Texte konvertieren */
			Artist.Strncpy(ti.interpret,80);
			Artist.Trim();
			Artist.Transcode(charset,"UTF-8");
			Title.Strncpy(ti.titel,80);
			Title.Trim();
			Title.Transcode(charset,"UTF-8");
			v.SetTitle(Title);
			v.SetArtist(Artist);
			if (ti.Bemerkung) {		// Bemerkung einlesen
				bff.Seek((ti.Bemerkung-1)*256);
				bff.Read(Remarks,256);
				Remarks.Trim();
				Remarks.Transcode(charset,"UTF-8");
				v.SetRemarks(Remarks);
			}
			// Restliche Parameter kopieren
			v.TitleId=ti.Nummer;
			v.DeviceId=ti.Index;
			v.Length=(int)ti.Laenge;
			ti.Laenge-=(float)v.Length;
			v.Length=v.Length*60+(int)(ti.Laenge*100);
			v.VersionId=ti.version;
			v.LabelId=0;
			v.BPM=ti.bpm;
			v.RecordDate=ti.Aufnahmedatum;
			v.ReleaseDate=ti.Erscheinungsjahr*10000;
			v.RecordSourceId=ti.Quelle;
			v.Track=ti.track;
			v.Bitrate=0;
			v.GenreId=ti.musikart;
			v.RecordDeviceId=ti.Aufnahmegeraet;
			v.DeviceType=ti.traeger;
			v.Page=ti.Seite;
			v.Channels=1;
			if (ti.InfoByte&128) v.Channels=5;
			else if (ti.InfoByte&16) v.Channels=4;
			else if ((ti.InfoByte&12)==12) v.Channels=3;
			else if ((ti.InfoByte&12)==4) v.Channels=2;
			v.Quality=0;
			v.Rating=0;
			v.Flags=0;
			if (ti.InfoByte&1) v.Flags|=2;
			if (ti.InfoByte&32) v.Flags|=1;
			//printf ("%i/%i: %s - %s\n",v.TitleId,num,(char*)Artist,(char*)Title);
			if (!s->Put(&v)) {
				ppl6::PrintError();
				return 0;
			}
		}
	}
	/*
	CVersionStore *vs=(CVersionStore*)Storage->FindStorageType("VERS");
	DataVersion *vers;
	DataTitle *a=s->Get(50387);
	if (a) {
		vers=(DataVersion*)vs->Get(a->VersionId);
		if (!vers) {
			ppl6::PrintError();
		} else {
			printf ("Titel %i gefunden: %s - %s (%s): %0i:%02i Min\n",
				a->TitleId,(char*)a->Artist,(char*)a->Title,(char*)vers->Value,
				a->Length/60, a->Length%60);
		}
	} else{
		ppl6::PrintError();
	}
	*/
	return 1;
}


int Cwm20Import::ImportDevices(const char *path, const char *fileprefix, ppluint8 DeviceType)
{
	CDeviceStore *ds;
	DataDevice v;
	ds=(CDeviceStore*)Storage->FindStorageType("DEVI");
	if (!ds) return 0;
	ppl6::CFile ff;
	if (!ff.Open("%s/%s.dir","rb",path,fileprefix)) {
		ppl6::SetError(10012,"%s/%s.dir",path,fileprefix);
		return 0;
	}
	int num=ff.Size()/sizeof(TraegerVerzeichnis);

	// Träger Titel
	ppl6::CFile tff;
	if (!tff.Open("%s/traeger.tit","rb",path)) {
		ppl6::SetError(10012,"%s/traeger.tit",path);
		return 0;
	}
	ppl6::CString Title, SubTitle;

	if (progress) {
		progress->current=0;
		progress->Update();
	}
	int p=0;

	TraegerVerzeichnis tv;
	TraegerTitel tt;

	for (int i=0;i<num;i++) {
		if (i*100/num!=p) {
			p=i*100/num;
			if (progress) {
				progress->current=p;
				progress->Update();
			}
		}

		ff.Read(&tv,sizeof(tv));
		v.Clear();
		v.DeviceType=DeviceType;
		v.DeviceId=i+1;
		// Länge umrechnen
		v.Length=(int)tv.Laenge;
		tv.Laenge-=(float)v.Length;
		v.Length=v.Length*60+(int)(tv.Laenge*100);
		v.Recorded=0;		// Muss später berechnet werden
		v.PurchaseDate=tv.Kaufdatum;
		v.DateCreated=tv.Erstelldatum;
		v.FirstDate=tv.Aufnahmestart;
		v.LastDate=tv.Aufnahmeende;
		v.NumTracks=tv.Tracks;
		v.PurchaseId=tv.Kaufquelle;
		v.LabelId=tv.Label;
		v.Pages=tv.Seiten;
		v.PurchasePrice=tv.Preis;
		if (tv.Bezeichnung) {		// Träger Titel lesen
			tff.Seek((tv.Bezeichnung-1)*sizeof(tt));
			tff.Read(&tt,sizeof(tt));
			Title.Strncpy(tt.Titel1,80);
			Title.Trim();
			Title.Transcode(charset,"UTF-8");
			SubTitle.Strncpy(tt.Titel2,80);
			SubTitle.Trim();
			SubTitle.Transcode(charset,"UTF-8");
			v.SetTitle(Title);
			v.SetSubTitle(SubTitle);
		}
		if (!ImportTracks(path,fileprefix,&v,tv.ErsterDatensatz)) return 0;
		/*
		printf ("===========================================================\n");
		printf ("DeviceType: %u, DeviceId: %u, Pages: %u\n",v.DeviceType, v.DeviceId,v.Pages);
		printf ("Length: %i, PurchaseDate: %u, DateCreated: %u\n",v.Length, v.PurchaseDate, v.DateCreated);
		printf ("FirstDate: %u, LastDate: %u, NumTracks: %u\n",v.FirstDate, v.LastDate, v.NumTracks);
		printf ("PurchaseId: %u, LabelId: %u, PurchasePrice: %0.2f\n",v.PurchaseId, v.LabelId,v.PurchasePrice);
		*/
		if (!ds->Put(&v)) {
			ppl6::PrintError();
			return 0;
		}
	}
	return 1;
}

int Cwm20Import::ImportTracks(const char *path, const char *fileprefix, DataDevice *device, ppluint32 first)
{
	CTrackStore *ts;
	ts=(CTrackStore*)Storage->FindStorageType("TRAK");
	if (!ts) return 0;
	/*
	CTitleStore *ti;
	ti=(CTitleStore*)Storage->FindStorageType("TITL");
	if (!ti) return 0;
	DataTitle *title;
	*/
	ppl6::CFile ff;
	if (!ff.Open("%s/%s.dat","rb",path,fileprefix)) {
		ppl6::SetError(10012,"%s/%s.dat",path,fileprefix);
		return 0;
	}
	ppluint32 num=ff.Size()/sizeof(TraegerDatensatz);
	TraegerDatensatz td;
	DataTrack v;
	ppluint32 pos=first;
	ppluint32 count=0;
	//printf ("==== Importiere Datenträger %i: =============================\n",device->DeviceId);
	while (pos) {
		if (pos>num) {
			ppl6::SetError(20021);
			return 0;
		}
		if (!ff.Read(&td,sizeof(td),(pos-1)*sizeof(td))) {
			ppl6::SetError(20021);
			return 0;
		}
		v.Clear();
		v.Device=device->DeviceType;
		v.DeviceId=device->DeviceId;
		v.TitleId=td.Titelnummer;
		v.Page=td.Seite;
		v.Track=td.Track;

		/*
		title=ti->Get(v.TitleId);
		if (title) {
			printf ("Page %3u, Track %3u: %s - %s\n",v.Page, v.Track,
					title->Artist, title->Title);
		}
		*/

		if (!ts->Put(&v)) {
			ppl6::PrintError();
			return 0;
		}
		count++;
		pos=td.NaechsterDatensatz;
	}
	device->NumTracks=count;
	return 1;
}

} // EOF namespace wm40

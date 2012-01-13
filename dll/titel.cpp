//#define DEBUGLOG
//#define ENABLE_MONITOR

#include <windows.h>
#include <time.h>
#include <ppl6.h>
#include "winmusik.h"


static DWORD WINAPI IOThread(LPVOID param)
{
	CTitel *titel=(CTitel*)param;
	titel->mon_thread_is_running=true;
	while (!titel->mon_thread_stop) {
		Sleep(2000);
		titel->CloseTitelDatei();
	}
	titel->mon_thread_is_running=false;
	return 0;
}


CTitel::CTitel(char * filename)
{
	Cppl6Core *core=PPLInit();
	core->SetLogfile("c:/wm20.txt");
	core->SetDebugLevel(10);
	core->Printf("Test");

	//MessageBox(NULL,filename,"DEBUG",0);
	tf=NULL;
	nameTiteldatei=NULL;
	tii=NULL;
	mon_thread_ID=0;
	mon_thread=0;
	mon_thread_is_running=true;
	mon_thread_stop=false;

#ifdef ENABLE_MONITOR
	mon_thread=CreateThread(NULL,NULL,IOThread,this,NULL,&mon_thread_ID);
#endif

	tf=new CpplFile();
	if (tf==NULL) {
		//MessageBox(NULL,"Err 1","DEBUG",0);
		err=10001;
		return;
	}
	if (!tf->Open(filename,"r+b")) {
		PushError();
		//MessageBox(NULL,"Err 2","DEBUG",0);
		delete tf;
		tf=NULL;
		PopError();
		return;
	}
	tf->SetMapReadAhead(1024*1024*1);		// 1 MB Filecache

	//MessageBox(NULL,"Err 3","DEBUG",0);
	cache.SetStartBufferSize((ppldd)tf->Size());

	//MessageBox(NULL,"Err 4","DEBUG",0);
	nameTiteldatei=(char*)malloc(strlen(filename)+1);
	if (nameTiteldatei==NULL) {
		//MessageBox(NULL,"Err 4","DEBUG",0);
		delete tf;
		tf=NULL;
		err=2;
		return;
	}
	strcpy (nameTiteldatei,filename);
	//MessageBox(NULL,nameTiteldatei,"DEBUG",0);

	HighestID=(ppldd)tf->Size()/sizeof(TITELDATEI);
	MaxTitelIndex=HighestID+1000;
	tii=(TITELINDEX*)malloc(MaxTitelIndex*sizeof(TITELINDEX));
	if (tii) {
		// Alle Elemente auf NULL setzen
		ZeroMemory((void *)tii,MaxTitelIndex*sizeof(TITELINDEX));
	}

}

CTitel::~CTitel ()
{
#ifdef ENABLE_MONITOR
	mon_thread_stop=true;
	while (mon_thread_is_running) {
		Sleep(100);
	}
	Sleep(100);
#endif
	//MessageBox(NULL,"~CTitel","DEBUG",0);
	ClearCache();
	if (nameTiteldatei!=NULL) free (nameTiteldatei);
	if (tf!=NULL) delete tf;
	if (tii) free(tii);
	tii=NULL;
	tf=NULL;
}

void CTitel::ClearCache()
{
	if (tii) {
		ZeroMemory((void *)tii,MaxTitelIndex*sizeof(TITELINDEX));
	}
	cache.Clear();
	cachefragments=0;
	lostcachesize=0;
}

void CTitel::IncreaseCache()
{
	if (tii) {
		TITELINDEX *tmp=(TITELINDEX*)malloc((MaxTitelIndex+1000)*sizeof(TITELINDEX));
		if (!tmp) {		// Out of Memory
			ClearCache();
			free(tii);	// Cache komplett loeschen
			tii=NULL;
			return;
		}
		ZeroMemory((void *)tmp,(MaxTitelIndex+1000)*sizeof(TITELINDEX));
		CopyMemory((void *)tmp,(void *)tii,MaxTitelIndex*sizeof(TITELINDEX));
		MaxTitelIndex+=1000;
		tii=tmp;
	}
}

ppldd CTitel::CalcLength(TITELDATEI *Ti)
{
	// Die Länge des Datensatzes besteht auf jeden Fall aus der
	// Länge der Structure TITEL_CACHE
	ppldd bytes=sizeof(TITEL_CACHE);

	// Plus die Länge von Interpret + 0-Byte
	bytes+=StrLenRight(Ti->interpret,80)+1;

	// und Titel + 0-Byte
	bytes+=StrLenRight(Ti->titel,80)+1;
	return bytes;
}

ppldd CTitel::StrLenRight(char *str, ppldd max)
{
	ppldd pos=max-1;
	while (pos) {
		if(str[pos]!=32) return pos+1;
		pos--;
	}
	return 0;
}

int CTitel::ReadFromDisk(ppldd id, TITELDATEI *Ti)
{
	iosection.Lock();
	lastaccess=GetTime();
	if (!tf) {
		tf=new CpplFile();
	}
	if (!tf->IsOpen()) {
		if (!tf->Open(nameTiteldatei,"r+b")) {
			iosection.Unlock();
			return GetErrorCode();
		}
	}
	tf->Read((char*)Ti,sizeof(TITELDATEI),(id-1)*sizeof(TITELDATEI));
	PushError();
	#ifdef ENABLE_MONITOR
		if(!mon_thread_is_running) ResumeThread(mon_thread);
	#endif
	iosection.Unlock();
	PopError();
	return GetErrorCode();
}

int CTitel::WriteToDisk(ppldd id, TITELDATEI *Ti)
{
	iosection.Lock();
	lastaccess=GetTime();
	if (!tf) {
		tf=new CpplFile();
	}
	if (!tf->IsOpen()) {
		if (!tf->Open(nameTiteldatei,"r+b")) {
			iosection.Unlock();
			return GetErrorCode();
		}
	}
	tf->Write((char*)Ti,sizeof(TITELDATEI),(id-1)*sizeof(TITELDATEI));
	PushError();
	tf->Flush();
	#ifdef ENABLE_MONITOR
		if(!mon_thread_is_running) ResumeThread(mon_thread);
	#endif
	iosection.Unlock();
	PopError();
	return GetErrorCode();
}

void CTitel::CloseTitelDatei()
{
	iosection.Lock();
	// Falls der letzte Zugriff schon laenger als 2 Sekunden zurueckliegt,
	// schliessen wir das File
	if (lastaccess+2<GetTime() ) {
		if (tf->IsOpen()) tf->Close();
		#ifdef ENABLE_MONITOR
			if(mon_thread_is_running) SuspendThread(mon_thread);
		#endif
	}
	iosection.Unlock();
}

int CTitel::Get (ppldd id, TITELDATEIALIGN *Tia)
{
	TITELDATEI Ti;
	if (id==0) return 10003;			// Ungueltige TitelID
	if (id>HighestID) return 10004;		// Titel nicht vorhanden

	// Haben wir den Titel schon im Cache?
	
	if (tii) {
		if (tii[id-1].ti!=NULL) {
			CopyFromCache(Tia,tii[id-1].ti);
			return 0;
		}
	}
	
	// Titel einlesen
	#ifdef ENABLE_MONITOR
		if (err=ReadFromDisk(id,&Ti)>0) {
			return err;
		}
	#else
		tf->Read((char*)&Ti,sizeof(TITELDATEI),(id-1)*sizeof(TITELDATEI));
	#endif

	if (tii) {
		if (id<MaxTitelIndex) {
			ppldd size;
			size=CalcLength(&Ti);
			tii[id-1].ti=(TITEL_CACHE*)cache.Malloc(size);
			if (tii[id-1].ti) {
				tii[id-1].size=size;
				CopyToCache(tii[id-1].ti,&Ti);
			}
		}
		CopyFromCache(Tia,tii[id-1].ti);
		return 0;
	}
	return 1;
}

int CTitel::Put (ppldd id, TITELDATEIALIGN *Tia)
{
	if (id==0) {	// Neuer Titel
		HighestID=(ppldd)tf->Size()/sizeof(TITELDATEI)+1;
		id=HighestID;
		Tia->Nummer=id;
	}
	// Zuerst auf die Platte schreiben
	TITELDATEI Ti;
	Tia2Ti(&Ti,Tia);
	#ifdef ENABLE_MONITOR
		err=WriteToDisk(id,&Ti);
		if (err>0) {
			return err;
		}
	#else
		tf->Write((char*)&Ti,sizeof(TITELDATEI),(id-1)*sizeof(TITELDATEI));
		tf->Flush();
	#endif
	// Und dann den Cache aktualisieren
	Update(id);
	return 0;
}

void CTitel::Update(ppldd id)
{
	// Falls id>MaxTitelIndex ist, muessen wir den Cache vergroessern,
	// sofern es einen gab
	if (tii!=NULL && id>MaxTitelIndex) {
		IncreaseCache();
	}
	// Wenn es keinen Cache gibt, machen wir nichts
	if (!tii) return;

	// Wenn dieser Datensatz noch garnicht im Cache war, brauchen wir
	// ihn nur zu laden
	TITELDATEI Ti;
	TITELDATEIALIGN Tia;
	if (tii[id-1].ti==NULL) {
		Get(id,&Tia);
		return;
	}

	// Titel laden
	#ifdef ENABLE_MONITOR
		if (err=ReadFromDisk(id,&Ti)>0) {
			return;
		}
	#else
		tf->Read((char*)&Ti,sizeof(TITELDATEI),(id-1)*sizeof(TITELDATEI));
	#endif

	ppldd size=CalcLength(&Ti);

	// Falls size<=alter Groesse, ersetzen wir den vorhandenen Datensatz
	if (size<=tii[id-1].size) {
		CopyToCache(tii[id-1].ti,&Ti);
		return;
	}

	
	lostcachesize+=tii[id-1].size;
	cachefragments++;

	// Wenn wir zu viel verlorenen Speicher haben, bauen wir den Cache lieber
	// neu auf
	if (lostcachesize>(1024*1024*4)) {
		ClearCache();
	}

	tii[id-1].ti=(TITEL_CACHE*)cache.Malloc(size);
	tii[id-1].size=size;
	CopyToCache(tii[id-1].ti,&Ti);
}

void CTitel::CopyFromCache (TITELDATEIALIGN *dst, TITEL_CACHE *src)
{
	ppldd len;
	if (!dst) return;
	if (!src) return;
	
	dst->InfoByte=src->InfoByte;
	dst->version=src->version;
	dst->musikart=src->musikart;
	dst->Laenge=src->Laenge;
	dst->bpm=src->bpm;
	dst->Quelle=src->Quelle;
	dst->Erscheinungsjahr=src->Erscheinungsjahr;
	dst->Aufnahmedatum=src->Aufnahmedatum;
	dst->Bewertung[0]=src->Bewertung[0];
	dst->Bewertung[1]=src->Bewertung[1];
	dst->Qualitaet[0]=src->Qualitaet[0];
	dst->Qualitaet[1]=src->Qualitaet[1];
	dst->Bemerkung=src->Bemerkung;
	dst->Aufnahmegeraet=src->Aufnahmegeraet;
	dst->Soundfile=src->Soundfile;
	dst->traeger=src->traeger;
	dst->Index=src->Index;
	dst->Seite=src->Seite;
	dst->track=src->track;
	dst->Nummer=src->Nummer;

	// Interpret und Titel mit Spaces fuellen, damit VisualBasic damit klar kommt
	FillMemory(dst->interpret,80,32);
	FillMemory(dst->titel,80,32);
	len=strlen(src->interpret);
	strncpy(dst->interpret,src->interpret,len);
	
	len=strlen(src->titel);
	strncpy(dst->titel,src->titel,len);

}

void CTitel::CopyToCache(TITEL_CACHE *dst, TITELDATEI *src)
{
	ppldd len;
	if (!dst) return;
	if (!src) return;

	dst->InfoByte=src->InfoByte;
	dst->version=src->version;
	dst->musikart=src->musikart;
	dst->Laenge=src->Laenge;
	dst->bpm=src->bpm;
	dst->Quelle=src->Quelle;
	dst->Erscheinungsjahr=src->Erscheinungsjahr;
	dst->Aufnahmedatum=src->Aufnahmedatum;
	dst->Bewertung[0]=src->Bewertung[0];
	dst->Bewertung[1]=src->Bewertung[1];
	dst->Qualitaet[0]=src->Qualitaet[0];
	dst->Qualitaet[1]=src->Qualitaet[1];
	dst->Bemerkung=src->Bemerkung;
	dst->Aufnahmegeraet=src->Aufnahmegeraet;
	dst->Soundfile=src->Soundfile;
	dst->traeger=src->traeger;
	dst->Index=src->Index;
	dst->Seite=src->Seite;
	dst->track=src->track;
	dst->Nummer=src->Nummer;

	dst->interpret=(char*)dst + sizeof(TITEL_CACHE);

	len=StrLenRight(src->interpret,80);
	strncpy(dst->interpret,src->interpret,len);
	dst->interpret[len]=0;

	dst->titel=dst->interpret + len+1;
	len=StrLenRight(src->titel,80);
	strncpy(dst->titel,src->titel,len);
	dst->titel[len]=0;
}

void CTitel::Tia2Ti(TITELDATEI *dst, TITELDATEIALIGN *src)
{
	if (!dst) return;
	if (!src) return;

	dst->InfoByte=src->InfoByte;
	dst->version=src->version;
	dst->musikart=src->musikart;
	dst->Laenge=src->Laenge;
	dst->bpm=src->bpm;
	dst->Quelle=src->Quelle;
	dst->Erscheinungsjahr=src->Erscheinungsjahr;
	dst->Aufnahmedatum=src->Aufnahmedatum;
	dst->Bewertung[0]=src->Bewertung[0];
	dst->Bewertung[1]=src->Bewertung[1];
	dst->Qualitaet[0]=src->Qualitaet[0];
	dst->Qualitaet[1]=src->Qualitaet[1];
	dst->Bemerkung=src->Bemerkung;
	dst->Aufnahmegeraet=src->Aufnahmegeraet;
	dst->Soundfile=src->Soundfile;
	dst->traeger=src->traeger;
	dst->Index=src->Index;
	dst->Seite=src->Seite;
	dst->track=src->track;
	dst->Nummer=src->Nummer;

	strncpy(dst->interpret,src->interpret,80);
	strncpy(dst->titel,src->titel,80);
}
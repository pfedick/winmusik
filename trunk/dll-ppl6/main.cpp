//#define DEBUGLOG
#include <windows.h>
#include <shlobj.h>
#include <ppl6.h>
#include <ppl6-sound.h>
#include "winmusik.h"

static ppl6::CpplMutex Mutex;
static CTitel *Titel=NULL;
static CTagSaver *TagSaver=NULL;

#define MAJOR	1
#define MINOR	15
#define RELEASE 20090503

//#include "select.h"

/****************************************************************************
   FUNCTION: DllMain(HANDLE, DWORD, LPVOID)

   PURPOSE:  DllMain is called by Windows when
             the DLL is initialized, Thread Attached, and other times.
             Refer to SDK documentation, as to the different ways this
             may be called.
             
             The DllMain function should perform additional initialization
             tasks required by the DLL.  In this example, no initialization
             tasks are required.  DllMain should return a value of 1 if
             the initialization is successful.
           
*******************************************************************************/
BOOL APIENTRY DllMain(HANDLE hInst, DWORD ul_reason_being_called, LPVOID lpReserved)
{
	//Debug.SetLogfile("o:/debug.txt");
	switch (ul_reason_being_called) {
	case DLL_PROCESS_DETACH:
		Mutex.Lock();
		if (Titel!=NULL) {
			delete Titel;
			Titel=NULL;
		}
		if (TagSaver!=NULL) {
			delete TagSaver;
			TagSaver=NULL;
		}
		Mutex.Unlock();

		break;
	}

    return 1;
        UNREFERENCED_PARAMETER(hInst);
        UNREFERENCED_PARAMETER(ul_reason_being_called);
        UNREFERENCED_PARAMETER(lpReserved);
}


INT APIENTRY WMLibVersion (ppldd *major, ppldd *minor, ppldd *release) {
	*major=MAJOR;
	*minor=MINOR;
	*release=RELEASE;
	return 0;

}



INT APIENTRY GetMP3Info (char *filename, MP3HEADER *mp3, ID3TAG *id3, ID3v2TAG *v2) {
	CpplFile ff;
	int ret;
	if (!filename) return 61;		// Ungueltiger Dateiname
	int l=strlen(filename);
	if (l<3) return 61;
	char *tmpfile=(char *)malloc(l+1);
	if (!tmpfile) return 2;			// Nicht genug Speicher
	strcpy (tmpfile,filename);
	strxchg(tmpfile,"\\","/");	// Backslash in Slash wandeln

	if (ff.Open(tmpfile,"rb")) {
		ret=GetMP3Info_internal(&ff,mp3,id3,v2);
		ff.Close();
		free(tmpfile);
		return (ret);

	}
	free(tmpfile);
	return 159;
}

INT APIENTRY ChopID3Tags (char *filename) {
	return ChopID3Tags_internal(filename);
}

INT APIENTRY RepairMp3File (char *filename) {
	PPL_MPEG_HEADER mp3;
	CpplFile ff;
	if (!filename) return 61;
	if (!ff.Open(filename,"rb")) return GetErrorCode();
	if (!IdentMPEG(&ff,&mp3)) return 20;		// Unbekanntes Format

	if (mp3.start>0 || mp3.end<mp3.filesize) {		// Gibt es was zu reparieren?
		// Eine tempor�re Datei erstellen
		int l=strlen(filename);
		char * tmpfile=(char *)malloc(l+20);
		if (tmpfile) {
			strcpy(tmpfile,filename);
			strxchg(tmpfile,"\\","/");	// Backslash in Slash wandeln
			strcat(tmpfile,".repair");
			CpplFile rep;
			if (rep.Open(tmpfile,"wb")) {
				rep.Copy(&ff,mp3.start,mp3.size,0);
				rep.Close();
				ff.Close();
				// Altes File loeschen
				unlink(filename);
				// Neues File umbenennen
				rename(tmpfile,filename);
			}
			free(tmpfile);
		}
	}
	ff.Close();
	return 0;
}


INT APIENTRY SaveTagAsync (char *filename, char *interpret, char *titel, char *version, char *comment, char *genre, char *year)
{
	CpplString Interpret=interpret;
	CpplString Titel=titel;
	CpplString Version=version;
	CpplString Genre=genre;
	CpplString Year=year;
	CpplString Comment=comment;

	Interpret.Trim();
	Titel.Trim();
	Version.Trim();
	Genre.Trim();
	Year.Trim();
	Comment.Trim();
	ppl6::CpplAssocArray tag;
	tag.Set("interpret",Interpret);
	tag.Set("titel",Titel);
	tag.Set("version",Version);
	tag.Set("genre",Genre);
	tag.Set("year",Year);
	tag.Set("comment",Comment);
	Mutex.Lock();
	if (!TagSaver) {
		TagSaver=new CTagSaver;
	}
	Mutex.Unlock();

	//TagSaver->UpdateNow(filename,&tag);
	TagSaver->Add(filename,&tag,false);
	return 0;
}

INT APIENTRY OverwriteTagAsync (char *filename, char *interpret, char *titel, char *version, char *comment, char *genre, char *year)
{
	CpplString Interpret=interpret;
	CpplString Titel=titel;
	CpplString Version=version;
	CpplString Genre=genre;
	CpplString Year=year;
	CpplString Comment=comment;

	Interpret.Trim();
	Titel.Trim();
	Version.Trim();
	Genre.Trim();
	Year.Trim();
	Comment.Trim();
	ppl6::CpplAssocArray tag;
	tag.Set("interpret",Interpret);
	tag.Set("titel",Titel);
	tag.Set("version",Version);
	tag.Set("genre",Genre);
	tag.Set("year",Year);
	tag.Set("comment",Comment);
	Mutex.Lock();
	if (!TagSaver) {
		TagSaver=new CTagSaver;
	}
	Mutex.Unlock();

	//TagSaver->UpdateNow(filename,&tag);
	TagSaver->Add(filename,&tag,true);
	return 0;
}




INT APIENTRY UpdateID3Tags (char *filename, char *interpret, char *titel, char *version, char *comment, char *genre, char *year)
{
	CpplString Interpret=interpret;
	CpplString Titel=titel;
	CpplString Version=version;
	CpplString Genre=genre;
	CpplString Year=year;
	CpplString Comment=comment;

	Interpret.Trim();
	Titel.Trim();
	Version.Trim();
	Genre.Trim();
	Year.Trim();
	Comment.Trim();
	ppl6::CpplAssocArray tag;
	tag.Set("interpret",Interpret);
	tag.Set("titel",Titel);
	tag.Set("version",Version);
	tag.Set("genre",Genre);
	tag.Set("year",Year);
	tag.Set("comment",Comment);
	Mutex.Lock();
	if (!TagSaver) {
		TagSaver=new CTagSaver;
	}
	Mutex.Unlock();
	TagSaver->Add(filename,&tag);
	//TagSaver->UpdateNow(filename,&tag);
	return 0;
}


INT APIENTRY InitTitle (char *titeldatei) {
	//ISDEBUG Debug.SetLogfile("O:/debug.txt");

	if (titeldatei==NULL) return 95;
	if (Titel!=NULL) {
		delete Titel;
		Titel=NULL;
	}

	Titel=new CTitel(titeldatei);

	if (Titel==NULL) {
		return 10000;
	}
	return Titel->err;
}

INT APIENTRY GetWMTitle (ppldd id, TITELDATEIALIGN *Ti)
{
	if (Titel==NULL) return 10002;	// Nicht initialisiert
	
	//MessageBox(NULL,"GetTitel","Debug",0);
	//return 1;
	//PPLDebug.HexDump(Ti,sizeof(TITELDATEI));
	//return 1;

	return Titel->Get(id,Ti);
}

INT APIENTRY PutTitle (ppldd id, TITELDATEIALIGN *Ti)
{
	if (!Titel) return 10002;
	return Titel->Put(id,Ti);
}

INT APIENTRY UpdateTitle (ppldd id)
{
	if (Titel) Titel->Update(id);
	return 0;
}

INT APIENTRY CloseTitle ()
{
	delete Titel;
	Titel=NULL;
	return 0;
}


INT APIENTRY CheckStruct (TITELDATEI *Ti) {
	//PPLDebug.HexDump((char*)Ti,sizeof(TITELDATEI));
	return 0;
}

INT APIENTRY GetAppPath(char *buffer, long size)
{
	if (!buffer) return 0;
	if (size<2) return 0;
	//char path[MAX_PATH];
	//HRESULT res;
	/*
	//res=SHGetSpecialFolderPath(NULL,path, CSIDL_COMMON_PROGRAMS  , false);
	//res=SHGetSpecialFolderPath(NULL,path, CSIDL_APPDATA , false);
	res=SHGetFolderPath(NULL,CSIDL_APPDATA,NULL,SHGFP_TYPE_CURRENT,path);
	if (res!=S_OK) {
	//if (res!=NOERROR) {
		return 0;
	}
	strncpy (buffer,path,size);
	

	return 1;
	*/
	return 0;
}
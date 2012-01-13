#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif
#include <ppl6.h>
#include <ppl6-sound.h>

#include "winmusik.h"
#include "mp3.h"

int frame_length(MP3HEADER *header) {
    return  (frame_size_index[3-header->layer]*((header->version&1)+1)*
            header->bitrate/header->frequency)+
            header->padding;
}


int GetMP3Info_internal (CpplFile *file, MP3HEADER *mp3, ID3TAG *id3, ID3v2TAG *v2) {
	PPL_MPEG_HEADER pmp3;
	char *buffer;

	/*
	CpplLog Log;
	Log.SetLogfile(ppl6::LOG::DEBUG,"p:/wm20.log");
	Log.SetLogLevel(ppl6::LOG::DEBUG,10);
	Log.Printf(ppl6::LOG::DEBUG,1,"Start");
	*/

	//Debug.SetLogfile("p:/debug.log");
	if (mp3==NULL || file==NULL) return 37;
	bzero(mp3,sizeof(MP3HEADER));
	if (!IdentMPEG(file,&pmp3)) return 20;	// Kein MPEG-File
	//Log.HexDump(ppl6::LOG::DEBUG,2,&pmp3,sizeof(pmp3));
	
	mp3->status|=1;		// MP3-File erkannt

	// ID3-Tag einlesen falls vorhanden
	buffer=file->Map(file->Lof()-128,128);
	if (buffer[0]=='T' && buffer[1]=='A' && buffer[2]=='G') {
		mp3->status|=2;		// ID3v1-Tag vorhanden
		if (id3!=NULL) {
			memcpy (id3,buffer,128);
		}
	}
	mp3->datasize=pmp3.size;
	mp3->filelength=pmp3.filesize;
	mp3->datastart=pmp3.start;
	mp3->dataend=pmp3.end;

	mp3->version=pmp3.version;
	mp3->layer=pmp3.layer;
	mp3->protection=pmp3.error_protection;
	mp3->bitrateindex=pmp3.bitrate_index;
	mp3->frequencyindex=pmp3.frequency_index;
	mp3->padding=pmp3.padding;
	mp3->privat=pmp3.extension;
	mp3->channelmode=pmp3.mode;
	mp3->modeextension=pmp3.mode_ext;
	mp3->copyright=pmp3.copyright;
	mp3->original=pmp3.original;
	mp3->emphasis=pmp3.emphasis;
	mp3->bitrate=pmp3.bitrate;
	mp3->frequency=pmp3.frequency;
	mp3->length=pmp3.length;
	mp3->mslength=pmp3.mslength;

	CID3Tag Tag;
	CString tmp,Title,Version;
	if (Tag.Load(file)) {
		mp3->status|=4;
		Title=Tag.GetTitle();
		Title.Trim();
		tmp=Tag.GetArtist();
		tmp.Trim();
		strncpy(v2->Artist,tmp,255);
		tmp=Tag.GetAlbum();
		tmp.Trim();
		strncpy(v2->Album,tmp,255);
		tmp=Tag.GetYear();
		tmp.Trim();
		strncpy(v2->Year,tmp,4);
		tmp=Tag.GetComment();
		tmp.Trim();
		strncpy(v2->Comment,tmp,255);
		tmp=Tag.GetGenre();
		tmp.Trim();
		strncpy(v2->Genre,tmp,255);
		tmp=Tag.GetRemixer();
		tmp.Trim();
		// Falls wir keinen Remixer haben, schauen wir im Titel nach
		if (!tmp.Len()) {
			if (Title.PregMatch("/^(.*)\\((.*)\\)$/")) {
				tmp=Title.GetMatch(2);
				tmp.Trim();
				Title=Title.GetMatch(1);
				//tmp="Remix";
			}
		}
		tmp.Replace("_"," ");
		tmp.PregReplace("/ rmx$/i"," Remix");
		tmp.Trim();
		Version=tmp;
		strncpy(v2->Version,tmp,255);

		// Falls die Version auch im Titel vorhanden ist, entfernen wir sie
		Title.Replace(Version,"");
		Title.Replace("()","");
		Title.Trim();

		strncpy(v2->SongName,Title,255);


	} else {
		strncpy(v2->SongName,"",255);
		strncpy(v2->Artist,"",255);
		strncpy(v2->Album,"",255);
		strncpy(v2->Year,"",4);
		strncpy(v2->Comment,"",255);
		strncpy(v2->Genre,"",255);
		strncpy(v2->Version,"",255);

		// Vielleicht können wir mit dem Dateinamen was anfangen
		CString Filename=ppl6::getfile((char*)file->GetFilename());
		Filename.Replace("_"," ");
		Filename.PregReplace("/.mp3$/i","");

		if (Filename.PregMatch("/^[0-9]{3}-/")) {
			Filename.Shl(0,4);
			if (Filename.PregMatch("/^(.*)-(.*)$/u")) {
				mp3->status|=8;
				tmp=Filename.GetMatch(1);
				tmp.Trim();
				strncpy(v2->Artist,tmp,255);
				Title=Filename.GetMatch(2);
				// Hier könnte die Version drinstehen
				if (Title.PregMatch("/^(.*)\\((.*)\\)")) {
					tmp=Title.GetMatch(1);
					tmp.Trim();
					strncpy(v2->SongName,tmp,255);
					tmp=Title.GetMatch(2);
					tmp.Trim();
					strncpy(v2->Version,tmp,255);
				} else {
					tmp=Filename.GetMatch(2);
					tmp.Trim();
					strncpy(v2->SongName,tmp,255);
				}
			}
		}
	}
	return 0;
}

int ChopID3Tags_internal (char *filename)
{
	/*
	char *tmpfilename;
	ppldd p,bytes,end=0;
	ppldds start=0;
	p=0;
	CpplFile file,tmp;
	if (!file.Open(filename,"rb")) return file.err;

	start=FindMp3Start(&file);
	if (start<0) return 20;			// Unbekanntes Format, kein header gefunden

	end=FindMp3End(&file);

	//printf ("start: %u, end: %u\n",start,end);

	if (start>0 || end<file.Lof()) {			// Muss ein Tag entfernt werden?
		//tmpfilename=mktemp("wm2cpy.XXXXXXXXXX");
		//printf ("Quelldatei: %u Bytes, Zieldatei: %u Bytes \n",file.Lof(),end-start+1);

		tmpfilename=tempnam(NULL,"wm2cpy.");
		//printf ("tmpfilename=%s\n",tmpfilename);
		if (tmp.Open(tmpfilename,"wb")) {
			bytes=tmp.Copy(&file,start,end-start+1,0);				// TODO: Funktioniert nicht!!!
			if (bytes==end-start+1) {
				file.Close();
				file.Open(filename,"wb");
				tmp.Seek(0);
				file.Copy(&tmp,bytes);								// oder das?
				file.Close();
				tmp.Close();
				remove(tmpfilename);
				return 0;
			}
			tmp.Close();
			remove(tmpfilename);
			return 13;
		}
	}
	*/
	return 0;
}

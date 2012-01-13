#include <stdio.h>
#include <strings.h>
#include <ppl4.h>
#include "winmusik.h"

/*
TraegerBez(1) = "kassette"
TraegerBez(2) = "cd"
TraegerBez(3) = "daten"
TraegerBez(4) = "platte"
TraegerBez(5) = "video"
TraegerBez(6) = "sonstige"
TraegerBez(7) = "mpeg"
TraegerBez(8) = "band"
*/

static char * TraegerBez[]={
	"unknown",
	"Kassette",
	"CD",
	"Daten",
	"Platte",
	"Video",
	"Sonstiger",
	"MPEG",
	"Tonband",
	NULL
};

void FillWMTHeader (WMTHEADER *h, char *buffer) {
	h->Tracks=(dw)peekw(buffer+0);
	h->TrackLength=(dw)peekw(buffer+2);
	h->TraegerTitel=peekd(buffer+4);
	h->TraegerTyp=(dw)peekw(buffer+8);
	h->TragerIndex=peekd(buffer+10);
	h->TabStart=peekd(buffer+14);
}

void ReadTrack(WMTENTRY *t, char *buffer) {
	strncpy(t->ti.interpret,buffer+2,80);
	t->ti.interpret[79]=0;
	strncpy(t->ti.titel,buffer+82,80);
	t->ti.titel[79]=0;

	strncpy(t->version,buffer+214,80);
	t->version[79]=0;

	strncpy(t->musikart,buffer+294,80);
	t->musikart[79]=0;

	t->ti.track=(dw)peekw(buffer+208);
}

/*
Function ereg_replace(such As String, ersetze As String, text As String) As String
Dim fertig As Boolean
Dim i As Integer

fertig = False

While fertig = False
    fertig = True
    i = InStr(text, such)
    'Stop
    If (i > 0) Then
        fertig = False
        text = Mid(text, 1, i - 1) + ersetze + Mid(text, i + 1)
    End If
Wend
ereg_replace = text

End Function
*/



/*
Function MakeMp3Filename(interpret As String, titel As String, version As String, space As String)
a$ = Trim(interpret) + "-" + Trim(titel) + " (" + Trim(version) + ").mp3"
If (space <> " ") Then
    a$ = ereg_replace(" ", space, a$):        ' Spaces entfernen
End If
a$ = ereg_replace("/", space, a$):        ' Slashes entfernen
a$ = ereg_replace("\", space, a$):        ' Backslashes entfernen
a$ = ereg_replace("&", "+", a$):        ' Ampersond ersetzen
a$ = ereg_replace(Chr(34), "''", a$):        ' Ampersond ersetzen
MakeMp3Filename = a$
End Function

*/

void MakeMp3Filename (char *ziel, char *quelle, char *space)
{
	strxchg (quelle," ",space);
	strxchg (quelle,"/",space);
	strxchg (quelle,"\\",space);
	strxchg (quelle,"&","+");
	strxchg (quelle,"\"","'");
	strcpy (ziel,quelle);

}

int main (int argc, char **argv)
{
	char	* wmtfile;
	char	* filepath;
	char	* buffer;
	char	defaultwmtfile[255];
	int		id;
	dd		p;

	PFPSTRUCT	pfp;
	WMTHEADER	wmthead;
	WMTENTRY	track;
	char		newname[255], tracknum[10], trackname[256];
	char		tmpc[256];

	CDirFile dir;

	File wmtff;

	//wmtfile="d:/mpeg/063/000index.wmt";
	//filepath="d:/mpeg/063";

	filepath=getargv(argc,argv,"-p");
	if (filepath==NULL) filepath="./";

	wmtfile=getargv(argc,argv,"-w");
	if (wmtfile==NULL) {
		sprintf(defaultwmtfile,"%s/000index.wmt",filepath);
		wmtfile=defaultwmtfile;
	}


	wmtff.Open(wmtfile,"rb");
	if (wmtff.err==0) {
		buffer=wmtff.Map(0,8192);
		id=ident(buffer,&pfp);
		if (id==34) {				// WinMusik WMT-File 2.0
			printf ("Processing %s...\n",wmtfile);
			p=pfp.header_length;
			FillWMTHeader(&wmthead,buffer+p);
			//memcpy((void *)&wmthead,buffer+pfp.header_length,sizeof(WMTHEADER));
			printf ("Tontraeger %s %u, %u Tracks\n",
				TraegerBez[wmthead.TraegerTyp],wmthead.TragerIndex,wmthead.Tracks);
			printf ("%s\n",buffer+p+wmthead.TraegerTitel);

			p=pfp.header_length+wmthead.TabStart;
			//wmthead.Tracks=2;
			for (int i=0;i<wmthead.Tracks;i++) {
				buffer=wmtff.Map(p+i*wmthead.TrackLength,wmthead.TrackLength);
				ReadTrack(&track,buffer);
				sprintf (tracknum,"%3u",i+1);
				strxchg (tracknum," ","0");

				sprintf (newname,"%s-%s-%s (%s).mp3",
					tracknum,
					trim(track.ti.interpret),
					trim(track.ti.titel),
					trim(track.version));
				
				MakeMp3Filename(trackname,newname," ");
				/*
				 * Track suchen
				 */
				sprintf (tmpc,"%s/%s*.mp3",filepath,tracknum);
				if ( dir.FindFirst(tmpc) ) {
					printf ("%s ==> ",dir.Filename);
					printf ("%s\n",trackname);
					sprintf(tmpc,"%s/%s",filepath,dir.Filename);
					sprintf(newname,"%s/%s",filepath,trackname);
					rename (tmpc,newname);
				} else {
					sprintf (tmpc,"%s/%s*.MP3",filepath,tracknum);
					if ( dir.FindFirst(tmpc) ) {
						printf ("%s ==> ",dir.Filename);
						printf ("%s\n",trackname);
						sprintf(tmpc,"%s/%s",filepath,dir.Filename);
						sprintf(newname,"%s/%s",filepath,trackname);
						rename (tmpc,newname);
					}
				}
			}
		} else {
			printf ("%s is not a valid WinMusik WMT-File!\n",wmtfile);
		}
	} else {
		printf ("Could not open %s!\n",wmtfile);
	}
	wmtff.Close();
	//int key=getchar();
	return 0;
}

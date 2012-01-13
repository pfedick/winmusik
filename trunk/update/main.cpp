#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ppl6.h>
#include "winmusik.h"

#define OLDFILE	"e:/wmusik32/daten/titel20.dat"
#define NEWFILE	"e:/wmusik32/daten/titel21.dat"

void Translate(TITELDATEI20 *t20, TITELDATEI21 *t21);
void Init_CRC32_Table();

ppluint32 crc32_table[256];  // Lookup table array 

int main(int argc, char **argv)
{
	/*
	Init_CRC32_Table();
	int c=0;
	for (int i=0;i<256;i++) {
		printf ("0x%08x,",crc32_table[i]);
		c++;
		if (c>5) {
			printf ("\n");
			c=0;
		}
	}
	printf ("\n");
	return 0;
	*/
	CpplFile in,out;
	if (!in.Open(OLDFILE,"rb")) {
		printf ("Alte Datei kann nicht geoeffnet werden");
		return in.GetErrorCode();
	}
	unlink(NEWFILE);
	if (!out.Open(NEWFILE,"wb")) {
		printf ("Alte Datei kann nicht geoeffnet werden");
		return in.GetErrorCode();
	}
	ppluint32 tracks=(ppluint32)in.Size()/sizeof(TITELDATEI20);
	printf ("Tracks: %u\n",tracks);
	TITELDATEI20	t20;
	TITELDATEI21	t21;
	ppluint32		errorcounter=0;
	for (ppluint32 i=0;i<tracks;i++) {
		in.Read(&t20,sizeof(t20));
		bzero(&t21,sizeof(t21));

		if (t20.InfoByte&1 && t20.Nummer==(i+1)) {
			/*
			printf ("Titel %u: %u-%u-%u-%u: %si - %s\n",t20.Nummer,
				t20.traeger, t20.Index, t20.Seite, t20.track,
				t20.interpret,t20.titel
			);
			*/
			Translate(&t20,&t21);


		} else {
			errorcounter++;
			printf ("ERROR: %u, %u: %u-%u-%u-%u\n",t20.Nummer,
				t20.InfoByte,
				t20.traeger, t20.Index, t20.Seite, t20.track
			);
			t21.Id=i+1;
		}
		unsigned char *buffer=(unsigned char*)&t21;
		t21.crc=crc32(buffer+4,sizeof(t21)-4);
		out.Write(&t21,sizeof(t21));
	}
	printf ("Anzahl fehlerhafter Datensaetze: %u\n",errorcounter);

	return 0;
}

void Translate(TITELDATEI20 *t20, TITELDATEI21 *t21)
{
	char *tmp;
	char buffer[81];
	t21->Id=t20->Nummer;
	t21->Status=t20->InfoByte;
	t21->References=0;
	if (t21->Status&256) t21->References=1;

	// Titel und Interpret werden nun mit 0-Bytes statt Spaces aufgefüllt
	strncpy(buffer,t20->interpret,80);
	buffer[80]=0;
	tmp=trim(buffer);
	strcpy(t21->Interpret,tmp);

	strncpy(buffer,t20->titel,80);
	buffer[80]=0;
	tmp=trim(buffer);
	strcpy(t21->Titel,tmp);

	t21->Version=t20->version;
	t21->Musikart=t20->musikart;
	t21->Laenge=t20->Laenge;
	t21->Bpm=t20->bpm;
	t21->Quelle=t20->Quelle;
	t21->Erscheinungsjahr=t20->Erscheinungsjahr;
	t21->Aufnahmedatum=t20->Aufnahmedatum;
	t21->Bewertung=0;
	t21->Qualitaet=0;
	t21->Bemerkung=t20->Bemerkung;
	t21->Aufnahmegeraet=t20->Aufnahmegeraet;
	t21->TraegerArt=t20->traeger;
	t21->Index=t20->Index;
	t21->Seite=t20->Seite;
	t21->Track=t20->track;
	t21->Date_Created=time(NULL);
	t21->Date_Changed=t21->Date_Created;
	
}


ppluint32 Reflect(ppluint32 ref, char ch)
{// Used only by Init_CRC32_Table().

      ppluint32 value(0);

      // Swap bit 0 for bit 7
      // bit 1 for bit 6, etc.
      for(int i = 1; i < (ch + 1); i++)
      {
            if(ref & 1)
                  value |= 1 << (ch - i);
            ref >>= 1;
      }
      return value;
}

void Init_CRC32_Table()
{// Call this function only once to initialize the CRC table.

      // This is the official polynomial used by CRC-32
      // in PKZip, WinZip and Ethernet.
      ppluint32 ulPolynomial = 0x04c11db7;

      // 256 values representing ASCII character codes.
      for(int i = 0; i <= 0xFF; i++)
      {
            crc32_table[i]=Reflect(i, 8) << 24;
            for (int j = 0; j < 8; j++)
                  crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? ulPolynomial : 0);
            crc32_table[i] = Reflect(crc32_table[i], 32);
      }
}


Attribute VB_Name = "UPDATE1"
DefInt A-Z

Type Titeldatei2
  InfoByte As Integer
  interpret As String * 80
  titel As String * 80
  version As Long
  musikart As Long
  Laenge As Single
  bpm As Integer
  Quelle As Long
  erscheinungsjahr As Integer
  aufnahmedatum As Long
  bewertung As String * 2
  Qualitaet As String * 2
  Bemerkung As Long
  Aufnahmegeraet As Long
  Soundfile As Long
  Traeger As Byte
  Index As Long
  Seite As Byte
  Track As Integer
  Nummer As Long
End Type

Type CDHeaderKennung2
  TraegerArt As Byte
  Index As Long
  Seite As Byte
  Tracks As Integer
  Gesamtlaenge As Long
  Startposition As Long
  StartPos(10) As Long
End Type


Type TraegerVerzeichnis2
  InfoByte As Integer
  Seiten As Byte
  Laenge As Single
  Rest As Single
  Bezeichnung As Long
  Label As Long
  Kaufquelle As Long
  Kaufdatum As Long
  Preis As Single
  Erstelldatum As Long
  Aufnahmestart As Long
  Aufnahmeende As Long
  Tracks As Integer
  Singles As Integer
  Mixe As Integer
  Cover As Long
  CoverBild As Long
  ErsterDatensatz As Long
End Type

Type TraegerDatensatz2
  LetzterDatensatz As Long
  NaechsterDatensatz As Long
  TraegerIndex As Long
  Titelnummer As Long
  Seite As Byte
  Track As Integer
End Type

Type TraegerTitel
  Titel1 As String * 80
  Titel2 As String * 80
End Type
'=========================================================

Type TrägerVerzeichnis1
  chk As String * 1
  Tonträgerart As Integer
  Länge As Single
  Rest As Single
  Seiten As String * 1
  Tracks As Integer
  Singles As Integer
  Mixe As Integer
  Status_Alphabet As String * 1
  Status_Sicherung As String * 1
  Status_Position As Integer: '==> Ort, and dem Tonträger ist
  Sicherungsort As Integer: '==> Nummer der Sicherungsdiskette
  Titelnummer As Long
  Preis As Single
  Erstelldatum As Long
  Aufnahmestart As Long
  Aufnahmeende As Long
  Druck As String * 1: ' 45 Byte
End Type

Type Titeldatei1
   chk As String * 1
   Status_Sicherung As String * 1
   interpret As String * 80
   titel As String * 80
   version As Long
   musikart As Integer
   Länge As Single
   Stereo As String * 1
   Dolby As String * 1
   EchterTitel As String * 1
   Bemerkung As Long
   bewertung As String * 2
   qualität As String * 2
   erscheinungsjahr As Integer
   aufnahmedatum As Long
   Quelle As Long
   aufnahmegerät As Integer
   Soundfile As Long
   bpm As Integer
   Bildfile As Integer
   Träger As String * 1
   Index As Long
   Seite As String * 1
   Track As Integer
   alpha As String * 1
   Nummer As Long
End Type

Type Träger_Tracks1
   chk As String * 1
   Seite As String * 1
   Track As Integer
   Titelnummer As Long
   Status_Alphabet As String * 1
End Type

Type Abkürzungen
   abk As String * 10
   interpret As String * 80
End Type

Type CDHeaderKennung1
  Art As String * 1
  Index As Long
  Seite As Integer
  Tracks As Integer
  Gesamtlänge As Long
  Startposition As Long
  StartPos(10) As Long
End Type


Global TrV1 As TrägerVerzeichnis1
Global Ti1 As Titeldatei1
Global TrTr1 As Träger_Tracks1
Global abk As Abkürzungen

Public Ti2 As Titeldatei2
Public TrDat2 As TraegerDatensatz2
Public TrTi As TraegerTitel
Public TrV2 As TraegerVerzeichnis2

Public DatenPfad As String
Public wm1pfad As String
Public wm2pfad As String
Public TiDatei1 As String
Public TiDatei2 As String

Public CDHeaderDatei As String
Public CDHead1 As CDHeaderKennung1
Public CDHead2 As CDHeaderKennung2

Public tmp As String



Sub ConvertTi(Ti1 As Titeldatei1, Ti2 As Titeldatei2)
Ti2.InfoByte = 1
If LCase(Ti1.Stereo) = "j" Then Ti2.InfoByte = Ti2.InfoByte Or 4
If LCase(Ti1.Dolby) = "j" Then Ti2.InfoByte = Ti2.InfoByte Or 8
If InStr(Ti1.titel, "n.g.") = 0 And Ti1.version <> 40 Then
    Ti2.InfoByte = Ti2.InfoByte Or 32
End If
If LCase(Ti1.EchterTitel) = "j" Then Ti2.InfoByte = Ti2.InfoByte Or 64
Ti2.interpret = Trim$(Ti1.interpret)
Ti2.titel = Trim$(Ti1.titel)
Ti2.version = CLng(Ti1.version)
Ti2.musikart = CLng(Ti1.musikart)
Ti2.Laenge = Ti1.Länge
Ti2.bpm = Ti1.bpm
Ti2.Quelle = Ti1.Quelle
Ti2.erscheinungsjahr = Ti1.erscheinungsjahr
Ti2.aufnahmedatum = Ti1.aufnahmedatum
Ti2.bewertung = Ti1.bewertung
Ti2.Qualitaet = Ti1.qualität
Ti2.Bemerkung = 0
Ti2.Aufnahmegeraet = CLng(Ti1.aufnahmegerät)
Ti2.Soundfile = Ti1.Soundfile
Ti2.Traeger = CByte(Ti1.Träger)
Ti2.Index = Ti1.Index
Ti2.Seite = CByte(Ti1.Seite)
Ti2.Track = Ti1.Track
Ti2.Nummer = 0
End Sub

Sub ConvertTrV(TrV1 As TrägerVerzeichnis1, TrV2 As TraegerVerzeichnis2)
TrV2.InfoByte = 1
TrV2.Kaufquelle = 0
TrV2.Label = CLng(TrV1.Tonträgerart)
TrV2.Laenge = TrV1.Länge
TrV2.Rest = TrV1.Rest
TrV2.Seiten = CByte(TrV1.Seiten)
TrV2.Bezeichnung = 0
TrV2.Kaufdatum = TrV1.Erstelldatum
TrV2.Erstelldatum = TrV1.Erstelldatum
TrV2.Aufnahmestart = TrV1.Aufnahmestart
TrV2.Aufnahmeende = TrV1.Aufnahmeende
TrV2.Preis = TrV1.Preis
TrV2.Tracks = TrV1.Tracks
TrV2.Singles = TrV1.Singles
TrV2.Mixe = TrV1.Mixe
TrV2.Cover = 0
TrV2.CoverBild = 0
TrV2.ErsterDatensatz = 0
End Sub

Sub CopyBez(Quelle$, ziel$)
Dim bez40 As String * 40
Dim bez80 As String * 80
On Error Resume Next
f1 = FreeFile
Open Quelle$ For Random As #f1 Len = 40
f2 = FreeFile
Kill ziel$
Open ziel$ For Random As #f2 Len = 80
For ii& = 1 To LOF(f1) / 40
  Get #f1, ii&, bez40: bez80 = Trim$(bez40)
  Put #f2, ii&, bez80
Next
Close f2
Close f1
End Sub

Sub Melde(text As String)
Form1.meldung.Caption = text
DoEvents
End Sub

Sub ShowQueue(prznt As Single)
If Fix(prznt) <> Form1.Queue.FloodPercent Then
  Form1.Queue.FloodPercent = prznt
  DoEvents
End If
End Sub

Sub StartUpdate()

ReDim TrägerFile(6) As Integer
ReDim ext(6) As String
ReDim TrägerDir(6) As String
ReDim trägerdat(6) As String
ReDim trägerpfad1(6) As String
ReDim TrägerStart(6) As Long
ReDim TrägerPos(6) As Long
ReDim TrägerName(6) As String

Dim pz As Single
Dim st As Single
Dim ll As Long

Dim cd1 As Integer
Dim cd2 As Integer

Dim Traeger As Integer
Dim TiF1 As Integer
Dim TiF2 As Integer
Dim tbl As Integer
Dim TrF1(6) As Integer
Dim TrF2(6) As Integer
Dim TraegerIndex(6) As Long
Dim TraegerDatum(6) As Long
Dim Found As Boolean
Dim Lowdat As Long
Dim LowTraeger As Integer
Dim Fill As Long
Dim qq As Integer
Dim zz As Integer
Dim LetzterSatz As Long
Dim Satz As Long
Dim chk As Long
Dim bem80 As String * 80
Dim bem256 As String * 256
Dim GesamtTräger As Long
Dim erledigt As Long

On Error Resume Next

Form1.Frame1.Visible = False
Form1.meldung.Visible = True
DoEvents

Melde "Initialisiere..."

TrägerDir(1) = "kassette.dir"
TrägerDir(2) = "cd.dir"
TrägerDir(3) = "daten.dir"
TrägerDir(4) = "platte.dir"
TrägerDir(5) = "video.dir"
TrägerDir(6) = "sonstige.dir"

trägerdat(1) = "Kassette.Dat"
trägerdat(2) = "CD.Dat"
trägerdat(6) = "Daten.Dat"
trägerdat(3) = "Platte.Dat"
trägerdat(4) = "Video.Dat"
trägerdat(5) = "Sonstige.Dat"

trägerpfad1(1) = wm1pfad + "\kas\"
trägerpfad1(2) = wm1pfad + "\cd\"
trägerpfad1(4) = wm1pfad + "\pla\"
trägerpfad1(5) = wm1pfad + "\vid\"
trägerpfad1(6) = wm1pfad + "\son\"

ext(1) = ".kas"
ext(2) = ".cd"
ext(6) = ".dcd"
ext(4) = ".pla"
ext(5) = ".vid"
ext(6) = ".son"

TrägerName(1) = "Kassette"
TrägerName(2) = "CD"
TrägerName(3) = "Daten-CD"
TrägerName(4) = "Platte"
TrägerName(5) = "Video"
TrägerName(6) = "Sonstige"

ShowQueue 0
DoEvents

openini wm1pfad + "\winmusik.ini"
If inierror() > 0 Then
  MsgBox "WinMusik 1.0 wurde nicht gefunden!", 48, "WinMusik Update"
  Form1.Frame1.Visible = True
  Form1.meldung.Visible = False
  Exit Sub
End If
selectTopic "Pfade"
wm1pfad = GetINI$("Main")
TiDatei1 = GetINI$("Titeldatei")
closeini

openini "wmusik20.ini"
If inierror() > 0 Then
  MsgBox "WinMusik 2.0 ist noch nicht installiert!" + Chr$(13) + "Bitte installieren Sie zuerst WinMusik 2.0!", 48, "WinMusik 2.0 Update"
  End
End If
selectTopic "Pfade"
wm2pfad = GetINI$("Daten")
TiDatei2 = GetINI$("Titeldatei")
closeini

Melde "Lösche Daten im WinMusik 2.0-Verzeichnis..."
pz = 0: ShowQueue pz: st = 100 / 6
Kill wm2pfad + "\*.dat": pz = pz + st: ShowQueue pz
Kill wm2pfad + "\*.dir": pz = pz + st: ShowQueue pz
Kill wm2pfad + "\*.bez": pz = pz + st: ShowQueue pz
Kill wm2pfad + "\*.tit": pz = pz + st: ShowQueue pz
Kill wm2pfad + "\*.fre": pz = pz + st: ShowQueue pz
Kill wm2pfad + "\*.tbl"
Kill TiDatei2: ShowQueue 100

Melde "Kopiere Dateien, die unverändert bleiben..."
pz = 0: st = 100 / 6
ShowQueue pz
CopyBez wm1pfad + "\musikver.bez", wm2pfad + "\version.bez"
pz = pz + st: ShowQueue pz
CopyBez wm1pfad + "\musikart.bez", wm2pfad + "\musikart.bez"
pz = pz + st: ShowQueue pz
CopyBez wm1pfad + "\agerät.bez", wm2pfad + "\aufnahme.bez"
pz = pz + st: ShowQueue pz
CopyBez wm1pfad + "\aquelle.bez", wm2pfad + "\quelle.bez"
pz = pz + st: ShowQueue pz
CopyBez wm1pfad + "\tträger.bez", wm2pfad + "\label.bez"
pz = pz + st: ShowQueue pz
FileCopy wm1pfad + "\abkürz.dat", wm2pfad + "\abk.dat"
ShowQueue 100

openini "CDPlay20"
If inierror() = 0 Then
  selectTopic "Pfade"
  If inierror() = 0 Then
    Melde "Konvertiere CD-Player 2.0 "
    CDHeaderDatei = GetINI("CD-Header")
    cd1 = FreeFile
    Open CDHeaderDatei For Random As #cd1 Len = Len(CDHead1)
    cd2 = FreeFile
    Open wm2pfad + "\cdheader.dat" For Random As #cd2 Len = Len(CDHead2)
    chk = LOF(cd1) / Len(CDHead1)
    For ll = 1 To chk
      Get #cd1, ll, CDHead1
      pz = ll / chk * 100: ShowQueue pz
      CDHead2.TraegerArt = 2
      CDHead2.Index = CDHead1.Index
      CDHead2.Seite = CByte(CDHead1.Seite)
      CDHead2.Tracks = CDHead1.Tracks
      CDHead2.Gesamtlaenge = CDHead1.Gesamtlänge
      CDHead2.Startposition = CDHead1.Startposition
      For i = 0 To 10
        CDHead2.StartPos(i) = CDHead1.StartPos(i)
      Next
      Put #cd2, ll, CDHead2
    Next
    Close cd2, cd1
  End If
End If
closeini




TiF1 = FreeFile
Open TiDatei1 For Random As #TiF1 Len = Len(Ti1)
TiF2 = FreeFile
Open TiDatei2 For Random As #TiF2 Len = Len(Ti2)
tbl = FreeFile
Open wm2pfad + "\wmusik10.tbl" For Random As #tbl Len = 4
Fill = 0
Melde "Bereite kompatibilitäts-Tabelle vor..."
pz = 0: ShowQueue pz: st = 100 / LOF(TiF1) / Len(Ti1)
qq = LOF(TiF1) / Len(Ti1)
For chk = 1 To qq
  pz = chk / qq * 100: ShowQueue pz
  Put #tbl, chk, Fill
Next

GesamtTräger = 0
For Traeger = 1 To 6
  If Traeger = 3 Then Traeger = 4
  TrF1(Traeger) = FreeFile
  Open wm1pfad + TrägerDir(Traeger) For Random As #TrF1(Traeger) Len = Len(TrV1)
  GesamtTräger = GesamtTräger + LOF(TrF1(Traeger)) / Len(TrV1)
  TrF2(Traeger) = FreeFile
  Open wm2pfad + "\" + TrägerDir(Traeger) For Random As #TrF2(Traeger) Len = Len(TrV2)
  TraegerIndex(Traeger) = 1
Next
ShowQueue 0
Do
  ' Nächsten Träger finden
  For Traeger = 1 To 6
    If Traeger = 3 Then Traeger = 4
    TraegerDatum(Traeger) = 99999999
    If TraegerIndex(Traeger) > 0 Then
      Do Until EOF(TrF1(Traeger))
        Get #TrF1(Traeger), TraegerIndex(Traeger), TrV1
        If LCase$(TrV1.chk) = "j" Then
          If TrV1.Aufnahmestart >= 99990000 Then TrV1.Aufnahmestart = 1
          TraegerDatum(Traeger) = TrV1.Aufnahmestart
          Exit Do
        End If
        TraegerIndex(Traeger) = TraegerIndex(Traeger) + 1: erledigt = erledigt + 1
      Loop
      If EOF(TrF1(Traeger)) Then TraegerIndex(Traeger) = 0: Else Found = True
    End If
  Next
  If Found = False Then Exit Do
  LowTraeger = 0
  Lowdat = 99999999
  For Traeger = 1 To 6
    If Traeger = 3 Then Traeger = 4
    If TraegerDatum(Traeger) < Lowdat Then Lowdat = TraegerDatum(Traeger): LowTraeger = Traeger
  Next
  Traeger = LowTraeger
  If Traeger = 0 Then Exit Do
  Melde "Konvertiere " + TrägerName(Traeger) + ": " + Format$(TraegerIndex(Traeger))
  Get #TrF1(Traeger), TraegerIndex(Traeger), TrV1
  If TraegerIndex(Traeger) > LOF(TrF2(Traeger)) + 1 Then
    TrV2.InfoByte = 0
    For Fill = LOF(TrF2(Traeger)) To TraegerIndex(Traeger)
      Put #TrF2(Traeger), Fill, TrV2
    Next
  End If
  ConvertTrV TrV1, TrV2
  If TrV1.Titelnummer > 0 Then
    ff = FreeFile
    Open wm1pfad + "caption" + ext(Traeger) For Random As #ff Len = Len(TrTi)
    Get #ff, TrV1.Titelnummer, TrTi
    Close #ff
    If Trim$(TrTi.Titel1) + Trim$(TrTi.Titel2) <> "" Then
      Open wm2pfad + "\traeger.tit" For Random As #ff Len = Len(TrTi)
      TrV2.Bezeichnung = LOF(ff) / Len(TrTi) + 1
      Put #ff, TrV2.Bezeichnung, TrTi
      Close #ff
    Else
      TrV2.Bezeichnung = 0
    End If
  End If
  
  ' Einlesen der Trägertitel
  qq = FreeFile
  Open trägerpfad1(Traeger) + Format$(TraegerIndex(Traeger)) + ext(Traeger) For Random As #qq Len = Len(TrTr1)
  zz = FreeFile
  Open wm2pfad + "\" + trägerdat(Traeger) For Random As #zz Len = Len(TrDat2)
  Satz = LOF(zz) / Len(TrDat2) + 1
  TrV2.ErsterDatensatz = Satz
  LetzterSatz = 0
  Do Until EOF(qq)
    Get #qq, , TrTr1
    If LCase$(TrTr1.chk) = "j" Then
      If LetzterSatz > 0 Then
        Get #zz, LetzterSatz, TrDat2
        TrDat2.NaechsterDatensatz = Satz
        Put #zz, LetzterSatz, TrDat2
      End If
      TrDat2.LetzterDatensatz = LetzterSatz
      TrDat2.NaechsterDatensatz = 0
      TrDat2.TraegerIndex = Traeger
      TrDat2.Seite = CByte(TrTr1.Seite)
      TrDat2.Track = TrTr1.Track
      Get #tbl, TrTr1.Titelnummer, chk
      If chk > 0 Then
        TrDat2.Titelnummer = chk
        Get #TiF2, chk, Ti2
        Ti2.InfoByte = Ti2.InfoByte Or 2
        Put #TiF2, chk, Ti2
      Else
        Get #TiF1, TrTr1.Titelnummer, Ti1
        ConvertTi Ti1, Ti2
        Ti2.Nummer = LOF(TiF2) / Len(Ti2) + 1
        If Ti1.Bemerkung > 0 Then
          ff = FreeFile
          Open wm1pfad + "bemerk" + ext(Traeger) For Random As #ff Len = 80
          Get #ff, Ti1.Bemerkung, bem80
          Close #ff
          Open wm2pfad + "\bemerk.dat" For Random As #ff Len = 256
          bem256 = Trim$(bem80)
          Ti2.Bemerkung = LOF(ff) / 256 + 1
          Put #ff, Ti2.Bemerkung, bem256
          Close #ff
        End If
        Put #TiF2, Ti2.Nummer, Ti2
        TrDat2.Titelnummer = Ti2.Nummer
        Put #tbl, TrTr1.Titelnummer, Ti2.Nummer
      End If
      Put #zz, Satz, TrDat2
      LetzterSatz = Satz
      Satz = Satz + 1
    End If
  Loop
  Close #zz
  Close #qq
  Put #TrF2(Traeger), TraegerIndex(Traeger), TrV2
  TraegerIndex(Traeger) = TraegerIndex(Traeger) + 1
  erledigt = erledigt + 1
  pz = erledigt / GesamtTräger * 100
  ShowQueue pz
Loop

For Traeger = 1 To 6
  If Traeger = 3 Then Traeger = 4
  Close TrF2(Traeger)
  Close TrF1(Traeger)
Next
Close tbl, TiF2, TiF1
Melde "Update beendet!"

End Sub


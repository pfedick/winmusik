Attribute VB_Name = "Definitionen"
DefInt A-Z

'Declare Function getmemmancopyright Lib "ppl2" () As Long
'Declare Function getstring Lib "ppl2" (ByVal zielstring As String, ByVal Laenge As Long, ByVal adresse As Long)
'Declare Function testvb Lib "ppl2" (ByVal zielstring As String)

Declare Function WMLibVersion Lib "wm20.dll" (ByRef major As Long, ByRef minor As Long, ByRef release As Long) As Integer

'INT APIENTRY InitTitle (char *titeldatei) {
Declare Function InitTitle Lib "wm20" (ByVal datei As String) As Integer
'INT APIENTRY GetTitle (ppldd id, TITELDATEI *Ti) {
Declare Function GetWMTitle Lib "wm20" (ByVal id As Long, ByRef blah As Titeldatei) As Integer
Declare Function PutTitle Lib "wm20" (ByVal id As Long, ByRef blah As Titeldatei) As Integer
Declare Function UpdateTitle Lib "wm20" (ByVal id As Long) As Integer
Declare Function CloseTitle Lib "wm20" () As Integer

Declare Function GetAppPath Lib "wm20" (ByVal buffer As String, ByVal size As Long) As Integer

Declare Function CheckStruct Lib "wm20" (ByRef blah As Titeldatei) As Integer
Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" _
  (ByVal hwnd As Long, ByVal lpOperation As String, ByVal lpFile As String, _
  ByVal lpParameters As String, ByVal lpDirectory As String, _
  ByVal nShowCmd As Long) As Long


Declare Function CreateMutex Lib "kernel32" _
        Alias "CreateMutexA" _
       (ByVal lpMutexAttributes As Long, _
        ByVal bInitialOwner As Long, _
        ByVal lpName As String) As Long

Declare Function CloseHandle Lib "kernel32" (ByVal handle As Long) As Boolean




' Debugging ein/aus
Public Const DebugLevel = 0
Public Const Programm = "WinMusik"
Public Const version = "2.0"
Public Const LibVersion = "1.15"
Public build As Long
Public Const Progdate = "20.04.2009"
Public Const Autor = "Patrick Fedick"
Public Const copyright = "(c) Copyright by Patrick F.-Productions 1996-2009"
Public Const ShortCopyright = "(c) 2009 by Patrick F.-Productions"
Public Const Shareware = False
Public Const SharewareLimit = 300
Public Const ShowHitparade = False
Public Const ShowMixer = False
Public Const ShowFTPUpdate = False
'Internet-Update
Public Const InetUpdateURL = "http://www.pfp.de/wm/wmupdate.html"

#If Win16 Then
  Declare Function SetWindowPos Lib "user" (ByVal hwnd As Integer, ByVal hWndInsertAfter As Integer, ByVal X As Integer, ByVal Y As Integer, ByVal cx As Integer, ByVal cy As Integer, ByVal wFlags As Integer) As Integer
  Declare Function auxGetVolume Lib "MMSYSTEM" (ByVal uDeviceID As Integer, lpdwvolume As Long) As Integer
  Declare Function auxSetVolume Lib "MMSYSTEM" (ByVal uDeviceID As Integer, ByVal dwVolume As Long) As Integer
#ElseIf Win32 Then
  Declare Function SetWindowPos Lib "user32" (ByVal hwnd As Long, ByVal hWndInsertAfter As Long, ByVal X As Long, ByVal Y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long
  Declare Function mciGetDeviceID Lib "winmm.dll" Alias "mciGetDeviceIDA" (ByVal lpstrName As String) As Long
  Declare Function auxGetVolume Lib "winmm.dll" (ByVal uDeviceID As Long, lpdwvolume As Long) As Long
  Declare Function auxSetVolume Lib "winmm.dll" (ByVal uDeviceID As Long, ByVal dwVolume As Long) As Long
  Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
#End If

Type Titeldatei
  InfoByte As Integer
  interpret As String * 80
  titel As String * 80
  version As Long
  musikart As Long
  laenge As Single
  bpm As Integer
  Quelle As Long
  Erscheinungsjahr As Integer
  Aufnahmedatum As Long
  Bewertung As String * 2
  Qualitaet As String * 2
  bemerkung As Long
  Aufnahmegeraet As Long
  Soundfile As Long
  traeger As Byte
  Index As Long
  Seite As Byte
  track As Integer
  nummer As Long
End Type
' Infobyte:
'   Bit 0:  Gültiger Titel                                  1
'   Bit 1:  ?                                               2
'   Bit 2:  Stereo                                          4
'   Bit 3:  Dolby                                           8
'   Bit 4:  Surround                                        16
'   Bit 5:  vollständig                                     32
'   Bit 6:  echter Titel                                    64
'   Bit 7:  Digital 5.1                                     128
'   Bit 8:  Referenziert durch Tonträger                    256 (neu 14.11.2001)

Type OriginalFilename
    filename As String * 256
End Type
    
Type CDHeaderKennung
  TraegerArt As Byte
  Index As Long
  Seite As Byte
  Tracks As Integer
  gesamtlaenge As Long
  Startposition As Long
  StartPos(10) As Long
End Type

Type TraegerVerzeichnis
  InfoByte As Integer
  Seiten As Byte
  laenge As Single
  rest As Single
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
  cover As Long
  coverbild As Long
  ErsterDatensatz As Long
End Type

Type TraegerDatensatz
  LetzterDatensatz As Long
  NaechsterDatensatz As Long
  traegerindex As Long
  Titelnummer As Long
  Seite As Byte
  track As Integer
End Type

Type EingabeListing
  chk As Byte
  Titelnummer As Long
  interpret As String * 80
  titel As String * 80
  Interpret_Titel As String * 160
  info As String * 80
  laenge As Single
End Type

Type TraegerTitel
  Titel1 As String * 80
  Titel2 As String * 80
End Type

Type CoverArt
  Bezeichnung As String * 40
  Index As Long
End Type

Type SearchListe
  Titelnummer As Long
  Interpret_Titel As String * 160
  version As String * 80
  laenge As Single
  traeger As Byte
  Index As Long
  Seite As Byte
  track As Integer
End Type

Type TabelleSuchen
  Index As Long
  Inhalt As String * 80
End Type

Type Abkuerzungen
   abk As String * 10
   interpret As String * 80
End Type

Type ChartHeader
  handle As Long
  TitelLang As String * 100
  TitelKurz As String * 20
  Plaetze As Integer
  Auswertungsart As Integer
  icon As String * 200
End Type

Type ChartData
  Position As Integer
  Platz As Integer
  Vorwoche As Integer
  Woche As Integer
  titel As Long
  Gespielt As Long
  Punkte As Long
End Type

Type ChartListing
  Platz As Integer
  Vorwoche As Integer
  titel As Long
  interpret As String * 160
  version As String * 80
  laenge As Single
  Woche As Integer
  traeger As Integer
  Index As Long
  Seite As Byte
  track As Integer
End Type

Type SortInterpret
  interpret As String * 80
  ErsterEintrag As Long
  LetzterEintrag As Long
End Type

Type InterpretenEintrag
  nummer As Long
  Letzter As Long
  Naechster As Long
End Type


Type MySQLDatabase
    host As String
    port As Long
    name As String
    user As String
    password As String
End Type

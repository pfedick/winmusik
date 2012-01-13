Attribute VB_Name = "cdplay"
DefInt A-Z



Global CDLoaded As Integer
Global TracksLoaded As Integer
Global Suchstatus As Integer
'      Suchstatus:    0 = Es wurde noch nicht gesucht
'                     1 = Suche wird gestartet
'                     2 = Suche im Gange
'                     3 = Suche beendet
'                     4 = CD nicht gefunden

Global CDHead As CDHeaderKennung
Global TIFile As Integer
Global TrVFile As Integer
Global CDFile As Integer
Global HDRFile As Integer
Global SuchHDR As CDHeaderKennung
Global CDHdrPos As Long
Global CDHdrDatensatz As Long

Global Tracks(50) As Long
Global TrackName(50)  As String


Global RunningTrack As Integer

Global TitelInfoLoaded As Integer
Global TitelListeLoaded As Integer

Global TrackPosition(100)  As Long
Global TrackLength(100) As Long

Global LastStatus As Integer
Global Pause As Integer
Global ShowTime As Integer

Global CDLength As Long
Global CDStart As Long
Global CDTracks As Integer

Global Const maxCDFastsearch = 2048
Global CDFastsearch(maxCDFastsearch) As Long
Global CDFastsearchStart As Long
Global CDFastsearchEnd As Long

Sub PrepareFastsearch()
Dim fcd As CDHeaderKennung
Dim i, p As Long
f = FreeFile
Open CDHeaderDatei For Random As #f Len = Len(fcd)
CDFastsearchEnd = LOF(f) / Len(fcd)
If CDFastsearchEnd > maxCDFastsearch Then
  CDFastsearchStart = CDFastsearchEnd - maxCDFastsearch - 1
Else
  CDFastsearchStart = 1
End If
p = 0
For i = CDFastsearchStart To CDFastsearchEnd
  Get #f, i, fcd
  CDFastsearch(i - CDFastsearchStart) = fcd.Gesamtlaenge
Next
Close #f
End Sub



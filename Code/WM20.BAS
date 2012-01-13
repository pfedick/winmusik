Attribute VB_Name = "Hauptmodul"
DefInt A-Z

Public myMutEx As Long

Public Ti As Titeldatei
Public WTi As Titeldatei
Public TrDat As TraegerDatensatz
Public TrTi As TraegerTitel
Public TrV As TraegerVerzeichnis
Public el As EingabeListing
Public CurrentTi As Titeldatei
Public TabSuch As TabelleSuchen

Public CDTi As Titeldatei
Public CDTrV As TraegerVerzeichnis
Public CDTrTi As TraegerTitel
Public CDTrDat As TraegerDatensatz

Public ch As ChartHeader
Public CD As ChartData
Public cl As ChartListing
Public ChartsLoaded As Boolean
Public ChartWeekLoaded As Boolean
Public ChartWeek As Integer
Public ChartYear As Integer

Public HelpFile As String

Public AppData As String
Public HauptPfad As String
Public DatenPfad As String
Public TiDatei As String
Public CDHeaderDatei As String
Public tmp As String
Public drucker As String
Public user As String
Public UserFirma As String
Public Waehrung As String
Public SprachDatei As String
Public Farbdruck As Boolean
Public AutoInterpret As Boolean
Public AutoTitel As Boolean
Public ShowSplashScreen As Boolean
Public InetProxy As String
Public InetPort As String
Public CDRipperPfad As String
Public db As MySQLDatabase


Public RequireSetup As Boolean
Public UseDLL As Boolean

' Fenster sichtbar?
Public eingabevisible As Boolean
Public traegerstammvisible As Boolean
Public copyrightvisible As Boolean
Public cdplayervisible As Boolean
Public quicksearchvisible As Boolean
Public ShowHelp As Boolean
Public mixervisible As Boolean
Public chartsvisible As Boolean
Public sqlexportvisible As Boolean
Public vcdcovervisible As Boolean
Public cdreadvisible As Boolean


Public saverequired As Boolean


Public tpx As Integer: ' TwipsPerPixelX
Public tpy As Integer: ' TwipsPerPixelY
Public ptpx As Integer: ' PrinterTwipsPerPixelX
Public ptpy As Integer: ' PrinterTwipsPerPixelY

Public EingabePos As Integer
Public LetzteEingabePos As Integer

Public TraegerBez(20) As String
Public traeger As Integer
Public traegerindex As Long
Public Seite As Integer
Public track As Integer
Public Traegerloaded As Integer

Public tab80 As String * 80
Public TabelleName As String
Public TabelleVorgabe As String
Public TabelleFuellFeld As String

Public ListingAnzeige As Integer: ' Indexnummer für Versions-anzeige Titellisting

Public last_e(17) As String
Public MarkierungEingabeListe As Integer

'Programme
Public MPEGPlayer As String
Public WAVEPlayer As String
Public MixerProg As String


Public mySQL As Long

Sub FindReferences()
Dim az As Long, p As Long, pp As Long
Dim titel As Titeldatei
Dim t As Long, i As Long, ii As Long
Dim ff As Integer, tff As Integer
Dim nf As Long
Dim tv As TraegerVerzeichnis
Dim tdat As TraegerDatensatz
Dim tret As Integer


'WMStat_Clear ("Prüfe Referenzierungen...")
druckmsg.Show
druckmsg.Caption = "Prüfe Referenzierungen..."
druckmsg.textfeld = "Diese Routine überprüft, ob jeder Titel in der Datenbank einem Tonträger zugeordnet ist und markiert alle, bei denen das nicht der Fall ist"
druckmsg.queue.Visible = True
druckmsg.Image1.Visible = False
druckmsg.queue.min = 0
druckmsg.queue.max = 100
druckmsg.queue.value = 0
DoEvents
ff = FreeFile
Open TiDatei For Random As #ff Len = Len(titel)
pc = 0: az = LOF(ff) / Len(titel)
' Zuerst das Referenz-Bit löschen
For p = 1 To az
  If Int(p / az * 100) <> pc Then
    pc = Int(p / az * 100)
    'WMStat_Queue (pc)
    druckmsg.queue.value = pc
    DoEvents
  End If
  Get #ff, p, titel
  titel.InfoByte = titel.InfoByte And (Not 256)
  Put #ff, p, titel
  tret = UpdateTitle(p)
Next

' Jetzt die Tonträger der Reihe nach durchgehen
For t = 1 To 8
    
    tff = FreeFile
    Open DatenPfad + "\" + TraegerBez(t) + ".dir" For Random As #tff Len = Len(tv)
    nf = LOF(tff) / Len(tv) + 1
    druckmsg.queue.value = 0
    For ii = 1 To nf
        'If t = 7 And ii = 54 Then Stop
        Get #tff, ii, tv
        druckmsg.textfeld = "Prüfe " + Lang(299 + t) + " " + Format(ii) + ", " + Format(tv.Tracks) + " Tracks..."
        
        DoEvents
        If tv.ErsterDatensatz > 0 Then
            di = FreeFile
            Open DatenPfad + "\" + TraegerBez(t) + ".dat" For Random As #di Len = Len(tdat)
            pp = tv.ErsterDatensatz
            Do
                Get #di, pp, tdat
                If tdat.Titelnummer > 0 Then
                   Get #ff, tdat.Titelnummer, titel
                   titel.InfoByte = titel.InfoByte Or 256
                   If titel.traeger = t Then
                        'Stop
                        titel.Index = ii
                        titel.Seite = tdat.Seite
                        titel.track = tdat.track
                   End If
                   Put #ff, tdat.Titelnummer, titel
                   tret = UpdateTitle(tdat.Titelnummer)
                End If
                pp = tdat.NaechsterDatensatz
            Loop Until pp = 0
            Close #di
        End If

    Next
    Close #tff
Next
Close #ff



If druckmsg.abbrechen.tag <> "true" Then
  openini AppData + "winmusik2.ini"
  selectTopic "Sort"
  putini "Referenziert=true"
  closeini
End If
Unload druckmsg

End Sub

Function TraegerTmpFile(Seite As String) As String
Dim t As String
t = tmp + "\~wm2in-" + Format(traeger) + "-" + Format(traegerindex) + "-" + Seite + ".tmp"
TraegerTmpFile = t
End Function

Sub WMStat_Clear(t As String)
If eingabevisible = True Then
    eingabe.StatusBar1.Panels(2).text = t
    WMStat_MinMax 0, 100
    WMStat_Queue -1
End If

End Sub
Sub WMStat_MinMax(min As Long, max As Long)
eingabe.ProgressBar1.min = min
eingabe.ProgressBar1.max = max

End Sub

Sub WMStat_Queue(wert As Long)
Dim prozent As Long
If wert >= 0 Then
    prozent = wert * 100 / eingabe.ProgressBar1.max
    eingabe.ProgressBar1.value = wert
    eingabe.StatusBar1.Panels(3).text = Format(prozent, "0") + "%"
Else
    eingabe.ProgressBar1.value = eingabe.ProgressBar1.min
    eingabe.StatusBar1.Panels(3).text = ""
End If
End Sub


Sub WMStat_Text(t As String)
If eingabevisible = True Then
    eingabe.StatusBar1.Panels(2).text = t
End If
End Sub
Sub CleanUp()
Dim fr As Integer
Dim t As Integer
Dim pp As Long
Dim nn As Integer
Dim i As Long
Dim zz As Long

On Error Resume Next

' FRE-Tabellen aufräumen
For t = 1 To 6
  fr = FreeFile
  Open DatenPfad + "\" + TraegerBez(t) + ".fre" For Random As #fr Len = 4
  If DebugLevel > 0 Then Debug.Print "Cleaning up " + DatenPfad + "\" + TraegerBez(t) + ".fre"
  If LOF(fr) > 0 Then
    Get #fr, 1, pp
    If pp = 0 Then
      Kill tmp + "~wm2free.tmp"
      nn = FreeFile
      Open tmp + "~wm2free.tmp" For Random As #nn Len = 4
      zz = 1
      For i = 1 To LOF(fr) / 4
        Get #fr, i, pp
        If pp > 0 Then Put #nn, zz, pp: zz = zz + 1
      Next
      If DebugLevel > 0 Then
        If LOF(nn) > 0 Then Debug.Print "  removed"; LOF(fr) / 4 - zz; " from"; LOF(fr) / 4
      End If
      Close #nn
      Close #fr
      fr = 0
      FileCopy tmp + "~wm2free.tmp", DatenPfad + "\" + TraegerBez(t) + ".fre"
    End If
  End If
  If fr > 0 Then Close #fr
Next

' Temporäres Verzeichnis aufräumen
If DebugLevel > 0 Then Debug.Print "Cleaning TMP: " + tmp
'Kill tmp + "\~wm2*.tmp"

End Sub

Sub ClearEingabeFelder()
eingabe.e(0).text = "": eingabe.e(1).text = ""
eingabe.e(6).text = "0,00": eingabe.e(7).text = "0"
eingabe.e(14).text = "": eingabe.e(15).text = "": eingabe.e(16).text = "": eingabe.e(17).text = ""
End Sub

Sub DeleteTrack(nr As Integer)
Dim tmpel As EingabeListing
Dim tmpti As Titeldatei
tt = FreeFile
Open TraegerTmpFile(Format(Seite)) For Random As #tt Len = Len(el)
high = LOF(tt) / Len(tmpel)
If high >= nr Then
  Get #tt, nr, tmpel
  If tmpel.chk > 0 Then
    tmpel.chk = 0
    Put #tt, nr, tmpel
  Else
    For i = nr To high - 1
      Get #tt, i + 1, tmpel
      If tmpel.chk > 0 Then
        GetTitel tmpti, tmpel.Titelnummer
        If tmpti.Index = traegerindex Then
          tmpti.track = i
          SaveTitel tmpti
        End If
      End If
      Put #tt, i, tmpel
    Next
    tmpel.chk = 0
    Put #tt, high, tmpel
  End If
End If
Close #tt
saverequired = True
eingabe.Listfeld.Refresh
End Sub

Sub FillChartAuswahl()
ff = FreeFile
Open DatenPfad + "\charts.dir" For Random As #ff Len = Len(ch)
Do
  Get #ff, , ch
  'If Not EOF(ff) Then Rem'charts.ChartAuswahl.AddItem Trim(ch.TitelKurz)
Loop Until EOF(ff)
Close #ff
End Sub

Function FindInterpret(such$) As String
Dim si As SortInterpret
Dim el As EingabeListing
ff = FreeFile
s$ = LCase(such$): l = Len(s$)
Open TraegerTmpFile(Format(Seite)) For Random As #ff Len = Len(el)
For i = 1 To LOF(ff) / Len(el)
  Get #ff, i, el
  If left$(LCase(Trim(el.interpret)), l) = s$ Then
    found$ = Trim(el.interpret)
    Exit For
  End If
Next
Close #ff
If Len(such$) > 2 And found$ = "" Then
  f$ = DatenPfad + "\sort" + LCase$(left$(such$, 1)) + ".ndx"
  ff = FreeFile
  Open f$ For Random As #ff Len = Len(si)
  p& = 0
  found$ = ""
  For z& = LOF(ff) / Len(si) To 1 Step -1
    Get #ff, z&, si
    If left$(LCase(Trim(si.interpret)), l) = s$ Then
      found$ = Trim(si.interpret)
      Exit For
    End If
  Next
  Close #ff
End If
FindInterpret = found$
End Function
Function FindTab(datei$, suchstring As String) As Long
Dim nr As Long, i As Long
Dim t As String * 80

On Error Resume Next
ff = FreeFile
If InStr(datei$, ".") = 0 Then datei$ = datei$ + ".bez"
Open DatenPfad + "\" + datei$ For Random As #ff Len = 80
nr = LOF(ff) / 80
suchstring = Trim(suchstring)
FindTab = 0
For i = 1 To nr
  Get #ff, i, t
  If LCase(Trim(t)) = LCase(suchstring) Then
     FindTab = i
     i = nr
  End If
Next
Close #ff

End Function

Function FindTitel(such$) As String
Dim el As EingabeListing
ff = FreeFile
s$ = LCase(such$): l = Len(s$)
Open TraegerTmpFile(Format(Seite)) For Random As #ff Len = Len(el)
For i = 1 To LOF(ff) / Len(el)
  Get #ff, i, el
  If left$(LCase(Trim(el.titel)), l) = s$ Then
    found$ = Trim(el.titel)
    Exit For
  End If
Next
Close #ff
FindTitel = found$
End Function


Sub FTaste(taste As Integer, Shift As Integer)
Dim id3 As id3tag
Dim id3v2 As id3v2tag
Dim mp3 As MP3HEADER
Dim genre As String
Dim p As Long
Dim ret As Long
Dim versionId As Long
Dim whitespace As Boolean


If DebugLevel > 0 Then Debug.Print "Tastenauswertung:"; taste; ", Shift:"; Shift
Select Case taste
   Case 0: If EingabePos = -3 Then Unload eingabe
           If EingabePos = -2 Then SaveTraeger: EingabePos = -3: eingabe.index_e.SetFocus
           If EingabePos = -1 And TrV.Seiten > 1 Then EingabePos = -2: eingabe.seite_e.SetFocus
           If EingabePos = -1 And TrV.Seiten = 1 Then SaveTraeger: EingabePos = -3: eingabe.index_e.SetFocus
           If EingabePos = 0 Then EingabePos = -1: eingabe.track_e.SetFocus
           If EingabePos > 0 Then EingabePos = 0: eingabe.e(0).SetFocus
   Case 1:
            If traeger = 7 And EingabePos >= 0 Then
                Dim m As New mp3check
                m.LoadFile (track), GetMp3FileName(traegerindex, (track))
            End If
   
   Case 2: If EingabePos > -3 Then traegerstamm.Show
   Case 3:
            whitespace = True
            If EingabePos >= 0 Then
                a$ = eingabe.e(EingabePos).text
                For i = 1 To Len(a$) Step 1
                    l$ = Mid(a$, i, 1)
                    If whitespace = True Then Mid(a$, i, 1) = UCase(l$) Else Mid(a$, i, 1) = LCase(l$)
                    whitespace = False
                    If l$ = " " Or l$ = "." Or l$ = "&" Or l$ = "-" Then whitespace = True
                Next
                eingabe.e(EingabePos).text = a$
            End If
   Case 4:
           If EingabePos > -1 And EingabePos < 18 Then
             If EingabePos = 0 Or EingabePos = 1 Then
                If quicksearchvisible = False Then
                    quicksearch.Show
                End If
                quicksearch.StopSearchNow
                quicksearch.z1.text = eingabe.e(0).text
                quicksearch.z2.text = eingabe.e(1).text
                quicksearch.StartSearch
                
                

             ElseIf EingabePos = 2 Then
               ZeigeTabelle "version.bez", eingabe.e(2).text, "eingabe 2"
             ElseIf EingabePos = 4 Then
               ZeigeTabelle "musikart.bez", eingabe.e(4).text, "eingabe 4"
             ElseIf EingabePos = 10 Then
               ZeigeTabelle "quelle.bez", eingabe.e(10).text, "eingabe 10"
             ElseIf EingabePos = 12 Then
               ZeigeTabelle "aufnahme.bez", eingabe.e(12).text, "eingabe 12"
             End If
           ElseIf EingabePos = -3 Then
             NextFree = GetNextFreeTraeger(traeger)
             eingabe.index_e.text = Format$(NextFree)
             SendKeys "{home}+{end}"
           ElseIf EingabePos = -1 Then
                If quicksearchvisible = False Then
                    quicksearch.Show
                Else
                    Unload quicksearch
                End If
           End If
   Case 5:
           If EingabePos = 0 And Shift = 0 Then
             b$ = Trim(LCase(eingabe.e(0).text))
             a$ = GetAbkuerzung(b$)
             eingabe.e(0).text = a$
             If b$ <> a$ Then
               eingabe.e(1).SetFocus
             Else
               eingabe.Enabled = False
               EingabeAbkuerzung.Show
               EingabeAbkuerzung.abk_e.text = b$
               EingabeAbkuerzung.abk_e.tag = b$
               EingabeAbkuerzung.text_e.SetFocus
             End If
           ElseIf EingabePos = 0 And Shift > 0 Then
             b$ = Trim(LCase(eingabe.e(0).text))
             a$ = GetAbkuerzung(b$)
             eingabe.Enabled = False
             EingabeAbkuerzung.Show
             EingabeAbkuerzung.abk_e.text = b$
             EingabeAbkuerzung.text_e.text = a$
             EingabeAbkuerzung.abk_e.tag = b$
             EingabeAbkuerzung.text_e.SetFocus
           End If
   Case 6:
         If traeger = 7 Then
           If track > 0 Then
             a$ = GetMp3FileName(traegerindex, (track))
             If a$ <> "" Then
               If Shift > 0 Then
                  ret = ShellExecute(eingabe.hwnd, "open", a$, "", "", SW_SHOWNORMAL)
               End If
               
               eingabe.QuickInfoPlay.tag = a$
               b$ = GetFileName(a$)
               b$ = ereg_replace("_", " ", b$)
               eingabe.quickinfo.text = "Filename: " + b$
               If LCase(right(b$, 4)) = ".mp3" Then b$ = left(b$, Len(b$) - 4)
               If Mid(b$, 4, 1) = "-" And Format(CLng(left(b$, 3)), "000") = left(b$, 3) Then
                 b$ = Mid(b$, 5)
               End If
               Clipboard.SetText b$, vbCFText
               eingabe.quickinfo.top = eingabe.Listfeld.Height - 300
               eingabe.QuickInfoPlay.top = eingabe.quickinfo.top
               eingabe.QuickInfoPlay.left = eingabe.quickinfo.left - Screen.TwipsPerPixelX * 16
               
               eingabe.quickinfo.Visible = True
               eingabe.QuickInfoPlay.Visible = True
               ret = 0
               mp3.status = 0
               On Error Resume Next
               ret = GetMP3Info(a$, mp3, id3, id3v2)
               If ret = 0 And (mp3.status And 1) = 1 Then
                'Debug_PrintMP3Header mp3
                eingabe.e(6).text = Format(Int(mp3.length / 60), "0") + "," + Format(mp3.length Mod 60, "00")
                If Shift = 0 And (mp3.status And 14) > 0 Then
                  eingabe.e(0).text = Cut0(id3v2.artist)
                  eingabe.e(1).text = Cut0(id3v2.SongName)
                  eingabe.e(2).text = 0
                  v$ = Cut0(id3v2.version)
                  If v$ <> "" Then
                    versionId = FindTab("version.bez", v$)
                    If versionId > 0 Then
                        eingabe.e(2).text = versionId
                    Else
                        eingabe.e(2).text = "**"
                        eingabe.e(3).text = v$
                    End If
                        
                  End If
                  a$ = Trim(Cut0(id3v2.album) + " " + Cut0(id3v2.comment))
                  If a$ <> v$ Then
                    eingabe.e(17).text = a$
                  Else
                    eingabe.e(17).text = ""
                  End If
                  a$ = Cut0(id3v2.year)
                  If a$ <> "" Then eingabe.e(8).text = a$
                  a$ = Cut0(id3v2.genre)
                    If Mid(a$, 1, 1) = "(" Then
                        v$ = Mid(a$, 2)
                        If InStr(v$, ")") > 0 Then
                            v$ = left(v$, InStr(v$, ")") - 1)
                        End If
                        p = CInt(v$)
                        a$ = Trim(Mid(a$, InStr(a$, ")") + 1))
                        If a$ = "" Then a$ = GetGenre(p)
                    End If
                  a$ = Trim(a$)
                  If a$ = "" Then
                      p = CInt(id3.genre)
                      a$ = GetGenre(p)
                      a$ = Trim(a$)
                  Else
                  End If
                  If a$ <> "" Then
                    versionId = FindTab("musikart.bez", a$)
                    If versionId > 0 Then
                       eingabe.e(4).text = versionId
                    Else
                        eingabe.e(4).text = "**"
                        eingabe.e(5).text = a$
                    End If
                  End If
                End If
               End If
             End If
           Else
             If EingabePos = -1 Then
               l = eingabe.fkey(6).left + eingabe.functions.left + eingabe.left + 30
               mp3auswahl.Height = 0
               mp3auswahl.left = l
               t = eingabe.functions.top + eingabe.top - mp3auswahl.Height + 360
               mp3auswahl.top = t
               DoEvents
               mp3auswahl.Show
               mp3auswahl.SetFocus
               DoEvents
               For i = 0 To 1560 Step 45
                 mp3auswahl.Height = i
                 mp3auswahl.Move l, t - i
                 Sleep (1)
                 DoEvents
               Next
               mp3auswahl.Height = 1560
               mp3auswahl.top = t - 1560
               'eingabe.Enabled = False
               'mp3load.Show
             End If
           End If
         End If
   
   Case 7:
           If EingabePos >= 0 And Shift = 0 Then
             CurrentTi.nummer = 0
             DeleteTrack track
             eingabe.InitFields
           ElseIf EingabePos >= -1 And Shift > 0 And MarkierungEingabeListe > 0 Then
             DeleteTrack MarkierungEingabeListe
             eingabe.track_e.text = Format(MarkierungEingabeListe)
             eingabe.InitFields
             If MarkierungEingabeListe = track Then CurrentTi.nummer = 0
           End If
          
   Case 8:
           If EingabePos >= 0 And Shift = 0 Then
             CurrentTi.nummer = 0
             InsertTrack track
             eingabe.InitFields
           ElseIf EingabePos >= -1 And Shift > 0 And MarkierungEingabeListe > 0 Then
             InsertTrack MarkierungEingabeListe
             eingabe.track_e.text = Format(MarkierungEingabeListe)
             eingabe.InitFields
             If MarkierungEingabeListe = track Then CurrentTi.nummer = 0
           End If
   Case 9:
           If traeger = 7 And Traegerloaded = True Then
             makeid3tags
           End If

   Case 10:
           If EingabePos = 2 Or EingabePos = 4 Or EingabePos = 10 Or EingabePos = 12 Then
             LetzteEingabePos = EingabePos + 1
             eingabe.e(LetzteEingabePos).SetFocus
           End If
           If traeger = 7 And Traegerloaded = True Then
             makempegindex
           End If

   Case 11:
            If EingabePos >= -3 Then
              coverdruck.Show
              coverdruck.von.text = eingabe.index_e.text
              coverdruck.bis.text = eingabe.index_e.text
              'eingabe.Enabled = False
            End If
   Case 12: If EingabePos >= 0 Then SaveTrack
            If eingabe.quickinfo.Visible = True Then
                eingabe.quickinfo.Visible = False
                eingabe.QuickInfoPlay.Visible = False
            End If
End Select
End Sub

Function GetAbkuerzung(abkuerzung$) As String
Dim abk As Abkuerzungen
a$ = LCase$(Trim(abkuerzung$))
ff = FreeFile
Open DatenPfad + "\abk.dat" For Random As #ff Len = Len(abk)
high& = LOF(ff) / Len(abk)
f$ = a$
For i& = high& To 1 Step -1
  Get #ff, i&, abk
  If InStr(abk.abk, a$) > 0 Then f$ = Trim(abk.interpret)
Next
Close #ff
GetAbkuerzung = f$
End Function


Function GetFile(datei$, nummer As Long) As String
Dim t As String * 256
ff = FreeFile
If InStr(datei$, ".") = 0 Then datei$ = datei$ + ".dat"
Open DatenPfad + "\" + datei$ For Random As #ff Len = Len(t)
If LOF(ff) / Len(t) < nummer Or nummer = 0 Then
  GetFile = "": a$ = ""
Else
  Get #ff, nummer, t
  GetFile = Trim$(t): a$ = Trim$(t)
End If
Close #ff
If DebugLevel > 1 Then Debug.Print "GetFile "; datei$; ":"; nummer; "=> "; a$

End Function
Sub clearTrV(a As TraegerVerzeichnis)
a.InfoByte = 0
a.Seiten = 0
a.laenge = 0
a.rest = 0
a.Bezeichnung = 0
a.Label = 0
a.Kaufquelle = 0
a.Kaufdatum = 0
a.Preis = 0
a.Erstelldatum = 0
a.Aufnahmestart = 0
a.Aufnahmeende = 0
a.Tracks = 0
a.Singles = 0
a.Mixe = 0
a.cover = 0
a.coverbild = 0
a.ErsterDatensatz = 0
End Sub

Function GetNextFreeFile(datei$) As Long
Dim t As String * 256
If DebugLevel > 0 Then Debug.Print "GetNextFreeFile "; datei$
ff = FreeFile
If InStr(datei$, ".") = 0 Then datei$ = datei$ + ".dat"
Open DatenPfad + "\" + datei$ For Random As #ff Len = Len(t)
GetNextFreeFile = LOF(ff) / Len(t) + 1
Close #ff
End Function


Function GetNextFreeTab(datei$) As Long
Dim ff As Integer
If DebugLevel > 0 Then Debug.Print "GetNextFreeTab "; datei$
ff = FreeFile
If InStr(datei$, ".") = 0 Then datei$ = datei$ + ".bez"
Open DatenPfad + "\" + datei$ For Random As #ff Len = 80
GetNextFreeTab = LOF(ff) / 80 + 1
Close #ff
End Function

Function GetNextFreeTraeger(ntraeger As Integer) As Long
Dim tmpTrV As TraegerVerzeichnis
ff = FreeFile
Open DatenPfad + "\" + TraegerBez(ntraeger) + ".dir" For Random As #ff Len = Len(tmpTrV)
NextFree = Format$(LOF(ff) / Len(tmpTrV) + 1)
If DebugLevel > 0 Then Debug.Print "Searching for next free Track..."
If NextFree > 0 Then
  For i& = NextFree To 1 Step -1
    Get #ff, i&, tmpTrV
    If DebugLevel > 1 Then Debug.Print "Checking Track"; i&; ": "; tmpTrV.InfoByte
    'If i& > 356 Then tmpTrV.InfoByte = 0: tmpTrV.ErsterDatensatz = 0: Put #ff, i&, tmpTrV
    If tmpTrV.InfoByte > 0 Then Exit For
  Next
  NextFree = i& + 1
End If
Close #ff
GetNextFreeTraeger = NextFree
End Function

Function GetNextFreeTraegerTitel() As Long
Dim ff As Integer
If DebugLevel > 0 Then Debug.Print "GetNextFreeTraegerTitel "
ff = FreeFile
Open DatenPfad + "\traeger.tit" For Random As #ff Len = Len(titel)
GetNextFreeTraegerTitel = LOF(ff) / Len(TrTi) + 1
Close #ff
End Function

Function GetTab(datei$, nummer As Long) As String
Dim t As String * 80
On Error Resume Next
ff = FreeFile
If InStr(datei$, ".") = 0 Then datei$ = datei$ + ".bez"
Open DatenPfad + "\" + datei$ For Random As #ff Len = 80
If nummer = 0 Then
  GetTab = "unbekannt"
ElseIf LOF(ff) / 80 < nummer Then
  GetTab = "": a$ = ""
Else
  Err = 0
  Get #ff, nummer, t
  If Err > 0 Then
    a$ = "unbekannt": nummer = 0
  Else
    GetTab = Trim$(t): a$ = Trim$(t)
  End If
End If
Close #ff
If DebugLevel > 1 Then Debug.Print "GetTab "; datei$; ":"; nummer; "=> "; a$

End Function
Sub GetTitel(TitelInhalt As Titeldatei, nummer As Long)
Dim ret As Integer
On Error Resume Next
'If UseDLL = True Then
'  ret = GetTitle(nummer, Ti)
'  TitelInhalt = Ti
'  If ret = 0 Then Exit Sub
'End If

ret = GetWMTitle(nummer, Ti)
If ret > 0 Then
    Ti.nummer = 0
End If

TitelInhalt = Ti

'ff = FreeFile
'Open TiDatei For Random As #ff Len = Len(Ti)
'If nummer <= LOF(ff) / Len(TitelInhalt) And nummer > 0 Then
'  Get #ff, nummer, Ti
'Else
'  Ti.nummer = 0
'End If
'TitelInhalt = Ti
'Close #ff
End Sub

Sub GetTraegerTitel(nummer As Long, titel As TraegerTitel)
On Error Resume Next
If DebugLevel > 1 Then Debug.Print "GetTraegerTitel:"; nummer
Err = 0
ff = FreeFile
If Err > 0 Then MsgBox "Error: " + Format$(Err) + "GetTraegerTitel" + r + "No further FreeFiles!", 16, Programm: Exit Sub
Open DatenPfad + "\traeger.tit" For Random As #ff Len = Len(titel)
If LOF(ff) / Len(titel) < nummer Or nummer = 0 Then
  titel.Titel1 = ""
  titel.Titel2 = ""
Else
  Get #ff, nummer, titel
End If
Close #ff
End Sub
Sub IndexTitel(TiNummer As Long, AlterInterpret As String, NeuerInterpret As String)
Dim si As SortInterpret
Dim ie As InterpretenEintrag

If TiNummer > 0 Then
  If AlterInterpret <> "" And AlterInterpret <> "?" And AlterInterpret <> "*" Then
    If AlterInterpret = NeuerInterpret Then Exit Sub
    'Stop
    i$ = SplitInterpret(AlterInterpret)
    Do
      f$ = DatenPfad + "\sort" + LCase$(left$(i$, 1)) + ".ndx"
      ff = FreeFile
      Open f$ For Random As #ff Len = Len(si)
      s$ = LCase(i$)
      p& = 0
      For z& = 1 To LOF(ff) / Len(si)
        Get #ff, z&, si
        If LCase(Trim(si.interpret)) = s$ Then
          p& = z&
          Exit For
        End If
      Next
      f1 = FreeFile
      Open DatenPfad + "\sort.dat" For Random As #f1 Len = Len(ie)
      If p& <> 0 Then
        Get #ff, p&, si
        'Get #f1, si.LetzterEintrag, ie
        'ie.Naechster = LOF(f1) / Len(ie) + 1
        'Put #f1, si.LetzterEintrag, ie
        'ie.Letzter = si.LetzterEintrag
        'si.LetzterEintrag = ie.Naechster
        'Put #ff, p&, si
        
      End If
      Close #ff
      'ie.Nummer = TiNummer: ie.Naechster = 0
      'Put #f1, si.LetzterEintrag, ie
      Close #f1
      i$ = SplitInterpret("")
    Loop Until i$ = ""
    
  End If
  If NeuerInterpret <> "" And NeuerInterpret <> "?" And NeuerInterpret <> "*" Then
    i$ = SplitInterpret(NeuerInterpret)
    Do
      f$ = DatenPfad + "\sort" + LCase$(left$(i$, 1)) + ".ndx"
      ff = FreeFile
      Open f$ For Random As #ff Len = Len(si)
      s$ = LCase(i$)
      p& = 0
      For z& = 1 To LOF(ff) / Len(si)
        Get #ff, z&, si
        If LCase(Trim(si.interpret)) = s$ Then
          p& = z&
          Exit For
        End If
      Next
      f1 = FreeFile
      Open DatenPfad + "\sort.dat" For Random As #f1 Len = Len(ie)
      If p& = 0 Then
        p& = LOF(f1) / Len(ie) + 1
        si.interpret = i$: si.ErsterEintrag = p&
        si.LetzterEintrag = p&
        z& = LOF(ff) / Len(si) + 1
        Put #ff, z&, si
        ie.Letzter = 0
      Else
        Get #ff, p&, si
        Get #f1, si.LetzterEintrag, ie
        ie.Naechster = LOF(f1) / Len(ie) + 1
        Put #f1, si.LetzterEintrag, ie
        ie.Letzter = si.LetzterEintrag
        si.LetzterEintrag = ie.Naechster
        Put #ff, p&, si
      End If
      Close #ff
      ie.nummer = TiNummer: ie.Naechster = 0
      Put #f1, si.LetzterEintrag, ie
      Close #f1
      i$ = SplitInterpret("")
    Loop Until i$ = ""
  End If
End If
End Sub

Sub InsertTrack(nr As Integer)
Dim tmpel As EingabeListing
Dim tmpti As Titeldatei
tt = FreeFile
Open TraegerTmpFile(Format(Seite)) For Random As #tt Len = Len(el)
high = LOF(tt) / Len(tmpel)
If high >= nr Then
  For i = high To nr Step -1
    Get #tt, i, tmpel
    If tmpel.chk > 0 Then
      GetTitel tmpti, tmpel.Titelnummer
      If tmpti.Index = traegerindex Then
        tmpti.track = i + 1
        SaveTitel tmpti
      End If
    End If
    Put #tt, i + 1, tmpel
  Next
  tmpel.chk = 0
  Put #tt, nr, tmpel
End If
Close #tt
saverequired = True
eingabe.Listfeld.Refresh

End Sub

Function LoadTraeger()
Dim f(10) As Integer
Dim ff As Integer
Dim pp As Long
Dim tret As Integer
WMStat_Clear ("Lade Daten...")
On Error Resume Next
' Alte Eingabedateien löschen, falls noch vorhanden
' Kill tmp + "\~wm2in*.tmp"
Kill TraegerTmpFile("*")
saverequired = False
If DebugLevel > 0 Then Debug.Print "LoadTraeger "; DatenPfad + "\" + TraegerBez(traeger) + ".dir"; ":"; traegerindex
On Error Resume Next
'tif = FreeFile
'Open TiDatei For Random As #tif Len = Len(Ti)
ff = FreeFile
Open DatenPfad + "\" + TraegerBez(traeger) + ".dir" For Random As #ff Len = Len(TrV)
nf = LOF(ff) / Len(TrV) + 1
LoadTraeger = False
If nf > traegerindex Then
  Get #ff, traegerindex, TrV
  If (TrV.InfoByte And 1) = 1 Then
    LoadTraeger = True
    For i = 1 To TrV.Seiten
      sf$ = TraegerTmpFile(Format(i))
      Kill sf$
      f(i) = FreeFile
      Open sf$ For Random As #f(i) Len = Len(el)
    Next
    If TrV.ErsterDatensatz > 0 Then
      If DebugLevel > 1 Then Debug.Print "  Loading Tracks..."
      di = FreeFile
      Open DatenPfad + "\" + TraegerBez(traeger) + ".dat" For Random As #di Len = Len(TrDat)
      pp = TrV.ErsterDatensatz
      Do
        Get #di, pp, TrDat
        If DebugLevel > 2 Then Debug.Print "  Loading"; pp; " => Page:"; TrDat.Seite; ", Track:"; TrDat.track; ", Ti:"; TrDat.Titelnummer
        el.Titelnummer = TrDat.Titelnummer
        tret = GetWMTitle(TrDat.Titelnummer, Ti)
        'Get #tif, TrDat.Titelnummer, Ti
        If Ti.traeger <> traeger Then
            'Duplikat des Titels erstellen
            'Stop
            saverequired = True
            Ti.nummer = 0
            Ti.traeger = traeger
            Ti.Index = traegerindex
            Ti.Seite = TrDat.Seite
            Ti.track = TrDat.track
            el.Titelnummer = Ti.nummer
            tret = PutTitle(0, Ti)
            'Put #tif, Ti.nummer, Ti
            IndexTitel Ti.nummer, "", Trim$(Ti.interpret)
        End If
        
        el.interpret = Trim$(Ti.interpret)
        el.titel = Trim$(Ti.titel)
        el.Interpret_Titel = Trim$(Ti.interpret) + ": " + Trim$(Ti.titel)
        el.laenge = Ti.laenge
        ListingAnzeige = 0
        eingabe.LVersion.Caption = Lang(73)
        If Ti.version > 0 Then
          el.info = GetTab("version", Ti.version)
        Else
          el.info = "unbekannt"
        End If
        pp = TrDat.NaechsterDatensatz
        nf = LOF(f(TrDat.Seite)) / Len(el) + 1
        If TrDat.track > nf Then
          el.chk = 0
          For ll = nf To TrDat.track
            Put #f(TrDat.Seite), ll, el
          Next
        End If
        el.chk = 1
        Put #f(TrDat.Seite), TrDat.track, el
      Loop Until pp = 0
      Close #di
    Else
      If DebugLevel > 1 Then Debug.Print "No Tracks found, TrV.ErsterDatensatz=0"
    End If
    For i = 1 To TrV.Seiten
      Close #f(i)
    Next
  Else
    clearTrV TrV
  End If
End If
Close #ff
'Close #tif
WMStat_Clear ("")
If saverequired = True Then
    SaveTraeger
    MsgBox "Datenträger enthielt problematische Querverweise." + Chr(13) + Chr(10) + "Fehler wurde korrigiert", vbInformation, "WinMusik - Hinweis"
End If
saverequired = False
End Function

Sub Main()
Dim ts As String * 1024
Dim a As Long
Dim ret As Integer
Dim hKey As Long

Dim major As Long, minor As Long, release As Long
Dim exmajor As Long, exminor As Long



build = App.Revision
exmajor = CLng(left(LibVersion, InStr(LibVersion, ".") - 1))
exminor = CLng(Mid(LibVersion, InStr(LibVersion, ".") + 1))
a = WMLibVersion(major, minor, release)

If minor < exminor And major <= exmajor Then
    MsgBox "wm20.dll ist nicht mit dieser WinMusik-Version kompatibel!" + Chr(10) + Chr(13) + "Bitte Installieren Sie WinMusik neu um das Problem zu beheben.", vbCritical, "WinMusik"
    End
End If

myMutEx = CreateMutex(0&, 0&, "WinMusik2")

If DebugLevel > 0 Then Debug.Print "Initializing Programm"
tpx = Screen.TwipsPerPixelX
tpy = Screen.TwipsPerPixelY

TraegerBez(1) = "kassette"
TraegerBez(2) = "cd"
TraegerBez(3) = "daten"
TraegerBez(4) = "platte"
TraegerBez(5) = "video"
TraegerBez(6) = "sonstige"
TraegerBez(7) = "mpeg"
TraegerBez(8) = "band"



hKey = 0
res = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders", 0, KEY_READ, hKey)
If res <> 0 Then
    MsgBox "AppData-Pfad konnte nicht festgestellt werden", vbCritical, "WinMusik: Registry-Error"
    End
End If
AppData = Reg_GetKey(hKey, "AppData")
RegCloseKey (hKey)
hKey = 0
If AppData = "" Then
    MsgBox "AppData-Pfad konnte nicht festgestellt werden", vbCritical, "WinMusik: Registry-Error"
    End
End If


AppData = AppData + "\pfp\"

' Pfad anlegen, falls es ihn noch nicht gibt
CreatePath (AppData)

openini AppData + "winmusik2.ini"
If inierror() > 0 Then
  ' Erster Start von WinMusik
  RequireSetup = True
  start1.Show
Else
  
  
  ReadINI
  
  If RequireSetup = True Then
    ' Erster Start von WinMusik
    start1.Show
  End If
  
  If ShowSplashScreen = True Then CopyrightForm.Show
  INITLanguage SprachDatei
  
  If Shareware = False Then
    leiste1.Show
    Load gfx
  End If
  'mySQL = db_Connect(db.host, db.port, db.name, db.User, db.password)
  
  'ret = InitTitle(TiDatei)
  ret = InitTitle(TiDatei)
  UseDLL = False
  'If (ret = 0) Then UseDLL = True
  
  openini AppData + "winmusik2.ini"
  selectTopic "Sort"
  Sorted$ = GetINI$("Sortiert")
  If Sorted$ <> "true" Then
    RunIndexInterpreten
  End If
  Referenced$ = GetINI$("Referenziert")
  If Referenced$ <> "true" Then
    FindReferences
    'RunIndexInterpreten
  End If
  
End If
End Sub
Function PrinterRGB(rot, grün, blau) As Long
' Liefert in Abhängigkeit der Boolschen Variable "farbdruck" einen Farbwert oder
' einen entsprechenden Grauwert.
If Farbdruck = True Then
  PrinterRGB = RGB(rot, grün, blau)
Else
  h = (rot + rot + grün + blau + blau) / 5
  PrinterRGB = h * 65536 + h * 256& + h
End If
End Function

Sub PutFile(datei$, nummer As Long, text$)
Dim t As String * 256
t = Trim$(text$)
ff = FreeFile
If InStr(datei$, ".") = 0 Then datei$ = datei$ + ".dat"
Open DatenPfad + "\" + datei$ For Random As #ff Len = Len(t)
If nummer = 0 Then nummer = LOF(ff) / Len(t) + 1
Put #ff, nummer, t
Close #ff
End Sub

Sub PutTab(datei$, nummer As Long, text$)
Dim t As String * 80
ff = FreeFile
If InStr(datei$, ".") = 0 Then datei$ = datei$ + ".bez"
Open DatenPfad + "\" + datei$ For Random As #ff Len = 80
If nummer = 0 Then nummer = LOF(ff) / Len(t) + 1
nf = LOF(ff) / Len(t) + 1
If nummer > nf Then
  t = ""
  For i& = nf To nummer
    Put #ff, i&, t
  Next
End If
t = Trim$(text$)
Put #ff, nummer, t
Close #ff
End Sub
Sub PutTraegerTitel(nummer As Long, titel As TraegerTitel)
Dim nf As Long
On Error Resume Next
Err = 0
ff = FreeFile
Open DatenPfad + "\traeger.tit" For Random As #ff Len = Len(titel)
If nummer = 0 Then nummer = LOF(ff) / Len(titel) + 1
Put #ff, nummer, titel
Close #ff
End Sub


Sub RunIndexInterpreten()
Dim titel As Titeldatei
Dim tret As Integer
On Error Resume Next
Kill DatenPfad + "\sort*.*"
druckmsg.Show
'WMStat_Clear (Lang(189))
druckmsg.Caption = Lang(188)
druckmsg.textfeld = Lang(189)
druckmsg.queue.Visible = True
druckmsg.Image1.Visible = False
DoEvents
druckmsg.queue.min = 0
druckmsg.queue.max = 100
druckmsg.queue.value = 0
ff = FreeFile
Open TiDatei For Random As #ff Len = Len(titel)
pc = 0: az& = LOF(ff) / Len(titel)
Close #ff
For p& = 1 To az&
  'Get #ff, p&, titel
  tret = GetWMTitle(p&, titel)
  If Int(p& / az& * 100) <> pc Then
    pc = Int(p& / az& * 100)
    'WMStat_Queue (pc)
    druckmsg.queue.value = pc
    DoEvents
  End If
  If p& Mod 10 = 0 Then DoEvents
  If druckmsg.abbrechen.tag = "true" Then Exit For
  If DebugLevel > 0 Then
    Debug.Print "Indexing"; p&; ": "; Trim(titel.interpret)
  End If
  If (titel.InfoByte And 256) = 256 Then
    IndexTitel p&, "", Trim(titel.interpret)
  End If
Next
'Close #ff
If druckmsg.abbrechen.tag <> "true" Then
  openini AppData + "winmusik2.ini"
  selectTopic "Sort"
  putini "Sortiert=true"
  closeini
End If
Unload druckmsg
End Sub

Sub SaveTitel(titel As Titeldatei)
Dim oldti As Titeldatei
Dim tret As Integer
If titel.track = 0 Then Exit Sub
'ff = FreeFile
'Open TiDatei For Random As #ff Len = Len(titel)
If titel.nummer = 0 Then
  'titel.nummer = LOF(ff) / Len(titel) + 1
  deindex$ = ""
Else
  'Get #ff, titel.nummer, oldti
  tret = GetWMTitle(titel.nummer, oldti)
  deindex$ = Trim$(oldti.interpret)
End If
If Shareware = True And titel.nummer > SharewareLimit Then
  LangPar 1, Format$(SharewareLimit)
  MsgBox Lang(125), 48, Programm + " " + version
  titel.nummer = 0
Else
  'Put #ff, titel.nummer, titel
    If titel.nummer = 0 Then
        tret = PutTitle(titel.nummer, titel)
        If titel.nummer > 0 And titel.nummer < 5 Then
            RunIndexInterpreten
        Else
            NewIndex$ = Trim$(titel.interpret)
            IndexTitel titel.nummer, deindex$, NewIndex$
        End If
    Else
        tret = PutTitle(titel.nummer, titel)
        NewIndex$ = Trim$(titel.interpret)
        IndexTitel titel.nummer, deindex$, NewIndex$
    End If
  'tret = UpdateTitle(titel.nummer)
End If
'Close #ff


End Sub
Sub SaveTrack()
Dim tt As Integer
Dim e As Object
Set e = eingabe.e
Dim i As Integer
Dim high As Integer

Dim datei As String, filename As String
Dim version As String
Dim genre As String
Dim comment As String
Dim year As String
Dim newtitle As Boolean



On Error Resume Next
Ti = CurrentTi
If Ti.nummer = 0 Then
  eingabe.TiNummer.Caption = Lang(118) + ": " + Format$(Ti.nummer)
  Ti.InfoByte = 0
  Ti.traeger = traeger
  Ti.Index = traegerindex
  Ti.Seite = Seite
  Ti.track = track
  Ti.Soundfile = 0
  Ti.bemerkung = 0
  newtitle = True
Else
    newtitle = False
End If
Ti.interpret = Trim$(e(0).text)
Ti.titel = Trim$(e(1).text)
Ti.version = CLng(e(2).text)
Ti.musikart = CLng(e(4).text)
Ti.laenge = CSng(e(6).text)
Ti.bpm = CSng(e(7).text)
Ti.Erscheinungsjahr = CInt(e(8).text)
Ti.Aufnahmedatum = dat2long(e(9).text)
Ti.Quelle = CLng(e(10).text)
Ti.Aufnahmegeraet = CLng(e(12).text)
Ti.Bewertung = Trim$(e(14).text)
Ti.Qualitaet = Trim$(e(15).text)
a$ = Trim$(e(16).text)
If a$ <> "" Then
  If Exists(a$) = True Then
    PutFile "sndfile.dir", Ti.Soundfile, a$
  End If
End If
a$ = Trim$(e(17).text)
If a$ <> "" Then
  PutFile "bemerk.dat", Ti.bemerkung, a$
Else
    ' Wenn vorher eine Bemerkung drin stand, muss diese geloescht werden
    If Ti.bemerkung > 0 Then
        PutFile "bemerk.dat", Ti.bemerkung, ""
    End If
End If
i = 1 + 256: ' Gültiger Titel
If eingabe.check(0).value <> 0 Then i = i Or 4: 'Stereo check
If eingabe.check(1).value <> 0 Then i = i Or 8: 'Dolby check
If eingabe.check(2).value <> 0 Then i = i Or 32: 'vollständig check
If eingabe.check(3).value <> 0 Then i = i Or 64: 'echter Titel
If eingabe.check(4).value <> 0 Then i = i Or 16: 'Surround
If eingabe.check(5).value <> 0 Then i = i Or 128: 'Digital 5.1
Ti.InfoByte = i
SaveTitel Ti

If Ti.traeger = 7 Then
    datei = MpegPfad + "\" + Format(Int(traegerindex / 100), "00") + "\" + Format(traegerindex, "000")
    filename = datei + "\" + Format(Ti.track, "000") + ".mp3"
    If Exists(filename) = False Then
        filename = datei + "\" + Dir(datei + "\" + Format(Ti.track, "000") + "-*.mp3")
    End If
    If Exists(filename) = True Then
        genre = GetTab("musikart", Ti.musikart)
        version = GetTab("version", Ti.version)
        year = Format(Ti.Erscheinungsjahr)
        comment = ""
        If Ti.bemerkung > 0 Then
            comment = GetFile("bemerk.dat", Ti.bemerkung)
            comment = version + " - " + comment
        Else
            comment = version
        End If
        If newtitle = True Then
            OverwriteTagAsync filename, Ti.interpret, Ti.titel, version, comment, genre, year
        Else
            SaveTagAsync filename, Ti.interpret, Ti.titel, version, comment, genre, year
        End If
        
    End If
End If


tt = FreeFile
Open TraegerTmpFile(Format(Seite)) For Random As #tt Len = Len(el)
high = LOF(tt) / Len(el)
If high < track Then
  el.chk = 0
  For i = high + 1 To track
    Put #tt, i, el
  Next
End If
el.chk = 1
el.Titelnummer = Ti.nummer
el.interpret = Trim$(Ti.interpret)
el.titel = Trim$(Ti.titel)
el.Interpret_Titel = Trim$(Ti.interpret) + ": " + Trim$(Ti.titel)
el.laenge = Ti.laenge
Select Case ListingAnzeige
  Case 0: el.info = GetTab("version", Ti.version)
  Case 1: el.info = GetTab("musikart", Ti.musikart)
  Case 2: el.info = GetTab("quelle", Ti.Quelle)
  Case 3: el.info = GetTab("aufnahme", Ti.Aufnahmegeraet)
  Case 4: el.info = Format$(Ti.bpm)
  Case 5: el.info = Trim$(Ti.Bewertung)
End Select
Put #tt, track, el
Close #tt

saverequired = True

SaveTraeger

'Eingabefelder für Kopiermöglichkeit zwischenspeichern
For i = 0 To 17
  last_e(i) = Trim$(e(i).text)
Next
CurrentTi.traeger = 0: EingabePos = -1
eingabe.Listfeld.Refresh
eingabe.track_e.text = Format$(track + 1)
eingabe.track_e.SetFocus



End Sub

Sub SaveTraeger()
Dim nf As Long
Dim TrVleer As TraegerVerzeichnis
Dim TrDat2 As TraegerDatensatz
Dim pp As Long:     ' Aktueller Titel in ***.DIR
Dim pplast As Long: ' Letzter Titel
Dim ppnext As Long: ' Nächster Titel
Dim s As Integer: ' Seite
Dim ff As Integer: ' Dateihandle TrV
Dim tif As Integer: ' Dateihandle Ti
Dim ss As Integer: ' Dateihandle TMP-Seite (el)
Dim tr As Integer: ' Dateihandle Trägerdatensatz
Dim fr As Integer: ' Dateihandle NextFreePosition
Dim i As Long
Dim tret As Integer
On Error Resume Next

If saverequired = True Then
  'tif = FreeFile
  'Open TiDatei For Random As #tif Len = Len(Ti)
  If DebugLevel > 0 Then Debug.Print "Saveing " + TraegerBez(traeger); traegerindex
  ff = FreeFile
  Open DatenPfad + "\" + TraegerBez(traeger) + ".dir" For Random As #ff Len = Len(TrV)
  nf = LOF(ff) / Len(TrV) + 1
  ' Wenn die Nummer des zu speichernden Trägers höher
  ' ist, als der nächste freie Datensatz, dann müssen
  ' die Datensätze dazwischen mit 0-Bytes gefüllt werden.
  If traegerindex > nf Then
    If DebugLevel > 0 Then Debug.Print "  filling empty space with 0"
    TrVleer.InfoByte = 0
    TrVleer.ErsterDatensatz = 0
    For i& = nf To traegerindex
      Put #ff, i&, TrVleer
    Next
  End If
  Put #ff, traegerindex, TrV
  
  'Hier müssen die Titel gespeichert werden:
  pp = TrV.ErsterDatensatz
  pplast = 0: ppnext = 0
  tr = FreeFile
  Open DatenPfad + "\" + TraegerBez(traeger) + ".dat" For Random As #tr Len = Len(TrDat)
  fr = FreeFile
  Open DatenPfad + "\" + TraegerBez(traeger) + ".fre" For Random As #fr Len = 4
  TrV.Tracks = 0: TrV.Singles = 0: TrV.Mixe = 0
  TrV.Aufnahmestart = 99999999: TrV.Aufnahmeende = 0
  For s = 1 To TrV.Seiten
    ss = FreeFile
    Open TraegerTmpFile(Format(s)) For Random As #ss Len = Len(el)
    If DebugLevel > 0 Then Debug.Print "  Saveing Page:"; s
    For i = 1 To LOF(ss) / Len(el)
      Get #ss, i, el
      If (el.chk And 1) = 1 Then
        TrV.Tracks = TrV.Tracks + 1
        'Get #tif, el.Titelnummer, Ti
        tret = GetWMTitle(el.Titelnummer, Ti)
        If Ti.version > 1 Then TrV.Mixe = TrV.Mixe + 1 Else TrV.Singles = TrV.Singles + 1
        If Ti.Aufnahmedatum < TrV.Aufnahmestart Then TrV.Aufnahmestart = Ti.Aufnahmedatum
        If Ti.Aufnahmedatum > TrV.Aufnahmeende Then TrV.Aufnahmeende = Ti.Aufnahmedatum
        If DebugLevel > 0 Then Debug.Print "  Saveing Track:"; i
        If pp = 0 Then
          If DebugLevel > 2 Then Debug.Print "searching for new entry"
          For f& = LOF(fr) / 4 To 1 Step -1
            Get #fr, f&, ppnext
            If ppnext = 0 Then
              If f& < LOF(fr) / 4 Then Put #fr, f& + 1, ppnext
              Exit For
            Else
              pp = ppnext
            End If
          Next
          If pp = 0 Then
            pp = LOF(tr) / Len(TrDat) + 1
            If DebugLevel > 1 Then Debug.Print "    New Entry:"; pp
          Else
            If DebugLevel > 1 Then Debug.Print "    Entry from Freefile:"; pp
          End If
          ppnext = 0
        Else
          If DebugLevel > 1 Then Debug.Print "    Overwriting old Entry:"; pp
          Get #tr, pp, TrDat
          ppnext = TrDat.NaechsterDatensatz
        End If
        If TrV.ErsterDatensatz = 0 Then TrV.ErsterDatensatz = pp
        If pplast > 0 Then
          Get #tr, pplast, TrDat2
          TrDat2.NaechsterDatensatz = pp
          Put #tr, pplast, TrDat2
        End If
        TrDat.LetzterDatensatz = pplast
        TrDat.NaechsterDatensatz = 0
        TrDat.traegerindex = traeger
        TrDat.Titelnummer = el.Titelnummer
        TrDat.Seite = s
        TrDat.track = i
        Put #tr, pp, TrDat
        pplast = pp
        pp = ppnext
      End If
    Next
    Close #ss
    'If DebugLevel > 0 Then Debug.Print "    Removing Temp-File "; tmp + "\~wm2in" + Format$(s) + ".tmp"
    'Kill tmp + "\~wm2in" + Format$(s) + ".tmp"
  Next
  Close #fr
  Close #tr
  Put #ff, traegerindex, TrV
  If DebugLevel > 0 Then Debug.Print "First Entry="; TrV.ErsterDatensatz
  Close #ff
  saverequired = False
  If DebugLevel > 0 Then Debug.Print "All Tracks saved."
  'Close tif
Else
  If DebugLevel > 0 Then Debug.Print "Nothing to save."
  For s = 1 To TrV.Seiten
    'Kill tmp + "\~wm2in" + Format$(s) + ".tmp"
  Next
End If
End Sub
Sub SetEingabePosition()
On Error GoTo ErrHandler
Select Case EingabePos
  Case -3: eingabe.index_e.SetFocus
  Case -2: eingabe.seite_e.SetFocus
  Case -1: eingabe.track_e.SetFocus
  Case 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17
           eingabe.e(EingabePos).SetFocus
End Select
Exit Sub
ErrHandler:

End Sub


Function SplitInterpret(interpret As String) As String
Static splitstring As String
Static SplittingWords As String
Static IgnoringWords As String

If interpret <> "" Then splitstring = interpret
If splitstring <> "" Then
  If SplittingWords = "" Or IgnoringWords = "" Then
    openini AppData + "winmusik2.ini"
    selectTopic "Sort"
    SplittingWords = LCase(GetINI("SplittingWords"))
    If SplittingWords = "" Then
      SplittingWords = LCase(Lang(186))
      putini "SplittingWords=" + SplittingWords
    End If
    IgnoringWords = LCase(GetINI("IgnoringWords"))
    If IgnoringWords = "" Then
      IgnoringWords = LCase(Lang(187))
      putini "IgnoringWords=" + IgnoringWords
    End If
    closeini
  End If
  ' Zuerst nach einem Trennzeichen suchen
  t = Len(splitstring) + 1: a$ = SplittingWords
  Do
    s = InStr(a$, " ")
    If s > 0 Then
      w$ = " " + left$(a$, s - 1) + " ": a$ = Mid$(a$, s + 1)
      If Trim$(w$) = "," Then w$ = ", "
    Else
      w$ = a$: a$ = ""
    End If
    p = InStr(LCase(splitstring), w$)
    If p > 0 And p < t Then t = p
  Loop Until a$ = ""
  If t < Len(splitstring) + 1 Then
    i$ = left$(splitstring, t - 1)
    splitstring = Mid$(splitstring, t + 1)
    t = InStr(splitstring, " ")
    If t > 0 Then splitstring = Mid$(splitstring, t + 1)
  Else
    i$ = splitstring: splitstring = ""
  End If
  ' Okay, jetzt haben wir einen einzelnen Interpreten, jetzt
  ' muessen wir nurnoch überflüssige Worte am Anfang entfernen
  Do
    s = InStr(i$, " ")
    If s > 0 Then
      w$ = LCase(left$(i$, s - 1))
      If InStr(IgnoringWords, w$) > 0 Then
        i$ = Mid$(i$, s + 1)
      Else
        s = 0
      End If
    End If
  Loop Until s = 0
Else
  i$ = ""
End If
SplitInterpret = i$
End Function

Sub ZeigeTabelle(TabName$, vorgabe$, backfeld$)
TabelleName = TabName$
TabelleVorgabe = LCase$(Trim$(vorgabe$))
If Val(TabelleVorgabe) > 0 Then TabelleVorgabe = ""
TabelleFuellFeld = backfeld$
tabelle.Show
End Sub



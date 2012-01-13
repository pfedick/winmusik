Attribute VB_Name = "MPEG"
DefInt A-Z

Type MP3HEADER
    status As Long
    framesize As Long
    filelength As Long
    length As Long
    mslength As Long
    bitrate As Long
    frequency As Long
    datastart As Long
    dataend As Long
    datasize As Long
    version As Byte
    layer As Byte
    protection As Byte
    bitrateindex As Byte
    frequencyindex As Byte
    padding As Byte
    privat As Byte
    channelmode As Byte
    modeextension As Byte
    copyright As Byte
    original As Byte
    emphasis As Byte
End Type

Type id3tag
  tag As String * 3
  SongName As String * 30
  artist As String * 30
  album As String * 30
  year As String * 4
  comment As String * 30
  'track As Byte
  genre As Byte
End Type

Type id3v2tag
  SongName As String * 256
  artist As String * 256
  album As String * 256
  year As String * 4
  comment As String * 256
  genre As String * 256
  version As String * 256
End Type

Type WmtHeader
  Tracks As Integer
  TrackLength As Integer
  TraegerTitel As Long
  traegertyp As Integer
  traegerindex As Long
  tabstart As Long
End Type
  
Type WmtFile
  Ti As Titeldatei
  version As String * 80
  musikart As String * 80
End Type

Type MP3impdataFile
  Ti As Titeldatei
  status As Integer
  filename As String
End Type
' status:   Bit 0:    Track war bereits vorhanden
'           Bit 1:

Global MpegPfad As String
'Global id3 As id3tag

Declare Function GetMP3Info Lib "wm20" (ByVal filename As String, ByRef mp3 As MP3HEADER, ByRef id3 As id3tag, ByRef v2 As id3v2tag) As Integer
Declare Function ChopID3Tags Lib "wm20" (ByVal filename As String) As Integer
Declare Function RepairMp3File Lib "wm20" (ByVal filename As String) As Integer
Declare Function UpdateID3Tags Lib "wm20" (ByVal filename As String, ByVal artist As String, ByVal title As String, ByVal version As String, ByVal comment As String, ByVal genre As String, ByVal year As String)
Declare Function SaveTagAsync Lib "wm20" (ByVal filename As String, ByVal artist As String, ByVal title As String, ByVal version As String, ByVal comment As String, ByVal genre As String, ByVal year As String)
Declare Function OverwriteTagAsync Lib "wm20" (ByVal filename As String, ByVal artist As String, ByVal title As String, ByVal version As String, ByVal comment As String, ByVal genre As String, ByVal year As String)






Function GetMp3FileName(mytraegerindex As Long, mytrack As Long) As String
Dim oldfile As String, newfile As String
GetMp3FileName = ""
a$ = MpegPfad + "\" + Format(Int(mytraegerindex / 100), "00") + "\" + Format(mytraegerindex, "000") + "\" + Format(mytrack, "000") + ".mp3"
If Exists(a$) = False Then
    b$ = MpegPfad + "\" + Format(Int(mytraegerindex / 100), "00") + "\" + Format(mytraegerindex, "000") + "\" + Format(mytrack, "000") + "-*.mp3"
    a$ = Dir(b$)
    If (a$ <> "") Then a$ = MpegPfad + "\" + Format(Int(mytraegerindex / 100), "00") + "\" + Format(mytraegerindex, "000") + "\" + a$
    If Exists(a$) = False Then
        ' Gibt es Dateien in diesem Verzeichnis, die noch keinen Index haben?
        b$ = MpegPfad + "\" + Format(Int(mytraegerindex / 100), "00") + "\" + Format(mytraegerindex, "000") + "\*.mp3"
        a$ = Dir(b$)
        While a$ > ""
            Dim match As Boolean
            oldfile = MpegPfad + "\" + Format(Int(mytraegerindex / 100), "00") + "\" + Format(mytraegerindex, "000") + "\" + a$
            a$ = LCase(a$)
            match = False
            ' Fängt der Dateiname mit [0-9]{3}.mp3 oder [0-9]{3}-.+.mp3 an?
            If right(a$, 4) = ".mp3" Then
                c$ = Mid(a$, 1, 3)
                If c$ > "000" And c$ <= "999" Then
                    If Mid(a$, 4) = ".mp3" Then
                        match = True
                    ElseIf Mid(a$, 4, 1) = "-" Then
                        match = True
                    End If
                End If
            End If
            If match = False Then
                ' Datei umbenennen
                newfile = MpegPfad + "\" + Format(Int(mytraegerindex / 100), "00") + "\" + Format(mytraegerindex, "000") + "\" + Format(mytrack, "000") + "-" + a$
                Name oldfile As newfile
                a$ = newfile
                GetMp3FileName = a$
                Exit Function
            End If
            a$ = Dir()
        Wend
        
    End If
End If

GetMp3FileName = a$
End Function
Function GetMp3Playlist(mytraegerindex As Long) As String
GetMp3Playlist = ""
a$ = MpegPfad + "\" + Format(Int(mytraegerindex / 100), "00") + "\" + Format(mytraegerindex, "000") + "\000index.m3u"
If Exists(a$) = True Then GetMp3Playlist = a$

End Function


Sub GetMP3TrackLength()
Dim myel As EingabeListing
Dim id3 As id3tag
Dim id3v2 As id3v2tag
Dim mp3 As MP3HEADER
Dim ret As Long
Dim titel As Titeldatei
Dim high As Long
Dim i As Long
On Error Resume Next

'status.Show
WMStat_Clear (Lang(258))
'status.Caption = Lang(257)
'status.text.text = Lang(258)

tt = FreeFile
Open TraegerTmpFile(Format(Seite)) For Random As #tt Len = Len(myel)
high = LOF(tt) / Len(myel)
WMStat_MinMax 1, high
'status.queue.min = 1
'status.queue.max = high
WMStat_Queue 1

DoEvents
For i = 1 To high
'For i = 47 To 47
   WMStat_Queue i
   DoEvents
   Get #tt, i, myel
   If myel.chk = 1 Then
     a$ = GetMp3FileName(traegerindex, i)
     If a$ <> "" Then
        'Debug.Print a$
        ret = GetMP3Info(a$, mp3, id3, id3v2)
        If ret = 0 And (mp3.status And 1) = 1 Then
          myel.laenge = CSng(Format(Int(mp3.length / 60), "0") + "," + Format(mp3.length Mod 60, "00"))
          GetTitel titel, myel.Titelnummer
          titel.laenge = myel.laenge
          SaveTitel titel
        End If
     End If
     Put #tt, i, myel
   End If
Next
Close #tt
'Unload status
WMStat_Text ("")

eingabe.Listfeld.Refresh
End Sub
Sub Debug_PrintMP3Header(mp3 As MP3HEADER)
Debug.Print "Status:         "; mp3.status
Debug.Print "Framesize:      "; mp3.framesize
Debug.Print "FileLength:     "; mp3.filelength
Debug.Print "Length:         "; mp3.length
Debug.Print "MsLength:       "; mp3.mslength
Debug.Print "Bitrate:        "; mp3.bitrate
Debug.Print "Frequency:      "; mp3.frequency
Debug.Print "Version:        "; mp3.version
Debug.Print "Layer:          "; mp3.layer
Debug.Print "Protection:     "; mp3.protection
Debug.Print "Bitrateindex:   "; mp3.bitrateindex
Debug.Print "Frequencyindex: "; mp3.frequencyindex
Debug.Print "Padding:        "; mp3.padding
Debug.Print "Privat:         "; mp3.privat
Debug.Print "Channelmode:    "; mp3.channelmode
Debug.Print "Modeextension:  "; mp3.modeextension
Debug.Print "Copyright:      "; mp3.copyright
Debug.Print "Original:       "; mp3.original
Debug.Print "Emphasis:       "; mp3.emphasis

End Sub


Sub LoadID3Tag(datei As String, tag As id3tag)
Dim ff As Integer
Dim p As Long
Dim t As id3tag

On Error Resume Next
ff = FreeFile
tag.genre = 0
tag.tag = "---"
Open datei For Binary Access Read As #ff
If Err = 0 Then
  p = LOF(ff) - 128: ' Die Länge feststellen und 128 Byte abziehen
  If p > 0 Then
    Get #ff, p, t:   ' Feststellen, ob es einen ID3-Tag gibt
    If t.tag = "TAG" Then
      Get #ff, p, tag
    Else
      Get #ff, p + 1, t
      If t.tag = "TAG" Then
        Get #ff, p + 1, tag
      End If
    End If
  End If
  Close #ff
End If

End Sub

Sub makeid3tags()
Dim datei As String, filename As String
Dim tiid3 As Titeldatei
Dim id3 As id3tag
Dim tret As Integer
Dim version As String
Dim genre As String
Dim comment As String
Dim year As String

datei = InputBox(Lang(218), Lang(219), MpegPfad + "\" + Format(Int(mytraegerindex / 100), "00") + "\" + Format(traegerindex, "000"))
If datei <> "" Then
  Do
    If right$(datei, 1) = "\" Then datei = left$(datei, Len(datei) - 1) Else Exit Do
  Loop
  'MpegPfad = datei
  'tt = FreeFile
  'Open TiDatei For Random Access Read As #tt Len = Len(Ti)
  
  ff = FreeFile
  Open TraegerTmpFile(Format(Seite)) For Random As #ff Len = Len(el)
  If Err = 0 Then
    zz = LOF(ff) / Len(el)
    If zz > 0 Then
      status.Show
      status.Caption = Lang(219)
      LangPar 1, datei
      status.text = Lang(220)
      status.queue.min = 1
      status.queue.max = zz
      status.queue.value = 1
      DoEvents
      For i = 1 To zz
        Get #ff, i, el
        status.queue.value = i
        DoEvents
        'Get #tt, el.Titelnummer, tiid3
        tret = GetWMTitle(el.Titelnummer, tiid3)
        'Ti2ID3Tag tiid3, id3
        'Stop
        filename = datei + "\" + Format(i, "000") + ".mp3"
        If Exists(filename) = False Then
          filename = datei + "\" + Dir(datei + "\" + Format(i, "000") + "-*.mp3")
        End If
        genre = GetTab("musikart", tiid3.musikart)
        version = GetTab("version", tiid3.version)
        year = Format(tiid3.Erscheinungsjahr)
        comment = ""
        If tiid3.bemerkung > 0 Then
            comment = GetFile("bemerk.dat", tiid3.bemerkung)
            comment = version + " - " + comment
        Else
            comment = version
        End If
        
        
        'SaveID3TAG filename, id3
        UpdateID3Tags filename, tiid3.interpret, tiid3.titel, version, comment, genre, year
        
        If status.abbrechen.tag = "stopit" Then Exit For
        
      Next
      Unload status
      eingabe.SetFocus
    End If
    Close #ff
  End If
  'Close #tt
End If
End Sub

Sub makempegindex()
Dim datei As String
Dim tiid3 As Titeldatei
Dim pf As PFPHeader
Dim wmf As WmtFile
Dim wmh As WmtHeader
Dim tret As Integer
On Error Resume Next

datei = InputBox(Lang(218), Lang(221), MpegPfad + "\" + Format(Int(traegerindex / 100), "00") + "\" + Format(traegerindex, "000"))
If datei <> "" Then
  Do
    If right$(datei, 1) = "\" Then datei = left$(datei, Len(datei) - 1) Else Exit Do
  Loop
  m3u = FreeFile
  Kill datei + "\000index.*": Err = 0
  Open datei + "\000index.m3u" For Binary As #m3u
  If Err = 0 Then
    pls = FreeFile
    Open datei + "\000index.pls" For Binary As #pls
    'b4s = FreeFile
    'Open datei + "\000index.b4s" For Binary As #b4s
    txt = FreeFile
    Open datei + "\000index.txt" For Binary As #txt
    'MpegPfad = datei
    wmt = FreeFile
    Open datei + "\000index.wmt" For Binary As #wmt

    'tt = FreeFile
    'Open TiDatei For Random Access Read As #tt Len = Len(Ti)
  
    ff = FreeFile
    Open TraegerTmpFile(Format(Seite)) For Random As #ff Len = Len(el)
    If Err = 0 Then
      a$ = "[Playlist]" + Chr$(13) + Chr$(10)
      Put #pls, , a$
      a$ = UnUML(Lang(223)) + Str$(traegerindex) + Chr$(13) + Chr$(10) + Chr$(13) + Chr$(10)
      ttt$ = ""
      If Trim(TrTi.Titel1 <> "") Then
        ttt$ = UnUML(Trim(TrTi.Titel1))
        a$ = a$ + UnUML(Trim(TrTi.Titel1)) + Chr$(13) + Chr$(10)
        If Trim(TrTi.Titel2 <> "") Then
          ttt$ = ttt$ + ", " + UnUML(Trim(TrTi.Titel2))
          a$ = a$ + UnUML(Trim(TrTi.Titel2)) + Chr$(13) + Chr$(10)
        End If
        a$ = a$ + Chr$(13) + Chr$(10)
      End If
      a$ = a$ + Fill(UnUML(Lang(65)), 5) + "|" + Fill(UnUML(Lang(71)) + ": " + UnUML(Lang(72)) + " (" + UnUML(Lang(73)) + ")", 65)
      a$ = a$ + "|" + Fill(UnUML(Lang(75)), 6) + Chr$(13) + Chr$(10)
      trenn$ = "-----+-----------------------------------------------------------------+------" + Chr$(13) + Chr$(10)
      Put #txt, , a$
      Put #txt, , trenn$
      INITPFP pf
      pf.Erstelldatum = dat2long(Datum())
      pf.Editdatum = pf.Erstelldatum
      pf.Erstellzeit = time2long(Time())
      pf.Editzeit = pf.Erstellzeit
      pf.FileID = "WM2i"
      pf.MainVersion = Chr$(2)
      pf.SubVersion = Chr$(0)
      zz = LOF(ff) / Len(el)
      a1$ = Programm + " " + version + Chr$(0)
      a2$ = copyright + Chr$(0)
      wmh.Tracks = zz
      wmh.TrackLength = Len(wmf)
      wmh.TraegerTitel = Len(wmh) + Len(a1$) + Len(a2$)
      wmh.traegertyp = traeger
      wmh.traegerindex = traegerindex
      ttt$ = ttt$ + Chr$(0)
      wmh.tabstart = wmh.TraegerTitel + Len(ttt$)
      pf.Pointer_Name = 38 + Len(wmh)
      pf.Pointer_Autor = pf.Pointer_Name + Len(a1$)
      Put #wmt, , pf
      Put #wmt, , wmh
      Put #wmt, , a1$: Put #wmt, , a2$: Put #wmt, , ttt$
      
      If zz > 0 Then
        status.Show
        status.Caption = Lang(221)
        LangPar 1, datei
        status.text = Lang(222)
        status.queue.min = 1
        status.queue.max = zz
        status.queue.value = 1
        DoEvents
        wmversion$ = Format(App.major) + "." + Format(App.minor) + "." + Format(App.Revision)
        a$ = "#EXTM3U" + Chr(13) + Chr(10)
        Put #m3u, , a$
        
        'a$ = "<?xml version=" + Chr(34) + "1.0" + Chr(34) + " encoding='UTF-8' standalone=" + Chr(34) + "yes" + Chr(34) + "?>" + Chr(13) + Chr(10)
        'Put #b4s, , a$
        'a$ = "<WinampXML>" + Chr(13) + Chr(10) + "<!-- Generated by: " + Programm + " " + wmversion$ + " -->" + Chr(13) + Chr(10)
        'Put #b4s, , a$
        'a$ = "<playlist num_entries=" + Chr(34) + Format(zz) + Chr(34) + " label=" + Chr(34) + "Playlist 001" + Chr(34) + ">" + Chr(13) + Chr(10)
        'Put #b4s, , a$

        For i = 1 To zz
          Get #ff, i, el
          status.queue.value = i
          DoEvents
          'Get #tt, el.Titelnummer, tiid3
          tret = GetWMTitle(el.Titelnummer, tiid3)
          filename$ = Format(i, "000") + ".mp3"
          ' Gibt es diese Datei?
          If Exists(datei + "\" + filename$) = False Then
            b$ = Dir(datei + "\" + Format(i, "000") + "-*.mp3")
            If (b$ <> "") Then filename$ = b$
          End If
          v$ = GetTab("version", tiid3.version)
          wmf.Ti = tiid3
          wmf.version = v$
          wmf.musikart = GetTab("musikart", tiid3.musikart)
          
          tiinve$ = Trim(tiid3.interpret) + ": " + Trim(tiid3.titel) + " (" + Trim(wmf.version) + ")"
          
          ' M3U Extended schreiben
          a$ = "#EXTINF:" + Format(Min2Sec(tiid3.laenge)) + "," + tiinve$ + Chr(13) + Chr(10)
          Put #m3u, , a$
          a$ = filename$ + Chr(13) + Chr(10)
          Put #m3u, , a$
          
          ' PLS Extended schreiben
          a$ = "File" + Format$(i) + "=" + filename$ + Chr$(13) + Chr$(10)
          Put #pls, , a$
          a$ = "Title" + Format$(i) + "=" + tiinve$ + Chr(13) + Chr(10)
          Put #pls, , a$
          a$ = "Length" + Format(i) + "=" + Format(Min2Sec(tiid3.laenge)) + Chr(13) + Chr(10)
          Put #pls, , a$

          'a$ = "   <entry Playstring=" + Chr(34) + "file:" + MakeXMLString(datei + "\" + filename$) + Chr(34) + ">" + Chr(13) + Chr(10)
          'Put #b4s, , a$
          'a$ = "      <Name>" + MakeXMLString(tiinve$) + "</Name>" + Chr(13) + Chr(10)
          'Put #b4s, , a$
          'a$ = "      <Length>" + Format(Min2Sec(tiid3.laenge)) + "000</Length>" + Chr(13) + Chr(10) + "   </entry>" + Chr(13) + Chr(10)
          'Put #b4s, , a$
          
          

          Put #wmt, , wmf
          b$ = UnUML(Trim$(tiid3.interpret) + ": " + Trim$(tiid3.titel) + " (" + Trim$(v$) + ")")
          
          a$ = Fill(Format(i), 5) + "|"
          If Len(b$) > 65 Then
            a$ = a$ + left$(b$, 65): b$ = Mid$(b$, 66)
          Else
            a$ = a$ + Fill(b$, 65): b$ = ""
          End If
          a$ = a$ + "| " + Format(tiid3.laenge, "00.00") + Chr$(13) + Chr$(10)
          Put #txt, , a$
          Do While b$ <> ""
            a$ = "     |"
            If Len(b$) > 65 Then
              a$ = a$ + left$(b$, 65): b$ = Mid$(b$, 66)
            Else
              a$ = a$ + Fill(b$, 65): b$ = ""
            End If
            a$ = a$ + "|" + Chr$(13) + Chr$(10)
            Put #txt, , a$
          Loop
          Put #txt, , trenn$
          
          If status.abbrechen.tag = "stopit" Then Exit For
        Next
        a$ = "NumberOfEntries=" + Format$(zz) + Chr$(13) + Chr$(10)
        Put #pls, , a$
        a$ = "Version=2" + Chr(13) + Chr(10)
        Put #pls, , a$
        
        'a$ = "</playlist>" + Chr(13) + Chr(10) + "</WinampXML>" + Chr(13) + Chr(10)
        'Put #b4s, , a$
        
        Unload status
        eingabe.SetFocus
      End If
      Close #ff
    End If
    'Close #tt
    Close #m3u, #pls, #txt, #wmt
    'Close #b4s
  End If
End If

End Sub


Sub SaveID3TAG(datei As String, tag As id3tag)
Dim ff As Integer
Dim p As Long
Dim t As id3tag
Dim alwaystag As Boolean
alwaystag = True
On Error Resume Next
If Exists(datei) = True Then
  ff = FreeFile
  Open datei For Binary Access Read As #ff
  If Err = 0 Then
    p = LOF(ff) - 127: ' Die Länge feststellen und 128 Byte abziehen
    If p > 0 Then
      Get #ff, p, t:   ' Feststellen, ob es schon einen ID3-Tag gibt
      If t.tag <> "TAG" Then
        p = p + 128:   ' Nein? Dann TAG am Ende hinzufügen
      End If
      Close #ff
      If alwaystag = True Or t.tag <> "TAG" Or t.SongName <> tag.SongName _
        Or t.artist <> tag.artist Or t.album <> tag.album Or t.year <> tag.year _
        Or t.comment <> tag.comment Or t.genre <> tag.genre Then
            Open datei For Binary As #ff
            Put #ff, p, tag
            Close #ff
      End If
    End If
  End If
End If
End Sub



Function GetGenreString() As String
Dim genre As String
genre = "Classic Rock,Country,Dance,Disco,Funk,Grunge,Hip-Hop,"
genre = genre + "Jazz,Metal,New Age,Oldies,Other,Pop,R&B,Rap,"
genre = genre + "Reggae,Rock,Techno,Industrial,Alternative,"
genre = genre + "Ska,Death Metal,Pranks,Soundtrack,Euro-Techno,"
genre = genre + "Ambient,Trip-Hop,Vocal,Jazz+Funk,Fusion,Trance,"
genre = genre + "Classical,Instrumental,Acid,House,Game,Sound Clip,"
genre = genre + "Gospel,Noise,AlternRock,Bass,Soul,Punk,Space,"
genre = genre + "Meditative,Instrumental Pop,Instrumental Rock,"
genre = genre + "Ethnic,Gothic,Darkwave,Techno-Industrial,Electronic,"
genre = genre + "Pop-Folk,Eurodance,Dream,Southern Rock,Comedy,Cult,"
genre = genre + "Gangsta,Top 40,Christian Rap,Pop/Funk,Jungle,"
genre = genre + "Native American,Cabaret,New Wave,Psychadelic,"
genre = genre + "Rave,Showtunes,Trailer,Lo-Fi,Tribal,Acid Punk,"
genre = genre + "Acid Jazz,Polka,Retro,Musical,Rock & Roll,"
genre = genre + "Hard Rock,"
' Erweiterungen von WinAmp
genre = genre + "Folk,Folk-Rock,National Folk,Swing,Fast Fusion,"
genre = genre + "Bebob,Latin,Revival,Celtic,Bluegrass,Avantgarde,Gothic Rock,"
genre = genre + "Progressive Rock,Psychedelic Rock, Symphonic Rock,Slow Rock,"
genre = genre + "Big Band,Chorus,Easy Listening,Acoustic,Humor,Speech,"
genre = genre + "Chanson,Opera,Chamber Music,Sonata,Symphony,Booty Brass,"
genre = genre + "Primus,Porn Groove,Satire,Slow Jam,Club,Tango,Samba,"
genre = genre + "Folklore,Ballad,Poweer Ballad,Rhytmic Soul,Freestyle,"
genre = genre + "Duet,Punk Rock,Drum Solo,A Capela,Euro-House,Dance Hall,"

GetGenreString = genre
End Function
Function GetGenre(nr As Long) As String
Dim genre As String
genre = GetGenreString()
p = 1
a$ = ""
For i = 1 To nr
  k = InStr(p, genre, ",")
  If k > 0 Then
    a$ = Mid$(genre, p, k - p)
    p = k + 1
  End If

Next
GetGenre = a$
End Function


Sub Ti2ID3Tag(titel As Titeldatei, tag As id3tag)
Dim v As String
Dim g As Byte
Dim p As Integer, k As Integer
Dim genre As String
genre = GetGenreString()

tag.tag = "TAG"
tag.SongName = Fill(titel.titel, 30)
tag.artist = Fill(titel.interpret, 30)
tag.album = space(30)
tag.year = Fill(Format$(titel.Erscheinungsjahr), 4)
v = GetTab("version", titel.version)
tag.comment = Fill(v, 29)
'tag.track = titel.track
'tag.track = 0
If Len(Trim(tag.SongName)) + 5 <= 30 Then
    tag.SongName = Fill(Trim(titel.titel) + " (" + Trim(v) + ")", 30)
End If
v = GetTab("musikart", titel.musikart)
tag.genre = 12
p = 1
genre = LCase(genre)
v = LCase(v)
g = 0
Do
  g = g + 1
  k = InStr(p, genre, ",")
  If k > 0 Then
    If Mid$(genre, p, k - p) = v Then tag.genre = g: Exit Do
  End If
  p = k + 1
Loop Until k = 0
End Sub

Function MakeXMLString(text As String) As String
Dim l As Long, i As Long
Dim z As String
l = Len(text)
z = ""
For i = 1 To l
    If Asc(Mid(text, i, 1)) < 128 Then
        z = z + Mid(text, i, 1)
    Else
        z = z + "&#x" + LCase(Hex(Asc(Mid(text, i, 1)))) + ";"
    End If
Next
MakeXMLString = z
End Function

Sub choptags()
Dim myel As EingabeListing
Dim id3 As id3tag
Dim mp3 As MP3HEADER
Dim ret As Long
Dim titel As Titeldatei
Dim high As Long
Dim i As Long
On Error Resume Next

'status.Show
WMStat_Clear (Lang(258))
DoEvents
'status.Caption = Lang(257)
'status.text.text = Lang(258)

tt = FreeFile
Open TraegerTmpFile(Format(Seite)) For Random As #tt Len = Len(myel)
high = LOF(tt) / Len(myel)
WMStat_MinMax 1, high
'status.queue.min = 1
'status.queue.max = high
WMStat_Queue 1

DoEvents
For i = 1 To high
   WMStat_Queue i
   DoEvents
   Get #tt, i, myel
   If myel.chk = 1 Then
     a$ = GetMp3FileName(traegerindex, i)
     If a$ <> "" Then
        ChopID3Tags a$
     End If
     Put #tt, i, myel
   End If
Next
Close #tt
'Unload status
WMStat_Text ("")
eingabe.Listfeld.Refresh

End Sub

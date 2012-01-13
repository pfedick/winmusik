Attribute VB_Name = "Utilities"
' BASIC-UTILITIES     (c) by Patrick Fedick in 1996
' ===============
' Version 2.0 vom 02.05.1996
'
' ToDo:
' Die Datumsauswertung muß noch für die internationale
' Verwendung angepaßt werden.

DefInt A-Z
Dim datform As String
Dim er As Long
#If Win32 Then
  Declare Function sndPlaySound Lib "winmm.dll" Alias "sndPlaySoundA" (ByVal lpszSoundName As String, ByVal uflags As Long) As Long
  Declare Function SetWindowPos Lib "user32" (ByVal hwnd As Long, ByVal hWndInsertAfter As Long, ByVal X As Long, ByVal Y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long
#Else
  Declare Function sndPlaySound Lib "MMSYSTEM" (ByVal lpszSoundName As String, ByVal uflags As Integer) As Integer
  Declare Function SetWindowPos Lib "user" (ByVal hwnd As Integer, ByVal hWndInsertAfter As Integer, ByVal X As Integer, ByVal Y As Integer, ByVal cx As Integer, ByVal cy As Integer, ByVal wFlags As Integer) As Integer
#End If

Const UmlCount = 183
Dim uml(UmlCount, 1) As String
Dim UmlInitFlag As Boolean

' Declare Type for API call:
      Private Type OSVERSIONINFO
        dwOSVersionInfoSize As Long
        dwMajorVersion As Long
        dwMinorVersion As Long
        dwBuildNumber As Long
        dwPlatformId As Long
        szCSDVersion As String * 128   '  Maintenance string for PSS usage
      End Type

      ' API declarations:

      Private Declare Function GetVersionEx Lib "kernel32" _
         Alias "GetVersionExA" _
         (lpVersionInformation As OSVERSIONINFO) As Long

      Private Declare Sub keybd_event Lib "user32" _
         (ByVal bVk As Byte, _
          ByVal bScan As Byte, _
          ByVal dwFlags As Long, ByVal dwExtraInfo As Long)

      Private Declare Function GetKeyboardState Lib "user32" _
         (pbKeyState As Byte) As Long

      Private Declare Function SetKeyboardState Lib "user32" _
         (lppbKeyState As Byte) As Long

      ' Constant declarations:
      Const VK_NUMLOCK = &H90
      Const VK_SCROLL = &H91
      Const VK_CAPITAL = &H14
      Const KEYEVENTF_EXTENDEDKEY = &H1
      Const KEYEVENTF_KEYUP = &H2
      Const VER_PLATFORM_WIN32_NT = 2
      Const VER_PLATFORM_WIN32_WINDOWS = 1
  

Sub ActivateNumLock()
    Dim o As OSVERSIONINFO
    Dim NumLockState As Boolean
    Dim ScrollLockState As Boolean
    Dim CapsLockState As Boolean
      o.dwOSVersionInfoSize = Len(o)
      GetVersionEx o
      Dim keys(0 To 255) As Byte
      GetKeyboardState keys(0)

      ' NumLock handling:
      NumLockState = keys(VK_NUMLOCK)
      If NumLockState <> True Then    'Turn numlock on
        If o.dwPlatformId = VER_PLATFORM_WIN32_WINDOWS Then  '=== Win95/98

          keys(VK_NUMLOCK) = 1
          SetKeyboardState keys(0)
        ElseIf o.dwPlatformId = VER_PLATFORM_WIN32_NT Then   '=== WinNT
        Debug.Print "Numlock"
        'Simulate Key Press
          keybd_event VK_NUMLOCK, &H45, KEYEVENTF_EXTENDEDKEY Or 0, 0
          DoEvents
        'Simulate Key Release
          keybd_event VK_NUMLOCK, &H45, KEYEVENTF_EXTENDEDKEY _
             Or KEYEVENTF_KEYUP, 0
        End If
      End If
End Sub


Function bool(text As String) As Boolean
a$ = LCase$(Trim$(text))
Select Case a$
  Case "wahr", "true", "yes", "y", "ja", "j", "1": bool = True
  Case Else: bool = False
End Select
End Function

Sub CreatePath(pfad As String)
Dim oldpfad As String
On Error Resume Next
pa$ = Trim$(pfad)
If InStr(pa$, ".") = 0 And right$(pa$, 1) <> "\" Then pa$ = pa$ + "\"
oldpfad = CurDir
p = 1
i = InStr(pa$, ":")
Err = 0
If i > 0 Then
  ChDrive Mid$(pa$, p, i - p + 1)
  p = i + 1
End If
If Err = 0 Then
  Do
    i = InStr(p, pa$, "\")
    If i > 0 Then
      pf$ = Mid$(pa$, p, i - p)
      If right$(pf$, 1) = "\" Then pf$ = Mid$(pa$, p + 1, i - p - 1)
      If pf$ = "" Then pf$ = "\"
      Err = 0: ChDir pf$
      If Err > 0 Then
        Err = 0: MkDir pf$
        If Err > 0 Then Exit Do
        ChDir pf$
      End If
      p = i + 1
    Else
      pf$ = Mid$(pa$, p)
      If pf$ <> "" Then
        ff = FreeFile
        Open pf$ For Random As #ff
        Close #ff
      End If
      Exit Do
    End If
  Loop
End If
er = Err:       ' Fehlercode zwischenspeichern
ChDrive left$(oldpfad, 2): ' Altes Verzeichnis wiederherstellen
ChDir oldpfad
Err = er:     ' Fehlercode übermitteln
End Sub


Function Cut0(t$) As String
n = InStr(t$, Chr$(0))
If n > 0 Then
  Cut0 = left$(t$, n - 1)
Else
  Cut0 = t$
End If
End Function

Function dat2long&(d$)
a$ = User2Dat(d$)
dat2long& = Val(right$(a$, 4) + Mid$(a$, 4, 2) + left$(a$, 2))
End Function

Function Dat2Ser&(d$)
a$ = User2Dat(d$)
t = Val(left$(a$, 2))
m = Val(Mid$(a$, 4, 2))
j = Val(right$(a$, 4))
Dat2Ser& = j * 365& + m * 31& + t
End Function

Function Dat2User(d$) As String
If datform = "" Then DatumsFormat ""
a$ = datform
i = InStr(a$, "dd")
Mid$(a$, i, 2) = left$(d$, 2)
i = InStr(a$, "mm")
Mid$(a$, i, 2) = Mid$(d$, 4, 2)
i = InStr(a$, "yyyy")
If i > 0 Then
  Mid$(a$, i, 4) = right$(d$, 4)
Else
  i = InStr(a$, "yy")
  Mid$(a$, i, 2) = right$(d$, 2)
End If
Dat2User = a$
End Function
Function Datum$()
a$ = Mid$(Date$, 4, 2) + "." + left$(Date$, 2) + "." + right$(Date$, 4)
Datum$ = Dat2User(a$)
End Function

Function DatumsAuswertung(oldDatum$) As String
a$ = oldDatum$
Do
  c = InStr(a$, ","): If c > 0 Then Mid$(a$, c, 1) = "." Else Exit Do
Loop
If Len(a$) < 10 Then
  d$ = Datum$()
  jh$ = Mid$(d$, 7, 2): j$ = right$(d$, 2): m$ = Mid$(d$, 4, 2)
  b$ = "": For i = 1 To Len(a$): If Mid$(a$, i, 1) = "." Then b$ = b$ + "." Else b$ = b$ + "x"
  Next
  Select Case b$
    Case "xxxxxxxx": a$ = left$(a$, 2) + "." + Mid$(a$, 3, 2) + "." + right$(a$, 4)
    Case "xxxxxx": a$ = left$(a$, 2) + "." + Mid$(a$, 3, 2) + "." + jh$ + right$(a$, 2)
    Case "x.x.xxxx": a$ = "0" + left$(a$, 2) + "0" + right$(a$, 6)
    Case "x.x.xx": a$ = "0" + left$(a$, 2) + "0" + Mid$(a$, 3, 2) + jh$ + right$(a$, 2)
    Case "xx.x.xxxx": a$ = left$(a$, 3) + "0" + Mid$(a$, 4)
    Case "xx.x.xx": a$ = left$(a$, 3) + "0" + Mid$(a$, 4, 2) + jh$ + right$(a$, 2)
    Case "x.xx.xx": a$ = "0" + left$(a$, 5) + jh$ + right$(a$, 2)
    Case "xx.xx.xx": a$ = left$(a$, 6) + jh$ + right$(a$, 2)
    Case "x.x.", "x.x": a$ = "0" + left$(a$, 2) + "0" + Mid$(a$, 3, 1) + "." + jh$ + j$
    Case "x.xx.", "x.xx": a$ = "0" + left$(a$, 4) + "." + jh$ + j$
    Case "xx.x.", "xx.x": a$ = left$(a$, 3) + "0" + Mid$(a$, 4, 1) + "." + jh$ + j$
    Case "xx.xx.", "xx.xx": a$ = left$(a$, 5) + "." + jh$ + j$
    Case "x.", "x": a$ = "0" + left$(a$, 1) + "." + m$ + "." + jh$ + j$
    Case "xx.", "xx": a$ = left$(a$, 2) + "." + m$ + "." + jh$ + j$
    Case Else: a$ = "!-Fehler-!"
  End Select
End If
DatumsAuswertung$ = a$
End Function

Sub DatumsFormat(text As String)
If text = "" Then
  datform = "dd.mm.yyyy"
Else
  datform = text
End If
End Sub

Function DOSPfad(pfad1 As String) As String
DOSPfad = pfad1
If InStr(pfad1, "/") > 0 Then
  DOSPfad = Slash(pfad1)
End If
End Function

Function Exists(datei$) As Boolean
On Error Resume Next
a$ = Trim$(datei$)
Exists = False
If a$ <> "" Then
  ff = FreeFile: Err = 0
  Open a$ For Input Access Read As #ff
  If Err = 0 Then Exists = True
  Close #ff
End If
End Function

Function Fill(text As String, laenge As Integer) As String
text = Trim(text)
If Len(text) > laenge Then text = left(text, laenge)
Fill = text + space(laenge - Len(text))
End Function

Function GetFileName(pfad As String) As String
Dim l As Integer, i As Integer, p As Integer

l = Len(pfad)
For i = 1 To l
  a$ = Mid(pfad, i, 1)
  If (a$ = "/" Or a$ = ":" Or a$ = "\") Then p = i + 1
Next
GetFileName = Mid(pfad, p)
End Function

Function GetPath(file As String) As String
Dim l As Integer, i As Integer, p As Integer

l = Len(file)
For i = l To 1 Step -1
  a$ = Mid(file, i, 1)
  If (a$ = "/" Or a$ = "\") Then
    GetPath = left(file, i - 1)
    Exit Function
    
  End If
Next
GetPath = ""
End Function


Function gettmp$()
a$ = Environ$("TEMP")
gettmp$ = a$ + "\"
End Function

Function Komma2Punkt$(z$)
Do
  a = InStr(z$, ","): If a > 0 Then Mid$(z$, a, 1) = "."
Loop Until a = 0
Komma2Punkt$ = z$
End Function

Function LastError() As Long
LastError = er
End Function
Function LER() As Long
LER = er
End Function


Function Long2Dat$(l As Long)
a$ = Format$(l)
d$ = right$(a$, 2) + "." + Mid$(a$, 5, 2) + "." + left$(a$, 4)
Long2Dat$ = Dat2User(d$)
End Function

Function Long2Ser&(l&)
a$ = Long2Dat$(l&)
Long2Ser& = Dat2Ser&(a$)
End Function

Function longDatum&(d$)
a$ = User2Dat(d$)
longDatum& = Val(right$(a$, 4) + Mid$(a$, 4, 2) + left$(a$, 2))
End Function

Function Min2Sec(minuten As Single) As Long
Dim sec As Long, min As Long
min = Int(minuten)
sec = (minuten - min) * 100
sec = sec + min * 60

Min2Sec = sec
End Function

Sub PrintWithLinefeed(X As Integer, breite As Integer, s As String)
Dim l As Integer, p As Integer
p = 1
Do
    Printer.CurrentX = X
    l = InStr(p, s, Chr(10))
    If l = 0 Then
        If breite = 0 Then
            Printer.Print Mid(s, p)
        Else
            PrintWidth Mid(s, p), X, breite
        End If
        Exit Do
    Else
        If breite = 0 Then
            Printer.Print Mid(s, p, l - p)
        Else
            PrintWidth Mid(s, p, l - p), X, breite
        End If
    End If
    p = l + 1
Loop
End Sub

Sub PrintWidth(text As String, startx As Integer, w As Integer)
    Dim fw As Integer
    w = w - 2
    fw = Printer.TextWidth(text)
    If fw <= w Then
        Printer.Print text
        Exit Sub
    End If
    ' Rausfinden, bis wohin es noch passt
    l = Len(text)
    s = 1
    Do While Printer.TextWidth(left(text, s)) < w
        s = s + 1
        If s >= l Then Exit Do
    Loop
    Printer.Print left(text, s - 1); "..."
End Sub

Function Sec2Min(seconds As Long) As Single
Dim rest As Long
Dim min As Single
min = Int(seconds / 60)
rest = seconds - (min * 60)
Sec2Min = min + (rest / 100)

End Function


Sub SelectPrinter(Printername As String)
Dim X As Printer
For Each X In Printers
    If X.DeviceName = Printername Then
        Set Printer = X
        Exit For
    End If
Next
End Sub

Sub SetError(Fehlercode As Long)
er = Fehlercode
End Sub

Function Slash(pfad1 As String) As String
Dim q As String
Dim z As String
Dim p As Integer
Dim s As String
s = pfad1
If InStr(s, "/") > 0 Then
  q = "/": z = "\"
ElseIf InStr(s, "\") > 0 Then
  q = "\": z = "/"
End If
Do
  p = InStr(s, q)
  If p = 0 Then Exit Do
  Mid$(s, p, 1) = z
Loop
Slash = s
End Function

Function StringDatum$(l As Long)
a$ = Format$(l)
d$ = right$(a$, 2) + "." + Mid$(a$, 5, 2) + "." + left$(a$, 4)
StringDatum$ = Dat2User(d$)
End Function

Sub swap(s1 As Double, s2 As Double)
Dim s3 As Double
s3 = s1
s1 = s2: s2 = s3
End Sub

Function time2long&(a$)
s& = 0
If Len(a$) > 5 Then
  s& = Val(right$(a$, 2))
  a$ = left$(a$, 5)
End If
s& = s& + 60& * (Val(right$(a$, 2)))
s& = s& + 3600& * (Val(left$(a$, 2)))
time2long& = s&
End Function

Function ToHTML(text As String) As String
If UmlInitFlag = False Then UmlInit
For i = 0 To UmlCount
  lp = 1
  p = InStr(lp, text, uml(i, 1))
  While p > 0
    a$ = left(text, p - 1) + uml(i, 0) + Mid(text, p + 1)
    text = a$
    lp = p + Len(uml(i, 0)) - 1
    p = InStr(lp, text, uml(i, 1))
  Wend
  
Next
ToHTML = text
End Function

Sub UmlInit()
' cat umls | awk 'BEGIN {c=0} {print "uml("c",0)=\"&amp;"$2
' ";\": uml("c",1)=\"&"$2";\"<br>";c++}' >t.html
UmlInitFlag = True

uml(0, 0) = "&amp;": uml(0, 1) = "&"
uml(1, 0) = "&ISOnum;": uml(1, 1) = "&ISOnum;"
uml(2, 0) = "&iexcl;": uml(2, 1) = "¡"
uml(3, 0) = "&cent;": uml(3, 1) = "¢"
uml(4, 0) = "&pound;": uml(4, 1) = "£"
uml(5, 0) = "&curren;": uml(5, 1) = "¤"
uml(6, 0) = "&yen;": uml(6, 1) = "¥"
uml(7, 0) = "&brvbar;": uml(7, 1) = "¦"
uml(8, 0) = "&ISOnum;": uml(8, 1) = "&ISOnum;"
uml(9, 0) = "&sect;": uml(9, 1) = "§"
uml(10, 0) = "&uml;": uml(10, 1) = "¨"
uml(11, 0) = "&ISOdia;": uml(11, 1) = "&ISOdia;"
uml(12, 0) = "&copy;": uml(12, 1) = "©"
uml(13, 0) = "&ordf;": uml(13, 1) = "ª"
uml(14, 0) = "&laquo;": uml(14, 1) = "«"
uml(15, 0) = "&left;": uml(15, 1) = "&left;"
uml(16, 0) = "&not;": uml(16, 1) = "¬"
uml(17, 0) = "&shy;": uml(17, 1) = "­"
uml(18, 0) = "&ISOnum;": uml(18, 1) = "&ISOnum;"
uml(19, 0) = "&reg;": uml(19, 1) = "®"
uml(20, 0) = "&ISOnum;": uml(20, 1) = "&ISOnum;"
uml(21, 0) = "&macr;": uml(21, 1) = "¯"
uml(22, 0) = "&APL;": uml(22, 1) = "&APL;"
uml(23, 0) = "&deg;": uml(23, 1) = "°"
uml(24, 0) = "&plusmn;": uml(24, 1) = "±"
uml(25, 0) = "&ISOnum;": uml(25, 1) = "&ISOnum;"
uml(26, 0) = "&sup2;": uml(26, 1) = "²"
uml(27, 0) = "&squared,;": uml(27, 1) = "&squared,;"
uml(28, 0) = "&sup3;": uml(28, 1) = "³"
uml(29, 0) = "&cubed,;": uml(29, 1) = "&cubed,;"
uml(30, 0) = "&acute;": uml(30, 1) = "´"
uml(31, 0) = "&ISOdia;": uml(31, 1) = "&ISOdia;"
uml(32, 0) = "&micro;": uml(32, 1) = "µ"
uml(33, 0) = "&para;": uml(33, 1) = "¶"
uml(34, 0) = "&ISOnum;": uml(34, 1) = "&ISOnum;"
uml(35, 0) = "&middot;": uml(35, 1) = "·"
uml(36, 0) = "&Greek;": uml(36, 1) = "&Greek;"
uml(37, 0) = "&cedil;": uml(37, 1) = "¸"
uml(38, 0) = "&sup1;": uml(38, 1) = "¹"
uml(39, 0) = "&ISOnum;": uml(39, 1) = "&ISOnum;"
uml(40, 0) = "&ordm;": uml(40, 1) = "º"
uml(41, 0) = "&ISOnum;": uml(41, 1) = "&ISOnum;"
uml(42, 0) = "&raquo;": uml(42, 1) = "»"
uml(43, 0) = "&right;": uml(43, 1) = "&right;"
uml(44, 0) = "&frac14;": uml(44, 1) = "¼"
uml(45, 0) = "&fraction;": uml(45, 1) = "&fraction;"
uml(46, 0) = "&frac12;": uml(46, 1) = "½"
uml(47, 0) = "&fraction;": uml(47, 1) = "&fraction;"
uml(48, 0) = "&frac34;": uml(48, 1) = "¾"
uml(49, 0) = "&fraction;": uml(49, 1) = "&fraction;"
uml(50, 0) = "&iquest;": uml(50, 1) = "¿"
uml(51, 0) = "&turned;": uml(51, 1) = "&turned;"
uml(52, 0) = "&Agrave;": uml(52, 1) = "À"
uml(53, 0) = "&latin;": uml(53, 1) = "&latin;"
uml(54, 0) = "&ISOlat1;": uml(54, 1) = "&ISOlat1;"
uml(55, 0) = "&Aacute;": uml(55, 1) = "Á"
uml(56, 0) = "&ISOlat1;": uml(56, 1) = "&ISOlat1;"
uml(57, 0) = "&Acirc;": uml(57, 1) = "Â"
uml(58, 0) = "&ISOlat1;": uml(58, 1) = "&ISOlat1;"
uml(59, 0) = "&Atilde;": uml(59, 1) = "Ã"
uml(60, 0) = "&ISOlat1;": uml(60, 1) = "&ISOlat1;"
uml(61, 0) = "&Auml;": uml(61, 1) = "Ä"
uml(62, 0) = "&ISOlat1;": uml(62, 1) = "&ISOlat1;"
uml(63, 0) = "&Aring;": uml(63, 1) = "Å"
uml(64, 0) = "&latin;": uml(64, 1) = "&latin;"
uml(65, 0) = "&ISOlat1;": uml(65, 1) = "&ISOlat1;"
uml(66, 0) = "&AElig;": uml(66, 1) = "Æ"
uml(67, 0) = "&latin;": uml(67, 1) = "&latin;"
uml(68, 0) = "&ISOlat1;": uml(68, 1) = "&ISOlat1;"
uml(69, 0) = "&Ccedil;": uml(69, 1) = "Ç"
uml(70, 0) = "&ISOlat1;": uml(70, 1) = "&ISOlat1;"
uml(71, 0) = "&Egrave;": uml(71, 1) = "È"
uml(72, 0) = "&ISOlat1;": uml(72, 1) = "&ISOlat1;"
uml(73, 0) = "&Eacute;": uml(73, 1) = "É"
uml(74, 0) = "&ISOlat1;": uml(74, 1) = "&ISOlat1;"
uml(75, 0) = "&Ecirc;": uml(75, 1) = "Ê"
uml(76, 0) = "&ISOlat1;": uml(76, 1) = "&ISOlat1;"
uml(77, 0) = "&Euml;": uml(77, 1) = "Ë"
uml(78, 0) = "&ISOlat1;": uml(78, 1) = "&ISOlat1;"
uml(79, 0) = "&Igrave;": uml(79, 1) = "Ì"
uml(80, 0) = "&ISOlat1;": uml(80, 1) = "&ISOlat1;"
uml(81, 0) = "&Iacute;": uml(81, 1) = "Í"
uml(82, 0) = "&ISOlat1;": uml(82, 1) = "&ISOlat1;"
uml(83, 0) = "&Icirc;": uml(83, 1) = "Î"
uml(84, 0) = "&ISOlat1;": uml(84, 1) = "&ISOlat1;"
uml(85, 0) = "&Iuml;": uml(85, 1) = "Ï"
uml(86, 0) = "&ISOlat1;": uml(86, 1) = "&ISOlat1;"
uml(87, 0) = "&ETH;": uml(87, 1) = "Ð"
uml(88, 0) = "&Ntilde;": uml(88, 1) = "Ñ"
uml(89, 0) = "&ISOlat1;": uml(89, 1) = "&ISOlat1;"
uml(90, 0) = "&Ograve;": uml(90, 1) = "Ò"
uml(91, 0) = "&ISOlat1;": uml(91, 1) = "&ISOlat1;"
uml(92, 0) = "&Oacute;": uml(92, 1) = "Ó"
uml(93, 0) = "&ISOlat1;": uml(93, 1) = "&ISOlat1;"
uml(94, 0) = "&Ocirc;": uml(94, 1) = "Ô"
uml(95, 0) = "&ISOlat1;": uml(95, 1) = "&ISOlat1;"
uml(96, 0) = "&Otilde;": uml(96, 1) = "Õ"
uml(97, 0) = "&ISOlat1;": uml(97, 1) = "&ISOlat1;"
uml(98, 0) = "&Ouml;": uml(98, 1) = "Ö"
uml(99, 0) = "&ISOlat1;": uml(99, 1) = "&ISOlat1;"
uml(100, 0) = "&times;": uml(100, 1) = "×"
uml(101, 0) = "&Oslash;": uml(101, 1) = "Ø"
uml(102, 0) = "&latin;": uml(102, 1) = "&latin;"
uml(103, 0) = "&ISOlat1;": uml(103, 1) = "&ISOlat1;"
uml(104, 0) = "&Ugrave;": uml(104, 1) = "Ù"
uml(105, 0) = "&ISOlat1;": uml(105, 1) = "&ISOlat1;"
uml(106, 0) = "&Uacute;": uml(106, 1) = "Ú"
uml(107, 0) = "&ISOlat1;": uml(107, 1) = "&ISOlat1;"
uml(108, 0) = "&Ucirc;": uml(108, 1) = "Û"
uml(109, 0) = "&ISOlat1;": uml(109, 1) = "&ISOlat1;"
uml(110, 0) = "&Uuml;": uml(110, 1) = "Ü"
uml(111, 0) = "&ISOlat1;": uml(111, 1) = "&ISOlat1;"
uml(112, 0) = "&Yacute;": uml(112, 1) = "Ý"
uml(113, 0) = "&ISOlat1;": uml(113, 1) = "&ISOlat1;"
uml(114, 0) = "&THORN;": uml(114, 1) = "Þ"
uml(115, 0) = "&ISOlat1;": uml(115, 1) = "&ISOlat1;"
uml(116, 0) = "&szlig;": uml(116, 1) = "ß"
uml(117, 0) = "&ISOlat1;": uml(117, 1) = "&ISOlat1;"
uml(118, 0) = "&agrave;": uml(118, 1) = "à"
uml(119, 0) = "&latin;": uml(119, 1) = "&latin;"
uml(120, 0) = "&ISOlat1;": uml(120, 1) = "&ISOlat1;"
uml(121, 0) = "&aacute;": uml(121, 1) = "á"
uml(122, 0) = "&ISOlat1;": uml(122, 1) = "&ISOlat1;"
uml(123, 0) = "&acirc;": uml(123, 1) = "â"
uml(124, 0) = "&ISOlat1;": uml(124, 1) = "&ISOlat1;"
uml(125, 0) = "&atilde;": uml(125, 1) = "ã"
uml(126, 0) = "&ISOlat1;": uml(126, 1) = "&ISOlat1;"
uml(127, 0) = "&auml;": uml(127, 1) = "ä"
uml(128, 0) = "&ISOlat1;": uml(128, 1) = "&ISOlat1;"
uml(129, 0) = "&aring;": uml(129, 1) = "å"
uml(130, 0) = "&latin;": uml(130, 1) = "&latin;"
uml(131, 0) = "&ISOlat1;": uml(131, 1) = "&ISOlat1;"
uml(132, 0) = "&aelig;": uml(132, 1) = "æ"
uml(133, 0) = "&latin;": uml(133, 1) = "&latin;"
uml(134, 0) = "&ccedil;": uml(134, 1) = "ç"
uml(135, 0) = "&ISOlat1;": uml(135, 1) = "&ISOlat1;"
uml(136, 0) = "&egrave;": uml(136, 1) = "è"
uml(137, 0) = "&ISOlat1;": uml(137, 1) = "&ISOlat1;"
uml(138, 0) = "&eacute;": uml(138, 1) = "é"
uml(139, 0) = "&ISOlat1;": uml(139, 1) = "&ISOlat1;"
uml(140, 0) = "&ecirc;": uml(140, 1) = "ê"
uml(141, 0) = "&ISOlat1;": uml(141, 1) = "&ISOlat1;"
uml(142, 0) = "&euml;": uml(142, 1) = "ë"
uml(143, 0) = "&ISOlat1;": uml(143, 1) = "&ISOlat1;"
uml(144, 0) = "&igrave;": uml(144, 1) = "ì"
uml(145, 0) = "&ISOlat1;": uml(145, 1) = "&ISOlat1;"
uml(146, 0) = "&iacute;": uml(146, 1) = "í"
uml(147, 0) = "&ISOlat1;": uml(147, 1) = "&ISOlat1;"
uml(148, 0) = "&icirc;": uml(148, 1) = "î"
uml(149, 0) = "&ISOlat1;": uml(149, 1) = "&ISOlat1;"
uml(150, 0) = "&iuml;": uml(150, 1) = "ï"
uml(151, 0) = "&ISOlat1;": uml(151, 1) = "&ISOlat1;"
uml(152, 0) = "&eth;": uml(152, 1) = "ð"
uml(153, 0) = "&ntilde;": uml(153, 1) = "ñ"
uml(154, 0) = "&ISOlat1;": uml(154, 1) = "&ISOlat1;"
uml(155, 0) = "&ograve;": uml(155, 1) = "ò"
uml(156, 0) = "&ISOlat1;": uml(156, 1) = "&ISOlat1;"
uml(157, 0) = "&oacute;": uml(157, 1) = "ó"
uml(158, 0) = "&ISOlat1;": uml(158, 1) = "&ISOlat1;"
uml(159, 0) = "&ocirc;": uml(159, 1) = "ô"
uml(160, 0) = "&ISOlat1;": uml(160, 1) = "&ISOlat1;"
uml(161, 0) = "&otilde;": uml(161, 1) = "õ"
uml(162, 0) = "&ISOlat1;": uml(162, 1) = "&ISOlat1;"
uml(163, 0) = "&ouml;": uml(163, 1) = "ö"
uml(164, 0) = "&ISOlat1;": uml(164, 1) = "&ISOlat1;"
uml(165, 0) = "&divide;": uml(165, 1) = "÷"
uml(166, 0) = "&oslash;": uml(166, 1) = "ø"
uml(167, 0) = "&latin;": uml(167, 1) = "&latin;"
uml(168, 0) = "&ISOlat1;": uml(168, 1) = "&ISOlat1;"
uml(169, 0) = "&ugrave;": uml(169, 1) = "ù"
uml(170, 0) = "&ISOlat1;": uml(170, 1) = "&ISOlat1;"
uml(171, 0) = "&uacute;": uml(171, 1) = "ú"
uml(172, 0) = "&ISOlat1;": uml(172, 1) = "&ISOlat1;"
uml(173, 0) = "&ucirc;": uml(173, 1) = "û"
uml(174, 0) = "&ISOlat1;": uml(174, 1) = "&ISOlat1;"
uml(175, 0) = "&uuml;": uml(175, 1) = "ü"
uml(176, 0) = "&ISOlat1;": uml(176, 1) = "&ISOlat1;"
uml(177, 0) = "&yacute;": uml(177, 1) = "ý"
uml(178, 0) = "&ISOlat1;": uml(178, 1) = "&ISOlat1;"
uml(179, 0) = "&thorn;": uml(179, 1) = "þ"
uml(180, 0) = "&ISOlat1;": uml(180, 1) = "&ISOlat1;"
uml(181, 0) = "&yuml;": uml(181, 1) = "ÿ"
uml(182, 0) = "&ISOlat1;": uml(182, 1) = "&ISOlat1;"
uml(183, 0) = "&#39;": uml(183, 1) = "'"

End Sub

Function UNIXPfad(pfad1 As String) As String
UNIXPfad = pfad1
If InStr(pfad1, "\") > 0 Then
  UNIXPfad = Slash(pfad1)
End If
End Function

Function UnUML(text As String) As String

Dim uml(32, 1) As String

uml(0, 0) = "ä": uml(0, 1) = "ae"
uml(1, 0) = "ö": uml(1, 1) = "oe"
uml(2, 0) = "ü": uml(2, 1) = "ue"
uml(3, 0) = "ß": uml(3, 1) = "ss"
uml(4, 0) = "Ä": uml(4, 1) = "Ae"
uml(5, 0) = "Ö": uml(5, 1) = "Oe"
uml(6, 0) = "Ü": uml(6, 1) = "Ue"
uml(7, 0) = "é": uml(7, 1) = "e"
uml(8, 0) = "è": uml(8, 1) = "e"
uml(9, 0) = "ê": uml(9, 1) = "e"
uml(10, 0) = "î": uml(10, 1) = "i"
uml(11, 0) = "í": uml(11, 1) = "i"
uml(12, 0) = "ì": uml(12, 1) = "i"
uml(13, 0) = "á": uml(13, 1) = "a"
uml(14, 0) = "à": uml(14, 1) = "a"
uml(15, 0) = "â": uml(15, 1) = "a"
uml(16, 0) = "â": uml(16, 1) = "a"
uml(17, 0) = "ó": uml(17, 1) = "o"
uml(18, 0) = "ò": uml(18, 1) = "o"
uml(19, 0) = "ô": uml(19, 1) = "o"
uml(20, 0) = "ú": uml(20, 1) = "u"
uml(21, 0) = "ù": uml(21, 1) = "u"
uml(22, 0) = "û": uml(22, 1) = "u"
uml(23, 0) = "Ã": uml(23, 1) = "A"
uml(24, 0) = "ã": uml(24, 1) = "a"
uml(25, 0) = "å": uml(25, 1) = "a"
uml(26, 0) = "ç": uml(26, 1) = "c"
uml(27, 0) = "ë": uml(27, 1) = "e"
uml(28, 0) = "ï": uml(28, 1) = "i"
uml(29, 0) = "ñ": uml(29, 1) = "n"
uml(30, 0) = "õ": uml(30, 1) = "o"
uml(31, 0) = "ý": uml(31, 1) = "y"
uml(32, 0) = "ÿ": uml(32, 1) = "y"
a$ = text
For i = 0 To 32
  Do
    p = InStr(a$, uml(i, 0))
    If p = 0 Then Exit Do Else a$ = left$(a$, p - 1) + uml(i, 1) + Mid$(a$, p + 1)
  Loop
Next
UnUML = a$
End Function

Function User2Dat(d$) As String
If datform = "" Then DatumsFormat ""
a$ = datform
i = InStr(a$, "dd")
b$ = Mid$(d$, i, 2) + "."
i = InStr(a$, "mm")
b$ = b$ + Mid$(d$, i, 2) + "."
i = InStr(a$, "yyyy")
If i > 0 Then
  b$ = b$ + Mid$(d$, i, 4)
Else
  i = InStr(a$, "yy")
  b$ = b$ + left$(Format$(Year(Now)), 2) + Mid$(d$, i, 2)
End If
User2Dat = b$
End Function


Function Week(d$) As Integer
wd = Weekday("01.01." + right$(d$, 4), 0)
t = DayofYear(d$) + wd - 2
Week = CInt(Int(t / 7) + 1)
End Function

Function DayofYear(d$) As Integer
m = CInt(Mid$(d$, 4, 2))
Y = CInt(right$(d$, 4))
t = CInt(left$(d$, 2))
For i = 1 To m - 1
  If i = 1 Or i = 3 Or i = 5 Or i = 7 Or i = 8 Or i = 10 Or i = 12 Then
    t = t + 31
  ElseIf i = 2 Then
    t = t + 28: If Y Mod 4 = 0 Then t = t + 1
  Else
    t = t + 30
  End If
Next
DayofYear = t
End Function

Sub WindowStayOnTop(handle As Long, Modus As Boolean)
#If Win32 Then
  Dim topmost As Long, flags As Long, success As Long
#Else
  Dim topmost As Integer, flags As Integer, success As Integer
#End If

If Modus = True Then
  topmost = -1
Else
  topmost = -2
End If
flags = 3
success = SetWindowPos(handle, topmost, 0, 0, 0, 0, flags)
If success <> 0 Then SetError 0 Else SetError 1
End Sub

Function ereg_replace(such As String, ersetze As String, text As String) As String
Dim fertig As Boolean
Dim i As Integer
Dim pp As Integer

fertig = False
pp = 1
While fertig = False
    fertig = True
    i = InStr(pp, text, such)
    'Stop
    If (i > 0) Then
        fertig = False
        text = Mid(text, 1, i - 1) + ersetze + Mid(text, i + 1)
        pp = i + Len(ersetze)
    End If
Wend
ereg_replace = text

End Function
Function UpperCaseWords(text As String) As String
Dim a As String
Dim p As Long
Dim l As Long
Dim trenner As String
Dim t As String
Dim tl As Long
Dim i As Long
trenner = " ()[]{}-_+&./\?;:#'" + Chr(34)
tl = Len(trenner)
text = LCase(text)
a = UCase(Mid(text, 1, 1))
Mid(text, 1, 1) = a
l = Len(text)

For i = 1 To tl
    t = Mid(trenner, i, 1)
    p = InStr(text, t)
    While (p)
        If p + 1 <= l Then
            a = UCase(Mid(text, p + 1, 1))
            Mid(text, p + 1, 1) = a
        End If
        p = InStr(p + 1, text, t)
    Wend
Next
UpperCaseWords = text
End Function

Attribute VB_Name = "drucke"
DefInt A-Z
Dim DruckIndex As Long
Dim dTraeger As Integer
Dim druckx As Long
Dim drucky As Long

Private Const LF_FACESIZE = 32

   Private Type LOGFONT
      lfHeight As Long
      lfWidth As Long
      lfEscapement As Long
      lfOrientation As Long
      lfWeight As Long
      lfItalic As Byte
      lfUnderline As Byte
      lfStrikeOut As Byte
      lfCharSet As Byte
      lfOutPrecision As Byte
      lfClipPrecision As Byte
      lfQuality As Byte
      lfPitchAndFamily As Byte
      lfFaceName As String * LF_FACESIZE
   End Type

   Private Type DOCINFO
      cbSize As Long
      lpszDocName As String
      lpszOutput As String
      lpszDatatype As String
      fwType As Long
   End Type

   Private Declare Function CreateFontIndirect Lib "gdi32" Alias _
   "CreateFontIndirectA" (lpLogFont As LOGFONT) As Long

   Private Declare Function SelectObject Lib "gdi32" _
   (ByVal hdc As Long, ByVal hObject As Long) As Long

   Private Declare Function DeleteObject Lib "gdi32" _
   (ByVal hObject As Long) As Long

   Private Declare Function CreateDC Lib "gdi32" Alias "CreateDCA" _
   (ByVal lpDriverName As String, ByVal lpDeviceName As String, _
   ByVal lpOutput As Long, ByVal lpInitData As Long) As Long

   Private Declare Function DeleteDC Lib "gdi32" (ByVal hdc As Long) _
   As Long

   Private Declare Function TextOut Lib "gdi32" Alias "TextOutA" _
   (ByVal hdc As Long, ByVal X As Long, ByVal Y As Long, _
   ByVal lpString As String, ByVal nCount As Long) As Long ' or Boolean
   
   Private Declare Function SetMapMode Lib "gdi32" (ByVal hdc As Long, ByVal fnMapMode As Long) As Long
   Private Declare Function GetMapMode Lib "gdi32" (ByVal hdc As Long) As Long


   Private Declare Function StartDoc Lib "gdi32" Alias "StartDocA" _
   (ByVal hdc As Long, lpdi As DOCINFO) As Long

   Private Declare Function EndDoc Lib "gdi32" (ByVal hdc As Long) _
   As Long

   Private Declare Function StartPage Lib "gdi32" (ByVal hdc As Long) _
   As Long

   Private Declare Function EndPage Lib "gdi32" (ByVal hdc As Long) _
   As Long


Type FontSelect
    name As String
    size As Integer
    bold As Boolean
    italian As Boolean
    underlined As Boolean
End Type


Public coverfont(4) As FontSelect

Sub DruckMP3Cover(dTraeger, dIndex As Long, tmpTrV As TraegerVerzeichnis, X, Y, w, h, maxh)
Dim tmpti As Titeldatei
Dim tmpTrDat As TraegerDatensatz
Dim tmpTrTi As TraegerTitel
Dim sp As Single, pp As Long
Dim tpmx As Single, tpmy As Single
Dim starty As Long, gesamtlaenge As Long
Dim px As Long, py As Long, py2 As Long
Dim ppTitel As String, ppIndex As String, ppTitelFront As String
Dim tret As Integer

'Definitionen für quer drucken (aus MS-Knowledge base Q175535)
'http://support.microsoft.com/support/kb/articles/Q175/5/35.ASP

 ' Combine API Calls with the Printer object
      Dim OutString As String
      Dim lf As LOGFONT
      Dim result As Long
      Dim hOldfont As Long
      Dim hPrintDc As Long
      Dim hFont As Long


On Error Resume Next

tpmx = 56.7
tpmy = 56.7
X = 0
Y = 0
Printer.ScaleMode = vbTwips                ' Twips
Printer.Font.name = "Arial"
Printer.FontTransparent = True

ppIndex = "MPEG " + Format$(dIndex)
ppTitelFront = ""
ppTitel = ""
If tmpTrV.Bezeichnung > 0 Then
    GetTraegerTitel tmpTrV.Bezeichnung, tmpTrTi
    ppTitel = Trim$(tmpTrTi.Titel1)
    ppTitelFront = ppTitel
    If ppTitel = ppIndex Then
        ppTitel = ""
        If Trim$(tmpTrTi.Titel2) <> "" Then ppTitel = Trim$(tmpTrTi.Titel1)
    Else
        If Trim$(tmpTrTi.Titel2) <> "" Then ppTitel = ppTitel + ", " + Trim$(tmpTrTi.Titel1)
    End If
    If Trim$(tmpTrTi.Titel2) <> "" Then ppTitelFront = ppTitelFront + ", " + Trim$(tmpTrTi.Titel1)
End If


' *********************** Vorderseite ***************************

Printer.Font.name = "Arial"
Printer.Font.size = 28: Printer.Font.bold = True
X1 = X + 65 * tpmx
Y1 = Y + 5 * tpmy
X2 = X + 179 * tpmx
Y2 = Y + 118 * tpmy

Printer.CurrentX = X1
Printer.CurrentY = Y1
Printer.Print ppIndex
Printer.Font.bold = False

xfront = X1 + 15 * tpmx
Printer.Font.size = 10
Printer.CurrentY = Y1 + 20 * tpmy
Printer.CurrentX = X1: Printer.Print "Titel: ";
Printer.Font.bold = True: Printer.CurrentX = xfront
Printer.Print ppTitelFront
Printer.Font.bold = False
Printer.Print
Printer.CurrentX = X1
Printer.CurrentX = X1: Printer.Print "Tracks: ";
Printer.Font.bold = True: Printer.CurrentX = xfront
Printer.Print Format(tmpTrV.Tracks)
Printer.Print
xlaenge = X1
ylaenge = Printer.CurrentY
Printer.Print
Printer.Print


' Überstehenden Text loeschen
Printer.Line (X + 180 * tpmx, Y)-(w, Y + 120 * tpmy), RGB(255, 255, 255), BF

' Den Rahmen zeichnen
Printer.Line (X, Y)-(X + 180 * tpmx, Y + 120 * tpmy), 0, B
Printer.Line (X + (60 * tpmx), Y)-(X + 60 * tpmx, Y + 120 * tpmy), 0



' *********************** Rückseite ***************************
Y = 120 * tpmy
X2 = X + 144 * tpmx
X1 = X + 10 * tpmx

Printer.ScaleMode = vbTwips                ' Twips

Printer.Font.name = "Arial"
Printer.Font.size = 28: Printer.Font.bold = True


Printer.CurrentX = X + 10 * tpmx:
Printer.CurrentY = Y + tpmy
Printer.Print ppIndex

Printer.Font.bold = False

Printer.Font.size = 8
Printer.CurrentX = X + 10

If tmpTrV.Label > 0 Then
    a$ = GetTab("label.bez", tmpTrV.Label)
    'Printer.Print a$
Else
    'Printer.Print
End If
Printer.Print

'Printer.Print Lang(102 + dTraeger); ": ";


Printer.CurrentX = X1
Printer.Font.size = 8
Printer.Font.bold = True

Printer.Print ppTitel
Printer.Print

starty = Printer.CurrentY
Y1 = starty

Printer.Font.bold = False
a$ = "Printed by " + Programm + " " + version + "." + Format(build) + ", " + copyright
Printer.Font.size = 6: Printer.Font.bold = False
Printer.CurrentX = X + 10 * tpmx: Printer.CurrentY = Y + 113 * tpmy
Printer.Print a$;

Y2 = Y + 113 * tpmy

lw = 25 * tpmx: lh = 7 * tpmy
If Farbdruck = True Then
  Printer.PaintPicture gfx.drucklogo_farbig.Picture, X2 - lw - tpmx, Y + tpmy, lw, lh
Else
  Printer.PaintPicture gfx.drucklogo_sw.Picture, X2 - lw - tpmx, Y + tpmy, lw, lh
End If
Printer.FontSize = 6
Printer.CurrentY = Y + tpmy + lh

a$ = Long2Dat(tmpTrV.Aufnahmestart) + " - " + Long2Dat(tmpTrV.Aufnahmeende)
Printer.CurrentX = X2 - Printer.TextWidth(a$) - tpmx
Printer.Print a$


Printer.ScaleMode = vbTwips                ' Twips

If tmpTrV.Tracks < 1000 Then fs = 3: sp = 1
If tmpTrV.Tracks < 150 Then fs = 4: sp = 1.2
If tmpTrV.Tracks < 120 Then fs = 4: sp = 1.5
If tmpTrV.Tracks < 100 Then fs = 5: sp = 1.8
If tmpTrV.Tracks < 80 Then fs = 5: sp = 2
If tmpTrV.Tracks < 50 Then fs = 6: sp = 2.5

sp = sp * tpmy

ffTrDat = FreeFile
Open DatenPfad + "\" + TraegerBez(dTraeger) + ".dat" For Random As #ffTrDat Len = Len(tmpTrDat)
'tif = FreeFile
'Open TiDatei For Random As #tif Len = Len(tmpti)
pp = tmpTrV.ErsterDatensatz
letzteSeite = 0
Printer.Font.size = fs
Printer.Font.bold = False
gesamtlaenge = 0
If tmpTrV.Tracks > 0 Then
  Do
    Get #ffTrDat, pp, tmpTrDat
    If tmpTrDat.Seite > 0 Then
      If tmpTrDat.Seite <> letzteSeite Then
        If (tmpTrV.Seiten > 1) Then
            'Y1 = ((Y1 / 10) + 0.2) * ptpy
            Printer.FontBold = True: Printer.CurrentX = X1
            Printer.CurrentY = Y1
            Printer.FontSize = 7
            Printer.Print Mid$("ABCDEFGHIJK", tmpTrDat.Seite, 1); ":"
            Printer.FontSize = fs: Printer.FontBold = False
            letzteSeite = tmpTrDat.Seite
        End If
      End If
      'Get #tif, tmpTrDat.Titelnummer, tmpti
      tret = GetWMTitle(tmpTrDat.Titelnummer, tmpti)
      Printer.CurrentY = Y1
      Printer.CurrentX = X1 + 5 * tpmx
      Printer.Print Format$(tmpTrDat.track); ":";
      Printer.CurrentX = X1 + 9 * tpmx
      Printer.Print Trim$(tmpti.interpret); ": "; Trim$(tmpti.titel);
      a$ = GetTab("version.bez", tmpti.version)
      Printer.ForeColor = PrinterRGB(192, 0, 0)
      Printer.Print " ("; Format$(tmpti.laenge, "###0.00"); " Min., "; a$; ")"
      gesamtlaenge = gesamtlaenge + Min2Sec(tmpti.laenge)
          
      Printer.ForeColor = 0
      Y1 = Y1 + sp
      If Y1 > Y2 - (2 * sp) Then
        If X1 < (20 * tpmx) Then
            X1 = X + 75 * tpmx
            Y1 = starty
            Printer.Line (X1 - tpmx, Y1)-(w, Y2), RGB(255, 255, 255), BF
            
        Else
            Exit Do
        End If
      End If
    End If
    pp = tmpTrDat.NaechsterDatensatz
  Loop Until pp = 0
  
  
  'If X1 < (20 * tpmx) Then
  '          X1 = x + 75 * tpmx
  '          Printer.Line (X1 - tpmx, starty)-(w, Y2), RGB(255, 255, 255), BF
  '          Y1 = starty
  'Else
            X1 = X + 143 * tpmx
            Printer.Line (X1 - tpmx, starty)-(w, Y2), RGB(255, 255, 255), BF
  'End If
  
End If

Printer.CurrentY = Y + 113 * tpmy
Printer.Font.size = 6
Printer.Font.bold = False
a$ = "Gesamtlänge: " + Format$(Sec2Min(gesamtlaenge), "##0.00") + " Min."
Printer.CurrentX = X + (143 * tpmx) - Printer.TextWidth(a$)
Printer.Print a$

' Die Länge in die Frontseite einsetzen:
Printer.Font.size = 10
Printer.Font.bold = False
Printer.CurrentX = xlaenge
Printer.CurrentY = ylaenge
Printer.Print "Länge: ";
Printer.Font.bold = True: Printer.CurrentX = xfront
Printer.Print Format$(Sec2Min(gesamtlaenge), "##0.00") + " Min."


' Text and den Seiten drucken
' Quelle: MS-Knowledge base Q175535
' http://support.microsoft.com/support/kb/articles/Q175/5/35.ASP

fs = 12
Printer.Font.size = 12
Printer.Font.bold = True

ptppx = Printer.TwipsPerPixelX
ptppy = Printer.TwipsPerPixelY

      
hPrintDc = Printer.hdc


lf.lfEscapement = -900
lf.lfHeight = (fs * -20) / ptppy
lf.lfWeight = 900

hFont = CreateFontIndirect(lf)
hOldfont = SelectObject(hPrintDc, hFont)
px = Int((X + 125.8 * tpmx) / ptppx)
py = Int((Y - tpmy * 18) / ptppy)

result = TextOut(hPrintDc, px, py, ppIndex, Len(ppIndex))
result = SelectObject(hPrintDc, hOldfont)
result = DeleteObject(hFont)

lf.lfHeight = (8 * -20) / ptppy
lf.lfWeight = 500

hFont = CreateFontIndirect(lf)
hOldfont = SelectObject(hPrintDc, hFont)
px = Int((X + 125 * tpmx) / ptppx)
py = Int((Y + tpmy * 10) / ptppy)

result = TextOut(hPrintDc, px, py, ppTitel, Len(ppTitel))
result = SelectObject(hPrintDc, hOldfont)
result = DeleteObject(hFont)

' Überstehenden Text loeschen
Printer.Line (X + 145 * tpmx, Y + 116 * tpmy)-(X + 152 * tpmx, h), RGB(255, 255, 255), BF


' Den Rahmen zeichnen
Printer.Line (X, Y)-(X + 6 * tpmx, Y + 117 * tpmy), 0, B
Printer.Line (X + 6 * tpmx, Y)-(X + 145 * tpmx, Y + 117 * tpmy), 0, B
Printer.Line (X + 145 * tpmx, Y)-(X + 151 * tpmx, Y + 117 * tpmy), 0, B
  
'Close tif
Close ffTrDat


End Sub

Sub DruckeCDNummern()
Dim ptpx As Long, ptpy As Long
m$ = "Drücken Sie OK, um den Druck zu starten!"
a = MsgBox(m$, 49, "CD-Etiketten drucken")
If a <> 1 Then End
SelectPrinter (drucker)
'Printer.ScaleMode = 1
'Printer.Orientation = 1
  aa$ = ""
  Printer.FontSize = 28: Printer.FontBold = True
  X1 = X + (kw - Printer.TextWidth(aa$)) / 2
  Printer.CurrentX = X1: Printer.CurrentY = Y + 0.05 * ptpy
  Printer.Print aa$

Y = 0: X = 0: h = Printer.Height: w = Printer.Width

ptpx = 567
ptpy = 567
Printer.FontTransparent = False
breite = Printer.Width: Höhe = Printer.Height
Printer.FontSize = 14
Printer.FontBold = True

For i = 596 To 800
  Printer.Line (X, Y)-(X + 850, Y + 340), 0, B
  aa$ = Format$(i)
  l# = Printer.TextWidth(aa$)
  Printer.CurrentX = X + 794 - l#: Printer.CurrentY = Y + 15
  Printer.Print aa$
  
  X = X + 850
  If X > 11056 Then
    X = 0: Y = Y + 340
    If Y + 340 > 2325 And Y < 2438 Then Y = 2381
    If Y + 340 > 4649 And Y < 4819 Then Y = 4763
  End If
  If X + 850 > 3969 And X < 4082 Then X = 4076
  If X + 850 > 7938 And X < 8108 Then X = 8101
Next
Printer.EndDoc
End Sub


Sub DruckSimpleVCDCover()
Dim sp As Single, pp As Long
Dim tpmx As Single, tpmy As Single
Dim starty As Long, gesamtlaenge As Long
Dim px As Long, py As Long, py2 As Long
Dim ppIndex1 As String, ppIndex2 As String
Dim w As Integer, h As Integer
Dim xxxpx, xxxpy As Integer

h = Printer.Height
w = Printer.Width

'Definitionen für quer drucken (aus MS-Knowledge base Q175535)
'http://support.microsoft.com/support/kb/articles/Q175/5/35.ASP

 ' Combine API Calls with the Printer object
      Dim OutString As String
      Dim lf As LOGFONT
      Dim result As Long
      Dim hOldfont As Long
      Dim hPrintDc As Long
      Dim hFont As Long


On Error Resume Next

tpmx = 56.7
tpmy = 56.7
X = 0
Y = 0
Printer.ScaleMode = vbTwips                ' Twips
Printer.Font.name = "Arial"
Printer.FontTransparent = True

' *********************** Vorderseite ***************************

Printer.Font.name = coverfont(0).name
fs = coverfont(0).size
Do
    Printer.Font.size = fs: Printer.Font.bold = coverfont(0).bold
    If Printer.TextWidth(vcdcover.ueberschrift) <= 110 * tpmx Then Exit Do
    fs = fs - 1
    If fs < 16 Then Exit Do
Loop


X1 = X + 65 * tpmx
Y1 = Y + 5 * tpmy
X2 = X + 179 * tpmx
Y2 = Y + 118 * tpmy

Printer.CurrentX = X1
Printer.CurrentY = Y1
Printer.Print vcdcover.ueberschrift

xfront = X1 + 15 * tpmx
Printer.Font.name = coverfont(1).name
Printer.Font.size = coverfont(1).size
Printer.Font.bold = coverfont(1).bold
Printer.CurrentY = Y1 + 30 * tpmy
PrintWithLinefeed X1, 0, vcdcover.text_front


' Überstehenden Text loeschen
Printer.Line (X + 180 * tpmx, Y)-(w, Y + 120 * tpmy), RGB(255, 255, 255), BF

' Den Rahmen zeichnen
Printer.Line (X, Y)-(X + 180 * tpmx, Y + 120 * tpmy), 0, B
Printer.Line (X + (60 * tpmx), Y)-(X + 60 * tpmx, Y + 120 * tpmy), 0



' *********************** Rückseite ***************************
Y = 120 * tpmy
X2 = X + 144 * tpmx
X1 = X + 10 * tpmx

Printer.ScaleMode = vbTwips                ' Twips

' Text and den Seiten drucken
' Quelle: MS-Knowledge base Q175535
' http://support.microsoft.com/support/kb/articles/Q175/5/35.ASP

fs = coverfont(2).size

ppIndex1 = Trim(vcdcover.seite_fett)
ppIndex2 = Trim(vcdcover.seite_normal)

Printer.Font.name = coverfont(2).name
Printer.Font.size = coverfont(2).size
Printer.Font.bold = coverfont(2).bold

ptppx = Printer.TwipsPerPixelX
ptppy = Printer.TwipsPerPixelY

      
hPrintDc = Printer.hdc


lf.lfEscapement = -900
lf.lfHeight = (fs * -20) / ptppy
lf.lfWeight = 900

hFont = CreateFontIndirect(lf)
xxxpx = SetMapMode(hPrintDc, 1)
xxxpx = GetMapMode(hPrintDc)


hOldfont = SelectObject(hPrintDc, hFont)
px = Int((X + 125.8 * tpmx) / ptppx)
py = Int((Y - tpmy * 18) / ptppy)

result = TextOut(hPrintDc, px, py, ppIndex1, Len(ppIndex1))
result = SelectObject(hPrintDc, hOldfont)
result = DeleteObject(hFont)

Printer.Font.size = coverfont(3).size
Printer.Font.bold = coverfont(3).bold
fs = coverfont(3).size

px = Int((X + 125 * tpmx) / ptppx)
py = Int((Y + tpmy * 65) / ptppy)


lf.lfHeight = (fs * -20) / ptppy
lf.lfWeight = 500

hFont = CreateFontIndirect(lf)
hOldfont = SelectObject(hPrintDc, hFont)

result = TextOut(hPrintDc, px, py, ppIndex2, Len(ppIndex2))
result = SelectObject(hPrintDc, hOldfont)
result = DeleteObject(hFont)

xxxpx = SetMapMode(hPrintDc, 1)

' Überstehenden Text loeschen
Printer.Line (X + 145 * tpmx, Y + 116 * tpmy)-(X + 152 * tpmx, h), RGB(255, 255, 255), BF


' Den Rahmen zeichnen
Printer.Line (X, Y)-(X + 6 * tpmx, Y + 117 * tpmy), 0, B
Printer.Line (X + 6 * tpmx, Y)-(X + 145 * tpmx, Y + 117 * tpmy), 0, B
Printer.Line (X + 145 * tpmx, Y)-(X + 151 * tpmx, Y + 117 * tpmy), 0, B
  
Printer.EndDoc
End Sub

Sub DruckStandardcover(dTraeger, dIndex As Long, tmpTrV As TraegerVerzeichnis, X, Y, w, h, maxh)
Dim tmpti As Titeldatei
Dim tmpTrDat As TraegerDatensatz
Dim tmpTrTi As TraegerTitel
Dim sp As Single, pp As Long
Dim tret As Integer

On Error Resume Next

' Position auf dem Papier bestimmen
If dTraeger = 2 Or dTraeger = 3 Then
  kw = 15.5 * ptpx: kh = 12 * ptpy
Else
  kw = 10.15 * ptpx: kh = 7.7 * ptpy
End If
If X + kw > w Then X = 0: Y = Y + maxh: maxh = 0
If Y + kh > h Then X = 0: Y = 0: Printer.NewPage
Printer.FontTransparent = True
'Kassettenrahmen zeichnen
Printer.Line (X, Y)-(X + kw, Y + kh), 0, B
If dTraeger = 2 Or dTraeger = 3 Then
  X1 = X: Y1 = Y + 0.2 * ptpy: X2 = X + 1.5 * ptpx: Y2 = Y + kh - 0.2 * ptpy
  Printer.Line (X1, Y1)-(X2, Y2), 0, B
  X1 = X2: X2 = X1 + 0.3 * ptpx
  Printer.Line (X1, Y1)-(X2, Y2), 0, B
  X1 = X2: X2 = X1 + 0.7 * ptpx
  Printer.Line (X1, Y1)-(X2, Y1), 0
  Printer.Line (X1, Y2)-(X2, Y2), 0, B
  X1 = X2: X2 = X1 + 0.3 * ptpx
  Printer.Line (X1, Y1)-(X2, Y), 0
  Printer.Line (X1, Y2)-(X2, Y + kh), 0
  lw = 2.5 * ptpx: lh = 0.7 * ptpy
  X1 = X + 2 * ptpx: X2 = X + kw - 0.1 * ptpx
  Y1 = Y + 0.5 * ptpy: Y2 = Y + kh - 0.5 * ptpy
Else
  Y1 = Y + 1.2 * ptpy
  Printer.Line (X, Y1)-(X + kw, Y1), 0
  a$ = Format$(dIndex)
  Printer.FontSize = 28: Printer.FontBold = True
  X1 = X + (kw - Printer.TextWidth(a$)) / 2
  Printer.CurrentX = X1: Printer.CurrentY = Y + 0.05 * ptpy
  Printer.Print a$
  Printer.FontSize = 6: Printer.FontBold = False
  Y1 = Y + 0.1 * ptpy: X1 = X + 0.1 * ptpx
  Printer.CurrentY = Y1: Printer.CurrentX = X1
  If tmpTrV.Label > 0 Then
    a$ = GetTab("label.bez", tmpTrV.Label)
    Printer.Print a$
  End If
  Printer.FontSize = 7
  Printer.CurrentY = Printer.CurrentY + 0.1 * ptpy
  Printer.CurrentX = X1
  Printer.Print Format$(tmpTrV.laenge, "###.00") + " Min."
  Printer.CurrentY = Printer.CurrentY + 0.1 * ptpy
  If tmpTrV.Bezeichnung > 0 Then
    Printer.CurrentX = X1
    Printer.FontSize = 8
    GetTraegerTitel tmpTrV.Bezeichnung, tmpTrTi
    Printer.Print Trim$(tmpTrTi.Titel1);
    If Trim$(tmpTrTi.Titel2) <> "" Then Printer.Print ", " + Trim$(tmpTrTi.Titel1)
  End If
  X1 = X + kw - 1.3 * ptpx
  Printer.FontSize = 6
  Printer.CurrentY = Y + 0.1 * ptpy: Printer.CurrentX = X1
  Printer.Print Long2Dat(tmpTrV.Aufnahmestart) + Chr$(13)
  Printer.CurrentX = X1
  Printer.Print Long2Dat(tmpTrV.Aufnahmeende)
  Y1 = Y + 0.45 * ptpy: X1 = X1 + 0.4 * ptpx
  Printer.Line (X1, Y1)-(X1 + 0.2 * ptpx, Y1), 0
  lw = 2.5 * ptpx: lh = 0.7 * ptpy
  X1 = X + 0.1 * ptpx: X2 = X + kw - 0.1 * ptpx
  Y1 = Y + 1.3 * ptpy: Y2 = Y + kh - 0.2 * ptpy
  Printer.FontSize = 12
  Printer.CurrentX = X1: Printer.CurrentY = Y1
  Printer.Print Lang(102 + dTraeger); ": ";
  Printer.FontBold = True
  Printer.Print dIndex
  Printer.FontBold = False: Printer.FontSize = 8
  Printer.CurrentX = X1
  If tmpTrV.Bezeichnung > 0 Then
    Printer.Print Trim$(tmpTrTi.Titel1);
    If Trim$(tmpTrTi.Titel2) <> "" Then Printer.Print ", " + Trim$(tmpTrTi.Titel1)
  End If
  fs = 2: sp = 0.05
  If tmpTrV.Tracks < 35 Then fs = 3: sp = 0.1
  If tmpTrV.Tracks < 30 Then fs = 4: sp = 0.15
  If tmpTrV.Tracks < 25 Then fs = 5: sp = 0.2
  If tmpTrV.Tracks < 20 Then fs = 6: sp = 0.25
  sp = sp * ptpy
End If
If Farbdruck = True Then
  Printer.PaintPicture gfx.drucklogo_farbig.Picture, X2 - lw, Y1, lw, lh
Else
  Printer.PaintPicture gfx.drucklogo_sw.Picture, X2 - lw, Y1, lw, lh
End If
Y1 = Y1 + 0.8 * ptpy
ffTrDat = FreeFile
Open DatenPfad + "\" + TraegerBez(dTraeger) + ".dat" For Random As #ffTrDat Len = Len(tmpTrDat)
'tif = FreeFile
'Open TiDatei For Random As #tif Len = Len(tmpti)
pp = tmpTrV.ErsterDatensatz
letzteSeite = 0
Printer.FontSize = fs
Printer.FontBold = False
If tmpTrV.Tracks > 0 Then
  Do
    Get #ffTrDat, pp, tmpTrDat
    If tmpTrDat.Seite > 0 Then
      If tmpTrDat.Seite <> letzteSeite Then
        Y1 = Y1 + 0.2 * ptpy
        Printer.FontBold = True: Printer.CurrentX = X1: Printer.CurrentY = Y1
        Printer.FontSize = 7
        Printer.Print Mid$("ABCDEFGHIJK", tmpTrDat.Seite, 1); ":"
        Printer.FontSize = fs: Printer.FontBold = False
        letzteSeite = tmpTrDat.Seite
      End If
      'Get #tif, tmpTrDat.Titelnummer, tmpti
      tret = GetWMTitle(tmpTrDat.Titelnummer, tmpti)
      Printer.CurrentY = Y1
      Printer.CurrentX = X1 + 0.5 * ptpx
      Printer.Print Format$(tmpti.track); ":";
      Printer.CurrentX = X1 + 0.9 * ptpx
      Printer.Print Trim$(tmpti.interpret); ": "; Trim$(tmpti.titel);
      a$ = GetTab("version.bez", tmpti.version)
      Printer.ForeColor = PrinterRGB(192, 0, 0)
      Printer.Print " ("; a$; ": "; Format$(tmpti.laenge, "###0.00"); " Min.)"
      Printer.ForeColor = 0
      Y1 = Y1 + sp
    End If
    pp = tmpTrDat.NaechsterDatensatz
  Loop Until pp = 0
Else

End If
'Close tif
Close ffTrDat
a$ = "Printed by " + Programm + " " + version + ", " + copyright
Printer.FontSize = 6: Printer.FontBold = False
Printer.CurrentX = X2 - Printer.TextWidth(a$)
Printer.CurrentY = Y + kh - 0.3 * ptpy
Printer.Print a$;

'Alles rechts vom Rahmen abschneiden
Printer.Line (X + kw, Y)-(w, Y + kh), RGB(255, 255, 255), BF
Printer.Line (X + kw, Y)-(X + kw, Y + kh), 0

If Shareware = True Then
  a$ = Lang(164): Printer.FontSize = 15: Printer.FontBold = True
  Printer.CurrentX = (kw - Printer.TextWidth(a$)) / 2 + X
  Printer.CurrentY = (kh - Printer.TextHeight(a$)) / 2 + Y
  Printer.Print a$;
  Printer.FontSize = 10: Printer.FontBold = False
End If
X = X + kw: maxh = kh
End Sub



Sub DruckTraeger(dTraeger As Integer, von As Long, bis As Long)
Dim tmpTrV As TraegerVerzeichnis
ff = FreeFile
Open DatenPfad + "\" + TraegerBez(dTraeger) + ".dir" For Random As #ff Len = Len(tmpTrV)
druckmsg.Show
WMStat_Clear (Lang(163))
druckmsg.textfeld.Caption = Lang(163)
druckmsg.queue.Visible = False
DoEvents
SelectPrinter (drucker)
Printer.Orientation = 1
Printer.FontName = "Arial"
Printer.ScaleMode = vbTwips

ptpx = 567
ptpy = 567
Y = 0: X = 0: h = Printer.Height: w = Printer.Width
maxh = 0
For DruckIndex = von To bis
  If DruckIndex > 0 And DruckIndex <= LOF(ff) / Len(tmpTrV) Then
    Get #ff, DruckIndex, tmpTrV
    If (tmpTrV.InfoByte And 1) = 1 Then
      druckmsg.abbrechen.tag = "false"
      a$ = Lang(162) + " " + Lang(102 + dTraeger) + " " + Format$(DruckIndex)
      druckmsg.textfeld.Caption = a$
      WMStat_Text (a$)
      DoEvents
      If tmpTrV.cover = 0 Then
        If dTraeger = 7 Then            ' Mp3
            DruckMP3Cover dTraeger, DruckIndex, tmpTrV, X, Y, w, h, maxh
            If DruckIndex < bis Then
                Printer.NewPage
            Else
                Printer.EndDoc
            End If
        Else
            DruckStandardcover dTraeger, DruckIndex, tmpTrV, X, Y, w, h, maxh
        End If
      End If
      If druckmsg.abbrechen.tag = "true" Then
        Printer.KillDoc
        Exit For
      End If
    End If
  End If
Next
Close ff
If druckmsg.abbrechen.tag <> "true" Then Printer.EndDoc
Unload druckmsg
DoEvents
eingabe.SetFocus
End Sub



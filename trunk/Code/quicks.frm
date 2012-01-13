VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCT2.OCX"
Begin VB.Form quicksearch 
   Caption         =   "Quick-Search"
   ClientHeight    =   4725
   ClientLeft      =   7800
   ClientTop       =   1470
   ClientWidth     =   8220
   FillColor       =   &H00C00000&
   Icon            =   "quicks.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   4725
   ScaleWidth      =   8220
   Begin MSComCtl2.FlatScrollBar ListScroll 
      Height          =   1335
      Left            =   6840
      TabIndex        =   23
      Top             =   960
      Width           =   255
      _ExtentX        =   450
      _ExtentY        =   2355
      _Version        =   393216
      Appearance      =   0
      Orientation     =   1245184
   End
   Begin VB.CommandButton Drucken 
      Caption         =   "Drucken"
      Height          =   315
      Left            =   6300
      TabIndex        =   22
      Top             =   420
      Width           =   855
   End
   Begin VB.Frame tontraeger 
      BorderStyle     =   0  'Kein
      Caption         =   "Tonträger"
      Height          =   735
      Left            =   4920
      TabIndex        =   13
      Top             =   60
      Width           =   1335
      Begin VB.CheckBox Check1 
         Height          =   315
         Index           =   7
         Left            =   960
         Picture         =   "quicks.frx":030A
         Style           =   1  'Grafisch
         TabIndex        =   21
         Top             =   360
         Width           =   315
      End
      Begin VB.CheckBox Check1 
         Height          =   315
         Index           =   6
         Left            =   660
         Picture         =   "quicks.frx":064C
         Style           =   1  'Grafisch
         TabIndex        =   20
         Top             =   360
         Width           =   315
      End
      Begin VB.CheckBox Check1 
         Height          =   315
         Index           =   5
         Left            =   360
         Picture         =   "quicks.frx":098E
         Style           =   1  'Grafisch
         TabIndex        =   19
         Top             =   360
         Width           =   315
      End
      Begin VB.CheckBox Check1 
         Height          =   315
         Index           =   4
         Left            =   60
         Picture         =   "quicks.frx":0CD0
         Style           =   1  'Grafisch
         TabIndex        =   18
         Top             =   360
         Width           =   315
      End
      Begin VB.CheckBox Check1 
         Height          =   315
         Index           =   3
         Left            =   960
         Picture         =   "quicks.frx":1012
         Style           =   1  'Grafisch
         TabIndex        =   17
         Top             =   0
         Width           =   315
      End
      Begin VB.CheckBox Check1 
         Height          =   315
         Index           =   2
         Left            =   660
         Picture         =   "quicks.frx":1354
         Style           =   1  'Grafisch
         TabIndex        =   16
         Top             =   0
         Width           =   315
      End
      Begin VB.CheckBox Check1 
         Height          =   315
         Index           =   1
         Left            =   360
         Picture         =   "quicks.frx":1696
         Style           =   1  'Grafisch
         TabIndex        =   15
         Top             =   0
         Width           =   315
      End
      Begin VB.CheckBox Check1 
         Height          =   315
         Index           =   0
         Left            =   60
         Picture         =   "quicks.frx":19D8
         Style           =   1  'Grafisch
         TabIndex        =   14
         Top             =   0
         Width           =   315
      End
   End
   Begin VB.CommandButton startstop 
      Caption         =   "Start"
      Default         =   -1  'True
      Height          =   315
      Left            =   6300
      TabIndex        =   12
      Top             =   60
      Width           =   855
   End
   Begin VB.CommandButton Llaenge 
      Caption         =   "Länge"
      Height          =   255
      Left            =   5100
      TabIndex        =   11
      Top             =   925
      Width           =   615
   End
   Begin VB.CommandButton Ltraeger 
      Caption         =   "Tonträger"
      Height          =   255
      Left            =   5700
      TabIndex        =   10
      Top             =   925
      Width           =   1155
   End
   Begin VB.CommandButton LTrack 
      Caption         =   "Pos"
      Height          =   255
      Left            =   100
      TabIndex        =   9
      Top             =   925
      Width           =   495
   End
   Begin VB.CommandButton LTitel 
      Caption         =   "Titel - Interpret (Version)"
      Height          =   255
      Left            =   600
      TabIndex        =   8
      Top             =   925
      Width           =   4515
   End
   Begin VB.PictureBox Listfeld 
      Appearance      =   0  '2D
      BackColor       =   &H80000005&
      BorderStyle     =   0  'Kein
      DragIcon        =   "quicks.frx":1D1A
      ForeColor       =   &H80000008&
      Height          =   1125
      Left            =   120
      ScaleHeight     =   1125
      ScaleWidth      =   6780
      TabIndex        =   6
      Top             =   1200
      Width           =   6780
   End
   Begin MSComctlLib.ProgressBar queue 
      Height          =   195
      Left            =   60
      TabIndex        =   5
      Top             =   2520
      Width           =   6915
      _ExtentX        =   12197
      _ExtentY        =   344
      _Version        =   393216
      Appearance      =   0
   End
   Begin MSComctlLib.StatusBar StatusBar1 
      Align           =   2  'Unten ausrichten
      Height          =   315
      Left            =   0
      TabIndex        =   4
      Top             =   4410
      Width           =   8220
      _ExtentX        =   14499
      _ExtentY        =   556
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   1
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            AutoSize        =   1
            Object.Width           =   13970
         EndProperty
      EndProperty
   End
   Begin VB.TextBox z2 
      Height          =   315
      Left            =   600
      TabIndex        =   1
      Text            =   "Text1"
      Top             =   435
      Width           =   3315
   End
   Begin VB.TextBox z1 
      Height          =   315
      Left            =   960
      TabIndex        =   0
      Text            =   "Text1"
      Top             =   60
      Width           =   2955
   End
   Begin VB.Label ListPanel 
      BorderStyle     =   1  'Fest Einfach
      Height          =   1455
      Left            =   60
      TabIndex        =   7
      Top             =   900
      Width           =   7095
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "Titel:"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   195
      Left            =   60
      TabIndex        =   3
      Top             =   495
      Width           =   450
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Interpret:"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   195
      Left            =   60
      TabIndex        =   2
      Top             =   120
      Width           =   795
   End
End
Attribute VB_Name = "quicksearch"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
DefInt A-Z
Dim StopSearch As Boolean
Dim searchrunning As Boolean
Dim sTi As Titeldatei
Dim SearchlistLen As Long
Dim Searchfile As Integer
Dim ll As SearchListe
Dim quicksearchprinting As Boolean
Dim quicksearchinterpret As String
Dim quicksearchtitel As String
Dim ShowTiIndex As Boolean
Dim OleDragPosition As Point

Private LastClickedTitle As Long

Sub AddSearch()
Dim f As Integer

If DebugLevel > 5 Then
  Debug.Print "AddSearch: "; Trim$(sTi.interpret); ": "; Trim$(sTi.titel)
End If
Dim sl As SearchListe

' nur gültige Titel (14.11.2001)
If (sTi.InfoByte And 1) = 0 Then Exit Sub
' nur durch Tonträger referenzierte Titel (14.11.2001)
If (sTi.InfoByte And 256) = 0 Then Exit Sub

' Falls nur bestimmte Tontraeger gewuenscht waren, muessen wir
' die unerwuenschten rausfiltern
f = 0
For i = 0 To 7
    If Check1(i).value > 0 Then f = 1
Next
If f = 1 Then
    If Check1(sTi.traeger - 1).value < 1 Then Exit Sub
End If

sl.Titelnummer = sTi.nummer
sl.Interpret_Titel = Trim$(sTi.interpret) + ": " + Trim$(sTi.titel)
sl.version = Trim$(GetTab("version.bez", sTi.version))
sl.laenge = sTi.laenge
sl.traeger = sTi.traeger
If sl.traeger > 8 Or sl.traeger < 0 Then sl.traeger = 6
sl.Index = sTi.Index
sl.Seite = sTi.Seite
sl.track = sTi.track
SearchlistLen = SearchlistLen + 1
Put #Searchfile, SearchlistLen, sl
ListScroll.max = SearchlistLen
If Listfeld.Height / (15 * tpy) >= SearchlistLen - 1 Then FillOutSearch
End Sub

Sub DrawSearch()
Drucken.left = Width - Drucken.Width - 12 * tpx
quicksearch.startstop.left = Drucken.left
tontraeger.left = startstop.left - tontraeger.Width - 2 * tpx
'ok.left = drucken.left - ok.Width - 4 * tpx
'queue.left = 60
'queue.top = StatusBar1.top + 60
z1.Width = tontraeger.left - 4 * tpx - z1.left
z2.Width = tontraeger.left - 4 * tpx - z2.left
ListPanel.Width = Width - 16 * tpx
ListPanel.Height = Height - ListPanel.top - 32 * tpy - StatusBar1.Height
'ListScroll.left = ListPanel.Width - ListScroll.Width - 2 * tpx
ListScroll.top = ListPanel.top + 2 * tpx
ListScroll.left = ListPanel.Width - ListScroll.Width + 1 * tpx
ListScroll.Height = ListPanel.Height - 4 * tpy
ListScroll.Width = 19 * tpx
Ltraeger.left = ListScroll.left - Ltraeger.Width
LLaenge.left = Ltraeger.left - LLaenge.Width
LTitel.Width = LLaenge.left - LTitel.left
Listfeld.left = ListPanel.left + 2 * tpx
Listfeld.top = ListPanel.top + LTitel.Height + 3 * tpy
Listfeld.Width = ListPanel.Width - ListScroll.Width - 7 * tpx
Listfeld.Height = ListPanel.Height - LTitel.Height - 5 * tpy
End Sub

Sub FillOutSearch()
On Error Resume Next
f& = RGB(220, 220, 220)
y0 = Listfeld.Height: X0 = Listfeld.Width
X1 = LTitel.left - 8 * tpx
X2 = LLaenge.left - 8 * tpx
x3 = Ltraeger.left - 8 * tpx
If SearchlistLen > 0 Then
  Y = 0
  For i = ListScroll.value + 1 To SearchlistLen
    If i <= SearchlistLen Then
      Get #Searchfile, i, ll
      If Err > 0 Then
        Debug.Print "Unnormal Exit in Module quicksearch.FillOutSearch!"
        Debug.Print "Searchfile="; Searchfile; ", i="; i; ", l1="; l1
        Exit Sub
      End If
      Y1 = Y + 14 * tpy
      Listfeld.Line (0, Y)-(X1, Y1), RGB(255, 255, 255), BF
      a$ = Format$(i)
      Listfeld.CurrentX = X1 - Listfeld.TextWidth(a$) - 3 * tpx: Listfeld.CurrentY = Y
      Listfeld.ForeColor = 0
      Listfeld.Print a$;
      Listfeld.Line (X1, Y)-(X2, Y1), RGB(255, 255, 255), BF
      Listfeld.CurrentX = X1 + 4 * tpx: Listfeld.CurrentY = Y: Listfeld.ForeColor = 0
      If ShowTiIndex = True Then Listfeld.Print "(" + Format$(ll.Titelnummer) + ") ";
      Listfeld.FontBold = True
      Listfeld.Print Trim$(ll.Interpret_Titel);
      Listfeld.FontBold = False
      Listfeld.ForeColor = 192
      Listfeld.Print " ("; Trim$(ll.version); ")";
      Listfeld.Line (X2, Y)-(X0, Y1), RGB(255, 255, 255), BF
      Listfeld.CurrentY = Y: Listfeld.ForeColor = 0
      a$ = Format$(ll.laenge, "0.00"): If ll.laenge = 0 Then a$ = "---"
      Listfeld.CurrentX = x3 - Listfeld.TextWidth(a$) - 1 * tpx: Listfeld.CurrentY = Y
      Listfeld.Print a$;
      Listfeld.CurrentX = x3 + 20 * tpx
      Listfeld.Print Format$(ll.Index); " ";
      Listfeld.Print Chr$(64 + ll.Seite); "-"; Format$(ll.track)
      Listfeld.PaintPicture gfx.traeger(ll.traeger).Picture, x3 + tpx, Y
      Y = Y + 15 * tpy
      Listfeld.Line (0, Y1)-(X0, Y1), 14474460
    Else
      Exit For
    End If
    If Y > y0 Then Exit For
  Next
  Listfeld.Line (0, Y)-(X0, y0), RGB(255, 255, 255), BF
Else
  Listfeld.Line (0, 0)-(X0, y0), RGB(255, 255, 255), BF
End If
Listfeld.Line (X1, 0)-(X1, y0), 11842740
'listfeld.Line (X1 + tpx, 0)-(X1 + tpx, y0), 14474460
Listfeld.Line (X2, 0)-(X2, y0), 11842740
'listfeld.Line (X2 + tpx, 0)-(X2 + tpx, y0), 14474460
Listfeld.Line (x3, 0)-(x3, y0), 11842740
'listfeld.Line (x3 + tpx, 0)-(x3 + tpx, y0), 14474460

End Sub

Sub QuickSearchPrint()
quicksearchprinting = True
quicksearch.Caption = Lang(36) + ": " + Lang(148)
druckmsg.Show
druckmsg.abbrechen.tag = "false"
LangPar 1, quicksearchinterpret + ": " + quicksearchtitel
druckmsg.Caption = Lang(151)
druckmsg.textfeld.Caption = Lang(152)
druckmsg.queue.Visible = False

WMStat_Clear (Lang(152))
DoEvents
SelectPrinter (drucker)
Printer.Orientation = 1
Printer.FontName = "Arial"
h = Printer.Height: w = Printer.Width
Y = 0
ptpx = Printer.TwipsPerPixelX
ptpy = Printer.TwipsPerPixelY
pp& = 1
Do
  If Y = 0 Then
    Printer.FontTransparent = True
    Printer.FontSize = 10
    Printer.FontName = "Arial"
    Printer.FontTransparent = True
    Printer.Print
    Printer.FontSize = 16
    Printer.FontBold = True
    Printer.ForeColor = 0
    a$ = Programm + " " + Lang(154)
    X = (w - Printer.TextWidth(a$)) / 2
    Printer.CurrentX = X
    Printer.Print a$
    Printer.FontSize = 12: Printer.FontBold = False
    a$ = Lang(71) + ": " + quicksearchinterpret + ", " + Lang(72) + ": " + quicksearchtitel
    X = (w - Printer.TextWidth(a$)) / 2
    Printer.CurrentX = X
    Printer.Print a$
    Printer.FontSize = 8
    Y1 = Printer.CurrentY + 283
    Printer.Line (1134, Y1)-(w - 567, Y1 + 250), PrinterRGB(128, 128, 128), BF
    Printer.Line (1134, Y1)-(w - 567, Y1 + 250), 0, B
    Printer.CurrentY = Y1 + 25
    Printer.CurrentX = 1134 + 100
    Printer.FontBold = True
    Printer.Print "Pos.";
    Printer.CurrentX = 1701 + 100
    Printer.Print "Interpret: Titel ";
    Printer.ForeColor = PrinterRGB(255, 0, 0)
    Printer.Print "(Version)";: Printer.ForeColor = 0
    Printer.CurrentX = w - 567 - 1984 + 100
    Printer.Print "Länge";
    Printer.CurrentX = w - 567 - 1417 + 100
    Printer.Print "Tonträger"
    Printer.FontBold = False
    Y = Y1 + 250
  End If
  Get #Searchfile, pp&, ll
  a$ = Format$(pp&): Printer.CurrentY = Y + 30
  Printer.CurrentX = 1701 - 100 - Printer.TextWidth(a$)
  Printer.Print a$;
  Printer.CurrentX = 1701 + 100
  Printer.Print Trim$(ll.Interpret_Titel); " ";
  Printer.ForeColor = PrinterRGB(255, 0, 0)
  Printer.Print "("; Trim$(ll.version); ")";
  Printer.ForeColor = 0
  Printer.Line (w - 567 - 1984, Y + 30)-(w, Y + 250), RGB(255, 255, 255), BF
  Printer.CurrentY = Y + 30
  a$ = Format$(ll.laenge, "0.00")
  Printer.CurrentX = w - 1984 - 100 - Printer.TextWidth(a$)
  Printer.Print a$;
  Printer.CurrentX = w - 1984 + 350
  Printer.Print Format$(ll.Index); " ";
  Printer.Print Chr$(64 + ll.Seite); "-"; Format$(ll.track)
  If Farbdruck = True Then
    Printer.PaintPicture gfx.traeger(ll.traeger).Picture, w - 1984 + 25, Y + 25
  Else
    Printer.PaintPicture gfx.traegerbw(ll.traeger).Picture, w - 1984 + 25, Y + 25
  End If
 
  pp& = pp& + 1: Y = Y + 250
  Printer.Line (1134, Y)-(w - 567, Y), 0
  If pp& > SearchlistLen Or Y + 250 > h - 1500 Then
    Printer.Line (1134, Y1)-(1701, Y), 0, B
    Printer.Line (w - 567 - 1984, Y1)-(w - 567 - 1417, Y), 0, B
    Printer.Line (1134, Y1)-(w - 567, Y), 0, B
    Printer.CurrentY = h - 1500
    Printer.CurrentX = 1134
    Printer.FontSize = 7
    Printer.Print "Printed by "; Programm; " "; version; ", Released "; Progdate
    Y = 0
    If pp& <= SearchlistLen Then Printer.NewPage
  End If
  DoEvents
  If druckmsg.abbrechen.tag = "true" Then
    ma = MsgBox(Lang(153), 36, Lang(151))
    If ma = 6 Then
      Printer.KillDoc
      Exit Do
    End If
    druckmsg.abbrechen.tag = "false"
  End If
Loop While pp& <= SearchlistLen
If druckmsg.abbrechen.tag <> "true" Then
  Printer.EndDoc
End If
druckmsg.abbrechen.tag = "false"
Unload druckmsg
quicksearch.Caption = Lang(36)
quicksearchprinting = False
End Sub

Sub RunSearch()
On Error Resume Next
Dim mxp As Long, pp As Long
Dim p As Integer, lp As Integer
Dim sl As SearchListe
Dim chk As Byte
Dim ret As Integer
Dim ssTi As Titeldatei


'quicksearch.ok.Default = False
'quicksearch.stop.Default = True
'quicksearch.stop.Enabled = True
'quicksearch.ok.Enabled = False
SearchlistLen = 0
Listfeld.Refresh: DoEvents
ListScroll.value = 0
quicksearchinterpret = Trim$(quicksearch.z1.text)
quicksearchtitel = Trim$(quicksearch.z2.text)
s1$ = LCase$(Trim$(quicksearch.z1.text))
s2$ = LCase$(Trim$(quicksearch.z2.text))
searchrunning = True
Close Searchfile
Kill tmp + "~wm2srch.tmp"
Error = 0
Searchfile = FreeFile
Open tmp + "~wm2srch.tmp" For Random As #Searchfile Len = Len(sl)
ss = FreeFile
Open TiDatei For Random As #ss Len = Len(sTi)
mxp = LOF(ss) / Len(sTi)
Close #ss

pp = mxp: p = 0: lp = -1
If pp > 0 Then
If s1$ = "" And s2$ <> "" Then
  Do
    'Stop
    ret = GetWMTitle(pp, sTi)
    If ret = 0 Then
    
        'Get #ss, pp, sTi
    
        p = (mxp - pp) / mxp * 100
        If p <> lp Then lp = p: quicksearch.queue.value = lp
        If InStr(LCase$(sTi.titel), s2$) > 0 Then AddSearch
    End If
    If StopSearch = True Then StopSearch = False: Exit Do
    If (pp Mod 10) = 0 Then DoEvents
    pp = pp - 1
  Loop Until pp < 1
Else
  Dim si As SortInterpret
  Dim ie As InterpretenEintrag
  
  ff = FreeFile
  Open tmp + "\wmsearch.ndx" For Binary As #ff
  a$ = String(1000, 0)
  For i& = 1 To mxp Step 1000
    Put #ff, i&, a$
  Next
  ndx = FreeFile
  Open DatenPfad + "\sort.dat" For Random As #ndx Len = Len(ie)
  f$ = DatenPfad + "\sort" + left$(s1$, 1) + ".ndx"
  ii = FreeFile
  Open f$ For Random As #ii Len = Len(si)
  For i& = 1 To LOF(ii) / Len(si)
    Get #ii, i&, si
    If left$(LCase(si.interpret), Len(s1$)) = s1$ Then
      ppp& = si.LetzterEintrag
      chk = 1
      Do
        Get #ndx, ppp&, ie
        ppp& = ie.Letzter
        'Stop
        'Get #ss, ie.nummer, sTi
        ret = GetWMTitle(ie.nummer, sTi)
        If ret = 0 Then
            'Get #ss, ie.nummer, ssTi
            If s2$ = "" Or InStr(LCase$(sTi.titel), s2$) > 0 Then
                Put #ff, ie.nummer, chk
                AddSearch
            End If
        End If
      Loop Until ppp& = 0
      
    End If
    If (i Mod 10) = 0 Then DoEvents
  Next
  Close #ii
  Close #ndx
  Do
    'Get #ss, pp, sTi
    'Stop
    ret = GetWMTitle(pp, sTi)
    p = (mxp - pp) / mxp * 100
    If p <> lp Then lp = p: quicksearch.queue.value = lp
    If ret = 0 Then
        If InStr(LCase$(sTi.interpret), s1$) > 0 Then
            If s2$ = "" Or InStr(LCase$(sTi.titel), s2$) > 0 Then
                Get #ff, pp, chk
                If chk = 0 Then AddSearch
            End If
        End If
    End If
    If StopSearch = True Then StopSearch = False: Exit Do
    'If (pp And 1) = 1 Then DoEvents
    pp = pp - 1
  Loop Until pp < 1
  Close #ff
  Kill tmp + "\wmsearch.ndx"
End If
End If
'Close #ss
startstop.Caption = "Start"
searchrunning = False
DoEvents
'quicksearch.ok.Enabled = True
'quicksearch.stop.Default = False
'quicksearch.stop.Enabled = False
'quicksearch.ok.Default = True


End Sub


Sub StartSearch()
StopSearchNow

If Trim$(z1.text) <> "" Or Trim$(z2.text) <> "" Then
  RunSearch
End If
End Sub

Sub StopSearchNow()
If searchrunning = True Then
    StopSearch = True
    Do While searchrunning = True
        DoEvents
    Loop
    startstop.Caption = "Start"
End If
End Sub

Private Sub drucken_Click()
If searchrunning = True Then
  MsgBox Lang(143), 64, Lang(144)
ElseIf SearchlistLen < 1 Then
  MsgBox Lang(145), 64, Lang(144)
Else
  If quicksearchprinting = True Then
    MsgBox Lang(147), 64, Lang(144)
  Else
    QuickSearchPrint
  End If
End If
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
If KeyCode = 27 Or KeyCode = 114 Then
  If searchrunning = True Then StopSearch = True
  quicksearchvisible = False
  Unload quicksearch
End If

End Sub

Private Sub Form_Load()
Dim rec As Rect
LastClickedTitle = 0
openini AppData + "winmusik2.ini"
selectTopic "Fenster"
a$ = GetINI$("Quicksearch")
If inierror = 0 Then
  String2Rect a$, rec
Else
  rec.left = (Screen.Width - 6060) / 2: rec.top = (Screen.Height - 2910) / 2
  rec.right = 6060: rec.bottom = 2910
End If
For i = 0 To 7
    a$ = GetINI$("Search_Object_" + Format(i))
    If inierror = 0 Then Check1(i).value = a$
    Check1(i).ToolTipText = Lang(350 + i)
Next
If rec.left > Screen.Width - quicksearch.Width Then rec.left = Screen.Width - quicksearch.Width
If rec.top > Screen.Height - quicksearch.Height Then rec.top = Screen.Height - quicksearch.Height
quicksearch.left = rec.left: quicksearch.top = rec.top
quicksearch.Width = rec.right: quicksearch.Height = rec.bottom

quicksearchvisible = True
z1.text = ""
z2.text = ""
'quicksearch.queue.FloodColor = RGB(220, 220, 250)

quicksearch.Caption = Lang(36)
Label1.Caption = Lang(71) + ":"
Label2.Caption = Lang(72) + ":"
LTrack.Caption = Lang(142)
a$ = " " + Lang(71) + ": " + Lang(72)
LTitel.Caption = a$
Listfeld.FontName = "Arial"
Listfeld.FontSize = 8
Listfeld.FontBold = False
'Label3.left = LTitel.left + Listfeld.TextWidth(a$) - 20 * tpx
ShowTiIndex = False
'Label3.Caption = "(" + Lang(73) + ")"
LLaenge.Caption = " " + Lang(75)
Ltraeger.Caption = " " + Lang(87)

End Sub



Private Sub Form_Resize()
If quicksearch.WindowState <> 1 Then
    queue.Width = StatusBar1.Panels(1).Width - 120
    queue.top = StatusBar1.top + 60
  If quicksearch.Width < 6060 Then quicksearch.Width = 6060
  If quicksearch.Height < 2910 Then quicksearch.Height = 2910
  DrawSearch
End If

End Sub

Private Sub Form_Unload(Cancel As Integer)
quicksearchvisible = False
SearchlistLen = 0
If searchrunning = True Then StopSearch = True
DoEvents
Close Searchfile
Dim rec As Rect
openini AppData + "winmusik2.ini"
selectTopic "Fenster"
rec.top = quicksearch.top: rec.left = quicksearch.left
rec.right = quicksearch.Width: rec.bottom = quicksearch.Height
a$ = Rect2String(rec)
putini "Quicksearch=" + a$
For i = 0 To 7
    putini "Search_Object_" + Format(i) + "=" + Format(Check1(i).value)
Next

closeini
End Sub


Private Sub TabStrip1_Click()

End Sub


Private Sub Listfeld_DblClick()
Dim file As String
If LastClickedTitle > 0 Then
    file = GetMp3FileName(ll.Index, (ll.track))
    'file = GetMp3FileName(ll.Index, t)
    If eingabevisible = True Then
        If EingabePos = 0 Or EingabePos = 1 Then
            eingabe.e(0).text = Format(LastClickedTitle)
            eingabe.LoadTitleIntoCurrentTrack
            CurrentTi.nummer = 0
            Ti.nummer = 0
            eingabe.TiNummer.Caption = Lang(118)
            eingabe.SetFocus
        Else
            If file <> "" Then
                ret = ShellExecute(Me.hwnd, "open", file, "", "", SW_SHOWNORMAL)
            End If

        End If
    Else
        If file <> "" Then
            ret = ShellExecute(Me.hwnd, "open", file, "", "", SW_SHOWNORMAL)
        End If
    End If
End If

End Sub

Private Sub Listfeld_DragDrop(Source As Control, X As Single, Y As Single)
Stop
Debug.Print "Listfeld_DragDrop ", X, Y, State
If LastClickedTitle > 0 Then
    
    Debug.Print Trim(ll.Interpret_Titel); ", "; Trim(ll.version); ", "; ll.Titelnummer
End If


End Sub

Private Sub Listfeld_DragOver(Source As Control, X As Single, Y As Single, State As Integer)
Stop
Debug.Print "Listfeld_DragOver ", X, Y, State
If LastClickedTitle > 0 Then
    Debug.Print Trim(ll.Interpret_Titel); ", "; Trim(ll.version); ", "; ll.Titelnummer
End If


End Sub

Private Sub Listfeld_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
'Debug.Print X, Y, Button
Dim y0 As Long, hoehe As Long, i As Long
LastClickedTitle = 0
If SearchlistLen > 0 Then
    y0 = Listfeld.Height
    hoehe = 15 * tpy
    i = Int(Y / hoehe) + ListScroll.value + 1
    'Debug.Print i
    If i <= SearchlistLen Then
        Get #Searchfile, i, ll
        Clipboard.Clear
        Clipboard.SetText (Trim(ll.Interpret_Titel) + " (" + Format(ll.laenge, "0.00") + " min, " + Trim(ll.version) + ")")
        Debug.Print Trim(ll.Interpret_Titel); ", "; Trim(ll.version); ", "; ll.Titelnummer
        LastClickedTitle = ll.Titelnummer
        'Listfeld.Drag vbBeginDrag
        x3 = Ltraeger.left - 8 * tpx
        OleDragPosition.X = X
        OleDragPosition.Y = Y
        
        If (ll.traeger = 7 And X >= x3) Or X < x3 Then
            Listfeld.OLEDragMode = 1
            Listfeld.OLEDrag
        End If
        
    End If
End If

End Sub



Private Sub Listfeld_OLECompleteDrag(Effect As Long)
Debug.Print "OLECompleteDrop"
Listfeld.OLEDragMode = 0
End Sub

Private Sub Listfeld_OLEDragDrop(Data As DataObject, Effect As Long, Button As Integer, Shift As Integer, X As Single, Y As Single)
Debug.Print "OLEDragDrop"
End Sub


Private Sub Listfeld_OLEDragOver(Data As DataObject, Effect As Long, Button As Integer, Shift As Integer, X As Single, Y As Single, State As Integer)
Debug.Print "OLEDragOver"
End Sub

Private Sub Listfeld_OLEGiveFeedback(Effect As Long, DefaultCursors As Boolean)
Debug.Print "OLEGiveFeedback"
End Sub

Private Sub Listfeld_OLESetData(Data As DataObject, DataFormat As Integer)
Debug.Print "OLESetData"
End Sub


Private Sub Listfeld_OLEStartDrag(Data As DataObject, AllowedEffects As Long)
Dim file As String
Dim t As Long
Dim x3 As Integer
Dim d As Variant

x3 = Ltraeger.left - 8 * tpx

Debug.Print "OLEStartDrag"

Data.Clear
d = "WM2###" + Format(ll.Titelnummer) + "###" + Trim(ll.Interpret_Titel) + "###" + Trim(ll.version) + "###" & ll.laenge & "###"
Data.SetData d, vbCFText

If ll.traeger = 7 Then
    t = ll.track
    file = GetMp3FileName(ll.Index, t)
    Data.files.Clear
    Data.files.Add file
    Data.SetData , vbCFFiles
End If

End Sub


Private Sub ListFeld_Paint()
FillOutSearch
End Sub


Private Sub ListScroll_Change()
Listfeld.Refresh
End Sub

Private Sub ListScroll_Scroll()
Listfeld.Refresh
End Sub

Private Sub Llaenge_Click()
z1.SetFocus
End Sub

Private Sub LTitel_Click()
z1.SetFocus

Listfeld.FontName = "Arial"
Listfeld.FontSize = 8
Listfeld.FontBold = False
If ShowTiIndex = False Then
  a$ = " (Index) " + Lang(71) + ": " + Lang(72)
  LTitel.Caption = a$
  ShowTiIndex = True
Else
  a$ = " " + Lang(71) + ": " + Lang(72)
  LTitel.Caption = a$
  ShowTiIndex = False
End If
'Label3.left = LTitel.left + Listfeld.TextWidth(a$) - 20 * tpx
Listfeld.Refresh
End Sub

Private Sub LTrack_Click()
z1.SetFocus
End Sub


Private Sub Ltraeger_Click()
z1.SetFocus
End Sub

Private Sub ok_Click()
If searchrunning = True Then
  StopSearch = True
  Do While searchrunning = True
    DoEvents
  Loop
End If
If Trim$(z1.text) <> "" Or Trim$(z2.text) <> "" Then
  RunSearch
Else
  MsgBox Lang(146), 64, Lang(36)
End If
End Sub

Private Sub ScrollBar1_Change()
Listfeld.Refresh
End Sub

Private Sub ScrollBar1_Scroll()
Listfeld.Refresh
End Sub


Private Sub startstop_Click()
If searchrunning = True Then
  StopSearch = True
  Do While searchrunning = True
    DoEvents
  Loop
  If SearchlistLen > 0 Then Drucken.Enabled = True Else Drucken.Enabled = False
  startstop.Caption = "Start"
  DoEvents
Else
    If Trim$(z1.text) <> "" Or Trim$(z2.text) <> "" Then
        Drucken.Enabled = False
        startstop.Caption = "Stop"
        DoEvents
        RunSearch
        If SearchlistLen > 0 Then Drucken.Enabled = True Else Drucken.Enabled = False
    Else
        MsgBox Lang(146), 64, Lang(36)
    End If
End If
End Sub

Private Sub stop_Click()
If searchrunning = True Then StopSearch = True
End Sub


Private Sub TextBox1_Change()

End Sub



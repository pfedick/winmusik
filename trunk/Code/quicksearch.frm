VERSION 4.00
Begin VB.Form quicksearch 
   Caption         =   "Quick-Search"
   ClientHeight    =   2505
   ClientLeft      =   7800
   ClientTop       =   1470
   ClientWidth     =   7335
   FillColor       =   &H00C00000&
   Height          =   2910
   Icon            =   "quicksearch.frx":0000
   Left            =   7740
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   ScaleHeight     =   2505
   ScaleWidth      =   7335
   Top             =   1125
   Width           =   7455
   Begin VB.TextBox z2 
      Height          =   315
      Left            =   600
      TabIndex        =   1
      Text            =   "Text1"
      Top             =   435
      Width           =   4335
   End
   Begin VB.TextBox z1 
      Height          =   315
      Left            =   960
      TabIndex        =   0
      Text            =   "Text1"
      Top             =   60
      Width           =   3975
   End
   Begin Threed.SSPanel ListPanel 
      Height          =   1575
      Left            =   60
      TabIndex        =   4
      Top             =   840
      Width           =   7110
      _Version        =   65536
      _ExtentX        =   12541
      _ExtentY        =   2778
      _StockProps     =   15
      ForeColor       =   -2147483640
      BackColor       =   12632256
      BevelWidth      =   2
      BevelOuter      =   1
      Begin VB.VScrollBar ListScroll 
         Height          =   1515
         LargeChange     =   5
         Left            =   6810
         TabIndex        =   6
         Top             =   30
         Width           =   255
      End
      Begin VB.PictureBox Listfeld 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BorderStyle     =   0  'None
         ForeColor       =   &H80000008&
         Height          =   1260
         Left            =   30
         ScaleHeight     =   1260
         ScaleWidth      =   6780
         TabIndex        =   5
         Top             =   270
         Width           =   6780
      End
      Begin Threed.SSPanel Llaenge 
         Height          =   240
         Left            =   5250
         TabIndex        =   7
         Top             =   30
         Width           =   600
         _Version        =   65536
         _ExtentX        =   1058
         _ExtentY        =   423
         _StockProps     =   15
         Caption         =   " Länge"
         ForeColor       =   -2147483640
         BackColor       =   12632256
         Alignment       =   1
      End
      Begin Threed.SSPanel Ltraeger 
         Height          =   240
         Left            =   5850
         TabIndex        =   8
         Top             =   30
         Width           =   960
         _Version        =   65536
         _ExtentX        =   1693
         _ExtentY        =   423
         _StockProps     =   15
         Caption         =   " Tonträger"
         ForeColor       =   -2147483640
         BackColor       =   12632256
         Alignment       =   1
      End
      Begin Threed.SSPanel LTitel 
         Height          =   240
         Left            =   435
         TabIndex        =   9
         Top             =   30
         Width           =   4815
         _Version        =   65536
         _ExtentX        =   8493
         _ExtentY        =   423
         _StockProps     =   15
         Caption         =   " Interpret: Titel"
         ForeColor       =   -2147483640
         BackColor       =   12632256
         Alignment       =   1
         Begin VB.Label Label3 
            AutoSize        =   -1  'True
            BackStyle       =   0  'Transparent
            Caption         =   "(Version)"
            ForeColor       =   &H000000C0&
            Height          =   195
            Left            =   1140
            TabIndex        =   11
            Top             =   15
            Width           =   615
         End
      End
      Begin Threed.SSPanel LTrack 
         Height          =   240
         Left            =   30
         TabIndex        =   10
         Top             =   30
         Width           =   405
         _Version        =   65536
         _ExtentX        =   714
         _ExtentY        =   423
         _StockProps     =   15
         Caption         =   "Pos."
         ForeColor       =   -2147483640
         BackColor       =   12632256
      End
   End
   Begin Threed.SSPanel queue 
      Height          =   330
      Left            =   5010
      TabIndex        =   14
      Top             =   435
      Width           =   1065
      _Version        =   65536
      _ExtentX        =   1879
      _ExtentY        =   582
      _StockProps     =   15
      Caption         =   "SSPanel1"
      BackColor       =   12632256
      FloodType       =   1
      FloodColor      =   12582912
   End
   Begin Threed.SSCommand drucken 
      Height          =   330
      Left            =   6135
      TabIndex        =   15
      Top             =   435
      Width           =   1035
      _Version        =   65536
      _ExtentX        =   1826
      _ExtentY        =   582
      _StockProps     =   78
      RoundedCorners  =   0   'False
      Outline         =   0   'False
      Picture         =   "quicksearch.frx":030A
   End
   Begin Threed.SSCommand stop 
      Height          =   330
      Left            =   6135
      TabIndex        =   13
      Top             =   60
      Width           =   1035
      _Version        =   65536
      _ExtentX        =   1826
      _ExtentY        =   582
      _StockProps     =   78
      RoundedCorners  =   0   'False
      Outline         =   0   'False
      Picture         =   "quicksearch.frx":0524
   End
   Begin Threed.SSCommand ok 
      Default         =   -1  'True
      Height          =   330
      Left            =   5010
      TabIndex        =   12
      Top             =   60
      Width           =   1065
      _Version        =   65536
      _ExtentX        =   1879
      _ExtentY        =   582
      _StockProps     =   78
      RoundedCorners  =   0   'False
      Outline         =   0   'False
      Picture         =   "quicksearch.frx":0768
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "Titel:"
      BeginProperty Font 
         name            =   "MS Sans Serif"
         charset         =   0
         weight          =   700
         size            =   8.25
         underline       =   0   'False
         italic          =   0   'False
         strikethrough   =   0   'False
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
         name            =   "MS Sans Serif"
         charset         =   0
         weight          =   700
         size            =   8.25
         underline       =   0   'False
         italic          =   0   'False
         strikethrough   =   0   'False
      EndProperty
      Height          =   195
      Left            =   60
      TabIndex        =   2
      Top             =   120
      Width           =   795
   End
End
Attribute VB_Name = "quicksearch"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
DefInt A-Z
Dim stopsearch As Boolean
Dim searchrunning As Boolean
Dim sTi As Titeldatei
Dim SearchlistLen As Long
Dim Searchfile As Integer
Dim ll As SearchListe
Dim quicksearchprinting As Boolean
Dim quicksearchinterpret As String
Dim quicksearchtitel As String
Dim ShowTiIndex As Boolean
Sub AddSearch()
If DebugLevel > 5 Then
  Debug.Print "AddSearch: "; Trim$(sTi.interpret); ": "; Trim$(sTi.Titel)
End If
Dim sl As SearchListe

sl.Titelnummer = sTi.Nummer
sl.Interpret_Titel = Trim$(sTi.interpret) + ": " + Trim$(sTi.Titel)
sl.version = Trim$(GetTab("version.bez", sTi.version))
sl.Laenge = sTi.Laenge
sl.Traeger = sTi.Traeger
If sl.Traeger > 6 Or sl.Traeger < 0 Then sl.Traeger = 6
sl.Index = sTi.Index
sl.Seite = sTi.Seite
sl.Track = sTi.Track
SearchlistLen = SearchlistLen + 1
Put #Searchfile, SearchlistLen, sl
ListScroll.Max = SearchlistLen
If ListFeld.Height / (15 * tpy) >= SearchlistLen - 1 Then FillOutSearch
End Sub

Sub DrawSearch()
drucken.left = Width - drucken.Width - 12 * tpx
quicksearch.stop.left = drucken.left
ok.left = drucken.left - ok.Width - 4 * tpx
queue.left = ok.left
z1.Width = ok.left - 4 * tpx - z1.left
z2.Width = ok.left - 4 * tpx - z2.left
ListPanel.Width = Width - 16 * tpx
ListPanel.Height = Height - ListPanel.top - 32 * tpy
ListScroll.left = ListPanel.Width - ListScroll.Width - 2 * tpx
ListScroll.Height = ListPanel.Height - 4 * tpy
Ltraeger.left = ListScroll.left - Ltraeger.Width
llaenge.left = Ltraeger.left - llaenge.Width
LTitel.Width = llaenge.left - LTitel.left
ListFeld.Width = ListPanel.Width - ListScroll.Width - 5 * tpx
ListFeld.Height = ListPanel.Height - ListFeld.top - 3 * tpy
End Sub

Sub FillOutSearch()
On Error Resume Next
f& = RGB(220, 220, 220)
y0 = ListFeld.Height: X0 = ListFeld.Width
X1 = LTitel.left - 3 * tpx
X2 = llaenge.left - 3 * tpx
x3 = Ltraeger.left - 3 * tpx
If SearchlistLen > 0 Then
  Y = 0
  For i = ListScroll.Value + 1 To SearchlistLen
    If i <= SearchlistLen Then
      Get #Searchfile, i, ll
      If Err > 0 Then
        Debug.Print "Unnormal Exit in Module quicksearch.FillOutSearch!"
        Debug.Print "Searchfile="; Searchfile; ", i="; i; ", l1="; l1
        Exit Sub
      End If
      Y1 = Y + 14 * tpy
      ListFeld.Line (0, Y)-(X1, Y1), RGB(255, 255, 255), BF
      a$ = Format$(i)
      ListFeld.CurrentX = X1 - ListFeld.TextWidth(a$) - 3 * tpx: ListFeld.CurrentY = Y
      ListFeld.ForeColor = 0
      ListFeld.Print a$;
      ListFeld.Line (X1, Y)-(X2, Y1), RGB(255, 255, 255), BF
      ListFeld.CurrentX = X1 + 4 * tpx: ListFeld.CurrentY = Y: ListFeld.ForeColor = 0
      If ShowTiIndex = True Then ListFeld.Print "(" + Format$(ll.Titelnummer) + ") ";
      ListFeld.FontBold = True
      ListFeld.Print Trim$(ll.Interpret_Titel);
      ListFeld.FontBold = False
      ListFeld.ForeColor = 192
      ListFeld.Print " ("; Trim$(ll.version); ")";
      ListFeld.Line (X2, Y)-(X0, Y1), RGB(255, 255, 255), BF
      ListFeld.CurrentY = Y: ListFeld.ForeColor = 0
      a$ = Format$(ll.Laenge, "0.00"): If ll.Laenge = 0 Then a$ = "---"
      ListFeld.CurrentX = x3 - ListFeld.TextWidth(a$) - 3 * tpx: ListFeld.CurrentY = Y
      ListFeld.Print a$;
      ListFeld.CurrentX = x3 + 17 * tpx
      ListFeld.Print Format$(ll.Index); " ";
      ListFeld.Print Chr$(64 + ll.Seite); "-"; Format$(ll.Track)
      ListFeld.PaintPicture gfx.Traeger(ll.Traeger).Picture, x3 + tpx, Y
      Y = Y + 15 * tpy
      ListFeld.Line (0, Y1)-(X0, Y1), 14474460
    Else
      Exit For
    End If
    If Y > y0 Then Exit For
  Next
  ListFeld.Line (0, Y)-(X0, y0), RGB(255, 255, 255), BF
Else
  ListFeld.Line (0, 0)-(X0, y0), RGB(255, 255, 255), BF
End If
ListFeld.Line (X1, 0)-(X1, y0), 11842740
ListFeld.Line (X1 + tpx, 0)-(X1 + tpx, y0), 14474460
ListFeld.Line (X2, 0)-(X2, y0), 11842740
ListFeld.Line (X2 + tpx, 0)-(X2 + tpx, y0), 14474460
ListFeld.Line (x3, 0)-(x3, y0), 11842740
ListFeld.Line (x3 + tpx, 0)-(x3 + tpx, y0), 14474460

End Sub

Sub QuickSearchPrint()
quicksearchprinting = True
quicksearch.Caption = Lang(36) + ": " + Lang(148)
druckmsg.Show
druckmsg.abbrechen.Tag = "false"
LangPar 1, quicksearchinterpret + ": " + quicksearchtitel
druckmsg.Caption = Lang(151)
druckmsg.textfeld.Caption = Lang(152)
DoEvents
SelectPrinter (Drucker)
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
  a$ = Format$(ll.Laenge, "0.00")
  Printer.CurrentX = w - 1984 - 100 - Printer.TextWidth(a$)
  Printer.Print a$;
  Printer.CurrentX = w - 1984 + 350
  Printer.Print Format$(ll.Index); " ";
  Printer.Print Chr$(64 + ll.Seite); "-"; Format$(ll.Track)
  If Farbdruck = True Then
    Printer.PaintPicture gfx.Traeger(ll.Traeger).Picture, w - 1984 + 25, Y + 25
  Else
    Printer.PaintPicture gfx.Traegerbw(ll.Traeger).Picture, w - 1984 + 25, Y + 25
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
  If druckmsg.abbrechen.Tag = "true" Then
    ma = MsgBox(Lang(153), 36, Lang(151))
    If ma = 6 Then
      Printer.KillDoc
      Exit Do
    End If
    druckmsg.abbrechen.Tag = "false"
  End If
Loop While pp& <= SearchlistLen
If druckmsg.abbrechen.Tag <> "true" Then
  Printer.EndDoc
End If
druckmsg.abbrechen.Tag = "false"
Unload druckmsg
quicksearch.Caption = Lang(36)
quicksearchprinting = False
End Sub

Sub RunSearch()
On Error Resume Next
Dim mxp As Long, pp As Long
Dim p As Integer, lp As Integer
Dim sl As SearchListe

quicksearch.ok.Default = False
quicksearch.stop.Default = True
quicksearch.ok.Enabled = False
SearchlistLen = 0
ListFeld.Refresh: DoEvents
ListScroll.Value = 0
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
pp = mxp: p = 0: lp = -1
If pp > 0 Then
If s1$ <> "" And s2$ = "" Then
  Do
    Get #ss, pp, sTi
    p = (mxp - pp) / mxp * 100
    If p <> lp Then lp = p: quicksearch.queue.FloodPercent = lp
    If InStr(LCase$(sTi.interpret), s1$) > 0 Then AddSearch
    If stopsearch = True Then stopsearch = False: Exit Do
    DoEvents
    pp = pp - 1
  Loop Until pp < 1
ElseIf s1$ = "" And s2$ <> "" Then
  Do
    Get #ss, pp, sTi
    p = (mxp - pp) / mxp * 100
    If p <> lp Then lp = p: quicksearch.queue.FloodPercent = lp
    If InStr(LCase$(sTi.Titel), s2$) > 0 Then AddSearch
    If stopsearch = True Then stopsearch = False: Exit Do
    DoEvents
    pp = pp - 1
  Loop Until pp < 1
Else
  Do
    Get #ss, pp, sTi
    p = (mxp - pp) / mxp * 100
    If p <> lp Then lp = p: quicksearch.queue.FloodPercent = lp
    If InStr(LCase$(sTi.interpret), s1$) > 0 And InStr(LCase$(sTi.Titel), s2$) > 0 Then AddSearch
    If stopsearch = True Then stopsearch = False: Exit Do
    DoEvents
    pp = pp - 1
  Loop Until pp < 1
End If
End If
Close #ss
searchrunning = False
quicksearch.ok.Enabled = True
quicksearch.stop.Default = False
quicksearch.ok.Default = True

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

Private Sub Form_Load()
Dim rec As Rect
openini "wmusik20"
selectTopic "Fenster"
a$ = GetINI$("Quicksearch")
If inierror = 0 Then
  String2Rect a$, rec
Else
  rec.left = (Screen.Width - 6060) / 2: rec.top = (Screen.Height - 2910) / 2
  rec.right = 6060: rec.bottom = 2910
End If
If rec.left > Screen.Width - quicksearch.Width Then rec.left = Screen.Width - quicksearch.Width
If rec.top > Screen.Height - quicksearch.Height Then rec.top = Screen.Height - quicksearch.Height
quicksearch.left = rec.left: quicksearch.top = rec.top
quicksearch.Width = rec.right: quicksearch.Height = rec.bottom

quicksearchvisible = True
z1.text = ""
z2.text = ""
quicksearch.queue.FloodColor = RGB(220, 220, 250)

quicksearch.Caption = Lang(36)
Label1.Caption = Lang(71)
Label2.Caption = Lang(72)
LTrack.Caption = Lang(142)
a$ = " " + Lang(71) + ": " + Lang(72)
LTitel.Caption = a$
ListFeld.FontName = "Arial"
ListFeld.FontSize = 8
ListFeld.FontBold = False
label3.left = LTitel.left + ListFeld.TextWidth(a$) - 20 * tpx
ShowTiIndex = False
label3.Caption = "(" + Lang(73) + ")"
llaenge.Caption = " " + Lang(75)
Ltraeger.Caption = " " + Lang(87)
End Sub


Private Sub Form_Resize()
If quicksearch.WindowState <> 1 Then
  If quicksearch.Width < 6060 Then quicksearch.Width = 6060
  If quicksearch.Height < 2910 Then quicksearch.Height = 2910
  DrawSearch
End If

End Sub

Private Sub Form_Unload(Cancel As Integer)
quicksearchvisible = False
SearchlistLen = 0
If searchrunning = True Then stopsearch = True
DoEvents
Close Searchfile
Dim rec As Rect
openini "wmusik20"
selectTopic "Fenster"
rec.top = quicksearch.top: rec.left = quicksearch.left
rec.right = quicksearch.Width: rec.bottom = quicksearch.Height
a$ = Rect2String(rec)
putini "Quicksearch=" + a$
closeini
End Sub


Private Sub TabStrip1_Click()

End Sub


Private Sub ListFeld_Paint()
FillOutSearch
End Sub


Private Sub ListScroll_Change()
ListFeld.Refresh
End Sub

Private Sub LTitel_Click()
ListFeld.FontName = "Arial"
ListFeld.FontSize = 8
ListFeld.FontBold = False
If ShowTiIndex = False Then
  a$ = " (Index) " + Lang(71) + ": " + Lang(72)
  LTitel.Caption = a$
  ShowTiIndex = True
Else
  a$ = " " + Lang(71) + ": " + Lang(72)
  LTitel.Caption = a$
  ShowTiIndex = False
End If
label3.left = LTitel.left + ListFeld.TextWidth(a$) - 20 * tpx
ListFeld.Refresh
End Sub

Private Sub ok_Click()
If searchrunning = True Then
  stopsearch = True
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

Private Sub stop_Click()
If searchrunning = True Then stopsearch = True
End Sub



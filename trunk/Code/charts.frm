VERSION 5.00
Object = "{0BA686C6-F7D3-101A-993E-0000C0EF6F5E}#1.0#0"; "THREED32.OCX"
Begin VB.Form charts 
   Caption         =   "Hitparade"
   ClientHeight    =   4785
   ClientLeft      =   1830
   ClientTop       =   1575
   ClientWidth     =   7545
   Icon            =   "charts.frx":0000
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   4785
   ScaleWidth      =   7545
   Begin VB.TextBox e 
      Height          =   315
      Index           =   9
      Left            =   1200
      TabIndex        =   8
      Text            =   "Text1"
      Top             =   4980
      Width           =   555
   End
   Begin VB.PictureBox listfeld 
      BackColor       =   &H00FFFFFF&
      Height          =   2595
      Left            =   0
      ScaleHeight     =   2535
      ScaleWidth      =   7395
      TabIndex        =   15
      Top             =   900
      Width           =   7455
      Begin VB.VScrollBar scroll 
         Height          =   2535
         Left            =   7140
         TabIndex        =   33
         Top             =   0
         Width           =   255
      End
      Begin Threed.SSPanel p 
         Height          =   255
         Index           =   0
         Left            =   0
         TabIndex        =   25
         Top             =   0
         Width           =   375
         _Version        =   65536
         _ExtentX        =   661
         _ExtentY        =   450
         _StockProps     =   15
         Caption         =   "Pos."
         BackColor       =   12632256
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
      End
      Begin Threed.SSPanel p 
         Height          =   255
         Index           =   1
         Left            =   360
         TabIndex        =   26
         Top             =   0
         Width           =   375
         _Version        =   65536
         _ExtentX        =   661
         _ExtentY        =   450
         _StockProps     =   15
         Caption         =   "Pl."
         BackColor       =   12632256
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
      End
      Begin Threed.SSPanel p 
         Height          =   255
         Index           =   2
         Left            =   720
         TabIndex        =   27
         Top             =   0
         Width           =   375
         _Version        =   65536
         _ExtentX        =   661
         _ExtentY        =   450
         _StockProps     =   15
         Caption         =   "Vw."
         BackColor       =   12632256
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
      End
      Begin Threed.SSPanel p 
         Height          =   255
         Index           =   3
         Left            =   1080
         TabIndex        =   28
         Top             =   0
         Width           =   3735
         _Version        =   65536
         _ExtentX        =   6588
         _ExtentY        =   450
         _StockProps     =   15
         Caption         =   "Interpret: Titel"
         BackColor       =   12632256
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Alignment       =   1
      End
      Begin Threed.SSPanel p 
         Height          =   255
         Index           =   4
         Left            =   4800
         TabIndex        =   29
         Top             =   0
         Width           =   435
         _Version        =   65536
         _ExtentX        =   767
         _ExtentY        =   450
         _StockProps     =   15
         Caption         =   "Wo."
         BackColor       =   12632256
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
      End
      Begin Threed.SSPanel p 
         Height          =   255
         Index           =   5
         Left            =   5220
         TabIndex        =   30
         Top             =   0
         Width           =   555
         _Version        =   65536
         _ExtentX        =   979
         _ExtentY        =   450
         _StockProps     =   15
         Caption         =   "Punkte"
         BackColor       =   12632256
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
      End
      Begin Threed.SSPanel p 
         Height          =   255
         Index           =   6
         Left            =   5760
         TabIndex        =   31
         Top             =   0
         Width           =   555
         _Version        =   65536
         _ExtentX        =   979
         _ExtentY        =   450
         _StockProps     =   15
         Caption         =   "Länge"
         BackColor       =   12632256
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
      End
      Begin Threed.SSPanel p 
         Height          =   255
         Index           =   7
         Left            =   6300
         TabIndex        =   32
         Top             =   0
         Width           =   855
         _Version        =   65536
         _ExtentX        =   1508
         _ExtentY        =   450
         _StockProps     =   15
         Caption         =   "Tonträger"
         BackColor       =   12632256
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
      End
   End
   Begin VB.Frame EingabeFeld 
      Height          =   1275
      Left            =   0
      TabIndex        =   14
      Top             =   3480
      Width           =   7515
      Begin VB.TextBox e 
         Height          =   285
         Index           =   8
         Left            =   2220
         TabIndex        =   38
         Text            =   "Text1"
         Top             =   900
         Width           =   1095
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   7
         Left            =   780
         TabIndex        =   36
         Text            =   "Text1"
         Top             =   900
         Width           =   555
      End
      Begin VB.CheckBox allesanzeigen 
         Caption         =   "Alle Plätze anzeigen"
         Height          =   195
         Left            =   5640
         TabIndex        =   20
         Top             =   240
         Value           =   1  'Aktiviert
         Width           =   1755
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   6
         Left            =   1200
         TabIndex        =   7
         Text            =   "Text7"
         Top             =   540
         Width           =   6255
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   5
         Left            =   3780
         TabIndex        =   6
         Text            =   "Text6"
         Top             =   180
         Width           =   675
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   4
         Left            =   2160
         TabIndex        =   5
         Text            =   "Text5"
         Top             =   180
         Width           =   615
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   3
         Left            =   840
         TabIndex        =   4
         Text            =   "Text4"
         Top             =   180
         Width           =   615
      End
      Begin Threed.SSCommand f 
         Height          =   345
         Index           =   7
         Left            =   5520
         TabIndex        =   39
         Top             =   900
         Width           =   615
         _Version        =   65536
         _ExtentX        =   1085
         _ExtentY        =   609
         _StockProps     =   78
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         AutoSize        =   2
      End
      Begin Threed.SSCommand f 
         Height          =   345
         Index           =   12
         Left            =   6840
         TabIndex        =   23
         Top             =   900
         Width           =   615
         _Version        =   65536
         _ExtentX        =   1085
         _ExtentY        =   609
         _StockProps     =   78
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         AutoSize        =   2
      End
      Begin Threed.SSCommand f 
         Height          =   345
         Index           =   11
         Left            =   6180
         TabIndex        =   22
         Top             =   900
         Width           =   615
         _Version        =   65536
         _ExtentX        =   1085
         _ExtentY        =   609
         _StockProps     =   78
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         AutoSize        =   2
      End
      Begin Threed.SSCommand f 
         Height          =   345
         Index           =   4
         Left            =   4800
         TabIndex        =   21
         Top             =   900
         Width           =   615
         _Version        =   65536
         _ExtentX        =   1085
         _ExtentY        =   609
         _StockProps     =   78
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         AutoSize        =   2
      End
      Begin VB.Label t 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Tonträger:"
         Height          =   195
         Index           =   9
         Left            =   1440
         TabIndex        =   37
         Top             =   960
         Width           =   735
      End
      Begin VB.Label t 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Woche:"
         Height          =   195
         Index           =   8
         Left            =   120
         TabIndex        =   35
         Top             =   960
         Width           =   570
      End
      Begin VB.Label t 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Interpret, Titel:"
         Height          =   195
         Index           =   7
         Left            =   120
         TabIndex        =   19
         Top             =   600
         Width           =   1020
      End
      Begin VB.Label t 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Vorwoche:"
         Height          =   195
         Index           =   6
         Left            =   2940
         TabIndex        =   18
         Top             =   240
         Width           =   765
      End
      Begin VB.Label t 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Platz:"
         Height          =   195
         Index           =   5
         Left            =   1680
         TabIndex        =   17
         Top             =   240
         Width           =   390
      End
      Begin VB.Label t 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Position:"
         Height          =   195
         Index           =   4
         Left            =   120
         TabIndex        =   16
         Top             =   240
         Width           =   600
      End
   End
   Begin VB.Frame HeaderFeld 
      Height          =   615
      Left            =   0
      TabIndex        =   9
      Top             =   -60
      Width           =   7515
      Begin VB.TextBox e 
         Height          =   285
         Index           =   2
         Left            =   6420
         MaxLength       =   10
         TabIndex        =   3
         Text            =   "23.11.1996"
         Top             =   180
         Width           =   915
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   1
         Left            =   5400
         MaxLength       =   2
         TabIndex        =   2
         Text            =   "47"
         Top             =   180
         Width           =   375
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   0
         Left            =   4080
         MaxLength       =   4
         TabIndex        =   1
         Text            =   "1995"
         Top             =   180
         Width           =   615
      End
      Begin VB.ComboBox ChartAuswahl 
         Height          =   315
         ItemData        =   "charts.frx":000C
         Left            =   840
         List            =   "charts.frx":0013
         Style           =   2  'Dropdown-Liste
         TabIndex        =   0
         Top             =   180
         Width           =   1935
      End
      Begin Threed.SSCommand f 
         Height          =   345
         Index           =   2
         Left            =   2880
         TabIndex        =   24
         Top             =   180
         Width           =   615
         _Version        =   65536
         _ExtentX        =   1085
         _ExtentY        =   609
         _StockProps     =   78
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         AutoSize        =   2
      End
      Begin VB.Label t 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Datum:"
         Height          =   195
         Index           =   3
         Left            =   5820
         TabIndex        =   13
         Top             =   240
         Width           =   510
      End
      Begin VB.Label t 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Woche:"
         Height          =   195
         Index           =   2
         Left            =   4740
         TabIndex        =   12
         Top             =   240
         Width           =   570
      End
      Begin VB.Label t 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Jahr:"
         Height          =   195
         Index           =   1
         Left            =   3660
         TabIndex        =   11
         Top             =   240
         Width           =   345
      End
      Begin VB.Label t 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Hitparade:"
         Height          =   195
         Index           =   0
         Left            =   60
         TabIndex        =   10
         Top             =   240
         Width           =   735
      End
   End
   Begin VB.Label ChartTitel 
      Alignment       =   2  'Zentriert
      BackStyle       =   0  'Transparent
      Caption         =   "Label9"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   60
      TabIndex        =   34
      Top             =   600
      Width           =   7395
   End
End
Attribute VB_Name = "charts"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
DefInt A-Z

Sub ChartFkey(Index)
Select Case Index
  Case 2: charts.Enabled = False
          chartstamm.Show
End Select
End Sub

Sub ChartsResize()
If charts.WindowState <> 1 Then
With charts
  e(9).left = Width
  w = Width - 8 * tpx
  HeaderFeld.Width = w
  ChartTitel.Width = w
  Listfeld.Width = w
  EingabeFeld.top = Height - EingabeFeld.Height - 30 * tpy
  EingabeFeld.Width = w
  Listfeld.Height = EingabeFeld.top - Listfeld.top
  scroll.left = w - scroll.Width - 4 * tpx
  scroll.Height = Listfeld.Height - 4 * tpy
  e(2).left = w - e(2).Width - 10 * tpx
  t(3).left = e(2).left - t(3).Width - 5 * tpx
  e(1).left = t(3).left - e(1).Width - 10 * tpx
  t(2).left = e(1).left - t(2).Width - 5 * tpx
  e(0).left = t(2).left - e(0).Width - 10 * tpx
  t(1).left = e(0).left - t(1).Width - 5 * tpx
  p(7).left = scroll.left - p(7).Width
  p(6).left = p(7).left - p(6).Width
  p(5).left = p(6).left - p(5).Width
  p(4).left = p(5).left - p(4).Width
  p(3).Width = p(4).left - p(3).left
  allesanzeigen.left = w - allesanzeigen.Width - 10 * tpx
  e(6).Width = w - 10 * tpx - e(6).left
  f(12).left = w - f(12).Width - 10 * tpx
  f(11).left = f(12).left - f(11).Width - 3 * tpx
  f(7).left = f(11).left - f(7).Width - 3 * tpx
  f(4).left = f(7).left - f(4).Width - 3 * tpx
End With
End If
End Sub

Private Sub Combo1_Change()

End Sub

Sub DrawChartsTitel()
anzeigen = charts.allesanzeigen.Value
Y1 = charts.p(0).Height
Y2 = Height
farbe& = 12632256
For i = 1 To 7
  X = p(i).left
  If i > 3 Then X = X - tpx
  charts.Listfeld.Line (X, Y1)-(X, Y2), farbe&
Next
X2 = Listfeld.Width - scroll.Width
Y = 12 * tpy + Y1
Do
  If Y > Y2 Then Exit Do
  charts.Listfeld.Line (0, Y)-(X2, Y), farbe&
  Y = Y + 12 * tpy
Loop
End Sub

Sub LoadCharts()
a$ = charts.ChartAuswahl.text
If a$ = Lang(118) Then
  charts.Enabled = False
  chartstamm.Show
  chartstamm.tag = "new"
Else
  a$ = Trim(ChartAuswahl.text)
  ff = FreeFile
  Open DatenPfad + "\charts.dir" For Random As #ff Len = Len(ch)
  Do
    Get #ff, , ch
    If Trim(ch.TitelKurz) = a$ Then Exit Do
  Loop Until EOF(ff)
  Close #ff
  ChartsLoaded = True
  ChartTitel.Caption = Trim(ch.TitelLang)
End If
End Sub

Private Sub ChartAuswahl_GotFocus()
ChartsLoaded = False
End Sub

Private Sub e_GotFocus(Index As Integer)
If ChartsLoaded = False Then
  DoEvents
  a$ = charts.ChartAuswahl.text
  If a$ = Lang(118) Then ChartAuswahl.SetFocus: DoEvents
  LoadCharts
End If
SendKeys "{home}+{end}"
If Index > 1 And WeekLoaded = False Then
  Stop
End If
End Sub


Private Sub f_Click(Index As Integer)
ChartFkey (Index)
End Sub

Private Sub Form_Load()
Dim rec As Rect
openini "wmusik20"
selectTopic "Fenster"
a$ = GetINI$("Charts")
If inierror = 0 Then
  String2Rect a$, rec
Else
  rec.left = (Screen.Width - 7665) / 2: rec.top = (Screen.Height - 5190) / 2
  rec.right = 7665: rec.bottom = 5190
End If
If rec.left > Screen.Width - charts.Width Then rec.left = Screen.Width - charts.Width
If rec.top > Screen.Height - charts.Height Then rec.top = Screen.Height - charts.Height
charts.left = rec.left: charts.top = rec.top
charts.Width = rec.right: charts.Height = rec.bottom
chartsvisible = True
selectTopic "Charts"
a$ = GetINI$("Letzte Hitparade")
ChartAuswahl.AddItem Lang(118), 0
FillChartAuswahl
If a$ = "" Or inierror > 0 Then
  ChartAuswahl.text = Lang(118)
Else
  ChartAuswahl.text = a$
End If
closeini
For i = 0 To 9
  e(i).text = ""
  t(i).Caption = Lang(167 + i) + ":"
  If i < 8 Then p(i).Caption = Lang(177 + i)
Next
ChartTitel.Caption = ""
allesanzeigen.Caption = Lang(185)
e(0).text = Year(Now)
e(1).text = Week(Datum$())
e(2).text = Datum$()
ChartsLoaded = False
EingabeFeld.Enabled = False
DoEvents
ChartsResize
End Sub


Private Sub Form_Resize()
If charts.WindowState <> 1 Then
  If charts.Width < 7665 Then charts.Width = 7665
  If charts.Height < 3500 Then charts.Height = 3500
End If
ChartsResize
Listfeld.Refresh
End Sub

Private Sub Form_Unload(Cancel As Integer)
ChartsLoaded = False
chartsvisible = False
Dim rec As Rect
openini "wmusik20"
selectTopic "Fenster"
rec.top = charts.top: rec.left = charts.left
rec.right = charts.Width: rec.bottom = charts.Height
a$ = Rect2String(rec)
putini "Charts=" + a$
selectTopic "Charts"
putini "Letzte Hitparade=" + Trim(ChartAuswahl.text)
closeini
End Sub


Private Sub ListFeld_Paint()
DrawChartsTitel
End Sub


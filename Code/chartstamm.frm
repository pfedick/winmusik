VERSION 5.00
Object = "{0BA686C6-F7D3-101A-993E-0000C0EF6F5E}#1.0#0"; "THREED32.OCX"
Begin VB.Form chartstamm 
   BorderStyle     =   1  'Fest Einfach
   Caption         =   "Form1"
   ClientHeight    =   2040
   ClientLeft      =   2160
   ClientTop       =   1800
   ClientWidth     =   7545
   Icon            =   "chartstamm.frx":0000
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   2040
   ScaleWidth      =   7545
   Begin VB.CommandButton abbrechen 
      Caption         =   "Abbrechen"
      Height          =   375
      Left            =   5640
      TabIndex        =   12
      Top             =   1620
      Width           =   1815
   End
   Begin VB.CommandButton ok 
      Caption         =   "OK"
      Height          =   375
      Left            =   60
      TabIndex        =   11
      Top             =   1620
      Width           =   1215
   End
   Begin VB.TextBox e 
      Height          =   285
      Index           =   3
      Left            =   480
      TabIndex        =   9
      Text            =   "e(3)"
      Top             =   1200
      Width           =   6435
   End
   Begin VB.ComboBox art 
      Height          =   315
      Left            =   1260
      Style           =   2  'Dropdown-Liste
      TabIndex        =   7
      Top             =   780
      Width           =   2055
   End
   Begin VB.TextBox e 
      Height          =   285
      Index           =   2
      Left            =   5520
      TabIndex        =   5
      Text            =   "e(2)"
      Top             =   420
      Width           =   855
   End
   Begin VB.TextBox e 
      Height          =   285
      Index           =   1
      Left            =   1500
      TabIndex        =   3
      Text            =   "e(1)"
      Top             =   420
      Width           =   1575
   End
   Begin VB.TextBox e 
      Height          =   285
      Index           =   0
      Left            =   1620
      TabIndex        =   1
      Text            =   "e(0)"
      Top             =   60
      Width           =   5835
   End
   Begin Threed.SSCommand folder 
      Height          =   315
      Left            =   7020
      TabIndex        =   10
      Top             =   1200
      Width           =   435
      _Version        =   65536
      _ExtentX        =   767
      _ExtentY        =   556
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
      Picture         =   "chartstamm.frx":030A
   End
   Begin VB.Image charticon 
      Height          =   735
      Left            =   6540
      Top             =   420
      Width           =   915
   End
   Begin VB.Label t 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Icon:"
      Height          =   195
      Index           =   4
      Left            =   60
      TabIndex        =   8
      Top             =   1260
      Width           =   360
   End
   Begin VB.Label t 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Auswertungsart:"
      Height          =   195
      Index           =   3
      Left            =   60
      TabIndex        =   6
      Top             =   840
      Width           =   1140
   End
   Begin VB.Label t 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Anzahl Platzierungen:"
      Height          =   195
      Index           =   2
      Left            =   3900
      TabIndex        =   4
      Top             =   480
      Width           =   1530
   End
   Begin VB.Label t 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Kurzbezeichnung:"
      Height          =   195
      Index           =   1
      Left            =   60
      TabIndex        =   2
      Top             =   480
      Width           =   1275
   End
   Begin VB.Label t 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Name der Hitparade:"
      Height          =   195
      Index           =   0
      Left            =   60
      TabIndex        =   0
      Top             =   120
      Width           =   1470
   End
End
Attribute VB_Name = "chartstamm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub abbrechen_Click()
Unload chartstamm
End Sub

Private Sub e_GotFocus(Index As Integer)
SendKeys "{home}+{end}"
End Sub


Private Sub Form_Load()
chartstamm.Caption = Lang(190) + " " + Lang(167)
For i = 0 To 4
  t(i).Caption = Lang(191 + i) + ":"
Next
ok.Caption = Lang(1)
abbrechen.Caption = Lang(2)
art.AddItem Lang(196), 0
e(0).left = t(0).left + t(0).Width + 3 * tpx
e(0).Width = Width - e(0).left - 10 * tpx
e(1).left = t(1).left + t(1).Width + 3 * tpx
e(1).Width = t(2).left - e(1).left - 15 * tpx
e(2).left = t(2).left + t(2).Width + 3 * tpx
art.left = t(3).left + t(3).Width + 3 * tpx
e(3).left = t(4).left + t(4).Width + 3 * tpx
e(3).Width = folder.left - e(3).left - 4 * tpx
For i = 0 To 3: e(i).text = "": Next
If ChartsLoaded = True Then
  e(0).text = Trim(ch.TitelLang)
  e(1).text = Trim(ch.TitelKurz)
  e(2).text = Format(ch.Plaetze)
  e(3).text = Trim(ch.icon)
  If ch.Auswertungsart = 0 Then
    art.text = Lang(196)
  Else
  End If
Else
  art.text = Lang(196)
End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
charts.Enabled = True
DoEvents
charts.ChartAuswahl.Clear
a$ = Trim(ch.TitelKurz)
charts.ChartAuswahl.AddItem Lang(118), 0
FillChartAuswahl
'charts.ChartAuswahl.text = "new"
If tag = "new" Then
  charts.ChartAuswahl.SetFocus
Else
  charts.SetFocus
End If
End Sub

Private Sub ok_Click()
ch.TitelLang = Trim(e(0).text)
ch.TitelKurz = Trim(e(1).text)
ch.Plaetze = Val(e(2).text)
ch.icon = Trim(e(3).text)
a$ = art.text
If a$ = Lang(196) Then
  ch.Auswertungsart = 0
Else
  
End If
f$ = DatenPfad + "\charts.dir"
ff = FreeFile
Open f$ For Random As #ff Len = Len(ch)
If ch.handle = 0 Then ch.handle = LOF(ff) / Len(ch) + 1
Put #ff, ch.handle, ch
Close #ff
Unload chartstamm
charts.ChartTitel.Caption = Trim(ch.TitelLang)
End Sub



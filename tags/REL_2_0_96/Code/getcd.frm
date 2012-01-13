VERSION 5.00
Object = "{0BA686C6-F7D3-101A-993E-0000C0EF6F5E}#1.0#0"; "THREED32.OCX"
Begin VB.Form GETCD 
   Appearance      =   0  '2D
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "CD erfassen"
   ClientHeight    =   2205
   ClientLeft      =   5055
   ClientTop       =   2070
   ClientWidth     =   3525
   BeginProperty Font 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H80000008&
   Icon            =   "getcd.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form3"
   LockControls    =   -1  'True
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   2205
   ScaleWidth      =   3525
   ShowInTaskbar   =   0   'False
   Begin Threed.SSFrame Frame3D1 
      Height          =   2040
      Left            =   60
      TabIndex        =   6
      Top             =   60
      Width           =   3375
      _Version        =   65536
      _ExtentX        =   5953
      _ExtentY        =   3598
      _StockProps     =   14
      ForeColor       =   0
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ShadowStyle     =   1
      Begin VB.OptionButton typ 
         Caption         =   "Daten"
         Height          =   300
         Index           =   1
         Left            =   1170
         TabIndex        =   3
         Top             =   480
         Width           =   900
      End
      Begin VB.OptionButton typ 
         Caption         =   "Audio"
         Height          =   300
         Index           =   0
         Left            =   135
         TabIndex        =   2
         Top             =   495
         Value           =   -1  'True
         Width           =   900
      End
      Begin VB.TextBox Seite 
         Height          =   360
         Left            =   750
         TabIndex        =   1
         Text            =   "1"
         Top             =   1440
         Width           =   375
      End
      Begin VB.TextBox Nummer 
         Height          =   330
         Left            =   930
         TabIndex        =   0
         Text            =   "Index"
         Top             =   975
         Width           =   1185
      End
      Begin VB.CommandButton OK 
         Appearance      =   0  '2D
         BackColor       =   &H80000005&
         Caption         =   "OK"
         Default         =   -1  'True
         Height          =   375
         Left            =   2490
         TabIndex        =   4
         Top             =   960
         Width           =   780
      End
      Begin VB.CommandButton Abbrechen 
         Appearance      =   0  '2D
         BackColor       =   &H80000005&
         Caption         =   "Abbrechen"
         Height          =   375
         Left            =   1875
         TabIndex        =   5
         Top             =   1440
         Width           =   1395
      End
      Begin Threed.SSPanel Panel3D3 
         Height          =   375
         Left            =   120
         TabIndex        =   7
         Top             =   0
         Width           =   1725
         _Version        =   65536
         _ExtentX        =   3043
         _ExtentY        =   661
         _StockProps     =   15
         Caption         =   "CD-Index eingeben:"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Autosize        =   1
      End
      Begin VB.Label Label2 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Seite:"
         ForeColor       =   &H80000008&
         Height          =   195
         Left            =   135
         TabIndex        =   9
         Top             =   1530
         Width           =   510
      End
      Begin VB.Label Label1 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Nummer:"
         ForeColor       =   &H80000008&
         Height          =   195
         Left            =   135
         TabIndex        =   8
         Top             =   1035
         Width           =   750
      End
   End
End
Attribute VB_Name = "GETCD"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub abbrechen_Click()
Unload GETCD
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
If KeyCode = 27 Then Unload GETCD
End Sub

Private Sub Form_Load()
Dim f As Rect
openini "wmusik20"
selectTopic "Fenster"
a$ = GetINI("GetCD")
If inierror() = 0 Then
  String2Rect a$, f
  If f.top > Screen.Height Then f.top = Screen.Height - GETCD.Height
  If f.left > Screen.Width Then f.left = Screen.Width - GETCD.Width
  GETCD.top = f.top
  GETCD.left = f.left
End If
closeini
Nummer.text = Format$(CDHead.Index)
If CDHead.Index = 0 Then Nummer.text = ""
Seite.text = Format$(CDHead.Seite)
If CDHead.Seite = 0 Then Seite.text = "1"
If CDHead.TraegerArt = 3 Then
  typ(1).Value = True
Else
  typ(0).Value = True
End If
GETCD.Caption = Lang(130)
Label1.Caption = Lang(56)
Label2.Caption = Lang(57)
ok.Caption = Lang(1)
abbrechen.Caption = Lang(2)
Panel3D3.Caption = Lang(127)
typ(0).Caption = Lang(128)
typ(1).Caption = Lang(129)
typ(1).left = typ(0).left + typ(0).Width + 10 * tpx
Nummer.left = Label1.left + Label1.Width + 2 * tpx
Seite.left = Label2.left + Label2.Width + 2 * tpx

End Sub


Private Sub Form_Unload(Cancel As Integer)
Dim f As Rect
f.top = GETCD.top
f.left = GETCD.left
openini "wmusik20"
selectTopic "Fenster"
a$ = Rect2String(f)
putini "GetCD=" + a$
closeini

End Sub

Private Sub Nummer_GotFocus()
SendKeys "{HOME}+{END}"
End Sub

Private Sub ok_Click()
If typ(0).Value = True Then
  CDHead.TraegerArt = 2
Else
  CDHead.TraegerArt = 3
End If
CDHead.Index = Val(Nummer.text)
CDHead.Seite = Val(Seite.text)
ff = FreeFile
Open CDHeaderDatei For Random As #ff Len = Len(CDHead)
If CDHdrDatensatz = 0 Then
  CDHdrDatensatz = LOF(ff) / Len(CDHead) + 1
End If
Put #ff, CDHdrDatensatz, CDHead
Close #ff
Unload GETCD
Suchstatus = 0: TracksLoaded = False
End Sub

Private Sub Seite_GotFocus()
SendKeys "{HOME}+{END}"
End Sub


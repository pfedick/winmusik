VERSION 5.00
Object = "{0BA686C6-F7D3-101A-993E-0000C0EF6F5E}#1.0#0"; "THREED32.OCX"
Begin VB.Form TitelInfo 
   Appearance      =   0  '2D
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "CD-Player: Titelinfo"
   ClientHeight    =   1875
   ClientLeft      =   2385
   ClientTop       =   3570
   ClientWidth     =   4950
   BeginProperty Font 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H00000000&
   Icon            =   "titelinf.frx":0000
   LinkTopic       =   "Form4"
   LockControls    =   -1  'True
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   1875
   ScaleWidth      =   4950
   Begin Threed.SSPanel Panel3D1 
      Height          =   1875
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   4950
      _Version        =   65536
      _ExtentX        =   8731
      _ExtentY        =   3307
      _StockProps     =   15
      ForeColor       =   16777215
      BackColor       =   0
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BevelWidth      =   2
      BevelOuter      =   1
      Begin VB.Label Info 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Info"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FF00&
         Height          =   210
         Index           =   11
         Left            =   2640
         TabIndex        =   2
         Top             =   1560
         Width           =   315
      End
      Begin VB.Label Info 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Info"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FF00&
         Height          =   210
         Index           =   10
         Left            =   960
         TabIndex        =   24
         Top             =   1560
         Width           =   315
      End
      Begin VB.Label Info 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Info"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FF00&
         Height          =   210
         Index           =   9
         Left            =   4080
         TabIndex        =   23
         Top             =   1320
         Width           =   315
      End
      Begin VB.Label Info 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Info"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FF00&
         Height          =   210
         Index           =   8
         Left            =   2160
         TabIndex        =   22
         Top             =   1320
         Width           =   315
      End
      Begin VB.Label Info 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Info"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FF00&
         Height          =   210
         Index           =   7
         Left            =   720
         TabIndex        =   21
         Top             =   1320
         Width           =   315
      End
      Begin VB.Label Info 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Info"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FF00&
         Height          =   210
         Index           =   6
         Left            =   840
         TabIndex        =   20
         Top             =   1080
         Width           =   315
      End
      Begin VB.Label Info 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Info"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FF00&
         Height          =   210
         Index           =   5
         Left            =   840
         TabIndex        =   19
         Top             =   840
         Width           =   315
      End
      Begin VB.Label Info 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Info"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FF00&
         Height          =   210
         Index           =   4
         Left            =   480
         TabIndex        =   18
         Top             =   600
         Width           =   315
      End
      Begin VB.Label Info 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Info"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FF00&
         Height          =   210
         Index           =   3
         Left            =   840
         TabIndex        =   17
         Top             =   360
         Width           =   315
      End
      Begin VB.Label Info 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Info"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FF00&
         Height          =   210
         Index           =   2
         Left            =   2520
         TabIndex        =   16
         Top             =   120
         Width           =   315
      End
      Begin VB.Label Info 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Info"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FF00&
         Height          =   210
         Index           =   1
         Left            =   1680
         TabIndex        =   15
         Top             =   120
         Width           =   315
      End
      Begin VB.Label Info 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Info"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FF00&
         Height          =   210
         Index           =   0
         Left            =   480
         TabIndex        =   14
         Top             =   120
         Width           =   315
      End
      Begin VB.Label Label1 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Quelle:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   210
         Index           =   11
         Left            =   2040
         TabIndex        =   13
         Top             =   1560
         Width           =   495
      End
      Begin VB.Label Label1 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Erscheinungsjahr:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   210
         Index           =   10
         Left            =   2760
         TabIndex        =   12
         Top             =   1320
         Width           =   1305
      End
      Begin VB.Label Label1 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Kaufdatum:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   210
         Index           =   9
         Left            =   120
         TabIndex        =   11
         Top             =   1560
         Width           =   825
      End
      Begin VB.Label Label1 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Musikart:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   210
         Index           =   8
         Left            =   120
         TabIndex        =   10
         Top             =   1080
         Width           =   645
      End
      Begin VB.Label Label1 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "min.     bpm:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   210
         Index           =   7
         Left            =   1200
         TabIndex        =   9
         Top             =   1320
         Width           =   855
      End
      Begin VB.Label Label1 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Länge:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   210
         Index           =   6
         Left            =   120
         TabIndex        =   8
         Top             =   1320
         Width           =   495
      End
      Begin VB.Label Label1 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Version:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   210
         Index           =   5
         Left            =   120
         TabIndex        =   7
         Top             =   840
         Width           =   615
      End
      Begin VB.Label Label1 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Titel:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   210
         Index           =   4
         Left            =   120
         TabIndex        =   6
         Top             =   600
         Width           =   330
      End
      Begin VB.Label Label1 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Interpret:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   210
         Index           =   3
         Left            =   120
         TabIndex        =   5
         Top             =   360
         Width           =   645
      End
      Begin VB.Label Label1 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Track:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   210
         Index           =   2
         Left            =   2040
         TabIndex        =   4
         Top             =   120
         Width           =   450
      End
      Begin VB.Label Label1 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Seite:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   210
         Index           =   1
         Left            =   1200
         TabIndex        =   3
         Top             =   120
         Width           =   405
      End
      Begin VB.Label Label1 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "CD:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   210
         Index           =   0
         Left            =   120
         TabIndex        =   1
         Top             =   120
         Width           =   255
      End
   End
End
Attribute VB_Name = "TitelInfo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()
Dim version As String * 80
If TracksLoaded = False Or CDLoaded = False Or RunningTrack = 0 Then
  Unload TitelInfo
  Exit Sub
End If
Dim f As Rect
openini "wmusik20"
selectTopic "Fenster"
a$ = GetINI("TitelInfo")
If inierror() = 0 Then
  String2Rect a$, f
  If f.top > Screen.Height Then f.top = Screen.Height - TitelInfo.Height
  If f.left > Screen.Width Then f.left = Screen.Width - TitelInfo.Width
  TitelInfo.top = f.top
  TitelInfo.left = f.left
End If
closeini


tr = RunningTrack
ff = FreeFile
Open TiDatei For Random Access Read As #ff Len = Len(CDTi)
Get #ff, tracks(tr), CDTi
Close #ff
Info(0).Caption = Format$(CDTi.Index)
Info(1).Caption = Format$(CDTi.Seite)
Info(2).Caption = Format$(CDTi.Track)
Info(3).Caption = Trim$(CDTi.Interpret)
Info(4).Caption = Trim$(CDTi.Titel)
Open DatenPfad + "\version.bez" For Random As #ff Len = 80
Get #ff, CDTi.version, version
Close #ff
Info(5).Caption = Trim$(version)
Open DatenPfad + "\musikart.bez" For Random As #ff Len = 80
Get #ff, CDTi.musikart, version
Close #ff
Info(6).Caption = Trim$(version)
Info(7).Caption = Format$(CDTi.Laenge, "#0.00")
Info(8).Caption = Format$(CDTi.bpm)
Info(9).Caption = Format$(CDTi.Erscheinungsjahr)
Info(10).Caption = Long2Dat(CDTi.Aufnahmedatum)
Open DatenPfad + "\quelle.bez" For Random As #ff Len = 80
Get #ff, CDTi.Quelle, version
Close #ff
Info(11).Caption = Trim$(version)
TitelInfoLoaded = True
End Sub

Private Sub Form_Unload(Cancel As Integer)
Dim f As Rect
f.top = TitelInfo.top
f.left = TitelInfo.left
openini "wmusik20"
selectTopic "Fenster"
a$ = Rect2String(f)
putini "TitelInfo=" + a$
closeini

TitelInfoLoaded = False
End Sub


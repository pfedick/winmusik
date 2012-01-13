VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form vcdcover 
   Caption         =   "Form1"
   ClientHeight    =   6810
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6540
   LinkTopic       =   "Form1"
   ScaleHeight     =   6810
   ScaleWidth      =   6540
   StartUpPosition =   3  'Windows-Standard
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   5760
      Top             =   6300
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      FontName        =   "Arial"
      FontSize        =   10
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Drucken"
      Height          =   375
      Left            =   2640
      TabIndex        =   2
      Top             =   6300
      Width           =   1035
   End
   Begin VB.Frame Frame2 
      Caption         =   "Seite"
      Height          =   2175
      Left            =   60
      TabIndex        =   1
      Top             =   3960
      Width           =   6375
      Begin VB.CommandButton Command2 
         Caption         =   "Auswahl"
         Height          =   315
         Index           =   3
         Left            =   5040
         TabIndex        =   22
         Top             =   1200
         Width           =   975
      End
      Begin VB.CommandButton Command2 
         Caption         =   "Auswahl"
         Height          =   315
         Index           =   2
         Left            =   5040
         TabIndex        =   19
         Top             =   180
         Width           =   975
      End
      Begin VB.TextBox seite_normal 
         Height          =   375
         Left            =   840
         TabIndex        =   10
         Top             =   1620
         Width           =   5295
      End
      Begin VB.TextBox seite_fett 
         Height          =   375
         Left            =   600
         TabIndex        =   8
         Top             =   600
         Width           =   5535
      End
      Begin VB.Label font1 
         BorderStyle     =   1  'Fest Einfach
         Caption         =   "Arial"
         Height          =   315
         Index           =   3
         Left            =   720
         TabIndex        =   21
         Top             =   1200
         Width           =   4215
      End
      Begin VB.Label Label5 
         AutoSize        =   -1  'True
         Caption         =   "Font:"
         Height          =   195
         Index           =   3
         Left            =   120
         TabIndex        =   20
         Top             =   1260
         Width           =   360
      End
      Begin VB.Label font1 
         BorderStyle     =   1  'Fest Einfach
         Caption         =   "Arial"
         Height          =   315
         Index           =   2
         Left            =   720
         TabIndex        =   18
         Top             =   180
         Width           =   4215
      End
      Begin VB.Label Label5 
         AutoSize        =   -1  'True
         Caption         =   "Font:"
         Height          =   195
         Index           =   2
         Left            =   120
         TabIndex        =   17
         Top             =   240
         Width           =   360
      End
      Begin VB.Label Label4 
         AutoSize        =   -1  'True
         Caption         =   "Normal:"
         Height          =   195
         Left            =   120
         TabIndex        =   9
         Top             =   1680
         Width           =   540
      End
      Begin VB.Label Label3 
         AutoSize        =   -1  'True
         Caption         =   "Fett:"
         Height          =   195
         Left            =   120
         TabIndex        =   7
         Top             =   660
         Width           =   315
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Front"
      Height          =   3675
      Left            =   60
      TabIndex        =   0
      Top             =   180
      Width           =   6375
      Begin VB.CommandButton Command2 
         Caption         =   "Auswahl"
         Height          =   315
         Index           =   1
         Left            =   5040
         TabIndex        =   16
         Top             =   1380
         Width           =   975
      End
      Begin VB.CommandButton Command2 
         Caption         =   "Auswahl"
         Height          =   315
         Index           =   0
         Left            =   5040
         TabIndex        =   13
         Top             =   240
         Width           =   975
      End
      Begin VB.TextBox text_front 
         Height          =   1695
         Left            =   720
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertikal
         TabIndex        =   6
         Top             =   1800
         Width           =   5415
      End
      Begin VB.TextBox ueberschrift 
         Height          =   315
         Left            =   1020
         TabIndex        =   4
         Top             =   660
         Width           =   5055
      End
      Begin VB.Label font1 
         BorderStyle     =   1  'Fest Einfach
         Caption         =   "Arial"
         Height          =   315
         Index           =   1
         Left            =   720
         TabIndex        =   15
         Top             =   1380
         Width           =   4215
      End
      Begin VB.Label Label5 
         AutoSize        =   -1  'True
         Caption         =   "Font:"
         Height          =   195
         Index           =   1
         Left            =   120
         TabIndex        =   14
         Top             =   1440
         Width           =   360
      End
      Begin VB.Label font1 
         BorderStyle     =   1  'Fest Einfach
         Caption         =   "Arial"
         Height          =   315
         Index           =   0
         Left            =   720
         TabIndex        =   12
         Top             =   240
         Width           =   4215
      End
      Begin VB.Label Label5 
         AutoSize        =   -1  'True
         Caption         =   "Font:"
         Height          =   195
         Index           =   0
         Left            =   120
         TabIndex        =   11
         Top             =   300
         Width           =   360
      End
      Begin VB.Label Label2 
         AutoSize        =   -1  'True
         Caption         =   "Text:"
         Height          =   195
         Left            =   120
         TabIndex        =   5
         Top             =   1860
         Width           =   360
      End
      Begin VB.Label Label1 
         AutoSize        =   -1  'True
         Caption         =   "Überschrift:"
         Height          =   195
         Left            =   120
         TabIndex        =   3
         Top             =   720
         Width           =   810
      End
   End
End
Attribute VB_Name = "vcdcover"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
DefInt A-Z


Sub RefreshFonts()
Dim bold As String
For i = 0 To 3
    If coverfont(i).bold = True Then bold = "bold" Else bold = "normal"
    font1(i).Caption = coverfont(i).name + ", " + Format(coverfont(i).size) + " Pt, " + bold
Next
End Sub

Private Sub Command1_Click()
DruckSimpleVCDCover
End Sub


Private Sub Command2_Click(Index As Integer)
CommonDialog1.FontName = coverfont(Index).name
CommonDialog1.FontBold = coverfont(Index).bold
CommonDialog1.FontItalic = coverfont(Index).italian
CommonDialog1.FontSize = coverfont(Index).size
CommonDialog1.FontUnderline = coverfont(Index).underlined
CommonDialog1.ShowFont


End Sub

Private Sub Form_Load()
For i = 0 To 3
    coverfont(i).name = "Arial"
    coverfont(i).bold = False
    coverfont(i).italian = False
    coverfont(i).underlined = False
    coverfont(i).size = 10
Next
coverfont(0).size = 28
coverfont(0).bold = True
coverfont(1).size = 10
coverfont(2).size = 12
coverfont(2).bold = True
coverfont(3).size = 8
End Sub



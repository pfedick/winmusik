VERSION 5.00
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.2#0"; "COMCTL32.OCX"
Begin VB.Form Form1 
   Caption         =   "CD-Nummern"
   ClientHeight    =   1875
   ClientLeft      =   3165
   ClientTop       =   2535
   ClientWidth     =   2970
   Icon            =   "cdnumform.frx":0000
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   1875
   ScaleWidth      =   2970
   Begin VB.CommandButton Command1 
      Caption         =   "Start"
      Height          =   375
      Left            =   720
      TabIndex        =   2
      Top             =   1440
      Width           =   1395
   End
   Begin VB.TextBox bis 
      Height          =   315
      Left            =   1920
      TabIndex        =   1
      Top             =   540
      Width           =   855
   End
   Begin VB.TextBox von 
      Height          =   285
      Left            =   420
      TabIndex        =   0
      Top             =   540
      Width           =   915
   End
   Begin ComctlLib.ProgressBar queue 
      Height          =   255
      Left            =   60
      TabIndex        =   6
      Top             =   960
      Width           =   2775
      _ExtentX        =   4895
      _ExtentY        =   450
      _Version        =   327682
      Appearance      =   1
   End
   Begin VB.Label Label3 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "bis:"
      Height          =   195
      Left            =   1560
      TabIndex        =   5
      Top             =   600
      Width           =   240
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "von:"
      Height          =   195
      Left            =   60
      TabIndex        =   4
      Top             =   600
      Width           =   315
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "CD-Nummern drucken"
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
      Left            =   420
      TabIndex        =   3
      Top             =   60
      Width           =   1860
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub bis_Change()

End Sub

Private Sub Command1_Click()

x = 0: y = 0
Printer.FontName = "Arial"
Printer.FontSize = 12
Printer.FontBold = True
queue.Min = 0
queue.Max = 100
s = CInt(von.Text)
z = CInt(bis.Text) - CInt(von.Text) + 1
For i = CInt(von.Text) To CInt(bis.Text)
  queue.Value = (i - s) * 100 / z
  Printer.Print Format(i);
  
Next
queue.Value = 100
Printer.EndDoc
End Sub



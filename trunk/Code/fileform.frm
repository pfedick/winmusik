VERSION 5.00
Begin VB.Form LoadSnd 
   Appearance      =   0  '2D
   BackColor       =   &H00C0C0C0&
   Caption         =   "Sounds 3.0 - Sound laden"
   ClientHeight    =   3825
   ClientLeft      =   2535
   ClientTop       =   1425
   ClientWidth     =   4650
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
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   3825
   ScaleWidth      =   4650
   Begin VB.CommandButton Command3 
      Appearance      =   0  '2D
      BackColor       =   &H80000005&
      Caption         =   "Anhören"
      Height          =   255
      Left            =   2760
      TabIndex        =   9
      Top             =   3525
      Width           =   1200
   End
   Begin VB.CommandButton Command2 
      Appearance      =   0  '2D
      BackColor       =   &H80000005&
      Caption         =   "Abbruch"
      Height          =   315
      Left            =   3480
      TabIndex        =   2
      Top             =   405
      Width           =   1080
   End
   Begin VB.CommandButton Command1 
      Appearance      =   0  '2D
      BackColor       =   &H80000005&
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   330
      Left            =   3480
      TabIndex        =   1
      Top             =   15
      Width           =   1080
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Left            =   570
      TabIndex        =   0
      Text            =   "Text1"
      Top             =   390
      Width           =   2850
   End
   Begin VB.FileListBox File1 
      Height          =   2820
      Left            =   105
      Pattern         =   "*.wav"
      TabIndex        =   5
      Top             =   825
      Width           =   2130
   End
   Begin VB.DirListBox Dir1 
      Height          =   2280
      Left            =   2340
      TabIndex        =   4
      Top             =   825
      Width           =   2190
   End
   Begin VB.DriveListBox Drive1 
      Height          =   315
      Left            =   2310
      TabIndex        =   3
      Top             =   3150
      Width           =   2220
   End
   Begin VB.Label Label3 
      Appearance      =   0  '2D
      BackColor       =   &H00C0C0C0&
      BackStyle       =   0  'Transparent
      Caption         =   "Label3"
      ForeColor       =   &H80000008&
      Height          =   240
      Left            =   660
      TabIndex        =   8
      Top             =   30
      Width           =   2760
   End
   Begin VB.Label Label2 
      Appearance      =   0  '2D
      BackColor       =   &H00C0C0C0&
      BackStyle       =   0  'Transparent
      Caption         =   "Titel:"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   15
      TabIndex        =   7
      Top             =   435
      Width           =   495
   End
   Begin VB.Label Label1 
      Appearance      =   0  '2D
      BackColor       =   &H00C0C0C0&
      BackStyle       =   0  'Transparent
      Caption         =   "Datei:"
      ForeColor       =   &H80000008&
      Height          =   270
      Left            =   30
      TabIndex        =   6
      Top             =   30
      Width           =   525
   End
End
Attribute VB_Name = "LoadSnd"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
If Label3.Caption <> "" Then
  If Text1.Text = "" Then Text1.Text = File1.filename
  sname$(my) = Text1.Text
  sfile$(my) = Label3.Caption
  Unload LoadSnd
  SavePage (Seite)
End If
End Sub

Private Sub Command2_Click()
Unload LoadSnd

End Sub

Private Sub Command3_Click()
If Label3.Caption <> "" Then
  Call sndplaysound(Label3.Caption, 1)
End If
End Sub

Private Sub Dir1_Change()
ChDir Dir1.Path
File1.Path = Dir1.Path
End Sub

Private Sub Dir1_Click()
Text1.SetFocus
End Sub

Private Sub Drive1_Change()
Dir1.Path = Drive1.Drive
Text1.SetFocus
End Sub

Private Sub File1_Click()
Label3.Caption = File1.Path + "\" + File1.filename
If Text1.Text = "" Or InStr(Text1.Text, ".wav") > 0 Then Text1.Text = File1.filename
Text1.SetFocus
End Sub

Private Sub File1_DblClick()
Label3.Caption = File1.Path + "\" + File1.filename
Text1.SetFocus
End Sub

Private Sub File1_PathChange()
Label3.Caption = ""
End Sub

Private Sub Form_Load()
Label3.Caption = ""
Text1.Text = ""
End Sub

Private Sub Form_Unload(Cancel As Integer)
s3.Enabled = True
End Sub


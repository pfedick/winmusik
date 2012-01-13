VERSION 5.00
Begin VB.Form updateform 
   Caption         =   "Form1"
   ClientHeight    =   4860
   ClientLeft      =   1500
   ClientTop       =   5880
   ClientWidth     =   6015
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   4860
   ScaleWidth      =   6015
   Begin VB.CommandButton abbrechen 
      Caption         =   "Abbrechen"
      Height          =   375
      Left            =   3780
      TabIndex        =   6
      Top             =   4320
      Width           =   1995
   End
   Begin VB.CommandButton download 
      Caption         =   "Download"
      Height          =   375
      Left            =   3780
      TabIndex        =   5
      Top             =   3840
      Width           =   1995
   End
   Begin VB.OptionButton Option1 
      Caption         =   "Nur DLL's"
      Height          =   255
      Index           =   2
      Left            =   60
      TabIndex        =   3
      Top             =   4440
      Width           =   3255
   End
   Begin VB.OptionButton Option1 
      Caption         =   "Nur Programmdateien, ohne DLL's"
      Height          =   255
      Index           =   1
      Left            =   60
      TabIndex        =   2
      Top             =   4140
      Width           =   3255
   End
   Begin VB.OptionButton Option1 
      Caption         =   "Komplettversion mit allen DLL's"
      Height          =   255
      Index           =   0
      Left            =   60
      TabIndex        =   1
      Top             =   3840
      Width           =   3255
   End
   Begin VB.TextBox text 
      Height          =   3375
      Left            =   60
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Beides
      TabIndex        =   0
      Text            =   "updateform.frx":0000
      Top             =   60
      Width           =   5835
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Welche Version möchten Sie downloaden?"
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
      TabIndex        =   4
      Top             =   3540
      Width           =   3645
   End
End
Attribute VB_Name = "updateform"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub abbrechen_Click()
Unload updateform
End Sub

Private Sub download_Click()
f$ = ""
If Option1(0).Value = True Then f$ = "1"
If Option1(1).Value = True Then f$ = "2"
If Option1(2).Value = True Then f$ = "3"
If f$ = "" Then
  Stop
Else
End If
End Sub

Private Sub Form_Load()
Caption = "WinMusik " + Lang(197)
Label1.Caption = Lang(202)
Option1(0).Caption = Lang(203)
Option1(1).Caption = Lang(204)
Option1(2).Caption = Lang(205)
download.Caption = Lang(206)
abbrechen.Caption = Lang(2)
End Sub


Private Sub Form_Resize()
br = updateform.Width: hh = updateform.Height
If br < 5715 Then br = 5715: updateform.Width = br
If hh < 2985 Then hh = 2985: updateform.Height = hh
text.Width = br - 20 * tpx
text.Height = hh - 1770
Label1.top = hh - 1665
Option1(0).top = hh - 1365
Option1(1).top = hh - 1065
Option1(2).top = hh - 765
download.top = hh - 1365
abbrechen.top = hh - 885
download.left = br - 2310
abbrechen.left = br - 2310
End Sub


Private Sub Inet1_StateChanged(ByVal State As Integer)

End Sub



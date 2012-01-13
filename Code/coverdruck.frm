VERSION 5.00
Begin VB.Form coverdruck 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "Cover drucken"
   ClientHeight    =   1710
   ClientLeft      =   4035
   ClientTop       =   2745
   ClientWidth     =   3765
   ControlBox      =   0   'False
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   1710
   ScaleWidth      =   3765
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton abbrechen 
      Caption         =   "Abbrechen"
      Height          =   375
      Left            =   2280
      TabIndex        =   5
      Top             =   1260
      Width           =   1395
   End
   Begin VB.CommandButton ok 
      Caption         =   "Ok"
      Default         =   -1  'True
      Height          =   375
      Left            =   60
      TabIndex        =   4
      Top             =   1260
      Width           =   1035
   End
   Begin VB.TextBox bis 
      Height          =   285
      Left            =   2400
      TabIndex        =   1
      Text            =   "Text1"
      Top             =   720
      Width           =   1275
   End
   Begin VB.TextBox von 
      Height          =   285
      Left            =   600
      TabIndex        =   0
      Text            =   "Text1"
      Top             =   720
      Width           =   1035
   End
   Begin VB.Label Label3 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Bitte wählen Sie die Tonträger aus, die Sie drucken möchten:"
      Height          =   555
      Left            =   780
      TabIndex        =   6
      Top             =   180
      Width           =   2895
      WordWrap        =   -1  'True
   End
   Begin VB.Image Image1 
      Height          =   480
      Left            =   60
      Picture         =   "coverdruck.frx":0000
      Top             =   180
      Width           =   480
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "bis:"
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
      Left            =   1980
      TabIndex        =   3
      Top             =   780
      Width           =   315
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "von:"
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
      Left            =   120
      TabIndex        =   2
      Top             =   780
      Width           =   390
   End
End
Attribute VB_Name = "coverdruck"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub abbrechen_Click()
Unload coverdruck
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
If KeyCode = 27 Then
    Unload coverdruck
    'eingabe.e(LetzteEingabePos).SetFocus
    'eingabe.SetFocus
End If
End Sub

Private Sub Form_Load()
Caption = Lang(211)
Label1.Caption = Lang(212)
Label2.Caption = Lang(213)
Label3.Caption = Lang(214)
OK.Caption = Lang(1)
abbrechen.Caption = Lang(2)
End Sub


Private Sub Form_Unload(Cancel As Integer)
'eingabe.Enabled = True
eingabe.SetFocus
End Sub


Private Sub ok_Click()
On Error Resume Next
Dim von As Long
Dim bis As Long

von = CLng(coverdruck.von.text)
bis = CLng(coverdruck.bis.text)
Unload coverdruck
eingabe.Enabled = True
eingabe.SetFocus
EingabePos = -3
If saverequired = True Then
    SaveTraeger
End If

DruckTraeger traeger, von, bis

End Sub



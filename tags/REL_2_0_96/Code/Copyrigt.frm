VERSION 5.00
Begin VB.Form CopyrightForm 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "Form2"
   ClientHeight    =   5955
   ClientLeft      =   1800
   ClientTop       =   1695
   ClientWidth     =   7425
   Icon            =   "Copyrigt.frx":0000
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   5955
   ScaleWidth      =   7425
   ShowInTaskbar   =   0   'False
   Begin VB.PictureBox Picture1 
      AutoSize        =   -1  'True
      Height          =   3465
      Left            =   120
      Picture         =   "Copyrigt.frx":0CFA
      ScaleHeight     =   3405
      ScaleWidth      =   4200
      TabIndex        =   5
      Top             =   120
      Width           =   4260
   End
   Begin VB.Timer ctimer1 
      Interval        =   3000
      Left            =   7080
      Top             =   3240
   End
   Begin VB.TextBox info 
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   9
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   2175
      Left            =   60
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertikal
      TabIndex        =   4
      Text            =   "Copyrigt.frx":2F614
      Top             =   3660
      Width           =   7335
   End
   Begin VB.CommandButton ok 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   5160
      TabIndex        =   3
      Top             =   3180
      Visible         =   0   'False
      Width           =   1395
   End
   Begin VB.Label Label3 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1035
      Left            =   4440
      TabIndex        =   2
      Top             =   1980
      Width           =   2895
   End
   Begin VB.Label Label2 
      Caption         =   "(c) by Patrick F.-Productions 1996-2009"
      Height          =   1035
      Left            =   4440
      TabIndex        =   1
      Top             =   780
      Width           =   2895
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Zentriert
      Caption         =   "WinMusik 2.0"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   -1  'True
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   4440
      TabIndex        =   0
      Top             =   120
      Width           =   2895
   End
End
Attribute VB_Name = "CopyrightForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub ctimer1_Timer()
Unload CopyrightForm
End Sub

Private Sub Form_Load()
Show
DoEvents
copyrightvisible = True
Caption = Programm + " " + version
Label1.Caption = Caption
a$ = copyright + Chr$(13)
a$ = a$ + "Programmed by: " + Autor + Chr$(13)
a$ = a$ + "Last Update: " + Progdate
Label2.Caption = a$
If Shareware = True Then
  Label3.Caption = Lang(124)
  ok.Visible = True
  ok.Caption = Lang(1)
  info.Visible = True
  Height = 6300
  f$ = HauptPfad + "\" + Lang(126)
  ff = FreeFile
  Open f$ For Input As #ff
  t$ = ""
  Do Until EOF(ff)
    Line Input #ff, a$
    t$ = t$ + a$ + Chr$(13) + Chr$(10)
  Loop
  Close #ff
  info.text = t$
Else
  Height = 4050
End If
ctimer1.Interval = 4000
ctimer1.Enabled = True
End Sub

Private Sub Form_Unload(Cancel As Integer)
copyrightvisible = False

End Sub


Private Sub ok_Click()
leiste1.Show
Unload CopyrightForm
End Sub


Private Sub tit1_Click()

End Sub


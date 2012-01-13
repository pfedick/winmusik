VERSION 4.00
Begin VB.Form CopyrightForm 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Form2"
   ClientHeight    =   5955
   ClientLeft      =   1800
   ClientTop       =   1695
   ClientWidth     =   7425
   Height          =   6360
   Icon            =   "Copyright.frx":0000
   Left            =   1740
   LinkTopic       =   "Form2"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5955
   ScaleWidth      =   7425
   ShowInTaskbar   =   0   'False
   Top             =   1350
   Width           =   7545
   Begin VB.Timer ctimer1 
      Interval        =   3000
      Left            =   7080
      Top             =   3240
   End
   Begin VB.TextBox info 
      BeginProperty Font 
         name            =   "Courier New"
         charset         =   0
         weight          =   400
         size            =   9
         underline       =   0   'False
         italic          =   0   'False
         strikethrough   =   0   'False
      EndProperty
      Height          =   2175
      Left            =   60
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   4
      Text            =   "Copyright.frx":030A
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
         name            =   "MS Sans Serif"
         charset         =   0
         weight          =   700
         size            =   8.25
         underline       =   0   'False
         italic          =   0   'False
         strikethrough   =   0   'False
      EndProperty
      Height          =   1035
      Left            =   4440
      TabIndex        =   2
      Top             =   1980
      Width           =   2895
   End
   Begin VB.Label Label2 
      Caption         =   "(c) by Patrick F.-Productions in 1996"
      Height          =   1035
      Left            =   4440
      TabIndex        =   1
      Top             =   780
      Width           =   2895
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Caption         =   "WinMusik 2.0a"
      BeginProperty Font 
         name            =   "MS Sans Serif"
         charset         =   0
         weight          =   700
         size            =   12
         underline       =   -1  'True
         italic          =   0   'False
         strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   4440
      TabIndex        =   0
      Top             =   120
      Width           =   2895
   End
   Begin VB.Image Image1 
      BorderStyle     =   1  'Fixed Single
      Height          =   3465
      Left            =   60
      Picture         =   "Copyright.frx":0312
      Top             =   90
      Width           =   4260
   End
End
Attribute VB_Name = "CopyrightForm"
Attribute VB_Creatable = False
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
a$ = Copyright + Chr$(13)
a$ = a$ + "Programmed by: " + Autor + Chr$(13)
a$ = a$ + "Last Update: " + Progdate
Label2.Caption = a$
If Shareware = True Then
  label3.Caption = Lang(124)
  ok.Visible = True
  ok.Caption = Lang(1)
  Info.Visible = True
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
  Info.text = t$
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
menu1.Show
Unload CopyrightForm
End Sub



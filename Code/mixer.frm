VERSION 5.00
Begin VB.Form mixer 
   BorderStyle     =   1  'Fest Einfach
   Caption         =   "Form1"
   ClientHeight    =   1860
   ClientLeft      =   9630
   ClientTop       =   1440
   ClientWidth     =   2160
   ClipControls    =   0   'False
   Icon            =   "mixer.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   1860
   ScaleWidth      =   2160
   Begin VB.VScrollBar slide 
      Height          =   1440
      Index           =   6
      Left            =   1860
      TabIndex        =   6
      Top             =   360
      Width           =   225
   End
   Begin VB.VScrollBar slide 
      Height          =   1440
      Index           =   4
      Left            =   1560
      TabIndex        =   5
      Top             =   360
      Width           =   225
   End
   Begin VB.VScrollBar slide 
      Height          =   1440
      Index           =   3
      Left            =   1260
      TabIndex        =   4
      Top             =   360
      Width           =   225
   End
   Begin VB.VScrollBar slide 
      Height          =   1440
      Index           =   2
      Left            =   960
      TabIndex        =   3
      Top             =   360
      Width           =   225
   End
   Begin VB.VScrollBar slide 
      Height          =   1440
      Index           =   1
      Left            =   660
      TabIndex        =   2
      Top             =   360
      Width           =   225
   End
   Begin VB.VScrollBar slide 
      Height          =   1440
      Index           =   0
      Left            =   360
      TabIndex        =   1
      Top             =   360
      Width           =   225
   End
   Begin VB.VScrollBar slide 
      Height          =   1440
      Index           =   5
      Left            =   60
      TabIndex        =   0
      Top             =   360
      Width           =   225
   End
   Begin VB.Image Image7 
      Height          =   300
      Left            =   1800
      Picture         =   "mixer.frx":030A
      Top             =   60
      Width           =   300
   End
   Begin VB.Image Image6 
      Height          =   300
      Left            =   1500
      Picture         =   "mixer.frx":047C
      Top             =   60
      Width           =   300
   End
   Begin VB.Image Image5 
      Height          =   300
      Left            =   1200
      Picture         =   "mixer.frx":05EE
      Top             =   60
      Width           =   300
   End
   Begin VB.Image Image4 
      Height          =   300
      Left            =   900
      Picture         =   "mixer.frx":0760
      Top             =   60
      Width           =   300
   End
   Begin VB.Image Image3 
      Height          =   300
      Left            =   660
      Picture         =   "mixer.frx":08D2
      Top             =   60
      Width           =   300
   End
   Begin VB.Image Image2 
      Height          =   300
      Left            =   360
      Picture         =   "mixer.frx":0A44
      Top             =   60
      Width           =   300
   End
   Begin VB.Image Image1 
      Height          =   300
      Left            =   30
      Picture         =   "mixer.frx":0BB6
      Top             =   15
      Width           =   300
   End
End
Attribute VB_Name = "mixer"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Click()
SavePicture Picture1.Image, "e:\wmusik32\img\mixer.ico"
End Sub

Private Sub Form_Load()
mixer.Caption = Lang(158)
Dim rec As Rect
Dim vv(6) As Long
openini "wmusik20"
selectTopic "Fenster"
a$ = GetINI$("Mixer")
If inierror = 0 Then
  String2Rect a$, rec
Else
  rec.left = (Screen.Width - 6060) / 2: rec.top = (Screen.Height - 2910) / 2
  rec.right = 6060: rec.bottom = 2910
End If
If rec.left > Screen.Width - mixer.Width Then rec.left = Screen.Width - mixer.Width
If rec.top > Screen.Height - mixer.Height Then rec.top = Screen.Height - mixer.Height
mixer.left = rec.left: mixer.top = rec.top
For i = 0 To 6
  aux& = i
  slide(i).Min = 0: slide(i).Max = 255
  slide(i).SmallChange = 1: slide(i).LargeChange = 16
  result& = auxGetVolume(aux&, v&)
  v& = v& / 256
  v& = 255 - v& And 255
  slide(i).Value = v&
Next
mixervisible = True

End Sub

Private Sub Form_Unload(Cancel As Integer)
mixervisible = False
Dim rec As Rect
openini "wmusik20"
selectTopic "Fenster"
rec.top = mixer.top: rec.left = mixer.left
rec.right = mixer.Width: rec.bottom = mixer.Height
a$ = Rect2String(rec)
putini "Mixer=" + a$
closeini
End Sub


Private Sub VScroll1_Change()

End Sub


Private Sub slide_Change(Index As Integer)
aux& = Index
v# = 255 - slide(Index).Value
v# = v# * 256#
v# = v# + v# * 65536#
If v# < 2139062144# Then
  vv& = v#
Else
  vv& = v# - 4294967294#
End If
result& = auxSetVolume(aux&, vv&)

End Sub

Private Sub slide_Scroll(Index As Integer)
aux& = Index
v# = 255 - slide(Index).Value
v# = v# * 256#
v# = v# + v# * 65536#
If v# < 2139062144# Then
  vv& = v#
Else
  vv& = v# - 4294967294#
End If
result& = auxSetVolume(aux&, vv&)

End Sub


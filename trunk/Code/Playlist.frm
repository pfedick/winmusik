VERSION 5.00
Begin VB.Form Playlist 
   Caption         =   "Playlist"
   ClientHeight    =   8250
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   10410
   LinkTopic       =   "Form1"
   ScaleHeight     =   8250
   ScaleWidth      =   10410
   StartUpPosition =   3  'Windows-Standard
   Begin VB.VScrollBar VScroll1 
      Height          =   5295
      Left            =   9480
      TabIndex        =   1
      Top             =   1800
      Width           =   255
   End
   Begin VB.PictureBox Picture1 
      Height          =   5295
      Left            =   120
      ScaleHeight     =   5235
      ScaleWidth      =   9195
      TabIndex        =   0
      Top             =   1800
      Width           =   9255
   End
End
Attribute VB_Name = "Playlist"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Picture1_DragDrop(Source As Control, X As Single, Y As Single)
Stop

End Sub

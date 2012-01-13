VERSION 5.00
Begin VB.Form files 
   Caption         =   "Form1"
   ClientHeight    =   3735
   ClientLeft      =   1860
   ClientTop       =   3915
   ClientWidth     =   5520
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   3735
   ScaleWidth      =   5520
   Begin VB.CommandButton Command1 
      Caption         =   "Abbrechen"
      Height          =   375
      Left            =   4200
      TabIndex        =   5
      Top             =   3300
      Width           =   1155
   End
   Begin VB.CommandButton OK 
      Caption         =   "OK"
      Height          =   375
      Left            =   60
      TabIndex        =   4
      Top             =   3300
      Width           =   645
   End
   Begin VB.TextBox file 
      Height          =   285
      Left            =   60
      TabIndex        =   3
      Top             =   60
      Width           =   5265
   End
   Begin VB.FileListBox File1 
      Height          =   2820
      Left            =   3420
      TabIndex        =   2
      Top             =   420
      Width           =   1965
   End
   Begin VB.DirListBox Dir1 
      Height          =   2340
      Left            =   60
      TabIndex        =   1
      Top             =   420
      Width           =   3285
   End
   Begin VB.DriveListBox Drive1 
      Height          =   315
      Left            =   60
      TabIndex        =   0
      Top             =   2880
      Width           =   3285
   End
End
Attribute VB_Name = "files"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Dir1_Change()
File1.Path = Dir1.Path
file.text = Dir1.Path

End Sub

Private Sub Drive1_Change()
On Error Resume Next
Dir1.Path = Drive1.Drive


End Sub


Private Sub File1_Click()
file.text = File1.Path + "\" + File1.filename

End Sub



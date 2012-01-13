VERSION 5.00
Begin VB.Form mp3auswahl 
   BackColor       =   &H80000015&
   BorderStyle     =   0  'Kein
   Caption         =   "MP3-Import"
   ClientHeight    =   1560
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   1530
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   Moveable        =   0   'False
   ScaleHeight     =   1560
   ScaleWidth      =   1530
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton mp3action 
      Caption         =   "6. ID3-Tags entfernen"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   6.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   5
      Left            =   0
      TabIndex        =   5
      Top             =   1285
      Width           =   1515
   End
   Begin VB.CommandButton mp3action 
      Caption         =   "5. Playlist schreiben"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   6.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   4
      Left            =   15
      TabIndex        =   4
      Top             =   1035
      Width           =   1515
   End
   Begin VB.CommandButton mp3action 
      Caption         =   "4. ID3-Tags schreiben"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   6.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   3
      Left            =   15
      TabIndex        =   3
      Top             =   780
      Width           =   1515
   End
   Begin VB.CommandButton mp3action 
      Caption         =   "3. Files umbenennen"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   6.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   2
      Left            =   15
      TabIndex        =   2
      Top             =   525
      Width           =   1515
   End
   Begin VB.CommandButton mp3action 
      Caption         =   "2. ID3-Tags einlesen"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   6.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   1
      Left            =   15
      TabIndex        =   1
      Top             =   270
      Width           =   1515
   End
   Begin VB.CommandButton mp3action 
      Caption         =   "1. Längen einlesen"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   6.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   0
      Left            =   15
      TabIndex        =   0
      Top             =   15
      Width           =   1515
   End
End
Attribute VB_Name = "mp3auswahl"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Sub Action(Index)
Dim verzeichnis As String

Unload Me
DoEvents
Select Case Index
    Case 1: 'verzeichnis = InputBox(Lang(218), Lang(219), MpegPfad + "\" + Format(traegerindex, "000"))
            GetMP3TrackLength
    Case 2: verzeichnis = InputBox(Lang(218), Lang(219), MpegPfad + "\" + Format(traegerindex, "000"))
            Debug.Print "Case 2: Verzeichnis=" + verzeichnis
    Case 3: eingabe.Enabled = False
            DoEvents
            mp3rename.Show
            
    
    Case 4: makeid3tags
    Case 5: makempegindex
    Case 6: choptags
    
End Select

End Sub


Private Sub Form_Deactivate()
Unload mp3auswahl


End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
If KeyCode = 27 Then Unload mp3auswahl

If KeyCode > 48 And KeyCode < 54 Then
  Action KeyCode - 48
End If
If KeyCode > 96 And KeyCode < 102 Then
  Action KeyCode - 96
End If

'Debug.Print KeyCode, Shift
If KeyCode = 117 Then Unload Me

End Sub


Private Sub Form_Load()
mp3action(0).Caption = Lang(243)
mp3action(1).Caption = Lang(244)
mp3action(2).Caption = Lang(245)
mp3action(3).Caption = Lang(246)
mp3action(4).Caption = Lang(247)

End Sub

Private Sub Form_LostFocus()
Unload Me
End Sub


Private Sub mp3action_Click(Index As Integer)
Action (Index + 1)

End Sub



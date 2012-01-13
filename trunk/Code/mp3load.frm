VERSION 5.00
Begin VB.Form mp3rename 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "MP3-Files einlesen"
   ClientHeight    =   4170
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7230
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4170
   ScaleWidth      =   7230
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows-Standard
   Begin VB.CommandButton abbrechen 
      Caption         =   "Abbrechen"
      Height          =   435
      Left            =   2520
      TabIndex        =   18
      Top             =   3660
      Width           =   1875
   End
   Begin VB.CommandButton OK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   435
      Left            =   120
      TabIndex        =   17
      Top             =   3660
      Width           =   1095
   End
   Begin VB.Frame Frame3 
      Caption         =   "Daten einlesen"
      Height          =   1035
      Left            =   4500
      TabIndex        =   14
      Top             =   3060
      Width           =   2595
      Begin VB.CheckBox tracklaenge 
         Caption         =   "Tracklänge"
         Height          =   195
         Left            =   120
         TabIndex        =   16
         Top             =   660
         Value           =   1  'Aktiviert
         Width           =   1275
      End
      Begin VB.CheckBox id3tag 
         Caption         =   "ID3-Tag"
         Height          =   195
         Left            =   120
         TabIndex        =   15
         Top             =   360
         Value           =   1  'Aktiviert
         Width           =   1755
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "Dateien umbenennen"
      Height          =   1755
      Left            =   4500
      TabIndex        =   9
      Top             =   1140
      Width           =   2595
      Begin VB.OptionButton rename 
         Caption         =   "Dateiname durch Index ersetzen"
         Height          =   435
         Index           =   2
         Left            =   120
         TabIndex        =   13
         Top             =   1260
         Width           =   1635
      End
      Begin VB.OptionButton rename 
         Caption         =   "Index ergänzen"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   11
         Top             =   600
         Value           =   -1  'True
         Width           =   1635
      End
      Begin VB.OptionButton rename 
         Caption         =   "Nein (nur ???.mp3 Dateien)"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   10
         Top             =   300
         Width           =   2355
      End
      Begin VB.Label Label3 
         Caption         =   "Dateiname.mp3 -> 001-Dateiname.mp3"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   435
         Left            =   480
         TabIndex        =   12
         Top             =   840
         Width           =   1455
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Titel einlesen"
      Height          =   915
      Left            =   4500
      TabIndex        =   6
      Top             =   120
      Width           =   2595
      Begin VB.OptionButton auswahl 
         Caption         =   "Alle"
         Height          =   315
         Index           =   0
         Left            =   120
         TabIndex        =   8
         Top             =   240
         Value           =   -1  'True
         Width           =   1275
      End
      Begin VB.OptionButton auswahl 
         Caption         =   "Nur markierte"
         Height          =   315
         Index           =   1
         Left            =   120
         TabIndex        =   7
         Top             =   540
         Width           =   1695
      End
   End
   Begin VB.TextBox filter 
      Height          =   285
      Left            =   3000
      TabIndex        =   5
      Text            =   "*.mp*"
      Top             =   360
      Width           =   1335
   End
   Begin VB.FileListBox File1 
      Height          =   2625
      Left            =   2520
      MultiSelect     =   2  'Erweitert
      Pattern         =   "*.mp*"
      TabIndex        =   3
      Top             =   780
      Width           =   1815
   End
   Begin VB.DirListBox Dir1 
      Height          =   2565
      Left            =   120
      TabIndex        =   1
      Top             =   840
      Width           =   2295
   End
   Begin VB.DriveListBox Drive1 
      Height          =   315
      Left            =   120
      TabIndex        =   0
      Top             =   360
      Width           =   2295
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "Filter:"
      Height          =   195
      Left            =   2520
      TabIndex        =   4
      Top             =   420
      Width           =   375
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Pfad der MP3-Dateien:"
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
      Top             =   120
      Width           =   1950
   End
End
Attribute VB_Name = "mp3rename"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub abbrechen_Click()
Unload mp3load
End Sub

Private Sub abbrechen_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub

Private Sub auswahl_KeyPress(Index As Integer, KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub

Private Sub Dir1_Change()
File1.Path = Dir1.Path

End Sub

Private Sub Dir1_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub

Private Sub Drive1_Change()
Dir1.Path = Drive1.Drive

End Sub

Private Sub Drive1_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub

Private Sub File1_Click()
auswahl(1).Value = True

End Sub

Private Sub File1_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub

Private Sub filter_Change()
On Error Resume Next
File1.Pattern = filter.text

End Sub

Private Sub filter_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub

Private Sub Form_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub

Private Sub Form_Load()
On Error Resume Next
Err = 0
Drive1.Drive = MpegPfad
If Err = 0 Then
  Dir1.Path = MpegPfad + "\" + Format(traegerindex, "000")
  If Err > 0 Then
    Dir1.Path = MpegPfad
  End If
End If
             
End Sub

Private Sub Form_Unload(Cancel As Integer)
eingabe.Enabled = True
End Sub

Private Sub id3tag_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub

Private Sub OK_Click()
Dim i As Integer
Dim file As String
Dim ImportFile As Integer, ff As Integer
Dim mp3impdata As MP3impdataFile
On Error Resume Next

mp3load.Enabled = False

If File1.ListCount = 0 Then
    MsgBox "Es wurden keine Dateien zum Einlesen ausgewählt!", vbInformation, "MP3-Files einlesen"
Else
    status.Show
    status.Caption = "MP3-Files einlesen"
    status.text.text = "MP3-Dateien werden eingelesen, bitte warten..."
    status.queue.Min = 1
    status.queue.Max = File1.ListCount
    status.queue.Value = 1
    
    ' Bereits vorhandene Tracks muessen in eine temporaere Datei
    ' eingelesen werden, damit sie beim Import nicht ueberschrieben
    ' werden
    
    ff = FreeFile
    Open tmp + "\~wm2in" + Format$(Seite) + ".tmp" For Random As #ff Len = Len(el)
    
    file = tmp + "\~wm2mp3import.tmp"
    Kill file
    ImportFile = FreeFile
    Open file For Random As #ImportFile Len = Len(mp3impdata)
    
    zz = LOF(ff) / Len(el)
    If zz > 0 Then
      For i = 1 To zz
        Get #ff, i, mp3impdata.Ti
        mp3impdata.Ti.track = i
        mp3impdata.Ti.Index = Index
        mp3impdata.Ti.Seite = Seite
        mp3impdata.Ti.traeger = traeger
        mp3impdata.filename = ""
        mp3impdata.status = 1
      Next
    End If
    Close ff
    
    For i = 0 To File1.ListCount - 1
        status.queue.Value = i + 1
        
        file = File1.Path + "\" + File1.List(i)
        
        If (auswahl(0).Value = True Or (auswahl(1).Value = True And File1.Selected(i) = True)) Then
            Debug.Print file
            '' Ist das File schon indiziert?
            
        End If
        
        If status.abbrechen.tag = "stopit" Then
          mp3load.Enabled = True
          Close ImportFile
          Unload status
          Exit Sub
        End If
        
    Next
    Close ImportFile
    Unload status
    mp3load.Enabled = True
    Unload mp3load
    mp3import.Show
    
End If


mp3load.Enabled = True

End Sub

Private Sub OK_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load

End Sub

Private Sub rename_Click(Index As Integer)
If Index = 0 Then filter.text = "???.mp3"
End Sub

Private Sub rename_KeyPress(Index As Integer, KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub

Private Sub tracklaenge_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub

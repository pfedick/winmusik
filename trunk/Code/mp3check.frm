VERSION 5.00
Begin VB.Form mp3check 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "MP3-Check"
   ClientHeight    =   3780
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5535
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3780
   ScaleWidth      =   5535
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows-Standard
   Begin VB.CommandButton Command2 
      Caption         =   "Schliessen"
      Height          =   375
      Left            =   3660
      TabIndex        =   24
      Top             =   3300
      Width           =   1815
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Reparieren"
      Enabled         =   0   'False
      Height          =   375
      Left            =   60
      TabIndex        =   23
      Top             =   3300
      Width           =   1575
   End
   Begin VB.Label datastart 
      BorderStyle     =   1  'Fest Einfach
      Height          =   315
      Left            =   3840
      TabIndex        =   22
      Top             =   2820
      Width           =   1635
   End
   Begin VB.Label Label12 
      AutoSize        =   -1  'True
      Caption         =   "Beginn der Daten:"
      Height          =   195
      Left            =   2340
      TabIndex        =   21
      Top             =   2880
      Width           =   1290
   End
   Begin VB.Label dateigroesse 
      BorderStyle     =   1  'Fest Einfach
      Height          =   315
      Left            =   1020
      TabIndex        =   20
      Top             =   2820
      Width           =   1155
   End
   Begin VB.Label Label11 
      AutoSize        =   -1  'True
      Caption         =   "Dateigröße:"
      Height          =   195
      Left            =   60
      TabIndex        =   19
      Top             =   2880
      Width           =   825
   End
   Begin VB.Label bemerkung 
      BorderStyle     =   1  'Fest Einfach
      Height          =   315
      Left            =   1020
      TabIndex        =   18
      Top             =   2400
      Width           =   4455
   End
   Begin VB.Label Label10 
      AutoSize        =   -1  'True
      Caption         =   "min"
      Height          =   195
      Left            =   3840
      TabIndex        =   17
      Top             =   2100
      Width           =   240
   End
   Begin VB.Label laenge 
      BorderStyle     =   1  'Fest Einfach
      Height          =   315
      Left            =   3000
      TabIndex        =   16
      Top             =   2040
      Width           =   795
   End
   Begin VB.Label Label9 
      AutoSize        =   -1  'True
      Caption         =   "Länge:"
      Height          =   195
      Left            =   2460
      TabIndex        =   15
      Top             =   2100
      Width           =   495
   End
   Begin VB.Label jahr 
      BorderStyle     =   1  'Fest Einfach
      Height          =   315
      Left            =   4680
      TabIndex        =   14
      Top             =   2040
      Width           =   795
   End
   Begin VB.Label genre 
      BorderStyle     =   1  'Fest Einfach
      Height          =   315
      Left            =   780
      TabIndex        =   13
      Top             =   2040
      Width           =   1575
   End
   Begin VB.Label album 
      BorderStyle     =   1  'Fest Einfach
      Height          =   315
      Left            =   780
      TabIndex        =   12
      Top             =   1680
      Width           =   4695
   End
   Begin VB.Label titel 
      BorderStyle     =   1  'Fest Einfach
      Height          =   315
      Left            =   780
      TabIndex        =   11
      Top             =   1260
      Width           =   4695
   End
   Begin VB.Label interpret 
      BorderStyle     =   1  'Fest Einfach
      Height          =   315
      Left            =   780
      TabIndex        =   10
      Top             =   840
      Width           =   4695
   End
   Begin VB.Label filename 
      BorderStyle     =   1  'Fest Einfach
      Height          =   315
      Left            =   780
      TabIndex        =   9
      Top             =   420
      Width           =   4695
   End
   Begin VB.Label trackno 
      BorderStyle     =   1  'Fest Einfach
      Height          =   315
      Left            =   780
      TabIndex        =   8
      Top             =   60
      Width           =   975
   End
   Begin VB.Label Label8 
      AutoSize        =   -1  'True
      Caption         =   "File:"
      Height          =   195
      Left            =   60
      TabIndex        =   7
      Top             =   480
      Width           =   285
   End
   Begin VB.Label Label7 
      AutoSize        =   -1  'True
      Caption         =   "Genre:"
      Height          =   195
      Left            =   60
      TabIndex        =   6
      Top             =   2100
      Width           =   480
   End
   Begin VB.Label Label6 
      AutoSize        =   -1  'True
      Caption         =   "Jahr:"
      Height          =   195
      Left            =   4260
      TabIndex        =   5
      Top             =   2100
      Width           =   345
   End
   Begin VB.Label Label5 
      AutoSize        =   -1  'True
      Caption         =   "Album:"
      Height          =   195
      Left            =   60
      TabIndex        =   4
      Top             =   1740
      Width           =   480
   End
   Begin VB.Label Label4 
      AutoSize        =   -1  'True
      Caption         =   "Bemerkung:"
      Height          =   195
      Left            =   60
      TabIndex        =   3
      Top             =   2460
      Width           =   855
   End
   Begin VB.Label Label3 
      AutoSize        =   -1  'True
      Caption         =   "Titel:"
      Height          =   195
      Left            =   60
      TabIndex        =   2
      Top             =   1320
      Width           =   345
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "Interpret:"
      Height          =   195
      Left            =   60
      TabIndex        =   1
      Top             =   900
      Width           =   630
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Track:"
      Height          =   195
      Left            =   60
      TabIndex        =   0
      Top             =   120
      Width           =   465
   End
End
Attribute VB_Name = "mp3check"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Function LoadFile(track As Long, file As String) As Integer
Dim ret As Integer
Dim id3 As id3tag
Dim id3v2 As id3v2tag
Dim mp3 As MP3HEADER

trackno.Caption = track
filename.Caption = file
ret = GetMP3Info(file, mp3, id3, id3v2)
If ret = 0 Then
    interpret.Caption = Trim(Cut0(id3.artist))
    titel.Caption = Trim(Cut0(id3.SongName))
    album.Caption = Trim(Cut0(id3.album))
    jahr.Caption = Trim(Cut0(id3.year))
    bemerkung.Caption = Trim(Cut0(id3.comment))
    dateigroesse.Caption = Format(mp3.filelength)
    datastart.Caption = Format(mp3.datastart)
    laenge.Caption = Format(mp3.length / 60, "0") + ":" + Format(mp3.length Mod 60, "00")
    Command1.Enabled = False
    If (mp3.dataend < mp3.filelength Or mp3.datastart > 0) Then Command1.Enabled = True
    Debug.Print mp3.filelength, mp3.dataend
    Show
    LoadFile = 1
End If
End Function

Private Sub Command1_Click()
Dim ret As Long
Dim file As String
Dim track As Long
file = Me.filename.Caption
track = Me.trackno.Caption
Me.MousePointer = vbHourglass
Command2.Enabled = False
Command1.Enabled = False
DoEvents
ret = RepairMp3File(file)
Command2.Enabled = True
Me.MousePointer = vbDefault
ret = LoadFile(track, file)

End Sub


Private Sub Command2_Click()
Unload Me

End Sub


Private Sub Form_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload Me
End Sub


VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{9F5F61C6-83A0-11D2-A800-00A0CC20D781}#1.0#0"; "ACD.OCX"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCT2.OCX"
Object = "{5E9E78A0-531B-11CF-91F6-C2863C385E30}#1.0#0"; "MSFLXGRD.OCX"
Begin VB.Form cdread 
   Caption         =   "CD-Read 1.0"
   ClientHeight    =   5790
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9165
   Icon            =   "cdread.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   5790
   ScaleWidth      =   9165
   StartUpPosition =   3  'Windows-Standard
   Begin VB.CommandButton stopp 
      Caption         =   "Stop"
      Enabled         =   0   'False
      Height          =   315
      Left            =   6180
      TabIndex        =   18
      Top             =   2550
      Width           =   1125
   End
   Begin VB.CommandButton start 
      Caption         =   "Start"
      Height          =   315
      Left            =   6180
      TabIndex        =   17
      Top             =   2130
      Width           =   1125
   End
   Begin VB.Frame Ziel 
      Caption         =   "Ziel"
      Height          =   915
      Left            =   60
      TabIndex        =   4
      Top             =   1980
      Width           =   4215
      Begin VB.TextBox prefix 
         Height          =   285
         Left            =   3000
         TabIndex        =   16
         Top             =   540
         Width           =   1065
      End
      Begin MSComCtl2.UpDown UpDown1 
         Height          =   285
         Left            =   2070
         TabIndex        =   14
         Top             =   540
         Width           =   240
         _ExtentX        =   423
         _ExtentY        =   503
         _Version        =   393216
         Enabled         =   -1  'True
      End
      Begin VB.TextBox starttrack 
         Height          =   285
         Left            =   1590
         TabIndex        =   13
         Text            =   "1"
         Top             =   540
         Width           =   435
      End
      Begin VB.CommandButton ordnersuchen 
         Height          =   315
         Left            =   3750
         Picture         =   "cdread.frx":030A
         Style           =   1  'Grafisch
         TabIndex        =   11
         Top             =   210
         Width           =   315
      End
      Begin VB.TextBox ordner 
         Height          =   285
         Left            =   720
         TabIndex        =   10
         Top             =   210
         Width           =   3015
      End
      Begin VB.Label Label4 
         AutoSize        =   -1  'True
         Caption         =   "Prefix:"
         Height          =   195
         Left            =   2490
         TabIndex        =   15
         Top             =   570
         Width           =   435
      End
      Begin VB.Label Label3 
         AutoSize        =   -1  'True
         Caption         =   "Startnummerierung:"
         Height          =   195
         Left            =   120
         TabIndex        =   12
         Top             =   570
         Width           =   1365
      End
      Begin VB.Label Label2 
         AutoSize        =   -1  'True
         Caption         =   "Ordner:"
         Height          =   195
         Left            =   120
         TabIndex        =   9
         Top             =   240
         Width           =   525
      End
   End
   Begin VB.Frame Quelle 
      Caption         =   "Quelle"
      Height          =   1875
      Left            =   0
      TabIndex        =   3
      Top             =   60
      Width           =   6375
      Begin VB.CheckBox ShowArtist 
         Caption         =   "Artist"
         Height          =   255
         Left            =   5490
         TabIndex        =   22
         Top             =   210
         Width           =   675
      End
      Begin MSFlexGridLib.MSFlexGrid grid1 
         Height          =   1155
         Left            =   120
         TabIndex        =   21
         Top             =   600
         Width           =   5115
         _ExtentX        =   9022
         _ExtentY        =   2037
         _Version        =   393216
         Cols            =   3
         ScrollTrack     =   -1  'True
      End
      Begin VB.OptionButton auswahl 
         Caption         =   "markierte Tracks"
         Height          =   255
         Index           =   1
         Left            =   3840
         TabIndex        =   20
         Top             =   210
         Width           =   1515
      End
      Begin VB.OptionButton auswahl 
         Caption         =   "Alle Tracks"
         Height          =   255
         Index           =   0
         Left            =   2700
         TabIndex        =   19
         Top             =   210
         Value           =   -1  'True
         Width           =   1305
      End
      Begin VB.CommandButton Eject 
         Caption         =   "Eject"
         Height          =   255
         Left            =   1860
         TabIndex        =   8
         Top             =   210
         Width           =   765
      End
      Begin MSComCtl2.UpDown spin1 
         Height          =   255
         Left            =   1530
         TabIndex        =   6
         Top             =   210
         Width           =   240
         _ExtentX        =   423
         _ExtentY        =   450
         _Version        =   393216
         Enabled         =   -1  'True
      End
      Begin VB.Label laufwerk 
         Alignment       =   2  'Zentriert
         BackColor       =   &H80000005&
         BorderStyle     =   1  'Fest Einfach
         Caption         =   "1"
         Height          =   255
         Left            =   1170
         TabIndex        =   7
         Top             =   210
         Width           =   315
      End
      Begin VB.Label Label1 
         AutoSize        =   -1  'True
         Caption         =   "CD-Laufwerk:"
         Height          =   195
         Left            =   120
         TabIndex        =   5
         Top             =   240
         Width           =   975
      End
   End
   Begin MSComctlLib.ProgressBar queue 
      Height          =   210
      Left            =   870
      TabIndex        =   2
      Top             =   2940
      Width           =   2325
      _ExtentX        =   4101
      _ExtentY        =   370
      _Version        =   393216
      Appearance      =   0
      Min             =   1e-4
   End
   Begin MSComctlLib.StatusBar StatusBar1 
      Align           =   2  'Unten ausrichten
      Height          =   315
      Left            =   0
      TabIndex        =   1
      Top             =   5475
      Width           =   9165
      _ExtentX        =   16166
      _ExtentY        =   556
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   4
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Object.Width           =   1411
            MinWidth        =   1411
            Text            =   "Track 1"
            TextSave        =   "Track 1"
         EndProperty
         BeginProperty Panel2 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            AutoSize        =   1
            Enabled         =   0   'False
            Object.Width           =   12224
         EndProperty
         BeginProperty Panel3 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Object.Width           =   970
            MinWidth        =   970
            Text            =   "0%"
            TextSave        =   "0%"
         EndProperty
         BeginProperty Panel4 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Style           =   5
            AutoSize        =   2
            Object.Width           =   979
            MinWidth        =   970
            TextSave        =   "12:38"
         EndProperty
      EndProperty
   End
   Begin ACDLib.ACD ACD1 
      Height          =   495
      Left            =   4560
      TabIndex        =   0
      Top             =   3060
      Visible         =   0   'False
      Width           =   495
      _Version        =   65536
      _ExtentX        =   873
      _ExtentY        =   873
      _StockProps     =   0
   End
End
Attribute VB_Name = "cdread"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
DefInt A-Z


Sub GetTracks(laufwerk As Integer)
Dim sec As Long, min As Long, rest As Long
Dim QueryString As String, CddbResponce As String
On Error Resume Next

ret = ACD1.OpenDriveByNumber(laufwerk)
If ret <> 0 Then Exit Sub


grid1.Rows = ACD1.TrackCount + 1

For i = 1 To grid1.Rows - 1
    grid1.Row = i
    grid1.Col = 0
    grid1.text = i

    grid1.Col = 1
    grid1.text = "unbekannt"

    grid1.Col = 2
    sec = ACD1.GetTrackLengthMs(i) / 1000
    min = Int(sec / 60)
    rest = sec Mod 60
    grid1.text = Format(min, "0") + ":" + Format(rest, "00")

Next
DoEvents


'QueryString = Cddb.GetQueryString
'If Err = -2147220902 Then Exit Sub
'CddbResponce = Cddb.GetCddbUseQueryString(QueryString)
'Debug.Print Err
'ListCDDBTracks

End Sub

Sub ListCDDBTracks()

'grid1.Col = 1
'For i = 1 To Cddb.Tracks
'    grid1.Row = i
'    If Trim(Cddb.SongName(i)) <> "" Then
'        If ShowArtist.Value > 0 Then
'            grid1.text = Cddb.Artist + " - " + Cddb.SongName(i)
'        Else
'            grid1.text = Cddb.SongName(i)
'        End If
'    Else
'        grid1.text = "unbekannt"
'    End If
'Next

End Sub

Sub InitGrid()
grid1.Rows = 1
grid1.Row = 0

grid1.Col = 0
grid1.text = "Track"
grid1.ColWidth(0) = 500
grid1.ColWidth(2) = 600
grid1.ColWidth(1) = grid1.Width - 1400

grid1.Col = 1
grid1.text = "Interpret - Titel (Version)"

grid1.Col = 2
grid1.text = "Länge"

End Sub

Sub ReadTrack(i As Integer)

Dim track As Integer
Dim datei As String

StatusBar1.Panels(1).text = "Track " + Format(i)
        track = CLng(starttrack.text)
        starttrack.text = Format(track + 1)
        datei = Trim(ordner.text)
        If right(datei, 1) <> "\" Then datei = datei + "\"
        datei = datei + prefix.text + Format(track, "000") + ".wav"
        queue.min = 0
        queue.max = 1000
        ACD1.tag = ""
        ret = ACD1.ReadTrack(i, datei)
        While (ACD1.tag = "")
            DoEvents
            If ACD1.tag = "stop" Then
                ACD1.Stop
                DoEvents
            End If
        Wend

End Sub

Private Sub ACD1_ActPosition(ByVal Position As Long)
queue.Value = Position
StatusBar1.Panels(3).text = Format(Position / 10, "0") + "%"

'Debug.Print "Max: "; queue.Max; ", Pos: "; Position
End Sub

Private Sub ACD1_Failure(ByVal ErrorCode As Long, ByVal ErrorString As String)
Debug.Print "ACD-Error" + Format(ErrorCode) + ": " + ErrorString
queue.Value = 0
StatusBar1.Panels(3).text = Format(0 / 10, "0") + "%"
If ErrorCode = 701 Then
  StatusBar1.Panels(3).text = "STOP"
Else
    StatusBar1.Panels(3).text = "ERR"
End If
  
End Sub


Private Sub ACD1_ReadComplete()
ACD1.tag = "complete"

End Sub

Private Sub Eject_Click()
'ACD1.Eject
ACD1.OpenTray
GetTracks (spin1.Value)
End Sub

Private Sub Form_Load()
Dim ret As Integer
cdreadvisible = True



ret = ACD1.Init
If ret <> 0 Then
   MsgBox "ACD-Init failed!", vbCritical, "CD-Read"
   End
End If

ret = ACD1.Authorize("nobody", "1326981878")
If ret <> 0 Then
   MsgBox "ACD-Authorisation failed!", vbCritical, "CD-Read"
   End
End If

queue.max = 100
queue.min = 0
queue.Value = 0
StatusBar1.Panels(1).text = ""
StatusBar1.Panels(3).text = ""

spin1.min = 1
spin1.max = ACD1.DriveCount
InitGrid
ordner.text = CDRipperPfad
Show
DoEvents
GetTracks (1)

End Sub

Private Sub Form_Paint()
queue.left = 870
queue.top = Height - 660
queue.Width = StatusBar1.Panels(2).Width - 120
Quelle.Width = Width - 165 - 60
Quelle.Height = Height - 1690

Ziel.top = StatusBar1.top - 900
Ziel.Width = Quelle.Width - start.Width - 120

'grid1.Width = Quelle.Width - 200
'grid1.Height = Quelle.Height - 600
'grid1.ColWidth(1) = grid1.Width - 1400
start.top = Ziel.top + 100
stopp.top = start.top + start.Height + 60

start.left = Width - start.Width - 145
stopp.left = start.left

End Sub

Private Sub Form_Resize()
If Height < 3600 Then Height = 3600
If Width < 6555 Then Width = 6555
'tracklist.Width = Width - 400
'tracklist.Height = Height - 2300
grid1.Width = Width - 400
grid1.Height = Height - 2300
grid1.ColWidth(1) = grid1.Width - 1400

Refresh

End Sub


Private Sub Form_Unload(Cancel As Integer)
cdreadvisible = False
ACD1.tag = "stop"
ACD1.Stop

End Sub

Private Sub grid1_DblClick()
Dim i As Integer
i = grid1.Row
start.Enabled = False
stopp.Enabled = True
Ziel.Enabled = False
Quelle.Enabled = False
DoEvents

ReadTrack (i)

queue.Value = 0
start.Enabled = True
stopp.Enabled = False
Ziel.Enabled = True
Quelle.Enabled = True
DoEvents

End Sub


Private Sub ShowArtist_Click()
'ListCDDBTracks
End Sub

Private Sub spin1_Change()
laufwerk.Caption = spin1.Value
GetTracks (spin1.Value)

End Sub


Private Sub start_Click()
Dim i As Integer

start.Enabled = False
stopp.Enabled = True
Ziel.Enabled = False
Quelle.Enabled = False

For i = 1 To grid1.Rows - 1
    ' Soll der Track eingelesen werden?
        ReadTrack (i)
        If ACD1.tag = "stop" Then
            i = grid1.Rows
        End If
Next

queue.Value = 0
start.Enabled = True
stopp.Enabled = False
Ziel.Enabled = True
Quelle.Enabled = True

End Sub

Private Sub stopp_Click()
ACD1.tag = "stop"
End Sub


Private Sub Toolbar1_ButtonClick(ByVal Button As MSComctlLib.Button)

End Sub

Private Sub tracklist_Resize()
'VScroll1.Left = tracklist.Width - 225 - 60
'VScroll1.Height = tracklist.Height - 60
End Sub



Private Sub UpDown1_DownClick()
t = CLng(starttrack.text) - 1
If t < 1 Then t = 1
starttrack.text = Format(t)

End Sub

Private Sub UpDown1_UpClick()
Dim t As Integer
t = CLng(starttrack.text) + 1
starttrack.text = Format(t)
End Sub


Private Sub VScroll1_Change()

End Sub

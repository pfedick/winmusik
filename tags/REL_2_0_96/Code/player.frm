VERSION 5.00
Object = "{0BA686C6-F7D3-101A-993E-0000C0EF6F5E}#1.0#0"; "THREED32.OCX"
Object = "{27395F88-0C0C-101B-A3C9-08002B2F49FB}#1.1#0"; "PICCLP32.OCX"
Object = "{C1A8AF28-1257-101B-8FB0-0020AF039CA3}#1.1#0"; "MCI32.OCX"
Object = "{FE0065C0-1B7B-11CF-9D53-00AA003C9CB6}#1.0#0"; "COMCT232.OCX"
Begin VB.Form cdplayer 
   Appearance      =   0  '2D
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   1  'Fest Einfach
   Caption         =   "CD-Player 2.0"
   ClientHeight    =   3225
   ClientLeft      =   5280
   ClientTop       =   1860
   ClientWidth     =   3990
   BeginProperty Font 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H80000008&
   Icon            =   "player.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   3225
   ScaleWidth      =   3990
   Begin ComCtl2.UpDown UpDown1 
      Height          =   375
      Left            =   3690
      TabIndex        =   13
      Top             =   390
      Width           =   240
      _ExtentX        =   423
      _ExtentY        =   661
      _Version        =   327681
      Enabled         =   -1  'True
   End
   Begin VB.PictureBox queue 
      Appearance      =   0  '2D
      BackColor       =   &H00000000&
      BorderStyle     =   0  'Kein
      Height          =   135
      Left            =   90
      MousePointer    =   9  'Größenänderung W O
      ScaleHeight     =   135
      ScaleWidth      =   1800
      TabIndex        =   12
      Top             =   615
      Width           =   1800
   End
   Begin VB.PictureBox ShowTimeFeld 
      Appearance      =   0  '2D
      BackColor       =   &H0000FFFF&
      ForeColor       =   &H80000008&
      Height          =   270
      Left            =   2535
      ScaleHeight     =   240
      ScaleWidth      =   525
      TabIndex        =   10
      Top             =   1155
      Visible         =   0   'False
      Width           =   555
      Begin VB.Label ShowTimeWert 
         Alignment       =   2  'Zentriert
         BackStyle       =   0  'Transparent
         Caption         =   "14:00"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   195
         Left            =   30
         TabIndex        =   11
         Top             =   15
         Width           =   465
      End
   End
   Begin VB.PictureBox Step0 
      Appearance      =   0  '2D
      BackColor       =   &H80000005&
      ForeColor       =   &H80000008&
      Height          =   360
      Left            =   135
      Picture         =   "player.frx":030A
      ScaleHeight     =   330
      ScaleWidth      =   465
      TabIndex        =   9
      Top             =   2055
      Width           =   495
   End
   Begin VB.PictureBox step1 
      Appearance      =   0  '2D
      BackColor       =   &H80000005&
      ForeColor       =   &H80000008&
      Height          =   345
      Left            =   735
      Picture         =   "player.frx":04B0
      ScaleHeight     =   315
      ScaleWidth      =   405
      TabIndex        =   8
      Top             =   2040
      Width           =   435
   End
   Begin VB.Timer Timer1 
      Interval        =   500
      Left            =   3000
      Top             =   2640
   End
   Begin Threed.SSPanel Panel3D3 
      Height          =   315
      Left            =   3690
      TabIndex        =   7
      Top             =   30
      Width           =   240
      _Version        =   65536
      _ExtentX        =   423
      _ExtentY        =   556
      _StockProps     =   15
      Caption         =   "Panel3D3"
      BackColor       =   0
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.26
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BevelOuter      =   1
      Begin VB.Image CDLaufwerk 
         Appearance      =   0  '2D
         Height          =   210
         Left            =   30
         Top             =   45
         Width           =   165
      End
   End
   Begin Threed.SSPanel Panel3D2 
      Height          =   255
      Left            =   30
      TabIndex        =   2
      Top             =   795
      Width           =   3930
      _Version        =   65536
      _ExtentX        =   6932
      _ExtentY        =   450
      _StockProps     =   15
      Caption         =   "Panel3D2"
      BackColor       =   0
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BevelOuter      =   1
      Begin VB.Label TrackInfo 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H00000000&
         BackStyle       =   0  'Transparent
         Caption         =   "© by Patrick F.-Productions in 1996/97"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FF00&
         Height          =   210
         Left            =   1305
         TabIndex        =   6
         Top             =   15
         Width           =   3045
      End
      Begin VB.Image ZeitPunkt 
         Appearance      =   0  '2D
         Height          =   195
         Left            =   960
         Picture         =   "player.frx":0648
         Top             =   0
         Width           =   30
      End
      Begin VB.Image Laenge 
         Appearance      =   0  '2D
         Height          =   255
         Index           =   3
         Left            =   480
         Top             =   0
         Width           =   135
      End
      Begin VB.Image Laenge 
         Appearance      =   0  '2D
         Height          =   255
         Index           =   2
         Left            =   240
         Top             =   0
         Width           =   135
      End
      Begin VB.Image Laenge 
         Appearance      =   0  '2D
         Height          =   255
         Index           =   1
         Left            =   120
         Top             =   0
         Width           =   135
      End
      Begin VB.Image Laenge 
         Appearance      =   0  '2D
         Height          =   255
         Index           =   0
         Left            =   0
         Top             =   -15
         Width           =   135
      End
   End
   Begin Threed.SSPanel Panel3D1 
      Height          =   735
      Left            =   30
      TabIndex        =   1
      Top             =   30
      Width           =   3615
      _Version        =   65536
      _ExtentX        =   6376
      _ExtentY        =   1296
      _StockProps     =   15
      Caption         =   "Panel3D1"
      BackColor       =   0
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BevelOuter      =   1
      Begin VB.PictureBox Trackfeld 
         Appearance      =   0  '2D
         BackColor       =   &H00000000&
         BorderStyle     =   0  'Kein
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   690
         Left            =   2505
         ScaleHeight     =   46
         ScaleMode       =   3  'Pixel
         ScaleWidth      =   73
         TabIndex        =   3
         Top             =   15
         Width           =   1095
      End
      Begin VB.Image ZeitAnzeige 
         Appearance      =   0  '2D
         Height          =   510
         Left            =   1920
         Picture         =   "player.frx":06FE
         Top             =   120
         Width           =   480
      End
      Begin VB.Image Image3 
         Appearance      =   0  '2D
         Height          =   105
         Left            =   1515
         Picture         =   "player.frx":09A4
         Top             =   450
         Width           =   255
      End
      Begin VB.Image Image2 
         Appearance      =   0  '2D
         Height          =   105
         Left            =   915
         Picture         =   "player.frx":0A8C
         Top             =   450
         Width           =   255
      End
      Begin VB.Image Image1 
         Appearance      =   0  '2D
         Height          =   105
         Left            =   90
         Picture         =   "player.frx":0B6A
         Top             =   450
         Width           =   450
      End
      Begin VB.Image TrackNr 
         Appearance      =   0  '2D
         Height          =   285
         Index           =   1
         Left            =   360
         Picture         =   "player.frx":0C86
         Top             =   120
         Width           =   180
      End
      Begin VB.Image TrackNr 
         Appearance      =   0  '2D
         Height          =   285
         Index           =   0
         Left            =   120
         Picture         =   "player.frx":0DC8
         Top             =   120
         Width           =   180
      End
      Begin VB.Image Laenge_Punkt 
         Appearance      =   0  '2D
         Height          =   285
         Left            =   1320
         Picture         =   "player.frx":0F0A
         Top             =   120
         Width           =   45
      End
      Begin VB.Image Zeit 
         Appearance      =   0  '2D
         Height          =   285
         Index           =   3
         Left            =   1680
         Picture         =   "player.frx":0FE4
         Top             =   120
         Width           =   180
      End
      Begin VB.Image Zeit 
         Appearance      =   0  '2D
         Height          =   285
         Index           =   2
         Left            =   1440
         Picture         =   "player.frx":1126
         Top             =   120
         Width           =   180
      End
      Begin VB.Image Zeit 
         Appearance      =   0  '2D
         Height          =   285
         Index           =   1
         Left            =   1080
         Picture         =   "player.frx":1268
         Top             =   120
         Width           =   180
      End
      Begin VB.Image Zeit 
         Appearance      =   0  '2D
         Height          =   285
         Index           =   0
         Left            =   840
         Picture         =   "player.frx":13AA
         Top             =   120
         Width           =   180
      End
   End
   Begin Threed.SSCommand Command3D1 
      Height          =   360
      Left            =   3135
      TabIndex        =   5
      Top             =   1080
      Width           =   420
      _Version        =   65536
      _ExtentX        =   741
      _ExtentY        =   635
      _StockProps     =   78
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BevelWidth      =   0
      Outline         =   0   'False
      AutoSize        =   2
      Picture         =   "player.frx":14EC
   End
   Begin Threed.SSCommand CD_ID 
      Height          =   360
      Left            =   3525
      TabIndex        =   4
      Top             =   1080
      Width           =   420
      _Version        =   65536
      _ExtentX        =   741
      _ExtentY        =   635
      _StockProps     =   78
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BevelWidth      =   0
      Outline         =   0   'False
      AutoSize        =   2
      Picture         =   "player.frx":16C2
   End
   Begin PicClip.PictureClip Clip1 
      Left            =   480
      Top             =   2640
      _ExtentX        =   4233
      _ExtentY        =   900
      _Version        =   327681
      Picture         =   "player.frx":188E
   End
   Begin PicClip.PictureClip Ziff2 
      Left            =   1920
      Top             =   2160
      _ExtentX        =   3122
      _ExtentY        =   344
      _Version        =   327681
      Picture         =   "player.frx":23C0
   End
   Begin PicClip.PictureClip Ziff1 
      Left            =   120
      Top             =   1680
      _ExtentX        =   5398
      _ExtentY        =   503
      _Version        =   327681
      Picture         =   "player.frx":275E
   End
   Begin MCI.MMControl CD 
      Height          =   315
      Left            =   45
      TabIndex        =   0
      Top             =   1095
      Width           =   2370
      _ExtentX        =   4180
      _ExtentY        =   556
      _Version        =   327681
      BorderStyle     =   0
      UpdateInterval  =   500
      Silent          =   -1  'True
      BackVisible     =   0   'False
      StepVisible     =   0   'False
      RecordVisible   =   0   'False
      Shareable       =   -1  'True
      DeviceType      =   ""
      FileName        =   ""
   End
   Begin VB.Image Ziff_B 
      Appearance      =   0  '2D
      Height          =   285
      Index           =   16
      Left            =   1680
      Picture         =   "player.frx":2FA8
      Top             =   2160
      Width           =   45
   End
   Begin VB.Image Ziff_B 
      Appearance      =   0  '2D
      Height          =   285
      Index           =   15
      Left            =   1560
      Picture         =   "player.frx":3082
      Top             =   2160
      Width           =   45
   End
   Begin VB.Image Ziff_B 
      Appearance      =   0  '2D
      Height          =   285
      Index           =   14
      Left            =   1440
      Picture         =   "player.frx":315C
      Top             =   2160
      Width           =   45
   End
End
Attribute VB_Name = "cdplayer"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
DefInt A-Z
Dim ThisTrack As Integer
Dim queuepercent As Integer

Sub DrawQueue()
Dim c&(2)
w = queue.Width: h = queue.Height - 4 * tpy
c&(0) = RGB(0, 128, 0)
c&(1) = RGB(0, 255, 0)
f = 1: mx = w / 100 * queuepercent
For x = 0 To w Step (3 * tpx)
  If x >= mx Then f = 0
  queue.Line (x, tpy)-(x + tpx, h), c&(f), BF
Next
End Sub

Private Sub CD_Done(NotifyCode As Integer)
If DebugLevel > 1 Then
  Debug.Print "CD_Done    Mode:"; cd.Mode
  Debug.Print "           NotifyCode:"; NotifyCode
End If
If cd.UpdateInterval = 0 Then cd.UpdateInterval = 500
End Sub

Private Sub CD_EjectCompleted(ErrorCode As Long)
If DebugLevel > 1 Then
  Debug.Print "CD_EjectCompleted   Mode:"; cd.Mode
  Debug.Print "                    ErrorCode:"; ErrorCode
End If
CDLoaded = False
CDTracks = 0
Trackfeld.Refresh
End Sub

Private Sub CD_ID_Click()
GETCD.Show
End Sub

Private Sub CD_PauseClick(Cancel As Integer)
pause = True
End Sub

Private Sub CD_PlayClick(Cancel As Integer)
pause = False
End Sub

Private Sub CD_StatusUpdate()
Static counter As Integer
Static Lastmode As Integer
Static ShownNoCD As Boolean
Static lastqueue
On Error Resume Next
Mode = cd.Mode
If Mode = 530 And ShownNoCD = True Then
  cd.UpdateInterval = 0: Suchstatus = 530
  Timer1.Interval = 1000
  Timer1.Enabled = True
  DoEvents
  Exit Sub
End If
If DebugLevel > 1 And Mode <> LastStatus Then
  Debug.Print "CD_StatusUpdate  Mode:"; Mode
End If
If Suchstatus > 5 Then Suchstatus = 0
If CDLoaded = False Then ThisTrack = 0
If CDLoaded = True And Suchstatus = 0 And LCase(Headerdatei) <> "none" Then StartSearch

LastStatus = Mode
Select Case LastStatus
  Case 530
     If ShownNoCD = False Then
            ' Keine CD eingelegt, oder Laufwerk offen
            ' "OPEN" wird angezeigt
            TrackNr(0).Picture = Ziff1.GraphicCell(0)
            TrackNr(1).Picture = Ziff1.GraphicCell(0)
            Zeit(0).Picture = Ziff1.GraphicCell(1)
            Zeit(1).Picture = Ziff1.GraphicCell(14)
            Zeit(2).Picture = Ziff1.GraphicCell(11)
            Zeit(3).Picture = Ziff1.GraphicCell(13)
            Laenge_Punkt.Picture = Ziff_B(14).Picture
            a$ = ""
            pause = False
            TracksLoaded = False
            Suchstatus = 0
            CDHdrDatensatz = 0
            CDLoaded = False
            If CDLoaded = True Then
              CDLoaded = False
              CDTracks = 0
              Trackfeld.Refresh
            End If
            queuepercent = 0: DrawQueue
            TrackInfo.Caption = Copyright
            cdplayer.Caption = "CD-Player 2.1"
            If TitelInfoLoaded = True Then Unload TitelInfo
            If TitelListeLoaded = True Then Unload TitelListe
            ShownNoCD = True
            DoEvents
    End If
  Case 526, 525
     If LastStatus = 526 Or pause = True Then
            ' CD spielt
            ' Aktuelle Position wird angezeigt
            ShownNoCD = False
            qq& = Fix((cd.Position - TrackPosition(ThisTrack)) / 1000)
            ll& = Fix(tracklength(ThisTrack) / 1000)
            queuepercent = Fix(qq& / ll& * 100)
            If queuepercent <> lastqueue Then
              lastqueue = queuepercent
              DrawQueue
            End If
            Select Case ShowTime
              Case 0: t& = qq&
              Case 1: t& = ll& - qq&
              Case 2: t& = Fix(cd.Position / 1000)
              Case 3: t& = Fix((CDLength - cd.Position) / 1000)
            End Select
            If LastStatus = 525 Then t& = Fix((cd.From - TrackPosition(ThisTrack)) / 1000)
            m = Fix(t& / 60)
            s = t& Mod 60
            a$ = String$(2 - Len(Format$(m)), "0") + Format$(m)
            a$ = a$ + String$(2 - Len(Format$(s)), "0") + Format$(s)
            ' Doppelpunkt blinkt
            If counter = 1 Then
              Laenge_Punkt.Picture = Ziff_B(16).Picture
            Else
              Laenge_Punkt.Picture = Ziff_B(14).Picture
            End If
            counter = counter + 1
            If counter > 1 Then counter = 0
            If LastStatus = 526 Then pause = 0
     End If
  Case 525
            ShownNoCD = False
            ' CD vorhanden, kein Play
            ' Gesamtzeit wird angezeigt
            If pause = False Then
              t& = Fix(CDLength / 1000)
              m = Fix(t& / 60)
              s = t& Mod 60
              a$ = String$(2 - Len(Format$(m)), "0") + Format$(m)
              a$ = a$ + String$(2 - Len(Format$(2)), "0") + Format$(2)
            End If
  Case Else
             ShownNoCD = False
             For i = 0 To 3
               Zeit(i).Picture = Ziff1.GraphicCell(0)
             Next
             Laenge_Punkt.Picture = Ziff_B(14).Picture
             a$ = ""
End Select
' Track ausgeben
tr = Current_Track()
If LastStatus <> Lastmode Then
  Lastmode = LastStatus
  'If Lastmode = 526 Then
  '  StepForward.Picture = Step1.Picture
  '  StepForward.Enabled = True
  'Else
  '  StepForward.Picture = step0.Picture
  '  StepForward.Enabled = False
  'End If
End If


If tr <> ThisTrack And Mode <> 530 Then
  ThisTrack = tr
  Trackfeld.Refresh
  b$ = Format$(ThisTrack): b$ = right$("00" + b$, 2)
  For i = 0 To 1
    TrackNr(i).Picture = Ziff1.GraphicCell(1 + Val(Mid$(b$, i + 1, 1)))
  Next
  t& = Fix((tracklength(ThisTrack)) / 1000)
  m = Fix(t& / 60)
  s = t& Mod 60
  b$ = String$(2 - Len(Format$(m)), "0") + Format$(m)
  b$ = b$ + String$(2 - Len(Format$(s)), "0") + Format$(s)
  For i = 0 To 3
    Laenge(i).Picture = Ziff2.GraphicCell(1 + Val(Mid$(b$, i + 1, 1)))
  Next
  If Suchstatus = 3 Then
    TrackInfo.Caption = TrackName(ThisTrack)
  End If
  If TitelInfoLoaded = True Then
    Unload TitelInfo
    TitelInfo.Show
  End If
End If

' Zeit ausgeben

If a$ <> "" Then
  For i = 0 To 3
    Zeit(i).Picture = Ziff1.GraphicCell(1 + Val(Mid$(a$, i + 1, 1)))
  Next
End If
End Sub

Private Sub cdwahl_MouseUp(Index As Integer, Button As Integer, Shift As Integer, x As Single, y As Single)
cd.SetFocus
End Sub

Private Sub Command3D1_Click()
If TracksLoaded = True And CDLoaded = True Then
  TitelListe.Show
End If
End Sub

Private Function Current_Track() As Integer
' Ermitteln des aktuellen Tracks

' Prüfen, ob CD eingelegt ist
If cd.Mode <> 530 Then
  ' Prüfen, ob aktuelle CD-Informationen schon geladen sind
  If CDLoaded = False Then LoadCD
  pp& = cd.Position
  For i = 1 To CDTracks
    If TrackPosition(i) > pp& Then Exit For
  Next
  i = i - 1
  If i > CDTracks Then i = 1
  RunningTrack = i
  Current_Track = i
Else
  RunningTrack = 0
  Current_Track = 0
End If
End Function


Private Sub Form_Load()
Dim rec As Rect
openini "wmusik20"
selectTopic "Fenster"
a$ = GetINI$("CDPlay20")
If inierror = 0 Then
  String2Rect a$, rec
Else
  rec.left = (Screen.Width - 4125) / 2: rec.top = (Screen.Height - 1860) / 2
End If
rec.right = 4080: rec.bottom = 1860
cdplayer.Width = rec.right
cdplayer.Height = rec.bottom

If rec.left > Screen.Width - cdplayer.Width Then rec.left = Screen.Width - cdplayer.Width
If rec.top > Screen.Height - cdplayer.Height Then rec.top = Screen.Height - cdplayer.Height
cdplayer.left = rec.left: cdplayer.top = rec.top
Show
DoEvents

Ziff1.Cols = 17
Ziff2.Cols = 16
Clip1.Cols = 5



cdplayervisible = True

For i = 0 To 3
  If i > 1 Then p = 3 Else p = 0
  Laenge(i).left = ((2 + i * 9 + p) * Screen.TwipsPerPixelX)
  Laenge(i).top = Screen.TwipsPerPixelY
  Laenge(i).Picture = Ziff2.GraphicCell(0)
Next
CDLaufwerk.Picture = Ziff2.GraphicCell(0)
ZeitPunkt.top = Screen.TwipsPerPixelY
ZeitPunkt.left = ((2 + 2 * 9) * Screen.TwipsPerPixelX)
TrackInfo.left = (9 + 4 * 9) * Screen.TwipsPerPixelX
TrackInfo.Caption = Copyright

cd.DeviceType = "CDAudio"
cd.UpdateInterval = 500
cd.Command = "Close"
cd.Command = "Open"
If DebugLevel > 0 Then
  Debug.Print "CD-Player started"
End If
cd.TimeFormat = 0

Timer1.Enabled = True
Timer1.Interval = 500
Suchstatus = 530: CDLoaded = False

Trackfeld.ToolTipText = Lang(65)
ZeitAnzeige.ToolTipText = Lang(308)
queue.ToolTipText = Lang(309)
CD_ID.ToolTipText = Lang(310)
Command3D1.ToolTipText = Lang(311)
a$ = Lang(312)
Laenge(0).ToolTipText = a$
Laenge(1).ToolTipText = a$
Laenge(2).ToolTipText = a$
Laenge(3).ToolTipText = a$
cdplayervisible = True
PrepareFastsearch
End Sub

Private Sub Form_Unload(Cancel As Integer)
cd.Command = "Close"
Unload TitelInfo
Unload TitelListe
Unload GETCD
Dim f As Rect
f.top = cdplayer.top
f.left = cdplayer.left
openini "wmusik20"
selectTopic "Fenster"
a$ = Rect2String(f)
putini "CDPlay20=" + a$
closeini
cdplayervisible = False
End Sub

Private Sub LoadCD()
' Laden der aktuellen CD-Informationen
'
' TrackPosition(xx) = Startposition eines Tracks
' TrackLength(xx)   = Länge des Tracks
' CDLength          = Gesamtlaenge
' CDStart           = Startposition der CD
' CDTracks          = Anzahl Tracks
'
' Die Variable CDLoaded wird anschliessend auf TRUE
' gesetzt
' *************************************************

' Prüfen, ob CD einliegt
If cd.Mode <> 530 Then
  CDTracks = cd.tracks
  If CDTracks = 0 Then
    cd.Command = "close"
    cd.Command = "open"
    CDTracks = cd.tracks
  End If
  cd.TimeFormat = 0
  CDLength = cd.Length
  CDStart = cd.Start
  For i = 1 To CDTracks
    cd.Track = i
    TrackPosition(i) = cd.TrackPosition
    tracklength(i) = cd.tracklength
  Next
  CDLoaded = True
  Trackfeld.Refresh
Else
  CDLoaded = False
End If
End Sub

Private Sub LoadTracks()
On Error Resume Next
Dim version As String * 80
TrVFile = FreeFile
Open DatenPfad + "\" + TraegerBez(CDHead.TraegerArt) + ".dir" For Random As #TrVFile Len = Len(CDTrV)
If Err = 0 Then
  If (LOF(TrVFile) / Len(CDTrV)) >= CDHead.Index Then
    Get #TrVFile, CDHead.Index, CDTrV
    If CDTrV.Bezeichnung > 0 Then
      ff = FreeFile
      Open DatenPfad + "\traeger.tit" For Random As #ff Len = Len(CDTrTi)
      Get #ff, CDTrV.Bezeichnung, CDTrTi
      If Err > 0 Then
        CDTrTi.Titel1 = "Error: CDTrV.Titelnummer > CD-Title-Filelength"
        CDTrTi.Titel2 = ""
      End If
      t$ = Trim$(CDTrTi.Titel1) + ": " + Trim$(CDTrTi.Titel2)
      Close #ff
    Else
      t$ = "ohne Titel"
    End If
    If CDTrV.Seiten > 1 Then
      cdplayer.Caption = "CD " + Format$(CDHead.Index) + "-" + Format$(CDHead.Seite) + ": " + t$
    Else
      cdplayer.Caption = "CD " + Format$(CDHead.Index) + ": " + t$
    End If
    Close #TrVFile
    TIFile = FreeFile
    Open TiDatei For Random As #TIFile Len = Len(CDTi)
    If Err = 0 Then
      If LOF(TIFile) > 0 Then
        For i = 1 To 50
          tracks(i) = 0: TrackName(i) = ""
        Next
        ff = FreeFile
        p$ = DatenPfad + "\" + TraegerBez(CDHead.TraegerArt) + ".dat"
        Open p$ For Random As #ff Len = Len(CDTrDat)
        If Err = 0 Then
          If LOF(ff) > 0 Then
            tr = Current_Track()
            vv = FreeFile
            Open DatenPfad + "\version.bez" For Random As #vv Len = 80
            If Err > 0 Then Close #vv: vv = 0
            Dim pp As Long
            pp = CDTrV.ErsterDatensatz
            Do
              Get #ff, pp, CDTrDat
              If Val(CDTrDat.Seite) = CDHead.Seite Then
                Get #TIFile, CDTrDat.Titelnummer, CDTi
                If CDTi.Laenge <= 0 Then
                  tt& = Fix((tracklength(CDTrDat.Track)) / 1000)
                  m = Fix(tt& / 60)
                  s = tt& Mod 60
                  CDTi.Laenge = (m * 100 + s) / 100
                  Put #TIFile, CDTrDat.Titelnummer, CDTi
                End If
                tracks(CDTrDat.Track) = CDTrDat.Titelnummer
                t$ = Trim$(CDTi.Interpret) + ": " + Trim$(CDTi.Titel)
                If vv > 0 Then
                  Get #vv, CDTi.version, version
                  t$ = t$ + " (" + Trim$(version) + ")"
                End If
                TrackName(CDTrDat.Track) = t$
                If CDTrDat.Track = tr Then TrackInfo.Caption = t$
              End If
              pp = CDTrDat.NaechsterDatensatz
              If pp = 0 Then Exit Do
            Loop Until EOF(ff)
            If vv > 0 Then Close #vv
          Else
            TrackInfo.Caption = Lang(131): 'Keine Tracks gespeichert
          End If
        Else
          LangPar 2, p$
          TrackInfo.Caption = Lang(132): 'p$ wurde nicht gefunden
        End If
        Close #ff
      Else
        TrackInfo.Caption = Lang(133): ' Titeldatei ist leer
      End If
    Else
      TrackInfo.Caption = Lang(134): 'Titeldatei nicht gefunden
    End If
    Close #TIFile
    TracksLoaded = True
  Else
    TrackInfo.Caption = Lang(135): 'CD noch nicht erfaßt
    TracksLoaded = False
  End If
Else
  TracksLoaded = False
End If
Close TrVFile
End Sub

Private Sub StartSearch()
On Error Resume Next
TrackInfo.Caption = Lang(136): 'Beginne Suche
DoEvents
Suchstatus = 1
Close
HDRFile = FreeFile
Open CDHeaderDatei For Random As #HDRFile Len = Len(CDHead)
If Err > 0 Then
  stopsearch
  Suchstatus = 4
  TrackInfo.Caption = Lang(137): 'Error: CD-Header-File not found
Else
  Suchstatus = 2
  CDHdrDatensatz = 0
  CDHead.Index = 0: CDHead.Seite = 1
  CDHead.TraegerArt = 2
  CDHdrPos = LOF(HDRFile) / Len(CDHead)
  CDHead.tracks = CDTracks
  CDHead.Gesamtlaenge = CDLength
  CDHead.Startposition = CDStart
  ' Hier kommt die Quicksearch rein
  If CDHdrPos > CDFastsearchEnd Then
    PrepareFastsearch
  End If
  For i = CDFastsearchEnd To CDFastsearchStart Step -1
    If CDFastsearch(i - CDFastsearchStart) = CDHead.Gesamtlaenge Then
      CDHdrPos = i: Exit For
    End If
  Next
      
  For i = 1 To 10
    If i <= CDTracks Then
      CDHead.StartPos(i) = TrackPosition(i)
    Else
      CDHead.StartPos(i) = 0
    End If
  Next
  If LOF(HDRFile) > 0 Then
    Timer1.Interval = 100
    Timer1.Enabled = True
    TrackInfo.Caption = Lang(138): 'Searching...
  Else
    stopsearch
    Suchstatus = 4
    TrackInfo.Caption = Lang(139): 'CD-Header-File empty
  End If
End If
End Sub





Private Sub queue_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
If Button > 0 And x > 0 And x < queue.Width Then
  ll& = tracklength(ThisTrack)
  pp! = x / queue.Width * 100
  np& = Fix(TrackPosition(ThisTrack) + ll& * pp! / 100)
  If x + ShowTimeFeld.Width < queue.Width Then
    ShowTimeFeld.left = x + queue.left
  Else
    ShowTimeFeld.left = x + queue.left - ShowTimeFeld.Width
  End If
  ShowTimeFeld.top = queue.top - ShowTimeFeld.Height
  ShowTimeFeld.Visible = True
  t& = Fix(np& - TrackPosition(ThisTrack)) / 1000
  m = Fix(t& / 60)
  s = t& Mod 60
  a$ = String$(2 - Len(Format$(m)), "0") + Format$(m)
  a$ = a$ + ":" + String$(2 - Len(Format$(s)), "0") + Format$(s)
  ShowTimeWert.Caption = a$
  DoEvents
End If
End Sub

Private Sub queue_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
ShowTimeFeld.Visible = False
If Button = 1 Then
  ll& = tracklength(ThisTrack)
  pp! = x / queue.Width * 100
  np& = Fix(TrackPosition(ThisTrack) + ll& * pp! / 100)
  If cd.Mode = 526 Then
    cd.Command = "stop"
    cd.From = np&
    cd.Command = "play"
  Else
    cd.From = np&
  End If
End If

End Sub

Private Sub StepForward_Click()
End Sub

Private Sub stopsearch()
TrackInfo.Caption = Lang(140): 'Search halted
DoEvents
Suchstatus = 3
Close #HDRFile
Timer1.Enabled = False
Timer1.Interval = 0
End Sub

Private Sub queue_Paint()
DrawQueue
End Sub

Private Sub Timer1_Timer()
Static suchsymbol As Integer
If CDLoaded = True And Suchstatus = 2 Then
  TrackInfo.Caption = Lang(138) + " " + Mid$("–\|/", suchsymbol + 1, 1)
  suchsymbol = suchsymbol + 1: If suchsymbol > 3 Then suchsymbol = 0
  For i = 1 To 50
    If CDHdrPos < 1 Then
      stopsearch
      Suchstatus = 4
      TrackInfo.Caption = "CD not found"
      CDHdrPos = 0
      Exit For
    Else
      Get #HDRFile, CDHdrPos, SuchHDR
      If CDHead.Gesamtlaenge = SuchHDR.Gesamtlaenge Then
        If CDHead.Startposition = SuchHDR.Startposition Then
          If CDHead.tracks = SuchHDR.tracks Then
            zz = 0
            For v = 1 To 10
              If CDHead.StartPos(v) = SuchHDR.StartPos(v) Then zz = zz + 1
            Next
            If zz = 10 Then
              CDHdrDatensatz = CDHdrPos
              stopsearch
              Suchstatus = 3
              TrackInfo.Caption = Lang(141): 'Found, loading Tracks...
              Exit For
              DoEvents
            End If
          End If
        End If
      End If
    End If
    CDHdrPos = CDHdrPos - 1
  Next
  If CDHdrPos = 0 Then
    'GETCD.Show
    stopsearch
    DoEvents
    TrackInfo.Caption = Lang(135)
    
    Suchstatus = 5
  End If
  If Suchstatus = 3 Then
    CDHead = SuchHDR
    LoadTracks
  End If
ElseIf Suchstatus = 530 Then
  If cd.Mode <> 530 Then
    cd.UpdateInterval = 500
    Timer1.Enabled = False
    t& = Fix(CDLength / 1000)
    m = Fix(t& / 60): s = t& Mod 60
    a$ = String$(2 - Len(Format$(m)), "0") + Format$(m)
    a$ = a$ + String$(2 - Len(Format$(s)), "0") + Format$(s)
    For i = 0 To 3
      Zeit(i).Picture = Ziff1.GraphicCell(1 + Val(Mid$(a$, i + 1, 1)))
    Next
  End If
Else
  stopsearch
  Suchstatus = 4
End If
End Sub

Private Sub Trackfeld_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
Mode = cd.Mode
If Mode <> 530 Then
  xa = 0: ya = 1: tr = 0
  For i = 0 To 19
    If i >= CDTracks Then Exit For
    If x > xa And x < xa + 14 And y > ya And y < ya + 10 Then tr = i + 1: Exit For
    xa = xa + 14
    If (i + 1) Mod 5 = 0 Then xa = 0: ya = ya + 10
    If ya = 21 Or ya = 42 Then ya = ya + 4
  Next
  If tr > 0 Then
    'If Mode = 526 Then cd.Command = "stop"
    cd.From = TrackPosition(tr)
    cd.To = CDLength
    If Button = 1 Then cd.Command = "play"
  End If
End If

End Sub

Private Sub Trackfeld_Paint()
tr = Current_Track() - 1
If tr < 0 Then tr = 0
x = 0: y = 1: c& = 32768
For i = 0 To 19
  If i = tr Then xa = x: ya = y: t = i + 1
  If i < CDTracks Then Trackfeld.Line (x, y)-(x + 14, y + 10), c&, B
  Trackfeld.CurrentX = x + 2: Trackfeld.CurrentY = y
  a$ = Format$(i + 1)
  Trackfeld.ForeColor = c&
  Trackfeld.Print a$
  x = x + 14
  If (i + 1) Mod 5 = 0 Then x = 0: y = y + 10
  If y = 21 Or y = 42 Then y = y + 4
Next
If CDLoaded = True Then
  c& = 65280
  a$ = Format$(t)
  Trackfeld.Line (xa, ya)-(xa + 14, ya + 10), c&, B
  Trackfeld.CurrentX = xa + 2: Trackfeld.CurrentY = ya
  Trackfeld.ForeColor = c&
  Trackfeld.Print a$
End If
End Sub

Private Sub TrackInfo_Click()
If TracksLoaded = True And CDLoaded = True Then
  TitelInfo.Show
  TitelInfo.Visible = True
End If
End Sub

Private Sub ZeitAnzeige_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
If Button = 1 Then
  ShowTime = ShowTime + 1
  If ShowTime > 3 Then ShowTime = 0
Else
  ShowTime = ShowTime - 1
  If ShowTime < 0 Then ShowTime = 3
End If
ZeitAnzeige.Picture = Clip1.GraphicCell(ShowTime + 1)

End Sub

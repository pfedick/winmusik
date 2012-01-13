VERSION 5.00
Object = "{27395F88-0C0C-101B-A3C9-08002B2F49FB}#1.1#0"; "PICCLP32.OCX"
Object = "{C1A8AF28-1257-101B-8FB0-0020AF039CA3}#1.1#0"; "MCI32.OCX"
Object = "{05589FA0-C356-11CE-BF01-00AA0055595A}#2.0#0"; "AMOVIE.OCX"
Begin VB.Form cdp3 
   BorderStyle     =   0  'Kein
   Caption         =   "CDPlayer 3.0"
   ClientHeight    =   4515
   ClientLeft      =   6075
   ClientTop       =   4140
   ClientWidth     =   4845
   Icon            =   "cdp3.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   Picture         =   "cdp3.frx":030A
   ScaleHeight     =   301
   ScaleMode       =   0  'Benutzer
   ScaleWidth      =   323
   Begin AMovieCtl.ActiveMovie ActiveMovie1 
      Height          =   60
      Left            =   2985
      TabIndex        =   2
      TabStop         =   0   'False
      Top             =   360
      Width           =   60
      _ExtentX        =   106
      _ExtentY        =   106
      EnableContextMenu=   0   'False
      ShowDisplay     =   0   'False
      ShowControls    =   0   'False
      EnablePositionControls=   0   'False
      EnableSelectionControls=   0   'False
      ShowTracker     =   0   'False
      EnableTracker   =   0   'False
      AllowHideDisplay=   0   'False
      AllowHideControls=   0   'False
      AutoStart       =   -1  'True
      PlayCount       =   0
      Appearance      =   0
      BorderStyle     =   0
      AllowChangeDisplayMode=   0   'False
   End
   Begin MCI.MMControl cd 
      Height          =   330
      Left            =   540
      TabIndex        =   1
      Top             =   3960
      Visible         =   0   'False
      Width           =   3540
      _ExtentX        =   6244
      _ExtentY        =   582
      _Version        =   327681
      AutoEnable      =   0   'False
      Shareable       =   -1  'True
      Enabled         =   0   'False
      DeviceType      =   "cdaudio l"
      FileName        =   ""
   End
   Begin VB.Timer Timer1 
      Interval        =   500
      Left            =   4260
      Top             =   3060
   End
   Begin PicClip.PictureClip dott1 
      Left            =   4260
      Top             =   2160
      _ExtentX        =   159
      _ExtentY        =   503
      _Version        =   327681
      Picture         =   "cdp3.frx":17740
   End
   Begin PicClip.PictureClip cdcopyright 
      Left            =   1860
      Top             =   3240
      _ExtentX        =   3969
      _ExtentY        =   476
      _Version        =   327681
      Rows            =   2
      Picture         =   "cdp3.frx":1790E
   End
   Begin PicClip.PictureClip cdactive 
      Left            =   1860
      Top             =   2940
      _ExtentX        =   1535
      _ExtentY        =   476
      _Version        =   327681
      Rows            =   2
      Picture         =   "cdp3.frx":19928
   End
   Begin PicClip.PictureClip but3 
      Left            =   60
      Top             =   2640
      _ExtentX        =   7620
      _ExtentY        =   397
      _Version        =   327681
      Picture         =   "cdp3.frx":1A5DA
   End
   Begin PicClip.PictureClip tnum1 
      Left            =   960
      Top             =   2880
      _ExtentX        =   1588
      _ExtentY        =   1058
      _Version        =   327681
      Rows            =   4
      Cols            =   5
      Picture         =   "cdp3.frx":1D8CC
   End
   Begin PicClip.PictureClip tnum0 
      Left            =   60
      Top             =   2880
      _ExtentX        =   1588
      _ExtentY        =   1058
      _Version        =   327681
      Rows            =   4
      Cols            =   5
      Picture         =   "cdp3.frx":1F53E
   End
   Begin VB.PictureBox queue 
      AutoSize        =   -1  'True
      BackColor       =   &H00404040&
      BorderStyle     =   0  'Kein
      Height          =   150
      Left            =   165
      Picture         =   "cdp3.frx":211B0
      ScaleHeight     =   10
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   182
      TabIndex        =   0
      Top             =   945
      Width           =   2730
   End
   Begin PicClip.PictureClip bigdigits 
      Left            =   60
      Top             =   2340
      _ExtentX        =   6985
      _ExtentY        =   503
      _Version        =   327681
      Picture         =   "cdp3.frx":2275A
   End
   Begin PicClip.PictureClip but2 
      Left            =   4200
      Top             =   60
      _ExtentX        =   1005
      _ExtentY        =   3175
      _Version        =   327681
      Rows            =   10
      Picture         =   "cdp3.frx":26274
   End
   Begin PicClip.PictureClip tdisp 
      Left            =   1860
      Top             =   1860
      _ExtentX        =   3704
      _ExtentY        =   794
      _Version        =   327681
      Picture         =   "cdp3.frx":29926
   End
   Begin PicClip.PictureClip but1 
      Left            =   120
      Top             =   1860
      _ExtentX        =   2910
      _ExtentY        =   291
      _Version        =   327681
      Picture         =   "cdp3.frx":2CAB0
   End
   Begin VB.Image showcdnummer 
      Height          =   225
      Left            =   3285
      Picture         =   "cdp3.frx":2D946
      Top             =   1395
      Width           =   270
   End
   Begin VB.Image showtitelliste 
      Height          =   225
      Left            =   3000
      Picture         =   "cdp3.frx":2DCD0
      Top             =   1395
      Width           =   270
   End
   Begin VB.Image eject 
      Height          =   225
      Left            =   1695
      Picture         =   "cdp3.frx":2E05A
      Top             =   1395
      Width           =   270
   End
   Begin VB.Image jumpnext 
      Height          =   225
      Left            =   1305
      Picture         =   "cdp3.frx":2E3E4
      Top             =   1395
      Width           =   270
   End
   Begin VB.Image stopbutton 
      Height          =   225
      Left            =   1020
      Picture         =   "cdp3.frx":2E76E
      Top             =   1395
      Width           =   270
   End
   Begin VB.Image pause 
      Height          =   225
      Left            =   735
      Picture         =   "cdp3.frx":2EAF8
      Top             =   1395
      Width           =   270
   End
   Begin VB.Image play 
      Height          =   225
      Left            =   450
      Picture         =   "cdp3.frx":2EE82
      Top             =   1395
      Width           =   270
   End
   Begin VB.Image jumpback 
      Height          =   225
      Left            =   165
      Picture         =   "cdp3.frx":2F20C
      Top             =   1395
      Width           =   270
   End
   Begin VB.Image slider 
      Height          =   150
      Left            =   120
      Picture         =   "cdp3.frx":2F596
      Top             =   2100
      Width           =   435
   End
   Begin VB.Image repeat 
      Height          =   180
      Left            =   2325
      Picture         =   "cdp3.frx":2F948
      ToolTipText     =   "CD am Ende wiederholen"
      Top             =   720
      Width           =   570
   End
   Begin VB.Image random 
      Height          =   180
      Left            =   2325
      Picture         =   "cdp3.frx":2FEFA
      ToolTipText     =   "Titel in zufälliger Reihenfolge abspielen"
      Top             =   525
      Width           =   570
   End
   Begin VB.Image display 
      Height          =   180
      Left            =   2325
      Picture         =   "cdp3.frx":304AC
      ToolTipText     =   "Zeitanzeige ändern"
      Top             =   330
      Width           =   570
   End
   Begin VB.Image com3 
      Height          =   165
      Left            =   3900
      Picture         =   "cdp3.frx":30A5E
      Top             =   45
      Width           =   165
   End
   Begin VB.Image com2 
      Height          =   165
      Left            =   3720
      Picture         =   "cdp3.frx":30C2C
      Top             =   45
      Width           =   165
   End
   Begin VB.Image com1 
      Height          =   165
      Left            =   45
      Picture         =   "cdp3.frx":30DFA
      Top             =   45
      Width           =   165
   End
End
Attribute VB_Name = "cdp3"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Dim cccc As Integer
Dim startx As Integer, starty As Integer
Dim mx As Integer, my As Integer, mu As Integer
Dim timedisplaystatus As Integer
Dim random_status As Boolean
Dim repeat_status As Boolean
Dim qx As Integer
Dim slideinprogress As Boolean
Dim timestring As String
Dim trackstring As String
Dim cd_identified As Boolean
Dim cd_loaded As Boolean
Dim player_has_focus As Boolean


Sub update_time()
d$ = " 0123456789ELNPro-ntUA"
cdp3.PaintPicture bigdigits.GraphicCell(InStr(d$, Mid$(timestring, 1, 1)) - 1), 47, 26
cdp3.PaintPicture bigdigits.GraphicCell(InStr(d$, Mid$(timestring, 2, 1)) - 1), 61, 26
cdp3.PaintPicture bigdigits.GraphicCell(InStr(d$, Mid$(timestring, 3, 1)) - 1), 75, 26
cdp3.PaintPicture bigdigits.GraphicCell(InStr(d$, Mid$(timestring, 4, 1)) - 1), 94, 26
cdp3.PaintPicture bigdigits.GraphicCell(InStr(d$, Mid$(timestring, 5, 1)) - 1), 108, 26
End Sub

Sub update_track()
d$ = " 0123456789ELNPro-nt"
cdp3.PaintPicture bigdigits.GraphicCell(InStr(d$, Mid$(trackstring, 1, 1)) - 1), 15, 26
cdp3.PaintPicture bigdigits.GraphicCell(InStr(d$, Mid$(trackstring, 2, 1)) - 1), 29, 26
End Sub

Private Sub Animation1_Click()

End Sub

Private Sub avi1_Error(ErrorCode As String)

End Sub

Private Sub avi1_ExtendedError(ByVal ErrorCode As Long)

End Sub

Private Sub avi1_ExtendedModeChange(ByVal Mode As Long)
Debug.Print Mode
If Mode = 525 Then
  avi1.Position = 0
  avi1.Command = "play"
End If
End Sub

Private Sub avi1_MediaChange(Media As String)

End Sub

Private Sub avi1_ModeChange(Mode As String)

End Sub

Private Sub avi1_Notify(ByVal NotifyCode As Long)
End Sub

Private Sub avi1_Signal(ByVal DeviceID As Long, ByVal dwUserParm As Long)

End Sub

Private Sub avi1_SizeChange()

End Sub

Private Sub CD_StatusUpdate()
t& = Fix(cd.Position / 1000)

m = Fix(t& / 60)
s = t& Mod 60
timestring = " " + Format$(m, "00") + Format$(s, "00")
update_time
       
End Sub

Private Sub com1_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
com1.Picture = but1.GraphicCell(1)
End Sub


Private Sub com1_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
com1.Picture = but1.GraphicCell(0)
End Sub


Private Sub com2_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
com2.Picture = but1.GraphicCell(3)
End Sub

Private Sub com2_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
com2.Picture = but1.GraphicCell(2)
cdp3.WindowState = 1
End Sub

Private Sub com3_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
com3.Picture = but1.GraphicCell(5)
End Sub

Private Sub com3_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
com3.Picture = but1.GraphicCell(4)
Unload cdp3

End Sub

Private Sub display_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
display.Picture = but2.GraphicCell(1)
If Button = 1 Then
  timedisplaystatus = timedisplaystatus + 1
  If timedisplaystatus > 4 Then timedisplaystatus = 1
Else
  timedisplaystatus = timedisplaystatus - 1
  If timedisplaystatus < 1 Then timedisplaystatus = 4
End If
cdp3.PaintPicture tdisp.GraphicCell(timedisplaystatus), 123, 26
End Sub

Private Sub display_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
display.Picture = but2.GraphicCell(0)
End Sub

Private Sub eject_Click()
cd.Command = "Eject"
End Sub

Private Sub eject_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
eject.Picture = but3.GraphicCell(11)

End Sub


Private Sub eject_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
eject.Picture = but3.GraphicCell(10)
End Sub


Private Sub Form_Activate()
player_has_focus = True
Refresh
End Sub

Private Sub Form_Deactivate()
player_has_focus = False
Refresh
End Sub

Private Sub Form_GotFocus()
player_has_focus = True
Refresh

End Sub

Private Sub Form_Load()
Dim rec As Rect

but1.Cols = 10: but1.Rows = 1
tdisp.Cols = 5: tdisp.Rows = 1
but2.Cols = 1: but2.Rows = 10
dott1.Cols = 2: dott1.Rows = 1
bigdigits.Cols = 22: bigdigits.Rows = 1
but3.Cols = 16: but3.Rows = 1
tnum0.Cols = 5: tnum0.Rows = 4
tnum1.Cols = 5: tnum1.Rows = 4
cdactive.Cols = 1: cdactive.Rows = 2
cdcopyright.Cols = 1: cdcopyright.Rows = 2


openini "wmusik20"
selectTopic "Fenster"
a$ = GetINI$("CDPlay20")
Width = 275 * tpx
Height = 115 * tpy

If inierror = 0 Then
  String2Rect a$, rec
Else
  rec.left = (Screen.Width - Width) / 2: rec.top = (Screen.Height - Height) / 2
End If

rec.right = Width: rec.bottom = Height

If rec.left > Screen.Width - Width Then rec.left = Screen.Width - Width
If rec.top > Screen.Height - Height Then rec.top = Screen.Height - Height
left = rec.left: top = rec.top
cdplayervisible = True
trackstring = "--"
timestring = "1N1t "
Show
DoEvents
random_status = False
repeat_status = False
timedisplaystatus = 0
qx = 0
queue.Refresh

cd.DeviceType = "CDAudio"
cd.UpdateInterval = 500
cd.Command = "Close"
cd.Command = "Open"
If DebugLevel > 0 Then
  Debug.Print "CD-Player started"
End If
cd.TimeFormat = 0
End Sub

Private Sub Form_LostFocus()
player_has_focus = False
Refresh
End Sub

Private Sub Form_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
startx = x
starty = y
mx1 = x: my1 = y
cccc = 0
mu = 0: al = 0
Do While mu = 0
  DoEvents
  If mx = mx1 And my = my1 And al = 0 Then
    nx = left + mx - startx: ny = top + my - starty
    Move nx, ny: al = 1
  Else
    mx1 = mx: my1 = my: al = 0
  End If
  
Loop
nx = left + mx - startx: ny = top + my - starty
Move nx, ny

End Sub

Private Sub Form_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
mx = x: my = y
Static inprogress
If Button > 0 And inprogress = 0 Then
  'Debug.Print X, Y
  nx = left + x - startx: ny = top + y - starty
  Move nx, ny
  'MoveWindow hwnd, nx / tpx, ny / tpy, 275, 115, 2
  cccc = cccc + 1
  If cccc > 10 Then
    inprogress = 1
    DoEvents
    inprogress = 0
    cccc = 0
  End If
  
End If

End Sub

Private Sub Form_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
mu = 1
End Sub

Private Sub Form_Paint()
cdp3.PaintPicture tdisp.GraphicCell(timedisplaystatus), 123, 26
update_track
update_time
If player_has_focus = True Then
  cdp3.PaintPicture cdactive.GraphicCell(1), 15, 5
  If cd_loaded = False Then
    cdp3.PaintPicture cdcopyright.GraphicCell(1), 89, 5
  Else

  End If
Else
  cdp3.PaintPicture cdactive.GraphicCell(0), 15, 5
  If cd_loaded = False Then
    cdp3.PaintPicture cdcopyright.GraphicCell(0), 89, 5
  Else

  End If
End If

End Sub

Private Sub Form_Terminate()
player_has_focus = False
Refresh

End Sub

Private Sub Form_Unload(Cancel As Integer)
'CD.Command = "Close"
'Unload TitelInfo
'Unload TitelListe
'Unload GETCD
Dim f As Rect
f.top = top
f.left = left
openini "wmusik20"
selectTopic "Fenster"
a$ = Rect2String(f)
putini "CDPlay20=" + a$
closeini
cdplayervisible = False

End Sub


Private Sub jumpback_Click()
cd.Command = "prev"
End Sub

Private Sub jumpback_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
jumpback.Picture = but3.GraphicCell(1)

End Sub


Private Sub jumpback_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
jumpback.Picture = but3.GraphicCell(0)
End Sub


Private Sub jumpnext_Click()
cd.Command = "next"
End Sub

Private Sub jumpnext_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
jumpnext.Picture = but3.GraphicCell(9)
End Sub


Private Sub jumpnext_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
jumpnext.Picture = but3.GraphicCell(8)
End Sub


Private Sub pause_Click()
Static pause As Boolean

If pause = True Then
  cd.Command = "Play": pause = False
  
Else
  cd.Command = "Pause": pause = True
  
End If
End Sub

Private Sub pause_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)

pause.Picture = but3.GraphicCell(5)


End Sub


Private Sub pause_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
pause.Picture = but3.GraphicCell(4)
End Sub


Private Sub play_Click()
cd.Command = "play"
End Sub

Private Sub play_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
play.Picture = but3.GraphicCell(3)
End Sub


Private Sub play_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
play.Picture = but3.GraphicCell(2)
End Sub


Private Sub queue_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
If qx >= 0 Then
   slideinprogress = True
   qx = x - 14
   If qx < 0 Then qx = 0
   If qx + 29 > 182 Then qx = 182 - 29
   queue.Refresh
End If
End Sub

Private Sub queue_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
If slideinprogress = True And qx >= 0 Then
qx = x - 14
If qx < 0 Then qx = 0
If qx + 29 > 182 Then qx = 182 - 29
queue.Refresh

End If
End Sub

Private Sub queue_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
slideinprogress = False
End Sub


Private Sub queue_Paint()
If qx >= 0 Then
  queue.PaintPicture slider.Picture, qx, 0
End If
End Sub

Private Sub random_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
random.Picture = but2.GraphicCell(5)
If random_status = False Then
  random_status = True
Else
  random_status = False
End If
End Sub

Private Sub random_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
If random_status = False Then
  random.Picture = but2.GraphicCell(2)
Else
  random.Picture = but2.GraphicCell(4)
End If

End Sub

Private Sub repeat_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
repeat.Picture = but2.GraphicCell(9)
If repeat_status = False Then
  repeat_status = True
Else
  repeat_status = False
End If

End Sub

Private Sub repeat_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
If repeat_status = False Then
  repeat.Picture = but2.GraphicCell(6)
Else
  repeat.Picture = but2.GraphicCell(8)
End If

End Sub








Private Sub showcdnummer_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
showcdnummer.Picture = but3.GraphicCell(15)
End Sub


Private Sub showcdnummer_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
showcdnummer.Picture = but3.GraphicCell(14)
End Sub


Private Sub showtitelliste_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
showtitelliste.Picture = but3.GraphicCell(13)
End Sub


Private Sub showtitelliste_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
showtitelliste.Picture = but3.GraphicCell(12)
End Sub


Private Sub stopbutton_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
stopbutton.Picture = but3.GraphicCell(7)
cd.Command = "Stop"
End Sub


Private Sub stopbutton_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
stopbutton.Picture = but3.GraphicCell(6)
End Sub



VERSION 5.00
Object = "{0BA686C6-F7D3-101A-993E-0000C0EF6F5E}#1.0#0"; "THREED32.OCX"
Begin VB.Form TitelListe 
   Appearance      =   0  '2D
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "CD-Player: Titelliste"
   ClientHeight    =   4440
   ClientLeft      =   2085
   ClientTop       =   2580
   ClientWidth     =   5055
   BeginProperty Font 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H00000000&
   Icon            =   "titellis.frx":0000
   LinkTopic       =   "Form5"
   LockControls    =   -1  'True
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   4440
   ScaleWidth      =   5055
   Begin Threed.SSPanel Panel3D1 
      Height          =   4455
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   5055
      _Version        =   65536
      _ExtentX        =   8916
      _ExtentY        =   7858
      _StockProps     =   15
      ForeColor       =   16777215
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
      BevelWidth      =   2
      BevelOuter      =   1
      Begin VB.PictureBox Playlist 
         Appearance      =   0  '2D
         BackColor       =   &H00000000&
         BorderStyle     =   0  'Kein
         ForeColor       =   &H80000008&
         Height          =   4335
         Left            =   60
         ScaleHeight     =   4335
         ScaleWidth      =   4935
         TabIndex        =   1
         Top             =   60
         Width           =   4935
      End
   End
End
Attribute VB_Name = "TitelListe"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()
TitelListeLoaded = True
Dim f As Rect
openini "wmusik20"
selectTopic "Fenster"
a$ = GetINI("TitelListe")
If inierror() = 0 Then
  String2Rect a$, f
  If f.top > Screen.Height Then f.top = Screen.Height - TitelListe.Height
  If f.left > Screen.Width Then f.left = Screen.Width - TitelListe.Width
  TitelListe.top = f.top
  TitelListe.left = f.left
End If
TitelListe.Caption = "CD-Player: " + Lang(159) + " CD " + Format$(CDHead.Index)
closeini
tr = CDHead.tracks
Playlist.Height = (5 + tr * 14 + 20) * Screen.TwipsPerPixelY
Panel3D1.Height = (18 + tr * 14 + 20) * Screen.TwipsPerPixelY
TitelListe.Height = (43 + tr * 14 + 20) * Screen.TwipsPerPixelY
End Sub

Private Sub Form_Unload(Cancel As Integer)
Dim f As Rect
f.top = TitelListe.top
f.left = TitelListe.left
openini "wmusik20"
selectTopic "Fenster"
a$ = Rect2String(f)
putini "TitelListe=" + a$
closeini
TitelListeLoaded = False
End Sub

Private Sub Playlist_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
tr = Fix(((Y / Screen.TwipsPerPixelY) - 20) / 14) + 1
If tr < 1 Then tr = 1
If tr > CDHead.tracks Then tr = CDHead.tracks
cdplayer.CD.From = TrackPosition(tr)
cdplayer.CD.To = CDLength
cdplayer.CD.Command = "play"
End Sub

Private Sub Playlist_Paint()
Playlist.FontName = "Arial"
Playlist.FontSize = 8
starty = 20 * Screen.TwipsPerPixelY
Playlist.ForeColor = RGB(255, 255, 255)
Playlist.FontBold = True
Playlist.CurrentX = 0: Playlist.CurrentY = 0
Playlist.Print Trim(CDTrTi.Titel1) + ", " + Trim(CDTrTi.Titel2)
Y1 = 16 * Screen.TwipsPerPixelY
Playlist.Line (0, Y1)-(Playlist.Width, Y1), RGB(255, 255, 255)
For i = 1 To CDHead.tracks
  Playlist.ForeColor = 40960
  Playlist.FontBold = True
  Playlist.CurrentX = 0
  Playlist.CurrentY = (i - 1) * Screen.TwipsPerPixelY * 14 + starty
  Playlist.Print Format$(i);
  Playlist.CurrentX = 16 * Screen.TwipsPerPixelX
  Playlist.ForeColor = 65280
  Playlist.FontBold = True
  Playlist.Print TrackName(i);
  Y1 = ((i - 1) * 14 + 1) * Screen.TwipsPerPixelY + starty
  Y2 = (i) * Screen.TwipsPerPixelY * 14 + starty
  X = Playlist.Width - 32 * Screen.TwipsPerPixelX
  Playlist.Line (X, Y1)-(Playlist.Width, Y2), 0, BF
  Playlist.ForeColor = 65535
  Playlist.CurrentX = X + 5 * Screen.TwipsPerPixelX
  Playlist.CurrentY = Y1
  t& = Fix((tracklength(i)) / 1000)
  m = Fix(t& / 60)
  s = t& Mod 60
  b$ = String$(2 - Len(Format$(m)), "0") + Format$(m)
  b$ = b$ + ":" + String$(2 - Len(Format$(s)), "0") + Format$(s)
  Playlist.Print b$
  Playlist.Line (0, Y2)-(Playlist.Width, Y2), 24576
Next

End Sub


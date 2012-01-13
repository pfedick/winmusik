VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{0BA686C6-F7D3-101A-993E-0000C0EF6F5E}#1.0#0"; "THREED32.OCX"
Begin VB.Form start2 
   Appearance      =   0  '2D
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "Setup WinMusik"
   ClientHeight    =   3600
   ClientLeft      =   3045
   ClientTop       =   3750
   ClientWidth     =   6240
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
   Icon            =   "START2.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   3600
   ScaleWidth      =   6240
   Begin VB.CommandButton Abbrechen 
      Appearance      =   0  '2D
      BackColor       =   &H80000005&
      Caption         =   "Abbrechen"
      Height          =   315
      Left            =   4440
      TabIndex        =   16
      Top             =   3180
      Width           =   1695
   End
   Begin VB.CommandButton ok 
      Appearance      =   0  '2D
      BackColor       =   &H80000005&
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   315
      Left            =   120
      TabIndex        =   15
      Top             =   3180
      Width           =   1095
   End
   Begin TabDlg.SSTab Ordner 
      Height          =   2460
      Left            =   120
      TabIndex        =   18
      Top             =   600
      Width           =   6015
      _ExtentX        =   10610
      _ExtentY        =   4339
      _Version        =   327680
      Style           =   1
      Tabs            =   5
      Tab             =   2
      TabsPerRow      =   6
      TabHeight       =   529
      ShowFocusRect   =   0   'False
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      TabCaption(0)   =   " User"
      TabPicture(0)   =   "START2.frx":030A
      Tab(0).ControlCount=   7
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "text(11)"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "text(10)"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "text(9)"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "Image1"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "Eingabe(7)"
      Tab(0).Control(4).Enabled=   -1  'True
      Tab(0).Control(5)=   "Eingabe(6)"
      Tab(0).Control(5).Enabled=   -1  'True
      Tab(0).Control(6)=   "Eingabe(5)"
      Tab(0).Control(6).Enabled=   -1  'True
      TabCaption(1)   =   " Path"
      TabPicture(1)   =   "START2.frx":041C
      Tab(1).ControlCount=   15
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "text(6)"
      Tab(1).Control(0).Enabled=   0   'False
      Tab(1).Control(1)=   "text(5)"
      Tab(1).Control(1).Enabled=   0   'False
      Tab(1).Control(2)=   "text(4)"
      Tab(1).Control(2).Enabled=   0   'False
      Tab(1).Control(3)=   "text(3)"
      Tab(1).Control(3).Enabled=   0   'False
      Tab(1).Control(4)=   "text(2)"
      Tab(1).Control(4).Enabled=   0   'False
      Tab(1).Control(5)=   "browse(0)"
      Tab(1).Control(5).Enabled=   0   'False
      Tab(1).Control(6)=   "browse(1)"
      Tab(1).Control(6).Enabled=   0   'False
      Tab(1).Control(7)=   "browse(2)"
      Tab(1).Control(7).Enabled=   0   'False
      Tab(1).Control(8)=   "browse(3)"
      Tab(1).Control(8).Enabled=   0   'False
      Tab(1).Control(9)=   "browse(4)"
      Tab(1).Control(9).Enabled=   0   'False
      Tab(1).Control(10)=   "Eingabe(4)"
      Tab(1).Control(10).Enabled=   -1  'True
      Tab(1).Control(11)=   "Eingabe(3)"
      Tab(1).Control(11).Enabled=   -1  'True
      Tab(1).Control(12)=   "Eingabe(2)"
      Tab(1).Control(12).Enabled=   -1  'True
      Tab(1).Control(13)=   "Eingabe(1)"
      Tab(1).Control(13).Enabled=   -1  'True
      Tab(1).Control(14)=   "Eingabe(0)"
      Tab(1).Control(14).Enabled=   -1  'True
      TabCaption(2)   =   " System"
      TabPicture(2)   =   "START2.frx":0516
      Tab(2).ControlCount=   8
      Tab(2).ControlEnabled=   -1  'True
      Tab(2).Control(0)=   "text(8)"
      Tab(2).Control(0).Enabled=   0   'False
      Tab(2).Control(1)=   "text(7)"
      Tab(2).Control(1).Enabled=   0   'False
      Tab(2).Control(2)=   "text(1)"
      Tab(2).Control(2).Enabled=   0   'False
      Tab(2).Control(3)=   "Label1"
      Tab(2).Control(3).Enabled=   0   'False
      Tab(2).Control(4)=   "sprache"
      Tab(2).Control(4).Enabled=   0   'False
      Tab(2).Control(5)=   "drucker"
      Tab(2).Control(5).Enabled=   0   'False
      Tab(2).Control(6)=   "farbe"
      Tab(2).Control(6).Enabled=   0   'False
      Tab(2).Control(7)=   "Eingabe(8)"
      Tab(2).Control(7).Enabled=   0   'False
      TabCaption(3)   =   " Update"
      TabPicture(3)   =   "START2.frx":0628
      Tab(3).ControlCount=   3
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "u1"
      Tab(3).Control(0).Enabled=   0   'False
      Tab(3).Control(1)=   "u2"
      Tab(3).Control(1).Enabled=   0   'False
      Tab(3).Control(2)=   "Update"
      Tab(3).Control(2).Enabled=   -1  'True
      TabCaption(4)   =   "Sonstiges"
      TabPicture(4)   =   "START2.frx":073A
      Tab(4).ControlCount=   2
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "Check2"
      Tab(4).Control(0).Enabled=   -1  'True
      Tab(4).Control(1)=   "Check1"
      Tab(4).Control(1).Enabled=   -1  'True
      Begin VB.CheckBox Check2 
         Caption         =   "Automatischer Titelvorschlag"
         Height          =   255
         Left            =   -74820
         TabIndex        =   37
         Top             =   780
         Value           =   1  'Aktiviert
         Width           =   3315
      End
      Begin VB.CheckBox Check1 
         Caption         =   "Automatischer Interpretenvorschlag"
         Height          =   255
         Left            =   -74820
         TabIndex        =   36
         Top             =   480
         Value           =   1  'Aktiviert
         Width           =   3555
      End
      Begin VB.CommandButton Update 
         Caption         =   "Update"
         Height          =   375
         Left            =   -72780
         TabIndex        =   35
         Top             =   1920
         Width           =   1335
      End
      Begin VB.TextBox Eingabe 
         Height          =   285
         Index           =   8
         Left            =   1410
         TabIndex        =   31
         Text            =   "dd.mm.yyyy"
         Top             =   1770
         Width           =   1215
      End
      Begin VB.CheckBox farbe 
         Caption         =   "farbig drucken"
         Height          =   315
         Left            =   1200
         TabIndex        =   30
         Top             =   870
         Value           =   1  'Aktiviert
         Width           =   4425
      End
      Begin VB.TextBox Eingabe 
         Height          =   285
         Index           =   5
         Left            =   -72360
         TabIndex        =   0
         Text            =   "Text1"
         Top             =   600
         Width           =   3135
      End
      Begin VB.TextBox Eingabe 
         Height          =   285
         Index           =   6
         Left            =   -72360
         TabIndex        =   1
         Text            =   "Text1"
         Top             =   1080
         Width           =   3135
      End
      Begin VB.TextBox Eingabe 
         Height          =   285
         Index           =   7
         Left            =   -72360
         TabIndex        =   2
         Text            =   "Text1"
         Top             =   1560
         Width           =   735
      End
      Begin VB.TextBox Eingabe 
         Height          =   285
         Index           =   0
         Left            =   -72300
         TabIndex        =   3
         Text            =   "Text1"
         Top             =   480
         Width           =   2715
      End
      Begin VB.TextBox Eingabe 
         Height          =   285
         Index           =   1
         Left            =   -73800
         TabIndex        =   5
         Text            =   "Text1"
         Top             =   840
         Width           =   4215
      End
      Begin VB.TextBox Eingabe 
         Height          =   285
         Index           =   2
         Left            =   -73380
         TabIndex        =   7
         Text            =   "Text1"
         Top             =   1200
         Width           =   3795
      End
      Begin VB.TextBox Eingabe 
         Height          =   285
         Index           =   3
         Left            =   -73740
         TabIndex        =   9
         Text            =   "Text1"
         Top             =   1560
         Width           =   4155
      End
      Begin VB.TextBox Eingabe 
         Height          =   285
         Index           =   4
         Left            =   -72600
         TabIndex        =   11
         Text            =   "Text1"
         Top             =   1920
         Width           =   3015
      End
      Begin VB.ComboBox drucker 
         Height          =   315
         ItemData        =   "START2.frx":0756
         Left            =   1200
         List            =   "START2.frx":0758
         Style           =   2  'Dropdown-Liste
         TabIndex        =   13
         Top             =   480
         Width           =   4635
      End
      Begin VB.ComboBox sprache 
         Height          =   315
         Left            =   1200
         Style           =   2  'Dropdown-Liste
         TabIndex        =   14
         Top             =   1290
         Width           =   4635
      End
      Begin Threed.SSCommand browse 
         Height          =   315
         Index           =   4
         Left            =   -69510
         TabIndex        =   12
         Top             =   1920
         Width           =   345
         _Version        =   65536
         _ExtentX        =   609
         _ExtentY        =   556
         _StockProps     =   78
         ForeColor       =   0
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Picture         =   "START2.frx":075A
      End
      Begin Threed.SSCommand browse 
         Height          =   315
         Index           =   3
         Left            =   -69510
         TabIndex        =   10
         Top             =   1560
         Width           =   345
         _Version        =   65536
         _ExtentX        =   609
         _ExtentY        =   556
         _StockProps     =   78
         ForeColor       =   0
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Picture         =   "START2.frx":0854
      End
      Begin Threed.SSCommand browse 
         Height          =   315
         Index           =   2
         Left            =   -69510
         TabIndex        =   8
         Top             =   1200
         Width           =   345
         _Version        =   65536
         _ExtentX        =   609
         _ExtentY        =   556
         _StockProps     =   78
         ForeColor       =   0
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Picture         =   "START2.frx":094E
      End
      Begin Threed.SSCommand browse 
         Height          =   315
         Index           =   1
         Left            =   -69510
         TabIndex        =   6
         Top             =   840
         Width           =   345
         _Version        =   65536
         _ExtentX        =   609
         _ExtentY        =   556
         _StockProps     =   78
         ForeColor       =   0
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Picture         =   "START2.frx":0A48
      End
      Begin Threed.SSCommand browse 
         Height          =   315
         Index           =   0
         Left            =   -69510
         TabIndex        =   4
         Top             =   480
         Width           =   345
         _Version        =   65536
         _ExtentX        =   609
         _ExtentY        =   556
         _StockProps     =   78
         ForeColor       =   0
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Picture         =   "START2.frx":0B42
      End
      Begin VB.Label u2 
         Caption         =   $"START2.frx":0C3C
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1035
         Left            =   -74880
         TabIndex        =   34
         Top             =   840
         Width           =   5775
      End
      Begin VB.Label u1 
         Alignment       =   2  'Zentriert
         Caption         =   "Überpüfen Sie per Tastendruck, ob Ihre Version von WinMusik noch aktuell ist! "
         Height          =   435
         Left            =   -74880
         TabIndex        =   33
         Top             =   420
         Width           =   5775
      End
      Begin VB.Label Label1 
         AutoSize        =   -1  'True
         Caption         =   "(dd.mm.yyyy)"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   195
         Left            =   2730
         TabIndex        =   32
         Top             =   1800
         Width           =   900
      End
      Begin VB.Label text 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Datumsformat:"
         Height          =   195
         Index           =   1
         Left            =   120
         TabIndex        =   29
         Top             =   1800
         Width           =   1230
      End
      Begin VB.Image Image1 
         Appearance      =   0  '2D
         Height          =   660
         Left            =   -71520
         Picture         =   "START2.frx":0D76
         Stretch         =   -1  'True
         Top             =   1560
         Width           =   2310
      End
      Begin VB.Label text 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Ihr Name:"
         Height          =   195
         Index           =   9
         Left            =   -74760
         TabIndex        =   28
         Top             =   660
         Width           =   840
      End
      Begin VB.Label text 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Firma:"
         Height          =   195
         Index           =   10
         Left            =   -74760
         TabIndex        =   27
         Top             =   1110
         Width           =   525
      End
      Begin VB.Label text 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Die Währung Ihres Landes:"
         Height          =   195
         Index           =   11
         Left            =   -74760
         TabIndex        =   26
         Top             =   1590
         Width           =   2340
      End
      Begin VB.Label text 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Programmpfad von WinMusik:"
         Height          =   195
         Index           =   2
         Left            =   -74880
         TabIndex        =   25
         Top             =   570
         Width           =   2535
      End
      Begin VB.Label text 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Titeldatei:"
         Height          =   195
         Index           =   3
         Left            =   -74880
         TabIndex        =   24
         Top             =   900
         Width           =   870
      End
      Begin VB.Label text 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "CD-Headerdatei:"
         Height          =   195
         Index           =   4
         Left            =   -74880
         TabIndex        =   23
         Top             =   1260
         Width           =   1425
      End
      Begin VB.Label text 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Datenpfad:"
         Height          =   195
         Index           =   5
         Left            =   -74880
         TabIndex        =   22
         Top             =   1650
         Width           =   960
      End
      Begin VB.Label text 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Temporäres Verzeichnis:"
         Height          =   195
         Index           =   6
         Left            =   -74880
         TabIndex        =   21
         Top             =   2010
         Width           =   2100
      End
      Begin VB.Label text 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Drucker:"
         Height          =   195
         Index           =   7
         Left            =   120
         TabIndex        =   20
         Top             =   540
         Width           =   750
      End
      Begin VB.Label text 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Sprache:"
         Height          =   195
         Index           =   8
         Left            =   120
         TabIndex        =   19
         Top             =   1320
         Width           =   780
      End
   End
   Begin VB.Label text 
      Appearance      =   0  '2D
      AutoSize        =   -1  'True
      BackColor       =   &H00C0C0C0&
      BackStyle       =   0  'Transparent
      Caption         =   "WinMusik Setup:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   24
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   555
      Index           =   0
      Left            =   120
      TabIndex        =   17
      Top             =   0
      Width           =   3885
   End
End
Attribute VB_Name = "start2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
DefInt A-Z

Private Sub abbrechen_Click()
Unload start2

End Sub


Private Sub Eingabe_GotFocus(Index As Integer)
SendKeys "{HOME}+{END}"
End Sub


Private Sub Form_Load()
Dim lpt As Printer
Dim tmpstr As String * 256

start2.Caption = Programm + " " + version + ": " + Lang(10)
start2.left = (Screen.Width - start2.Width) / 2
start2.top = (Screen.Height - start2.Height) / 2
text(0).Caption = Programm + " " + Lang(11)

For i = 0 To 9
  text(2 + i).Caption = Lang(15 + i) + ":"
Next

For i = 2 To 0 Step -1
  Ordner.TabCaption(i) = " " + Lang(12 + i)
Next
Ordner.TabCaption(3) = " " + Lang(207)

Check1.Caption = Lang(215)
Check2.Caption = Lang(216)

'Verfügbare Drucker in das Listfeld eintragen
For Each lpt In Printers
  drucker.AddItem lpt.DeviceName
Next
drucker.text = Printer.DeviceName

'Felder ausfüllen
openini "wmusik20.ini"
If inierror() > 0 Then
  openini "win.ini"
  selectTopic "MS User Info"
  Eingabe(5).text = Trim$(GetINI$("DefName"))
  Eingabe(6).text = Trim$(GetINI$("DefCompany"))
  selectTopic "intl"
  Eingabe(7).text = GetINI$("sCurrency")
  HauptPfad = App.Path
  Eingabe(0).text = HauptPfad
  Eingabe(1).text = CurDir + "\titel20.dat"
  Eingabe(2).text = CurDir + "\cdheader.dat"
  Eingabe(3).text = CurDir + "\data"
  a$ = Environ$("TMP")
  If a$ = "" Then a$ = Environ$("TEMP")
  If a$ = "" Then a$ = CurDir + "\tmp"
  Eingabe(4).text = a$
  Check1.Value = 1: Check2.Value = 1
  'farbe.Value = True
Else
  ReadINI
  Eingabe(5).text = User
  Eingabe(6).text = UserFirma
  Eingabe(7).text = Waehrung
  drucker.text = drucker
  farbe.Value = Abs(Farbdruck)
  Eingabe(0).text = HauptPfad
  Eingabe(1).text = TiDatei
  Eingabe(2).text = CDHeaderDatei
  Eingabe(3).text = DatenPfad
  Eingabe(4).text = tmp
  
  Check1.Value = Abs(AutoInterpret)
  Check2.Value = Abs(AutoTitel)
End If

'Sprachen anzeigen
f$ = Dir$(HauptPfad + "\*.lng")
If f$ = "" Then
  MsgBox "Sprachdatei nicht gefunden!" + Chr$(13) + Chr$(13) + "Language-File not found!", 16, Programm + " " + version, HelpFile, 100
Else
  Do
    ff = FreeFile: f$ = HauptPfad + "\" + f$
    Open f$ For Binary As #ff
    If Err = 0 Then
      Get #ff, 1, pfp
      If pfp.pfpID = "PFP-File" And pfp.FileID = "LANG" Then
        Get #ff, 47, p
        Get #ff, p + 1, tmpstr
        n = InStr(tmpstr, Chr$(0))
        sprache.AddItem left$(tmpstr, n - 1)
      End If
    Else
      MsgBox "Sprachdatei nicht gefunden!" + Chr$(13) + Chr$(13) + "Language-File not found!", 16, Programm + " " + version, HelpFile, 101
    End If
    Close #ff
    f$ = Dir$
  Loop Until f$ = ""
End If
sprache.text = LangName()
u1.Caption = Lang(208)
u2.Caption = Lang(209)
update.Caption = Lang(207)
ok.Caption = Lang(1)
abbrechen.Caption = Lang(2)
End Sub




Private Sub ok_Click()
Dim tmpstr As String * 256

On Error Resume Next
User = Trim$(Eingabe(5).text)
UserFirma = Trim$(Eingabe(6).text)
Waehrung = Trim$(Eingabe(7).text)
drucker = Trim$(drucker.text)
Farbdruck = farbe.Value
HauptPfad = Trim$(Eingabe(0).text)
TiDatei = Trim$(Eingabe(1).text)
CDHeaderDatei = Trim$(Eingabe(2).text)
DatenPfad = Trim$(Eingabe(3).text)
tmp = Trim$(Eingabe(4).text)
oldpfad$ = CurDir

'Sprache ermitteln
SprachDatei = LangFile()
f$ = Dir$(HauptPfad + "\*.lng")
If f$ = "" Then
  MsgBox "Sprachdatei nicht gefunden!" + Chr$(13) + Chr$(13) + "Language-File not found!", 16, Programm + " " + version, HelpFile, 100
Else
  Do
    ff = FreeFile: f$ = HauptPfad + "\" + f$
    Open f$ For Binary As #ff
    If Err = 0 Then
      Get #ff, 1, pfp
      If pfp.pfpID = "PFP-File" And pfp.FileID = "LANG" Then
        Get #ff, 47, p
        Get #ff, p + 1, tmpstr
        n = InStr(tmpstr, Chr$(0))
        If left$(tmpstr, n - 1) = Trim$(sprache.text) Then
          SprachDatei = f$
          INITLanguage SprachDatei
          Exit Do
        End If
      End If
    Else
      MsgBox "Sprachdatei nicht gefunden!" + Chr$(13) + Chr$(13) + "Language-File not found!", 16, Programm + " " + version, HelpFile, 101
    End If
    Close #ff
    f$ = Dir$
  Loop Until f$ = ""
End If
ChDir HauptPfad
If Err = 0 Then
  CreatePath DatenPfad
  If Err = 0 Then
    CreatePath TiDatei
    If Err = 0 Then
      CreatePath CDHeaderDatei
      If Err = 0 Then
        CreatePath tmp
        If Err = 0 Then
          SaveINI
          Unload start2
          Main
        Else
          MsgBox Lang(25), 64, Programm + " " + version
        End If
      Else
        MsgBox Lang(26), 64, Programm + " " + version
      End If
    Else
      MsgBox Lang(27), 64, Programm + " " + version
    End If
  Else
    MsgBox Lang(28), 64, Programm + " " + version
  End If
Else
  HauptPfad = oldpfad$: MsgBox Lang(29), 64, Programm + " " + version
End If
ChDir HauptPfad
If Err > 0 Then ChDir oldpfad$
End Sub

Private Sub SaveINI()
openini "wmusik20.ini"
If inierror() > 0 Then
  CreateINI "wmusik20.ini", Programm + " " + version + ", " + Datum$() + ", " + Time$
  selectTopic "Programm"
  putini "Installationsdatum=" + Datum$() + ", " + Time$
End If
selectTopic "Programm"
putini "Benutzer=" + User
putini "Firma=" + UserFirma
putini "Letzter Aufruf=" + Datum$() + ", " + Time$
putini "Währung=" + Waehrung
putini "Drucker=" + drucker
putini "Farbdruck=" + Str$(Farbdruck)
putini "Sprache=" + LangName()
AutoInterpret = Check1.Value
AutoTitel = Check2.Value
putini "AutoInterpret=" + Str$(AutoInterpret)
putini "AutoTitel=" + Str$(AutoTitel)

selectTopic "Pfade"
putini "Main=" + HauptPfad
putini "Titeldatei=" + TiDatei
putini "CDHeaderDatei=" + CDHeaderDatei
putini "Temp=" + tmp
putini "Daten=" + DatenPfad
putini "Sprachdatei=" + LangFile()
closeini
End Sub


Private Sub Update_Click()
On Error Resume Next
If ShowFTPUpdate = False Then
  MsgBox Lang(210), 64, "WinMusik"
  Exit Sub
End If
openini "wmusik20.ini"
selectTopic "Update"
servers$ = GetINI("Server")
If servers$ = "" Or inierror() > 0 Then servers$ = "ftp.pfp.de/pub/win95/WinMusik,ftp.dpn.de/pub/win95/WinMusik"
status.Show
status.Caption = "WinMusik " + Lang(197)
m$ = ""
End Sub



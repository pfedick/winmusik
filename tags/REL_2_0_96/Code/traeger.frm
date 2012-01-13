VERSION 5.00
Begin VB.Form traegerstamm 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "Trägerstamm"
   ClientHeight    =   4245
   ClientLeft      =   450
   ClientTop       =   1710
   ClientWidth     =   6075
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   4245
   ScaleWidth      =   6075
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton coversuch 
      Height          =   315
      Left            =   5700
      Style           =   1  'Grafisch
      TabIndex        =   30
      Top             =   3840
      Width           =   315
   End
   Begin VB.CommandButton Kaufdatum 
      Height          =   315
      Left            =   2280
      Style           =   1  'Grafisch
      TabIndex        =   29
      Top             =   2340
      Width           =   375
   End
   Begin VB.Frame Frame1 
      Caption         =   "Cover"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Left            =   90
      TabIndex        =   26
      Top             =   2940
      Width           =   2985
      Begin VB.CommandButton bearbeiten 
         Caption         =   "Bearbeiten"
         Enabled         =   0   'False
         Height          =   225
         Left            =   1650
         TabIndex        =   27
         Top             =   570
         Width           =   1185
      End
      Begin VB.CommandButton NeuesCover 
         Caption         =   "Neu"
         Height          =   225
         Left            =   120
         TabIndex        =   11
         Top             =   570
         Width           =   645
      End
      Begin VB.ComboBox cover 
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   120
         Style           =   2  'Dropdown-Liste
         TabIndex        =   10
         Top             =   210
         Width           =   2745
      End
   End
   Begin VB.CommandButton abbrechen 
      Caption         =   "Abbrechen"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Left            =   4710
      TabIndex        =   14
      Top             =   2760
      Width           =   1275
   End
   Begin VB.CommandButton ok 
      Caption         =   "OK"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Left            =   4710
      TabIndex        =   13
      Top             =   2370
      Width           =   1275
   End
   Begin VB.TextBox e 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Index           =   10
      Left            =   1050
      TabIndex        =   12
      Text            =   "Text1"
      Top             =   3870
      Width           =   4575
   End
   Begin VB.TextBox e 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Index           =   8
      Left            =   1140
      TabIndex        =   8
      Text            =   "Text1"
      Top             =   2310
      Width           =   1095
   End
   Begin VB.TextBox e 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Index           =   9
      Left            =   600
      TabIndex        =   9
      Text            =   "Text1"
      Top             =   2640
      Width           =   885
   End
   Begin VB.TextBox e 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Index           =   7
      Left            =   1740
      TabIndex        =   7
      Text            =   "Text1"
      Top             =   1980
      Width           =   4245
   End
   Begin VB.TextBox e 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Index           =   6
      Left            =   1110
      TabIndex        =   6
      Text            =   "Text1"
      Top             =   1980
      Width           =   585
   End
   Begin VB.TextBox e 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Index           =   5
      Left            =   2250
      TabIndex        =   5
      Text            =   "Text1"
      Top             =   1500
      Width           =   3735
   End
   Begin VB.TextBox e 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Index           =   4
      Left            =   1680
      TabIndex        =   4
      Text            =   "Text1"
      Top             =   1500
      Width           =   525
   End
   Begin VB.TextBox e 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Index           =   3
      Left            =   600
      TabIndex        =   3
      Text            =   "Text1"
      Top             =   1080
      Width           =   5385
   End
   Begin VB.TextBox e 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Index           =   2
      Left            =   600
      TabIndex        =   2
      Text            =   "Text1"
      Top             =   750
      Width           =   5385
   End
   Begin VB.TextBox e 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Index           =   1
      Left            =   4680
      TabIndex        =   1
      Text            =   "Text1"
      Top             =   420
      Width           =   855
   End
   Begin VB.TextBox e 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Index           =   0
      Left            =   3480
      TabIndex        =   0
      Text            =   "Text1"
      Top             =   420
      Width           =   435
   End
   Begin VB.Label nummer 
      Alignment       =   2  'Zentriert
      BorderStyle     =   1  'Fest Einfach
      Caption         =   "100"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Left            =   900
      TabIndex        =   28
      Top             =   60
      Width           =   915
   End
   Begin VB.Image coverbild 
      BorderStyle     =   1  'Fest Einfach
      Height          =   1395
      Left            =   3120
      Stretch         =   -1  'True
      Top             =   2400
      Width           =   1425
   End
   Begin VB.Label text 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Min."
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
      Index           =   11
      Left            =   5610
      TabIndex        =   25
      Top             =   480
      Width           =   375
   End
   Begin VB.Label text 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Coverbild:"
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
      Index           =   9
      Left            =   90
      TabIndex        =   24
      Top             =   3930
      Width           =   870
   End
   Begin VB.Label text 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Kaufdatum:"
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
      Index           =   6
      Left            =   90
      TabIndex        =   23
      Top             =   2370
      Width           =   975
   End
   Begin VB.Label text 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "DM"
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
      Index           =   8
      Left            =   1560
      TabIndex        =   22
      Top             =   2700
      Width           =   300
   End
   Begin VB.Label text 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Preis:"
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
      Index           =   7
      Left            =   90
      TabIndex        =   21
      Top             =   2700
      Width           =   495
   End
   Begin VB.Label text 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Kaufquelle:"
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
      Index           =   5
      Left            =   90
      TabIndex        =   20
      Top             =   2040
      Width           =   975
   End
   Begin VB.Label text 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Hersteller / Label:"
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
      Index           =   4
      Left            =   90
      TabIndex        =   19
      Top             =   1560
      Width           =   1560
   End
   Begin VB.Label text 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Titel:"
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
      Index           =   3
      Left            =   90
      TabIndex        =   18
      Top             =   780
      Width           =   450
   End
   Begin VB.Label text 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Länge:"
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
      Index           =   2
      Left            =   4050
      TabIndex        =   17
      Top             =   480
      Width           =   600
   End
   Begin VB.Label text 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Anzahl Seiten:"
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
      Index           =   1
      Left            =   2190
      TabIndex        =   16
      Top             =   480
      Width           =   1245
   End
   Begin VB.Label text 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Kassette:"
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
      Index           =   0
      Left            =   90
      TabIndex        =   15
      Top             =   90
      Width           =   810
   End
End
Attribute VB_Name = "traegerstamm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
DefInt A-Z

Dim oldTrV As TraegerVerzeichnis

Private Sub abbrechen_Click()
Unload traegerstamm
End Sub

Private Sub Cover_Change()
If cover.text <> Lang(119) Then
  bearbeiten.Enabled = True
Else
  bearbeiten.Enabled = False
End If
End Sub

Private Sub coversuch_Click()
On Error GoTo ErrHandler
eingabe.dialog.filename = e(10).text
eingabe.dialog.filter = "Windows Bitmap (*.bmp)|*.bmp|Windows Metafile (*.wmf)|*.wmf|Alle Dateien (*.*)|*.*"
eingabe.dialog.FilterIndex = 0
eingabe.dialog.DefaultExt = ".bmp"
eingabe.dialog.DialogTitle = Lang(121)
eingabe.dialog.flags = &H4&
eingabe.dialog.ShowOpen
e(10).text = eingabe.dialog.filename
If Exists(Trim$(e(10).text)) Then
  coverbild.Picture = LoadPicture(Trim$(e(10).text))
End If

ErrHandler:
SetEingabePosition

End Sub

Private Sub e_Change(Index As Integer)
Select Case Index
  Case 4: e(5).text = GetTab("label", Val(e(4).text))
             e(5).tag = e(5).text
  Case 6: e(7).text = GetTab("kauf", Val(e(6).text))
             e(7).tag = e(7).text
End Select
End Sub

Private Sub e_GotFocus(Index As Integer)
SendKeys "{home}+{end}"
End Sub


Private Sub e_KeyDown(Index As Integer, KeyCode As Integer, Shift As Integer)
If KeyCode = 13 Then SendKeys "{tab}"
If KeyCode = 115 And Index = 4 Then ZeigeTabelle "label.bez", traegerstamm.e(4).text, "stamm 4"
If KeyCode = 115 And Index = 6 Then ZeigeTabelle "kauf.bez", traegerstamm.e(6).text, "stamm 6"
End Sub

Private Sub e_LostFocus(Index As Integer)
On Error Resume Next
Select Case Index
  Case 1: If Len(Trim$(e(1).text)) = 0 Then e(1).text = "0"
     e(1).text = Format$(CSng(e(1).text), "##0.00")
  Case 4:
          If InStr(e(4).text, "*") > 0 Then
             e(4).text = Format$(GetNextFreeTab("label")): e(5).text = "": e(4).SetFocus
          Else
             e(5).text = GetTab("label", Val(e(4).text))
             e(5).tag = e(5).text
             If Trim$(e(5).text) <> "" Then e(6).SetFocus
          End If
  Case 5:
          If e(5).text <> e(5).tag Then
            PutTab "label", Val(e(4).text), e(5).text
          End If
  Case 6:
          If InStr(e(6).text, "*") > 0 Then
             e(6).text = Format$(GetNextFreeTab("kauf"))
             e(7).text = "": e(6).SetFocus
          Else
             e(7).text = GetTab("kauf", Val(e(6).text))
             e(7).tag = e(7).text
             If Trim$(e(7).text) <> "" Then e(8).SetFocus
          End If
  Case 7:
          If e(7).text <> e(7).tag Then
            PutTab "kauf", Val(e(6).text), e(7).text
          End If
  Case 8: e(8).text = DatumsAuswertung(e(8).text)
  Case 9: e(9).text = Format$(CSng(e(9).text), "##0.00")
  Case 10:
           If Exists(Trim$(e(10).text)) Then
             coverbild.Picture = LoadPicture(Trim$(e(10).text))
           End If
End Select
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
Select Case KeyCode
  Case 27: Unload traegerstamm
  Case 13: KeyCode = 0: SendKeys "{tab}"
End Select
End Sub


Private Sub Form_KeyPress(KeyAscii As Integer)
If KeyAscii = 13 Then KeyAscii = 0
End Sub

Private Sub Form_KeyUp(KeyCode As Integer, Shift As Integer)
If KeyCode = 13 Then KeyCode = 0
End Sub

Private Sub Form_Load()
Dim rec As Rect
Dim cov As CoverArt
On Error Resume Next
openini AppData + "winmusik2.ini"
selectTopic "Fenster"
a$ = GetINI$("Traegerstamm")
If inierror() = 0 Then
  String2Rect a$, rec
Else
  rec.left = (Screen.Width - traegerstamm.Width) / 2
  rec.top = (Screen.Height - traegerstamm.Height) / 2
End If
traegerstamm.left = rec.left: traegerstamm.top = rec.top
traegerstamm.icon = gfx.TraegerIcon(traeger).Picture
traegerstammvisible = True
LangPar 1, Lang(349 + traeger)
If Traegerloaded = False Then
  TrV.InfoByte = 1
  TrV.Kaufquelle = 0
  TrV.Label = 0
  selectTopic "Default"
  Select Case traeger
    Case 1, 6: TrV.laenge = Val(GetINI$("Kassettenlaenge"))
               If inierror() > 0 Then TrV.laenge = 90
    Case 5:    TrV.laenge = Val(GetINI$("Videolaenge"))
               If inierror() > 0 Then TrV.laenge = 240
    Case Else: TrV.laenge = 0
  End Select
  TrV.rest = 0
  If traeger = 2 Or traeger = 3 Or traeger = 5 Or traeger = 7 Then
    TrV.Seiten = 1
  Else
    TrV.Seiten = 2
  End If
  TrV.Bezeichnung = 0
  TrV.Kaufdatum = dat2long&(Datum$())
  TrV.Erstelldatum = TrV.Kaufdatum
  TrV.Aufnahmestart = TrV.Kaufdatum
  TrV.Aufnahmeende = TrV.Kaufdatum
  TrV.Preis = 0
  Select Case traeger
    Case 1, 6: TrV.Preis = Val(GetINI$("Kassettenpreis"))
    Case 2:    TrV.Preis = Val(GetINI$("CDpreis"))
    Case 5:    TrV.laenge = Val(GetINI$("Videopreis"))
  End Select
  TrV.Tracks = 0
  TrV.Singles = 0
  TrV.Mixe = 0
  TrV.cover = 0
  TrV.coverbild = 0
  TrV.ErsterDatensatz = 0
  traegerstamm.Caption = Lang(102)
Else
  traegerstamm.Caption = Lang(109)
End If
nummer.Caption = Format$(traegerindex)
text(0).Caption = Lang(102 + traeger)
text(1).Caption = Lang(61)
text(2).Caption = Lang(58)
text(11).Caption = Lang(59)
text(3).Caption = Lang(72)
For i = 0 To 3
  text(4 + i).Caption = Lang(110 + i)
Next
text(8).Caption = Waehrung
Frame1.Caption = Lang(114)
text(9).Caption = Lang(115)
NeuesCover.Caption = Lang(118)
bearbeiten.Caption = Lang(120)
ok.Caption = Lang(1)
abbrechen.Caption = Lang(2)
For i = 0 To 10
  e(i).text = ""
Next
cover.AddItem Lang(119)
cover.text = Lang(119)
ff = FreeFile
Open DatenPfad + "\cover.dir" For Random As #ff Len = Len(cov)
If LOF(ff) > 0 Then
  Do
    Get #ff, , cov
    cover.AddItem cov.Bezeichnung
    If TrV.cover = cov.Index Then cover.text = cov.Bezeichnung
  Loop Until EOF(ff)
End If
Close #ff
If cover.text <> Lang(119) Then bearbeiten.Enabled = True

' TrV zwischenspeichern, damit beim OK festgestellt
' werden kann, ob der Benutzer etwas geändert hat
oldTrV = TrV

'Felder einrichten
nummer.left = text(0).left + text(0).Width + 2 * tpx

Show
e(0).text = Format$(TrV.Seiten)
e(1).text = Format$(TrV.laenge, "##0.00")
GetTraegerTitel TrV.Bezeichnung, TrTi
e(2).text = Trim$(TrTi.Titel1)
e(3).text = Trim$(TrTi.Titel2)
e(4).text = Format$(TrV.Label)
e(5).text = GetTab("label", TrV.Label)
e(6).text = Format$(TrV.Kaufquelle)
e(7).text = GetTab("kauf", TrV.Kaufquelle)
e(8).text = Long2Dat$(TrV.Kaufdatum)
e(9).text = Format$(TrV.Preis, "#,##0.00")

If TrV.coverbild = 0 Then
  'coverbild.Picture = eingabe.cover.Picture
  e(10).text = ""
Else
  e(10).text = GetFile("bildfile.dir", TrV.coverbild)
  coverbild.Picture = LoadPicture(Trim$(e(10).text))
End If
coversuch.Picture = gfx.folder1.Picture
d = Day(Now)
Kaufdatum.Picture = gfx.Kalender.GraphicCell(d)
eingabe.Enabled = False
End Sub


Private Sub Form_LostFocus()
Unload traegerstamm
End Sub

Private Sub Form_Unload(Cancel As Integer)
Dim rec As Rect
openini AppData + "winmusik2.ini"
selectTopic "Fenster"
rec.top = traegerstamm.top: rec.left = traegerstamm.left
rec.right = traegerstamm.Width: rec.bottom = traegerstamm.Height
a$ = Rect2String(rec)
putini "Traegerstamm=" + a$
closeini
traegerstammvisible = False
DoEvents
If Traegerloaded = False Then
  EingabePos = -3
End If
eingabe.Enabled = True
SetEingabePosition
eingabe.fillHeader
DoEvents
End Sub


Private Sub ok_Click()
If Trim$(e(10).text) = "" Then
  If TrV.coverbild > 0 Then PutFile "bildfile.dir", TrV.coverbild, ""
  'TrV.CoverBild = 0
Else
  If TrV.coverbild = 0 Then TrV.coverbild = GetNextFreeFile("bildfile.dir")
  PutFile "bildfile.dir", TrV.coverbild, Trim$(e(10).text)
End If
TrV.InfoByte = TrV.InfoByte Or 1
TrV.Kaufquelle = Val(e(6).text)
TrV.Label = Val(e(4).text)
TrV.laenge = CSng(e(1).text)
TrV.Seiten = Val(e(0).text)
If Trim$(e(2).text) <> "" Or Trim$(e(3).text) <> "" Then
  TrTi.Titel1 = Trim$(e(2).text)
  TrTi.Titel2 = Trim$(e(3).text)
  TrV.Bezeichnung = GetNextFreeTraegerTitel
  PutTraegerTitel TrV.Bezeichnung, TrTi
Else
  TrTi.Titel1 = ""
  TrTi.Titel2 = ""
  If TrV.Bezeichnung > 0 Then PutTraegerTitel TrV.Bezeichnung, TrTi
  TrV.Bezeichnung = 0
End If
TrV.Kaufdatum = dat2long&(e(8).text)
TrV.Preis = CSng(e(9).text)
TrV.cover = 0
' Hier muß noch eine Abfrage für das Cover hin!

saverequired = True
Traegerloaded = True
Unload traegerstamm
End Sub



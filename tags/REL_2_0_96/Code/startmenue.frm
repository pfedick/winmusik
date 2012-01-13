VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form leiste1 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "WinMusik"
   ClientHeight    =   555
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6375
   Icon            =   "startmenue.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   555
   ScaleWidth      =   6375
   Begin MSComctlLib.ImageList ToolImages 
      Left            =   540
      Top             =   780
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   25
      ImageHeight     =   25
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   12
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "startmenue.frx":0CFA
            Key             =   ""
            Object.Tag             =   "cassette"
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "startmenue.frx":14B8
            Key             =   ""
            Object.Tag             =   "cd"
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "startmenue.frx":1C76
            Key             =   ""
            Object.Tag             =   "data"
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "startmenue.frx":2434
            Key             =   ""
            Object.Tag             =   "vinyl"
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "startmenue.frx":2BF2
            Key             =   ""
            Object.Tag             =   "video"
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "startmenue.frx":33B0
            Key             =   ""
            Object.Tag             =   "other"
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "startmenue.frx":3B6E
            Key             =   ""
            Object.Tag             =   "mp3"
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "startmenue.frx":432C
            Key             =   ""
            Object.Tag             =   "tape"
         EndProperty
         BeginProperty ListImage9 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "startmenue.frx":4AEA
            Key             =   ""
            Object.Tag             =   "search"
         EndProperty
         BeginProperty ListImage10 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "startmenue.frx":52A8
            Key             =   ""
            Object.Tag             =   "setup"
         EndProperty
         BeginProperty ListImage11 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "startmenue.frx":5A66
            Key             =   ""
            Object.Tag             =   "sql"
         EndProperty
         BeginProperty ListImage12 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "startmenue.frx":6224
            Key             =   ""
            Object.Tag             =   "cover"
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.Toolbar Toolbar1 
      Align           =   1  'Oben ausrichten
      Height          =   495
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   6375
      _ExtentX        =   11245
      _ExtentY        =   873
      ButtonWidth     =   847
      ButtonHeight    =   820
      AllowCustomize  =   0   'False
      Wrappable       =   0   'False
      Appearance      =   1
      Style           =   1
      ImageList       =   "ToolImages"
      _Version        =   393216
      BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
         NumButtons      =   14
         BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Description     =   "Kassette editieren"
            Object.ToolTipText     =   "Kassette editieren"
            ImageIndex      =   1
            Object.Width           =   1e-4
            BeginProperty ButtonMenus {66833FEC-8583-11D1-B16A-00C0F0283628} 
               NumButtonMenus  =   1
               BeginProperty ButtonMenu1 {66833FEE-8583-11D1-B16A-00C0F0283628} 
               EndProperty
            EndProperty
         EndProperty
         BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Description     =   "CD editieren"
            Object.ToolTipText     =   "CD editieren"
            ImageIndex      =   2
         EndProperty
         BeginProperty Button3 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Description     =   "Datenträger editieren"
            Object.ToolTipText     =   "Datenträger editieren"
            ImageIndex      =   3
         EndProperty
         BeginProperty Button4 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Description     =   "Schallplatte editieren"
            Object.ToolTipText     =   "Schallplatte editieren"
            ImageIndex      =   4
         EndProperty
         BeginProperty Button5 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Description     =   "Videokassette editieren"
            Object.ToolTipText     =   "Videokassette editieren"
            ImageIndex      =   5
         EndProperty
         BeginProperty Button6 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Description     =   "Sonstigen Tonträger editieren"
            Object.ToolTipText     =   "Sonstigen Tonträger editieren"
            ImageIndex      =   6
         EndProperty
         BeginProperty Button7 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Description     =   "MP3-Datenträger editieren"
            Object.ToolTipText     =   "MP3-Datenträger editieren"
            ImageIndex      =   7
         EndProperty
         BeginProperty Button8 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Description     =   "Tonband editieren"
            Object.ToolTipText     =   "Tonband editieren"
            ImageIndex      =   8
         EndProperty
         BeginProperty Button9 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Description     =   "Trennzeichen"
            Style           =   3
         EndProperty
         BeginProperty Button10 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Description     =   "Titel suchen"
            Object.ToolTipText     =   "Titel suchen"
            ImageIndex      =   9
         EndProperty
         BeginProperty Button11 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Description     =   "Einstellungen"
            Object.ToolTipText     =   "Einstellungen"
            ImageIndex      =   10
         EndProperty
         BeginProperty Button12 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Description     =   "SQL-Export"
            Object.ToolTipText     =   "SQL-Export"
            ImageIndex      =   11
         EndProperty
         BeginProperty Button13 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Description     =   "Video-CD-Cover drucken"
            Object.ToolTipText     =   "Video-CD-Cover drucken"
            ImageIndex      =   12
         EndProperty
         BeginProperty Button14 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Description     =   "Datenbank-Playlist erstellen"
            Object.ToolTipText     =   "Playlist"
         EndProperty
      EndProperty
   End
End
Attribute VB_Name = "leiste1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub Form_Load()
On Error Resume Next
Dim rec As Rect
openini AppData + "winmusik2.ini"
selectTopic "Fenster"
a$ = GetINI$("Menü1")
If inierror = 0 Then
  String2Rect a$, rec
Else
  rec.left = 0: rec.top = 0
End If
closeini

If left > Screen.Width - Width Then rec.left = Screen.Width - Width
If top > Screen.Height - Height Then rec.top = Screen.Height - Height
left = rec.left: top = rec.top
Caption = Programm + " " + version + "." + Format(build)

For i = 1 To 8
  Toolbar1.Buttons(i).ToolTipText = Lang(299 + i)
Next
Toolbar1.Buttons(10).ToolTipText = Lang(36)
Toolbar1.Buttons(11).ToolTipText = Lang(40)
Toolbar1.Buttons(12).ToolTipText = Lang(237)


End Sub

Private Sub Form_Unload(Cancel As Integer)
Dim rec As Rect
openini AppData + "winmusik2.ini"
selectTopic "Fenster"
rec.top = top: rec.left = left
a$ = Rect2String(rec)
putini "Menü1=" + a$
closeini
If traegerstammvisible = True Then Unload traegerstamm
If eingabevisible = True Then Unload eingabe
'If cdplayervisible = True Then Unload cdplayer
If quicksearchvisible = True Then Unload quicksearch
'If mixervisible = True Then Unload mixer
'If chartsvisible = True Then Unload charts
If vcdcovervisible = True Then Unload vcdcover

Unload CopyrightForm
Unload gfx
CloseHandle (myMutEx)
End
End Sub


Private Sub Toolbar1_ButtonClick(ByVal Button As MSComctlLib.Button)
Dim Index As Integer
Index = Button.Index
If copyrightvisible = True Then Unload CopyrightForm
If Index <= 8 Then
  If Index = traeger And eingabevisible = True Then
    Unload eingabe
  Else
    If eingabevisible = True Then Unload eingabe
    traeger = Index
    eingabe.Show
  End If
ElseIf Index = 10 Then
  If quicksearchvisible = False Then
    quicksearch.Show
  Else
    Unload quicksearch
  End If
ElseIf Index = 31 Then
  If cdplayervisible = False Then
    'cdplayer.Show
    
  Else
    'Unload cdplayer
    
  End If
ElseIf Index = 11 Then
   leiste1.MousePointer = 11
   If eingabevisible = True Then Unload eingabe
   'start2.Show
   property1.Show
   'Unload leiste1
   leiste1.MousePointer = 0
ElseIf Index = 12 Then
    If sqlexportvisible = True Then
        Unload sqlexport
    Else
        sqlexport.Show
    End If
ElseIf Index = 13 Then
    If vcdcovervisible = True Then
        Unload vcdcover
    Else
        vcdcover.Show
    End If
ElseIf Index = 14 Then
    cdread.Show
End If

End Sub



VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form sqlexport 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "MySQL-Export"
   ClientHeight    =   4320
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4560
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4320
   ScaleWidth      =   4560
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows-Standard
   Begin MSComctlLib.ProgressBar progress 
      Height          =   315
      Left            =   60
      TabIndex        =   21
      Top             =   3960
      Width           =   4395
      _ExtentX        =   7752
      _ExtentY        =   556
      _Version        =   393216
      Appearance      =   1
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Stop"
      Enabled         =   0   'False
      Height          =   435
      Left            =   3360
      TabIndex        =   18
      Top             =   3060
      Width           =   915
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Start"
      Default         =   -1  'True
      Height          =   435
      Left            =   2040
      TabIndex        =   17
      Top             =   3060
      Width           =   1095
   End
   Begin VB.CommandButton durchsuchen 
      Height          =   315
      Index           =   0
      Left            =   4200
      Picture         =   "sqlexport.frx":0000
      Style           =   1  'Grafisch
      TabIndex        =   16
      Top             =   3600
      Width           =   315
   End
   Begin MSComDlg.CommonDialog dialog 
      Left            =   2160
      Top             =   2460
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.TextBox datei 
      Height          =   315
      Left            =   840
      TabIndex        =   15
      Text            =   "winmusik.sql"
      Top             =   3600
      Width           =   3315
   End
   Begin VB.Frame Frame2 
      Caption         =   "Art des Exports"
      Height          =   1995
      Left            =   2100
      TabIndex        =   8
      Top             =   300
      Width           =   2355
      Begin VB.OptionButton art 
         Caption         =   "Komplettexport als Replace-Strings"
         Height          =   495
         Index           =   2
         Left            =   120
         TabIndex        =   20
         Top             =   720
         Width           =   1755
      End
      Begin VB.TextBox auhr 
         Height          =   285
         Left            =   1440
         TabIndex        =   12
         Text            =   "0:00"
         Top             =   1560
         Visible         =   0   'False
         Width           =   555
      End
      Begin VB.TextBox adatum 
         Height          =   315
         Left            =   360
         TabIndex        =   11
         Text            =   "dd.mm.jjjj"
         Top             =   1560
         Visible         =   0   'False
         Width           =   975
      End
      Begin VB.OptionButton art 
         Caption         =   "Änderungen seit:"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   10
         Top             =   1260
         Visible         =   0   'False
         Width           =   1755
      End
      Begin VB.OptionButton art 
         Caption         =   "Komplettexport als Insert-Strings"
         Height          =   495
         Index           =   0
         Left            =   120
         TabIndex        =   9
         Top             =   300
         Value           =   -1  'True
         Width           =   1755
      End
      Begin VB.Label Label1 
         AutoSize        =   -1  'True
         Caption         =   "Uhr"
         Height          =   195
         Left            =   2040
         TabIndex        =   13
         Top             =   1620
         Visible         =   0   'False
         Width           =   255
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Tonträger"
      Height          =   3195
      Left            =   120
      TabIndex        =   0
      Top             =   300
      Width           =   1875
      Begin VB.CheckBox tontraeger 
         Caption         =   "Check1"
         Height          =   375
         Index           =   7
         Left            =   120
         TabIndex        =   19
         Top             =   2700
         Width           =   1695
      End
      Begin VB.CheckBox tontraeger 
         Caption         =   "Check1"
         Height          =   375
         Index           =   6
         Left            =   120
         TabIndex        =   7
         Top             =   2340
         Width           =   1695
      End
      Begin VB.CheckBox tontraeger 
         Caption         =   "Check1"
         Height          =   375
         Index           =   5
         Left            =   120
         TabIndex        =   6
         Top             =   1980
         Width           =   1695
      End
      Begin VB.CheckBox tontraeger 
         Caption         =   "Check1"
         Height          =   375
         Index           =   4
         Left            =   120
         TabIndex        =   5
         Top             =   1620
         Width           =   1695
      End
      Begin VB.CheckBox tontraeger 
         Caption         =   "Check1"
         Height          =   375
         Index           =   3
         Left            =   120
         TabIndex        =   4
         Top             =   1260
         Width           =   1635
      End
      Begin VB.CheckBox tontraeger 
         Caption         =   "Check1"
         Height          =   375
         Index           =   2
         Left            =   120
         TabIndex        =   3
         Top             =   900
         Width           =   1635
      End
      Begin VB.CheckBox tontraeger 
         Caption         =   "Check1"
         Height          =   375
         Index           =   1
         Left            =   120
         TabIndex        =   2
         Top             =   540
         Width           =   1635
      End
      Begin VB.CheckBox tontraeger 
         Caption         =   "Check1"
         Height          =   375
         Index           =   0
         Left            =   120
         TabIndex        =   1
         Top             =   180
         Width           =   1695
      End
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "Zieldatei:"
      Height          =   195
      Left            =   120
      TabIndex        =   14
      Top             =   3660
      Width           =   645
   End
End
Attribute VB_Name = "sqlexport"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
DefInt A-Z

Dim exportisrunning As Boolean

Sub ExportTraeger(sqlmethod As String, out As Integer)


'Print #out, sqlmethod + "Test"


End Sub

Function sqlfilter(text As String) As String
sqlfilter = ereg_replace("'", "\'", text)
End Function

Private Sub Command1_Click()
Dim nr As Long, zaehler As Long
Dim abk As Abkuerzungen
Dim sqlmethod As String
On Error Resume Next

If art(0).Value = True Then
    sqlmethod = "insert"
Else
    sqlmethod = "replace"
End If

out = FreeFile
Err = 0
Open datei For Output As #out
If Err <> 0 Then
  MsgBox "Datei " + datei + " konnte nicht geöffnet werden!", 48, "Fehler"
  Exit Sub
End If
exportisrunning = True
Command2.Enabled = True
Command1.Enabled = False

ff = FreeFile
Open TiDatei For Random As #ff Len = Len(Ti)
nr = LOF(ff) / Len(Ti)
progress.max = nr
progress.Value = 0
DoEvents
If art(0).Value = True Then ' Tabelle muss erstellt werden
  Print #out, "# Created by " + Programm + " " + version + "." + build
  Print #out, "#"
  Print #out, "create table wm_titel ("
  Print #out, "  nummer             int4 not null primary key,"
  Print #out, "  interpret          char(80) not null,"
  Print #out, "  titel              char(80) not null,"
  Print #out, "  versionint         int4,"
  Print #out, "  version            text,"
  Print #out, "  musikartint        int4 not null,"
  Print #out, "  musikart           text,"
  Print #out, "  laenge             float,"
  Print #out, "  erscheinungsjahr   int4,"
  Print #out, "  aufnahmedatum      int4,"
  Print #out, "  bemerkung          text,"
  Print #out, "  traeger            tinyint not null,"
  Print #out, "  tindex             int4 not null,"
  Print #out, "  seite              tinyint,"
  Print #out, "  track              smallint,"
  Print #out, "  index (nummer),"
  Print #out, "  index (interpret),"
  Print #out, "  index (titel),"
  Print #out, "  index (traeger),"
  Print #out, "  index (tindex),"
  Print #out, "  index (musikartint)"
  Print #out, ");"
  Print #out, ""
End If

For zaehler = 1 To nr
  Get #ff, zaehler, Ti
  progress.Value = zaehler
  If zaehler Mod 100 = 0 Then DoEvents
  If exportisrunning = False Then Exit For
  
  If Ti.traeger > 0 And Ti.traeger < 9 And Ti.track > 0 Then
    If tontraeger(Ti.traeger - 1).Value = 1 Then
      If art(0).Value = True Or art(2).Value = True Then
         'Alles exportieren
         
         Print #out, sqlmethod + " into wm_titel values(";
         
         Print #out, "'" + Format(Ti.nummer) + "',";
         Print #out, "'" + sqlfilter(Trim(Ti.interpret)) + "',";
         Print #out, "'" + sqlfilter(Trim(Ti.titel)) + "',"
         Print #out, "'" + Format(Ti.version) + "',";
         Print #out, "'" + sqlfilter(Trim(GetTab("version.bez", Ti.version))) + "',";
         Print #out, "'" + Format(Ti.musikart) + "',";
         Print #out, "'" + sqlfilter(Trim(GetTab("musikart.bez", Ti.musikart))) + "',"
         Print #out, "'" + Komma2Punkt(Format(Ti.laenge, "#.00")) + "',";
         Print #out, "'" + Format(Ti.Erscheinungsjahr) + "',";
         Print #out, "'" + Format(Ti.Aufnahmedatum) + "',";
         Print #out, "'";
         If bemerkung > 0 Then
         
         End If
         Print #out, "',";
         Print #out, "'" + Format(Ti.traeger) + "','" + Format(Ti.Index) + "',"
         Print #out, "'" + Format(Ti.Seite) + "',";
         Print #out, "'" + Format(Ti.track) + "');"
         
      Else
      End If
    End If
  End If
Next

Close #ff

Print #out, "";
Print #out, "create table cdheaderkennung ("
Print #out, "   traegerart      tinyint not null default 0,"
Print #out, "   traegerindex    int4 not null default 0,"
Print #out, "   seite           tinyint not null default 1,"
Print #out, "   tracks          int4 not null default 0,"
Print #out, "   gesamtlaenge    int4 not null default 0,"
Print #out, "   startposition   int4 not null default 0,"
Print #out, "   startpos0       int4 not null default 0,"
Print #out, "   startpos1       int4 not null default 0,"
Print #out, "   startpos2       int4 not null default 0,"
Print #out, "   startpos3       int4 not null default 0,"
Print #out, "   startpos4       int4 not null default 0,"
Print #out, "   startpos5       int4 not null default 0,"
Print #out, "   startpos6       int4 not null default 0,"
Print #out, "   startpos7       int4 not null default 0,"
Print #out, "   startpos8       int4 not null default 0,"
Print #out, "   startpos9       int4 not null default 0"
Print #out, ");"
Print #out, ""

Print #out, "create table traegerverzeichnis ("
Print #out, "   infobyte        int4 not null default 0,"
Print #out, "   seiten          tinyint not null default 0,"
Print #out, "   laenge          float not null default 0,"
Print #out, "   rest            float not null default 0,"
Print #out, "   titel1          char(80) default '',"
Print #out, "   titel2          char(80) default '',"
Print #out, "   label           int4 not null default 0,"
Print #out, "   kaufquelle      int4 not null default 0,"
Print #out, "   kaufdatum       int4 not null default 0,"
Print #out, "   preis           int4 not null default 0,"
Print #out, "   erstelldatum    int4 not null default 0,"
Print #out, "   aufnahmestart   int4 not null default 0,"
Print #out, "   aufnahmeende    int4 not null default 0,"
Print #out, "   tracks          int4 not null default 0,"
Print #out, "   singles         int4 not null default 0,"
Print #out, "   mixe            int4 not null default 0,"
Print #out, "   cover           int4 not null default 0,"
Print #out, "   coverbild       int4 not null default 0"
Print #out, ");"
Print #out, ""

Print #out, "create table traegerdatensatz ("
Print #out, "   traegerindex    int4 not null default 0,"
Print #out, "   titelnummer     int4 not null default 0,"
Print #out, "   seite           int4 not null default 0,"
Print #out, "   track           int4 not null default 0"
Print #out, ");"
Print #out, ""

ExportTraeger sqlmethod, out


Print #out, "create table abkuerzungen ("
Print #out, "   abk             char(10) not null default '',"
Print #out, "   interpret       char(80) not null default ''"
Print #out, ");"
Print #out, ""
ff = FreeFile
Open DatenPfad + "\abk.dat" For Random As #ff Len = Len(abk)
high& = LOF(ff) / Len(abk)
For i& = high& To 1 Step -1
  Get #ff, i&, abk
  Print #out, sqlmethod + " into abkuerzungen values(";
  Print #out, "'" + sqlfilter(Trim(abk.abk)) + "','" + sqlfilter(Trim(abk.interpret)) + "');"
Next
Close #ff



Close #out
exportisrunning = False
progress.Value = 0
Command2.Enabled = False
Command1.Enabled = True

End Sub

Private Sub Command2_Click()
exportisrunning = False
End Sub


Private Sub durchsuchen_Click(Index As Integer)
dialog.DialogTitle = "Datei speichern"
dialog.DefaultExt = "sql"
dialog.filename = datei.text
dialog.flags = &H4 Or &H200000
dialog.Action = 2
'dialog.ShowSave
datei.text = dialog.filename


End Sub


Private Sub Form_Load()
sqlexportvisible = True
Frame1.Caption = Lang(87)
tontraeger(0).Caption = Lang(103)
tontraeger(1).Caption = Lang(104)
tontraeger(2).Caption = Lang(105)
tontraeger(3).Caption = Lang(106)
tontraeger(4).Caption = Lang(107)
tontraeger(5).Caption = Lang(108)
tontraeger(6).Caption = Lang(356)
tontraeger(7).Caption = Lang(357)
progress.min = 0
progress.max = 100
progress.Value = 0
datei.text = tmp + "\winmusik.sql"

End Sub

Private Sub Form_Unload(Cancel As Integer)
sqlexportvisible = False
DoEvents
End Sub



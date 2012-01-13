VERSION 5.00
Begin VB.Form start1 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "First Start of WinMusik"
   ClientHeight    =   3705
   ClientLeft      =   510
   ClientTop       =   2595
   ClientWidth     =   7500
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
   Icon            =   "START1.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   3705
   ScaleWidth      =   7500
   ShowInTaskbar   =   0   'False
   Begin VB.Frame Frame1 
      Height          =   2775
      Left            =   4440
      TabIndex        =   2
      Top             =   840
      Width           =   2895
      Begin VB.OptionButton langa 
         Caption         =   "Option1"
         Height          =   285
         Index           =   0
         Left            =   120
         TabIndex        =   9
         Top             =   240
         Value           =   -1  'True
         Width           =   2340
      End
      Begin VB.OptionButton langa 
         Caption         =   "Option1"
         Height          =   285
         Index           =   1
         Left            =   165
         TabIndex        =   8
         Top             =   540
         Width           =   2340
      End
      Begin VB.OptionButton langa 
         Caption         =   "Option1"
         Height          =   285
         Index           =   2
         Left            =   225
         TabIndex        =   7
         Top             =   855
         Width           =   2340
      End
      Begin VB.OptionButton langa 
         Caption         =   "Option1"
         Height          =   285
         Index           =   3
         Left            =   195
         TabIndex        =   6
         Top             =   1140
         Width           =   2340
      End
      Begin VB.OptionButton langa 
         Caption         =   "Option1"
         Height          =   285
         Index           =   4
         Left            =   240
         TabIndex        =   5
         Top             =   1440
         Width           =   2340
      End
      Begin VB.OptionButton langa 
         Caption         =   "Option1"
         Height          =   285
         Index           =   5
         Left            =   165
         TabIndex        =   4
         Top             =   1770
         Width           =   2340
      End
      Begin VB.CommandButton ok 
         Caption         =   "OK"
         Height          =   375
         Left            =   1080
         TabIndex        =   3
         Top             =   2160
         Width           =   855
      End
   End
   Begin VB.PictureBox Picture1 
      Height          =   3465
      Left            =   120
      Picture         =   "START1.frx":0CFA
      ScaleHeight     =   3405
      ScaleWidth      =   4200
      TabIndex        =   1
      Top             =   120
      Width           =   4260
   End
   Begin VB.Label Welcome 
      Appearance      =   0  '2D
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Please choose your language:"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   13.5
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   765
      Left            =   4440
      TabIndex        =   0
      Top             =   0
      Width           =   2895
   End
End
Attribute VB_Name = "start1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
DefInt A-Z

Private Sub Command1_Click()

End Sub

Private Sub Form_Load()
Dim tmpstr As String * 256
On Error Resume Next
start1.Caption = Programm + " " + version
start1.left = (Screen.Width - start1.Width) / 2
start1.top = (Screen.Height - start1.Height) / 2
For i = 0 To 5
  langa(i).top = 200 + i * 300
  langa(i).left = 200
  langa(i).Visible = False
Next
openini "win.ini"
selectTopic "intl"
laendercode = Val(GetINI$("iCountry"))
closeini
If InStr("49,43,41", Format$(laendercode)) > 0 Then
  HelpFile = "wm20-de.hlp"
Else
  HelpFile = "wm20-en.hlp"
End If
i = 0
f$ = Dir$("*.lng"): pa$ = ""
If f$ = "" Then
  f$ = Dir$(App.Path + "\*.lng")
  If f$ = "" Then
    MsgBox "Sprachdatei nicht gefunden!" + Chr$(13) + Chr$(13) + "Language-File not found!", 16, Programm + " " + version, HelpFile, 100
    End
  End If
  pa$ = App.Path + "\"
End If
If HauptPfad = "" Then HauptPfad = App.Path
Do
  ff = FreeFile
  Open pa$ + f$ For Binary As #ff
  If Err = 0 Then
    Get #ff, 1, pfp
    If pfp.pfpID = "PFP-File" And pfp.FileID = "LANG" Then
      Get #ff, 47, p
      Get #ff, p + 1, tmpstr
      n = InStr(tmpstr, Chr$(0))
      langa(i).Caption = left$(tmpstr, n - 1)
      langa(i).Visible = True
      langa(i).tag = f$
      Get #ff, 53, p
      Get #ff, p + 1, tmpstr
      n = InStr(tmpstr, Chr$(0))
      a$ = left$(tmpstr, n - 1)
      If InStr(a$, Format$(laendercode)) > 0 Then
        langa(i).Value = True
        INITLanguage langa(i).tag
        start1.Welcome.Caption = Lang(7)
      End If
      i = i + 1
    End If
  Else
    MsgBox "Sprachdatei nicht gefunden!" + Chr$(13) + Chr$(13) + "Language-File not found!", 16, Programm + " " + version, HelpFile, 101
    End
  End If
  Close #ff
  f$ = Dir$
Loop Until f$ = ""
End Sub



Private Sub ok_Click()
For i = 0 To 5
  If langa(i).Value = True Then f$ = langa(i).tag
Next
INITLanguage f$
Unload start1
property1.Show
End Sub


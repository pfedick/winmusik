VERSION 5.00
Object = "{0BA686C6-F7D3-101A-993E-0000C0EF6F5E}#1.0#0"; "THREED32.OCX"
Begin VB.Form menu1 
   BorderStyle     =   1  'Fest Einfach
   Caption         =   "WinMusik 2.0"
   ClientHeight    =   735
   ClientLeft      =   8280
   ClientTop       =   1425
   ClientWidth     =   4335
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
   Icon            =   "MENU1.frx":0000
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   735
   ScaleWidth      =   4335
   Begin Threed.SSPanel Quickhelp 
      Height          =   270
      Left            =   0
      TabIndex        =   10
      Top             =   420
      Width           =   4335
      _Version        =   65536
      _ExtentX        =   7646
      _ExtentY        =   476
      _StockProps     =   15
      BackColor       =   12632256
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BevelWidth      =   2
      BevelOuter      =   1
      Alignment       =   1
   End
   Begin Threed.SSCommand Auswahl 
      Height          =   375
      Index           =   10
      Left            =   3960
      TabIndex        =   11
      Top             =   0
      Width           =   375
      _Version        =   65536
      _ExtentX        =   661
      _ExtentY        =   661
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
      RoundedCorners  =   0   'False
      Outline         =   0   'False
      Picture         =   "MENU1.frx":030A
   End
   Begin Threed.SSCommand Auswahl 
      Height          =   375
      Index           =   9
      Left            =   3600
      TabIndex        =   9
      Top             =   0
      Width           =   375
      _Version        =   65536
      _ExtentX        =   661
      _ExtentY        =   661
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
      RoundedCorners  =   0   'False
      Outline         =   0   'False
      Picture         =   "MENU1.frx":0326
   End
   Begin Threed.SSCommand Auswahl 
      Height          =   375
      Index           =   8
      Left            =   3240
      TabIndex        =   8
      Top             =   0
      Width           =   375
      _Version        =   65536
      _ExtentX        =   661
      _ExtentY        =   661
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
      RoundedCorners  =   0   'False
      Outline         =   0   'False
      Picture         =   "MENU1.frx":0342
   End
   Begin Threed.SSCommand Auswahl 
      Height          =   375
      Index           =   7
      Left            =   2880
      TabIndex        =   7
      Top             =   0
      Width           =   375
      _Version        =   65536
      _ExtentX        =   661
      _ExtentY        =   661
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
      RoundedCorners  =   0   'False
      Outline         =   0   'False
      Picture         =   "MENU1.frx":035E
   End
   Begin Threed.SSCommand Auswahl 
      Height          =   375
      Index           =   6
      Left            =   2520
      TabIndex        =   6
      Top             =   0
      Width           =   375
      _Version        =   65536
      _ExtentX        =   661
      _ExtentY        =   661
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
      RoundedCorners  =   0   'False
      Outline         =   0   'False
      Picture         =   "MENU1.frx":037A
   End
   Begin Threed.SSCommand Auswahl 
      Height          =   375
      Index           =   5
      Left            =   1800
      TabIndex        =   5
      Top             =   0
      Width           =   375
      _Version        =   65536
      _ExtentX        =   661
      _ExtentY        =   661
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
      RoundedCorners  =   0   'False
      Outline         =   0   'False
      Picture         =   "MENU1.frx":0396
   End
   Begin Threed.SSCommand Auswahl 
      Height          =   375
      Index           =   4
      Left            =   1440
      TabIndex        =   4
      Top             =   0
      Width           =   375
      _Version        =   65536
      _ExtentX        =   661
      _ExtentY        =   661
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
      RoundedCorners  =   0   'False
      Outline         =   0   'False
      Picture         =   "MENU1.frx":03B2
   End
   Begin Threed.SSCommand Auswahl 
      Height          =   375
      Index           =   3
      Left            =   1080
      TabIndex        =   3
      Top             =   0
      Width           =   375
      _Version        =   65536
      _ExtentX        =   661
      _ExtentY        =   661
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
      RoundedCorners  =   0   'False
      Outline         =   0   'False
      Picture         =   "MENU1.frx":03CE
   End
   Begin Threed.SSCommand Auswahl 
      Height          =   375
      Index           =   2
      Left            =   720
      TabIndex        =   2
      Top             =   0
      Width           =   375
      _Version        =   65536
      _ExtentX        =   661
      _ExtentY        =   661
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
      RoundedCorners  =   0   'False
      Outline         =   0   'False
      Picture         =   "MENU1.frx":03EA
   End
   Begin Threed.SSCommand Auswahl 
      Height          =   375
      Index           =   1
      Left            =   360
      TabIndex        =   1
      Top             =   0
      Width           =   375
      _Version        =   65536
      _ExtentX        =   661
      _ExtentY        =   661
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
      RoundedCorners  =   0   'False
      Outline         =   0   'False
      Picture         =   "MENU1.frx":0406
   End
   Begin Threed.SSCommand Auswahl 
      Height          =   375
      Index           =   0
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   375
      _Version        =   65536
      _ExtentX        =   661
      _ExtentY        =   661
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
      RoundedCorners  =   0   'False
      Outline         =   0   'False
      Picture         =   "MENU1.frx":0422
   End
   Begin VB.Image SelectButton 
      Height          =   135
      Index           =   2
      Left            =   2370
      Picture         =   "MENU1.frx":043E
      Top             =   90
      Width           =   90
   End
   Begin VB.Image SelectButton 
      Height          =   135
      Index           =   1
      Left            =   2205
      Picture         =   "MENU1.frx":04E4
      Top             =   90
      Width           =   90
   End
   Begin VB.Image SelectButton 
      Height          =   90
      Index           =   0
      Left            =   2280
      Picture         =   "MENU1.frx":058A
      Top             =   240
      Width           =   135
   End
End
Attribute VB_Name = "menu1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Auswahl_Click(Index As Integer)
If copyrightvisible = True Then Unload CopyrightForm
If Index <= 5 Then
  If Index + 1 = traeger And eingabevisible = True Then
    Unload Eingabe
  Else
    If eingabevisible = True Then Unload Eingabe
    traeger = Index + 1
    Eingabe.Show
  End If
ElseIf Index = 10 Then
   If CompileVersion = 2.1 Then
     leiste1.MousePointer = 11
   Else
     menu1.MousePointer = 11
   End If
   If eingabevisible = True Then Unload Eingabe
   start2.Show
   If CompileVersion = 2.1 Then
     Unload leiste1
     leiste1.MousePointer = 0
   Else
     Unload menu1
     menu1.MousePointer = 0
   End If
ElseIf Index = 8 Then
  If cdplayervisible = False Then
    cdplayer.Show
  Else
    Unload cdplayer
  End If
ElseIf Index = 9 Then
  If mixervisible = False Then
    mixer.Show
  Else
    Unload mixer
  End If
ElseIf Index = 7 Then
  If chartsvisible = False Then
    charts.Show
  Else
    Unload charts
  End If
ElseIf Index = 6 Then
  If quicksearchvisible = False Then
    quicksearch.Show
  Else
    Unload quicksearch
  End If
End If
End Sub

Private Sub Auswahl_MouseMove(Index As Integer, Button As Integer, Shift As Integer, X As Single, Y As Single)
Static lastx, lasty
If lastx <> X Or lasty <> Y Then
  lastx = X: lasty = Y
  Quickhelp.Caption = Lang(30 + Index)
End If
End Sub

Private Sub Form_Load()
Dim rec As Rect
openini "wmusik20"
selectTopic "Fenster"
a$ = GetINI$("Menü1")
If inierror = 0 Then
  String2Rect a$, rec
Else
  rec.left = 0: rec.top = 0
End If
If rec.left > Screen.Width - menu1.Width Then rec.left = Screen.Width - menu1.Width
If rec.top > Screen.Height - menu1.Height Then rec.top = Screen.Height - menu1.Height
menu1.left = rec.left: menu1.top = rec.top
menu1.Height = 750
If ShowHitparade = False Then Auswahl(7).Visible = False
If ShowMixer = False Then Auswahl(9).Visible = False

Show
DoEvents
For i = 0 To 10
  Auswahl(i).Picture = gfx.but(i).Picture
Next
'WindowStayOnTop menu1.hwnd, False

End Sub

Private Sub Form_Paint()
WindowStayOnTop menu1.hwnd, True

End Sub

Private Sub Form_Unload(Cancel As Integer)
Dim rec As Rect
openini "wmusik20"
selectTopic "Fenster"
rec.top = menu1.top: rec.left = menu1.left
a$ = Rect2String(rec)
putini "Menü1=" + a$
closeini
If traegerstammvisible = True Then Unload traegerstamm
If eingabevisible = True Then Unload Eingabe
If cdplayervisible = True Then Unload cdplayer
If quicksearchvisible = True Then Unload quicksearch
If mixervisible = True Then Unload mixer
If chartsvisible = True Then Unload charts
Unload CopyrightForm
Unload gfx
End Sub


Private Sub SelectButton_Click(Index As Integer)
Select Case Index
  Case 0:
    If ShowHelp = True Then
      ShowHelp = False
      menu1.Height = 750
    Else
      ShowHelp = True
      menu1.Height = 1075
    End If
  Case 2:
    If menu1.Width = 2575 Then
       Quickhelp.Width = 4335
       menu1.Width = 4440
    Else
      Quickhelp.Width = 2470
      menu1.Width = 2575
    End If
End Select
End Sub


Private Sub SelectButton_MouseDown(Index As Integer, Button As Integer, Shift As Integer, X As Single, Y As Single)
Select Case Index
  Case 0: SelectButton(Index).Picture = gfx.kpu1.Picture
  Case 1: SelectButton(Index).Picture = gfx.kpl1.Picture
  Case 2: SelectButton(Index).Picture = gfx.kpr1.Picture
End Select
End Sub


Private Sub SelectButton_MouseMove(Index As Integer, Button As Integer, Shift As Integer, X As Single, Y As Single)
Static lastx, lasty
If lastx <> X Or lasty <> Y Then
  lastx = X: lasty = Y
  Quickhelp.Caption = Lang(155 + Index)
End If
End Sub

Private Sub SelectButton_MouseUp(Index As Integer, Button As Integer, Shift As Integer, X As Single, Y As Single)
Select Case Index
  Case 0: SelectButton(Index).Picture = gfx.kpu0.Picture
  Case 1: SelectButton(Index).Picture = gfx.kpl0.Picture
  Case 2: SelectButton(Index).Picture = gfx.kpr0.Picture
End Select

End Sub



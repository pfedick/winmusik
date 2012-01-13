VERSION 4.00
Begin VB.Form Form1 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "WinMusik 2.0 Update"
   ClientHeight    =   4545
   ClientLeft      =   1470
   ClientTop       =   1440
   ClientWidth     =   7005
   BeginProperty Font 
      name            =   "MS Sans Serif"
      charset         =   0
      weight          =   700
      size            =   8.25
      underline       =   0   'False
      italic          =   0   'False
      strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H80000008&
   Height          =   4950
   Icon            =   "UPDATE.frx":0000
   Left            =   1410
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4545
   ScaleWidth      =   7005
   Top             =   1095
   Width           =   7125
   Begin VB.Frame Frame1 
      Height          =   2415
      Left            =   2580
      TabIndex        =   3
      Top             =   1500
      Width           =   4395
      Begin VB.TextBox wm1 
         Height          =   375
         Left            =   60
         TabIndex        =   10
         Text            =   "Text1"
         Top             =   480
         Width           =   3795
      End
      Begin VB.CommandButton Command1 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         Caption         =   "Abbrechen"
         Height          =   435
         Left            =   300
         TabIndex        =   5
         Top             =   1815
         Width           =   1230
      End
      Begin VB.CommandButton Start 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         Caption         =   "Weiter"
         Default         =   -1  'True
         Height          =   435
         Left            =   2760
         TabIndex        =   4
         Top             =   1815
         Width           =   1290
      End
      Begin Threed.SSPanel NewPath 
         Height          =   420
         Left            =   30
         TabIndex        =   6
         Top             =   1230
         Width           =   4305
         _Version        =   65536
         _ExtentX        =   7594
         _ExtentY        =   741
         _StockProps     =   15
         ForeColor       =   0
         BackColor       =   -2147483633
         BevelOuter      =   0
         BevelInner      =   1
         Alignment       =   1
      End
      Begin Threed.SSCommand Command3D1 
         Height          =   345
         Left            =   3900
         TabIndex        =   9
         Top             =   495
         Width           =   390
         _Version        =   65536
         _ExtentX        =   688
         _ExtentY        =   609
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "UPDATE.frx":030A
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Pfad von WinMusik 1.0:"
         ForeColor       =   &H80000008&
         Height          =   195
         Left            =   45
         TabIndex        =   8
         Top             =   180
         Width           =   2055
      End
      Begin VB.Label Label3 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Datenpfad von WinMusik 2.0:"
         ForeColor       =   &H80000008&
         Height          =   195
         Left            =   60
         TabIndex        =   7
         Top             =   1020
         Width           =   2550
      End
   End
   Begin Threed.SSPanel Queue 
      Height          =   330
      Left            =   2580
      TabIndex        =   0
      Top             =   4050
      Width           =   4380
      _Version        =   65536
      _ExtentX        =   7726
      _ExtentY        =   582
      _StockProps     =   15
      Caption         =   "0 %"
      ForeColor       =   0
      BackColor       =   -2147483633
      BevelOuter      =   1
      FloodType       =   1
      FloodColor      =   65535
      Font3D          =   4
   End
   Begin VB.Label meldung 
      BackColor       =   &H00FFFFFF&
      BorderStyle     =   1  'Fixed Single
      Height          =   2295
      Left            =   2580
      TabIndex        =   11
      Top             =   1620
      Visible         =   0   'False
      Width           =   4335
   End
   Begin MSComDlg.CommonDialog Dialog 
      Left            =   5100
      Top             =   4440
      _Version        =   65536
      _ExtentX        =   847
      _ExtentY        =   847
      _StockProps     =   0
   End
   Begin VB.Image Image1 
      BorderStyle     =   1  'Fixed Single
      Height          =   4290
      Left            =   120
      Picture         =   "UPDATE.frx":0404
      Top             =   120
      Width           =   2370
   End
   Begin VB.Label Label4 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      BackStyle       =   0  'Transparent
      Caption         =   "Achtung: Winmusik 2.0 muß installiert sein. Daten, die bereits mit WinMusik 2.0 erstellt wurden, gehen verloren!"
      ForeColor       =   &H000000C0&
      Height          =   675
      Left            =   2700
      TabIndex        =   2
      Top             =   855
      Width           =   4260
   End
   Begin VB.Label Label2 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Benutzen Sie dieses Programm, wenn Sie bereits Daten mit WinMusik 1.0 angelegt haben und nun auf die Version 2.0 umsteigen wollen."
      ForeColor       =   &H80000008&
      Height          =   675
      Left            =   2700
      TabIndex        =   1
      Top             =   135
      Width           =   4245
   End
End
Attribute VB_Name = "Form1"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
Unload Form1
End
End Sub

Private Sub Command3D1_Click()
  Dim i As Integer
  Dialog.filename = Trim$(wm1.text)
  Dialog.Filter = "INI-Dateien (*.ini)|*.ini|Programm-Dateien (*.exe)|*.exe|Alle Dateien (*.*)|*.*"
  Dialog.FilterIndex = 0
  Dialog.DefaultExt = ".ini"
  Dialog.DialogTitle = "Alte Winmusik-Version finden"
  Dialog.Flags = &H4&
  Dialog.ShowOpen
  wm1pfad = Dialog.filename
  For i = Len(wm1pfad) To 1 Step -1
    If Mid$(wm1pfad, i, 1) = "\" Then wm1pfad = left$(wm1pfad, i - 1): Exit For
  Next
  wm1.text = wm1pfad
End Sub

Private Sub Form_Load()
wm1pfad = "C:\WINMUSIK"
openini "wmusik20.ini"
If inierror() > 0 Then
  newpath.Caption = "Nicht installiert!!!"
  DatenPfad = ""
Else
  selectTopic "Pfade"
  DatenPfad = GetINI("Daten")
  newpath.Caption = DatenPfad
End If
closeini
wm1.text = wm1pfad
Form1.left = (Screen.Width - Form1.Width) / 2
Form1.top = (Screen.Height - Form1.Height) / 2
End Sub

Private Sub Form_Resize()
Form1.Width = 7125
Form1.Height = 4950
End Sub

Private Sub Start_Click()
wm1pfad = Trim$(wm1.text)
StartUpdate
End Sub


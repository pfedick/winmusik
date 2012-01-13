VERSION 5.00
Object = "{5E9E78A0-531B-11CF-91F6-C2863C385E30}#1.0#0"; "MSFLXGRD.OCX"
Begin VB.Form mp3import 
   Caption         =   "MP3-Files einlesen"
   ClientHeight    =   4605
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7380
   LinkTopic       =   "Form1"
   ScaleHeight     =   4605
   ScaleWidth      =   7380
   StartUpPosition =   3  'Windows-Standard
   Begin VB.CommandButton Command3 
      Caption         =   "Zurück"
      Height          =   375
      Left            =   3060
      TabIndex        =   8
      Top             =   4140
      Width           =   1155
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Abbrechen"
      Height          =   375
      Left            =   5640
      TabIndex        =   7
      Top             =   4140
      Width           =   1695
   End
   Begin VB.CommandButton Command1 
      Caption         =   "OK"
      Height          =   375
      Left            =   60
      TabIndex        =   6
      Top             =   4140
      Width           =   975
   End
   Begin VB.Frame korrekturframe 
      Caption         =   "Track-Korrektur"
      Height          =   975
      Left            =   60
      TabIndex        =   1
      Top             =   3000
      Width           =   7275
      Begin VB.TextBox titel 
         Height          =   285
         Left            =   960
         TabIndex        =   5
         Top             =   600
         Width           =   6195
      End
      Begin VB.TextBox interpret 
         Height          =   285
         Left            =   960
         TabIndex        =   4
         Top             =   240
         Width           =   6195
      End
      Begin VB.Label Label2 
         AutoSize        =   -1  'True
         Caption         =   "Titel:"
         Height          =   195
         Left            =   120
         TabIndex        =   3
         Top             =   660
         Width           =   345
      End
      Begin VB.Label Label1 
         AutoSize        =   -1  'True
         Caption         =   "Interpret:"
         Height          =   195
         Left            =   120
         TabIndex        =   2
         Top             =   300
         Width           =   630
      End
   End
   Begin MSFlexGridLib.MSFlexGrid grid 
      Height          =   2715
      Left            =   60
      TabIndex        =   0
      Top             =   120
      Width           =   7275
      _ExtentX        =   12832
      _ExtentY        =   4789
      _Version        =   393216
      Rows            =   10
      Cols            =   5
      AllowUserResizing=   1
      BorderStyle     =   0
      Appearance      =   0
   End
End
Attribute VB_Name = "mp3import"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()
eingabe.Enabled = False
grid.Width = Width - 300
grid.Height = Height - 2000
grid.Col = 0: grid.Row = 0: grid.ColWidth(0) = 600: grid.text = "Track"
grid.Col = 1: grid.Row = 0: grid.ColWidth(1) = 2000: grid.text = "Interpret"
grid.Col = 2: grid.Row = 0: grid.ColWidth(2) = 2000: grid.text = "Titel"
grid.Col = 3: grid.Row = 0: grid.ColWidth(3) = 800: grid.text = "Länge"
grid.Col = 4: grid.Row = 0: grid.ColWidth(4) = 800: grid.text = "ID"
End Sub

Private Sub Form_Resize()
If Width < 7500 Then Width = 7500
If Height < 5000 Then Height = 5000
grid.Width = Width - 300
grid.Height = Height - 2000
End Sub

Private Sub Form_Unload(Cancel As Integer)
eingabe.Enabled = True

End Sub


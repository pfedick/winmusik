VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "Mscomctl.ocx"
Begin VB.Form druckmsg 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "Drucken"
   ClientHeight    =   1695
   ClientLeft      =   6915
   ClientTop       =   1890
   ClientWidth     =   3720
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   1695
   ScaleWidth      =   3720
   ShowInTaskbar   =   0   'False
   Begin MSComctlLib.ProgressBar queue 
      Height          =   255
      Left            =   180
      TabIndex        =   2
      Top             =   900
      Width           =   3075
      _ExtentX        =   5424
      _ExtentY        =   450
      _Version        =   393216
      Appearance      =   0
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
      Height          =   300
      Left            =   780
      TabIndex        =   0
      Top             =   1320
      Width           =   2175
   End
   Begin VB.Image Image1 
      Height          =   240
      Left            =   3300
      Picture         =   "druckmsg.frx":0000
      Top             =   900
      Width           =   240
   End
   Begin VB.Label textfeld 
      Alignment       =   2  'Zentriert
      BorderStyle     =   1  'Fest Einfach
      Caption         =   "Es wird gedruckt..."
      Height          =   1185
      Left            =   75
      TabIndex        =   1
      Top             =   45
      Width           =   3555
   End
End
Attribute VB_Name = "druckmsg"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub abbrechen_Click()
abbrechen.tag = "true"
End Sub


Private Sub Form_Load()
druckmsg.left = (Screen.Width - 3840) / 2
druckmsg.top = (Screen.Height - 1740) / 2
druckmsg.abbrechen.Caption = Lang(2)
druckmsg.Caption = Lang(149)
druckmsg.textfeld = Lang(150)
abbrechen.tag = ""
End Sub



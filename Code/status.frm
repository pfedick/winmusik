VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form status 
   BorderStyle     =   1  'Fest Einfach
   Caption         =   "Status"
   ClientHeight    =   1725
   ClientLeft      =   1710
   ClientTop       =   1560
   ClientWidth     =   4425
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   1725
   ScaleWidth      =   4425
   Begin MSComctlLib.ProgressBar queue 
      Height          =   255
      Left            =   60
      TabIndex        =   2
      Top             =   960
      Width           =   4275
      _ExtentX        =   7541
      _ExtentY        =   450
      _Version        =   393216
      Appearance      =   0
   End
   Begin VB.TextBox text 
      Height          =   795
      Left            =   60
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertikal
      TabIndex        =   1
      Text            =   "status.frx":0000
      Top             =   60
      Width           =   4275
   End
   Begin VB.CommandButton Abbrechen 
      Caption         =   "Abbrechen"
      Height          =   315
      Left            =   1440
      TabIndex        =   0
      Top             =   1320
      Width           =   1335
   End
End
Attribute VB_Name = "status"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub abbrechen_Click()
If abbrechen.tag = "bye" Then Unload status
abbrechen.tag = "stopit"
End Sub

Private Sub Form_Load()
abbrechen.Caption = Lang(2)
abbrechen.tag = ""
End Sub



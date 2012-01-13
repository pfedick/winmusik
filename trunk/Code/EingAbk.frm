VERSION 5.00
Begin VB.Form EingabeAbkuerzung 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "Eingabe Abkürzung"
   ClientHeight    =   1380
   ClientLeft      =   1980
   ClientTop       =   3765
   ClientWidth     =   5010
   Icon            =   "EingAbk.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   1380
   ScaleWidth      =   5010
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton abbrechen 
      Caption         =   "Abbrechen"
      Height          =   375
      Left            =   3540
      TabIndex        =   4
      Top             =   840
      Width           =   1395
   End
   Begin VB.CommandButton delete 
      Caption         =   "löschen"
      Height          =   375
      Left            =   1740
      TabIndex        =   3
      Top             =   840
      Width           =   1335
   End
   Begin VB.CommandButton ok 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   60
      TabIndex        =   2
      Top             =   840
      Width           =   1215
   End
   Begin VB.TextBox text_e 
      Height          =   285
      Left            =   600
      TabIndex        =   1
      Top             =   480
      Width           =   4335
   End
   Begin VB.TextBox abk_e 
      Height          =   285
      Left            =   960
      TabIndex        =   0
      Top             =   120
      Width           =   1455
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Text:"
      Height          =   195
      Left            =   60
      TabIndex        =   6
      Top             =   540
      Width           =   360
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Abkürzung:"
      Height          =   195
      Left            =   60
      TabIndex        =   5
      Top             =   180
      Width           =   810
   End
End
Attribute VB_Name = "EingabeAbkuerzung"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub abbrechen_Click()
Unload EingabeAbkuerzung
End Sub

Private Sub abk_e_GotFocus()
SendKeys "{home}+{end}"
End Sub


Private Sub Delete_Click()
Unload EingabeAbkuerzung
End Sub


Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
If KeyCode = 27 Then Unload EingabeAbkuerzung
End Sub

Private Sub Form_Load()
EingabeAbkuerzung.left = Eingabe.left + 2000
EingabeAbkuerzung.top = Eingabe.top + 2000
Label1.Caption = Lang(165)
Label2.Caption = Lang(166)
ok.Caption = Lang(1)
delete.Caption = Lang(167)
abbrechen.Caption = Lang(2)
Caption = Lang(168) + " " + Lang(165)

End Sub


Private Sub Form_Unload(Cancel As Integer)
Eingabe.Enabled = True
Eingabe.SetFocus
End Sub


Private Sub ok_Click()
Dim abk As Abkuerzungen
alt$ = abk_e.tag
neu$ = LCase(Trim(abk_e.text))
t$ = Trim(text_e.text)
If alt$ = "" Then alt$ = neu$
ff = FreeFile
Open DatenPfad + "\abk.dat" For Random As #ff Len = Len(abk)
high& = LOF(ff) / Len(abk)
For i& = high& To 1 Step -1
  Get #ff, i&, abk
  If LCase(Trim(abk.abk)) = alt$ Then
    If DebugLevel > 0 Then Debug.Print "Exchanging Shortcut "; alt$; "=>"; neu$; ":"; t$
    abk.abk = neu$: abk.Interpret = t$: Put #ff, i&, abk: Exit For
  End If
Next
If i& < 1 Then
  If DebugLevel > 0 Then Debug.Print "Adding New Shortcut "; alt$; "=>"; neu$; ":"; t$
  abk.abk = neu$: abk.Interpret = t$
  high& = high& + 1
  Put #ff, high&, abk
End If
Close #ff
Eingabe.e(0).text = t$
Unload EingabeAbkuerzung
Eingabe.e(1).SetFocus
End Sub


Private Sub text_e_GotFocus()
SendKeys "{home}+{end}"
End Sub



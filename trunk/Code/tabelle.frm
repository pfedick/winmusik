VERSION 5.00
Begin VB.Form tabelle 
   Caption         =   "Tabelle suchen"
   ClientHeight    =   4545
   ClientLeft      =   8115
   ClientTop       =   2010
   ClientWidth     =   4365
   ClipControls    =   0   'False
   Icon            =   "tabelle.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   4545
   ScaleWidth      =   4365
   Begin VB.CommandButton drucken 
      Height          =   315
      Left            =   2640
      Picture         =   "tabelle.frx":0442
      Style           =   1  'Grafisch
      TabIndex        =   5
      Top             =   3660
      Width           =   915
   End
   Begin VB.VScrollBar scroll 
      Height          =   3615
      Left            =   3300
      TabIndex        =   4
      Top             =   0
      Width           =   255
   End
   Begin VB.PictureBox listfeld 
      Height          =   3615
      Left            =   0
      ScaleHeight     =   3555
      ScaleWidth      =   3195
      TabIndex        =   3
      Top             =   0
      Width           =   3255
   End
   Begin VB.CommandButton suchen 
      Caption         =   "Suchen"
      Default         =   -1  'True
      Height          =   315
      Left            =   1860
      TabIndex        =   1
      Top             =   3660
      Width           =   735
   End
   Begin VB.TextBox suchfeld 
      Height          =   285
      Left            =   720
      TabIndex        =   0
      Text            =   "Text1"
      Top             =   3660
      Width           =   1095
   End
   Begin VB.Label suchwort 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Suchwort:"
      Height          =   195
      Left            =   0
      TabIndex        =   2
      Top             =   3720
      Width           =   720
   End
End
Attribute VB_Name = "tabelle"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim tableff As Long
Sub TabelleRuecksprung(a$)
  Select Case TabelleFuellFeld
    Case "eingabe 2": eingabe.e(2).text = a$
    Case "eingabe 4": eingabe.e(4).text = a$
    Case "eingabe 10": eingabe.e(10).text = a$
    Case "eingabe 12": eingabe.e(12).text = a$
    Case "stamm 4": traegerstamm.e(4).text = a$
    Case "stamm 6": traegerstamm.e(6).text = a$
  End Select
  Unload tabelle
End Sub

Sub TabelleSuchen(such$)
On Error Resume Next
Listfeld.MousePointer = 11
DoEvents
a$ = tmp + "\~wm20tab.tmp"
Kill a$
Close tableff
such$ = LTrim$(Trim$(such$))
tableff = FreeFile
Open a$ For Random As #tableff Len = Len(TabSuch)
ff = FreeFile
Open DatenPfad + "\" + TabelleName For Random As #ff Len = Len(tab80)
p& = 0
If such$ <> "" Then
  For i& = 1 To LOF(ff) / Len(tab80)
    Get #ff, i&, tab80
    If InStr(LCase$(tab80), such$) > 0 Then
      p& = p& + 1
      TabSuch.Index = i&: TabSuch.Inhalt = tab80
      Put #tableff, p&, TabSuch
    End If
  Next
Else
  For i& = 1 To LOF(ff) / Len(tab80)
    Get #ff, i&, tab80
    p& = p& + 1
    TabSuch.Index = i&: TabSuch.Inhalt = tab80
    Put #tableff, p&, TabSuch
  Next
End If
Close #ff
scroll.min = 0
scroll.max = p& - 1
scroll.LargeChange = 15
scroll.value = 0
Listfeld.Refresh
Listfeld.MousePointer = 0
End Sub


Private Sub Form_Deactivate()
Unload tabelle
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
If KeyCode = 27 Then Unload tabelle
If KeyCode = 115 Then
  TabelleVorgabe = LTrim$(Trim$(suchfeld.text))
  TabelleSuchen TabelleVorgabe
ElseIf KeyCode = 13 Then
  TabelleVorgabe = LTrim$(Trim$(suchfeld.text))
  If Val(TabelleVorgabe) > 0 Then
    TabelleRuecksprung TabelleVorgabe
  End If
End If
End Sub


Private Sub Form_Load()
Form_Resize
suchfeld = Trim$(TabelleVorgabe)
scroll.max = 0
suchwort.Caption = Lang(160)
suchen.Caption = Lang(161)
Select Case TabelleName
  Case "version.bez": a$ = Lang(73) + " " + Lang(161)
  Case "musikart.bez": a$ = Lang(74) + " " + Lang(161)
  Case "quelle.bez": a$ = Lang(80) + " " + Lang(161)
  Case "aufnahme.bez": a$ = Lang(81) + " " + Lang(161)
  Case "label.bez": a$ = Lang(110) + " " + Lang(161)
  Case "kauf.bez": a$ = Lang(111) + " " + Lang(161)
End Select
Caption = a$
Show
DoEvents
TabelleSuchen TabelleVorgabe
End Sub

Private Sub Form_LostFocus()
' Unload tabelle
End Sub


Private Sub Form_Resize()
If Height < 4410 Then Height = 4410
If Width < 3720 Then Width = 3720
Listfeld.Width = Width - 465
Listfeld.Height = Height - 795
scroll.left = Width - 420
scroll.Height = Height - 795
suchwort.top = Height - 690
suchfeld.top = Height - 750
suchen.top = Height - 750
drucken.top = Height - 750


End Sub

Private Sub Form_Unload(Cancel As Integer)
On Error Resume Next
Select Case TabelleFuellFeld
  Case "eingabe 2": eingabe.e(2).SetFocus
  Case "eingabe 4": eingabe.e(4).SetFocus
  Case "eingabe 10": eingabe.e(10).SetFocus
  Case "eingabe 12": eingabe.e(12).SetFocus
  Case "stamm 4": traegerstamm.e(4).SetFocus
  Case "stamm 6": traegerstamm.e(6).SetFocus
End Select
Close tableff
Kill tmp + "\~wm20tab.tmp"
End Sub

Private Sub Listfeld_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
On Error Resume Next
p& = Y / tpy / 13 + scroll.value
If p& <= scroll.max Then
  Get #tableff, p& + 1, TabSuch
  a$ = Format$(TabSuch.Index)
  TabelleRuecksprung a$
End If
End Sub


Private Sub ListFeld_Paint()
On Error Resume Next
Y = 0
For i& = scroll.value To scroll.max
  Get #tableff, i& + 1, TabSuch
  Listfeld.CurrentY = Y
  Listfeld.Print TabSuch.Index;
  Listfeld.CurrentX = 50 * tpx
  Listfeld.FontBold = True
  Listfeld.Print Trim$(TabSuch.Inhalt)
  Listfeld.FontBold = False
  Y = Y + 13 * tpy
  If Y > Listfeld.Height Then Exit For
Next
End Sub


Private Sub Scroll_Change()
Listfeld.Refresh
End Sub

Private Sub Scroll_Scroll()
Listfeld.Refresh
End Sub

Private Sub suchen_Click()
  TabelleVorgabe = LTrim$(Trim$(suchfeld.text))
  TabelleSuchen TabelleVorgabe
End Sub

Private Sub suchfeld_GotFocus()
SendKeys "{home}+{end}"
End Sub

Private Sub suchfeld_KeyDown(KeyCode As Integer, Shift As Integer)
On Error Resume Next
TabelleVorgabe = LTrim$(Trim$(suchfeld.text))
If KeyCode = 13 And Val(TabelleVorgabe) > 0 And Shift > 0 Then
    TabelleRuecksprung TabelleVorgabe
End If

If KeyCode = 13 Or KeyCode = 115 Then
  TabelleSuchen TabelleVorgabe
ElseIf KeyCode = 40 And scroll.value < scroll.max Then: scroll.value = scroll.value + 1
ElseIf KeyCode = 38 And scroll.value > 0 Then: scroll.value = scroll.value - 1
ElseIf KeyCode = 34 Then
  scroll.value = scroll.value + scroll.LargeChange
  If Err > 0 Then scroll.value = scroll.max
ElseIf KeyCode = 33 Then
  scroll.value = scroll.value - scroll.LargeChange
  If Err > 0 Then scroll.value = 0
End If

End Sub



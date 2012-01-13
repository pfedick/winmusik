VERSION 5.00
Begin VB.Form mp3rename 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "MP3-Files umbenennen"
   ClientHeight    =   6150
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6090
   Icon            =   "mp3rename.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6150
   ScaleWidth      =   6090
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows-Standard
   Begin VB.OptionButton Option2 
      Caption         =   "Träger+Track (1.001....)"
      Height          =   255
      Index           =   1
      Left            =   2940
      TabIndex        =   20
      Top             =   5280
      Width           =   2295
   End
   Begin VB.OptionButton Option2 
      Caption         =   "nur Track (001....)"
      Height          =   255
      Index           =   0
      Left            =   1320
      TabIndex        =   19
      Top             =   5280
      Width           =   1575
   End
   Begin VB.OptionButton Option1 
      Caption         =   "Index entfernen"
      Height          =   315
      Index           =   3
      Left            =   120
      TabIndex        =   16
      Top             =   4800
      Width           =   1455
   End
   Begin VB.ComboBox Combo_Space 
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   6.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   300
      ItemData        =   "mp3rename.frx":0CFA
      Left            =   5160
      List            =   "mp3rename.frx":0D04
      TabIndex        =   15
      Text            =   "Space"
      Top             =   4380
      Width           =   795
   End
   Begin VB.OptionButton Option1 
      Caption         =   "Interpret/Titel ergänzen"
      Height          =   315
      Index           =   2
      Left            =   120
      TabIndex        =   12
      Top             =   4380
      Width           =   2055
   End
   Begin VB.OptionButton Option1 
      Caption         =   "Nur Index"
      Height          =   315
      Index           =   1
      Left            =   120
      TabIndex        =   10
      Top             =   3960
      Value           =   -1  'True
      Width           =   1875
   End
   Begin VB.OptionButton Option1 
      Caption         =   "Index ergänzen"
      Height          =   315
      Index           =   0
      Left            =   120
      TabIndex        =   8
      Top             =   3540
      Width           =   2055
   End
   Begin VB.CommandButton abbrechen 
      Caption         =   "Abbrechen"
      Height          =   435
      Left            =   4140
      TabIndex        =   7
      Top             =   5640
      Width           =   1875
   End
   Begin VB.CommandButton OK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   435
      Left            =   60
      TabIndex        =   6
      Top             =   5640
      Width           =   1095
   End
   Begin VB.TextBox filter 
      Height          =   285
      Left            =   4620
      TabIndex        =   5
      Text            =   "*.mp*"
      Top             =   360
      Width           =   1335
   End
   Begin VB.FileListBox File1 
      Height          =   2625
      Left            =   3120
      Pattern         =   "*.mp*"
      TabIndex        =   3
      Top             =   780
      Width           =   2835
   End
   Begin VB.DirListBox Dir1 
      Height          =   2565
      Left            =   120
      TabIndex        =   1
      Top             =   840
      Width           =   2835
   End
   Begin VB.DriveListBox Drive1 
      Height          =   315
      Left            =   120
      TabIndex        =   0
      Top             =   360
      Width           =   2835
   End
   Begin VB.Line Line4 
      Index           =   1
      X1              =   60
      X2              =   6000
      Y1              =   5580
      Y2              =   5580
   End
   Begin VB.Label Label8 
      AutoSize        =   -1  'True
      Caption         =   "Index-Format:"
      Height          =   195
      Left            =   120
      TabIndex        =   18
      Top             =   5280
      Width           =   960
   End
   Begin VB.Line Line4 
      Index           =   0
      X1              =   60
      X2              =   6000
      Y1              =   5160
      Y2              =   5160
   End
   Begin VB.Line Line3 
      X1              =   60
      X2              =   6000
      Y1              =   3900
      Y2              =   3900
   End
   Begin VB.Line Line2 
      X1              =   60
      X2              =   6000
      Y1              =   4320
      Y2              =   4320
   End
   Begin VB.Line Line1 
      X1              =   60
      X2              =   6000
      Y1              =   4740
      Y2              =   4740
   End
   Begin VB.Label Label7 
      Caption         =   "001-Interpret-Titel.mp3 -> Interpret-Titel.mp3"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   6.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   2220
      TabIndex        =   17
      Top             =   4860
      Width           =   3075
   End
   Begin VB.Label Label6 
      Caption         =   "Aus ""Space"" wird:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   6.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   3960
      TabIndex        =   14
      Top             =   4440
      Width           =   1155
   End
   Begin VB.Label Label5 
      Caption         =   "001.mp3 -> 001-Interpret-Titel.mp3"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   6.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2220
      TabIndex        =   13
      Top             =   4380
      Width           =   1635
   End
   Begin VB.Label Label3 
      Caption         =   "001-Dateiname.mp3 -> 001.mp3"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   6.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   2220
      TabIndex        =   11
      Top             =   4020
      Width           =   3075
   End
   Begin VB.Label Label4 
      Caption         =   "Dateiname.mp3 -> 001-Dateiname.mp3"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   6.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   2220
      TabIndex        =   9
      Top             =   3600
      Width           =   3075
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "Filter:"
      Height          =   195
      Left            =   4200
      TabIndex        =   4
      Top             =   420
      Width           =   375
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Pfad der MP3-Dateien:"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   195
      Left            =   120
      TabIndex        =   2
      Top             =   120
      Width           =   1950
   End
End
Attribute VB_Name = "mp3rename"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub abbrechen_Click()
Unload mp3rename
End Sub

Private Sub abbrechen_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub


Private Sub Dir1_Change()
File1.Path = Dir1.Path

End Sub

Private Sub Dir1_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub

Private Sub Drive1_Change()
Dir1.Path = Drive1.Drive

End Sub

Private Sub Drive1_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub



Private Sub File1_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub

Private Sub filter_Change()
On Error Resume Next
File1.Pattern = filter.text

End Sub

Private Sub filter_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
If KeyCode = 27 Then Unload mp3rename
End Sub

Private Sub Form_KeyPress(KeyAscii As Integer)
'If KeyAscii = 27 Then Unload mp3rename
End Sub

Private Sub Form_Load()
On Error Resume Next
Me.Caption = Lang(248)
Label1.Caption = Lang(249)
Label2.Caption = Lang(250)
Option1(0).Caption = Lang(251)
Option1(1).Caption = Lang(252)
Label4.Caption = Lang(253)
Label3.Caption = Lang(254)
ok.Caption = Lang(1)
abbrechen.Caption = Lang(2)


Err = 0
Drive1.Drive = MpegPfad
If Err = 0 Then
  Dir1.Path = MpegPfad + "\" + Format(Int(traegerindex / 100), "00") + "\" + Format(traegerindex, "000")
  If Err > 0 Then
    Dir1.Path = MpegPfad
  End If
End If
             
End Sub

Private Sub Form_Unload(Cancel As Integer)
eingabe.Enabled = True
End Sub


Private Sub ok_Click()
Dim i As Integer
Dim file As String
Dim newfile As String
Dim endung As String
Dim prefix As String
Dim highest As Long
Dim ImportFile As Integer, ff As Integer
Dim mp3impdata As MP3impdataFile
Dim oldfilter As String
Dim num As Long
Dim myel As EingabeListing
Dim space As String

If Option1(3).value = True Then
    If MsgBox("Die Umbenennung kann nicht mehr rückgängig gemacht werden! Sind Sie sicher, daß Sie den Index entfernen wollen?", 48 + 4, "ACHTUNG!!!") <> vbYes Then Exit Sub
End If

space = Trim(Combo_Space.text)
If LCase(space) = "space" Then space = " "

On Error Resume Next

If File1.ListCount = 0 Then
    MsgBox Lang(256), vbInformation, Lang(248)
    mp3rename.Enabled = False
Else
    status.Show
    status.Caption = Lang(248)
    status.text.text = Lang(255)
    status.queue.min = 1
    status.queue.max = File1.ListCount
    status.queue.value = 1
    
    If Option1(0).value = True Then
        ' Hoechsten Track in diesem Verzeichnis raussuchen
        oldfilter = File1.Pattern
        highest = 0
        File1.Pattern = "???-*.mp3"
        For i = 0 To File1.ListCount - 1
            prefix = left(File1.List(i), 4)
            num = CLng(left(prefix, 3))
            If num > highest Then highest = num
        Next
        File1.Pattern = "???.mp3"
        For i = 0 To File1.ListCount - 1
            prefix = left(File1.List(i), 3)
            num = CLng(left(prefix, 3))
            If num > highest Then highest = num
        Next
        File1.Pattern = "*.???.mp3"
        For i = 0 To File1.ListCount - 1
            prefix = Mid(File1.List(i), InStr(File1.List(i), ".") + 1, 3)
            num = CLng(left(prefix, 3))
            If num > highest Then highest = num
        Next
        
        File1.Pattern = oldfilter
        
    ElseIf Option1(2).value = True Or Option1(3).value = True Then
        ffel = FreeFile
        'Debug.Print tmp + "\~wm2in" + Format$(Seite) + ".tmp"
        Open TraegerTmpFile(Format(Seite)) For Random As #ffel Len = Len(myel)
    
    End If
    
    For i = 0 To File1.ListCount - 1
        status.queue.value = i + 1
        'If i = 26 Then Stop
        file = File1.Path + "\" + File1.List(i)
        
        If Option1(0).value = True Then
            ' Dateiname.mp3 --> 001-Dateiname.mp3
            ' Nur umbenennen, wenn noch keine dreistellige Zahl gefolgt
            ' von einem Minuszeichen vorhanden ist.
            prefix = left(File1.List(i), 4)
            If right(prefix, 1) <> "-" Or Format(CLng(left(prefix, 3)), "000") <> left(prefix, 3) Then
              If right(prefix, 1) <> "." Then
                ' Naechsten freien Track zuweisen
                highest = highest + 1
                newfile = File1.Path + "\" + Format(highest, "000") + "-" + File1.List(i)
                'Debug.Print "Rename: " + file + " -> " + newfile
                Name file As newfile
              End If
            End If
            
            
        ElseIf Option1(1).value = True Then
            ' 001-Dateiname.mp3 --> 001.mp3
            endung = ".mp3"
            If LCase(right(file, 4)) <> endung Then
               p = InStr(File1.List(i), ".")
               If p > 0 Then endung = LCase(Mid(File1.List(i), p))
            End If
            newfile = File1.Path + "\" + left(File1.List(i), 3) + endung
            Name file As newfile
        Else
            ' 001.mp3 --> 001-Interpret-Titel.mp3
            endung = ".mp3"
            prefix = left(File1.List(i), 4)
            If right(prefix, 1) = "." Or right(prefix, 1) = "-" Then
            ThisTrack = CLng(left(prefix, 3))
            If LOF(ffel) / Len(myel) >= ThisTrack Then
                Get #ffel, ThisTrack, myel
                
                If Option1(3).value = True Then
                    newfile = File1.Path + "\" + MakeMp3Filename(myel.interpret, myel.titel, myel.info, space)
                Else
                    newfile = File1.Path + "\" + left(File1.List(i), 3) + "-" + MakeMp3Filename(myel.interpret, myel.titel, myel.info, space)
                End If
                'Debug.Print newfile
                Name file As newfile
            End If
            End If
        End If
        If status.abbrechen.tag = "stopit" Then
          mp3rename.Enabled = True
          Unload status
          Exit Sub
        End If
        
    Next
    If Option1(2).value = True Or Option1(3).value = True Then
           Close #ffel
    End If

    Unload status
    mp3rename.Enabled = True
    Unload mp3rename
    eingabe.SetFocus
End If


mp3load.Enabled = True

End Sub

Private Sub OK_KeyPress(KeyAscii As Integer)
If KeyAscii = 27 Then Unload mp3load

End Sub

Private Sub Option1_Click(Index As Integer)
If Index = 1 Then filter.text = "???-*.mp3"
If Index = 0 Then filter.text = "*.mp*"
End Sub

Function MakeMp3Filename(interpret As String, titel As String, version As String, space As String)
a$ = Trim(interpret) + "-" + Trim(titel) + " (" + Trim(version) + ").mp3"
If (space <> " ") Then
    a$ = ereg_replace(" ", space, a$):        ' Spaces entfernen
End If
a$ = ereg_replace("/", space, a$):        ' Slashes entfernen
a$ = ereg_replace("\", space, a$):        ' Backslashes entfernen
a$ = ereg_replace("?", space, a$):        ' Fragezeichen entfernen
a$ = ereg_replace(":", "-", a$):        ' Doppelpunkt ersetzen
a$ = ereg_replace("&", "+", a$):        ' Ampersond ersetzen
a$ = ereg_replace(Chr(34), "''", a$):        ' Ampersond ersetzen
MakeMp3Filename = a$
End Function

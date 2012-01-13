Attribute VB_Name = "pfpfunktionen"
DefInt A-Z
Type PFPHeader
  pfpID As String * 8
  FileVersion As String * 1
  HeaderLength As String * 1
  FileID As String * 4
  SubVersion As String * 1
  MainVersion As String * 1
  Pointer_Name As Integer
  Pointer_Autor As Integer
  Erstelldatum As Long
  Erstellzeit As Long
  Editdatum As Long
  Editzeit As Long
  Komprimierung As Integer
End Type

Global pfp As PFPHeader

Private Lng_Missing As String
Private Lng_Choose As String
Private Lng_File As String
Private Lng_Lang As String
Private Lng_Int As String
Private Lng_FileName As String
Private Lng_FileAutor As String
Private Lng_High As Long
Private Lng() As String
Private Lng_Par(10) As String

Sub INITLanguage(datei$)
Dim tmpstr As String * 256
Lng_File = datei$
If InStr(datei$, "\") = 0 Then Lng_File = CurDir + "\" + datei$
On Error Resume Next
ff = FreeFile
Open datei$ For Binary As #ff
If Err = 0 Then
  Get #ff, 1, pfp
  Get #ff, (1 + pfp.Pointer_Name), tmpstr
  Lng_FileName = Cut0(tmpstr)
  Get #ff, (1 + pfp.Pointer_Autor), tmpstr
  Lng_FileAutor = Cut0(tmpstr)
  Get #ff, 47, nr
  Get #ff, 1 + nr, tmpstr
  Lng_Lang = Cut0(tmpstr)
  Get #ff, 49, nr
  Get #ff, 1 + nr, tmpstr
  Lng_Int = Cut0(tmpstr)
  Get #ff, 51, nr
  Get #ff, 1 + nr, tmpstr
  Lng_Choose = Cut0(tmpstr)
  Get #ff, 45, nr
  Get #ff, 1 + nr, tmpstr
  Lng_Missing = Cut0(tmpstr)
  Get #ff, 39, Lng_High
  Get #ff, 43, startpp
  p& = startpp
  ReDim Lng(Lng_High) As String
  Do
    Get #ff, 1 + p&, nr
    p& = p& + 2
    If nr > 0 Then
      Get #ff, 1 + p&, tmpstr
      Lng(nr) = Cut0(tmpstr)
      p& = p& + Len(Lng(nr)) + 1
    End If
  Loop Until nr = 0
End If
Close #ff
End Sub

Sub INITPFP(PFPVar As PFPHeader)
PFPVar.pfpID = "PFP-File"
PFPVar.FileVersion = Chr$(2)
PFPVar.HeaderLength = Chr$(38)
PFPVar.FileID = "????"
PFPVar.SubVersion = Chr$(0)
PFPVar.MainVersion = Chr$(0)
PFPVar.Pointer_Name = 0
PFPVar.Pointer_Autor = 0
PFPVar.Erstelldatum = 0
PFPVar.Erstellzeit = 0
PFPVar.Editdatum = 0
PFPVar.Editzeit = 0
PFPVar.Komprimierung = 0
End Sub

Function Lang(nr As Integer) As String
If nr = 0 Or nr > Lng_High Then
  Lang = Lng_Missing + ":" + Str$(nr)
Else
  a$ = Lng(nr)
  If a$ = "" Then
    a$ = Lng_Missing + ":" + Str$(nr)
  Else
   Do
    OK = True
    n = InStr(a$, "%inc")
    If n > 0 Then
      OK = False: e = InStr(n + 1, a$, "%")
      neu = Val(Mid$(a$, n + 4, e - (n + 4)))
      a$ = left$(a$, n - 1) + Lng(neu) + Mid$(a$, e + 1)
    End If
    n = InStr(a$, "~")
    If n > 0 Then
      OK = False: a$ = left$(a$, n - 1) + Chr$(13) + Chr$(10) + Mid$(a$, n + 1)
    End If
    n = InStr(a$, "%par")
    If n > 0 Then
      OK = False: e = InStr(n + 1, a$, "%")
      neu = Val(Mid$(a$, n + 4, e - (n + 4)))
      a$ = left$(a$, n - 1) + Lng_Par(neu) + Mid$(a$, e + 1)
    End If
    n = InStr(a$, "%name%")
    If n > 0 Then
      OK = False: a$ = left$(a$, n - 1) + Programm + Mid$(a$, n + 6)
    End If
    n = InStr(a$, "%ver%")
    If n > 0 Then
      OK = False: a$ = left$(a$, n - 1) + version + Mid$(a$, n + 5)
    End If
    n = InStr(a$, "%progdate%")
    If n > 0 Then
      OK = False: a$ = left$(a$, n - 1) + Progdate + Mid$(a$, n + 10)
    End If
   Loop Until OK = True
  End If
  Lang = a$
End If
End Function
    

Function LangFile() As String
LangFile = Lng_File
End Function

Function LangName() As String
LangName = Lng_Lang
End Function
Function IntLangName() As String
IntLangName = Lng_Int
End Function

Sub LangPar(Parameter%, Inhalt$)
If Parameter% < 11 And Parameter > 0 Then
  Lng_Par(Parameter%) = Inhalt$
Else
  Debug.Print "Module: PFP, Sub: LangPar: Wrong number for Parameter%"
End If
End Sub


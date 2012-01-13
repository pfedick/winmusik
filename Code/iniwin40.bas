Attribute VB_Name = "iniwin40"
'*********************************************************
'*                  INIWIN 4.0   32-Bit                  *
'*                                                       *
'*     (c) by Patrick F.-Productions am 29.04.1996       *
'*                                                       *
'*********************************************************

'OPENINI Filename$     Öffnet eine INI-Datei, die sich im aktuellen Pfad, im Windows-Pfad,
'                      im PFP-Unterverzeichnis von Windows oder in einem durch die
'                      Umgebungsvariable PATH angegebenen Pfad befindet.
'SELECTTopic Topic$    Bestimmt den Abschnitt in der INI-Datei, in dem die Daten zu finden
'                      sind.
'GETINI (Eintrag$)     Liefert den String zum angegebenen Eintrag
'PUTINI Eintrag$       Schreibt den Eintrag in den aktuellen Abschnitt
'CLOSEINI              Schließt die INI-Datei
'                      (Dieser Befehl ist ab der Version 3.0 nicht mehr notwendig)

Const INIVerString = "INIWIN 4.0 16/32-Bit (c) by Patrick Fedick am 29.04.1996"

Type Rect
    left As Integer
    top As Integer
    right As Integer
    bottom As Integer
End Type


Dim INIFileName As String
Dim INITopic As String
Dim INIStatus As Integer
Dim INIFehlerCode As Integer

#If Win16 Then
  Declare Function GetPrivateProfileString Lib "Kernel" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Integer, ByVal lpFileName As String) As Integer
  Declare Function WritePrivateProfileString Lib "Kernel" (ByVal lpApplicationName As Any, ByVal lpKeyName As Any, ByVal lpString As Any, ByVal lplFileName As String) As Integer
  Declare Function GetWindowsDirectory Lib "Kernel" (ByVal lpBuffer As String, ByVal nSize As Integer) As Integer
  Declare Function GetSystemDirectory Lib "Kernel" (ByVal lpBuffer As String, ByVal nSize As Integer) As Integer
#ElseIf Win32 Then
  Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long
  Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpString As Any, ByVal lpFileName As String) As Long
  Declare Function GetWindowsDirectory Lib "kernel32" Alias "GetWindowsDirectoryA" (ByVal lpBuffer As String, ByVal nSize As Long) As Long
  Declare Function GetSystemDirectory Lib "kernel32" Alias "GetSystemDirectoryA" (ByVal lpBuffer As String, ByVal nSize As Long) As Long
  Declare Function GetTempPath Lib "kernel32" Alias "GetTempPathA" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long
#End If


Sub closeini()
INIFileName = ""
End Sub

Sub CreateINI(file, INITitel)
#If Win32 Then
  Dim x As Long
#Else
  Dim x As Integer
#End If
Dim Eintrag As String, Inhalt As String
INIFileName = file
If InStr(file, "\") = 0 Then INIFileName = WindowsPfad() + "\" + file
Eintrag = "Created"
Inhalt = Mid$(Date$, 4, 2) + "." + left$(Date$, 2) + "." + right$(Date$, 4)
x = WritePrivateProfileString(INITitel, Eintrag, Inhalt, INIFileName)
End Sub

Sub CreateTopic(Topic)
Dim x As Integer
If INIFileName = "" Then INIFehlerCode = 1: Exit Sub
INITopic = Topic
End Sub

Function GetINI(Suchtext As String) As String
If INIFileName = "" Then INIFehlerCode = 1: Exit Function
If INITopic = "" Then INIFehlerCode = 6: Exit Function

#If Win32 Then
  Dim worked As Long
#Else
  Dim worked As Integer
#End If
Dim retval As String
INIFehlerCode = 0
retval = String$(255, 0)
worked = GetPrivateProfileString(INITopic, Suchtext, "", retval, Len(retval), INIFileName)
If worked = 0 Then
  GetINI = "": INIFehlerCode = 7
Else
  GetINI = Trim$(left(retval, worked))
End If
End Function

Function GetSysINI(section As String, key As String)
#If Win32 Then
  Dim retval As String, AppName As String, worked As Long
#Else
  Dim retval As String, AppName As String, worked As Integer
#End If
retval = String$(255, 0)
worked = GetPrivateProfileString(section, key, "", retval, Len(retval), "System.ini")
If worked = 0 Then
  GetSysINI = "unknown"
Else
  GetSysINI = left(retval, worked)
End If
End Function

Function inierror() As Integer
inierror = INIFehlerCode
End Function

Function inierrortext() As String
Dim a As String
Select Case INIFehlerCode
  Case 0: a = "Kein Fehler"
  Case 1: a = "Keine INI-Datei geöffnet"
  Case 2: a = "INI-Datei nicht gefunden"
  Case 3: a = "INI-Datei konnte nicht erstellt werden"
  Case 4: a = "INI-Datei ist zu groß (über 30000 Einträge)"
  Case 5: a = "Topic wurde nicht gefunden"
  Case 6: a = "Kein Topic angegeben"
  Case 7: a = "Eintrag wurde nicht gefunden"
  Case 8: a = "Topic konnte nicht erstellt werden"
  Case 9: a = "Topic bereits vorhanden"
  Case 10: a = "Eintrag konnte nicht erstellt werden"
  Case 11: a = "INI-Datei kann nicht geschlossen werden"
  Case 12: a = "INI-Datei kann nicht geöffnet werden"
  Case 13: a = "INI-Datei bereits vorhanden"
  Case 14: a = "Ausdruck unvollständig: '=' fehlt"
  Case 15: a = "Funktion wird nicht mehr unterstützt!"
  Case Else: a = "unbekannter Fehler"
End Select
inierrortext = a
End Function

Function INIVer() As String
INIVer = INIVerString
End Function

Sub openini(file$)
Dim INIPfad As String, INIPfadLänge As Integer
Dim dummy As Integer, intern As String
On Error Resume Next
If InStr(file$, ".") = 0 Then file$ = file$ + ".INI"
INIFile = FreeFile
If InStr(file$, "\") > 0 Then
  intern = file$
Else
  intern = CurDir$ + "\" + UCase$(file$)
End If
Err = 0
Open intern For Input As #INIFile
If Err > 0 Then
  Close INIFile
  INIPfad = WindowsPfad()
  intern = Trim$(INIPfad) + "\" + file$
  Err = 0
  Open intern For Input As #INIFile
  If Err > 0 Then
    Close INIFile
    intern = Trim$(INIPfad) + "\PFP\" + file$
    Err = 0
    Open intern For Input As #INIFile
    If Err > 0 Then
      Close INIFile
      INIFileName = ""
      INIFehlerCode = 2
      Exit Sub
    End If
  End If
End If
INIFileName = intern
Close INIFile: INIFile = 0
INIFehlerCode = 0
INISelectStart = -1
End Sub

Sub printINI()
INIFehlerCode = 15
End Sub

Sub putini(Eintrag As String)
#If Win32 Then
  Dim x As Long
#Else
  Dim x As Integer
#End If
Dim Inhalt As String

If INIFileName = "" Then INIFehlerCode = 1: Exit Sub
If INITopic = "" Then INIFehlerCode = 6: Exit Sub
x = InStr(Eintrag, "="): If x = 0 Then INIFehlerCode = 14: Exit Sub
If x = 0 Then Exit Sub
Inhalt = Mid$(Eintrag, x + 1)
Eintrag = left$(Eintrag, x - 1)
x = WritePrivateProfileString(INITopic, Eintrag + Chr$(0), Inhalt + Chr$(0), INIFileName)
If x <> 1 Then INIFehlerCode = 10
End Sub

Function Rect2String(Ausdruck As Rect) As String
Dim a$
a$ = Format$(Ausdruck.left) + "," + Format$(Ausdruck.top) + ","
a$ = a$ + Format$(Ausdruck.right) + "," + Format$(Ausdruck.bottom)
Rect2String = a$
End Function

Sub selectTopic(Topic As String)
INITopic = Topic
End Sub

Sub String2Rect(StringAusdruck As String, RectAusdruck As Rect)
Dim a$, i As Integer, f As Rect
a$ = StringAusdruck
f.left = Val(a$): i = InStr(a$, ","): a$ = Mid$(a$, i + 1)
f.top = Val(a$): i = InStr(a$, ","): a$ = Mid$(a$, i + 1)
f.right = Val(a$): i = InStr(a$, ","): a$ = Mid$(a$, i + 1)
f.bottom = Val(a$)
RectAusdruck = f
End Sub

Function WindowsPfad() As String
#If Win16 Then
  Dim dummy As Integer, l As Integer
  l = 127
#Else
  Dim dummy As Long, l As Long
  l = 256
#End If
Dim a$

a$ = Space$(l)
dummy = GetWindowsDirectory(a$, l)
a$ = Trim$(a$)
l = InStr(a$, Chr$(0))
If l > 0 Then a$ = left$(a$, l - 1)
WindowsPfad = a$
End Function


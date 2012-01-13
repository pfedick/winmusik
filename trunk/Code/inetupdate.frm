VERSION 5.00
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.2#0"; "COMCTL32.OCX"
Object = "{48E59290-9880-11CF-9754-00AA00C00908}#1.0#0"; "MSINET.OCX"
Begin VB.Form inetupdate 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "Form1"
   ClientHeight    =   4125
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6255
   Icon            =   "inetupdate.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4125
   ScaleWidth      =   6255
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows-Standard
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   3300
      Top             =   3540
   End
   Begin VB.CommandButton abbrechen 
      Caption         =   "Abbrechen"
      Enabled         =   0   'False
      Height          =   375
      Left            =   4740
      TabIndex        =   3
      Top             =   3660
      Width           =   1335
   End
   Begin VB.CommandButton download 
      Caption         =   "Download"
      Enabled         =   0   'False
      Height          =   375
      Left            =   120
      TabIndex        =   2
      Top             =   3660
      Width           =   1395
   End
   Begin ComctlLib.ProgressBar progress 
      Height          =   195
      Left            =   1020
      TabIndex        =   1
      Top             =   4140
      Visible         =   0   'False
      Width           =   4485
      _ExtentX        =   7911
      _ExtentY        =   344
      _Version        =   327682
      Appearance      =   1
   End
   Begin InetCtlsObjects.Inet Inet1 
      Left            =   1860
      Top             =   3480
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   327681
      Protocol        =   4
      RemoteHost      =   "www2.pfp.de"
      URL             =   "http://www2.pfp.de/wm/wmupdate.html"
      Document        =   "/wm/wmupdate.html"
   End
   Begin VB.Label prozent 
      Caption         =   "0%"
      Height          =   255
      Left            =   5580
      TabIndex        =   4
      Top             =   4140
      Visible         =   0   'False
      Width           =   495
   End
   Begin VB.Label info 
      BackColor       =   &H80000005&
      BorderStyle     =   1  'Fest Einfach
      Height          =   3315
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   5955
   End
End
Attribute VB_Name = "inetupdate"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
DefInt A-Z

Dim downloadrunning As Boolean
Dim server As String
Dim updateurl As String
Dim updatefile As String
Dim filesize As Long
Dim fullversion As String
Dim localfile As String
Dim filehandle As Integer



Private Sub abbrechen_Click()
If downloadrunning = False Then
  Unload inetupdate
Else
  If filehandle > 0 Then Close filehandle
  Inet1.Cancel
  downloadrunning = False
  Timer1.Enabled = False
  
  filehandle = 0
  Unload inetupdate
  
End If

End Sub


Private Sub download_Click()
download.Enabled = False
progress.Value = 0
info.Caption = Lang(234)
r = InStr(server, ",")
If updatefile = "full" Then a$ = fullversion Else a$ = updatefile

a$ = left$(server, r - 1) + a$
r = InStr(a$, "=")
f$ = Mid$(a$, r + 1)
h$ = left$(a$, r - 1)
localfile = tmp + "\wmupdate.exe"
On Error Resume Next
Kill localfile

Inet1.URL = a$
filehandle = 0
Inet1.Execute h$, "GET " + f$ + " " + localfile




End Sub


Private Sub Form_Load()
Dim t As Variant
Dim ret As String
Dim desc As String
Dim currentversion As String, currentbuild As String, currentdate As String


On Error Resume Next

Width = 6330
Height = 4500
'Height = 4905

ret = Chr$(13) + Chr$(10)

a$ = Trim(InetProxy) + ":" + Trim(InetPort)
If InetProxy = "" Then a$ = ""
Inet1.proxy = a$
a$ = InetUpdateURL + "?version=" + version + "&build=" + Format(build) + "&lang=" + IntLangName()
Inet1.URL = a$
info.Caption = Lang(227)

inetupdate.Show
DoEvents
Inet1.RequestTimeout = 10
t = Inet1.OpenURL
If (Err > 0) Then
  info.Caption = Lang(227) + ret + Lang(228) + ret + Lang(229)
Else
  p = 1
  d = 0
  server = ""
  Do
    r = InStr(p, t, Chr$(10))
    a$ = Trim(Mid$(t, p, r - p))
    p = r + 1
    If (left$(a$, 7) = "server=") Then
      server = server + Mid$(a$, 8) + ","
    ElseIf (left$(a$, 10) = "updateurl=") Then
      updateurl = Mid$(a$, 11)
    ElseIf (left$(a$, 11) = "updatefile=") Then
      updatefile = Mid$(a$, 12)
    ElseIf (left$(a$, 12) = "fullversion=") Then
      fullversion = Mid$(a$, 13)
    ElseIf (left$(a$, 15) = "currentversion=") Then
      currentversion = Mid$(a$, 16)
    ElseIf (left$(a$, 13) = "currentbuild=") Then
      currentbuild = Mid$(a$, 14)
    ElseIf (left$(a$, 12) = "currentdate=") Then
      currentdate = Mid$(a$, 13)
    ElseIf (left$(a$, 9) = "filesize=") Then
      filesize = CLng(Mid$(a$, 10))
    ElseIf (left$(a$, 12) = "description:") Then
      d = 1
    
    End If
    If d = 1 Then desc = desc + a$ + "~"
    
    If r = 0 Or a$ = "-->" Then Exit Do
  Loop
  
  If updatefile = "none" Then
    
    info.Caption = info.Caption + ret + Lang(198)
    abbrechen.Enabled = True
    
    
  
  Else
    LangPar 1, version
    LangPar 2, build
    LangPar 3, Progdate
       
    a$ = ret + Lang(230) + " "
    LangPar 1, currentversion
    LangPar 2, currentbuild
    LangPar 3, currentdate
    a$ = a$ + Lang(231) + ret
    If updatefile = "full" Then
      a$ = a$ + Lang(232) + ret
    End If
    LangPar 1, Str$(filesize)
    a$ = a$ + ret + Lang(233) + ret + ret + Lang(201)
    
    
    info.Caption = info.Caption + a$
    p = 1: dd$ = ""
    Do
      r = InStr(p, desc, "~")
      a$ = Trim(Mid$(desc, p, r - p))
      p = r + 1
      If r = 0 Or a$ = "-->" Then Exit Do
      r = InStr(a$, ":")
      b = CLng(left$(a$, r - 1))
      If b > build Then
        dd$ = dd$ + Mid$(a$, r + 1) + ret
      End If
    Loop
    
    info.Caption = info.Caption + dd$
    downloadrunning = False
    
    download.Enabled = True
    abbrechen.Enabled = True
    
  
  End If
  
  
End If







End Sub


Private Sub Inet1_StateChanged(ByVal State As Integer)
' Mit der GetChunk-Methode eine Server-Antwort
' auswerten wenn State = 12 ist.

Dim vtData As Variant ' Datenvariable.
Select Case State
    ' ... Andere Fälle nicht gezeigt.
    Case 4
        info.Caption = info.Caption + Chr$(13) + Chr$(10) + Lang(236) + Chr$(13) + Chr$(10)
    Case 8
        downloadrunning = True
        Timer1.Enabled = True
        
    Case 11 ' 11
        ' Falls ein Fehler auftritt, ResponseCode
        ' und ResponseInfo zurückgeben.
  
        vtData = Inet1.ResponseCode & ":" & _
        Inet1.ResponseInfo
        
    Case 12 ' 12
        vtData = Inet1.GetChunk(1024)
        Unload inetupdate
        downloadrunning = False
        If filehandle > 0 Then Close filehandle
        Timer1.Enabled = False
        filehandle = 0
        
        Unload leiste1
        DoEvents
        LangPar 1, localfile
        
        MsgBox Lang(235), 0, Programm
        DoEvents
        Sleep 1
        
        Shell localfile, vbNormalFocus
        
        DoEvents
        
        
End Select
'Debug.Print vtData

End Sub

Private Sub Timer1_Timer()
If downloadrunning = True Then

  If filehandle = 0 Then
    filehandle = FreeFile
    On Error Resume Next
    Open localfile For Binary Access Read As #filehandle
    If Err > 0 Then filehandle = 0
  End If
  
  
  
Else

End If

End Sub



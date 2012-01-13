Attribute VB_Name = "wm2common"

Sub ReadINI()
openini AppData + "winmusik2.ini"
If inierror() = 0 Then
  selectTopic "Programm"
  putini "Letzter Aufruf=" + Datum$() + ", " + Time$
  user = GetINI$("Benutzer")
  UserFirma = GetINI$("Firma")
  Waehrung = GetINI$("Währung")
  drucker = GetINI$("Drucker")
  a$ = GetINI$("Farbdruck")
  Farbdruck = bool(a$)
  AutoInterpret = bool(GetINI$("AutoInterpret"))
  AutoTitel = bool(GetINI$("AutoTitel"))
  ShowSplashScreen = bool(GetINI$("ShowSplashScreen"))
  If inierror() > 0 Then ShowSplashScreen = True
  InetProxy = GetINI$("InetProxy")
  InetPort = GetINI$("InetPort")
  
  RequireSetup = bool(GetINI$("RequireSetup"))
  If inierror() > 0 Then RequireSetup = False
  

  selectTopic "Pfade"
  HauptPfad = GetINI$("Main")
  TiDatei = GetINI$("Titeldatei")
  CDHeaderDatei = GetINI$("CDHeaderDatei")
  tmp = GetINI$("Temp")
  DatenPfad = GetINI$("Daten")
  SprachDatei = GetINI$("Sprachdatei")
  MpegPfad = GetINI$("MPEG")
  If MpegPfad = "" Then MpegPfad = tmp
  CDRipperPfad = GetINI$("CDRipper")
  If CDRipperPfad = "" Then CDRipperPfad = tmp
  
  selectTopic "Programms"
  WAVEPlayer = GetINI$("WAVEPlayer")
  MPEGPlayer = GetINI$("MPEGPlayer")
  MixerProg = GetINI$("Mixer")
  
  selectTopic "Database"
  db.host = GetINI$("Host")
  db.port = Val(GetINI$("Port"))
  db.name = GetINI$("Name")
  db.user = GetINI$("User")
  db.password = GetINI$("Password")
  
  TraegerBez(1) = "kassette"
  TraegerBez(2) = "cd"
  TraegerBez(3) = "daten"
  TraegerBez(4) = "platte"
  TraegerBez(5) = "video"
  TraegerBez(6) = "sonstige"
  TraegerBez(7) = "mpeg"
End If
End Sub


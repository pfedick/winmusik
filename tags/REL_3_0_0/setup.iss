;/*
; * This file is part of WinMusik 3.0 by Patrick Fedick
; *
; * $Author: pafe $
; * $Revision: 1.1 $
; * $Date: 2010/05/16 12:23:30 $
; * $Id: setup.iss,v 1.1 2010/05/16 12:23:30 pafe Exp $
; *
; *
; * Copyright (c) 2009 Patrick Fedick
; *
; * This program is free software: you can redistribute it and/or modify
; * it under the terms of the GNU General Public License as published by
; * the Free Software Foundation, either version 3 of the License, or
; * (at your option) any later version.
; *
; * This program is distributed in the hope that it will be useful,
; * but WITHOUT ANY WARRANTY; without even the implied warranty of
; * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; * GNU General Public License for more details.
; *
; * You should have received a copy of the GNU General Public License
; * along with this program.  If not, see <http://www.gnu.org/licenses/>.
; */


[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{611E500A-4D60-400C-9829-0114ADE682D5}}
AppName=WinMusik
AppVerName=WinMusik 3.0
AppCopyright=Copyright (C) 2009 Patrick Fedick
AppVersion=3.0.0
VersionInfoVersion=3.0.0
AppPublisher=Patrick F.-Productions
AppPublisherURL=http://www.pfp.de/
AppSupportURL=http://www.pfp.de/
AppUpdatesURL=http://www.pfp.de/
DefaultDirName={pf}\Patrick F.-Productions\WinMusik3
DefaultGroupName=Patrick F.-Productions\WinMusik 3
AllowNoIcons=yes
OutputDir=distfiles
OutputBaseFilename=WinMusik-3.0.0-Setup
SetupIconFile=resources\\WinMusik3.ico
Compression=lzma/ultra
SolidCompression=yes
WizardImageFile=resources\setup.bmp
WizardSmallImageFile=resources\setup-icon.bmp
LicenseFile=LICENSE.TXT
PrivilegesRequired=none


[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"; InfoBeforeFile: README_en.TXT
Name: "de"; MessagesFile: "compiler:Languages\German.isl"; InfoBeforeFile: README_de.TXT


[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked;
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked;


[Files]
Source: "release\WinMusik3.exe"; DestDir: "{app}"; Flags: ignoreversion;
Source: "LICENSE.TXT"; DestDir: "{app}"; Flags: ignoreversion;
Source: "HISTORY.TXT"; DestDir: "{app}"; Flags: ignoreversion;
Source: "README_de.TXT"; DestDir: "{app}"; DestName: "LIESMICH.TXT"; Languages: de; Flags: ignoreversion;
Source: "README_en.TXT"; DestDir: "{app}"; DestName: "README.TXT"; Languages: de; Flags: ignoreversion;
Source: "docs/Userguide_de.pdf"; DestDir: "{app}"; DestName: "Userguide_de.pdf"; Flags: ignoreversion;
Source: "resources\uninstall.ico"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\Qt\4.5.2\bin\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\Qt\4.5.2\bin\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\Qt\4.5.2\bin\mingwm10.dll"; DestDir: "{app}"; Flags: ignoreversion;


[Icons]
Name: "{group}\WinMusik 3"; WorkingDir: "{app}"; Filename: "{app}\WinMusik3.exe"
Name: "{group}\{cm:UninstallProgram,WinMusik 3}"; IconFilename: "{app}\uninstall.ico"; Filename: "{uninstallexe}"
Name: "{group}\Liesmich.txt"; WorkingDir: "{app}"; Filename: "{app}\LIESMICH.TXT"; Languages: de;
Name: "{group}\Readme.txt"; WorkingDir: "{app}"; Filename: "{app}\README.TXT"; Languages: en;
Name: "{group}\Lizensbedingungen.txt"; WorkingDir: "{app}"; Filename: "{app}\LICENSE.TXT"; Languages: de;
Name: "{group}\License.txt"; WorkingDir: "{app}"; Filename: "{app}\LICENSE.TXT"; Languages: en;
Name: "{group}\Handbuch.pdf"; WorkingDir: "{app}"; Filename: "{app}\Userguide_de.pdf"; Languages: de;
Name: "{group}\Userguide.pdf"; WorkingDir: "{app}"; Filename: "{app}\Userguide_de.pdf"; Languages: en;
Name: "{commondesktop}\WinMusik 3"; Filename: "{app}\WinMusik3.exe"; Tasks: desktopicon;


[Run]
Filename: "{app}\WinMusik3.exe"; Description: "{cm:LaunchProgram,WinMusik 3}"; Flags: nowait postinstall skipifsilent;
















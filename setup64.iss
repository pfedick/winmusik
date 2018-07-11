;/*
; * This file is part of WinMusik 3.0 by Patrick Fedick
; *
; * $Author: pafe $
; * $Revision: 1.7 $
; * $Date: 2012/01/09 21:00:00 $
; * $Id: setup.iss,v 1.7 2012/01/09 21:00:00 pafe Exp $
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
AppVerName=WinMusik 3.5.1
AppCopyright=Copyright (C) 2018 Patrick Fedick
AppVersion=3.5.1.0
VersionInfoVersion=3.5.1
AppPublisher=Patrick F.-Productions
AppPublisherURL=http://www.pfp.de/
AppSupportURL=http://www.pfp.de/
AppUpdatesURL=http://www.pfp.de/
DefaultDirName={pf}\Patrick F.-Productions\WinMusik3
DefaultGroupName=Patrick F.-Productions\WinMusik 3
AllowNoIcons=yes
OutputDir=distfiles
OutputBaseFilename=WinMusik-3.5.1-Setup-64bit
SetupIconFile=resources\WinMusik3.ico
Compression=lzma/ultra64
SolidCompression=yes
WizardImageFile=resources\setup.bmp
WizardSmallImageFile=resources\setup-icon.bmp
LicenseFile=LICENSE.TXT
PrivilegesRequired=none
ArchitecturesAllowed=x64

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"; InfoBeforeFile: README_en.TXT
Name: "de"; MessagesFile: "compiler:Languages\German.isl"; InfoBeforeFile: README_de.TXT


[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked;
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked;


[Files]
Source: "release\WinMusik.exe"; DestDir: "{app}"; Flags: ignoreversion;
Source: "LICENSE.TXT"; DestDir: "{app}"; Flags: ignoreversion;
Source: "HISTORY.TXT"; DestDir: "{app}"; Flags: ignoreversion;
Source: "README_de.TXT"; DestDir: "{app}"; DestName: "LIESMICH.TXT"; Languages: de; Flags: ignoreversion;
Source: "README_en.TXT"; DestDir: "{app}"; DestName: "README.TXT"; Languages: de; Flags: ignoreversion;
Source: "docs/Userguide_de.pdf"; DestDir: "{app}"; DestName: "Userguide_de.pdf"; Flags: ignoreversion;
Source: "resources\uninstall.ico"; DestDir: "{app}"; Flags: ignoreversion;

Source: "C:\msys64\mingw64\bin\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libicuuc61.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libicuin61.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libicudt61.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libpcre16-0.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libgcc_s_seh-1.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libharfbuzz-0.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libfreetype-6.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libglib-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libgraphite2.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libintl-8.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libbz2-1.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libpcre-1.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libiconv-2.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\SSLEAY32.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\LIBEAY32.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libmp3lame-0.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libpcre2-16-0.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libjpeg-8.dll"; DestDir: "{app}"; Flags: ignoreversion;

; Dependencies of qt imageplugins
Source: "C:\msys64\mingw64\bin\libjasper-4.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libmng-2.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libtiff-5.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libpng16-16.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libpng16-16.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libwebp-7.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\bin\libwebpdemux-2.dll"; DestDir: "{app}"; Flags: ignoreversion;

; Qt specific
Source: "release\deploy\*.dll"; DestDir: "{app}"; Flags: ignoreversion;
; Qt Plugins
Source: "release\deploy\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion;
Source: "release\deploy\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion;
Source: "release\deploy\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion;
Source: "release\deploy\printsupport\*"; DestDir: "{app}\printsupport"; Flags: ignoreversion;
Source: "release\deploy\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion;
Source: "release\deploy\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion;
Source: "C:\msys64\mingw64\share\qt5\plugins\audio\qtaudio_windows.dll"; DestDir: "{app}\audio"; Flags: ignoreversion;

[Icons]
Name: "{group}\WinMusik 3"; WorkingDir: "{app}"; Filename: "{app}\WinMusik.exe"
Name: "{group}\{cm:UninstallProgram,WinMusik 3}"; IconFilename: "{app}\uninstall.ico"; Filename: "{uninstallexe}"
Name: "{group}\Liesmich.txt"; WorkingDir: "{app}"; Filename: "{app}\LIESMICH.TXT"; Languages: de;
Name: "{group}\Readme.txt"; WorkingDir: "{app}"; Filename: "{app}\README.TXT"; Languages: en;
Name: "{group}\Lizenzbedingungen.txt"; WorkingDir: "{app}"; Filename: "{app}\LICENSE.TXT"; Languages: de;
Name: "{group}\License.txt"; WorkingDir: "{app}"; Filename: "{app}\LICENSE.TXT"; Languages: en;
Name: "{group}\Handbuch.pdf"; WorkingDir: "{app}"; Filename: "{app}\Userguide_de.pdf"; Languages: de;
Name: "{group}\Userguide.pdf"; WorkingDir: "{app}"; Filename: "{app}\Userguide_de.pdf"; Languages: en;
Name: "{commondesktop}\WinMusik 3"; Filename: "{app}\WinMusik.exe"; Tasks: desktopicon;


[Run]
Filename: "{app}\WinMusik.exe"; Description: "{cm:LaunchProgram,WinMusik 3}"; Flags: 64bit nowait postinstall skipifsilent;


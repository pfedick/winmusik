VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form propertynew 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "Form1"
   ClientHeight    =   4290
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6030
   Icon            =   "propertynew.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4290
   ScaleWidth      =   6030
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows-Standard
   Begin VB.CommandButton ok 
      Caption         =   "OK"
      Height          =   375
      Left            =   60
      TabIndex        =   9
      Top             =   3840
      Width           =   975
   End
   Begin VB.CommandButton abbrechen 
      Caption         =   "Abbrechen"
      Height          =   375
      Left            =   2220
      TabIndex        =   8
      Top             =   3840
      Width           =   1335
   End
   Begin VB.CommandButton uebernehmen 
      Caption         =   "Übernehmen"
      Height          =   375
      Left            =   4620
      TabIndex        =   7
      Top             =   3840
      Width           =   1335
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   3735
      Left            =   60
      TabIndex        =   0
      Top             =   60
      Width           =   5895
      _ExtentX        =   10398
      _ExtentY        =   6588
      _Version        =   393216
      Style           =   1
      Tabs            =   5
      TabsPerRow      =   5
      TabHeight       =   520
      TabCaption(0)   =   " WinMusik"
      TabPicture(0)   =   "propertynew.frx":030A
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "Label6"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "Label5"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "Label4"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "Label3"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "Label2"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "Label1"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "Image1"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).ControlCount=   7
      TabCaption(1)   =   " Benutzer"
      TabPicture(1)   =   "propertynew.frx":065C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Text1"
      Tab(1).Control(1)=   "Text2"
      Tab(1).Control(2)=   "Label7"
      Tab(1).Control(3)=   "Label8"
      Tab(1).ControlCount=   4
      TabCaption(2)   =   " Pfade"
      TabPicture(2)   =   "propertynew.frx":078C
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "pfad2(0)"
      Tab(2).Control(1)=   "pfad2(1)"
      Tab(2).Control(2)=   "pfad2(2)"
      Tab(2).Control(3)=   "pfad2(3)"
      Tab(2).Control(4)=   "pfad2(4)"
      Tab(2).Control(5)=   "pfad2(5)"
      Tab(2).Control(6)=   "durchsuchen(0)"
      Tab(2).Control(7)=   "durchsuchen(1)"
      Tab(2).Control(8)=   "durchsuchen(2)"
      Tab(2).Control(9)=   "durchsuchen(3)"
      Tab(2).Control(10)=   "durchsuchen(4)"
      Tab(2).Control(11)=   "durchsuchen(5)"
      Tab(2).Control(12)=   "pfad2(6)"
      Tab(2).Control(13)=   "durchsuchen(6)"
      Tab(2).Control(14)=   "pfad1(0)"
      Tab(2).Control(15)=   "pfad1(1)"
      Tab(2).Control(16)=   "pfad1(2)"
      Tab(2).Control(17)=   "pfad1(3)"
      Tab(2).Control(18)=   "pfad1(4)"
      Tab(2).Control(19)=   "pfad1(5)"
      Tab(2).Control(20)=   "Label13"
      Tab(2).ControlCount=   21
      TabCaption(3)   =   " System"
      TabPicture(3)   =   "propertynew.frx":0ADE
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "checksonstiges(0)"
      Tab(3).Control(1)=   "checksonstiges(1)"
      Tab(3).Control(2)=   "checksonstiges(2)"
      Tab(3).Control(3)=   "druckerauswahl"
      Tab(3).Control(4)=   "farbe"
      Tab(3).Control(5)=   "sprachauswahl"
      Tab(3).Control(6)=   "waehrungsfeld"
      Tab(3).Control(7)=   "system1(0)"
      Tab(3).Control(8)=   "system1(2)"
      Tab(3).Control(9)=   "system1(4)"
      Tab(3).ControlCount=   10
      TabCaption(4)   =   " Datenbank"
      TabPicture(4)   =   "propertynew.frx":0C26
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "Frame1"
      Tab(4).ControlCount=   1
      Begin VB.Frame Frame1 
         Caption         =   "Externe MySQL-Datenbank"
         Height          =   2955
         Left            =   -74880
         TabIndex        =   45
         Top             =   480
         Width           =   5595
         Begin VB.TextBox db_password 
            Height          =   315
            Left            =   960
            TabIndex        =   55
            Top             =   2220
            Width           =   2115
         End
         Begin VB.TextBox db_user 
            Height          =   315
            Left            =   960
            TabIndex        =   54
            Top             =   1800
            Width           =   2115
         End
         Begin VB.TextBox db_name 
            Height          =   315
            Left            =   1500
            TabIndex        =   53
            Top             =   1200
            Width           =   3495
         End
         Begin VB.TextBox db_port 
            Height          =   315
            Left            =   1020
            TabIndex        =   49
            Top             =   780
            Width           =   975
         End
         Begin VB.TextBox db_host 
            Height          =   285
            Left            =   1020
            TabIndex        =   47
            Top             =   420
            Width           =   3975
         End
         Begin VB.Label Label14 
            AutoSize        =   -1  'True
            Caption         =   "Passwort:"
            Height          =   195
            Left            =   120
            TabIndex        =   52
            Top             =   2280
            Width           =   690
         End
         Begin VB.Label Label12 
            AutoSize        =   -1  'True
            Caption         =   "Username:"
            Height          =   195
            Left            =   120
            TabIndex        =   51
            Top             =   1860
            Width           =   765
         End
         Begin VB.Label Label11 
            AutoSize        =   -1  'True
            Caption         =   "Datenbank-Name:"
            Height          =   195
            Left            =   120
            TabIndex        =   50
            Top             =   1260
            Width           =   1305
         End
         Begin VB.Label Label10 
            AutoSize        =   -1  'True
            Caption         =   "Port:"
            Height          =   195
            Left            =   120
            TabIndex        =   48
            Top             =   840
            Width           =   330
         End
         Begin VB.Label Label9 
            AutoSize        =   -1  'True
            Caption         =   "Hostname:"
            Height          =   195
            Left            =   120
            TabIndex        =   46
            Top             =   420
            Width           =   765
         End
      End
      Begin VB.CheckBox checksonstiges 
         Caption         =   "Automatischer Interpretenvorschlag"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Index           =   0
         Left            =   -74880
         TabIndex        =   44
         Top             =   2460
         Width           =   4515
      End
      Begin VB.CheckBox checksonstiges 
         Caption         =   "Automatischer Titelvorschlag"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Index           =   1
         Left            =   -74880
         TabIndex        =   43
         Top             =   2820
         Width           =   4515
      End
      Begin VB.CheckBox checksonstiges 
         Caption         =   "Startfenster anzeigen"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Index           =   2
         Left            =   -74880
         TabIndex        =   42
         Top             =   3180
         Width           =   4155
      End
      Begin VB.ComboBox druckerauswahl 
         Height          =   315
         Left            =   -73320
         TabIndex        =   38
         Text            =   "Combo1"
         Top             =   420
         Width           =   3315
      End
      Begin VB.CheckBox farbe 
         Caption         =   "Farbig drucken"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   -74880
         TabIndex        =   37
         Top             =   900
         Width           =   2595
      End
      Begin VB.ComboBox sprachauswahl 
         Height          =   315
         Left            =   -73320
         TabIndex        =   36
         Text            =   "Combo1"
         Top             =   1380
         Width           =   3315
      End
      Begin VB.TextBox waehrungsfeld 
         Height          =   315
         Left            =   -73320
         TabIndex        =   35
         Text            =   "Text4"
         Top             =   1800
         Width           =   855
      End
      Begin VB.TextBox pfad2 
         Height          =   285
         Index           =   0
         Left            =   -72600
         TabIndex        =   27
         Text            =   "Text4"
         Top             =   900
         Width           =   2775
      End
      Begin VB.TextBox pfad2 
         Height          =   285
         Index           =   1
         Left            =   -72600
         TabIndex        =   26
         Text            =   "Text4"
         Top             =   1260
         Width           =   2775
      End
      Begin VB.TextBox pfad2 
         Height          =   285
         Index           =   2
         Left            =   -72600
         TabIndex        =   25
         Text            =   "Text4"
         Top             =   1620
         Width           =   2775
      End
      Begin VB.TextBox pfad2 
         Height          =   285
         Index           =   3
         Left            =   -72600
         TabIndex        =   24
         Text            =   "Text4"
         Top             =   1980
         Width           =   2775
      End
      Begin VB.TextBox pfad2 
         Height          =   285
         Index           =   4
         Left            =   -72600
         TabIndex        =   23
         Text            =   "Text4"
         Top             =   2340
         Width           =   2775
      End
      Begin VB.TextBox pfad2 
         Height          =   285
         Index           =   5
         Left            =   -72600
         TabIndex        =   22
         Text            =   "Text4"
         Top             =   2700
         Width           =   2775
      End
      Begin VB.CommandButton durchsuchen 
         Height          =   315
         Index           =   0
         Left            =   -69720
         Picture         =   "propertynew.frx":0C42
         Style           =   1  'Grafisch
         TabIndex        =   21
         Top             =   900
         Width           =   375
      End
      Begin VB.CommandButton durchsuchen 
         Height          =   315
         Index           =   1
         Left            =   -69720
         Picture         =   "propertynew.frx":0D2C
         Style           =   1  'Grafisch
         TabIndex        =   20
         Top             =   1260
         Width           =   375
      End
      Begin VB.CommandButton durchsuchen 
         Height          =   315
         Index           =   2
         Left            =   -69720
         Picture         =   "propertynew.frx":0E16
         Style           =   1  'Grafisch
         TabIndex        =   19
         Top             =   1620
         Width           =   375
      End
      Begin VB.CommandButton durchsuchen 
         Height          =   315
         Index           =   3
         Left            =   -69720
         Picture         =   "propertynew.frx":0F00
         Style           =   1  'Grafisch
         TabIndex        =   18
         Top             =   1980
         Width           =   375
      End
      Begin VB.CommandButton durchsuchen 
         Height          =   315
         Index           =   4
         Left            =   -69720
         Picture         =   "propertynew.frx":0FEA
         Style           =   1  'Grafisch
         TabIndex        =   17
         Top             =   2340
         Width           =   375
      End
      Begin VB.CommandButton durchsuchen 
         Height          =   315
         Index           =   5
         Left            =   -69720
         Picture         =   "propertynew.frx":10D4
         Style           =   1  'Grafisch
         TabIndex        =   16
         Top             =   2700
         Width           =   375
      End
      Begin VB.TextBox pfad2 
         Height          =   285
         Index           =   6
         Left            =   -72600
         TabIndex        =   15
         Text            =   "Text4"
         Top             =   3060
         Width           =   2775
      End
      Begin VB.CommandButton durchsuchen 
         Height          =   315
         Index           =   6
         Left            =   -69720
         Picture         =   "propertynew.frx":11BE
         Style           =   1  'Grafisch
         TabIndex        =   14
         Top             =   3060
         Width           =   375
      End
      Begin VB.TextBox Text1 
         Height          =   300
         Left            =   -73305
         TabIndex        =   11
         Text            =   "Text1"
         Top             =   960
         Width           =   3495
      End
      Begin VB.TextBox Text2 
         Height          =   300
         Left            =   -73305
         TabIndex        =   10
         Text            =   "Text2"
         Top             =   1440
         Width           =   3495
      End
      Begin VB.Label system1 
         AutoSize        =   -1  'True
         Caption         =   "Drucker:"
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
         Index           =   0
         Left            =   -74880
         TabIndex        =   41
         Top             =   480
         Width           =   750
      End
      Begin VB.Label system1 
         AutoSize        =   -1  'True
         Caption         =   "Sprache:"
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
         Index           =   2
         Left            =   -74880
         TabIndex        =   40
         Top             =   1440
         Width           =   780
      End
      Begin VB.Label system1 
         AutoSize        =   -1  'True
         Caption         =   "Währung:"
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
         Index           =   4
         Left            =   -74880
         TabIndex        =   39
         Top             =   1860
         Width           =   840
      End
      Begin VB.Label pfad1 
         AutoSize        =   -1  'True
         Caption         =   "WinMusik Programmpfad:"
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
         Index           =   0
         Left            =   -74880
         TabIndex        =   34
         Top             =   900
         Width           =   2160
      End
      Begin VB.Label pfad1 
         AutoSize        =   -1  'True
         Caption         =   "Datenpfad:"
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
         Index           =   1
         Left            =   -74880
         TabIndex        =   33
         Top             =   1260
         Width           =   960
      End
      Begin VB.Label pfad1 
         AutoSize        =   -1  'True
         Caption         =   "Temporäres Verzeichnis:"
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
         Index           =   2
         Left            =   -74880
         TabIndex        =   32
         Top             =   1620
         Width           =   2100
      End
      Begin VB.Label pfad1 
         AutoSize        =   -1  'True
         Caption         =   "Titeldatei:"
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
         Index           =   3
         Left            =   -74880
         TabIndex        =   31
         Top             =   1980
         Width           =   870
      End
      Begin VB.Label pfad1 
         AutoSize        =   -1  'True
         Caption         =   "CD-Header Datei:"
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
         Index           =   4
         Left            =   -74880
         TabIndex        =   30
         Top             =   2340
         Width           =   1515
      End
      Begin VB.Label pfad1 
         AutoSize        =   -1  'True
         Caption         =   "Pfad für MPEG-Dateien:"
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
         Index           =   5
         Left            =   -74880
         TabIndex        =   29
         Top             =   2700
         Width           =   2055
      End
      Begin VB.Label Label13 
         AutoSize        =   -1  'True
         Caption         =   "Pfad für CD-Ripper:"
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
         Left            =   -74880
         TabIndex        =   28
         Top             =   3060
         Width           =   1680
      End
      Begin VB.Label Label7 
         Caption         =   "Name:"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   -74880
         TabIndex        =   13
         Top             =   1035
         Width           =   975
      End
      Begin VB.Label Label8 
         Caption         =   "Firma:"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   -74880
         TabIndex        =   12
         Top             =   1515
         Width           =   1095
      End
      Begin VB.Image Image1 
         BorderStyle     =   1  'Fest Einfach
         Height          =   2580
         Left            =   120
         Picture         =   "propertynew.frx":12A8
         Top             =   780
         Width           =   3015
      End
      Begin VB.Label Label1 
         Alignment       =   2  'Zentriert
         Caption         =   "WinMusik 2.0"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   3240
         TabIndex        =   6
         Top             =   900
         Width           =   2295
      End
      Begin VB.Label Label2 
         Alignment       =   2  'Zentriert
         Caption         =   "(c) Copyright by Patrick F.-Productions in 1996-98"
         Height          =   255
         Left            =   120
         TabIndex        =   5
         Top             =   3420
         Width           =   5415
      End
      Begin VB.Label Label3 
         Caption         =   "Released:"
         Height          =   255
         Left            =   3240
         TabIndex        =   4
         Top             =   1380
         Width           =   975
      End
      Begin VB.Label Label4 
         Caption         =   "Licensed to:"
         Height          =   255
         Left            =   3240
         TabIndex        =   3
         Top             =   2220
         Width           =   1695
      End
      Begin VB.Label Label5 
         Caption         =   "23.05.1998"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   3240
         TabIndex        =   2
         Top             =   1620
         Width           =   1695
      End
      Begin VB.Label Label6 
         Caption         =   "Patrick Fedick, Patrick F.-Productions"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   855
         Left            =   3240
         TabIndex        =   1
         Top             =   2460
         Width           =   2295
      End
   End
End
Attribute VB_Name = "propertynew"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()
Caption = Programm + " " + version + ": " + Lang(10)
End Sub



VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form property1 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "Form1"
   ClientHeight    =   13245
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   14295
   Icon            =   "property1.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   13245
   ScaleWidth      =   14295
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows-Standard
   Begin VB.Frame Frame 
      Height          =   3015
      Index           =   5
      Left            =   7380
      TabIndex        =   19
      Top             =   6480
      Width           =   5655
      Begin VB.TextBox port 
         Height          =   315
         Left            =   4980
         TabIndex        =   64
         Top             =   2100
         Width           =   495
      End
      Begin VB.TextBox proxy 
         Height          =   285
         Left            =   2760
         TabIndex        =   62
         Top             =   2100
         Width           =   1815
      End
      Begin VB.CommandButton update 
         Caption         =   "Update"
         Height          =   375
         Left            =   3360
         TabIndex        =   59
         Top             =   2520
         Visible         =   0   'False
         Width           =   1455
      End
      Begin VB.Label Label12 
         Caption         =   "Port:"
         Height          =   195
         Left            =   4620
         TabIndex        =   63
         Top             =   2160
         Width           =   315
      End
      Begin VB.Label Label9 
         AutoSize        =   -1  'True
         Caption         =   "Proxy:"
         Height          =   195
         Left            =   2280
         TabIndex        =   61
         Top             =   2160
         Width           =   435
      End
      Begin VB.Label Label11 
         Caption         =   "Label11"
         Height          =   1215
         Left            =   2280
         TabIndex        =   58
         Top             =   840
         Width           =   3075
         WordWrap        =   -1  'True
      End
      Begin VB.Label Label10 
         Caption         =   "Label10"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   735
         Left            =   2280
         TabIndex        =   57
         Top             =   180
         Width           =   3255
         WordWrap        =   -1  'True
      End
      Begin VB.Image Image2 
         Height          =   3000
         Left            =   0
         Picture         =   "property1.frx":030A
         Top             =   0
         Width           =   2250
      End
   End
   Begin VB.Frame Frame 
      Height          =   3075
      Index           =   6
      Left            =   5160
      TabIndex        =   20
      Top             =   9600
      Width           =   5655
      Begin VB.Frame Frame1 
         Caption         =   "MySQL-Datenbank"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1515
         Left            =   120
         TabIndex        =   68
         Top             =   1440
         Width           =   5415
         Begin VB.TextBox db_host 
            Height          =   285
            Left            =   960
            TabIndex        =   69
            Top             =   240
            Width           =   3075
         End
         Begin VB.TextBox db_port 
            Height          =   315
            Left            =   4560
            TabIndex        =   71
            Top             =   240
            Width           =   735
         End
         Begin VB.TextBox db_name 
            Height          =   315
            Left            =   1500
            TabIndex        =   73
            Top             =   600
            Width           =   3795
         End
         Begin VB.TextBox db_user 
            Height          =   315
            Left            =   960
            TabIndex        =   75
            Top             =   960
            Width           =   1755
         End
         Begin VB.TextBox db_password 
            Height          =   315
            Left            =   3660
            TabIndex        =   77
            Top             =   960
            Width           =   1635
         End
         Begin VB.Label Label18 
            AutoSize        =   -1  'True
            Caption         =   "Hostname:"
            Height          =   195
            Left            =   120
            TabIndex        =   78
            Top             =   300
            Width           =   765
         End
         Begin VB.Label Label17 
            AutoSize        =   -1  'True
            Caption         =   "Port:"
            Height          =   195
            Left            =   4140
            TabIndex        =   76
            Top             =   300
            Width           =   330
         End
         Begin VB.Label Label16 
            AutoSize        =   -1  'True
            Caption         =   "Datenbank-Name:"
            Height          =   195
            Left            =   120
            TabIndex        =   74
            Top             =   660
            Width           =   1305
         End
         Begin VB.Label Label15 
            AutoSize        =   -1  'True
            Caption         =   "Username:"
            Height          =   195
            Left            =   120
            TabIndex        =   72
            Top             =   1020
            Width           =   765
         End
         Begin VB.Label Label14 
            AutoSize        =   -1  'True
            Caption         =   "Passwort:"
            Height          =   195
            Left            =   2940
            TabIndex        =   70
            Top             =   1020
            Width           =   690
         End
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
         Left            =   120
         TabIndex        =   60
         Top             =   1020
         Width           =   4155
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
         Left            =   120
         TabIndex        =   56
         Top             =   660
         Width           =   4515
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
         Left            =   120
         TabIndex        =   55
         Top             =   300
         Width           =   4515
      End
   End
   Begin VB.Frame Frame 
      Height          =   3015
      Index           =   4
      Left            =   120
      TabIndex        =   18
      Top             =   7560
      Width           =   5655
      Begin VB.CommandButton multimedia3 
         Height          =   315
         Index           =   2
         Left            =   5160
         Picture         =   "property1.frx":1646C
         Style           =   1  'Grafisch
         TabIndex        =   54
         Top             =   1320
         Width           =   375
      End
      Begin VB.CommandButton multimedia3 
         Height          =   315
         Index           =   1
         Left            =   5160
         Picture         =   "property1.frx":16556
         Style           =   1  'Grafisch
         TabIndex        =   53
         Top             =   900
         Width           =   375
      End
      Begin VB.TextBox multimedia2 
         Height          =   315
         Index           =   2
         Left            =   1860
         TabIndex        =   52
         Text            =   "Text5"
         Top             =   1320
         Width           =   3195
      End
      Begin VB.TextBox multimedia2 
         Height          =   315
         Index           =   1
         Left            =   1860
         TabIndex        =   51
         Text            =   "Text5"
         Top             =   900
         Width           =   3195
      End
      Begin VB.CommandButton multimedia3 
         Height          =   315
         Index           =   0
         Left            =   5160
         Picture         =   "property1.frx":16640
         Style           =   1  'Grafisch
         TabIndex        =   48
         Top             =   480
         Width           =   375
      End
      Begin VB.TextBox multimedia2 
         Height          =   315
         Index           =   0
         Left            =   1860
         TabIndex        =   47
         Text            =   "Text5"
         Top             =   480
         Width           =   3195
      End
      Begin VB.Label multimedia 
         AutoSize        =   -1  'True
         Caption         =   "WAV-Player:"
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
         Left            =   240
         TabIndex        =   50
         Top             =   1380
         Width           =   1080
      End
      Begin VB.Label multimedia 
         AutoSize        =   -1  'True
         Caption         =   "MPEG-Player:"
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
         Left            =   240
         TabIndex        =   49
         Top             =   960
         Width           =   1185
      End
      Begin VB.Label multimedia 
         AutoSize        =   -1  'True
         Caption         =   "Lautstärkeregler:"
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
         Left            =   240
         TabIndex        =   46
         Top             =   540
         Width           =   1455
      End
   End
   Begin VB.Frame Frame 
      Height          =   3015
      Index           =   3
      Left            =   120
      TabIndex        =   17
      Top             =   4440
      Width           =   5655
      Begin VB.TextBox waehrungsfeld 
         Height          =   315
         Left            =   1860
         TabIndex        =   45
         Text            =   "Text4"
         Top             =   1980
         Width           =   855
      End
      Begin VB.ComboBox sprachauswahl 
         Height          =   315
         Left            =   1860
         TabIndex        =   44
         Text            =   "Combo1"
         Top             =   1560
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
         Left            =   300
         TabIndex        =   43
         Top             =   900
         Width           =   2595
      End
      Begin VB.ComboBox druckerauswahl 
         Height          =   315
         Left            =   1860
         TabIndex        =   42
         Text            =   "Combo1"
         Top             =   420
         Width           =   3315
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
         Left            =   300
         TabIndex        =   41
         Top             =   2040
         Width           =   840
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
         Left            =   300
         TabIndex        =   40
         Top             =   1620
         Width           =   780
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
         Left            =   300
         TabIndex        =   39
         Top             =   480
         Width           =   750
      End
   End
   Begin VB.Frame Frame 
      Height          =   3015
      Index           =   2
      Left            =   240
      TabIndex        =   16
      Top             =   720
      Width           =   5655
      Begin VB.CommandButton durchsuchen 
         Height          =   315
         Index           =   6
         Left            =   5280
         Picture         =   "property1.frx":1672A
         Style           =   1  'Grafisch
         TabIndex        =   67
         Top             =   2520
         Width           =   375
      End
      Begin VB.TextBox pfad2 
         Height          =   285
         Index           =   6
         Left            =   2400
         TabIndex        =   66
         Text            =   "Text4"
         Top             =   2520
         Width           =   2775
      End
      Begin VB.CommandButton durchsuchen 
         Height          =   315
         Index           =   5
         Left            =   5280
         Picture         =   "property1.frx":16814
         Style           =   1  'Grafisch
         TabIndex        =   38
         Top             =   2160
         Width           =   375
      End
      Begin VB.CommandButton durchsuchen 
         Height          =   315
         Index           =   4
         Left            =   5280
         Picture         =   "property1.frx":168FE
         Style           =   1  'Grafisch
         TabIndex        =   37
         Top             =   1800
         Width           =   375
      End
      Begin VB.CommandButton durchsuchen 
         Height          =   315
         Index           =   3
         Left            =   5280
         Picture         =   "property1.frx":169E8
         Style           =   1  'Grafisch
         TabIndex        =   36
         Top             =   1440
         Width           =   375
      End
      Begin VB.CommandButton durchsuchen 
         Height          =   315
         Index           =   2
         Left            =   5280
         Picture         =   "property1.frx":16AD2
         Style           =   1  'Grafisch
         TabIndex        =   35
         Top             =   1080
         Width           =   375
      End
      Begin VB.CommandButton durchsuchen 
         Height          =   315
         Index           =   1
         Left            =   5280
         Picture         =   "property1.frx":16BBC
         Style           =   1  'Grafisch
         TabIndex        =   34
         Top             =   720
         Width           =   375
      End
      Begin VB.CommandButton durchsuchen 
         Height          =   315
         Index           =   0
         Left            =   5280
         Picture         =   "property1.frx":16CA6
         Style           =   1  'Grafisch
         TabIndex        =   33
         Top             =   360
         Width           =   375
      End
      Begin VB.TextBox pfad2 
         Height          =   285
         Index           =   5
         Left            =   2400
         TabIndex        =   32
         Text            =   "Text4"
         Top             =   2160
         Width           =   2775
      End
      Begin VB.TextBox pfad2 
         Height          =   285
         Index           =   4
         Left            =   2400
         TabIndex        =   31
         Text            =   "Text4"
         Top             =   1800
         Width           =   2775
      End
      Begin VB.TextBox pfad2 
         Height          =   285
         Index           =   3
         Left            =   2400
         TabIndex        =   30
         Text            =   "Text4"
         Top             =   1440
         Width           =   2775
      End
      Begin VB.TextBox pfad2 
         Height          =   285
         Index           =   2
         Left            =   2400
         TabIndex        =   29
         Text            =   "Text4"
         Top             =   1080
         Width           =   2775
      End
      Begin VB.TextBox pfad2 
         Height          =   285
         Index           =   1
         Left            =   2400
         TabIndex        =   28
         Text            =   "Text4"
         Top             =   720
         Width           =   2775
      End
      Begin VB.TextBox pfad2 
         Height          =   285
         Index           =   0
         Left            =   2400
         TabIndex        =   27
         Text            =   "Text4"
         Top             =   360
         Width           =   2775
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
         Left            =   120
         TabIndex        =   65
         Top             =   2520
         Width           =   1680
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
         Left            =   120
         TabIndex        =   26
         Top             =   2160
         Width           =   2055
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
         Left            =   120
         TabIndex        =   25
         Top             =   1800
         Width           =   1515
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
         Left            =   120
         TabIndex        =   24
         Top             =   1440
         Width           =   870
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
         Left            =   120
         TabIndex        =   23
         Top             =   1080
         Width           =   2100
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
         Left            =   120
         TabIndex        =   22
         Top             =   720
         Width           =   960
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
         Left            =   120
         TabIndex        =   21
         Top             =   360
         Width           =   2160
      End
   End
   Begin VB.Frame Frame 
      Height          =   3015
      Index           =   1
      Left            =   6720
      TabIndex        =   11
      Top             =   3360
      Width           =   5655
      Begin VB.TextBox Text2 
         Height          =   300
         Left            =   1815
         TabIndex        =   15
         Text            =   "Text2"
         Top             =   765
         Width           =   3495
      End
      Begin VB.TextBox Text1 
         Height          =   300
         Left            =   1815
         TabIndex        =   14
         Text            =   "Text1"
         Top             =   285
         Width           =   3495
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
         Left            =   240
         TabIndex        =   13
         Top             =   840
         Width           =   1095
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
         Left            =   240
         TabIndex        =   12
         Top             =   360
         Width           =   975
      End
   End
   Begin VB.Frame Frame 
      Height          =   3015
      Index           =   0
      Left            =   6720
      TabIndex        =   4
      Top             =   120
      Width           =   5655
      Begin VB.Label Label20 
         Caption         =   "?"
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
         Left            =   4320
         TabIndex        =   80
         Top             =   1200
         Width           =   1095
      End
      Begin VB.Label Label19 
         AutoSize        =   -1  'True
         Caption         =   "DLL-Version:"
         Height          =   195
         Left            =   3240
         TabIndex        =   79
         Top             =   1200
         Width           =   915
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
         TabIndex        =   10
         Top             =   1800
         Width           =   2295
      End
      Begin VB.Label Label5 
         Caption         =   "20.04.2009"
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
         TabIndex        =   9
         Top             =   960
         Width           =   1695
      End
      Begin VB.Label Label4 
         Caption         =   "Licensed to:"
         Height          =   255
         Left            =   3240
         TabIndex        =   8
         Top             =   1560
         Width           =   1695
      End
      Begin VB.Label Label3 
         Caption         =   "Released:"
         Height          =   255
         Left            =   3240
         TabIndex        =   7
         Top             =   720
         Width           =   975
      End
      Begin VB.Label Label2 
         Alignment       =   2  'Zentriert
         Caption         =   "(c) Copyright by Patrick F.-Productions in 1996-2009"
         Height          =   255
         Left            =   120
         TabIndex        =   6
         Top             =   2760
         Width           =   5415
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
         TabIndex        =   5
         Top             =   240
         Width           =   2295
      End
      Begin VB.Image Image1 
         BorderStyle     =   1  'Fest Einfach
         Height          =   2580
         Left            =   120
         Picture         =   "property1.frx":16D90
         Top             =   120
         Width           =   3015
      End
   End
   Begin VB.CommandButton uebernehmen 
      Caption         =   "Übernehmen"
      Height          =   375
      Left            =   4680
      TabIndex        =   3
      Top             =   3960
      Width           =   1335
   End
   Begin VB.CommandButton abbrechen 
      Caption         =   "Abbrechen"
      Height          =   375
      Left            =   2280
      TabIndex        =   2
      Top             =   3960
      Width           =   1335
   End
   Begin VB.CommandButton ok 
      Caption         =   "OK"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   3960
      Width           =   975
   End
   Begin MSComctlLib.TabStrip tab1 
      Height          =   3855
      Left            =   120
      TabIndex        =   0
      Top             =   0
      Width           =   5895
      _ExtentX        =   10398
      _ExtentY        =   6800
      MultiRow        =   -1  'True
      ImageList       =   "ImageList1"
      _Version        =   393216
      BeginProperty Tabs {1EFB6598-857C-11D1-B16A-00C0F0283628} 
         NumTabs         =   7
         BeginProperty Tab1 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "WinMusik"
            ImageVarType    =   2
            ImageIndex      =   1
         EndProperty
         BeginProperty Tab2 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "Benutzer"
            ImageVarType    =   2
            ImageIndex      =   2
         EndProperty
         BeginProperty Tab3 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "Pfade"
            ImageVarType    =   2
            ImageIndex      =   3
         EndProperty
         BeginProperty Tab4 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "System"
            ImageVarType    =   2
            ImageIndex      =   4
         EndProperty
         BeginProperty Tab5 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "Multimedia"
            ImageVarType    =   2
            ImageIndex      =   5
         EndProperty
         BeginProperty Tab6 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "Update"
            ImageVarType    =   2
            ImageIndex      =   6
         EndProperty
         BeginProperty Tab7 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "Sonstiges"
            ImageVarType    =   2
            ImageIndex      =   7
         EndProperty
      EndProperty
      MouseIcon       =   "property1.frx":2F252
   End
   Begin MSComctlLib.ImageList ImageList1 
      Left            =   5940
      Top             =   5880
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483648
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   7
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "property1.frx":2F56C
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "property1.frx":2F8BE
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "property1.frx":2F9D0
            Key             =   ""
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "property1.frx":2FD22
            Key             =   ""
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "property1.frx":2FE34
            Key             =   ""
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "property1.frx":2FF46
            Key             =   ""
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "property1.frx":30298
            Key             =   ""
         EndProperty
      EndProperty
   End
End
Attribute VB_Name = "property1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
DefInt A-Z
Private mintCurFrame As Integer ' Aktueller Rahmen sichtbar
Private Sub SaveINI()
openini AppData + "winmusik2.ini"
If inierror() > 0 Then
  CreateINI AppData + "winmusik2.ini", Programm + " " + version + ", " + Datum$() + ", " + Time$
  selectTopic "Programm"
  putini "Installationsdatum=" + Datum$() + ", " + Time$
End If
selectTopic "Programm"
putini "Benutzer=" + user
putini "Firma=" + UserFirma
putini "Letzter Aufruf=" + Datum$() + ", " + Time$
putini "Währung=" + Waehrung
putini "Drucker=" + drucker
putini "Farbdruck=" + Str$(Farbdruck)
putini "Sprache=" + LangName()
AutoInterpret = checksonstiges(0).value
AutoTitel = checksonstiges(1).value
putini "AutoInterpret=" + Str$(AutoInterpret)
putini "AutoTitel=" + Str$(AutoTitel)
putini "ShowSplashScreen=" + Str$(ShowSplashScreen)
putini "InetProxy=" + InetProxy
putini "InetPort=" + InetPort
putini "RequireSetup=false"

selectTopic "Pfade"
putini "Main=" + HauptPfad
putini "Titeldatei=" + TiDatei
putini "CDHeaderDatei=" + CDHeaderDatei
putini "Temp=" + tmp
putini "Daten=" + DatenPfad
putini "Sprachdatei=" + LangFile()
putini "MPEG=" + MpegPfad
putini "CDRipper=" + CDRipperPfad

selectTopic "Programms"
putini "WAVEPlayer=" + WAVEPlayer
putini "MPEGPlayer=" + MPEGPlayer
putini "Mixer=" + MixerProg

selectTopic "Database"
putini "Host=" + db.host
putini "Port=" + Format(db.port)
putini "Name=" + db.name
putini "User=" + db.user
putini "Password=" + db.password

closeini
End Sub

Sub SaveProperties()
Dim tmpstr As String * 256
user = Trim(Text1.text)
UserFirma = Trim(Text2.text)
Waehrung = Trim(waehrungsfeld.text)
drucker = Trim(druckerauswahl.text)
Farbdruck = farbe.value
RequireSetup = False

AutoInterpret = checksonstiges(0).value
AutoTitel = checksonstiges(1).value
ShowSplashScreen = checksonstiges(2).value

HauptPfad = Trim(pfad2(0).text)
DatenPfad = Trim(pfad2(1).text)
tmp = Trim(pfad2(2).text)
TiDatei = Trim(pfad2(3).text)
CDHeaderDatei = Trim(pfad2(4).text)
MpegPfad = Trim(pfad2(5).text)
CDRipperPfad = Trim(pfad2(6).text)

WAVEPlayer = Trim(multimedia2(0).text)
MPEGPlayer = Trim(multimedia2(1).text)
MixerProg = Trim(multimedia2(2).text)
InetProxy = proxy.text
InetPort = port.text

db.host = Trim(db_host.text)
db.port = Val(Trim(db_port.text))
db.name = Trim(db_name.text)
db.user = Trim(db_user.text)
db.password = Trim(db_password.text)



oldpfad$ = CurDir

'Sprache ermitteln
SprachDatei = LangFile()
f$ = Dir$(HauptPfad + "\*.lng")
If f$ = "" Then
  MsgBox "Sprachdatei nicht gefunden!" + Chr$(13) + Chr$(13) + "Language-File not found!", 16, Programm + " " + version, HelpFile, 100
Else
  Do
    ff = FreeFile: f$ = HauptPfad + "\" + f$
    Open f$ For Binary As #ff
    If Err = 0 Then
      Get #ff, 1, pfp
      If pfp.pfpID = "PFP-File" And pfp.FileID = "LANG" Then
        Get #ff, 47, p
        Get #ff, p + 1, tmpstr
        n = InStr(tmpstr, Chr$(0))
        If left$(tmpstr, n - 1) = Trim$(sprachauswahl.text) Then
          SprachDatei = f$
          INITLanguage SprachDatei
          Close #ff
          Exit Do
        End If
      End If
    Else
      MsgBox "Sprachdatei nicht gefunden!" + Chr$(13) + Chr$(13) + "Language-File not found!", 16, Programm + " " + version, HelpFile, 101
    End If
    Close #ff
    f$ = Dir$
  Loop Until f$ = ""
End If
ChDir HauptPfad
If Err = 0 Then
  CreatePath DatenPfad + "\"
  If Err = 0 Then
    CreatePath TiDatei
    If Err = 0 Then
      CreatePath CDHeaderDatei
      If Err = 0 Then
        CreatePath tmp + "\"
        If Err = 0 Then
          CreatePath MpegPfad + "\"
          If Err = 0 Then
            SaveINI
            Unload property1
            Main
          End If
        Else
          MsgBox Lang(25), 64, Programm + " " + version
        End If
      Else
        MsgBox Lang(26), 64, Programm + " " + version
      End If
    Else
      MsgBox Lang(27), 64, Programm + " " + version
    End If
  Else
    MsgBox Lang(28), 64, Programm + " " + version
  End If
Else
  HauptPfad = oldpfad$: MsgBox Lang(29), 64, Programm + " " + version
End If
ChDir HauptPfad
If Err > 0 Then ChDir oldpfad$

End Sub


Private Sub abbrechen_Click()
Unload property1
End Sub

Private Sub Form_Load()
Dim lpt As Printer
Dim tmpstr As String * 256
Dim major As Long, minor As Long, release As Long
Dim ret As Long

'Felder ausfüllen
openini AppData + "winmusik2.ini"
If inierror() > 0 Then
  openini "win.ini"
  selectTopic "MS User Info"
  user = Trim$(GetINI$("DefName"))
  UserFirma = Trim$(GetINI$("DefCompany"))
  selectTopic "intl"
  Waehrung$ = GetINI$("sCurrency")
  HauptPfad = App.Path
  TiDatei = AppData + "winmusik\titel20.dat"
  CDHeaderDatei = AppData + "winmusik\cdheader.dat"
  DatenPfad = AppData + "winmusik"
  MpegPfad = AppData + "winmusik"
  MPEGPlayer = ""
  WAVEPlayer = WindowsPfad() + "\mplayer.exe"
  MixerProg = WindowsPfad() + "\sndvol32.exe"
  a$ = Environ$("TMP")
  If a$ = "" Then a$ = Environ$("TEMP")
  If a$ = "" Then a$ = CurDir + "\tmp"
  tmp = a$
  
 Else
  ReadINI
End If

Caption = Programm + " " + version + ": " + Lang(10)
Width = 6315
Height = 4785
Label1.Caption = Programm + " " + version + " Build " + Format(build)
Label2.Caption = copyright
Label5.Caption = Progdate
Label6.Caption = user + Chr$(13) + UserFirma
Text1.text = user
Text2.text = UserFirma


ret = WMLibVersion(major, minor, release)

Label20.Caption = Format(major) + "." + Format(minor)


For i = 0 To 6
  Frame(i).BorderStyle = 0
  Frame(i).left = 240: Frame(i).top = 720
  Frame(i).Visible = False
Next
Frame(0).Visible = True
Set tab1.ImageList = ImageList1
Show
DoEvents

Label10.Caption = Lang(208)
'Label11.Caption = Lang(209)
Label11.Caption = Lang(239)
Label9.Caption = Lang(225)
Label12.Caption = Lang(226)

update.Caption = Lang(207)
pfad2(0).text = HauptPfad
pfad2(1).text = DatenPfad
pfad2(2).text = tmp
pfad2(3).text = TiDatei
pfad2(4).text = CDHeaderDatei
pfad2(5).text = MpegPfad
pfad2(6).text = CDRipperPfad

'Verfügbare Drucker in das Listfeld eintragen
For Each lpt In Printers
  druckerauswahl.AddItem lpt.DeviceName
Next
druckerauswahl.text = Lang(238)
On Error Resume Next
druckerauswahl.text = Printer.DeviceName
farbe.value = Abs(Farbdruck)

'Sprachen anzeigen
f$ = Dir$(HauptPfad + "\*.lng")
If f$ = "" Then
  MsgBox "Sprachdatei nicht gefunden!" + Chr$(13) + Chr$(13) + "Language-File not found!", 16, Programm + " " + version, HelpFile, 100
Else
  Do
    ff = FreeFile: f$ = HauptPfad + "\" + f$
    Err = 0
    Open f$ For Binary As #ff
    If Err = 0 Then
      Get #ff, 1, pfp
      If pfp.pfpID = "PFP-File" And pfp.FileID = "LANG" Then
        Get #ff, 47, p
        Get #ff, p + 1, tmpstr
        n = InStr(tmpstr, Chr$(0))
        sprachauswahl.AddItem left$(tmpstr, n - 1)
      End If
    Else
      MsgBox "Sprachdatei nicht gefunden!" + Chr$(13) + Chr$(13) + "Language-File not found!", 16, Programm + " " + version, HelpFile, 101
    End If
    Close #ff
    f$ = Dir$
  Loop Until f$ = ""
End If
sprachauswahl.text = LangName()
waehrungsfeld.text = Waehrung$
checksonstiges(0).Caption = Lang(215)
checksonstiges(1).Caption = Lang(216)
checksonstiges(2).Caption = Lang(224)

checksonstiges(0).value = Abs(AutoInterpret)
checksonstiges(1).value = Abs(AutoTitel)
checksonstiges(2).value = Abs(ShowSplashScreen)

multimedia2(0).text = WAVEPlayer
multimedia2(1).text = MPEGPlayer
multimedia2(2).text = MixerProg

db_host = db.host
db_port = db.port
db_name = db.name
db_user = db.user
db_password = db.password


DoEvents
If RequireSetup = True Then
    MsgBox Lang(240), vbOKOnly + vbInformation, Lang(10)
End If
End Sub

Private Sub ok_Click()
SaveProperties
Unload property1

End Sub

Private Sub tab1_Click()
    If tab1.SelectedItem.Index = mintCurFrame _
        Then Exit Sub ' Rahmen muß nicht geändert
    ' werden. Andernfalls alten Rahmen ausblenden,
    ' neuen anzeigen.
    Frame(tab1.SelectedItem.Index - 1).Visible = True
    If mintCurFrame > 0 Then
      Frame(mintCurFrame - 1).Visible = False
    End If
    ' mintCurFrame auf neuen Wert setzen.
    mintCurFrame = tab1.SelectedItem.Index
End Sub

Private Sub uebernehmen_Click()
SaveProperties


End Sub

Private Sub update_Click()
SaveProperties
Unload property1
'inetupdate.Show

End Sub



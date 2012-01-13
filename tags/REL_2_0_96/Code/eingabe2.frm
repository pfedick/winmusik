VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCT2.OCX"
Begin VB.Form eingabe 
   Appearance      =   0  '2D
   Caption         =   "Eingabe"
   ClientHeight    =   7320
   ClientLeft      =   1590
   ClientTop       =   2070
   ClientWidth     =   9675
   ClipControls    =   0   'False
   BeginProperty Font 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H80000008&
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   7320
   ScaleWidth      =   9675
   Begin MSComctlLib.ProgressBar ProgressBar1 
      Height          =   195
      Left            =   5520
      TabIndex        =   85
      Top             =   6780
      Width           =   1275
      _ExtentX        =   2249
      _ExtentY        =   344
      _Version        =   393216
      Appearance      =   0
   End
   Begin MSComctlLib.StatusBar StatusBar1 
      Align           =   2  'Unten ausrichten
      Height          =   315
      Left            =   0
      TabIndex        =   84
      Top             =   7005
      Width           =   9675
      _ExtentX        =   17066
      _ExtentY        =   556
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   5
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            AutoSize        =   2
            Text            =   "WinMusik 2.1.32"
            TextSave        =   "WinMusik 2.1.32"
         EndProperty
         BeginProperty Panel2 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Object.Width           =   7056
            MinWidth        =   7056
         EndProperty
         BeginProperty Panel3 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Alignment       =   2
            AutoSize        =   1
            Object.Width           =   4700
            MinWidth        =   176
            Text            =   "0 %"
            TextSave        =   "0 %"
         EndProperty
         BeginProperty Panel4 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Style           =   2
            AutoSize        =   2
            Object.Width           =   1058
            MinWidth        =   1058
            TextSave        =   "NUM"
         EndProperty
         BeginProperty Panel5 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Style           =   5
            Alignment       =   1
            Object.Width           =   1058
            MinWidth        =   1058
            TextSave        =   "11:19"
         EndProperty
      EndProperty
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
   Begin VB.PictureBox ListPanel 
      Height          =   1545
      Left            =   60
      ScaleHeight     =   1485
      ScaleWidth      =   8895
      TabIndex        =   75
      Top             =   1200
      Width           =   8955
      Begin MSComCtl2.FlatScrollBar ListScroll 
         Height          =   1455
         Left            =   8640
         TabIndex        =   90
         Top             =   0
         Width           =   255
         _ExtentX        =   450
         _ExtentY        =   2566
         _Version        =   393216
         Appearance      =   0
         Orientation     =   1245184
      End
      Begin VB.CommandButton lAuswahl 
         Height          =   255
         Index           =   1
         Left            =   6210
         Picture         =   "eingabe2.frx":0000
         Style           =   1  'Grafisch
         TabIndex        =   83
         TabStop         =   0   'False
         Top             =   0
         Width           =   195
      End
      Begin VB.CommandButton lAuswahl 
         Height          =   255
         Index           =   0
         Left            =   6030
         Picture         =   "eingabe2.frx":0092
         Style           =   1  'Grafisch
         TabIndex        =   82
         TabStop         =   0   'False
         Top             =   0
         Width           =   195
      End
      Begin VB.CommandButton LTitel 
         Caption         =   "Interpret: Titel"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   540
         TabIndex        =   81
         TabStop         =   0   'False
         Top             =   0
         Width           =   5505
      End
      Begin VB.CommandButton LTrack 
         Caption         =   "Track"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   0
         TabIndex        =   80
         TabStop         =   0   'False
         Top             =   0
         Width           =   555
      End
      Begin VB.CommandButton LLaenge 
         Caption         =   "Länge"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   7950
         TabIndex        =   79
         TabStop         =   0   'False
         Top             =   0
         Width           =   705
      End
      Begin VB.CommandButton LVersion 
         Caption         =   "Version"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   6390
         TabIndex        =   78
         TabStop         =   0   'False
         Top             =   0
         Width           =   1575
      End
      Begin VB.PictureBox Listfeld 
         Appearance      =   0  '2D
         BackColor       =   &H80000005&
         BorderStyle     =   0  'Kein
         ClipControls    =   0   'False
         DragIcon        =   "eingabe2.frx":0124
         ForeColor       =   &H80000008&
         Height          =   1260
         Left            =   0
         ScaleHeight     =   1260
         ScaleWidth      =   8655
         TabIndex        =   76
         Top             =   240
         Width           =   8655
         Begin VB.TextBox quickinfo 
            Appearance      =   0  '2D
            BackColor       =   &H80000018&
            Height          =   255
            Left            =   1560
            Locked          =   -1  'True
            TabIndex        =   77
            Text            =   "Text1"
            Top             =   600
            Visible         =   0   'False
            Width           =   7035
         End
         Begin VB.Image QuickInfoPlay 
            Height          =   240
            Left            =   1320
            MouseIcon       =   "eingabe2.frx":0E1E
            MousePointer    =   99  'Benutzerdefiniert
            Picture         =   "eingabe2.frx":1128
            Top             =   600
            Visible         =   0   'False
            Width           =   240
         End
      End
   End
   Begin VB.Frame topframe 
      Height          =   1095
      Left            =   60
      TabIndex        =   62
      Top             =   60
      Width           =   8985
      Begin VB.TextBox index_e 
         Height          =   285
         Left            =   675
         TabIndex        =   0
         Text            =   "Text1"
         Top             =   150
         Width           =   975
      End
      Begin VB.TextBox seite_e 
         Height          =   285
         Left            =   2295
         MaxLength       =   1
         TabIndex        =   1
         Text            =   "Text1"
         Top             =   150
         Width           =   315
      End
      Begin VB.Image TraegerIcon 
         Appearance      =   0  '2D
         Height          =   480
         Left            =   8400
         Top             =   540
         Width           =   480
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Index:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   0
         Left            =   105
         TabIndex        =   74
         Top             =   180
         Width           =   540
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Seite:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   1
         Left            =   1740
         TabIndex        =   73
         Top             =   180
         Width           =   510
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Länge:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   2
         Left            =   2805
         TabIndex        =   72
         Top             =   180
         Width           =   600
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Min.   Rest:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   3
         Left            =   4200
         TabIndex        =   71
         Top             =   180
         Width           =   1005
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Min.   Seiten:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   4
         Left            =   6015
         TabIndex        =   70
         Top             =   180
         Width           =   1155
      End
      Begin VB.Label text 
         Alignment       =   1  'Rechts
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Titel:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   6
         Left            =   90
         TabIndex        =   69
         Top             =   525
         Width           =   450
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Tracks:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   5
         Left            =   7665
         TabIndex        =   68
         Top             =   180
         Width           =   660
      End
      Begin VB.Label panel2 
         BorderStyle     =   1  'Fest Einfach
         Height          =   285
         Index           =   0
         Left            =   3405
         TabIndex        =   67
         Top             =   150
         Width           =   675
      End
      Begin VB.Label panel2 
         BorderStyle     =   1  'Fest Einfach
         Height          =   285
         Index           =   1
         Left            =   5205
         TabIndex        =   66
         Top             =   150
         Width           =   675
      End
      Begin VB.Label panel2 
         BorderStyle     =   1  'Fest Einfach
         Height          =   285
         Index           =   2
         Left            =   7185
         TabIndex        =   65
         Top             =   150
         Width           =   405
      End
      Begin VB.Label panel2 
         BorderStyle     =   1  'Fest Einfach
         Height          =   285
         Index           =   3
         Left            =   8355
         TabIndex        =   64
         Top             =   150
         Width           =   495
      End
      Begin VB.Label TraegerTitel 
         BorderStyle     =   1  'Fest Einfach
         Height          =   555
         Left            =   585
         TabIndex        =   63
         Top             =   480
         Width           =   7755
      End
   End
   Begin VB.PictureBox functions 
      Height          =   435
      Left            =   60
      ScaleHeight     =   375
      ScaleWidth      =   8895
      TabIndex        =   49
      Top             =   6120
      Width           =   8955
      Begin VB.CommandButton fkey 
         Height          =   375
         Index           =   12
         Left            =   8280
         Style           =   1  'Grafisch
         TabIndex        =   23
         Top             =   0
         Width           =   615
      End
      Begin VB.CommandButton fkey 
         Height          =   375
         Index           =   11
         Left            =   7680
         Style           =   1  'Grafisch
         TabIndex        =   61
         Top             =   0
         Width           =   615
      End
      Begin VB.CommandButton fkey 
         Height          =   375
         Index           =   10
         Left            =   7080
         Style           =   1  'Grafisch
         TabIndex        =   60
         Top             =   0
         Width           =   615
      End
      Begin VB.CommandButton fkey 
         Height          =   375
         Index           =   9
         Left            =   6480
         Style           =   1  'Grafisch
         TabIndex        =   59
         Top             =   0
         Width           =   615
      End
      Begin VB.CommandButton fkey 
         Height          =   375
         Index           =   8
         Left            =   5580
         Style           =   1  'Grafisch
         TabIndex        =   58
         Top             =   0
         Width           =   615
      End
      Begin VB.CommandButton fkey 
         Height          =   375
         Index           =   7
         Left            =   4980
         Style           =   1  'Grafisch
         TabIndex        =   57
         Top             =   0
         Width           =   615
      End
      Begin VB.CommandButton fkey 
         Height          =   375
         Index           =   6
         Left            =   4380
         Style           =   1  'Grafisch
         TabIndex        =   56
         Top             =   0
         Width           =   615
      End
      Begin VB.CommandButton fkey 
         Height          =   375
         Index           =   5
         Left            =   3780
         Style           =   1  'Grafisch
         TabIndex        =   55
         Top             =   0
         Width           =   615
      End
      Begin VB.CommandButton fkey 
         Height          =   375
         Index           =   4
         Left            =   2880
         Style           =   1  'Grafisch
         TabIndex        =   54
         Top             =   0
         Width           =   615
      End
      Begin VB.CommandButton fkey 
         Height          =   375
         Index           =   3
         Left            =   2280
         Style           =   1  'Grafisch
         TabIndex        =   53
         Top             =   0
         Width           =   615
      End
      Begin VB.CommandButton fkey 
         Height          =   375
         Index           =   2
         Left            =   1680
         Style           =   1  'Grafisch
         TabIndex        =   52
         Top             =   0
         Width           =   615
      End
      Begin VB.CommandButton fkey 
         Height          =   375
         Index           =   1
         Left            =   1080
         Style           =   1  'Grafisch
         TabIndex        =   51
         Top             =   0
         Width           =   615
      End
      Begin VB.CommandButton fkey 
         Height          =   375
         Index           =   0
         Left            =   0
         Style           =   1  'Grafisch
         TabIndex        =   50
         Top             =   0
         Width           =   615
      End
   End
   Begin VB.Frame eingabeframe 
      Height          =   3315
      Left            =   60
      TabIndex        =   24
      Top             =   2760
      Width           =   8955
      Begin VB.TextBox e 
         Appearance      =   0  '2D
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   285
         Index           =   18
         Left            =   1080
         MaxLength       =   160
         TabIndex        =   21
         Text            =   "18"
         Top             =   2880
         Width           =   7755
      End
      Begin VB.CommandButton PlaySound 
         Height          =   255
         Left            =   8520
         Style           =   1  'Grafisch
         TabIndex        =   89
         Top             =   2220
         Width           =   315
      End
      Begin VB.CommandButton soundsuch 
         Height          =   255
         Left            =   8220
         Style           =   1  'Grafisch
         TabIndex        =   88
         Top             =   2220
         Width           =   315
      End
      Begin VB.CommandButton ADatum 
         Height          =   315
         Left            =   8520
         Style           =   1  'Grafisch
         TabIndex        =   87
         Top             =   1500
         Width           =   375
      End
      Begin VB.CommandButton jahr 
         Height          =   315
         Left            =   5400
         Style           =   1  'Grafisch
         TabIndex        =   86
         Top             =   1500
         Width           =   375
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   10
         Left            =   1470
         MaxLength       =   8
         TabIndex        =   13
         Text            =   "10"
         Top             =   1860
         Width           =   585
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   11
         Left            =   2100
         MaxLength       =   80
         TabIndex        =   14
         Text            =   "11"
         Top             =   1860
         Width           =   2115
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   12
         Left            =   5670
         MaxLength       =   8
         TabIndex        =   15
         Text            =   "12"
         Top             =   1860
         Width           =   615
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   13
         Left            =   6330
         MaxLength       =   80
         TabIndex        =   16
         Text            =   "13"
         Top             =   1860
         Width           =   2505
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   14
         Left            =   1050
         MaxLength       =   2
         TabIndex        =   17
         Text            =   "14"
         Top             =   2190
         Width           =   675
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   15
         Left            =   2610
         MaxLength       =   2
         TabIndex        =   18
         Text            =   "15"
         Top             =   2190
         Width           =   675
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   16
         Left            =   4320
         MaxLength       =   256
         TabIndex        =   19
         Text            =   "16"
         Top             =   2190
         Width           =   3885
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   17
         Left            =   1080
         MaxLength       =   160
         TabIndex        =   20
         Text            =   "17"
         Top             =   2520
         Width           =   7755
      End
      Begin VB.CheckBox check 
         Caption         =   "echter Musiktitel"
         Height          =   255
         Index           =   3
         Left            =   5460
         TabIndex        =   42
         Top             =   180
         Width           =   2055
      End
      Begin VB.TextBox track_e 
         Height          =   285
         Left            =   750
         TabIndex        =   2
         Text            =   "track_e"
         Top             =   180
         Width           =   675
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   0
         Left            =   840
         MaxLength       =   80
         TabIndex        =   3
         Text            =   "0"
         Top             =   540
         Width           =   6555
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   1
         Left            =   600
         MaxLength       =   80
         TabIndex        =   4
         Text            =   "1"
         Top             =   840
         Width           =   6825
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   2
         Left            =   810
         MaxLength       =   40
         TabIndex        =   5
         Text            =   "2"
         Top             =   1170
         Width           =   585
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   3
         Left            =   1440
         MaxLength       =   80
         TabIndex        =   6
         Text            =   "3"
         Top             =   1170
         Width           =   2475
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   4
         Left            =   4770
         MaxLength       =   8
         TabIndex        =   7
         Text            =   "4"
         Top             =   1170
         Width           =   585
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   5
         Left            =   5400
         MaxLength       =   80
         TabIndex        =   8
         Text            =   "5"
         Top             =   1170
         Width           =   2025
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   6
         Left            =   690
         MaxLength       =   5
         TabIndex        =   9
         Text            =   "6"
         Top             =   1500
         Width           =   585
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   7
         Left            =   2400
         MaxLength       =   3
         TabIndex        =   10
         Text            =   "7"
         Top             =   1500
         Width           =   585
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   8
         Left            =   4650
         MaxLength       =   4
         TabIndex        =   11
         Text            =   "8"
         Top             =   1500
         Width           =   675
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   9
         Left            =   7380
         MaxLength       =   10
         TabIndex        =   12
         Text            =   "9"
         Top             =   1500
         Width           =   1125
      End
      Begin VB.CheckBox check 
         Caption         =   "Stereo"
         Height          =   255
         Index           =   0
         Left            =   7590
         TabIndex        =   29
         Top             =   210
         Width           =   1335
      End
      Begin VB.CheckBox check 
         Caption         =   "Dolby"
         Height          =   255
         Index           =   1
         Left            =   7590
         TabIndex        =   28
         Top             =   510
         Width           =   1335
      End
      Begin VB.CheckBox check 
         Caption         =   "Surround"
         Height          =   255
         Index           =   4
         Left            =   7590
         TabIndex        =   27
         Top             =   810
         Width           =   1335
      End
      Begin VB.CheckBox check 
         Caption         =   "vollständig"
         Height          =   255
         Index           =   2
         Left            =   4110
         TabIndex        =   26
         Top             =   180
         Width           =   1335
      End
      Begin VB.CheckBox check 
         Caption         =   "Digital 5.1"
         Height          =   255
         Index           =   5
         Left            =   7590
         TabIndex        =   25
         Top             =   1110
         Width           =   1335
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Dateiname:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   7
         Left            =   60
         TabIndex        =   91
         Top             =   2940
         Width           =   975
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Soundfile:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   27
         Left            =   3360
         TabIndex        =   48
         Top             =   2220
         Width           =   870
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Aufnahmequelle:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   23
         Left            =   60
         TabIndex        =   47
         Top             =   1920
         Width           =   1425
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Bemerkung:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   28
         Left            =   60
         TabIndex        =   46
         Top             =   2580
         Width           =   1020
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Aufnahmegerät:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   24
         Left            =   4305
         TabIndex        =   45
         Top             =   1920
         Width           =   1350
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Bewertung:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   25
         Left            =   60
         TabIndex        =   44
         Top             =   2250
         Width           =   975
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Qualität:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   26
         Left            =   1830
         TabIndex        =   43
         Top             =   2250
         Width           =   735
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Track:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   8
         Left            =   60
         TabIndex        =   41
         Top             =   240
         Width           =   570
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Titelnummer:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   9
         Left            =   1575
         TabIndex        =   40
         Top             =   240
         Width           =   1095
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Interpret:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   14
         Left            =   60
         TabIndex        =   39
         Top             =   570
         Width           =   795
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Titel:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   15
         Left            =   60
         TabIndex        =   38
         Top             =   900
         Width           =   450
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Version:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   16
         Left            =   60
         TabIndex        =   37
         Top             =   1230
         Width           =   705
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Musikart:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   17
         Left            =   3975
         TabIndex        =   36
         Top             =   1230
         Width           =   795
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Länge:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   18
         Left            =   60
         TabIndex        =   35
         Top             =   1560
         Width           =   600
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Min."
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   19
         Left            =   1395
         TabIndex        =   34
         Top             =   1560
         Width           =   375
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Bpm:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   20
         Left            =   1920
         TabIndex        =   33
         Top             =   1560
         Width           =   435
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Erscheinungsjahr:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   21
         Left            =   3105
         TabIndex        =   32
         Top             =   1560
         Width           =   1530
      End
      Begin VB.Label text 
         Appearance      =   0  '2D
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Aufnahmedatum:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   22
         Left            =   5925
         TabIndex        =   31
         Top             =   1560
         Width           =   1425
      End
      Begin VB.Label TiNummer 
         BorderStyle     =   1  'Fest Einfach
         Height          =   255
         Left            =   2730
         TabIndex        =   30
         Top             =   210
         Width           =   1155
      End
   End
   Begin VB.TextBox dummyFeld 
      Height          =   405
      Left            =   9120
      TabIndex        =   22
      Text            =   "Text1"
      Top             =   2940
      Width           =   345
   End
   Begin MSComDlg.CommonDialog dialog 
      Left            =   9120
      Top             =   2310
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      FontSize        =   2,54052e-29
   End
End
Attribute VB_Name = "eingabe"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
DefInt A-Z

Dim ListingStart As Integer
Dim MarkedField As Integer
Dim TitlesInListFeld As Integer
Dim FormIsLoading As Boolean

Dim OleDragTiNum As Long
Sub fillHeader()
If Traegerloaded = True Then
  panel2(0).Caption = Format$(TrV.laenge, "###0.00")
  panel2(1).Caption = Format$(TrV.rest, "###0.00")
  panel2(2).Caption = Format$(TrV.Seiten)
  panel2(3).Caption = Format$(TrV.Tracks)
  GetTraegerTitel TrV.Bezeichnung, TrTi
  TraegerTitel.Caption = Trim$(TrTi.Titel1) + Chr$(13) + Trim$(TrTi.Titel2)
  a$ = GetFile("bildfile.dir", TrV.coverbild)
  On Error Resume Next
  'If TrV.coverbild = 0 Then Cover.Picture = gfx.pfp2.Picture Else Cover.Picture = LoadPicture(a$)

Else
  For i = 0 To 3: panel2(i).Caption = "": Next
  TraegerTitel.Caption = ""
  'Cover.Picture = gfx.pfp2.Picture
End If
End Sub

Sub InitFields()
CurrentTi.traeger = traeger
CurrentTi.Index = traegerindex
CurrentTi.Seite = TragerSeite
track = CInt(track_e.text)
CurrentTi.track = track
ff = FreeFile
Open TraegerTmpFile(Format(Seite)) For Random As #ff Len = Len(el)
If LOF(ff) / Len(el) >= track Then
  Get #ff, track, el
Else
  el.chk = 0
End If
Close #ff
If el.chk = 0 Then
  e(0).text = ""
  e(1).text = ""
  e(6).text = "0,00"
  e(7).text = "0"
  e(14).text = ""
  e(15).text = ""
  e(16).text = ""
  e(17).text = ""
  e(18).text = GetMp3FileName(traegerindex, (track))
  TiNummer.Caption = Lang(118)
Else
  GetTitel Ti, el.Titelnummer
  CurrentTi = Ti
  TiNummer.Caption = Format$(Ti.nummer)
  e(0).text = Trim$(Ti.interpret)
  e(1).text = Trim$(Ti.titel)
  e(2).text = Format$(Ti.version)
  e(4).text = Format$(Ti.musikart)
  e(6).text = Format$(Ti.laenge, "###0.00")
  e(7).text = Format$(Ti.bpm)
  e(8).text = Format$(Ti.Erscheinungsjahr)
  e(9).text = Long2Dat$(Ti.Aufnahmedatum)
  e(10).text = Format$(Ti.Quelle)
  e(12).text = Format$(Ti.Aufnahmegeraet)
  e(14).text = Trim$(Ti.Bewertung)
  e(15).text = Trim$(Ti.Qualitaet)
  e(16).text = GetFile("sndfile.dir", Ti.Soundfile)
  e(17).text = GetFile("bemerk.dat", Ti.bemerkung)
  e(18).text = GetMp3FileName(traegerindex, (track))
  For i = 0 To 3
    check(i).value = False
  Next
  If (Ti.InfoByte And 4) = 4 Then check(0).value = 1
  If (Ti.InfoByte And 8) = 8 Then check(1).value = 1
  If (Ti.InfoByte And 32) = 32 Then check(2).value = 1
  If (Ti.InfoByte And 64) = 64 Then check(3).value = 1
  If (Ti.InfoByte And 16) = 16 Then check(4).value = 1
  If (Ti.InfoByte And 128) = 128 Then check(5).value = 1
  
End If
If track < ListingStart Or track >= ListingStart + TitlesInListFeld Then
  If track <= ListScroll.max Then
    w = track - 5
  Else
    w = ListScroll.max - 5
  End If
  If w < 1 Then w = 1
  ListScroll.value = w
  PaintListFeld
End If

End Sub
Sub LoadTitleIntoCurrentTrack()
    Dim gt As Titeldatei
    'Dim tret As Integer
    On Error Resume Next
    tt& = CLng(e(0).text)
    If tt& = 0 Then Exit Sub
    GetTitel gt, tt&
    If gt.nummer > 0 Then
      Ti = gt
      Ti.nummer = 0
      If Ti.traeger = traeger And Ti.Index = traegerindex Then
        Ti.Seite = Seite: Ti.track = track
        SaveTitel Ti
      End If
      CurrentTi = Ti
      'TiNummer.Caption = Format$(Ti.nummer)
      TiNummer.Caption = "Neu"
      e(0).text = Trim$(Ti.interpret)
      e(1).text = Trim$(Ti.titel)
      e(2).text = Format$(Ti.version)
      e(4).text = Format$(Ti.musikart)
      e(6).text = Format$(Ti.laenge, "###0.00")
      e(7).text = Format$(Ti.bpm)
      e(8).text = Format$(Ti.Erscheinungsjahr)
      e(9).text = Long2Dat$(Ti.Aufnahmedatum)
      e(10).text = Format$(Ti.Quelle)
      e(12).text = Format$(Ti.Aufnahmegeraet)
      e(14).text = Trim$(Ti.Bewertung)
      e(15).text = Trim$(Ti.Qualitaet)
      e(16).text = GetFile("sndfile.dir", Ti.Soundfile)
      e(17).text = GetFile("bemerk.dat", Ti.bemerkung)
      e(18).text = GetMp3FileName(traegerindex, (track))
      For i = 0 To 3
        check(i).value = False
      Next
      If (Ti.InfoByte And 4) = 4 Then check(0).value = 1
      If (Ti.InfoByte And 8) = 8 Then check(1).value = 1
      If (Ti.InfoByte And 32) = 32 Then check(2).value = 1
      If (Ti.InfoByte And 64) = 64 Then check(3).value = 1
      If (Ti.InfoByte And 16) = 16 Then check(4).value = 1
      If (Ti.InfoByte And 128) = 128 Then check(5).value = 1

    End If

End Sub

Sub PaintListFeld()
Dim hg As Long
Dim currentlength As Long

MarkierungEingabeListe = MarkedField
X1 = 0: X2 = Listfeld.Width
Y1 = 0: Y2 = Listfeld.Height
If Traegerloaded = True And Seite > 0 Then
  Listfeld.FontSize = 8: Listfeld.FontBold = True
  Listfeld.FontName = "Arial"
  ff = FreeFile
  Open TraegerTmpFile(Format(Seite)) For Random As #ff Len = Len(el)
  ListScroll.LargeChange = Y2 / (15 * tpy) - 1
  ListScroll.min = 1
  ListScroll.max = LOF(ff) / Len(el)
  titel_on_this_side = LOF(ff) / Len(el)
  ListingStart = ListScroll.value
  If ListingStart > titel_on_this_side Then ListingStart = titel_on_this_side - 10
  
  Y = 0
  If ListingStart <= 0 Then ListingStart = 1
  currentlength = 0
  For i = 1 To ListingStart - 1
    Get #ff, i, el
    currentlength = currentlength + Min2Sec(el.laenge)
  Next
  TitlesInListFeld = 0
  For i = ListingStart To titel_on_this_side
    TitlesInListFeld = TitlesInListFeld + 1
    hg = RGB(255, 255, 255)
    If i = MarkedField Then
      hg = RGB(200, 255, 255)
    End If
    Listfeld.Line (0, Y)-(403 * tpx, Y + 15 * tpy), hg, BF
    Get #ff, i, el
    a$ = Format$(i)
    X = 33 * tpx - TextWidth(a$)
    Listfeld.CurrentX = X: Listfeld.CurrentY = Y
    Listfeld.ForeColor = 0: Listfeld.Print a$;
    If el.chk = 1 Then
      Listfeld.CurrentX = 37 * tpx
      Listfeld.ForeColor = RGB(0, 0, 128)
      Listfeld.Print Trim$(el.Interpret_Titel)
      Listfeld.Line (403 * tpx, Y)-(533 * tpx, Y + 15 * tpy), hg, BF
      Listfeld.ForeColor = RGB(255, 0, 0)
      Listfeld.CurrentX = 405 * tpx: Listfeld.CurrentY = Y
      Listfeld.Print Trim$(el.info)
      Listfeld.Line (533 * tpx, Y)-(X2, Y + 15 * tpy), hg, BF
      Listfeld.ForeColor = RGB(0, 128, 0)
      a$ = Format$(el.laenge, "##0.00")
      currentlength = currentlength + Min2Sec(el.laenge)
      Listfeld.CurrentX = X2 - TextWidth(a$): Listfeld.CurrentY = Y
      Listfeld.Print a$
    End If
    Y = Y + 15 * tpy
    If Y >= Y2 - 15 * tpy Then Exit For
  Next
  Close #ff

  For X = Y To Y2 Step 15 * tpy
    Listfeld.Line (0, X)-(X2, X + 14 * tpy), RGB(255, 255, 255), BF
    If X = Y And i >= titel_on_this_side Then
      Listfeld.ForeColor = RGB(0, 128, 0)
      a$ = Format$(Sec2Min(currentlength), "##0.00")
      Listfeld.CurrentX = X2 - TextWidth(a$): Listfeld.CurrentY = Y + 10
      Listfeld.Print a$
      Listfeld.Line (533 * tpx, X + 0)-(X2, X + tpy * 2), 0, BF
    End If
  Next
End If
X = 34: c& = RGB(192, 192, 192)
Listfeld.Line (X * tpx, Y1)-(X * tpx, Y2), c&
X = 403
Listfeld.Line (X * tpx, Y1)-(X * tpx, Y2), c&
X = 533
Listfeld.Line (X * tpx, Y1)-(X * tpx, Y2), c&
Y2 = Fix(Y2 / tpy)
For Y = 15 To Y2 - 15 Step 15
  Listfeld.Line (0, Y * tpy)-(X2, Y * tpy), c&
Next
End Sub

Private Sub ADatum_Click()
e(9).text = Datum$
SetEingabePosition
End Sub

Private Sub Cover_Click()
'bigcover.Show
End Sub

Private Sub Command1_Click()
On Error Resume Next
FTaste Index, 1024
If eingabevisible = True And Index <> 6 Then
  SetEingabePosition
End If

End Sub

Private Sub Command1_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
Static lastx, lasty
If lastx <> X Or lasty <> Y Then
  lastx = X: lasty = Y
End If

End Sub


Private Sub Command2_Click()

End Sub

Private Sub dummyFeld_GotFocus()
If FormIsLoading = False Then SaveTrack
If quickinfo.Visible = True Then
    quickinfo.Visible = False
    QuickInfoPlay.Visible = False
End If
End Sub


Private Sub e_Change(Index As Integer)
Select Case Index
  Case 0:
          If EingabePos = Index Then
            a$ = e(0).text
            If Len(a$) > Len(e(0).tag) And AutoInterpret = True Then
              e(0).tag = e(0).text
              If Len(a$) > 0 Then
                b$ = FindInterpret(a$)
                If b$ <> "" Then
                  e(0).text = a$ + Mid$(b$, Len(a$) + 1)
                  e(0).tag = a$
                  SendKeys "{home}"
                  For i = 1 To Len(a$)
                    SendKeys "{right}"
                  Next
                  SendKeys "+{end}"
                End If
              End If
            Else
              e(0).tag = e(0).text
            End If
          End If
  Case 1:
          If EingabePos = Index Then
            a$ = e(1).text
            If Len(a$) > Len(e(1).tag) And AutoTitel = True Then
              e(1).tag = e(1).text
              If Len(a$) > 0 Then
                b$ = FindTitel(a$)
                If b$ <> "" Then
                  e(1).text = a$ + Mid$(b$, Len(a$) + 1)
                  e(1).tag = a$
                  SendKeys "{home}"
                  For i = 1 To Len(a$)
                    SendKeys "{right}"
                  Next
                  SendKeys "+{end}"
                End If
              End If
            Else
              e(1).tag = e(1).text
            End If
          End If
          
  Case 2: e(3).text = GetTab("version", Val(e(2).text))
             e(3).tag = e(3).text
  Case 4: e(5).text = GetTab("musikart", Val(e(4).text))
             e(5).tag = e(5).text
  Case 10: e(11).text = GetTab("quelle", Val(e(10).text))
             e(11).tag = e(11).text
  Case 12: e(13).text = GetTab("aufnahme", Val(e(12).text))
             e(13).tag = e(13).text
            
End Select

End Sub

Private Sub e_Click(Index As Integer)
LetzteEingabePos = Index
EingabePos = Index
End Sub

Private Sub e_GotFocus(Index As Integer)
On Error Resume Next
If eingabeframe.Enabled = False Then eingabeframe.Enabled = True: functions.Enabled = True
If EingabePos < 0 And Index > 0 Then e(0).SetFocus: Exit Sub
EingabePos = Index
Select Case Index
  Case 0
    If CurrentTi.traeger = 0 Then
      If CLng(track_e.text) < 1 Then
        track_e.SetFocus
        Exit Sub
      End If
      'Felder initialisieren
      EingabePos = -50
      InitFields
      e(0).tag = e(0).text
      EingabePos = 0
    End If
  Case 3: If LetzteEingabePos = 2 And Trim$(e(3).text) <> "" Then e(4).SetFocus: Exit Sub
          If LetzteEingabePos = 4 Then e(2).SetFocus: Exit Sub
  Case 5: If LetzteEingabePos = 4 And Trim$(e(5).text) <> "" Then e(6).SetFocus: Exit Sub
          If LetzteEingabePos = 6 Then e(4).SetFocus: Exit Sub
  Case 11: If LetzteEingabePos = 10 And Trim$(e(11).text) <> "" Then e(12).SetFocus: Exit Sub
           If LetzteEingabePos = 12 Then e(10).SetFocus: Exit Sub
  Case 13: If LetzteEingabePos = 12 And Trim$(e(13).text) <> "" Then e(14).SetFocus: Exit Sub
           If LetzteEingabePos = 14 Then e(12).SetFocus: Exit Sub

End Select
If EingabePos >= 0 Then fkey(3).Picture = gfx.fkey(16).Picture
ActivateNumLock
SendKeys "{home}+{end}"
DoEvents

'keystatus.Value = True
LetzteEingabePos = Index
End Sub

Private Sub e_KeyDown(Index As Integer, KeyCode As Integer, Shift As Integer)
Dim value As String
If Shift = 2 Then
  If KeyCode = 68 Then e(9).SetFocus
  If KeyCode = 66 Then e(17).SetFocus
  If KeyCode = 76 Then e(6).SetFocus
  If KeyCode = 81 Then e(10).SetFocus
  If KeyCode = 67 And EingabePos = 0 Then
        LoadTitleIntoCurrentTrack
  End If
  If KeyCode = 75 Then
    value = UpperCaseWords(e(Index).text)
    e(Index).text = value
  End If
  Debug.Print KeyCode, Shift
End If
End Sub
Private Sub e_LostFocus(Index As Integer)
On Error Resume Next
Select Case Index
  Case 2:
          If InStr(e(2).text, "**") > 0 Then
             a$ = e(3).text
             e(2).text = Format$(GetNextFreeTab("version"))
             e(3).text = a$
             LetzteEingabePos = 3
             e(3).SetFocus
          ElseIf InStr(e(2).text, "*") > 0 Then
             e(2).text = Format$(GetNextFreeTab("version"))
             e(3).text = "": e(2).SetFocus
          Else
             e(3).text = GetTab("version", Val(e(2).text))
             e(3).tag = e(3).text
          End If
  Case 3:
          If e(3).text <> e(3).tag Then
            PutTab "version", Val(e(2).text), e(3).text
          End If
  Case 4:
          If InStr(e(4).text, "**") > 0 Then
             a$ = e(5).text
             e(4).text = Format$(GetNextFreeTab("musikart"))
             e(5).text = a$
             LetzteEingabePos = 5
             e(5).SetFocus
          ElseIf InStr(e(4).text, "*") > 0 Then
             e(4).text = Format$(GetNextFreeTab("musikart"))
             e(5).text = "": e(4).SetFocus
          Else
             e(5).text = GetTab("musikart", Val(e(4).text))
             e(5).tag = e(5).text
          End If
  Case 5:
          If e(5).text <> e(5).tag Then
            PutTab "musikart", Val(e(4).text), e(5).text
          End If
  Case 6: e(6).text = Format$(CSng(e(6).text), "##0.00")
          If Err > 0 Then e(6).text = Format$(0, "##0.00")
  Case 8: a$ = Trim$(e(8).text)
          If Len(a$) < 4 Then a$ = left$(Format$(year(Now)), 4 - Len(a$)) + a$
          e(8).text = a$
  Case 9: a$ = e(9).text
          If a$ <> "" Then a$ = DatumsAuswertung(a$) Else a$ = Datum$()
          e(9).text = a$
  Case 10:
          If InStr(e(10).text, "*") > 0 Then
             e(10).text = Format$(GetNextFreeTab("quelle"))
             e(11).text = "": e(10).SetFocus
          Else
             e(11).text = GetTab("quelle", Val(e(10).text))
             e(11).tag = e(11).text
          End If
  Case 11:
          If e(11).text <> e(11).tag Then
            PutTab "quelle", Val(e(10).text), e(11).text
          End If
  Case 12:
          If InStr(e(12).text, "*") > 0 Then
             e(12).text = Format$(GetNextFreeTab("aufnahme"))
             e(13).text = "": e(12).SetFocus
          Else
             e(13).text = GetTab("aufnahme", Val(e(12).text))
             e(13).tag = e(13).text
          End If
  Case 13:
          If e(13).text <> e(13).tag Then
            PutTab "aufnahme", Val(e(12).text), e(13).text
          End If

End Select
fkey(3).Picture = gfx.fkey(13).Picture
End Sub

Private Sub e_MouseDown(Index As Integer, Button As Integer, Shift As Integer, X As Single, Y As Single)
LetzteEingabePos = Index
EingabePos = Index
End Sub

Private Sub e_MouseUp(Index As Integer, Button As Integer, Shift As Integer, X As Single, Y As Single)
'SendKeys "{home}+{end}"
End Sub


Private Sub fkey_Click(Index As Integer)
On Error Resume Next
FTaste Index, 1024
If eingabevisible = True And Index <> 6 Then
  SetEingabePosition
End If

End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
Select Case KeyCode
  Case 27: FTaste 0, Shift
  Case 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123
           FTaste KeyCode - 111, Shift
  Case 13: KeyCode = 0: SendKeys "{tab}"
  Case 33: If EingabePos > -2 And Shift = 0 And ListScroll.value > ListScroll.min Then ListScroll.value = ListScroll.value - 1
           If EingabePos > -2 And Shift > 0 And ListScroll.value - ListScroll.LargeChange > ListScroll.min Then
             ListScroll.value = ListScroll.value - ListScroll.LargeChange
           ElseIf EingabePos > -2 And Shift > 0 Then
             ListScroll.value = ListScroll.min
           End If
  Case 34: If EingabePos > -2 And Shift = 0 And ListScroll.value < ListScroll.max Then ListScroll.value = ListScroll.value + 1
           If EingabePos > -2 And Shift > 0 And ListScroll.value + ListScroll.LargeChange < ListScroll.max Then
             ListScroll.value = ListScroll.value + ListScroll.LargeChange
           ElseIf EingabePos > -2 And Shift > 0 Then
             ListScroll.value = ListScroll.max
           End If
  Case 38: If EingabePos > -3 Then EingabePos = EingabePos - 1: SetEingabePosition
  Case 40: If EingabePos < 17 Then EingabePos = EingabePos + 1: SetEingabePosition
  
End Select
'Debug.Print KeyCode
End Sub

Private Sub Form_KeyPress(KeyAscii As Integer)
If KeyAscii = 13 Then KeyAscii = 0
End Sub

Private Sub Form_Load()
On Error Resume Next
StatusBar1.Panels(1).text = Programm + " " + version + "." + Format(build)

eingabe.Width = 9225
tpx = Screen.TwipsPerPixelX
tpy = Screen.TwipsPerPixelY
eingabevisible = True


Dim rec As Rect
openini AppData + "winmusik2.ini"
selectTopic "Fenster"
a$ = GetINI$("Eingabe")
If inierror = 0 Then
  String2Rect a$, rec
Else
  rec.left = leiste1.left: rec.top = leiste1.top + 1140
  rec.right = 9225: rec.bottom = 6585
End If
If rec.left > Screen.Width - eingabe.Width Then rec.left = Screen.Width - eingabe.Width
If rec.top > Screen.Height - eingabe.Height Then rec.top = Screen.Height - eingabe.Height
eingabe.left = rec.left: eingabe.top = rec.top
eingabe.Width = rec.right: eingabe.Height = rec.bottom
topframe.Visible = False
ListPanel.Visible = False
functions.Visible = False
eingabeframe.Visible = False
FormIsLoading = True
Show
DoEvents
FormIsLoading = False
eingabe.icon = gfx.traegericon(traeger).Picture
gfx.TrImageList.MaskColor = RGB(192, 192, 192)
eingabe.traegericon.Picture = gfx.TrImageList.ListImages(traeger).Picture
eingabe.Caption = Lang(299 + traeger)
WMStat_Clear (eingabe.Caption)

eingabe.LTrack.Caption = Lang(65)
eingabe.LTitel.Caption = Lang(71) + ": " + Lang(72)
eingabe.LVersion.Caption = Lang(73)
ListingAnzeige = 0
eingabe.LLaenge.Caption = Lang(75)
text(0).Caption = Lang(56) + ":"
index_e.left = text(0).left + text(0).Width + 2 * tpx
text(1).left = index_e.left + index_e.Width + 6 * tpx
text(1).Caption = Lang(57) + ":"
seite_e.left = text(1).left + text(1).Width + 2 * tpx
text(2).left = seite_e.left + seite_e.Width + 6 * tpx
text(2).Caption = Lang(58) + ":"
panel2(0).left = text(2).left + text(2).Width + 2 * tpx
text(3).left = panel2(0).left + panel2(0).Width + 6 * tpx
text(3).Caption = Lang(59) + "  " + Lang(60) + ":"
panel2(1).left = text(3).left + text(3).Width + 2 * tpx
text(4).left = panel2(1).left + panel2(1).Width + 6 * tpx
text(4).Caption = Lang(59) + "  " + Lang(61) + ":"
panel2(2).left = text(4).left + text(4).Width + 2 * tpx
text(5).left = panel2(2).left + panel2(2).Width + 6 * tpx
text(5).Caption = Lang(62) + ":"
panel2(3).left = text(5).left + text(5).Width + 2 * tpx
text(6).Caption = Lang(72) + ":"
index_e.text = ""
topframe.Visible = True
DoEvents
For i = 0 To 20
  text(8 + i) = Lang(65 + i) + ":"
Next
track_e.left = text(8).left + text(8).Width + 2 * tpx
text(9).left = track_e.left + track_e.Width + 10 * tpx
TiNummer.left = text(9).left + text(9).Width + 2 * tpx

'For i = 3 To 0 Step -1
'  text(10 + i).left = check(i).left - 2 * tpx - text(10 + i).Width
'  If i > 0 Then check(i - 1).left = text(10 + i).left - 6 * tpx - check(i - 1).Width
'Next

check(0).value = 1
check(1).value = 1
check(2).value = 1
check(3).value = 1

check(0).Caption = Lang(67)
check(1).Caption = Lang(68)
check(2).Caption = Lang(69)
check(3).Caption = Lang(70)
check(4).Caption = Lang(241)
check(5).Caption = Lang(242)




'Feldgröße für Interpret, Titel und Bemerkung anpassen
e(0).left = text(14).left + text(14).Width + 2 * tpx: e(0).Width = 7455 - e(0).left
e(1).left = text(15).left + text(15).Width + 2 * tpx: e(1).Width = 7455 - e(1).left
e(17).left = text(28).left + text(28).Width + 2 * tpx: e(17).Width = 8865 - e(17).left

'Erscheinungsjahr und Aufnahmedatum rechtsbündig
text(22).left = e(9).left - 2 * tpx - text(22).Width
jahr.left = text(22).left - 6 * tpx - jahr.Width
e(8).left = jahr.left - 2 * tpx - e(8).Width
text(21).left = e(8).left - 2 * tpx - text(21).Width
'Länge, Bpm linksbündig
text(19).Caption = Lang(76)
e(6).left = text(18).left + text(18).Width + 2 * tpx
text(19).left = e(6).left + e(6).Width + 2 * tpx
text(20).left = text(19).left + text(19).Width + 6 * tpx
e(7).left = text(20).left + text(20).Width + 2 * tpx
'Bewertung, Qualität, Soundfile
e(14).left = text(25).left + text(25).Width + 2 * tpx
text(26).left = e(14).left + e(14).Width + 6 * tpx
e(15).left = text(26).left + text(26).Width + 2 * tpx
text(27).left = e(15).left + e(15).Width + 6 * tpx
e(16).left = text(27).left + text(27).Width + 2 * tpx
e(16).Width = 8235 - e(16).left
'Version, Musikart
e(2).left = text(16).left + text(16).Width + 2 * tpx
e(3).left = e(2).left + e(2).Width + 2 * tpx
e(3).Width = 3945 - e(3).left
e(4).left = text(17).left + text(17).Width + 2 * tpx
e(5).left = e(4).left + e(4).Width + 2 * tpx
e(5).Width = 7455 - e(5).left
'Aufnahmequelle, Aufnahmegerät
e(10).left = text(23).left + text(23).Width + 2 * tpx
e(11).left = e(10).left + e(10).Width + 2 * tpx
e(11).Width = 4245 - e(11).left
e(12).left = text(24).left + text(24).Width + 2 * tpx
e(13).left = e(12).left + e(12).Width + 2 * tpx
e(13).Width = 8865 - e(13).left

'Eingabefelder löschen, bzw. voreinstellen
For i = 0 To 17
  e(i).text = ""
Next
seite_e.text = "1"
track_e.text = "1"
ff = FreeFile
index_e.text = GetNextFreeTraeger(traeger)

DoEvents
eingabeframe.Enabled = False
functions.Enabled = False
MaskPos = -1
Show
DoEvents
If eingabe.Enabled = False Then eingabe.Enabled = True: functions.Enabled = True

j = Val(right$(year(Now), 2))
e(8).text = year(Now)
e(9).text = Datum()
d = Day(Now)
jahr.Picture = gfx.Kalender.GraphicCell(j)
adatum.Picture = gfx.Kalender.GraphicCell(d)
soundsuch.Picture = gfx.folder1.Picture
PlaySound.Picture = gfx.speaker.Picture

For i = 0 To 12
  fkey(i).Picture = gfx.fkey(i).Picture
Next
fkey(1).Picture = gfx.fkey(13).Picture
fkey(3).Picture = gfx.fkey(13).Picture
fkey(6).Picture = gfx.fkey(13).Picture
If traeger = 7 Then
  fkey(6).Picture = gfx.fkey(6).Picture
  fkey(9).Picture = gfx.fkey(14).Picture
  fkey(10).Picture = gfx.fkey(15).Picture
Else
  fkey(9).Picture = gfx.fkey(13).Picture
  fkey(10).Picture = gfx.fkey(13).Picture

End If


e(2).text = "1"
e(4).text = "1"
e(10).text = "0"
e(12).text = "0"
ListPanel.Visible = True
functions.Visible = True
eingabeframe.Visible = True
quickinfo.Visible = False
QuickInfoPlay.Visible = False
functions.Enabled = False


index_e.SetFocus
End Sub

Private Sub Form_Resize()
If eingabe.WindowState <> 1 Then
  eingabe.Width = 9225
  If eingabe.Height < 6300 Then eingabe.Height = 6300
  functions.top = eingabe.Height - functions.Height - 450 - 300
  eingabeframe.top = functions.top - 3315
  ListPanel.Height = eingabeframe.top - ListPanel.top - 3 * tpy
  Listfeld.Height = ListPanel.Height - 21 * tpy
  ListScroll.Height = ListPanel.Height - 4 * tpy
    ProgressBar1.left = StatusBar1.Panels(3).left + 50
    ProgressBar1.top = StatusBar1.top + 75
    ProgressBar1.Width = StatusBar1.Panels(3).Width - 600
    ProgressBar1.value = 100
    PaintListFeld

End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
If eingabe.WindowState <> 0 Then eingabe.WindowState = 0: DoEvents
If traegerstammvisible = True Then Unload traegerstamm
eingabevisible = False
Dim rec As Rect
openini AppData + "winmusik2.ini"
selectTopic "Fenster"
rec.top = eingabe.top: rec.left = eingabe.left
rec.right = eingabe.Width: rec.bottom = eingabe.Height
a$ = Rect2String(rec)
putini "Eingabe=" + a$
closeini
Traegerloaded = False
End Sub

Private Sub Frame1_DragDrop(Source As Control, X As Single, Y As Single)

End Sub


Private Sub Image1_Click()

End Sub

Private Sub functions2_Click()
SetEingabePosition
End Sub

Private Sub index_e_GotFocus()
If Traegerloaded = True Then
    SaveTraeger
    'Kill tmp + "\~wm2in?.tmp"
End If
EingabePos = -3
Traegerloaded = False
eingabeframe.Enabled = False
functions.Enabled = False
ListPanel.Enabled = False
MarkedField = 0
If quickinfo.Visible = True Then
    quickinfo.Visible = False
    QuickInfoPlay.Visible = False
End If
DoEvents
SendKeys "{home}+{end}"
DoEvents
ActivateNumLock

End Sub


Private Sub jahr_Click()
e(8).text = Format$(year(Now))
SetEingabePosition
End Sub

Private Sub Label1_Click()

End Sub

Private Sub lAuswahl_Click(Index As Integer)
Dim tmpel As EingabeListing
Dim tmpti As Titeldatei
Dim tret As Long
If Index = 0 Then a = 1 Else a = -1
ListingAnzeige = ListingAnzeige + a
If ListingAnzeige > 6 Then ListingAnzeige = 0
If ListingAnzeige < 0 Then ListingAnzeige = 6
Select Case ListingAnzeige
  Case 0: b$ = Lang(73): ' Version
  Case 1: b$ = Lang(74): ' Musikart
  Case 2: b$ = Lang(77): ' BPM
  Case 3: b$ = Lang(79): ' Aufnahmedatum
  Case 4: b$ = Lang(80): ' Aufnahmequelle
  Case 5: b$ = Lang(85): ' Bemerkung
  Case 6: b$ = Lang(66): ' Titelnummer
End Select
eingabe.LVersion.Caption = b$
'tif = FreeFile
'Open TiDatei For Random As #tif Len = Len(tmpti)
For s = 1 To TrV.Seiten
  ff = FreeFile
  Open TraegerTmpFile(Format(s)) For Random As #ff Len = Len(tmpel)
  For i = 1 To LOF(ff) / Len(tmpel)
    Get #ff, i, tmpel
    If tmpel.chk = 1 Then
      'Get #tif, tmpel.Titelnummer, tmpti
      tret = GetWMTitle(tmpel.Titelnummer, tmpti)
      Select Case ListingAnzeige
        Case 0: b$ = GetTab("version", tmpti.version)
        Case 1: b$ = GetTab("musikart", tmpti.musikart)
        Case 2: b$ = Format$(tmpti.bpm)
        Case 3: b$ = Long2Dat(tmpti.Aufnahmedatum)
        Case 4: b$ = GetTab("quelle", tmpti.Quelle)
        Case 5: b$ = GetFile("bemerk.dat", tmpti.bemerkung)
        Case 6: b$ = Format$(tmpel.Titelnummer)
      End Select
      tmpel.info = b$
      Put #ff, i, tmpel
    End If
  Next
  Close #ff
Next
'Close tif
PaintListFeld
SetEingabePosition
End Sub

Private Sub Listfeld_Click()

SetEingabePosition
End Sub

Private Sub Listfeld_DblClick()
Dim file As String
Dim ret As Long
file = Trim(Listfeld.tag)
If file <> "" Then
    ret = ShellExecute(Me.hwnd, "open", file, "", "", SW_SHOWNORMAL)
End If

End Sub

Private Sub Listfeld_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
Dim t As Long
t = Int(Y / tpy / 15) + ListScroll.value
If DebugLevel > 0 Then Debug.Print "Klick on Listfeld, Track:"; t
If t > 0 And t <= ListScroll.max Then
  If MarkedField = t Then MarkedField = 0 Else MarkedField = t
  PaintListFeld
End If
If Traegerloaded = True And Seite > 0 Then
  ff = FreeFile
  Open TraegerTmpFile(Format(Seite)) For Random As #ff Len = Len(el)
  If t > 0 And t <= LOF(ff) / Len(el) Then
    Get #ff, t, el
    OleDragTiNum = el.Titelnummer
    Clipboard.Clear
    Clipboard.SetText (Trim(el.Interpret_Titel) + " (" + Format(el.laenge, "0.00") + " min, " + Trim(el.info) + ")")
    If traeger = 7 Then
        Listfeld.tag = GetMp3FileName(traegerindex, t)
    End If
    Listfeld.OLEDragMode = 1
    Listfeld.OLEDrag
  End If
  Close #ff
End If

End Sub


Private Sub Listfeld_OLECompleteDrag(Effect As Long)
Listfeld.OLEDragMode = 0

End Sub

Private Sub Listfeld_OLEStartDrag(Data As DataObject, AllowedEffects As Long)
Dim file As String
Dim d As Variant

Debug.Print "OLEStartDrag"
Data.Clear
Data.files.Clear
file = Trim(Listfeld.tag)
If traeger = 7 And file <> "" Then
    Data.files.Add file
    Data.SetData , vbCFFiles
End If

d = "WM2###" + Format(OleDragTiNum) + "###"
Data.SetData d, vbCFText


End Sub

Private Sub ListFeld_Paint()
PaintListFeld
End Sub

Private Sub ListPanel_Click()
SetEingabePosition
End Sub

Private Sub ListScroll_Change()
SetEingabePosition
PaintListFeld
End Sub

Private Sub ListScroll_Scroll()
SetEingabePosition
PaintListFeld

End Sub

Private Sub Llaenge_Click()
SetEingabePosition
End Sub

Private Sub LTitel_Click()
SetEingabePosition
End Sub

Private Sub LTrack_Click()
SetEingabePosition
End Sub

Private Sub lVersion_Click()
SetEingabePosition
End Sub

Private Sub panel2_Click(Index As Integer)
SetEingabePosition
End Sub

Private Sub PlaySound_Click()
#If Win32 Then
  Dim uflags As Long, a As Long
#Else
  Dim uflags As Integer, a As Integer
#End If

If EingabePos >= 0 Then
  f$ = Trim$(e(16).text)
  If f$ <> "" Then
    If Exists(f$) = True Then
      uflags = 1
      a = sndPlaySound(f$, uflags)
    Else
      MsgBox Lang(123), 48, Lang(122)
    End If
  End If
End If
SetEingabePosition
End Sub

Private Sub QuickInfoPlay_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
QuickInfoPlay.OLEDragMode = 1
QuickInfoPlay.OLEDrag

End Sub

Private Sub QuickInfoPlay_OLECompleteDrag(Effect As Long)
OLEDragMode = 0
End Sub

Private Sub QuickInfoPlay_OLEStartDrag(Data As DataObject, AllowedEffects As Long)
Dim file As String
Data.Clear
Data.files.Clear
file = Trim(QuickInfoPlay.tag)
If traeger = 7 And file <> "" Then
    Data.files.Add file
    Data.SetData , vbCFFiles
End If
End Sub

Private Sub seite_e_Change()
SendKeys "{home}+{end}"
DoEvents
ActivateNumLock
End Sub

Private Sub seite_e_GotFocus()
MarkedField = 0
Dim NextFree As Long, antwort As Integer
If quickinfo.Visible = True Then
    quickinfo.Visible = False
    QuickInfoPlay.Visible = False
End If
If Traegerloaded = True And EingabePos > -2 And TrV.Seiten = 1 Then
  index_e.SetFocus
End If
track_e.text = "1"
EingabePos = -2
DoEvents
If Traegerloaded = False Then
  traegerindex = Val(index_e.text)
  If LoadTraeger() = True Then
    Traegerloaded = True
    If TrV.Seiten > 1 Then Listfeld.Refresh
    SendKeys "{home}+{end}"
    DoEvents
  Else
    ff = FreeFile
    Open DatenPfad + "\" + TraegerBez(traeger) + ".dir" For Random As #ff Len = Len(TrV)
    NextFree = Format$(LOF(ff) / Len(TrV) + 1)
    Close #ff
    LangPar 2, Format$(traegerindex)
    antwort = 6
    If traegerindex = NextFree + 1 Then
      LangPar 1, Format$(NextFree)
      antwort = MsgBox(Lang(101) + Lang(98), 36, Programm + " " + version + ": " + Lang(99))
    ElseIf traegerindex > NextFree + 1 Then
      LangPar 1, Format$(NextFree) + " - " + Format$(traegerindex - 1)
      antwort = MsgBox(Lang(101) + Lang(100), 36, Programm + " " + version + ": " + Lang(99))
    End If
    If antwort = 6 Then
      'eingabe.Enabled = False
      clearTrV TrV
      TrV.ErsterDatensatz = 0
      traegerstamm.Show
      SendKeys "{home}+{end}"
      DoEvents
    Else
      index_e.SetFocus
    End If
  End If
  seite_e.text = "1": Seite = 1
  Listfeld.Refresh
End If
fillHeader
If Traegerloaded = True Then
  eingabeframe.Enabled = True
  functions.Enabled = True
  ListPanel.Enabled = False
  If TrV.Seiten = 1 Then
    track_e.SetFocus: Seite = 1
    track_e.text = Format(TrV.Tracks + 1)
  End If
End If

End Sub


Private Sub soundsuch_Click()
On Error GoTo ErrHandler
If EingabePos >= 0 Then
  dialog.filename = e(16).text
  dialog.filter = "Wave Dateien (*.wav)|*.wav|MIDI Dateien (*.mid)|*.mid|Alle Dateien (*.*)|*.*"
  dialog.FilterIndex = 0
  dialog.DefaultExt = ".wav"
  dialog.DialogTitle = Lang(97)
  dialog.flags = &H4&
  dialog.ShowOpen
  e(16).text = dialog.filename
End If
ErrHandler:
SetEingabePosition
End Sub

Private Sub topframe_Click()
SetEingabePosition
End Sub

Private Sub track_e_GotFocus()
If EingabePos < -1 Then
  Seite = Val(seite_e.text)
  If Seite < 1 Or Seite > TrV.Seiten Then seite_e.SetFocus: Exit Sub
  eingabeframe.Enabled = True
  ListPanel.Enabled = True
  functions.Enabled = True
  'SendKeys "{home}+{end}"
  'DoEvents
  'ActivateNumLock
  EingabePos = -1
  Listfeld.Refresh
End If
If quickinfo.Visible = True Then
    quickinfo.Visible = False
    QuickInfoPlay.Visible = False
End If
CurrentTi.traeger = 0
CurrentTi.nummer = 0
track = 0
SendKeys "{home}+{end}"
DoEvents
ActivateNumLock

End Sub


Private Sub TraegerIcon_DblClick()
Dim file As String
If traeger = 7 And Traegerloaded = True Then
    file = GetMp3Playlist(traegerindex)
    If file <> "" Then
        ret = ShellExecute(Me.hwnd, "open", file, "", "", SW_SHOWNORMAL)
    End If

End If

End Sub


Private Sub TraegerIcon_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
Dim file As String
If traeger = 7 And Traegerloaded = True Then
    file = GetMp3Playlist(traegerindex)
    If file <> "" Then
        traegericon.OLEDragMode = 1
        traegericon.OLEDrag
    End If
End If

End Sub


Private Sub TraegerIcon_OLECompleteDrag(Effect As Long)
traegericon.OLEDragMode = 0
End Sub

Private Sub TraegerIcon_OLEStartDrag(Data As DataObject, AllowedEffects As Long)
Dim file As String
If traeger = 7 And Traegerloaded = True Then
    file = GetMp3Playlist(traegerindex)
    If file <> "" Then
        Debug.Print "OLEStartDrag"
        Data.Clear
        Data.files.Clear
        Data.files.Add file
        Data.SetData , vbCFFiles
        'ret = ShellExecute(Me.hwnd, "open", file, "", "", SW_SHOWNORMAL)
    End If

End If


End Sub

Private Sub TraegerTitel_Click()
SetEingabePosition
End Sub



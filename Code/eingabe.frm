VERSION 4.00
Begin VB.Form eingabe 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "Eingabe"
   ClientHeight    =   6270
   ClientLeft      =   930
   ClientTop       =   1695
   ClientWidth     =   10050
   BeginProperty Font 
      name            =   "MS Sans Serif"
      charset         =   0
      weight          =   700
      size            =   8.25
      underline       =   0   'False
      italic          =   0   'False
      strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H80000008&
   Height          =   6675
   KeyPreview      =   -1  'True
   Left            =   870
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   ScaleHeight     =   6270
   ScaleWidth      =   10050
   Top             =   1350
   Width           =   10170
   Begin VB.TextBox dummyFeld 
      Height          =   405
      Left            =   9180
      TabIndex        =   21
      Text            =   "Text1"
      Top             =   5100
      Width           =   345
   End
   Begin Threed.SSPanel functions 
      Height          =   405
      Left            =   45
      TabIndex        =   60
      Top             =   5730
      Width           =   9000
      _Version        =   65536
      _ExtentX        =   15875
      _ExtentY        =   714
      _StockProps     =   15
      BevelOuter      =   1
      Begin Threed.SSCommand fkey 
         Height          =   345
         Index           =   0
         Left            =   30
         TabIndex        =   25
         Top             =   30
         Width           =   600
         _Version        =   65536
         _ExtentX        =   1058
         _ExtentY        =   609
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":0000
      End
      Begin Threed.SSCommand fkey 
         Height          =   345
         Index           =   1
         Left            =   960
         TabIndex        =   26
         Top             =   30
         Width           =   600
         _Version        =   65536
         _ExtentX        =   1058
         _ExtentY        =   609
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":001C
      End
      Begin Threed.SSCommand fkey 
         Height          =   345
         Index           =   2
         Left            =   1575
         TabIndex        =   27
         Top             =   30
         Width           =   600
         _Version        =   65536
         _ExtentX        =   1058
         _ExtentY        =   609
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":0038
      End
      Begin Threed.SSCommand fkey 
         Height          =   345
         Index           =   3
         Left            =   2190
         TabIndex        =   28
         Top             =   30
         Width           =   600
         _Version        =   65536
         _ExtentX        =   1058
         _ExtentY        =   609
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":0054
      End
      Begin Threed.SSCommand fkey 
         Height          =   345
         Index           =   4
         Left            =   2805
         TabIndex        =   29
         Top             =   30
         Width           =   600
         _Version        =   65536
         _ExtentX        =   1058
         _ExtentY        =   609
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":0070
      End
      Begin Threed.SSCommand fkey 
         Height          =   345
         Index           =   5
         Left            =   3720
         TabIndex        =   30
         Top             =   30
         Width           =   600
         _Version        =   65536
         _ExtentX        =   1058
         _ExtentY        =   609
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":008C
      End
      Begin Threed.SSCommand fkey 
         Height          =   345
         Index           =   6
         Left            =   4335
         TabIndex        =   31
         Top             =   30
         Width           =   600
         _Version        =   65536
         _ExtentX        =   1058
         _ExtentY        =   609
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":00A8
      End
      Begin Threed.SSCommand fkey 
         Height          =   345
         Index           =   7
         Left            =   4950
         TabIndex        =   32
         Top             =   30
         Width           =   600
         _Version        =   65536
         _ExtentX        =   1058
         _ExtentY        =   609
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":00C4
      End
      Begin Threed.SSCommand fkey 
         Height          =   345
         Index           =   8
         Left            =   5565
         TabIndex        =   33
         Top             =   30
         Width           =   600
         _Version        =   65536
         _ExtentX        =   1058
         _ExtentY        =   609
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":00E0
      End
      Begin Threed.SSCommand fkey 
         Height          =   345
         Index           =   9
         Left            =   6510
         TabIndex        =   34
         Top             =   30
         Width           =   600
         _Version        =   65536
         _ExtentX        =   1058
         _ExtentY        =   609
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":00FC
      End
      Begin Threed.SSCommand fkey 
         Height          =   345
         Index           =   10
         Left            =   7125
         TabIndex        =   35
         Top             =   30
         Width           =   600
         _Version        =   65536
         _ExtentX        =   1058
         _ExtentY        =   609
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":0118
      End
      Begin Threed.SSCommand fkey 
         Height          =   345
         Index           =   11
         Left            =   7740
         TabIndex        =   36
         Top             =   30
         Width           =   600
         _Version        =   65536
         _ExtentX        =   1058
         _ExtentY        =   609
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":0134
      End
      Begin Threed.SSCommand fkey 
         Height          =   345
         Index           =   12
         Left            =   8355
         TabIndex        =   37
         Top             =   30
         Width           =   615
         _Version        =   65536
         _ExtentX        =   1085
         _ExtentY        =   609
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":0150
      End
   End
   Begin Threed.SSPanel ListPanel 
      Height          =   1575
      Left            =   45
      TabIndex        =   40
      Top             =   1215
      Width           =   9000
      _Version        =   65536
      _ExtentX        =   15875
      _ExtentY        =   2778
      _StockProps     =   15
      BevelWidth      =   2
      BevelOuter      =   1
      Begin VB.PictureBox Listfeld 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BorderStyle     =   0  'None
         ForeColor       =   &H80000008&
         Height          =   1260
         Left            =   30
         ScaleHeight     =   1260
         ScaleWidth      =   8655
         TabIndex        =   47
         Top             =   270
         Width           =   8655
      End
      Begin VB.VScrollBar ListScroll 
         Height          =   1515
         LargeChange     =   5
         Left            =   8700
         TabIndex        =   48
         Top             =   30
         Width           =   255
      End
      Begin Threed.SSPanel lVersion 
         Height          =   240
         Left            =   6480
         TabIndex        =   46
         Top             =   30
         Width           =   1560
         _Version        =   65536
         _ExtentX        =   2752
         _ExtentY        =   423
         _StockProps     =   15
         Caption         =   "Version"
         Alignment       =   1
      End
      Begin Threed.SSPanel llaenge 
         Height          =   240
         Left            =   8040
         TabIndex        =   43
         Top             =   30
         Width           =   660
         _Version        =   65536
         _ExtentX        =   1164
         _ExtentY        =   423
         _StockProps     =   15
         Caption         =   "Länge"
         Alignment       =   1
      End
      Begin Threed.SSPanel LTitel 
         Height          =   240
         Left            =   555
         TabIndex        =   42
         Top             =   30
         Width           =   5535
         _Version        =   65536
         _ExtentX        =   9763
         _ExtentY        =   423
         _StockProps     =   15
         Caption         =   "Interpret: Titel"
         Alignment       =   1
      End
      Begin Threed.SSPanel LTrack 
         Height          =   240
         Left            =   30
         TabIndex        =   41
         Top             =   30
         Width           =   525
         _Version        =   65536
         _ExtentX        =   926
         _ExtentY        =   423
         _StockProps     =   15
         Caption         =   "Track"
      End
      Begin Threed.SSCommand lAuswahl 
         Height          =   240
         Index           =   1
         Left            =   6285
         TabIndex        =   45
         Top             =   30
         Width           =   195
         _Version        =   65536
         _ExtentX        =   344
         _ExtentY        =   423
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":016C
      End
      Begin Threed.SSCommand lAuswahl 
         Height          =   240
         Index           =   0
         Left            =   6090
         TabIndex        =   44
         Top             =   30
         Width           =   195
         _Version        =   65536
         _ExtentX        =   344
         _ExtentY        =   423
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":020E
      End
   End
   Begin Threed.SSFrame eingabeframe 
      Height          =   2850
      Left            =   60
      TabIndex        =   39
      Top             =   2850
      Width           =   9000
      _Version        =   65536
      _ExtentX        =   15875
      _ExtentY        =   5027
      _StockProps     =   14
      Begin VB.TextBox e 
         Height          =   285
         Index           =   17
         Left            =   1110
         MaxLength       =   160
         TabIndex        =   20
         Text            =   "17"
         Top             =   2460
         Width           =   7755
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   16
         Left            =   4350
         MaxLength       =   256
         TabIndex        =   19
         Text            =   "16"
         Top             =   2130
         Width           =   3885
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   15
         Left            =   2640
         MaxLength       =   2
         TabIndex        =   18
         Text            =   "15"
         Top             =   2130
         Width           =   675
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   14
         Left            =   1080
         MaxLength       =   2
         TabIndex        =   17
         Text            =   "14"
         Top             =   2130
         Width           =   675
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   13
         Left            =   6360
         MaxLength       =   80
         TabIndex        =   16
         Text            =   "13"
         Top             =   1800
         Width           =   2505
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   12
         Left            =   5700
         MaxLength       =   8
         TabIndex        =   15
         Text            =   "12"
         Top             =   1800
         Width           =   615
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   11
         Left            =   2130
         MaxLength       =   80
         TabIndex        =   14
         Text            =   "11"
         Top             =   1800
         Width           =   2115
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   10
         Left            =   1500
         MaxLength       =   8
         TabIndex        =   13
         Text            =   "10"
         Top             =   1800
         Width           =   585
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   9
         Left            =   7410
         MaxLength       =   10
         TabIndex        =   12
         Text            =   "9"
         Top             =   1470
         Width           =   1125
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   8
         Left            =   4680
         MaxLength       =   4
         TabIndex        =   11
         Text            =   "8"
         Top             =   1470
         Width           =   675
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   7
         Left            =   2430
         MaxLength       =   3
         TabIndex        =   10
         Text            =   "7"
         Top             =   1470
         Width           =   585
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   6
         Left            =   720
         MaxLength       =   5
         TabIndex        =   9
         Text            =   "6"
         Top             =   1470
         Width           =   585
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   5
         Left            =   5430
         MaxLength       =   80
         TabIndex        =   8
         Text            =   "5"
         Top             =   1140
         Width           =   2025
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   4
         Left            =   4800
         MaxLength       =   8
         TabIndex        =   7
         Text            =   "4"
         Top             =   1140
         Width           =   585
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   3
         Left            =   1470
         MaxLength       =   80
         TabIndex        =   6
         Text            =   "3"
         Top             =   1140
         Width           =   2475
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   2
         Left            =   840
         MaxLength       =   8
         TabIndex        =   5
         Text            =   "2"
         Top             =   1140
         Width           =   585
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   1
         Left            =   630
         MaxLength       =   80
         TabIndex        =   4
         Text            =   "1"
         Top             =   810
         Width           =   6825
      End
      Begin VB.TextBox e 
         Height          =   285
         Index           =   0
         Left            =   900
         MaxLength       =   80
         TabIndex        =   3
         Text            =   "0"
         Top             =   480
         Width           =   6555
      End
      Begin VB.TextBox track_e 
         Height          =   285
         Left            =   780
         TabIndex        =   2
         Text            =   "track_e"
         Top             =   150
         Width           =   675
      End
      Begin Threed.SSPanel Panel3D1 
         Height          =   960
         Left            =   7545
         TabIndex        =   83
         Top             =   465
         Width           =   1320
         _Version        =   65536
         _ExtentX        =   2328
         _ExtentY        =   1693
         _StockProps     =   15
         BevelOuter      =   1
         Begin VB.Image Cover 
            Appearance      =   0  'Flat
            Height          =   915
            Left            =   15
            Stretch         =   -1  'True
            Top             =   15
            Width           =   1275
         End
      End
      Begin Threed.SSPanel TiNummer 
         Height          =   285
         Left            =   2790
         TabIndex        =   82
         Top             =   150
         Width           =   885
         _Version        =   65536
         _ExtentX        =   1561
         _ExtentY        =   503
         _StockProps     =   15
         BevelOuter      =   1
      End
      Begin Threed.SSCommand PlaySound 
         Height          =   285
         Left            =   8580
         TabIndex        =   89
         Top             =   2130
         Width           =   285
         _Version        =   65536
         _ExtentX        =   503
         _ExtentY        =   503
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
      End
      Begin Threed.SSCommand jahr 
         Height          =   285
         Left            =   5400
         TabIndex        =   22
         Top             =   1470
         Width           =   285
         _Version        =   65536
         _ExtentX        =   503
         _ExtentY        =   503
         _StockProps     =   78
         ForeColor       =   16711680
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            name            =   "Arial"
            charset         =   0
            weight          =   400
            size            =   6
            underline       =   0   'False
            italic          =   0   'False
            strikethrough   =   0   'False
         EndProperty
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":02B0
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Soundfile:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   27
         Left            =   3450
         TabIndex        =   81
         Top             =   2190
         Width           =   870
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Qualität:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   26
         Left            =   1860
         TabIndex        =   80
         Top             =   2190
         Width           =   735
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Bewertung:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   25
         Left            =   90
         TabIndex        =   79
         Top             =   2190
         Width           =   975
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Aufnahmegerät:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   24
         Left            =   4335
         TabIndex        =   78
         Top             =   1860
         Width           =   1350
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Bemerkung:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   28
         Left            =   90
         TabIndex        =   77
         Top             =   2520
         Width           =   1020
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Aufnahmedatum:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   22
         Left            =   5955
         TabIndex        =   76
         Top             =   1530
         Width           =   1425
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Erscheinungsjahr:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   21
         Left            =   3135
         TabIndex        =   75
         Top             =   1530
         Width           =   1530
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Aufnahmequelle:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   23
         Left            =   90
         TabIndex        =   74
         Top             =   1860
         Width           =   1425
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Bpm:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   20
         Left            =   1950
         TabIndex        =   73
         Top             =   1530
         Width           =   435
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Min."
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   19
         Left            =   1425
         TabIndex        =   72
         Top             =   1530
         Width           =   375
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "echter Titel"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   13
         Left            =   7470
         TabIndex        =   71
         Top             =   210
         Width           =   990
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "vollständig"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   12
         Left            =   6000
         TabIndex        =   70
         Top             =   210
         Width           =   930
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Dolby"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   11
         Left            =   4950
         TabIndex        =   69
         Top             =   210
         Width           =   495
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Stereo"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   10
         Left            =   3870
         TabIndex        =   68
         Top             =   210
         Width           =   570
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Länge:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   18
         Left            =   90
         TabIndex        =   67
         Top             =   1530
         Width           =   600
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Musikart:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   17
         Left            =   4005
         TabIndex        =   66
         Top             =   1200
         Width           =   795
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Version:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   16
         Left            =   90
         TabIndex        =   65
         Top             =   1200
         Width           =   705
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Titel:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   15
         Left            =   90
         TabIndex        =   64
         Top             =   870
         Width           =   450
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Interpret:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   14
         Left            =   90
         TabIndex        =   63
         Top             =   540
         Width           =   795
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Titelnummer:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   9
         Left            =   1605
         TabIndex        =   62
         Top             =   210
         Width           =   1095
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Track:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   8
         Left            =   90
         TabIndex        =   61
         Top             =   210
         Width           =   570
      End
      Begin Threed.SSCommand ADatum 
         Height          =   285
         Left            =   8580
         TabIndex        =   23
         Top             =   1470
         Width           =   285
         _Version        =   65536
         _ExtentX        =   503
         _ExtentY        =   503
         _StockProps     =   78
         ForeColor       =   16711680
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            name            =   "Arial"
            charset         =   0
            weight          =   400
            size            =   6
            underline       =   0   'False
            italic          =   0   'False
            strikethrough   =   0   'False
         EndProperty
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":02CC
      End
      Begin Threed.SSCommand soundsuch 
         Height          =   285
         Left            =   8280
         TabIndex        =   24
         Top             =   2130
         Width           =   285
         _Version        =   65536
         _ExtentX        =   503
         _ExtentY        =   503
         _StockProps     =   78
         BevelWidth      =   1
         RoundedCorners  =   0   'False
         Outline         =   0   'False
         Picture         =   "eingabe.frx":02E8
      End
      Begin Threed.SSRibbon check 
         Height          =   315
         Index           =   3
         Left            =   8550
         TabIndex        =   84
         Top             =   120
         Width           =   360
         _Version        =   65536
         _ExtentX        =   635
         _ExtentY        =   556
         _StockProps     =   65
         Value           =   -1  'True
         GroupNumber     =   4
         GroupAllowAllUp =   -1  'True
         RoundedCorners  =   0   'False
         BevelWidth      =   0
         Outline         =   0   'False
         PictureUp       =   "eingabe.frx":0304
         PictureDn       =   "eingabe.frx":0320
      End
      Begin Threed.SSRibbon check 
         Height          =   315
         Index           =   2
         Left            =   6960
         TabIndex        =   85
         Top             =   120
         Width           =   360
         _Version        =   65536
         _ExtentX        =   635
         _ExtentY        =   556
         _StockProps     =   65
         Value           =   -1  'True
         GroupNumber     =   3
         GroupAllowAllUp =   -1  'True
         RoundedCorners  =   0   'False
         BevelWidth      =   0
         Outline         =   0   'False
         PictureUp       =   "eingabe.frx":033C
         PictureDn       =   "eingabe.frx":0358
      End
      Begin Threed.SSRibbon check 
         Height          =   315
         Index           =   1
         Left            =   5490
         TabIndex        =   86
         Top             =   120
         Width           =   360
         _Version        =   65536
         _ExtentX        =   635
         _ExtentY        =   556
         _StockProps     =   65
         Value           =   -1  'True
         GroupNumber     =   2
         GroupAllowAllUp =   -1  'True
         RoundedCorners  =   0   'False
         BevelWidth      =   0
         Outline         =   0   'False
         PictureUp       =   "eingabe.frx":0374
         PictureDn       =   "eingabe.frx":0390
      End
      Begin Threed.SSRibbon check 
         Height          =   315
         Index           =   0
         Left            =   4440
         TabIndex        =   87
         Top             =   120
         Width           =   360
         _Version        =   65536
         _ExtentX        =   635
         _ExtentY        =   556
         _StockProps     =   65
         Value           =   -1  'True
         GroupAllowAllUp =   -1  'True
         RoundedCorners  =   0   'False
         BevelWidth      =   0
         Outline         =   0   'False
         PictureUp       =   "eingabe.frx":03AC
         PictureDn       =   "eingabe.frx":03C8
      End
   End
   Begin Threed.SSFrame topframe 
      Height          =   1125
      Left            =   60
      TabIndex        =   38
      Top             =   0
      Width           =   9000
      _Version        =   65536
      _ExtentX        =   15875
      _ExtentY        =   1984
      _StockProps     =   14
      Begin VB.TextBox seite_e 
         Height          =   285
         Left            =   2280
         MaxLength       =   1
         TabIndex        =   1
         Text            =   "Text1"
         Top             =   150
         Width           =   315
      End
      Begin VB.TextBox index_e 
         Height          =   285
         Left            =   660
         TabIndex        =   0
         Text            =   "Text1"
         Top             =   150
         Width           =   975
      End
      Begin Threed.SSPanel panel2 
         Height          =   285
         Index           =   0
         Left            =   3420
         TabIndex        =   52
         Top             =   150
         Width           =   705
         _Version        =   65536
         _ExtentX        =   1244
         _ExtentY        =   503
         _StockProps     =   15
         BevelOuter      =   1
      End
      Begin Threed.SSPanel panel2 
         Height          =   285
         Index           =   1
         Left            =   5250
         TabIndex        =   54
         Top             =   150
         Width           =   705
         _Version        =   65536
         _ExtentX        =   1244
         _ExtentY        =   503
         _StockProps     =   15
         BevelOuter      =   1
      End
      Begin Threed.SSPanel panel2 
         Height          =   285
         Index           =   2
         Left            =   7200
         TabIndex        =   59
         Top             =   150
         Width           =   375
         _Version        =   65536
         _ExtentX        =   661
         _ExtentY        =   503
         _StockProps     =   15
         BevelOuter      =   1
      End
      Begin Threed.SSPanel TraegerTitel 
         Height          =   495
         Left            =   630
         TabIndex        =   57
         Top             =   540
         Width           =   7665
         _Version        =   65536
         _ExtentX        =   13520
         _ExtentY        =   873
         _StockProps     =   15
         BevelOuter      =   1
         Alignment       =   0
      End
      Begin Threed.SSPanel panel2 
         Height          =   285
         Index           =   3
         Left            =   8340
         TabIndex        =   88
         Top             =   150
         Width           =   375
         _Version        =   65536
         _ExtentX        =   661
         _ExtentY        =   503
         _StockProps     =   15
         BevelOuter      =   1
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Tracks:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   5
         Left            =   7650
         TabIndex        =   58
         Top             =   180
         Width           =   660
      End
      Begin VB.Label text 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Titel:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   6
         Left            =   75
         TabIndex        =   56
         Top             =   525
         Width           =   450
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Min.   Seiten:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   4
         Left            =   6000
         TabIndex        =   55
         Top             =   180
         Width           =   1155
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Min.   Rest:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   3
         Left            =   4185
         TabIndex        =   53
         Top             =   180
         Width           =   1005
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Länge:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   2
         Left            =   2790
         TabIndex        =   51
         Top             =   180
         Width           =   600
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Seite:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   1
         Left            =   1725
         TabIndex        =   50
         Top             =   180
         Width           =   510
      End
      Begin VB.Label text 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Index:"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   0
         Left            =   90
         TabIndex        =   49
         Top             =   180
         Width           =   540
      End
      Begin VB.Image TraegerIcon 
         Appearance      =   0  'Flat
         Height          =   480
         Left            =   8385
         Top             =   540
         Width           =   480
      End
   End
   Begin KeyStatLib.MhState KeyStatus 
      Height          =   465
      Left            =   9300
      TabIndex        =   90
      TabStop         =   0   'False
      Top             =   4110
      Width           =   405
      _Version        =   65536
      _ExtentX        =   714
      _ExtentY        =   820
      _StockProps     =   65
      Value           =   -1  'True
      Style           =   1
      TimerInterval   =   100
      MouseIcon       =   "eingabe.frx":03E4
   End
   Begin MSComDlg.CommonDialog dialog 
      Left            =   9120
      Top             =   2310
      _Version        =   65536
      _ExtentX        =   847
      _ExtentY        =   847
      _StockProps     =   0
   End
End
Attribute VB_Name = "eingabe"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
DefInt A-Z

Dim ListingStart As Integer
Dim MarkedField As Integer
Sub fillHeader()
If Traegerloaded = True Then
  panel2(0).Caption = Format$(TrV.Laenge, "###0.00")
  panel2(1).Caption = Format$(TrV.Rest, "###0.00")
  panel2(2).Caption = Format$(TrV.Seiten)
  panel2(3).Caption = Format$(TrV.Tracks)
  GetTraegerTitel TrV.Bezeichnung, TrTi
  TraegerTitel.Caption = Trim$(TrTi.Titel1) + Chr$(13) + Trim$(TrTi.Titel2)
  a$ = GetFile("bildfile.dir", TrV.CoverBild)
  If TrV.CoverBild = 0 Then a$ = HauptPfad + "\img\pfp2.bmp"
  Cover.Picture = LoadPicture(a$)

Else
  For i = 0 To 3: panel2(i).Caption = "": Next
  TraegerTitel.Caption = ""
  a$ = HauptPfad + "\img\pfp2.bmp"
  Cover.Picture = LoadPicture(a$)
End If
End Sub

Sub InitFields()
CurrentTi.Traeger = Traeger
CurrentTi.Index = TraegerIndex
CurrentTi.Seite = TragerSeite
Track = CInt(track_e.text)
CurrentTi.Track = Track
ff = FreeFile
Open tmp + "\~wm2in" + Format$(Seite) + ".tmp" For Random As #ff Len = Len(el)
If LOF(ff) / Len(el) >= Track Then
  Get #ff, Track, el
Else
  el.chk = 0
End If
Close #ff
If el.chk = 0 Then
  e(0).text = ""
  e(1).text = ""
  e(6).text = ""
  e(7).text = ""
  e(14).text = ""
  e(15).text = ""
  e(16).text = ""
  e(17).text = ""
  TiNummer.Caption = Lang(118)
Else
  GetTitel Ti, el.Titelnummer
  CurrentTi = Ti
  TiNummer.Caption = Format$(Ti.Nummer)
  e(0).text = Trim$(Ti.Interpret)
  e(1).text = Trim$(Ti.Titel)
  e(2).text = Format$(Ti.version)
  e(4).text = Format$(Ti.MusikArt)
  e(6).text = Format$(Ti.Laenge, "###0.00")
  e(7).text = Format$(Ti.bpm)
  e(8).text = Format$(Ti.Erscheinungsjahr)
  e(9).text = Long2Dat$(Ti.Aufnahmedatum)
  e(10).text = Format$(Ti.Quelle)
  e(12).text = Format$(Ti.Aufnahmegeraet)
  e(14).text = Trim$(Ti.Bewertung)
  e(15).text = Trim$(Ti.Qualitaet)
  e(16).text = GetFile("sndfile.dir", Ti.Soundfile)
  e(17).text = GetFile("bemerk.dat", Ti.Bemerkung)
  For i = 0 To 3
    check(i).Value = False
  Next
  If (Ti.InfoByte And 4) = 4 Then check(0).Value = True
  If (Ti.InfoByte And 8) = 8 Then check(1).Value = True
  If (Ti.InfoByte And 32) = 32 Then check(2).Value = True
  If (Ti.InfoByte And 64) = 64 Then check(3).Value = True
End If
End Sub
Sub PaintListFeld()
Dim hg As Long

X1 = 0: X2 = ListFeld.Width
Y1 = 0: Y2 = ListFeld.Height
If Traegerloaded = True And Seite > 0 Then
  ListFeld.FontSize = 8: ListFeld.FontBold = True
  ListFeld.FontName = "Arial"
  ff = FreeFile
  Open tmp + "\~wm2in" + Format$(Seite) + ".tmp" For Random As #ff Len = Len(el)
  ListScroll.LargeChange = Y2 / (15 * tpy) - 1
  ListScroll.Min = 1
  ListScroll.Max = LOF(ff) / Len(el)
  ListingStart = ListScroll.Value
  If ListingStart > LOF(ff) / Len(el) Then ListingStart = 1
  y = 0
  If ListingStart = 0 Then ListingStart = 1
  For i = ListingStart To LOF(ff) / Len(el)
    hg = RGB(255, 255, 255)
    If i = MarkedField Then
      hg = RGB(200, 255, 255)
    End If
    ListFeld.Line (0, y)-(403 * tpx, y + 15 * tpy), hg, BF
    Get #ff, i, el
    a$ = Format$(i)
    X = 33 * tpx - TextWidth(a$)
    ListFeld.CurrentX = X: ListFeld.CurrentY = y
    ListFeld.ForeColor = 0: ListFeld.Print a$;
    If el.chk = 1 Then
      ListFeld.CurrentX = 37 * tpx
      ListFeld.ForeColor = RGB(0, 0, 128)
      ListFeld.Print Trim$(el.Interpret_Titel)
      ListFeld.Line (403 * tpx, y)-(533 * tpx, y + 15 * tpy), hg, BF
      ListFeld.ForeColor = RGB(255, 0, 0)
      ListFeld.CurrentX = 405 * tpx: ListFeld.CurrentY = y
      ListFeld.Print Trim$(el.Info)
      ListFeld.Line (533 * tpx, y)-(X2, y + 15 * tpy), hg, BF
      ListFeld.ForeColor = RGB(0, 128, 0)
      a$ = Format$(el.Laenge, "##0.00")
      ListFeld.CurrentX = X2 - TextWidth(a$): ListFeld.CurrentY = y
      ListFeld.Print a$
    End If
    y = y + 15 * tpy
    If y >= Y2 - 15 * tpy Then Exit For
  Next
  Close #ff
  For X = y To Y2 Step 15 * tpy
    ListFeld.Line (0, X)-(X2, X + 14 * tpy), RGB(255, 255, 255), BF
  Next
End If
X = 34: c& = RGB(192, 192, 192)
ListFeld.Line (X * tpx, Y1)-(X * tpx, Y2), c&
X = 403
ListFeld.Line (X * tpx, Y1)-(X * tpx, Y2), c&
X = 533
ListFeld.Line (X * tpx, Y1)-(X * tpx, Y2), c&
Y2 = Fix(Y2 / tpy)
For y = 15 To Y2 - 15 Step 15
  ListFeld.Line (0, y * tpy)-(X2, y * tpy), c&
Next
End Sub

Private Sub ADatum_Click()
e(9).text = Datum$
SetEingabePosition
End Sub

Private Sub Cover_Click()
'bigcover.Show
End Sub

Private Sub dummyFeld_GotFocus()
SaveTrack
End Sub


Private Sub e_Change(Index As Integer)
Select Case Index
  Case 2: e(3).text = GetTab("version", Val(e(2).text))
             e(3).Tag = e(3).text
  Case 4: e(5).text = GetTab("musikart", Val(e(4).text))
             e(5).Tag = e(5).text
  Case 10: e(11).text = GetTab("quelle", Val(e(10).text))
             e(11).Tag = e(11).text
  Case 12: e(13).text = GetTab("aufnahme", Val(e(12).text))
             e(13).Tag = e(13).text
            
End Select

End Sub

Private Sub e_GotFocus(Index As Integer)
eingabeframe.Enabled = True
If EingabePos < 0 And Index > 0 Then e(0).SetFocus: Exit Sub
EingabePos = Index
Select Case Index
  Case 0
    If CurrentTi.Traeger = 0 Then
      'Felder initialisieren
      InitFields
    End If
End Select
keystatus.Value = True
SendKeys "{home}+{end}"
End Sub

Private Sub e_LostFocus(Index As Integer)
On Error Resume Next
Select Case Index
  Case 2:
          If InStr(e(2).text, "*") > 0 Then
             e(2).text = Format$(GetNextFreeTab("version"))
             e(3).text = "": e(2).SetFocus
          Else
             e(3).text = GetTab("version", Val(e(2).text))
             e(3).Tag = e(3).text
             If Trim$(e(3).text) <> "" Then e(4).SetFocus
          End If
  Case 3:
          If e(3).text <> e(3).Tag Then
            PutTab "version", Val(e(2).text), e(3).text
          End If
  Case 4:
          If InStr(e(4).text, "*") > 0 Then
             e(4).text = Format$(GetNextFreeTab("musikart"))
             e(5).text = "": e(4).SetFocus
          Else
             e(5).text = GetTab("musikart", Val(e(4).text))
             e(5).Tag = e(5).text
             If Trim$(e(5).text) <> "" Then e(6).SetFocus
          End If
  Case 5:
          If e(5).text <> e(5).Tag Then
            PutTab "musikart", Val(e(4).text), e(5).text
          End If
  Case 6: e(6).text = Format$(CSng(e(6).text), "##0.00")
          If Err > 0 Then e(6).text = Format$(0, "##0.00")
  Case 8: a$ = Trim$(e(8).text)
          If Len(a$) < 4 Then a$ = left$(Format$(Year(Now)), 4 - Len(a$)) + a$
          e(8).text = a$
  Case 9: a$ = e(9).text
          If a$ <> "" Then a$ = DatumsAuswertung(a$): Else a$ = Datum$()
          e(9).text = a$
  Case 10:
          If InStr(e(10).text, "*") > 0 Then
             e(10).text = Format$(GetNextFreeTab("quelle"))
             e(11).text = "": e(10).SetFocus
          Else
             e(11).text = GetTab("quelle", Val(e(10).text))
             e(11).Tag = e(11).text
             If Trim$(e(11).text) <> "" Then e(12).SetFocus
          End If
  Case 11:
          If e(11).text <> e(11).Tag Then
            PutTab "quelle", Val(e(10).text), e(11).text
          End If
  Case 12:
          If InStr(e(12).text, "*") > 0 Then
             e(12).text = Format$(GetNextFreeTab("aufnahme"))
             e(13).text = "": e(12).SetFocus
          Else
             e(13).text = GetTab("aufnahme", Val(e(12).text))
             e(13).Tag = e(13).text
             If Trim$(e(13).text) <> "" Then e(14).SetFocus
          End If
  Case 13:
          If e(13).text <> e(13).Tag Then
            PutTab "aufnahme", Val(e(12).text), e(13).text
          End If

End Select

End Sub

Private Sub fkey_Click(Index As Integer)
On Error Resume Next
FTaste Index, 0
If eingabevisible = True Then
  SetEingabePosition
End If
End Sub

Private Sub fkey_MouseMove(Index As Integer, Button As Integer, Shift As Integer, X As Single, y As Single)
Static lastx, lasty
If lastx <> X Or lasty <> y Then
  lastx = X: lasty = y
  menu1.quickhelp.Caption = Lang(43 + Index)
End If

End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
Select Case KeyCode
  Case 27: FTaste 0, Shift
  Case 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123
           FTaste KeyCode - 111, Shift
  Case 13: KeyCode = 0: SendKeys "{tab}"
  Case 33: If EingabePos > -2 And Shift = 0 And ListScroll.Value > ListScroll.Min Then ListScroll.Value = ListScroll.Value - 1
           If EingabePos > -2 And Shift > 0 And ListScroll.Value - ListScroll.LargeChange > ListScroll.Min Then
             ListScroll.Value = ListScroll.Value - ListScroll.LargeChange
           ElseIf EingabePos > -2 And Shift > 0 Then
             ListScroll.Value = ListScroll.Min
           End If
  Case 34: If EingabePos > -2 And Shift = 0 And ListScroll.Value < ListScroll.Max Then ListScroll.Value = ListScroll.Value + 1
           If EingabePos > -2 And Shift > 0 And ListScroll.Value + ListScroll.LargeChange < ListScroll.Max Then
             ListScroll.Value = ListScroll.Value + ListScroll.LargeChange
           ElseIf EingabePos > -2 And Shift > 0 Then
             ListScroll.Value = ListScroll.Max
           End If
  Case 38: If EingabePos > -3 Then EingabePos = EingabePos - 1: SetEingabePosition
  Case 40: If EingabePos < 17 Then EingabePos = EingabePos + 1: SetEingabePosition
  
End Select
End Sub

Private Sub Form_KeyPress(KeyAscii As Integer)
If KeyAscii = 13 Then KeyAscii = 0
End Sub

Private Sub Form_Load()
eingabe.Width = 9225
tpx = Screen.TwipsPerPixelX
tpy = Screen.TwipsPerPixelY
eingabevisible = True
Dim rec As Rect
openini "wmusik20"
selectTopic "Fenster"
a$ = GetINI$("Eingabe")
If inierror = 0 Then
  String2Rect a$, rec
Else
  rec.left = menu1.left: rec.top = menu1.top + 1140
  rec.right = 9225: rec.bottom = 6585
End If
If rec.left > Screen.Width - eingabe.Width Then rec.left = Screen.Width - eingabe.Width
If rec.top > Screen.Height - eingabe.Height Then rec.top = Screen.Height - eingabe.Height
eingabe.left = rec.left: eingabe.top = rec.top
eingabe.Width = rec.right: eingabe.Height = rec.bottom
eingabe.Icon = LoadPicture(HauptPfad + "\img\" + Format$(Traeger) + ".ico")
eingabe.TraegerIcon.Picture = LoadPicture(HauptPfad + "\img\" + Format$(Traeger) + ".ico")
eingabe.Caption = Lang(29 + Traeger)
eingabe.LTrack.Caption = Lang(65)
eingabe.LTitel.Caption = Lang(71) + ": " + Lang(72)
eingabe.lVersion.Caption = Lang(73)
eingabe.llaenge.Caption = Lang(75)
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
For i = 0 To 20
  text(8 + i) = Lang(65 + i) + ":"
Next
track_e.left = text(8).left + text(8).Width + 2 * tpx
text(9).left = track_e.left + track_e.Width + 10 * tpx
TiNummer.left = text(9).left + text(9).Width + 2 * tpx
For i = 3 To 0 Step -1
  text(10 + i).left = check(i).left - 2 * tpx - text(10 + i).Width
  If i > 0 Then check(i - 1).left = text(10 + i).left - 6 * tpx - check(i - 1).Width
Next
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
Open DatenPfad + "\" + TraegerBez(Traeger) + ".dir" For Random As #ff Len = Len(TrV)
index_e.text = Format$(LOF(ff) / Len(TrV) + 1)
Close #ff


DoEvents
eingabeframe.Enabled = False
MaskPos = -1
Show
DoEvents
index_e.SetFocus

For i = 0 To 3
  check(i).PictureDn = LoadPicture(HauptPfad + "\img\check1.bmp")
  check(i).PictureUp = LoadPicture(HauptPfad + "\img\check0.bmp")
Next
j = Val(right$(Year(Now), 2))
e(8).text = Year(Now)
e(9).text = Datum()
d = Day(Now)
jahr.Picture = gfx.kalender.GraphicCell(j)
ADatum.Picture = gfx.kalender.GraphicCell(d)
soundsuch.Picture = LoadPicture(HauptPfad + "\img\folder1.bmp")
PlaySound.Picture = LoadPicture(HauptPfad + "\img\speaker.bmp")

For i = 0 To 12
  fkey(i).Picture = LoadPicture(HauptPfad + "\img\f" + Format$(i) + ".bmp")
Next
Cover.Picture = LoadPicture(HauptPfad + "\img\pfp2.bmp")
End Sub

Private Sub Form_Resize()
If eingabe.WindowState <> 1 Then
  eingabe.Width = 9225
  If eingabe.Height < 6000 Then eingabe.Height = 6000
  functions.top = eingabe.Height - functions.Height - 450
  eingabeframe.top = functions.top - 2925
  ListPanel.Height = eingabeframe.top - ListPanel.top - 3 * tpy
  ListFeld.Height = ListPanel.Height - 21 * tpy
  ListScroll.Height = ListPanel.Height - 4 * tpy
End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
If eingabe.WindowState <> 0 Then eingabe.WindowState = 0: DoEvents
If traegerstammvisible = True Then Unload traegerstamm
eingabevisible = False
Dim rec As Rect
openini "wmusik20"
selectTopic "Fenster"
rec.top = eingabe.top: rec.left = eingabe.left
rec.right = eingabe.Width: rec.bottom = eingabe.Height
a$ = Rect2String(rec)
putini "Eingabe=" + a$
closeini
Traegerloaded = False
End Sub

Private Sub functions_Click()
SetEingabePosition
End Sub

Private Sub index_e_GotFocus()
SendKeys "{home}+{end}"
If Traegerloaded = True Then SaveTraeger
EingabePos = -3
Traegerloaded = False
eingabeframe.Enabled = False
ListPanel.Enabled = False
End Sub


Private Sub jahr_Click()
e(8).text = Format$(Year(Now))
SetEingabePosition
End Sub

Private Sub lAuswahl_Click(Index As Integer)

SetEingabePosition
End Sub

Private Sub ListFeld_Click()

SetEingabePosition
End Sub

Private Sub ListFeld_Paint()
Select Case ListingAnzeige
  Case 0: lVersion.Caption = Lang(73)
  Case 1: lVersion.Caption = Lang(74)
  Case 2: lVersion.Caption = Lang(88)
  Case 3: lVersion.Caption = Lang(81)
  Case 4: lVersion.Caption = Lang(77)
  Case 5: lVersion.Caption = Lang(82)
End Select
PaintListFeld

End Sub

Private Sub ListPanel_Click()
SetEingabePosition
End Sub

Private Sub ListScroll_Change()
SetEingabePosition
PaintListFeld
End Sub

Private Sub llaenge_Click()
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

Private Sub seite_e_GotFocus()
Dim NextFree As Long, antwort As Integer
SendKeys "{home}+{end}"
If Traegerloaded = True And EingabePos > -2 And TrV.Seiten = 1 Then
  index_e.SetFocus
End If

EingabePos = -2
DoEvents
If Traegerloaded = False Then
  TraegerIndex = Val(index_e.text)
  If LoadTraeger() = True Then
    Traegerloaded = True
    If TrV.Seiten > 1 Then ListFeld.Refresh
  Else
    ff = FreeFile
    Open DatenPfad + "\" + TraegerBez(Traeger) + ".dir" For Random As #ff Len = Len(TrV)
    NextFree = Format$(LOF(ff) / Len(TrV) + 1)
    Close #ff
    LangPar 2, Format$(TraegerIndex)
    antwort = 6
    If TraegerIndex = NextFree + 1 Then
      LangPar 1, Format$(NextFree)
      antwort = MsgBox(Lang(101) + Lang(98), 36, Programm + " " + version + ": " + Lang(99))
    ElseIf TraegerIndex > NextFree + 1 Then
      LangPar 1, Format$(NextFree) + " - " + Format$(TraegerIndex - 1)
      antwort = MsgBox(Lang(101) + Lang(100), 36, Programm + " " + version + ": " + Lang(99))
    End If
    If antwort = 6 Then
      'eingabe.Enabled = False
      traegerstamm.Show
    Else
      index_e.SetFocus
    End If
  End If
  seite_e.text = "1"
End If
fillHeader
If Traegerloaded = True Then
  eingabeframe.Enabled = True
  ListPanel.Enabled = False
  If TrV.Seiten = 1 Then track_e.SetFocus: Seite = 1
End If
End Sub


Private Sub soundsuch_Click()
On Error GoTo ErrHandler
If EingabePos >= 0 Then
  dialog.filename = e(16).text
  dialog.Filter = "Wave Dateien (*.wav)|*.wav|MIDI Dateien (*.mid)|*.mid|Alle Dateien (*.*)|*.*"
  dialog.FilterIndex = 0
  dialog.DefaultExt = ".wav"
  dialog.DialogTitle = Lang(97)
  dialog.Flags = &H4&
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
Seite = Val(seite_e.text)
CurrentTi.Traeger = 0
CurrentTi.Nummer = 0
eingabeframe.Enabled = True
ListPanel.Enabled = True
SendKeys "{home}+{end}"
EingabePos = -1
ListFeld.Refresh
Track = 0
End Sub


Private Sub TraegerTitel_Click()
SetEingabePosition
End Sub



VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{27395F88-0C0C-101B-A3C9-08002B2F49FB}#1.1#0"; "PICCLP32.OCX"
Begin VB.Form gfx 
   Caption         =   "Grafiken"
   ClientHeight    =   5175
   ClientLeft      =   2760
   ClientTop       =   2370
   ClientWidth     =   8625
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'ZReihenfolge
   ScaleHeight     =   5175
   ScaleWidth      =   8625
   Begin MSComctlLib.ImageList TrImageList 
      Left            =   1380
      Top             =   4140
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   32
      ImageHeight     =   32
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   9
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "gfx.frx":0000
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "gfx.frx":0C52
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "gfx.frx":18A4
            Key             =   ""
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "gfx.frx":24F6
            Key             =   ""
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "gfx.frx":3148
            Key             =   ""
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "gfx.frx":3D9A
            Key             =   ""
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "gfx.frx":49EC
            Key             =   ""
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "gfx.frx":563E
            Key             =   ""
         EndProperty
         BeginProperty ListImage9 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "gfx.frx":6290
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin PicClip.PictureClip Kalender 
      Left            =   5880
      Top             =   2760
      _ExtentX        =   4233
      _ExtentY        =   4233
      _Version        =   393216
      Rows            =   10
      Cols            =   10
      Picture         =   "gfx.frx":6EE2
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   16
      Left            =   2160
      Picture         =   "gfx.frx":19B34
      Top             =   4080
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   15
      Left            =   660
      Picture         =   "gfx.frx":1A26E
      Top             =   4020
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   14
      Left            =   60
      Picture         =   "gfx.frx":1A9A8
      Top             =   4020
      Width           =   555
   End
   Begin VB.Image traegerbw 
      Height          =   240
      Index           =   8
      Left            =   4200
      Picture         =   "gfx.frx":1B0E2
      Top             =   300
      Width           =   240
   End
   Begin VB.Image traegerbw 
      Height          =   240
      Index           =   7
      Left            =   3840
      Picture         =   "gfx.frx":1B424
      Top             =   240
      Width           =   240
   End
   Begin VB.Image traeger 
      Height          =   240
      Index           =   8
      Left            =   4200
      Picture         =   "gfx.frx":1B766
      Top             =   0
      Width           =   240
   End
   Begin VB.Image traeger 
      Height          =   240
      Index           =   7
      Left            =   3900
      Picture         =   "gfx.frx":1BAA8
      Top             =   0
      Width           =   240
   End
   Begin VB.Image traegericon 
      Height          =   480
      Index           =   8
      Left            =   5040
      Picture         =   "gfx.frx":1BDEA
      Top             =   3480
      Width           =   480
   End
   Begin VB.Image traegericon 
      Height          =   480
      Index           =   7
      Left            =   4440
      Picture         =   "gfx.frx":1C9AC
      Top             =   3480
      Width           =   480
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   13
      Left            =   3660
      Picture         =   "gfx.frx":1D6A6
      Top             =   3720
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   12
      Left            =   3060
      Picture         =   "gfx.frx":1DDE0
      Top             =   3720
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   11
      Left            =   2460
      Picture         =   "gfx.frx":1E51A
      Top             =   3720
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   10
      Left            =   1860
      Picture         =   "gfx.frx":1EC54
      Top             =   3720
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   9
      Left            =   1260
      Picture         =   "gfx.frx":1F38E
      Top             =   3720
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   8
      Left            =   660
      Picture         =   "gfx.frx":1FAC8
      Top             =   3720
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   7
      Left            =   60
      Picture         =   "gfx.frx":20202
      Top             =   3720
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   6
      Left            =   3660
      Picture         =   "gfx.frx":2093C
      Top             =   3360
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   5
      Left            =   3060
      Picture         =   "gfx.frx":211CE
      Top             =   3360
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   4
      Left            =   2460
      Picture         =   "gfx.frx":21908
      Top             =   3360
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   3
      Left            =   1860
      Picture         =   "gfx.frx":22042
      Top             =   3360
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   2
      Left            =   1260
      Picture         =   "gfx.frx":2277C
      Top             =   3360
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   1
      Left            =   660
      Picture         =   "gfx.frx":22EB6
      Top             =   3360
      Width           =   555
   End
   Begin VB.Image fkey 
      Height          =   285
      Index           =   0
      Left            =   60
      Picture         =   "gfx.frx":235F0
      Top             =   3360
      Width           =   555
   End
   Begin VB.Image pfp2 
      Height          =   795
      Left            =   6060
      Picture         =   "gfx.frx":23D2A
      Stretch         =   -1  'True
      Top             =   0
      Width           =   1215
   End
   Begin VB.Image speaker 
      Height          =   240
      Left            =   2520
      Picture         =   "gfx.frx":52644
      Top             =   1560
      Width           =   240
   End
   Begin VB.Image folder1 
      Height          =   195
      Left            =   2520
      Picture         =   "gfx.frx":52780
      Top             =   1200
      Width           =   240
   End
   Begin VB.Image check 
      Height          =   240
      Index           =   1
      Left            =   360
      Picture         =   "gfx.frx":5286A
      Top             =   3000
      Width           =   270
   End
   Begin VB.Image check 
      Height          =   240
      Index           =   0
      Left            =   60
      Picture         =   "gfx.frx":529AC
      Top             =   3000
      Width           =   270
   End
   Begin VB.Image traegericon 
      Height          =   480
      Index           =   6
      Left            =   2520
      Picture         =   "gfx.frx":52AEE
      Top             =   2460
      Width           =   480
   End
   Begin VB.Image traegericon 
      Height          =   480
      Index           =   5
      Left            =   2160
      Picture         =   "gfx.frx":536B0
      Top             =   2460
      Width           =   480
   End
   Begin VB.Image traegericon 
      Height          =   480
      Index           =   4
      Left            =   1620
      Picture         =   "gfx.frx":54272
      Top             =   2460
      Width           =   480
   End
   Begin VB.Image traegericon 
      Height          =   480
      Index           =   3
      Left            =   1080
      Picture         =   "gfx.frx":54F6C
      Top             =   2400
      Width           =   480
   End
   Begin VB.Image traegericon 
      Height          =   480
      Index           =   2
      Left            =   540
      Picture         =   "gfx.frx":55C66
      Top             =   2460
      Width           =   480
   End
   Begin VB.Image traegericon 
      Height          =   480
      Index           =   1
      Left            =   0
      Picture         =   "gfx.frx":56960
      Top             =   2400
      Width           =   480
   End
   Begin VB.Image drucklogo_sw 
      Height          =   900
      Left            =   3240
      Picture         =   "gfx.frx":5765A
      Top             =   1800
      Width           =   3960
   End
   Begin VB.Image drucklogo_farbig 
      Height          =   900
      Left            =   3240
      Picture         =   "gfx.frx":5A372
      Top             =   840
      Width           =   3960
   End
   Begin VB.Image kpu1 
      Height          =   90
      Left            =   3495
      Picture         =   "gfx.frx":5D166
      Top             =   645
      Width           =   135
   End
   Begin VB.Image kpr1 
      Height          =   135
      Left            =   3255
      Picture         =   "gfx.frx":5D218
      Top             =   630
      Width           =   90
   End
   Begin VB.Image kpl1 
      Height          =   135
      Left            =   3060
      Picture         =   "gfx.frx":5D2BE
      Top             =   570
      Width           =   90
   End
   Begin VB.Image kpu0 
      Height          =   90
      Left            =   2820
      Picture         =   "gfx.frx":5D364
      Top             =   615
      Width           =   135
   End
   Begin VB.Image kpr0 
      Height          =   135
      Left            =   2655
      Picture         =   "gfx.frx":5D416
      Top             =   600
      Width           =   90
   End
   Begin VB.Image kpl0 
      Height          =   135
      Left            =   2490
      Picture         =   "gfx.frx":5D4BC
      Top             =   615
      Width           =   90
   End
   Begin VB.Image traegerbw 
      Height          =   240
      Index           =   6
      Left            =   3540
      Picture         =   "gfx.frx":5D562
      Top             =   285
      Width           =   240
   End
   Begin VB.Image traegerbw 
      Height          =   240
      Index           =   5
      Left            =   3315
      Picture         =   "gfx.frx":5D8A4
      Top             =   285
      Width           =   240
   End
   Begin VB.Image traegerbw 
      Height          =   240
      Index           =   4
      Left            =   3105
      Picture         =   "gfx.frx":5DBE6
      Top             =   285
      Width           =   240
   End
   Begin VB.Image traegerbw 
      Height          =   240
      Index           =   3
      Left            =   2880
      Picture         =   "gfx.frx":5DF28
      Top             =   270
      Width           =   240
   End
   Begin VB.Image traegerbw 
      Height          =   240
      Index           =   2
      Left            =   2640
      Picture         =   "gfx.frx":5E26A
      Top             =   285
      Width           =   240
   End
   Begin VB.Image traegerbw 
      Height          =   240
      Index           =   1
      Left            =   2385
      Picture         =   "gfx.frx":5E5AC
      Top             =   300
      Width           =   240
   End
   Begin VB.Image traeger 
      Height          =   240
      Index           =   6
      Left            =   3555
      Picture         =   "gfx.frx":5E8EE
      Top             =   15
      Width           =   240
   End
   Begin VB.Image traeger 
      Height          =   240
      Index           =   5
      Left            =   3315
      Picture         =   "gfx.frx":5EC30
      Top             =   60
      Width           =   240
   End
   Begin VB.Image traeger 
      Height          =   240
      Index           =   4
      Left            =   3075
      Picture         =   "gfx.frx":5EF72
      Top             =   45
      Width           =   240
   End
   Begin VB.Image traeger 
      Height          =   240
      Index           =   3
      Left            =   2850
      Picture         =   "gfx.frx":5F2B4
      Top             =   45
      Width           =   240
   End
   Begin VB.Image traeger 
      Height          =   240
      Index           =   2
      Left            =   2610
      Picture         =   "gfx.frx":5F5F6
      Top             =   60
      Width           =   240
   End
   Begin VB.Image traeger 
      Height          =   240
      Index           =   1
      Left            =   2385
      Picture         =   "gfx.frx":5F938
      Top             =   60
      Width           =   240
   End
End
Attribute VB_Name = "gfx"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub WCGI1_Error(Number As Integer, Description As String, Scode As Long, Source As String, HelpFile As String, HelpContext As Long, CancelDisplay As Boolean)

End Sub



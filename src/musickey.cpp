#include "musickey.h"


static int harms[24][6] = {
        {1,3,23,2,0,0},			// 1:  G#m
        {2,1,24,4,0,0},			// 2:  B
        {3,1,5,4,0,0},			// 3:  D#m
        {4,2,6,3,0,0},			// 4:  F#
        {5,3,7,6,0,0},			// 5:  A#m
        {6,4,8,5,0,0},			// 6:  C#
        {7,5,9,8,0,0},			// 7:  Fm
        {8,6,10,7,0,0},			// 8:  G#
        {9,7,11,10,0,0},		// 9:  Cm
        {10,8,12,9,0,0},		// 10: D#
        {11,9,13,12,0,0},		// 11: Gm
        {12,10,14,11,0,0},		// 12: A#
        {13,11,15,14,0,0},		// 13: Dm
        {14,12,16,13,0,0},		// 14: F
        {15,13,17,16,0,0},		// 15: Am
        {16,14,18,15,0,0},		// 16: C
        {17,15,19,18,0,0},		// 17: Em
        {18,16,20,17,0,0},		// 18: G
        {19,17,21,20,0,0},		// 19: Bm
        {20,18,22,19,0,0},		// 20: D
        {21,19,23,22,0,0},		// 21: F#m
        {22,20,24,21,0,0},		// 22: A
        {23,21,1,24,0,0},		// 23: C#m
        {24,22,2,23,0,0}		// 24: E

};

void getHarmonicKeys(std::set<int> &harmonics, int key)
{
    harmonics.clear();
    if (key<1 || key>24) return;
    MusicKey k(key);
    harmonics.insert(k.wmKeyId());                      // sameKey
    harmonics.insert(k.nextKey().wmKeyId());            // nextKey
    harmonics.insert(k.previousKey().wmKeyId());        // previousKey
    harmonics.insert(k.minorMajorSwitch().wmKeyId());   // minorMajorSwitch
    harmonics.insert(k.minorMajorJump3().wmKeyId());         // keySwitch3
    harmonics.insert(k.minorMajorJump1().wmKeyId());         // keySwitch1
    harmonics.insert(k.plus1Semitone().wmKeyId());      // plus1Semitone
    harmonics.insert(k.minus1Semitone().wmKeyId());     // minus1Semitone
    harmonics.insert(k.plus2Semitone().wmKeyId());      // plus2Semitone
    harmonics.insert(k.minus2Semitone().wmKeyId());     // minus2Semitone
    harmonics.insert(k.avbBoost().wmKeyId());           // minus2Semitone
    return;

    key--;
    for (int i=0;i<6;i++) {
        if (harms[key][i]>0) harmonics.insert(harms[key][i]);
        else break;
    }
}

void getHarmonicKeys(std::map<int,HarmonicType> &harmonics, int key)
{
    harmonics.clear();
    if (key<1 || key>24) return;
    MusicKey k(key);
    harmonics.insert(std::pair<int,HarmonicType>(k.wmKeyId(),harmonicSameKey));
    harmonics.insert(std::pair<int,HarmonicType>(k.nextKey().wmKeyId(),harmonicNextKey));
    harmonics.insert(std::pair<int,HarmonicType>(k.previousKey().wmKeyId(),harmonicPreviousKey));
    harmonics.insert(std::pair<int,HarmonicType>(k.minorMajorSwitch().wmKeyId(),harmonicMinorMajorSwitch));
    harmonics.insert(std::pair<int,HarmonicType>(k.minorMajorJump3().wmKeyId(),harmonicMinorMajorJump3));
    harmonics.insert(std::pair<int,HarmonicType>(k.minorMajorJump1().wmKeyId(),harmonicMinorMajorJump1));
    harmonics.insert(std::pair<int,HarmonicType>(k.plus1Semitone().wmKeyId(),harmonicSemitoneUp));
    harmonics.insert(std::pair<int,HarmonicType>(k.minus1Semitone().wmKeyId(),harmonicSemitoneDown));
    harmonics.insert(std::pair<int,HarmonicType>(k.plus2Semitone().wmKeyId(),harmonicTwoSemitoneUp));
    harmonics.insert(std::pair<int,HarmonicType>(k.minus2Semitone().wmKeyId(),harmonicTwoSemitoneDown));
    harmonics.insert(std::pair<int,HarmonicType>(k.avbBoost().wmKeyId(),harmonicAvbBoost));
}



int getMusicKeyId(const ppl6::CString &name)
{
    ppl6::CString k=ppl6::LCase(ppl6::Trim(name));
    k.Replace("♯","#");
    k.Replace("♭","b");
    if (k.IsEmpty()) return 0;
    for (int i=1;i<26;i++) {
        ppl6::CString tmp=ppl6::LCase(ppl6::Trim(wm_main->conf.customMusicKey[i]));
        if (tmp==k) return i;
    }
    if (k=="g#m" || k=="g#min" || k=="abm" || k=="1a" || k=="6m") return 1;
    else if (k=="b" || k=="bmaj" || k=="1b" || k=="6d") return 2;
    else if (k=="d#m" || k=="d#min" || k=="ebm" || k=="2a" || k=="7m") return 3;
    else if (k=="f#" || k=="f#maj" || k=="gb" || k=="2b" || k=="7d") return 4;
    else if (k=="a#m" || k=="a#min" || k=="bbm" || k=="3a" || k=="8m") return 5;
    else if (k=="c#" || k=="c#maj" || k=="db" || k=="3b" || k=="8d") return 6;
    else if (k=="fm" || k=="fmin" || k=="4a" || k=="6m" || k=="9m") return 7;
    else if (k=="g#" || k=="g#maj" || k=="ab" || k=="4b" || k=="9d") return 8;
    else if (k=="cm"|| k=="cmin" || k=="5a" || k=="10m") return 9;
    else if (k=="d#" || k=="d#maj" || k=="eb" || k=="5b" || k=="10d") return 10;
    else if (k=="gm" || k=="gmin" || k=="6a" || k=="11m") return 11;
    else if (k=="a#" || k=="a#maj" || k=="bb" || k=="6b" || k=="11d") return 12;
    else if (k=="dm" || k=="dmin" || k=="7a" || k=="12m") return 13;
    else if (k=="f" || k=="fmaj" || k=="7b" || k=="12d") return 14;
    else if (k=="am" || k=="amin" || k=="8a" || k=="1m") return 15;
    else if (k=="c" || k=="cmaj" || k=="8b" || k=="1d") return 16;
    else if (k=="em" || k=="emin" || k=="9a" || k=="2m") return 17;
    else if (k=="g" || k=="gmaj" || k=="2b" || k=="2d") return 18;
    else if (k=="bm" || k=="bmin" || k=="10a" || k=="3m") return 19;
    else if (k=="d" || k=="dmaj" || k=="10b" || k=="3d") return 20;
    else if (k=="f#m" || k=="f#min" || k=="gbm" || k=="11a" || k=="4m") return 21;
    else if (k=="a" || k=="amaj" || k=="11b" || k=="4d") return 22;
    else if (k=="c#m" || k=="c#min" || k=="dbm" || k=="12a" || k=="5m") return 23;
    else if (k=="e" || k=="emaj" || k=="12b" || k=="5d") return 24;
    return 0;
}

ppl6::CString keyNameSharps(int id)
{
    switch (id) {
        case 1: return "G#m";
        case 2: return "B";
        case 3: return "D#m";
        case 4: return "F#";
        case 5: return "A#m";
        case 6: return "C#";
        case 7: return "Fm";
        case 8: return "G#";
        case 9: return "Cm";
        case 10: return "D#";
        case 11: return "Gm";
        case 12: return "A#";
        case 13: return "Dm";
        case 14: return "F";
        case 15: return "Am";
        case 16: return "C";
        case 17: return "Em";
        case 18: return "G";
        case 19: return "Bm";
        case 20: return "D";
        case 21: return "F#m";
        case 22: return "A";
        case 23: return "C#m";
        case 24: return "E";
        case 25: return "none";
        default: return "";
    }
}

ppl6::CString keyNameOpenKey(int id)
{
    switch (id) {
        case 1: return " 6m";	// G#m
        case 2: return " 6d";	// B
        case 3: return " 7m";	// D#m
        case 4: return " 7d";	// F#
        case 5: return " 8m";	// A#m
        case 6: return " 8d";	// C#
        case 7: return " 9m";	// Fm
        case 8: return " 9d";	// G#
        case 9: return "10m";	// Cm
        case 10: return "10d";	// D#
        case 11: return "11m";	// Gm
        case 12: return "11d";	// A#
        case 13: return "12m";	// Dm
        case 14: return "12d";	// F
        case 15: return " 1m";	// Am
        case 16: return " 1d";	// C
        case 17: return " 2m";	// Em
        case 18: return " 2d";	// G
        case 19: return " 3m";	// Bm
        case 20: return " 3d";	// D
        case 21: return " 4m";	// F#m
        case 22: return " 4d";	// A
        case 23: return " 5m";	// C#m
        case 24: return " 5d";	// E
        case 25: return "none";	// E
        default: return "";		// ""
    }
}

ppl6::CString keyNameCustom(int id)
{
    if (id==0 || id>25) return "";
    if (id==25) return "none";
    return wm_main->conf.customMusicKey[id];
}



MusicKey::MusicKey()
{
    ok=0;
    wmid=0;
    type=unknown;
}

MusicKey::MusicKey(int wm_keyid)
{
    setKey(wm_keyid);
}

MusicKey::MusicKey(const ppl6::CString &name)
{
    setKey(getMusicKeyId(name));
}

MusicKey::MusicKey(int openkeyId, KeyType type)
{
    setKey(openkeyId, type);
}

void MusicKey::setKey(int wm_keyid)
{
    ok=0;
    wmid=wm_keyid;
    type=unknown;
    switch (wm_keyid) {
    case 1: ok=6; type=minor; break;	// G#m
    case 2: ok=6; type=major; break;    // B
    case 3: ok=7; type=minor; break; 	// D#m
    case 4: ok=7; type=major; break;	// F#
    case 5: ok=8; type=minor; break;	// A#m
    case 6: ok=8; type=major; break;	// C#
    case 7: ok=9; type=minor; break;	// Fm
    case 8: ok=9; type=major; break;    // G#
    case 9: ok=10; type=minor; break;	// Cm
    case 10: ok=10; type=major; break;	// D#
    case 11: ok=11; type=minor; break;	// Gm
    case 12: ok=11; type=major; break;  // A#
    case 13: ok=12; type=minor; break;	// Dm
    case 14: ok=12; type=major; break;	// F
    case 15: ok=1; type=minor; break;	// Am
    case 16: ok=1; type=major; break; 	// C
    case 17: ok=2; type=minor; break;	// Em
    case 18: ok=2; type=major; break;   // G
    case 19: ok=3; type=minor; break;	// Bm
    case 20: ok=3; type=major; break;	// D
    case 21: ok=4; type=minor; break;	// F#m
    case 22: ok=4; type=major; break;	// A
    case 23: ok=5; type=minor; break;	// C#m
    case 24: ok=5; type=major; break;	// E
    case 25: ok=0; type=unknown; break;	// NONE
    default: ok=0; type=unknown; break; // ""
    }
}

void MusicKey::setKey(int openkeyId, KeyType type)
{
    wmid=0;
    ok=openkeyId;
    this->type=type;
    if (openkeyId<1 || openkeyId>12 || type==unknown) return;
    switch (openkeyId) {
    case 1: wmid=15; break;
    case 2: wmid=17; break;
    case 3: wmid=19; break;
    case 4: wmid=21; break;
    case 5: wmid=23; break;
    case 6: wmid=1; break;
    case 7: wmid=3; break;
    case 8: wmid=5; break;
    case 9: wmid=7; break;
    case 10: wmid=9; break;
    case 11: wmid=11; break;
    case 12: wmid=13; break;
    }
    if (type==major) wmid++;
}

int MusicKey::wmKeyId() const
{
    return wmid;
}

ppl6::CString MusicKey::name(MusicKeyType type) const
{
    if (type==musicKeyTypeMusicalSharps) return keyNameSharps(wmid);
    else if (type==musicKeyTypeOpenKey) return keyNameOpenKey(wmid);
    else if (type==musicKeyTypeCustom) return keyNameCustom(wmid);
    return "";
}

static int clampKey(int ok)
{
    if (ok<1) return ok+12;
    if (ok>12) return ok-12;
    return ok;
}

MusicKey MusicKey::nextKey() const
{
    if (ok==0 || type==unknown) return MusicKey();
    return MusicKey(clampKey(ok+1),type);
}

MusicKey MusicKey::previousKey() const
{
    if (ok==0 || type==unknown) return MusicKey();
    return MusicKey(clampKey(ok-1),type);
}

MusicKey MusicKey::minorMajorSwitch() const
{
    if (ok==0 || type==unknown) return MusicKey();
    if (type==minor) return MusicKey(ok,major);
    return MusicKey(ok,minor);
}

MusicKey MusicKey::minorMajorJump3() const
{
    if (ok==0 || type==unknown) return MusicKey();
    if (type==minor) return MusicKey(clampKey(ok+3),major);
    return MusicKey(clampKey(ok-3),minor);
}

MusicKey MusicKey::minorMajorJump1() const
{
    if (ok==0 || type==unknown) return MusicKey();
    if (type==major) return MusicKey(clampKey(ok+1),minor);
    return MusicKey(clampKey(ok-1),major);
}

MusicKey MusicKey::plus1Semitone() const
{
    if (ok==0 || type==unknown) return MusicKey();
    return MusicKey(clampKey(ok+7),type);
}

MusicKey MusicKey::minus1Semitone() const
{
    if (ok==0 || type==unknown) return MusicKey();
    return MusicKey(clampKey(ok-7),type);
}

MusicKey MusicKey::plus2Semitone() const
{
    if (ok==0 || type==unknown) return MusicKey();
    return MusicKey(clampKey(ok+2),type);
}

MusicKey MusicKey::minus2Semitone() const
{
    if (ok==0 || type==unknown) return MusicKey();
    return MusicKey(clampKey(ok-2),type);
}

MusicKey MusicKey::avbBoost() const
{
    if (ok==0 || type==unknown) return MusicKey();
    return MusicKey(clampKey(ok+4),type);
}

MusicKey MusicKey::add(int modification) const
{
    if (ok==0 || type==unknown) return MusicKey();
    if (modification>12) modification=12;
    if (modification<-12) modification=-12;
    return MusicKey(clampKey(ok+modification),type);
}

MusicKey MusicKey::addSemitone(int modification) const
{
    if (ok==0 || type==unknown) return MusicKey();
    int m=(7*abs(modification))%12;
    if (modification<0) return MusicKey(clampKey(ok-m),type);
    return MusicKey(clampKey(ok+m),type);
}


bool MusicKey::operator==(const MusicKey &other) const
{
    if (wmid==other.wmid) return true;
    return false;
}

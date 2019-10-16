#ifndef MUSICKEY_H
#define MUSICKEY_H

#include <set>
#include <map>
#include <ppl6.h>
#include "winmusik3.h"

int getMusicKeyId(const ppl6::CString &name);
ppl6::CString keyNameSharps(int id);
ppl6::CString keyNameOpenKey(int id);
ppl6::CString keyNameCustom(int id);

enum HarmonicType {
    harmonicSameKey=0,
    harmonicNextKey,			// +1 on wheel
    harmonicPreviousKey,		// -1 on wheel
    harmonicMinorMajorSwitch,
    harmonicMinorMajorJump3,    // from major -3 on wheel to minor
                                // from minor +3 on wheel to major
    harmonicMinorMajorJump1,    // from major +1 on wheel to minor
                                // from minor -1 on wheel to major
    harmonicSemitoneUp,			// +7 on wheel
    harmonicSemitoneDown,		// -7 on wheel
    harmonicTwoSemitoneUp,		// +2 on wheel
    harmonicTwoSemitoneDown,    // -2 on wheel
    harmonicAvbBoost            // +4 on wheel
};

void getHarmonicKeys(std::map<int,HarmonicType> &harmonics, int key);

class MusicKey
{
public:
    enum KeyType {
        unknown=0,
        minor=1,
        major=2
    };

private:
    int ok;
    int wmid;
    KeyType type;

public:
    MusicKey();
    MusicKey(int wm_keyid);
    MusicKey(int openkeyId, KeyType type);
    MusicKey(const ppl6::CString &name);

    void setKey(int wm_keyid);
    void setKey(int openkeyId, KeyType type);
    ppl6::CString name(MusicKeyType type=musicKeyTypeOpenKey) const;
    int wmKeyId() const;

    MusicKey add(int modification) const;
    MusicKey addSemitone(int modification) const;

    MusicKey nextKey() const;
    MusicKey previousKey() const;
    MusicKey minorMajorSwitch() const;
    MusicKey minorMajorJump3() const;
    MusicKey minorMajorJump1() const;
    MusicKey plus1Semitone() const;
    MusicKey minus1Semitone() const;
    MusicKey plus2Semitone() const;
    MusicKey minus2Semitone() const;
    MusicKey avbBoost() const;

    bool operator==(const MusicKey &other) const;
};

class RelatedKeys
{
public:
};


#endif // MUSICKEY_H

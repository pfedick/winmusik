#include "wm_musicalkeys.h"

namespace de {
namespace pfp {
namespace winmusik {

static inline ppl7::String keyNameSharps(int id)
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

static inline ppl7::String keyNameOpenKey(int id)
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

void MusicalKeys::setCustomName(const ppl7::String& name)
{
	customname.set(name);
}

const ppl7::String& MusicalKeys::customName() const
{
	return customname;
}

void MusicalKeys::setCustomKeyName(int key, const ppl7::String& name)
{
	if (key < 1 || key>25) return;
	customkey[key].set(name);
}

ppl7::String MusicalKeys::keyName(int id, MusicKeyType type) const
{
	if (type == musicKeyTypeMusicalSharps) return keyNameSharps(id);
	else if (type == musicKeyTypeOpenKey) return keyNameOpenKey(id);
	else if (type == musicKeyTypeCustom) {
		if (id < 1 || id>25) return "";
		return customkey[id];
	}
	return "";
}

ppl7::String MusicalKeys::sharpName(int id)
{
	return keyNameSharps(id);
}

ppl7::String MusicalKeys::openKeyName(int id)
{
	return keyNameOpenKey(id);
}

int MusicalKeys::keyId(const ppl7::String& name) const
{
	ppl7::String k=ppl7::LowerCase(ppl7::Trim(name));
	k.replace("♯", "#");
	k.replace("♭", "b");
	if (k.isEmpty()) return 0;

	for (int i=1;i < 26;i++) {
		ppl7::String tmp=ppl7::LowerCase(ppl7::Trim(customkey[i]));
		if (tmp == k) return i;
	}

	if (k == "g#m" || k == "g#min" || k == "abm" || k == "1a" || k == "6m") return 1;
	else if (k == "b" || k == "bmaj" || k == "1b" || k == "6d") return 2;
	else if (k == "d#m" || k == "d#min" || k == "ebm" || k == "2a" || k == "7m") return 3;
	else if (k == "f#" || k == "f#maj" || k == "gb" || k == "2b" || k == "7d") return 4;
	else if (k == "a#m" || k == "a#min" || k == "bbm" || k == "3a" || k == "8m") return 5;
	else if (k == "c#" || k == "c#maj" || k == "db" || k == "3b" || k == "8d") return 6;
	else if (k == "fm" || k == "fmin" || k == "4a" || k == "6m" || k == "9m") return 7;
	else if (k == "g#" || k == "g#maj" || k == "ab" || k == "4b" || k == "9d") return 8;
	else if (k == "cm" || k == "cmin" || k == "5a" || k == "10m") return 9;
	else if (k == "d#" || k == "d#maj" || k == "eb" || k == "5b" || k == "10d") return 10;
	else if (k == "gm" || k == "gmin" || k == "6a" || k == "11m") return 11;
	else if (k == "a#" || k == "a#maj" || k == "bb" || k == "6b" || k == "11d") return 12;
	else if (k == "dm" || k == "dmin" || k == "7a" || k == "12m") return 13;
	else if (k == "f" || k == "fmaj" || k == "7b" || k == "12d") return 14;
	else if (k == "am" || k == "amin" || k == "8a" || k == "1m") return 15;
	else if (k == "c" || k == "cmaj" || k == "8b" || k == "1d") return 16;
	else if (k == "em" || k == "emin" || k == "9a" || k == "2m") return 17;
	else if (k == "g" || k == "gmaj" || k == "2b" || k == "2d") return 18;
	else if (k == "bm" || k == "bmin" || k == "10a" || k == "3m") return 19;
	else if (k == "d" || k == "dmaj" || k == "10b" || k == "3d") return 20;
	else if (k == "f#m" || k == "f#min" || k == "gbm" || k == "11a" || k == "4m") return 21;
	else if (k == "a" || k == "amaj" || k == "11b" || k == "4d") return 22;
	else if (k == "c#m" || k == "c#min" || k == "dbm" || k == "12a" || k == "5m") return 23;
	else if (k == "e" || k == "emaj" || k == "12b" || k == "5d") return 24;
	return 0;
}



}
}
}	// EOF Namespace de.pfp.winmusik

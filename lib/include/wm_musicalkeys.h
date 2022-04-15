/*
 * cwmfile.h
 *
 *  Created on: 20.06.2016
 *      Author: patrick
 */

#ifndef LIB_INCLUDE_WM_MUSICALKEYS_H_
#define LIB_INCLUDE_WM_MUSICALKEYS_H_

#include <stdlib.h>
#include <stdio.h>

#include "../../ppl6/include/ppl7.h"

namespace de {
namespace pfp {
namespace winmusik {


enum MusicKeyType
{
	musicKeyTypeNone=0,
	musicKeyTypeMusicalSharps,
	musicKeyTypeOpenKey,
	musicKeyTypeCustom=99
};


class MusicalKeys
{
private:
	ppl7::String customname;
	ppl7::String customkey[26];
public:
	void setCustomName(const ppl7::String &name);
	const ppl7::String& customName() const;
	void setCustomKeyName(int key, const ppl7::String &name);

	ppl7::String keyName(int id, MusicKeyType type) const;
	int keyId(const ppl7::String &name) const;

	static ppl7::String sharpName(int id);
	static ppl7::String openKeyName(int id);
};



}}}	// EOF Namespace de.pfp.winmusik

#endif /* LIB_INCLUDE_WM_PLAYLIST_H_ */


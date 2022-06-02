/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2022 Patrick Fedick
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ppl7.h>
#include <libwinmusik3.h>

namespace de {
namespace pfp {
namespace winmusik {

Config::Config()
{
    DataPath=ppl7::Dir::homePath() + "/WinMusik3";
    TmpPath=ppl7::Dir::tempPath();
    ConfigFile=ppl7::Dir::homePath() + ".config/Patrick F.-Productions/WinMusik3.ini";

}

void Config::load()
{

}

void Config::save()
{

}

void Config::setConfigFile(const ppl7::String& filename)
{
    ConfigFile=filename;
}

} // EOF namespace winmusik
} // EOF namespace pfp
} // EOF namespace de

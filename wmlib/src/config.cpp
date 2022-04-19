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

#ifndef WM_REGEXPCAPTURE_H_
#define WM_REGEXPCAPTURE_H_


#include <vector>
#include <list>

#include <ppl7.h>
#include <ppl7-types.h>
namespace de {
namespace pfp {
namespace winmusik {

class RegExpPattern
{
	public:
		RegExpPattern();
		void copyFrom(const RegExpPattern &other);
		ppl7::String Name;
		ppl7::String Pattern;
		int artist;
		int title;
		int version;
		int genre;
		int label;
		int bpm;
		int album;
		int hours;
		int minutes;
		int seconds;
		int releasedate;
		bool isHTML;
};

class RegExpMatch
{
	public:
		RegExpMatch();
		ppl7::String	Artist;
		ppl7::String	Title;
		ppl7::String	Version;
		ppl7::String	Genre;
		ppl7::String	Label;
		ppl7::String	Bpm;
		ppl7::String	Album;
		ppl7::String	ReleaseDate;
		int				Length;		// in Sekunden

};

class RegularExpressionCapture
{
	private:
		std::vector<RegExpPattern> patterns;
		void copyToMatch(const RegExpPattern &p, const ppl7::Array &res, RegExpMatch &match) const;
		void addDefaultPatterns();

	public:
		RegularExpressionCapture();
		~RegularExpressionCapture();

		void load(const ppl7::String &configfile);
		void save(const ppl7::String &configfile);
		void addPattern(const RegExpPattern &pattern);
		void insertPattern(size_t pos, const RegExpPattern &pattern);
		void setPattern(size_t pos, const RegExpPattern &pattern);
		void deletePattern(size_t pos);
		const RegExpPattern &getPattern(size_t pos) const;
		size_t size() const;

		bool match(const ppl7::String &data, RegExpMatch &match) const;
		//bool match(const RegExpClipboard &data, RegExpMatch &match) const;
		bool testMatch(const ppl7::String &data, RegExpMatch &match, const RegExpPattern &pattern) const;
		bool buildinMatch(const ppl7::String &data, RegExpMatch &match) const;

};

}}}	// EOF Namespace de.pfp.winmusik
#endif /* WM_REGEXPCAPTURE_H_ */

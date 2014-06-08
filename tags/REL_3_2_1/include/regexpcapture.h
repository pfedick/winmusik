/*
 * regexpcapture.h
 *
 *  Created on: 16.06.2013
 *      Author: patrick
 */

#ifndef REGEXPCAPTURE_H_
#define REGEXPCAPTURE_H_


#include <vector>

class RegExpPattern
{
	public:
		RegExpPattern();

		void copyFrom(const RegExpPattern &other);

		ppl6::CString Name;
		ppl6::CString Pattern;
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
};

class RegExpMatch
{
	public:
		RegExpMatch() {
			Length=0;
		}
		ppl6::CString	Artist;
		ppl6::CString	Title;
		ppl6::CString	Version;
		ppl6::CString	Genre;
		ppl6::CString	Label;
		ppl6::CString	Bpm;
		ppl6::CString	Album;
		ppl6::CString	ReleaseDate;
		int				Length;		// in Sekunden

};

class RegExpClipboard
{
	public:
		void copyFromClipboard();
		ppl6::CString PlainText;
		ppl6::CString Html;
};


class RegularExpressionCapture
{
	private:
		std::vector<RegExpPattern> patterns;
		void copyToMatch(const RegExpPattern &p, const ppl6::CArray &res, RegExpMatch &match) const;
		void addDefaultPatterns();
	public:
		RegularExpressionCapture();
		~RegularExpressionCapture();

		void load();
		void save();
		void addPattern(const RegExpPattern &pattern);
		void insertPattern(size_t pos, const RegExpPattern &pattern);
		void setPattern(size_t pos, const RegExpPattern &pattern);
		void deletePattern(size_t pos);
		const RegExpPattern &getPattern(size_t pos) const;
		size_t size() const;

		bool match(const ppl6::CString &data, RegExpMatch &match) const;
		bool match(const RegExpClipboard &data, RegExpMatch &match) const;
		bool testMatch(const ppl6::CString &data, RegExpMatch &match, const RegExpPattern &pattern) const;
		bool buildinMatch(const ppl6::CString &data, RegExpMatch &match) const;




};

#endif /* REGEXPCAPTURE_H_ */
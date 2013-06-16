/*
 * regexpcapture.h
 *
 *  Created on: 16.06.2013
 *      Author: patrick
 */

#ifndef REGEXPCAPTURE_H_
#define REGEXPCAPTURE_H_

class RegExpPattern
{
	public:
		ppl6::CString Name;
		ppl6::CString Pattern;

};


class RegularExpressionCapture
{
	private:
	public:
		RegularExpressionCapture();
		~RegularExpressionCapture();

		void load();
		void save();



};

#endif /* REGEXPCAPTURE_H_ */

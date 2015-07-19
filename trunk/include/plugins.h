/*
 * plugins.h
 *
 *  Created on: 17.02.2015
 *      Author: patrick
 */

#ifndef PLUGINS_H_
#define PLUGINS_H_

#include <ppl6.h>
#include <list>

class Plugins
{
	private:
		std::list<ppl6::CString> python_modules;

	public:
		Plugins();
		~Plugins();
		void loadFromPath(const ppl6::CString &Path);

};




#endif /* PLUGINS_H_ */

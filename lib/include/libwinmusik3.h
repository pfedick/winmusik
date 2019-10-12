/*
 * libwinmusik3.h
 *
 *  Created on: 20.06.2016
 *      Author: patrick
 */

#ifndef LIBWINMUSIK3_H_
#define LIBWINMUSIK3_H_

#include <ppl7.h>
#include <ppl7-config.h>

#include <wm_musicalkeys.h>
#include <wm_dataobjects.h>

namespace de {
namespace pfp {
namespace winmusik {

class CStorage;




class CProgressUpdate
{
	public:
		virtual ~CProgressUpdate()=0;
		virtual void Update(int progress)=0;
};

class CDataBase
{
	private:
	public:
		CDataBase();
		~CDataBase();
		void clear();
        void load(const ppl7::String &filename);
		static void update(const ppl7::String &oldfile, const ppl7::String &newfile);

};

class CStorage
{
	private:
	public:
		CStorage();
		~CStorage();

		void loadDatabase(const ppl7::String &filename, CDataBase &db);


};


}
}
}



#endif /* LIBWINMUSIK3_H_ */

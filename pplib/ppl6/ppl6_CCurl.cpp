/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 6 (PPL6).
 * Web: http://www.pfp.de/ppl/
 *
 * $Author$
 * $Revision$
 * $Date$
 * $Id$
 *
 *******************************************************************************
 * Copyright (c) 2010, Patrick Fedick <patrick@pfp.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Keine MFCs
#define _WIN32_WINNT 0x501
#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <windows.h>
#endif

#include "prolog.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STDARG_H
    #include <stdarg.h>
#endif
#ifdef HAVE_STRINGS_h
    #include <strings.h>
#endif


#include "ppl6.h"


#ifdef HAVE_LIBCURL
#include <curl/curl.h>
#endif

namespace ppl6 {

/*!\class CCurl
 * \ingroup PPLGroupInternet
 *
 * Die Klasse CCurl ist eine Wrapper-Klasse zur Curl-Library, mit der Daten von
 * Web- und FTP-Server geholt werden können.
 *
 */

#ifdef HAVE_LIBCURL
static ppl6::CMutex CurlMutex;
static int ppl_curl_instance=0;



	static size_t write_function(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		if (!stream) return -1;
		size_t bytes=size*nmemb;
		CCurl *curl=(CCurl*)stream;

		return curl->StoreResult(ptr,bytes,1);
	}

	//size_t function( void *ptr, size_t size, size_t nmemb, void *stream)
	static size_t header_function(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		if (!stream) return -1;
		size_t bytes=size*nmemb;
		CCurl *curl=(CCurl*)stream;
		return curl->StoreResult(ptr,bytes,2);
	}

	static int debug_function(CURL *handle, curl_infotype type, char *data, size_t size, void *stream)
	{
		if (!stream) return 0;
		CCurl *curl=(CCurl*)stream;
		if (curl) curl->DebugHandler((int)type,data,size);
		return 0;
	}

#endif

bool CCurl::isSupported()
{
#ifdef HAVE_LIBCURL
	return true;
#else
	return false;
#endif
}

CCurl::CCurl()
/*!\brief Constructor der Curl-Klasse
 *
 * Der Konstruktor prüft zunächst, ob "curl_global_init" schon aufgerufen wurde.
 * Dazu wird ein globaler Mutex gesetzt.
 *
 * Liefert "curl_global_init" einen Fehler zurück, wird "handle" auf NULL gesetzt und
 * der Konstruktor wird ohne weitere Initialisierung beendet. Alle weiteren Funktionen
 * geben bei Aufruf einen Fehler zurück und es wird Fehlercode 353 gesetzt.
 *
 * War Curl bereits initialisiert oder die Initialisierung war erfolgreich, wird ein
 * Instance-Counter hochgezählt. Der Destructor zählt diesen wieder runter und bei Erreichen
 * von 0 wird Curl wieder Deinitialisiert.
 */
{
	#ifdef HAVE_LIBCURL
		handle=NULL;
		CurlMutex.Lock();
		if (!ppl_curl_instance) {
			if (curl_global_init(CURL_GLOBAL_WIN32|CURL_GLOBAL_SSL)!=0) {
				CurlMutex.Unlock();
				SetError(353);
				return;
			}
		}
		ppl_curl_instance++;
		CurlMutex.Unlock();
		handle=curl_easy_init();
		if (!handle) {
			SetError(353);
			return;
		}
		curl_easy_setopt((CURL*)handle,CURLOPT_COOKIEJAR,"");

		long t=1;
		curl_easy_setopt((CURL*)handle, CURLOPT_NOSIGNAL, t);	// Curl soll keine Signals verwenden


		log=NULL;
		post=NULL;
		last=NULL;
		headers=NULL;
		resultbuffer=NULL;
		errorbuffer=(char*)malloc(CURL_ERROR_SIZE+32);
		/*
		storetofile=NULL;
		filename=NULL;
		filesize=0;
		*/
		if (errorbuffer) {
			errorbuffer[0]=0;
			curl_easy_setopt((CURL*)handle,CURLOPT_ERRORBUFFER,errorbuffer);
		}

	#endif
}

CCurl::~CCurl()
{
	#ifdef HAVE_LIBCURL
		PushError();
		if (post) curl_formfree((curl_httppost*) post);
		if (headers) curl_slist_free_all((struct curl_slist *)headers);

		if (handle) {
			curl_easy_cleanup((CURL *) handle );
			handle=NULL;
		}
		if (errorbuffer) free(errorbuffer);
		//if (storetofile) delete storetofile;
		if (resultbuffer) free(resultbuffer);
		resultbuffer=NULL;

		CurlMutex.Lock();
		if (ppl_curl_instance>0) {
			ppl_curl_instance--;
			if (!ppl_curl_instance) {
				curl_global_cleanup();
			}
		}
		CurlMutex.Unlock();
		PopError();
	#endif
}


void CCurl::Clear()
{
#ifdef HAVE_LIBCURL
	if (post) curl_formfree((curl_httppost*) post);
	last=post=NULL;
	if (headers) {
		curl_slist_free_all((struct curl_slist *)headers);
		headers=NULL;
	}
	if (handle) curl_easy_reset((CURL *)handle );
#endif
}

void CCurl::SetLogfile(CLog *log)
{
	this->log=log;
#ifdef HAVE_LIBCURL
	if (log) {
		curl_easy_setopt((CURL*)handle,CURLOPT_DEBUGFUNCTION, debug_function);
		curl_easy_setopt((CURL*)handle,CURLOPT_DEBUGDATA, this);
		curl_easy_setopt((CURL*)handle,CURLOPT_VERBOSE, TRUE);
	} else {
		curl_easy_setopt((CURL*)handle,CURLOPT_VERBOSE, FALSE);
	}
#endif

}

int CCurl::SetOptOk(int ret)
{
	#ifdef HAVE_LIBCURL
		if (ret==CURLE_OK) return 1;
		const char *e=curl_easy_strerror((CURLcode)ret);
		int myerr;
		switch (ret) {
			case CURLE_OPERATION_TIMEOUTED:
				myerr=174;
				break;
			case CURLE_COULDNT_RESOLVE_HOST:
				myerr=273;
				break;
			case CURLE_COULDNT_CONNECT:
				myerr=274;
				break;
			default:
				myerr=354;
				break;
		}
		SetError(myerr,"%s",e);
		return 0;
	#else
		SetError(356);
		return 0;
	#endif
}


/*!\brief Signal-Handler ein- oder ausschalten
 *
 * \desc
 * Signals werden innerhalb der Curl-Bibliothek zur Implementierung von Timeouts bei DNS-Abfragen verwendet.
 * Verwendet die Anwendung mehrere Threads, sollte die Verwendung von Signals deaktiviert werden
 * (siehe http://curl.haxx.se/libcurl/c/libcurl-tutorial.html#Multi-threading). Seit Version 6.4.10 der
 * PPLib sind die Signals in der CCurl-Klasse standardmässig deaktiviert.
 * \par
 * Ist dieses Verhalten nicht gewünscht, muss die Verwendendung von Signals explizit aktiviert werden.
 *
 * @param enable true=Signals sind zugelassen, false=keine Signals (Default)
 * @return
 */
int CCurl::EnableSignals(bool enable)
{
#ifdef HAVE_LIBCURL
	long t;
	if (enable==true) t=0;
	else t=1;
	return SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_NOSIGNAL, t));
#else
	SetError(356);
	return 0;
#endif

}

int CCurl::SetBrowser(const char *cbrowser)
{
	#ifdef HAVE_LIBCURL
		if (!handle) {
			SetError(353);
			return 0;
		}
		if (!cbrowser) {
			SetError(194,"CCurl::SetBrowser(char *cbrowser)");
			return 0;
		}
		Browser=cbrowser;
		return SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_USERAGENT, cbrowser));
	#else
		SetError(356);
		return 0;
	#endif
}

int CCurl::SetURL(const char *surl)
{
	#ifdef HAVE_LIBCURL
		if (!handle) {
			SetError(353);
			return 0;
		}
		if (!surl) {
			SetError(194,"CCurl::SetURL(char *surl)");
			return 0;
		}
		Url=surl;
		GetCall.Clear();
		return 1;
	#else
		SetError(356);
		return 0;
	#endif
}

int CCurl::SetReferer(const char *url)
{
	#ifdef HAVE_LIBCURL
		if (!handle) {
			SetError(353);
			return 0;
		}
		if (!url) {
			SetError(194,"CCurl::SetReferer(char *url)");
			return 0;
		}
		Referer=url;
		return SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_REFERER, url));
	#else
		SetError(356);
		return 0;
	#endif
}

int CCurl::SetUserPassword(const char *suserpassword)
{
	#ifdef HAVE_LIBCURL
		if (!handle) {
			SetError(353);
			return 0;
		}
		if (!suserpassword) {
			SetError(194,"CCurl::SetUserPassword(char *suserpassword)");
			return 0;
		}
		UserPassword=suserpassword;
		return SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_USERPWD, suserpassword));
	#else
		SetError(356);
		return 0;
	#endif
}

int CCurl::SetUserPassword(const char *username, const char *password)
{
	#ifdef HAVE_LIBCURL
		if (!handle) {
			SetError(353);
			return 0;
		}
		if (!username) {
			SetError(194,"CCurl::SetUserPassword(>>>char *username<<<, char *password)");
			return 0;
		}
		if (!password) {
			SetError(194,"CCurl::SetUserPassword(char *username, >>>char *password<<<)");
			return 0;
		}
		UserPassword.Setf("%s:%s",username,password);
		return SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_USERPWD, (const char*)UserPassword));
	#else
		SetError(356);
		return 0;
	#endif
}

int CCurl::SetUsername(const CString &username)
{
	#ifdef HAVE_LIBCURL
		if (!handle) {
			SetError(353);
			return 0;
		}
		return SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_USERNAME, (const char*)username));
	#else
		SetError(356);
		return 0;
	#endif
}

int CCurl::SetPassword(const CString &password)
{
	#ifdef HAVE_LIBCURL
		if (!handle) {
			SetError(353);
			return 0;
		}
		return SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_PASSWORD, (const char*)password));
	#else
		SetError(356);
		return 0;
	#endif
}

int CCurl::SetProxy(const char *proxy, int port)
{
	#ifdef HAVE_LIBCURL
		if (!handle) {
			SetError(353);
			return 0;
		}
		if (!proxy) {
			SetError(194,"CCurl::SetProxy(>>>char *proxy<<<, int port)");
			return 0;
		}
		if (!port) {
			SetError(194,"CCurl::SetProxy(char *proxy, >>>int port<<<)");
			return 0;
		}
		Proxy.Setf("%s:%i",proxy,port);
		return SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_PROXY, (const char*)Proxy));
	#else
		SetError(356);
		return 0;
	#endif
}

int CCurl::SetHttpVersion(CCurl::HTTPVERSION version)
{
#ifdef HAVE_LIBCURL
	if (!handle) {
		SetError(353);
		return 0;
	}
	if (version==HTTP_1_1) {
		return SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1));
	} else {
		return SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0));
	}
#else
	SetError(356);
	return 0;
#endif
}

int CCurl::SetTimeout(int seconds)
{
	#ifdef HAVE_LIBCURL
		if (!handle) {
			SetError(353);
			return 0;
		}
		long t=1;
		curl_easy_setopt((CURL*)handle, CURLOPT_NOSIGNAL, t);	// Curl soll keine Signals verwenden
		t=seconds;
		return SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_TIMEOUT, t));
	#else
		SetError(356);
		return 0;
	#endif
}


int	CCurl::SetMaximumPersistantConnects(int value)
{
#ifdef HAVE_LIBCURL
	if (!handle) {
		SetError(353);
		return 0;
	}
	long t=value;
	return SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_MAXCONNECTS, t));
#else
	SetError(356);
	return 0;
#endif

}

int CCurl::Reset()
{
#ifdef HAVE_LIBCURL
	if (!handle) {
		SetError(353);
		return 0;
	}
	if (post) curl_formfree((curl_httppost*) post);
	post=NULL;
	if (headers) curl_slist_free_all((struct curl_slist *)headers);
	headers=NULL;

	if (handle) {
		curl_easy_cleanup((CURL *) handle );
		handle=NULL;
	}
	handle=curl_easy_init();
	if (!handle) {
		SetError(353);
		return 0;
	}
	curl_easy_setopt((CURL*)handle,CURLOPT_COOKIEJAR,"");
	if (resultbuffer) free(resultbuffer);
	resultbuffer=NULL;
	errorbuffer[0]=0;
	curl_easy_setopt((CURL*)handle,CURLOPT_ERRORBUFFER,errorbuffer);



	return 1;
#else
	SetError(356);
	return 0;
#endif
}


int CCurl::Escape(CString &string)
{
	#ifdef HAVE_LIBCURL
		if (!handle) {
			SetError(353);
			return 0;
		}
		#if LIBCURL_VERSION_NUM >  0x070f03
			char *res=curl_easy_escape((CURL*)handle, string.GetPtr(), string.Len());
		#else
			char *res=curl_escape(string.GetPtr(), string.Len());
		#endif
		if (!res) {
			SetError(355);
			return 0;
		}
		string.Set(res);
		curl_free(res);
		return 1;
	#else
		SetError(356);
		return 0;
	#endif
}

int CCurl::Escape(CString &target, CAssocArray &source)
{
	#ifdef HAVE_LIBCURL
		if (!handle) {
			SetError(353);
			return 0;
		}
		source.Reset();
		CString key,value;
		//char *k,*v;
		int c=0;
		while (source.GetNext(&key,&value)) {
			#if LIBCURL_VERSION_NUM >  0x070f03
				char *k=curl_easy_escape((CURL*)handle, key.GetPtr(), key.Len());
				char *v=curl_easy_escape((CURL*)handle, value.GetPtr(), value.Len());
			#else
				char *k=curl_escape(key.GetPtr(), key.Len());
				char *v=curl_escape(value.GetPtr(), value.Len());
			#endif
			if ((!k) || (!v)) {
				if (k) curl_free(k);
				if (v) curl_free(v);
				SetError(355,"key=%s, value=%s",(const char*)key,(const char*)value);
				return 0;
			}
			if (c) target.Concat("&");
			target.Concatf("%s=%s",k,v);
			curl_free(k);
			curl_free(v);
			c++;
		}
		return 1;
	#else
		SetError(356);
		return 0;
	#endif
}


int CCurl::Get(const char *parameter)
{
	#ifdef HAVE_LIBCURL
		if (!handle) {
			SetError(353);
			return 0;
		}
		GetCall=Url;
		if (parameter) {
			if (GetCall.Instr("?")>0) GetCall.Concat("&");
			else GetCall.Concat("?");
			GetCall.Concat(parameter);
		}
		return Get();
	#else
		SetError(356);
		return 0;
	#endif
}

/*!\brief GET-Aufruf mit Parametern aus einem Array
 *
 * \desc
 * GET-Aufruf mit Parametern aus dem Array \p param.
 *
 * \param[in] param Ein Assoziatives Array mit den zusätzlichen Parametern. Die Parameter werden
 * automatisch escaped!
 */
int CCurl::Get(CAssocArray &param)
{
	#ifdef HAVE_LIBCURL
		if (!handle) {
			SetError(353);
			return 0;
		}
		GetCall=Url;
		if (GetCall.Instr("?")>0) GetCall.Concat("&");
		else GetCall.Concat("?");
		if (!Escape(GetCall, param)) return 0;
		return Get();
	#else
		SetError(356);
		return 0;
	#endif
}

const char *CCurl::GetURL()
{
	return Url.GetPtr();
}

int CCurl::Get()
{
	#ifdef HAVE_LIBCURL
		if (!handle) {
			SetError(353);
			return 0;
		}
		curl_easy_setopt((CURL*)handle,CURLOPT_WRITEDATA,this);
		curl_easy_setopt((CURL*)handle,CURLOPT_WRITEFUNCTION,write_function);
		curl_easy_setopt((CURL*)handle,CURLOPT_WRITEHEADER,this);
		curl_easy_setopt((CURL*)handle,CURLOPT_HEADERFUNCTION,header_function);


		if (GetCall.IsEmpty()) GetCall=Url;
		if (GetCall.IsEmpty()) {
			SetError(537);
			return 0;
		}
		if (!SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_URL, (const char*)GetCall))) return 0;

		long a=1;
		if (!SetOptOk(curl_easy_setopt((CURL*)handle,CURLOPT_HTTPGET,a))) return 0;
		// Falls der Server Redirects sendet. folgen wir diesen:
		if (!SetOptOk(curl_easy_setopt((CURL*)handle,CURLOPT_FOLLOWLOCATION,a))) return 0;

		if (headers) {
			curl_easy_setopt((CURL*)handle, CURLOPT_HTTPHEADER, headers);
		}
		if (resultbuffer) free(resultbuffer);
		resultbuffer=NULL;
		resultbuffer_size=0;
		Header="";
		call_send.Notify(NULL);
		aboard=false;
		int ret=curl_easy_perform((CURL*)handle);
		if (!SetOptOk(ret)) {
			PushError();
			call_done.Notify(NULL);
			PopError();
			return 0;
		}
		a=0;
		long num_connects=0, num_redirects=0;
		ret=curl_easy_getinfo((CURL*)handle, CURLINFO_NUM_CONNECTS, &num_connects);
		ret=curl_easy_getinfo((CURL*)handle, CURLINFO_REDIRECT_COUNT, &num_redirects);

		if (log) log->Printf(ppl6::LOG::DEBUG,5,"ppl6::CCurl","Get",__FILE__,__LINE__,"New Connects: %i, Redirects: %i",((int)num_connects), (int)num_redirects);
		call_done.Notify(NULL);
		return 1;
	#else
		SetError(356);
		return 0;
	#endif
}

size_t CCurl::StoreResult(void *ptr, size_t bytes, int type)
{
	#ifdef HAVE_LIBCURL
	/*
		PPL_CURL_NOTIFYDATA ndata;
		ndata.curl=this;
		ndata.size=filesize;
		ndata.bytes=0;
		ndata.bps=0;
		ndata.bps_average=0;
		ndata.filename=(const char *)filename;
		*/
		if (aboard) return -1;			// Abbruch

		/*
		if (storetofile) {
			size_t storedbytes=storetofile->Write((char*)ptr,(ppldd)bytes);
			ndata.currentsize=(size_t)storetofile->Size();
			call_receive.Notify(&ndata);
			return storedbytes;
		}
		*/
		if (type==1) {
			if (!resultbuffer) {
				resultbuffer=(char*)malloc(bytes+1);
				if (!resultbuffer) return 0;
				resultbuffer_size=bytes;
				memcpy(resultbuffer,ptr,bytes);
				resultbuffer[bytes]=0;
				return bytes;
			}
			size_t b=resultbuffer_size+bytes;
			char *buffer=(char*)realloc(resultbuffer,b+1);
			if (!buffer) return 0;
			resultbuffer=buffer;
			memcpy((ppldb*)resultbuffer+resultbuffer_size,ptr,bytes);
			resultbuffer_size+=bytes;
			resultbuffer[resultbuffer_size]=0;
			call_receive.Notify(&bytes);
			return bytes;
		} else if (type==2) {
			Header.Concat((char*)ptr,bytes);
			return bytes;
		}
	#endif
	return -1;
}

CString CCurl::GetLastURL()
{
#ifdef HAVE_LIBCURL
	if (!handle) {
		SetError(353);
		return "";
	}
	char *u;
	if (!SetOptOk(curl_easy_getinfo((CURL*)handle, CURLINFO_EFFECTIVE_URL , &u))) return "";
	return u;
#else
	SetError(356);
	return "";
#endif
}


int CCurl::GetResultBuffer(void **buffer, size_t *size)
{
	if (!resultbuffer) return 0;
	*buffer=resultbuffer;
	*size=resultbuffer_size;
	return 1;
}

int CCurl::GetResultBuffer(CBinary &bin)
{
	if (!resultbuffer) return 0;
	return bin.Set(resultbuffer,resultbuffer_size);
}

int CCurl::CopyResultBuffer(CBinary &bin)
{
	if (!resultbuffer) return 0;
	return bin.Copy(resultbuffer,resultbuffer_size);
}

CString CCurl::GetResultBuffer()
{
	CString ret;
	if (resultbuffer) ret.Set(resultbuffer,resultbuffer_size);
	return ret;
}

CString CCurl::GetHeader()
{
	return Header;
}

int CCurl::GetHeader(CString &str)
{
	str=Header;
	return 1;
}

void CCurl::DebugHandler(int type, const char *data, size_t size)
/*!\brief Ausgabe von Debug-Information durch libcurl
 *
 * \desc
 * Diese Funktion wird intern von Curl aufgerufen, um Devug-Informationen auszugeben. Dies
 * muss vorher jedoch durch AUfruf von CCurl::SetLogfile aktiviert worden sein.
 *
 * @param type Art der Information. Mögliche Werte: CURLINFO_TEXT, CURLINFO_HEADER_IN,
 * CURLINFO_HEADER_OUT, CURLINFO_DATA_IN, CURLINFO_DATA_OUT
 * @param data Der Debug-Text
 * @param size Länge des Textes
 */
{
#ifdef HAVE_LIBCURL
	if (!log) return;
	CString msg;
	msg.Set(data,size);
	switch (type) {
		case CURLINFO_TEXT:
			log->Printf(LOG::DEBUG,5,"CCurl","DebugHandler",__FILE__,__LINE__,"INFO: %s",(const char*)msg);
			/*
			if (strncmp(d,"Getting file with size:",23)==0) {
				filesize=atol(d+24);
			}
			*/
			break;
		case CURLINFO_HEADER_IN:
			log->Printf(LOG::DEBUG,5,"CCurl","DebugHandler",__FILE__,__LINE__,"HEADER_IN: %s",(const char*)msg);
			/*
			if (strncmp(d,"Content-Length: ",16)==0) {
				filesize=atol(d+16);
				if ((comdebug) && (PPLCore)) PPLCore->Printf("Filesize detected: %u",filesize);
			}
			*/
			break;
		case CURLINFO_HEADER_OUT:
			log->Printf(LOG::DEBUG,5,"CCurl","DebugHandler",__FILE__,__LINE__,"HEADER_OUT: %s",(const char*)msg);
			/*
			if (strncmp(d,"RETR ",5)==0) {
				if (filename) free(filename);
				filename=strdup(d+5);
				trim(filename);
				if ((comdebug)  && (PPLCore)) PPLCore->Printf("Filename detected: %s",filename);
			}
			*/
			break;
		case CURLINFO_DATA_IN:				// Protocol data
			log->Printf(LOG::DEBUG,10,"CCurl","DebugHandler",__FILE__,__LINE__,"DATA_IN: %s",(const char*)msg);
			call_receive.Notify(NULL);
			//printf ("CURLINFO_DATA_IN: %u Bytes\n",size);
			break;
		case CURLINFO_DATA_OUT:
			log->Printf(LOG::DEBUG,10,"CCurl","DebugHandler",__FILE__,__LINE__,"DATA_OUT: %s",(const char*)msg);
			call_send.Notify(NULL);
			//printf ("CURLINFO_DATA_OUT: %u Bytes\n",size);
			break;
	}
#endif
}

int CCurl::SetHeader(const char *name, const char *value)
/*!\brief HTTP-Header setzen
 *
 * Mit dieser Funktion wird eine zusätzliche Zeile im HTTP-Header gesetzt oder gelöscht.
 * Zum Setzen einer Variable müssen beide Parameter (\p name und \p value) angegeben werden.
 * Um eine Variable zu löschen, muss \p name angegeben werden und \p value muss NULL sein.
 * Um alle Custom-Header-Zeilen zu löschen, kann die Funktion CCurl::ClearHeader verwendet
 * werden.
 *
 * @param[in] name Name der Header-Variablen
 * @param[in] value Wert der Variablen oder NULL, wenn die Variable gelöscht werden soll
 * @return Bei Erfolg gibt die Funktion 1 zurück, im Fehlerfall 0.
 */
{
#ifdef HAVE_LIBCURL
	if (!handle) {
		SetError(353);
		return 0;
	}
	ppl6::CString Buffer;
	Buffer.Setf("%s: %s",name,(value!=NULL?value:""));
	struct curl_slist *slist;
	slist = curl_slist_append((struct curl_slist *)headers, (const char *)Buffer);
	if (!slist) {

		return 0;
	}
	headers=slist;
	return 1;
#else
	SetError(356);
	return 0;
#endif
}

int CCurl::ClearHeader()
/*!\brief Angepasste HTTP-Header löschen
 *
 * Mit dieser Funktion werden alle angepassten Header-Zeilen gelöscht. Siehe auch
 * CCurl::SetHeader.
 *
 * @return Bei Erfolg gibt die Funktion 1 zurück, im Fehlerfall 0.
 */
{
#ifdef HAVE_LIBCURL
	if (!handle) {
		SetError(353);
		return 0;
	}
	if (!SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_HTTPHEADER, NULL))) return 0;
	if (headers) {
		curl_slist_free_all((struct curl_slist *)headers);
		headers=NULL;
	}
	return 1;
#else
	SetError(356);
	return 0;
#endif
}


int CCurl::AddPostVar(const char *name, int val, const char *contenttype)
{
	CString s;
	s.Setf("%i",val);
	return AddPostVar(name,(const char *)s,contenttype);
}

int CCurl::AddPostVar(const char *name, const char *data, const char *contenttype)
{
#ifdef HAVE_LIBCURL
	if (!handle) {
		SetError(353);
		return 0;
	}
	if (!contenttype) {
		curl_formadd((curl_httppost**)&post, (curl_httppost**)&last,
			CURLFORM_COPYNAME, name,
			CURLFORM_COPYCONTENTS, data,
			CURLFORM_END);
	} else {
		curl_formadd((curl_httppost**)&post, (curl_httppost**)&last,
			CURLFORM_COPYNAME, name,
			CURLFORM_COPYCONTENTS, data,
			CURLFORM_CONTENTTYPE, contenttype,
			CURLFORM_END);
	}
	return 1;
#else
	SetError(356);
	return 0;
#endif
}

int CCurl::AddPostVar(CAssocArray &param, const char *prefix)
{
#ifdef HAVE_LIBCURL
	if (!handle) {
		SetError(353);
		return 0;
	}
	CString key;
	CString Prefix,name;
	if (prefix) {
		Prefix=prefix;
	}
	ARRAY_RESULT res;
	param.Reset();
	while ((res=param.GetNext())) {
		param.GetKey(res,&key);
		if (Prefix.NotEmpty()) {
			name=Prefix;
			name.Concatf("[%s]",(const char*)key);
		} else {
			name=key;
		}
		if (param.IsCString(res)) {
			CString *s=param.GetCString(res);
			curl_formadd((curl_httppost**)&post, (curl_httppost**)&last,
				CURLFORM_COPYNAME, (const char*)name,
				CURLFORM_COPYCONTENTS, s->GetPtr(),
				CURLFORM_END);

		} else if (param.IsCWString(res)) {
			CWString *s=param.GetCWString(res);
			curl_formadd((curl_httppost**)&post, (curl_httppost**)&last,
				CURLFORM_COPYNAME, (const char*)name,
				CURLFORM_COPYCONTENTS, s->GetPtr(),
				CURLFORM_END);
		} else if (param.IsArray(res)) {
			CAssocArray *a=param.GetArray(res);
			if (!AddPostVar(*a,name)) return 0;
		}
	}
	param.Reset();
	return 1;
#else
	SetError(356);
	return 0;
#endif

}

int CCurl::Post(CAssocArray &param)
/*!\brief Daten Multipart-encoded senden
 *
 */
{
	if (!AddPostVar(param)) return 0;
	return Post();
}

int CCurl::Post()
{
#ifdef HAVE_LIBCURL
	if (!handle) {
		SetError(353);
		return 0;
	}
	CURLcode ret;
	// Falls der Server Redirects sendet. folgen wir diesen:
	if (!SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_URL, (const char*)Url))) return 0;
	ret=curl_easy_setopt((CURL*)handle, CURLOPT_FOLLOWLOCATION, 1);
	if (post) ret=curl_easy_setopt((CURL*)handle,CURLOPT_HTTPPOST,post);

	if (headers) {
		curl_easy_setopt((CURL*)handle, CURLOPT_HTTPHEADER, headers);
	}

	curl_easy_setopt((CURL*)handle,CURLOPT_WRITEFUNCTION,write_function);
	curl_easy_setopt((CURL*)handle,CURLOPT_WRITEDATA,this);
	if (resultbuffer) free(resultbuffer);
	resultbuffer=NULL;
	resultbuffer_size=0;
	Header="";

	call_send.Notify(NULL);
	aboard=false;
	ret=curl_easy_perform((CURL*)handle);
	call_done.Notify(NULL);
	if (post) curl_formfree((curl_httppost*) post);
	post=last=NULL;
	if (ret==0) return 1;
	SetError(354,errorbuffer);
	return 0;
#else
	SetError(356);
	return 0;
#endif

}

int CCurl::PostFields(CAssocArray &param)
/*!\brief Daten URL-encoded posten
 *
 * Diese Funktion baut aus den angegebenen Parametern einen einzelnen URL-encodeten String
 * und verwendet die Option CURLOPT_POSTFIELDS zum Senden der Daten.
 *
 * @param param
 * @return
 */
{
#ifdef HAVE_LIBCURL
	if (!handle) {
		SetError(353);
		return 0;
	}
	CString Data;
	if (!Escape(Data,param)) return 0;
	if (!SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_POSTFIELDS, (const char*)Data))) return 0;
	return Post();
#else
	SetError(356);
	return 0;
#endif
}


int CCurl::AddCAFile(const char *filename)
{
#ifdef HAVE_LIBCURL
	if (!handle) {
		SetError(353);
		return 0;
	}
	return SetOptOk(curl_easy_setopt((CURL*)handle, CURLOPT_CAINFO, filename));
#else
	SetError(356);
	return 0;
#endif
}

int CCurl::VerifyPeer(bool verify)
{
#ifdef HAVE_LIBCURL
	if (!handle) {
		SetError(353);
		return 0;
	}
	int v=verify;
	return SetOptOk(curl_easy_setopt((CURL*)handle,CURLOPT_SSL_VERIFYPEER , v));
#else
	SetError(356);
	return 0;
#endif
}

void *CCurl::getCurlHandle() const
{
	return handle;
}


/*
int CCurl::POST()
{
	if (!handle) return 0;
	CURLcode ret;
	// Falls der Server Redirects sendet. folgen wir diesen:
	ret=curl_easy_setopt((CURL*)handle, CURLOPT_FOLLOWLOCATION, 1);
	if (post)
		ret=curl_easy_setopt((CURL*)handle,CURLOPT_HTTPPOST,post);

	curl_easy_setopt((CURL*)handle,CURLOPT_WRITEFUNCTION,write_function);
	if (filename) free(filename);
	filename=NULL;
	filesize=0;
	curl_easy_setopt((CURL*)handle,CURLOPT_WRITEDATA,this);
	//#ifdef _DEBUG
	curl_easy_setopt((CURL*)handle,CURLOPT_DEBUGFUNCTION, debug_function);
	curl_easy_setopt((CURL*)handle,CURLOPT_DEBUGDATA, this);
	curl_easy_setopt((CURL*)handle,CURLOPT_VERBOSE, TRUE);
	//#endif

	call_send.Notify(NULL);
	aboard=false;
	ret=curl_easy_perform((CURL*)handle);
	call_done.Notify(NULL);
	if (post) curl_formfree((curl_httppost*) post);
	if (storetofile) {
		storetofile->Close();
		delete storetofile;
		storetofile=NULL;
	}
	post=NULL;
	last=NULL;
	if (ret==0) return 1;
	return 0;
}

void CCurl::StopTransfer()
{
	aboard=true;
}



void CCurl::PostFile(char *name, char *filename)
{
	if (!handle) return;
	curl_formadd((curl_httppost**)&post, (curl_httppost**)&last,
		CURLFORM_COPYNAME, name,
		CURLFORM_FILE, filename,
		CURLFORM_END);
}


ppl6::CFile *CCurl::GetResultFile()
{
	if (!resultbuffer) return NULL;
	ppl6::CMemFile *ff=new ppl6::CMemFile((char*)resultbuffer,(ppldd)resultbuffer_size);
	return ff;
}


const char *CCurl::GetErrorBuffer()
{
	return errorbuffer;
}

int CCurl::StoreToFile(char *filename)
{
	if (storetofile) delete storetofile;
	storetofile=NULL;
	CFile *ff=new CFile();
	if (!ff->Open(filename,"wb")) {
		delete ff;
		return 0;
	}
	storetofile=ff;
	return 1;
}


const char *CCurl::GetFilename()
{
	return (const char*)filename;
}

size_t CCurl::GetFilesize()
{
	return filesize;
}
*/



}



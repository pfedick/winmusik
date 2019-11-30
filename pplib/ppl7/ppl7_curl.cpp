/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 7 (PPL7).
 * Web: http://www.pfp.de/ppl/
 *
 * $Author$
 * $Revision$
 * $Date$
 * $Id$
 *
 *******************************************************************************
 * Copyright (c) 2013, Patrick Fedick <patrick@pfp.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright notice, this
 *       list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
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

#include "prolog_ppl7.h"
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif
#ifdef HAVE_STRINGS_H
    #include <strings.h>
#endif


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Keine MFCs
#define _WIN32_WINNT 0x501
#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <windows.h>
#endif


#include "ppl7.h"
#include "ppl7-inet.h"


#ifdef HAVE_LIBCURL
#include <curl/curl.h>
#endif

namespace ppl7 {

/*!\class Curl
 * \ingroup PPLGroupInternet
 *
 * Die Klasse Curl ist eine Wrapper-Klasse zur Curl-Library, mit der Daten von
 * Web- und FTP-Server geholt werden können.
 *
 */

#ifdef HAVE_LIBCURL
static ppl7::Mutex CurlMutex;
static int ppl_curl_instance=0;
#endif


#ifdef HAVE_LIBCURL
	static size_t write_function(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		if (!stream) return -1;
		size_t bytes=size*nmemb;
		Curl *curl=static_cast<Curl*>(stream);
		return curl->storeResult(ptr,bytes,1);
	}

	//size_t function( void *ptr, size_t size, size_t nmemb, void *stream)
	static size_t header_function(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		if (!stream) return -1;
		size_t bytes=size*nmemb;
		Curl *curl=static_cast<Curl*>(stream);
		return curl->storeResult(ptr,bytes,2);
	}

	static int debug_function(CURL *handle, curl_infotype type, char *data, size_t size, void *stream)
	{
		if (!stream) return 0;
		Curl *curl=static_cast<Curl*>(stream);
		if (curl) curl->debugHandler((int)type,data,size);
		return 0;
	}

#endif

bool Curl::isSupported()
{
#ifdef HAVE_LIBCURL
	return true;
#else
	return false;
#endif
}

Curl::Curl()
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
	handle=NULL;
	resultbuffer_size=0;
	log=NULL;
	httppost=NULL;
	last_httppost=NULL;
	headers=NULL;
	resultbuffer=NULL;
	errorbuffer=NULL;
	aboard=false;

	#ifdef HAVE_LIBCURL
		CurlMutex.lock();
		if (!ppl_curl_instance) {
			if (!(curl_global_init(CURL_GLOBAL_WIN32|CURL_GLOBAL_SSL)==0)) {
				CurlMutex.unlock();
				throw Curl::InitializationFailedException();
			}
		}
		ppl_curl_instance++;
		CurlMutex.unlock();
		handle=curl_easy_init();
		if (!handle) {
			throw Curl::InitializationFailedException();
		}
		curl_easy_setopt((CURL*)handle,CURLOPT_COOKIEJAR,"");

		long t=1;
		curl_easy_setopt((CURL*)handle, CURLOPT_NOSIGNAL, t);	// Curl soll keine Signals verwenden


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

	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}

Curl::Curl(const Curl &other)
{
	handle=NULL;
	resultbuffer_size=0;
	log=NULL;
	httppost=NULL;
	last_httppost=NULL;
	headers=NULL;
	resultbuffer=NULL;
	errorbuffer=NULL;
	aboard=false;
	#ifdef HAVE_LIBCURL
		CurlMutex.lock();
		if (!ppl_curl_instance) {
			if (!(curl_global_init(CURL_GLOBAL_WIN32|CURL_GLOBAL_SSL)==0)) {
				CurlMutex.unlock();
				throw Curl::InitializationFailedException();
			}
		}
		ppl_curl_instance++;
		CurlMutex.unlock();
		handle=curl_easy_init();
		if (!handle) {
			throw Curl::InitializationFailedException();
		}
		curl_easy_setopt((CURL*)handle,CURLOPT_COOKIEJAR,"");

		long t=1;
		curl_easy_setopt((CURL*)handle, CURLOPT_NOSIGNAL, t);	// Curl soll keine Signals verwenden


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
		log=other.log;
		Header=other.Header;
		Browser=other.Browser;
		Url=other.Url;
		UserPassword=other.UserPassword;
		Referer=other.Referer;
		Proxy=other.Proxy;
		GetCall=other.GetCall;

	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}

Curl::~Curl()
{
	#ifdef HAVE_LIBCURL
		if (httppost) curl_formfree((curl_httppost*) httppost);
		if (headers) curl_slist_free_all((struct curl_slist *)headers);

		if (handle) {
			curl_easy_cleanup((CURL *) handle );
			handle=NULL;
		}
		if (errorbuffer) free(errorbuffer);
		//if (storetofile) delete storetofile;
		if (resultbuffer) free(resultbuffer);
		resultbuffer=NULL;

		CurlMutex.lock();
		if (ppl_curl_instance>0) {
			ppl_curl_instance--;
			if (!ppl_curl_instance) {
				curl_global_cleanup();
			}
		}
		CurlMutex.unlock();
	#endif
}

void Curl::clear()
{
#ifdef HAVE_LIBCURL
	if (httppost) curl_formfree((curl_httppost*) httppost);
	last_httppost=httppost=NULL;
	if (headers) {
		curl_slist_free_all((struct curl_slist *)headers);
		headers=NULL;
	}
	if (handle) curl_easy_reset((CURL *)handle );
#endif
}

void Curl::setLogger(Logger *log)
{
	this->log=log;
#ifdef HAVE_LIBCURL
	long t;
	if (log) {
		curl_easy_setopt((CURL*)handle,CURLOPT_DEBUGFUNCTION, debug_function);
		curl_easy_setopt((CURL*)handle,CURLOPT_DEBUGDATA, this);
		t=1;
		curl_easy_setopt((CURL*)handle,CURLOPT_VERBOSE, t);
	} else {
		t=0;
		curl_easy_setopt((CURL*)handle,CURLOPT_VERBOSE, t);
	}
#endif

}


void Curl::curlResultOk(int ret) const
{
	#ifdef HAVE_LIBCURL
		if (ret==CURLE_OK) return;
		const char *e=curl_easy_strerror((CURLcode)ret);
		switch (ret) {
			case CURLE_OPERATION_TIMEOUTED:
				throw TimeoutException("%s",e);
				break;
			case CURLE_COULDNT_RESOLVE_HOST:
				throw ResolverException("%s",e);
				break;
			case CURLE_COULDNT_CONNECT:
				throw ConnectionFailedException("%s",e);
				break;
			default:
				throw Curl::MiscException("result code=%i, error=%s",ret,e );
				break;
		}
		return;
	#else
		throw UnsupportedFeatureException("libCurl");
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
void Curl::enableSignals(bool enable)
{
#ifdef HAVE_LIBCURL
	long t;
	if (enable==true) t=0;
	else t=1;
	curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_NOSIGNAL, t));
#else
	throw UnsupportedFeatureException("libCurl");
#endif

}

void Curl::setBrowser(const String &browser)
{
	#ifdef HAVE_LIBCURL
		Browser=browser;
		curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_USERAGENT, (const char*)browser));
	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}

void Curl::setURL(const String &url)
{
	#ifdef HAVE_LIBCURL
		this->Url=url;
		GetCall.clear();
	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}

void Curl::setReferer(const String &url)
{
	#ifdef HAVE_LIBCURL
		Referer=url;
		curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_REFERER, (const char*)url));
	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}

void Curl::setUserPassword(const String &suserpassword)
{
	#ifdef HAVE_LIBCURL
		UserPassword=suserpassword;
		curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_USERPWD, (const char*)suserpassword));
	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}

void Curl::setUserPassword(const String &username, const String &password)
{
	#ifdef HAVE_LIBCURL
		UserPassword=username+":"+password;
		curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_USERPWD, (const char*)UserPassword));
	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}

void Curl::setUsername(const String &username)
{
	#ifdef HAVE_LIBCURL
		curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_USERNAME, (const char*)username));
	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}

void Curl::setPassword(const String &password)
{
	#ifdef HAVE_LIBCURL
		curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_PASSWORD, (const char*)password));
	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}

void Curl::setProxy(const String &proxy, int port)
{
	#ifdef HAVE_LIBCURL
		Proxy.setf("%s:%i",(const char*)proxy,port);
		curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_PROXY, (const char*)Proxy));
	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}

void Curl::setHttpVersion(Curl::HTTPVERSION version)
{
#ifdef HAVE_LIBCURL
	if (version==HTTP_2_0) {
		curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0));
	} else 	if (version==HTTP_1_1) {
		curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1));
	} else {
		curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0));
	}
#else
	throw UnsupportedFeatureException("libCurl");
#endif
}


void Curl::setTimeout(int seconds)
{
	#ifdef HAVE_LIBCURL
		long t=1;
		curl_easy_setopt((CURL*)handle, CURLOPT_NOSIGNAL, t);	// Curl soll keine Signals verwenden
		t=seconds;
		curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_TIMEOUT, t));
	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}


void Curl::setMaximumPersistantConnects(int value)
{
#ifdef HAVE_LIBCURL
	long t=value;
	curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_MAXCONNECTS, t));
#else
	throw UnsupportedFeatureException("libCurl");
#endif

}


void Curl::reset()
{
#ifdef HAVE_LIBCURL
	if (httppost) curl_formfree((curl_httppost*) httppost);
	httppost=NULL;
	if (headers) curl_slist_free_all((struct curl_slist *)headers);
	headers=NULL;

	if (handle) {
		curl_easy_cleanup((CURL *) handle );
		handle=NULL;
	}
	handle=curl_easy_init();
	if (!handle) {
		throw Curl::InitializationFailedException();
	}
	curl_easy_setopt((CURL*)handle,CURLOPT_COOKIEJAR,"");
	if (resultbuffer) free(resultbuffer);
	resultbuffer=NULL;
	errorbuffer[0]=0;
	curl_easy_setopt((CURL*)handle,CURLOPT_ERRORBUFFER,errorbuffer);
#else
	throw UnsupportedFeatureException("libCurl");
#endif
}

void Curl::escape(String &string)
{
	#ifdef HAVE_LIBCURL
		#if LIBCURL_VERSION_NUM >  0x070f03
			char *res=curl_easy_escape((CURL*)handle, string.getPtr(), string.size());
		#else
			char *res=curl_escape(string.getPtr(), string.size());
		#endif
		if (!res) {
			throw Curl::StringEscapeException(string);
		}
		string.set(res);
		curl_free(res);
	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}

void Curl::escape(String &target, const AssocArray &source)
{
	#ifdef HAVE_LIBCURL

		AssocArray::Iterator it;
		source.reset(it);
		String key,value;
		//char *k,*v;
		int c=0;
		while (source.getNext(it, key,value)) {
			#if LIBCURL_VERSION_NUM >  0x070f03
				char *k=curl_easy_escape((CURL*)handle, key.getPtr(), key.size());
				char *v=curl_easy_escape((CURL*)handle, value.getPtr(), value.size());
			#else
				char *k=curl_escape(key.getPtr(), key.size());
				char *v=curl_escape(value.getPtr(), value.size());
			#endif
			if ((!k) || (!v)) {
				if (k) curl_free(k);
				if (v) curl_free(v);
				throw Curl::StringEscapeException("key=%s, value=%s",(const char*)key,(const char*)value);
			}
			if (c) target.append("&");
			target.appendf("%s=%s",k,v);
			curl_free(k);
			curl_free(v);
			c++;
		}
	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}

void Curl::get(const String &parameter)
{
	#ifdef HAVE_LIBCURL
		GetCall=Url;
		if (parameter.notEmpty()) {
			if (GetCall.instr("?")>0) GetCall.append("&");
			else GetCall.append("?");
			GetCall.append(parameter);
		}
		get();
	#else
		throw UnsupportedFeatureException("libCurl");
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
void Curl::get(const AssocArray &param)
{
	#ifdef HAVE_LIBCURL
		GetCall=Url;
		if (GetCall.instr("?")>0) GetCall.append("&");
		else GetCall.append("?");
		escape(GetCall, param);
		get();
	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}

String Curl::getURL() const
{
	return Url;
}

void Curl::get()
{
	#ifdef HAVE_LIBCURL
		curl_easy_setopt((CURL*)handle,CURLOPT_WRITEDATA,this);
		curl_easy_setopt((CURL*)handle,CURLOPT_WRITEFUNCTION,write_function);
		curl_easy_setopt((CURL*)handle,CURLOPT_WRITEHEADER,this);
		curl_easy_setopt((CURL*)handle,CURLOPT_HEADERFUNCTION,header_function);


		if (GetCall.isEmpty()) GetCall=Url;
		if (GetCall.isEmpty()) {
			throw Curl::InvalidURLException();
		}
		curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_URL, (const char*)GetCall));

		long a=1;
		curlResultOk(curl_easy_setopt((CURL*)handle,CURLOPT_HTTPGET,a));
		// Falls der Server Redirects sendet. folgen wir diesen:
		curlResultOk(curl_easy_setopt((CURL*)handle,CURLOPT_FOLLOWLOCATION,a));

		if (headers) {
			curl_easy_setopt((CURL*)handle, CURLOPT_HTTPHEADER, headers);
		}
		if (resultbuffer) free(resultbuffer);
		resultbuffer=NULL;
		resultbuffer_size=0;
		Header="";
		//call_send.Notify(NULL);
		aboard=false;
		curlResultOk(curl_easy_perform((CURL*)handle));
		a=0;
		long num_connects=0, num_redirects=0;
		curl_easy_getinfo((CURL*)handle, CURLINFO_NUM_CONNECTS, &num_connects);
		curl_easy_getinfo((CURL*)handle, CURLINFO_REDIRECT_COUNT, &num_redirects);

		if (log) log->print(Logger::DEBUG,5,"ppl6::CCurl","Get",__FILE__,__LINE__,
				ToString("New Connects: %i, Redirects: %i",((int)num_connects), (int)num_redirects));
		//call_done.Notify(NULL);
	#else
		throw UnsupportedFeatureException("libCurl");
	#endif
}

size_t Curl::storeResult(void *ptr, size_t bytes, int type)
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
			memcpy((uint8_t*)resultbuffer+resultbuffer_size,ptr,bytes);
			resultbuffer_size+=bytes;
			resultbuffer[resultbuffer_size]=0;
			//call_receive.Notify(&bytes);
			return bytes;
		} else if (type==2) {
			Header.append((const char*)ptr,bytes);
			return bytes;
		}
	#endif
	return -1;
}



String Curl::getLastURL() const
{
#ifdef HAVE_LIBCURL
	char *u;
	curlResultOk(curl_easy_getinfo((CURL*)handle, CURLINFO_EFFECTIVE_URL , &u));
	return u;
#else
	throw UnsupportedFeatureException("libCurl");
#endif
}

void Curl::getResultBuffer(void **buffer, size_t *size) const
{
	if (!resultbuffer) throw Curl::NoResultException();
	*buffer=resultbuffer;
	*size=resultbuffer_size;
}

ByteArrayPtr Curl::getResultBuffer() const
{
	if (!resultbuffer) throw Curl::NoResultException();
	return ByteArrayPtr(resultbuffer, resultbuffer_size);
}

void Curl::copyResultBuffer(ByteArray &bin) const
{
	if (!resultbuffer) throw Curl::NoResultException();
	bin.copy(resultbuffer,resultbuffer_size);
}

String Curl::getResultBufferAsString() const
{
	String ret;
	if (resultbuffer) ret.set(resultbuffer,resultbuffer_size);
	return ret;
}

String Curl::getHeader() const
{
	return Header;
}

void Curl::debugHandler(int type, const char *data, size_t size)
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
	String msg;
	msg.set(data,size);
	switch (type) {
		case CURLINFO_TEXT:
			log->print(Logger::DEBUG,5,"CCurl","DebugHandler",__FILE__,__LINE__,
					ToString("INFO: %s",(const char*)msg));
			/*
			if (strncmp(d,"Getting file with size:",23)==0) {
				filesize=atol(d+24);
			}
			*/
			break;
		case CURLINFO_HEADER_IN:
			log->print(Logger::DEBUG,5,"CCurl","DebugHandler",__FILE__,__LINE__,
					ToString("HEADER_IN: %s",(const char*)msg));
			/*
			if (strncmp(d,"Content-Length: ",16)==0) {
				filesize=atol(d+16);
				if ((comdebug) && (PPLCore)) PPLCore->Printf("Filesize detected: %u",filesize);
			}
			*/
			break;
		case CURLINFO_HEADER_OUT:
			log->print(Logger::DEBUG,5,"CCurl","DebugHandler",__FILE__,__LINE__,
					ToString("HEADER_OUT: %s",(const char*)msg));
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
			log->print(Logger::DEBUG,10,"CCurl","DebugHandler",__FILE__,__LINE__,
					ToString("DATA_IN: %s",(const char*)msg));
			//call_receive.Notify(NULL);
			break;
		case CURLINFO_DATA_OUT:
			log->print(Logger::DEBUG,10,"CCurl","DebugHandler",__FILE__,__LINE__,
					ToString("DATA_OUT: %s",(const char*)msg));
			//call_send.Notify(NULL);
			break;
	}
#endif
}


void Curl::setHeader(const String &name, const String &value)
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
	String Buffer;
	Buffer.setf("%s: %s",(const char*) name, (const char*)value);
	struct curl_slist *slist;
	slist = curl_slist_append((struct curl_slist *)headers, (const char *)Buffer);
	if (!slist) {
		throw OutOfMemoryException();
	}
	headers=slist;
#else
	throw UnsupportedFeatureException("libCurl");
#endif
}

void Curl::clearHeader()
/*!\brief Angepasste HTTP-Header löschen
 *
 * Mit dieser Funktion werden alle angepassten Header-Zeilen gelöscht. Siehe auch
 * CCurl::SetHeader.
 *
 * @return Bei Erfolg gibt die Funktion 1 zurück, im Fehlerfall 0.
 */
{
#ifdef HAVE_LIBCURL
	curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_HTTPHEADER, NULL));
	if (headers) {
		curl_slist_free_all((struct curl_slist *)headers);
		headers=NULL;
	}
#else
	throw UnsupportedFeatureException("libCurl");
#endif
}

void Curl::addPostVar(const String &name, int val, const String &contenttype)
{
	String s;
	s.setf("%i",val);
	addPostVar(name,s,contenttype);
}

void Curl::addPostVar(const String &name, const String &data, const String &contenttype)
{
#ifdef HAVE_LIBCURL
	if (contenttype.isEmpty()) {
		curl_formadd((curl_httppost**)&httppost, (curl_httppost**)&last_httppost,
			CURLFORM_COPYNAME, (const char*)name,
			CURLFORM_COPYCONTENTS, (const char*)data,
			CURLFORM_END);
	} else {
		curl_formadd((curl_httppost**)&httppost, (curl_httppost**)&last_httppost,
			CURLFORM_COPYNAME, (const char*)name,
			CURLFORM_COPYCONTENTS, (const char*)data,
			CURLFORM_CONTENTTYPE, (const char*)contenttype,
			CURLFORM_END);
	}
#else
	throw UnsupportedFeatureException("libCurl");
#endif
}

void Curl::addPostVar(const AssocArray &param, const String &prefix)
{
#ifdef HAVE_LIBCURL
	String key;
	String Prefix,name;
	AssocArray::Iterator it;
	param.reset(it);
	while (param.getNext(it)) {
		key=it.key();
		if (prefix.notEmpty()) {
			name=prefix;
			name.appendf("[%s]",(const char*)key);
		} else {
			name=key;
		}
		if (it.value().isString()) {
			String s=it.value().toString();
			curl_formadd((curl_httppost**)&httppost, (curl_httppost**)&last_httppost,
				CURLFORM_COPYNAME, (const char*)name,
				CURLFORM_COPYCONTENTS, (const char*)s,
				CURLFORM_END);

		} else if (it.value().isWideString()) {
			String s=it.value().toWideString();
			curl_formadd((curl_httppost**)&httppost, (curl_httppost**)&last_httppost,
				CURLFORM_COPYNAME, (const char*)name,
				CURLFORM_COPYCONTENTS, (const char*)s,
				CURLFORM_END);
		} else if (it.value().isAssocArray()) {
			AssocArray a=it.value().toAssocArray();
			addPostVar(a,name);
		}
	}
#else
	throw UnsupportedFeatureException("libCurl");
#endif
}

void Curl::post(const AssocArray &param)
/*!\brief Daten Multipart-encoded senden
 *
 */
{
	addPostVar(param);
	post();
}

void Curl::post()
{
#ifdef HAVE_LIBCURL
	CURLcode ret;
	// Falls der Server Redirects sendet. folgen wir diesen:
	curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_URL, (const char*)Url));
	ret=curl_easy_setopt((CURL*)handle, CURLOPT_FOLLOWLOCATION, 1);
	if (httppost) ret=curl_easy_setopt((CURL*)handle,CURLOPT_HTTPPOST,httppost);

	if (headers) {
		curl_easy_setopt((CURL*)handle, CURLOPT_HTTPHEADER, headers);
	}

	curl_easy_setopt((CURL*)handle,CURLOPT_WRITEFUNCTION,write_function);
	curl_easy_setopt((CURL*)handle,CURLOPT_WRITEDATA,this);
	if (resultbuffer) free(resultbuffer);
	resultbuffer=NULL;
	resultbuffer_size=0;
	Header="";

	//call_send.Notify(NULL);
	aboard=false;
	ret=curl_easy_perform((CURL*)handle);
	//call_done.Notify(NULL);
	if (httppost) curl_formfree((curl_httppost*) httppost);
	httppost=last_httppost=NULL;
	if (ret==0) return;
	throw Curl::OperationFailedException("%s",errorbuffer);
#else
	throw UnsupportedFeatureException("libCurl");
#endif

}

void Curl::postFields(const AssocArray &param)
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
	String Data;
	escape(Data,param);
	curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_POSTFIELDS, (const char*)Data));
	post();
#else
	throw UnsupportedFeatureException("libCurl");
#endif
}


void Curl::addCAFile(const String &filename)
{
#ifdef HAVE_LIBCURL
	curlResultOk(curl_easy_setopt((CURL*)handle, CURLOPT_CAINFO, (const char*)filename));
#else
	throw UnsupportedFeatureException("libCurl");
#endif
}

void Curl::verifyPeer(bool verify)
{
#ifdef HAVE_LIBCURL
	int v=verify;
	curlResultOk(curl_easy_setopt((CURL*)handle,CURLOPT_SSL_VERIFYPEER , v));
#else
	throw UnsupportedFeatureException("libCurl");
#endif
}

void *Curl::getCurlHandle() const
{
#ifdef HAVE_LIBCURL
	return handle;
#else
	throw UnsupportedFeatureException("libCurl");
#endif
}


String Curl::getUri(const String &uri)
{
	Curl curl;
	curl.setURL(uri);
	curl.get();
	return curl.getResultBufferAsString();
}
} // namespace ppl7



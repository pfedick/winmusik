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

#ifdef HAVE_ERRNO_H
	#include <errno.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <winerror.h>
#endif


#include "ppl7.h"
#include "ppl7-inet.h"

#ifdef WIN32
#define strdup _strdup
#endif

namespace ppl7 {

Exception::Exception() throw()
{
	ErrorText=NULL;
}

Exception::~Exception() throw()
{
	if (ErrorText) free(ErrorText);
}

const char* Exception::what() const throw()
{
	return "PPLException";
}

Exception::Exception(const Exception &other) throw()
{
	if (other.ErrorText) {
		ErrorText=strdup(other.ErrorText);
	} else {
		ErrorText=NULL;
	}
}

Exception& Exception::operator= (const Exception &other) throw()
{
	if (other.ErrorText) {
		ErrorText=strdup(other.ErrorText);
	} else {
		ErrorText=NULL;
	}
	return *this;
}

Exception::Exception(const char *msg, ...) throw()
{
	if (msg) {
		String Msg;
		va_list args;
		va_start(args, msg);
		try {
			Msg.vasprintf(msg,args);
			ErrorText=strdup((const char*)Msg);
		} catch (...) {
			ErrorText=NULL;
		}
		va_end(args);

	} else {
		ErrorText=NULL;
	}
}

void Exception::copyText(const char *str) throw()
{
	free(ErrorText);
	ErrorText=strdup(str);
}

void Exception::copyText(const char *fmt, va_list args) throw()
{
	free(ErrorText);
	try {
		String Msg;
		Msg.vasprintf(fmt,args);
		ErrorText=strdup((const char*)Msg);
	} catch (...) {
		ErrorText=NULL;
	}
}

const char* Exception::text() const throw()
{
	if (ErrorText) return ErrorText;
	else return "";
}

String Exception::toString() const throw()
{
	String str;
	str.setf("%s", what());
	if (ErrorText) str.appendf(" [%s]", (const char*)ErrorText);
	return str;
}

void Exception::print() const
{
	PrintDebug("Exception: %s", what());
	if (ErrorText) PrintDebug(" [%s]", (const char*)ErrorText);
	PrintDebug("\n");
}

std::ostream& operator<<(std::ostream& s, const Exception &e)
{
	String str = e.toString();
	return s.write((const char*)str, str.size());
}



/*!\brief %Exception anhand errno-Variable werfen
 *
 * \desc
 * Diese Funktion wird verwendet, um nach Auftreten eines Fehlers, anhand der globalen
 * "errno"-Variablen die passende Exception zu werfen.
 *
 * @param e Errorcode aus der errno-Variablen
 * @param info Zusätzliche Informationen zum Fehler (optional)
 */
void throwExceptionFromErrno(int e, const String &info)
{
	switch (e) {
	case ENOMEM: throw OutOfMemoryException();
	case EINVAL: throw InvalidArgumentsException();
	case ENOTDIR:
	case ENAMETOOLONG: throw InvalidFileNameException(info);
	case EACCES:
	case EPERM: throw PermissionDeniedException(info);
	case ENOENT: throw FileNotFoundException(info);
#ifdef ELOOP
	case ELOOP: throw TooManySymbolicLinksException(info);
#endif
	case EISDIR: throw NoRegularFileException(info);
	case EROFS: throw ReadOnlyException(info);
	case EMFILE: throw TooManyOpenFilesException();
#ifdef EOPNOTSUPP
	case EOPNOTSUPP: throw UnsupportedFileOperationException(info);
#endif
	case ENOSPC: throw FilesystemFullException();
#ifdef EDQUOT
	case EDQUOT: throw QuotaExceededException();
#endif
	case EIO: throw IOErrorException();
	case EBADF: throw BadFiledescriptorException();
	case EFAULT: throw BadAddressException();
#ifdef EOVERFLOW
	case EOVERFLOW: throw OverflowException();
#endif
	case EEXIST: throw FileExistsException();
	case EAGAIN: throw OperationBlockedException();
	case EDEADLK: throw DeadlockException();
	case EINTR: throw OperationInterruptedException();
	case ENOLCK: throw TooManyLocksException();
	case ESPIPE: throw IllegalOperationOnPipeException();
	case ETIMEDOUT: throw TimeoutException(info);

	case ENETDOWN: throw NetworkDownException(info);
	case ENETUNREACH: throw NetworkUnreachableException(info);
	case ENETRESET: throw NetworkDroppedConnectionOnResetException(info);
	case ECONNABORTED: throw SoftwareCausedConnectionAbortException(info);
	case ECONNRESET: throw ConnectionResetByPeerException(info);
	case ENOBUFS: throw NoBufferSpaceException(info);
	case EISCONN: throw SocketIsAlreadyConnectedException(info);
	case ENOTCONN: throw NotConnectedException(info);
#ifdef ESHUTDOWN
	case ESHUTDOWN: throw CantSendAfterSocketShutdownException(info);
#endif
#ifdef ETOOMANYREFS
	case ETOOMANYREFS: throw TooManyReferencesException(info);
#endif
	case ECONNREFUSED: throw ConnectionRefusedException(info);
#ifdef EHOSTDOWN
	case EHOSTDOWN: throw HostDownException(info);
#endif
	case EHOSTUNREACH: throw NoRouteToHostException(info);
	case ENOTSOCK: throw InvalidSocketException(info);
	case ENOPROTOOPT: throw UnknownOptionException(info);
	case EPIPE: throw BrokenPipeException(info);
	case EINPROGRESS: throw OperationBlockedException(info);
	case EALREADY: throw OperationAlreadyInProgressException(info);
	case EDESTADDRREQ: throw DestinationAddressRequiredException(info);
	case EMSGSIZE: throw MessageTooLongException(info);
	case EPROTOTYPE: throw ProtocolWrongTypeForSocketException(info);
	default: {
		String ret;
#ifdef HAVE_STRERROR_S
		ByteArray buffer(128);
		if (NULL == strerror_s((char*)buffer.ptr(), buffer.size(), e)) {
			ret.set((const char*)buffer);
		}
#else
				ret=strerror(e);
#endif
			ret+=": "+info;
			throw UnknownException(ret);
		}
	}
}

void throwSocketException(int e,const String &info)
{
#ifndef WIN32
	throwExceptionFromErrno(e,info);
#else
	switch (e) {
		case WSA_INVALID_HANDLE: throw InvalidArgumentsException(info);
		case WSA_NOT_ENOUGH_MEMORY: throw OutOfMemoryException(info);
		case WSA_INVALID_PARAMETER: throw InvalidArgumentsException(info);
		case WSA_OPERATION_ABORTED: throw OperationAbortedException(info);
		case WSA_IO_INCOMPLETE: throw ObjectNotInSignaledStateException(info);
		case WSA_IO_PENDING: throw OverlappedOperationPendingException(info);
		case WSAEINTR: throw OperationInterruptedException(info);
		case WSAEBADF: throw BadFiledescriptorException(info);
		case WSAEACCES: throw PermissionDeniedException(info);
		case WSAEFAULT: throw BadAddressException(info);
		case WSAEINVAL: throw InvalidArgumentsException(info);
		case WSAEMFILE: throw TooManyOpenFilesException(info);
		case WSAEWOULDBLOCK: throw OperationBlockedException(info);
		case WSAEINPROGRESS: throw OperationInProgressException(info);
		case WSAEALREADY: throw OperationAlreadyInProgressException(info);
		case WSAENOTSOCK: throw SocketOperationOnNonSocketException(info);
		case WSAEDESTADDRREQ: throw DestinationAddressRequiredException(info);
		case WSAEMSGSIZE: throw MessageTooLongException(info);
		case WSAEPROTOTYPE: throw ProtocolWrongTypeForSocketException(info);
		case WSAENOPROTOOPT: throw ProtocolNotAvailableException(info);
		case WSAEPROTONOSUPPORT: throw ProtocolNotSupportedException(info);
		case WSAESOCKTNOSUPPORT: throw SocketTypeNotSupportedException(info);
		case WSAEOPNOTSUPP: throw UnsupportedFileOperationException(info);
		case WSAEPFNOSUPPORT: throw ProtocolFamilyNotSupportedException(info);
		case WSAEAFNOSUPPORT: throw AddressFamilyNotSupportedException(info);
		case WSAEADDRINUSE: throw AddressAlreadyInUseException(info);
		case WSAEADDRNOTAVAIL: throw AddressNotAvailableException(info);
		case WSAENETDOWN: throw NetworkIsDownException(info);
		case WSAENETUNREACH: throw NetworkUnreachableException(info);
		case WSAENETRESET: throw ConnectionAbortedByNetworkException(info);
		case WSAECONNABORTED: throw ConnectionAbortedException(info);
		case WSAECONNRESET: throw ConnectionResetException(info);
		case WSAENOBUFS: throw NoBufferSpaceAvailableException(info);
		case WSAEISCONN: throw SocketIsConnectedException(info);
		case WSAENOTCONN: throw SocketNotConnectedException(info);
		case WSAESHUTDOWN: throw TransportEndpointHasShutdownException(info);
		case WSAETOOMANYREFS: throw TooManyReferencesException(info);
		case WSAETIMEDOUT: throw ConnectionTimeoutException(info);
		case WSAECONNREFUSED: throw ConnectionRefusedException(info);
		case WSAELOOP: throw InvalidFileNameException(info);
		case WSAENAMETOOLONG: throw InvalidFileNameException(info);
		case WSAEHOSTDOWN: throw HostDownException(info);
		case WSAEHOSTUNREACH: throw HostIsUnreachableException(info);
		case WSAENOTEMPTY: throw DirectoryNotEmptyException(info);
		case WSAEPROCLIM: throw ProcessLimitException(info);
		case WSAEUSERS: throw TooManyUsersException(info);
		case WSAEDQUOT: throw QuotaExceededException(info);
		case WSAESTALE: throw StaleFileHandleException(info);
		case WSAEREMOTE: throw ObjectIsRemoteException(info);
		case WSASYSNOTREADY: throw NetworkSubsystemUnavailableException(info);
		case WSAVERNOTSUPPORTED: throw UnsupportedWinsockVersionException(info);
		case WSANOTINITIALISED: throw NotInitializedException(info);
		case WSAEDISCON: throw GracefulShutdownInProgressException(info);
		case WSAENOMORE: throw NoMoreResultsException(info);
		case WSAECANCELLED: throw CallHasBeenCanceledException(info);
		case WSAEINVALIDPROCTABLE: throw ProcedureCallTableIsInvalidException(info);
		case WSAEINVALIDPROVIDER: throw ServiceProviderIsInvalidException(info);
		case WSAEPROVIDERFAILEDINIT: throw ServiceProviderFailedToInitializeException(info);
		case WSASYSCALLFAILURE: throw SystemCallFailureException(info);
		case WSASERVICE_NOT_FOUND: throw ServiceNotFoundException(info);
		case WSATYPE_NOT_FOUND: throw ClassTypeNotFoundException(info);
		case WSA_E_NO_MORE: throw NoMoreResultsException(info);
		case WSA_E_CANCELLED: throw CallHasBeenCanceledException(info);
		case WSAEREFUSED: throw QueryRefusedException(info);
		case WSAHOST_NOT_FOUND: throw HostNotFoundException(info);
		case WSATRY_AGAIN: throw NonauthoritativeHostNotFound(info);
		case WSANO_RECOVERY: throw UnrecoverableErrorException(info);
		case WSA_QOS_RECEIVERS:
		case WSA_QOS_SENDERS:
		case WSA_QOS_NO_SENDERS:
		case WSA_QOS_NO_RECEIVERS:
		case WSA_QOS_REQUEST_CONFIRMED:
		case WSA_QOS_ADMISSION_FAILURE:
		case WSA_QOS_POLICY_FAILURE:
		case WSA_QOS_BAD_STYLE:
		case WSA_QOS_BAD_OBJECT:
		case WSA_QOS_TRAFFIC_CTRL_ERROR:
		case WSA_QOS_GENERIC_ERROR:
		case WSA_QOS_ESERVICETYPE:
		case WSA_QOS_EFLOWSPEC:
		case WSA_QOS_EPROVSPECBUF:
		case WSA_QOS_EFILTERSTYLE:
		case WSA_QOS_EFILTERTYPE:
		case WSA_QOS_EFILTERCOUNT:
		case WSA_QOS_EOBJLENGTH:
		case WSA_QOS_EUNKOWNPSOBJ:
		case WSA_QOS_EPOLICYOBJ:
		case WSA_QOS_EFLOWDESC:
		case WSA_QOS_EPSFLOWSPEC:
		case WSA_QOS_EPSFILTERSPEC:
		case WSA_QOS_ESDMODEOBJ:
		case WSA_QOS_ESHAPERATEOBJ:
		case WSA_QOS_RESERVED_PETYPE:
			throw QoSException(info);
		default: throwExceptionFromErrno(e,info);
	}
#endif
}


}

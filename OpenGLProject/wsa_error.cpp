/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	wsa_error.cpp
**
**	Summary		:	Exception for WSA error codes.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

// Library Includes
#include <WinSock2.h>

// This Include
#include "wsa_error.hpp"



char const* GetWSAErrorInfoMessage(int error)
{
	switch (error)
	{
	case WSA_INVALID_HANDLE:
		return
			"WSA_INVALID_HANDLE\n"
			"Specified event object handle is invalid.\n"
			"An application attempts to use an event object, but the specified handle is not valid. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.\n"
			; break;
	case WSA_NOT_ENOUGH_MEMORY:
		return
			"WSA_NOT_ENOUGH_MEMORY\n"
			"Insufficient memory available.\n"
			"An application used a Windows Sockets function that directly maps to a Windows function. The Windows function is indicating a lack of required memory resources. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.\n"
			; break;
	case WSA_INVALID_PARAMETER:
		return
			"WSA_INVALID_PARAMETER\n"
			"One or more parameters are invalid.\n"
			"An application used a Windows Sockets function which directly maps to a Windows function. The Windows function is indicating a problem with one or more parameters. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.\n"
			; break;
	case WSA_OPERATION_ABORTED:
		return
			"WSA_OPERATION_ABORTED\n"
			"Overlapped operation aborted.\n"
			"An overlapped operation was canceled due to the closure of the socket, or the execution of the SIO_FLUSH command in WSAIoctl. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.\n"
			; break;
	case WSA_IO_INCOMPLETE:
		return
			"WSA_IO_INCOMPLETE\n"
			"Overlapped I/O event object not in signaled state.\n"
			"The application has tried to determine the status of an overlapped operation which is not yet completed. Applications that use WSAGetOverlappedResult (with the fWait flag set to false) in a polling mode to determine when an overlapped operation has completed, get this error code until the operation is complete. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.\n"
			; break;
	case WSA_IO_PENDING:
		return
			"WSA_IO_PENDING\n"
			"Overlapped operations will complete later.\n"
			"The application has initiated an overlapped operation that cannot be completed immediately. A completion indication will be given later when the operation has been completed. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.\n"
			; break;
	case WSAEINTR:
		return
			"WSAEINTR\n"
			"Interrupted function call.\n"
			"A blocking operation was interrupted by a call to WSACancelBlockingCall.\n"
			; break;
	case WSAEBADF:
		return
			"WSAEBADF\n"
			"File handle is not valid.\n"
			"The file handle supplied is not valid.\n"
			; break;
	case WSAEACCES:
		return
			"WSAEACCES\n"
			"Permission denied.\n"
			"An attempt was made to access a socket in a way forbidden by its access permissions. An example is using a broadcast address for sendto without broadcast permission being set using setsockopt(SO_BROADCAST).    Another possible reason for the WSAEACCES error is that when the bind function is called (on Windows NT 4.0 with SP4 and later), another application, service, or kernel mode driver is bound to the same address with exclusive access. Such exclusive access is a new feature of Windows NT 4.0 with SP4 and later, and is implemented by using the SO_EXCLUSIVEADDRUSE option.\n"
			; break;
	case WSAEFAULT:
		return
			"WSAEFAULT\n"
			"Bad address.\n"
			"The system detected an invalid pointer address in attempting to use a pointer argument of a call. This error occurs if an application passes an invalid pointer value, or if the length of the buffer is too small. For instance, if the length of an argument, which is a sockaddr structure, is smaller than the sizeof(sockaddr).\n"
			; break;
	case WSAEINVAL:
		return
			"WSAEINVAL\n"
			"Invalid argument.\n"
			"Some invalid argument was supplied (for example, specifying an invalid level to the setsockopt function). In some instances, it also refers to the current state of the socket—for instance, calling accept on a socket that is not listening.\n"
			; break;
	case WSAEMFILE:
		return
			"WSAEMFILE\n"
			"Too many open files.\n"
			"Too many open sockets. Each implementation may have a maximum number of socket handles available, either globally, per process, or per thread.\n"
			; break;
	case WSAEWOULDBLOCK:
		return
			"WSAEWOULDBLOCK\n"
			"Resource temporarily unavailable.\n"
			"This error is returned from operations on nonblocking sockets that cannot be completed immediately, for example recv when no data is queued to be read from the socket. It is a nonfatal error, and the operation should be retried later. It is normal for WSAEWOULDBLOCK to be reported as the result from calling connect on a nonblocking SOCK_STREAM socket, since some time must elapse for the connection to be established.\n"
			; break;
	case WSAEINPROGRESS:
		return
			"WSAEINPROGRESS\n"
			"Operation now in progress.\n"
			"A blocking operation is currently executing. Windows Sockets only allows a single blocking operation—per- task or thread—to be outstanding, and if any other function call is made (whether or not it references that or any other socket) the function fails with the WSAEINPROGRESS error.\n"
			; break;
	case WSAEALREADY:
		return
			"WSAEALREADY\n"
			"Operation already in progress.\n"
			"An operation was attempted on a nonblocking socket with an operation already in progress—that is, calling connect a second time on a nonblocking socket that is already connecting, or canceling an asynchronous request (WSAAsyncGetXbyY) that has already been canceled or completed.\n"
			; break;
	case WSAENOTSOCK:
		return
			"WSAENOTSOCK\n"
			"Socket operation on nonsocket.\n"
			"An operation was attempted on something that is not a socket. Either the socket handle parameter did not reference a valid socket, or for select, a member of an fd_set was not valid.\n"
			; break;
	case WSAEDESTADDRREQ:
		return
			"WSAEDESTADDRREQ\n"
			"Destination address required.\n"
			"A required address was omitted from an operation on a socket. For example, this error is returned if sendto is called with the remote address of ADDR_ANY.\n"
			; break;
	case WSAEMSGSIZE:
		return
			"WSAEMSGSIZE\n"
			"Message too long.\n"
			"A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram was smaller than the datagram itself.\n"
			; break;
	case WSAEPROTOTYPE:
		return
			"WSAEPROTOTYPE\n"
			"Protocol wrong type for socket.\n"
			"A protocol was specified in the socket function call that does not support the semantics of the socket type requested. For example, the ARPA Internet UDP protocol cannot be specified with a socket type of SOCK_STREAM.\n"
			; break;
	case WSAENOPROTOOPT:
		return
			"WSAENOPROTOOPT\n"
			"Bad protocol option.\n"
			"An unknown, invalid or unsupported option or level was specified in a getsockopt or setsockopt call.\n"
			; break;
	case WSAEPROTONOSUPPORT:
		return
			"WSAEPROTONOSUPPORT\n"
			"Protocol not supported.\n"
			"The requested protocol has not been configured into the system, or no implementation for it exists. For example, a socket call requests a SOCK_DGRAM socket, but specifies a stream protocol.\n"
			; break;
	case WSAESOCKTNOSUPPORT:
		return
			"WSAESOCKTNOSUPPORT\n"
			"Socket type not supported.\n"
			"The support for the specified socket type does not exist in this address family. For example, the optional type SOCK_RAW might be selected in a socket call, and the implementation does not support SOCK_RAW sockets at all.\n"
			; break;
	case WSAEOPNOTSUPP:
		return
			"WSAEOPNOTSUPP\n"
			"Operation not supported.\n"
			"The attempted operation is not supported for the type of object referenced. Usually this occurs when a socket descriptor to a socket that cannot support this operation is trying to accept a connection on a datagram socket.\n"
			; break;
	case WSAEPFNOSUPPORT:
		return
			"WSAEPFNOSUPPORT\n"
			"Protocol family not supported.\n"
			"The protocol family has not been configured into the system or no implementation for it exists. This message has a slightly different meaning from WSAEAFNOSUPPORT. However, it is interchangeable in most cases, and all Windows Sockets functions that return one of these messages also specify WSAEAFNOSUPPORT.\n"
			; break;
	case WSAEAFNOSUPPORT:
		return
			"WSAEAFNOSUPPORT\n"
			"Address family not supported by protocol family.\n"
			"An address incompatible with the requested protocol was used. All sockets are created with an associated address family (that is, AF_INET for Internet Protocols) and a generic protocol type (that is, SOCK_STREAM). This error is returned if an incorrect protocol is explicitly requested in the socket call, or if an address of the wrong family is used for a socket, for example, in sendto.\n"
			; break;
	case WSAEADDRINUSE:
		return
			"WSAEADDRINUSE\n"
			"Address already in use.\n"
			"Typically, only one usage of each socket address (protocol/IP address/port) is permitted. This error occurs if an application attempts to bind a socket to an IP address/port that has already been used for an existing socket, or a socket that was not closed properly, or one that is still in the process of closing. For server applications that need to bind multiple sockets to the same port number, consider using setsockopt (SO_REUSEADDR). Client applications usually need not call bind at all— connect chooses an unused port automatically. When bind is called with a wildcard address (involving ADDR_ANY), a WSAEADDRINUSE error could be delayed until the specific address is committed. This could happen with a call to another function later, including connect, listen, WSAConnect, or WSAJoinLeaf.\n"
			; break;
	case WSAEADDRNOTAVAIL:
		return
			"WSAEADDRNOTAVAIL\n"
			"Cannot assign requested address.\n"
			"The requested address is not valid in its context. This normally results from an attempt to bind to an address that is not valid for the local computer. This can also result from connect, sendto, WSAConnect, WSAJoinLeaf, or WSASendTo when the remote address or port is not valid for a remote computer (for example, address or port 0).\n"
			; break;
	case WSAENETDOWN:
		return
			"WSAENETDOWN\n"
			"Network is down.\n"
			"A socket operation encountered a dead network. This could indicate a serious failure of the network system (that is, the protocol stack that the Windows Sockets DLL runs over), the network interface, or the local network itself.\n"
			; break;
	case WSAENETUNREACH:
		return
			"WSAENETUNREACH\n"
			"Network is unreachable.\n"
			"A socket operation was attempted to an unreachable network. This usually means the local software knows no route to reach the remote host.\n"
			; break;
	case WSAENETRESET:
		return
			"WSAENETRESET\n"
			"Network dropped connection on reset.\n"
			"The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress. It can also be returned by setsockopt if an attempt is made to set SO_KEEPALIVE on a connection that has already failed.\n"
			; break;
	case WSAECONNABORTED:
		return
			"WSAECONNABORTED\n"
			"Software caused connection abort.\n"
			"An established connection was aborted by the software in your host computer, possibly due to a data transmission time-out or protocol error.\n"
			; break;
	case WSAECONNRESET:
		return
			"WSAECONNRESET\n"
			"Connection reset by peer.\n"
			"An existing connection was forcibly closed by the remote host. This normally results if the peer application on the remote host is suddenly stopped, the host is rebooted, the host or remote network interface is disabled, or the remote host uses a hard close (see setsockopt for more information on the SO_LINGER option on the remote socket). This error may also result if a connection was broken due to keep-alive activity detecting a failure while one or more operations are in progress. Operations that were in progress fail with WSAENETRESET. Subsequent operations fail with WSAECONNRESET.\n"
			; break;
	case WSAENOBUFS:
		return
			"WSAENOBUFS\n"
			"No buffer space available.\n"
			"An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.\n"
			; break;
	case WSAEISCONN:
		return
			"WSAEISCONN\n"
			"Socket is already connected.\n"
			"A connect request was made on an already-connected socket. Some implementations also return this error if sendto is called on a connected SOCK_DGRAM socket (for SOCK_STREAM sockets, the to parameter in sendto is ignored) although other implementations treat this as a legal occurrence.\n"
			; break;
	case WSAENOTCONN:
		return
			"WSAENOTCONN\n"
			"Socket is not connected.\n"
			"A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using sendto) no address was supplied. Any other type of operation might also return this error—for example, setsockopt setting SO_KEEPALIVE if the connection has been reset.\n"
			; break;
	case WSAESHUTDOWN:
		return
			"WSAESHUTDOWN\n"
			"Cannot send after socket shutdown.\n"
			"A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call. By calling shutdown a partial close of a socket is requested, which is a signal that sending or receiving, or both have been discontinued.\n"
			; break;
	case WSAETOOMANYREFS:
		return
			"WSAETOOMANYREFS\n"
			"Too many references.\n"
			"Too many references to some kernel object.\n"
			; break;
	case WSAETIMEDOUT:
		return
			"WSAETIMEDOUT\n"
			"Connection timed out.\n"
			"A connection attempt failed because the connected party did not properly respond after a period of time, or the established connection failed because the connected host has failed to respond.\n"
			; break;
	case WSAECONNREFUSED:
		return
			"WSAECONNREFUSED\n"
			"Connection refused.\n"
			"No connection could be made because the target computer actively refused it. This usually results from trying to connect to a service that is inactive on the foreign host—that is, one with no server application running.\n"
			; break;
	case WSAELOOP:
		return
			"WSAELOOP\n"
			"Cannot translate name.\n"
			"Cannot translate a name.\n"
			; break;
	case WSAENAMETOOLONG:
		return
			"WSAENAMETOOLONG\n"
			"Name too long.\n"
			"A name component or a name was too long.\n"
			; break;
	case WSAEHOSTDOWN:
		return
			"WSAEHOSTDOWN\n"
			"Host is down.\n"
			"A socket operation failed because the destination host is down. A socket operation encountered a dead host. Networking activity on the local host has not been initiated. These conditions are more likely to be indicated by the error WSAETIMEDOUT.\n"
			; break;
	case WSAEHOSTUNREACH:
		return
			"WSAEHOSTUNREACH\n"
			"No route to host.\n"
			"A socket operation was attempted to an unreachable host. See WSAENETUNREACH.\n"
			; break;
	case WSAENOTEMPTY:
		return
			"WSAENOTEMPTY\n"
			"Directory not empty.\n"
			"Cannot remove a directory that is not empty.\n"
			; break;
	case WSAEPROCLIM:
		return
			"WSAEPROCLIM\n"
			"Too many processes.\n"
			"A Windows Sockets implementation may have a limit on the number of applications that can use it simultaneously. WSAStartup may fail with this error if the limit has been reached.\n"
			; break;
	case WSAEUSERS:
		return
			"WSAEUSERS\n"
			"User quota exceeded.\n"
			"Ran out of user quota.\n"
			; break;
	case WSAEDQUOT:
		return
			"WSAEDQUOT\n"
			"Disk quota exceeded.\n"
			"Ran out of disk quota.\n"
			; break;
	case WSAESTALE:
		return
			"WSAESTALE\n"
			"Stale file handle reference.\n"
			"The file handle reference is no longer available.\n"
			; break;
	case WSAEREMOTE:
		return
			"WSAEREMOTE\n"
			"Item is remote.\n"
			"The item is not available locally.\n"
			; break;
	case WSASYSNOTREADY:
		return
			"WSASYSNOTREADY\n"
			"Network subsystem is unavailable.\n"
			"This error is returned by WSAStartup if the Windows Sockets implementation cannot function at this time because the underlying system it uses to provide network services is currently unavailable. Users should check:\n"
			; break;
	default:
		return
			"Unknown winsock error.....\n"
			; break;
	}
}

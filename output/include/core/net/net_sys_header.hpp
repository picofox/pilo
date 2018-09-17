#pragma once

#include  "../coredefs.hpp"


#if defined(WIN32) || defined(WINDOWS)
#include <WinSock2.h>
#include <Mswsock.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

#else
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
//extern int h_errno;
#endif

enum EnumErrorPacketReason
{
    enPacketInvalidHeader = -1,
};

#define MF_MAKE_IPV4_ADDRESS(a,b,c,d) ((a<<24)|(b<<16)|(c<<8)|d)


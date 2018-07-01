/*******************************************************************************

* portdefines.h－ csc2100移植unix到windows线程相关映射宏定义 
           
			      包括如下内容:
				pthread_mutexattr_init
				pthread_mutexattr_destroy
				pthread_mutex_init
					
				pthread_mutex_lock					
				pthread_mutex_trylock				
				pthread_mutex_unlock				
				pthread_mutex_destroy              
				         
				pthread_condattr_init		 
				pthread_condattr_destroy		
				
				#pthread_cond_init  //使用时注意:WINDOWS平台下第二个参数为一个字符串标识常量		
				pthread_cond_destroy				
				pthread_cond_signal					
				pthread_cond_wait
				
				pthread_create	 //线程体函数声明 THREAD_PROC
				pthread_kill  
				
				pause
				sleep
	//其它
	                        			
				is_pthread_create_ok//断定线程创建是否成功
				strcasecmp
				strcasencmp
						
            
				  
* CopyRight (C) 1994-2005   All rights reserved.

* 北京四方继保自动化有限公司 / 研究所 / 自动化室

* 历史记录：

*	  2005-10-17 , 高卓，由于关键段不能跨越进程边界，修改关键代码段为互斥对象。

*         2005-07-20 ，邓俊波 ，移植第一稿成型
*********************************************************************************/
#ifndef PORT_DEF_H
#define PORT_DEF_H

#if !defined(WINDOWS_ENV)&&!defined(UNIX_ENV)
#error "WINDOWS_ENV or UNIX_ENV macro is required!"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>

#ifdef UNIX_ENV
#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif //UNIX_ENV

#ifdef WINDOWS_ENV

#pragma warning(disable:4996) 
#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS
//提示sprintf不安全之类的错误


#include "AfxMT.h"
#include <time.h>
#include <winsock2.h>
#include <ws2tcpip.h>
typedef u_long in_addr_t;
typedef int pid_t;
#include <process.h>
#endif//WINDOWS_ENV


#ifdef WINDOWS_ENV
#define    THREAD_PROC void //DWORD WINAPI

#define pthread_rwlock_t								DWORD

#define pthread_mutexattr_t								LPVOID
#define pthread_mutexattr_init(ptr_mutex_attr)			NULL
#define pthread_mutexattr_destroy(ptr_mutex_attr)		NULL

#define pthread_mutex_t									CRITICAL_SECTION					
#define pthread_mutex_init(ptr_mutex,ptr_mutex_attr)	InitializeCriticalSection(ptr_mutex)
#define pthread_mutex_lock(ptr_mutex)					EnterCriticalSection(ptr_mutex)
#define pthread_mutex_trylock(ptr_mutex)				TryEnterCriticalSection(ptr_mutex)
#define pthread_mutex_unlock(ptr_mutex)					LeaveCriticalSection(ptr_mutex)
#define pthread_mutex_destroy(ptr_mutex)                DeleteCriticalSection(ptr_mutex)

#define pthread_condattr_t                              LPVOID    
#define pthread_condattr_init(ptr_cond_attr)			NULL 
#define pthread_condattr_destroy(ptr_cond_attr)			NULL

#define pthread_cond_t									HANDLE
#define pthread_cond_init(ptr_cond,ptr_cond_attr)		*ptr_cond=CreateEvent(NULL,TRUE,FALSE,ptr_cond_attr)		
#define pthread_cond_destroy(ptr_cond)					CloseHandle(*ptr_cond)
#define pthread_cond_signal(ptr_cond)					PulseEvent(*ptr_cond)
//#define pthread_cond_broadcast(ptr_cond)
#define pthread_cond_wait(ptr_cond,ptr_mutex)		    { LeaveCriticalSection(ptr_mutex);\
														 WaitForSingleObject(*ptr_cond,INFINITE);\
														 EnterCriticalSection(ptr_mutex);}
														
//#define pthread_cond_timedwait(ptr_cond, ptr_mutex, const struct timespec * abstime);

#define pthread_attr_t									LPVOID
#define pthread_attr_init(ptr_attr)						NULL
#define pthread_attr_destroy(ptr_attr)					NULL

#define pthread_t										DWORD
//unix :pthread_create  return 0      or [EINVAL] [EAGAIN]  
//win32:CreateThread    return HANDLE or NULL
#define pthread_create(ptr_thread_id,ptr_attr,ptr_function,ptr_arg)	(*ptr_thread_id=_beginthread(ptr_function,0,ptr_arg))
																	


//	CreateThread(NULL,0,ptr_function,ptr_arg,0,ptr_thread_id);					  
#define pthread_kill(ptr_thread_id,exit_code)			 NULL
#define is_pthread_create_ok(ptr_create)                (-1!=ptr_create)  
#define pthread_exit(NULL)								(NULL)

#define sleep(A)										Sleep(A*1000) 
#define pause()                                         WaitMessage()

#define strcasecmp(a,b)		_stricmp(a,b)
#define strncasecmp(a,b,c)	strncmp(a,b,c)

#define getsockopt(p1,p2,p3,p4,p5) getsockopt(p1,p2,p3,(char*)p4,p5) 
#define setsockopt(p1,p2,p3,p4,p5) setsockopt(p1,p2,p3,(char*)p4,p5) 
//tjs 2015.04.16
//#define EINPROGRESS             WSAEINPROGRESS
//#define EALREADY                WSAEALREADY
//#define ENOTSOCK                WSAENOTSOCK
//#define EDESTADDRREQ            WSAEDESTADDRREQ
//#define EMSGSIZE                WSAEMSGSIZE
//#define EPROTOTYPE              WSAEPROTOTYPE
//#define ENOPROTOOPT             WSAENOPROTOOPT
//#define EPROTONOSUPPORT         WSAEPROTONOSUPPORT
//#define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT
//#define EOPNOTSUPP              WSAEOPNOTSUPP
//#define EPFNOSUPPORT            WSAEPFNOSUPPORT
//#define EAFNOSUPPORT            WSAEAFNOSUPPORT
//#define EADDRINUSE              WSAEADDRINUSE
//#define EADDRNOTAVAIL           WSAEADDRNOTAVAIL
//#define ENETDOWN                WSAENETDOWN
//#define ENETUNREACH             WSAENETUNREACH
//#define ENETRESET               WSAENETRESET
//#define ECONNABORTED            WSAECONNABORTED
//#define ECONNRESET              WSAECONNRESET
//#define ENOBUFS                 WSAENOBUFS
//#define ETIMEDOUT               WSAETIMEDOUT
//#define EISCONN					WSAEISCONN

#endif //WINDOWS_ENV


#ifdef UNIX_ENV
#define    THREAD_PROC void*

#define is_pthread_create_ok(ptr_create)                (0==ptr_create)
#endif //UNIX_ENV

#endif  //PORT_DEF_H
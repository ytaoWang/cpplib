/**
 * @file   event.h
 * @author wyt <wyt@wyt-laptop>
 * @date   Sat Feb  4 20:42:07 2012
 * 
 * @brief  event definition
 * @email:datao0907@163.com
 * 
 */


#ifndef __EVENT_H__
#define __EVENT_H__

#include <sys/epoll.h>

/// TIMER event definition once
const unsigned int EVENT_TIMER_ONCE = 0x00000001;
/// TIMER event again
const unsigned int EVENT_TIMER_AGAIN = 0x00000010;
/// TIMER event delete
const unsigned int EVENT_TIMER_DEL = 0x00001000;

/// ALWAYS event
const unsigned int EVENT_ALWAYS = 0x00000100;
const unsigned int EVENT_ALWAYS_DEL = 0x00010000;


/**
 * Epoll built-in supported event
 **/

/// Epoll read/receive data
const unsigned int EVENT_READ = EPOLLIN;
/// Epoll write/send data
const unsigned int EVENT_WRITE = EPOLLOUT;

/// Epoll modify read/write event
const unsigned int EVENT_CHANGE = EPOLL_CTL_MOD;

/// Epoll add read/write event
const unsigned int EVENT_ADD = EPOLL_CTL_ADD;
/// Epoll del read/write event
const unsigned int EVENT_DEL = EPOLL_CTL_DEL;



#endif

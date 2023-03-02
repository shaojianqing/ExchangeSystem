#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/epoll.h>

#include "../constant/constants.h"
#include "../common/commonType.h"

#include "event.h"

static EpollState* createEpollState();

static bool registeEpollEvent(EventLoop *loop, int fd, int mask);

static bool unregisteEpollEvent(EventLoop *eventLoop, int fd, int delMask);

static void processNetworkEvent(EventLoop *eventLoop);

EventLoop* createEventLoop() {
	EventLoop *eventLoop = (EventLoop *)malloc(sizeof(EventLoop));
	if (eventLoop!=NULL) {
		EpollState *epollState = createEpollState();
		if (epollState!=NULL) {
			eventLoop->epollState = epollState;
			return eventLoop;
		}
	}
	return NULL;	
}

static EpollState* createEpollState() {
	EpollState *state = (EpollState *)malloc(sizeof(EpollState));
	if (state!=NULL) {
		state->epollFd = epoll_create(1024);
		if (state->epollFd>=0) {
			return state;
		}
	}
	return NULL;
}

void executeEventLoop(EventLoop *eventLoop) {
	if (eventLoop!=NULL) {
		while (true) {
			processNetworkEvent(eventLoop);
		}
	}
}

void createNetworkEvent(EventLoop *eventLoop, int fd, int mask, NetworkProcessor processor, Object *client) {
	if (eventLoop!=NULL && fd<EVENT_SIZE) {
		if (registeEpollEvent(eventLoop, fd, mask)) {
			NetworkEvent *networkEvent = &(eventLoop->networkEventList[fd]);
			networkEvent->mask |= mask;
			networkEvent->client = client;
			if (mask == EVENT_READ_ABLE) {
				networkEvent->readProcessor = processor;
			}
			if (mask == EVENT_WRITE_ABLE) {
				networkEvent->writeProcessor = processor;
			}
		}
	}
}

void deleteNetworkEvent(EventLoop *eventLoop, int fd, int mask) {
	if (eventLoop!=NULL && fd>0 && fd<EVENT_SIZE && mask!=EVENT_NONE) {
		NetworkEvent *networkEvent = &(eventLoop->networkEventList[fd]);
		networkEvent->mask = networkEvent->mask & (~mask);
		unregisteEpollEvent(eventLoop, fd, mask);
	}
}

static bool registeEpollEvent(EventLoop *eventLoop, int fd, int mask) {
	if (eventLoop!=NULL) {
		EpollState *epollState = eventLoop->epollState;
		EpollEvent epollEvent;

		NetworkEvent networkEvent = eventLoop->networkEventList[fd];
		int operator = (networkEvent.mask==EVENT_NONE?EPOLL_CTL_ADD:EPOLL_CTL_MOD);
		epollEvent.events = 0;
		epollEvent.data.fd = fd;

		mask |= networkEvent.mask;
		if (mask & EVENT_READ_ABLE) {
			epollEvent.events |= EPOLLIN;
		}
		if (mask & EVENT_WRITE_ABLE) {
			epollEvent.events |= EPOLLOUT;
		}
		if (epoll_ctl(epollState->epollFd, operator, fd, &epollEvent)==-1) {
			return false;		
		} else {
			return true;		
		}
	}
	return false;
}

static bool unregisteEpollEvent(EventLoop *eventLoop, int fd, int delMask) {
	EpollState *epollState = eventLoop->epollState;
	EpollEvent epollEvent;

	NetworkEvent networkEvent = eventLoop->networkEventList[fd];
	int operator = (networkEvent.mask==EVENT_NONE?EPOLL_CTL_DEL:EPOLL_CTL_MOD);
	int mask = networkEvent.mask & (~delMask);
	
	epollEvent.events = 0;
	epollEvent.data.fd = fd;
    if (mask & EVENT_READ_ABLE) {
		 epollEvent.events |= EPOLLIN;
	}
    if (mask & EVENT_WRITE_ABLE) {
		 epollEvent.events |= EPOLLOUT;
	}

	epoll_ctl(epollState->epollFd, operator, fd, &epollEvent);	
}

static int processEpollEvent(EventLoop *eventLoop) {
	if (eventLoop!=NULL) {
		EpollState *epollState = eventLoop->epollState;
		int eventNum = epoll_wait(epollState->epollFd, epollState->epollEvents, MAX_EVENT_NUM, 0);
		if (eventNum>0) {
			int i = 0, mask = 0;
			for (i=0;i<eventNum;++i) {
				EpollEvent event = epollState->epollEvents[i];
				if (event.events & EPOLLIN) {
					mask |= EVENT_READ_ABLE;
				}
				if (event.events & EPOLLOUT) {
					mask |= EVENT_WRITE_ABLE;				
				}
				eventLoop->fireEventList[i].fd = event.data.fd;
				eventLoop->fireEventList[i].mask = mask;
			}	
		}
		return eventNum;
	}
	return 0;
}

static void processNetworkEvent(EventLoop *eventLoop) {
	if (eventLoop!=NULL) {
		int eventNum = processEpollEvent(eventLoop);
		if (eventNum>0) {
			int i = 0, mask = 0;
			for (i=0;i<eventNum;++i) {
				FireEvent fireEvent = eventLoop->fireEventList[i];
				NetworkEvent networkEvent = eventLoop->networkEventList[fireEvent.fd];
				if (networkEvent.mask & fireEvent.mask & EVENT_READ_ABLE) {
					networkEvent.readProcessor(eventLoop, fireEvent.fd, fireEvent.mask, networkEvent.client);
				}

				if (networkEvent.mask & fireEvent.mask & EVENT_WRITE_ABLE) {
					networkEvent.writeProcessor(eventLoop, fireEvent.fd, fireEvent.mask, networkEvent.client);
				} 
			}
		}
	}
}
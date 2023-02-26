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

static void processFileEvent(EventLoop *eventLoop);

static void processAllEvents(EventLoop *eventLoop);

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
			processAllEvents(eventLoop);
		}
	}
}

void createFileEvent(EventLoop *eventLoop, int fd, int mask, NetworkProcessor processor, Object *client) {
	if (eventLoop!=NULL && fd<EVENT_SIZE) {
		NetworkEvent *fileEvent = &(eventLoop->networkEventList[fd]);
		if (registeEpollEvent(eventLoop, fd, mask)) {
			fileEvent->mask |= mask;
			fileEvent->client = client;
			if (mask & EVENT_READ_ABLE) {
				fileEvent->readProcessor = processor;
			}
			if (mask & EVENT_WRITE_ABLE) {
				fileEvent->writeProcessor = processor;
			}
		}		
	}
}

void deleteFileEvent(EventLoop *eventLoop, int fd, int mask) {
	if (eventLoop!=NULL && fd>0 && fd<EVENT_SIZE && mask!=EVENT_NONE) {
		NetworkEvent *fileEvent = &(eventLoop->networkEventList[fd]);
		fileEvent->mask = fileEvent->mask & (~mask);
		unregisteEpollEvent(eventLoop, fd, mask);
	}
}

static bool registeEpollEvent(EventLoop *eventLoop, int fd, int mask) {
	if (eventLoop!=NULL) {
		EpollState *epollState = eventLoop->epollState;
		EpollEvent epollEvent;

		NetworkEvent fileEvent = eventLoop->networkEventList[fd];
		int operator = (fileEvent.mask==EVENT_NONE?EPOLL_CTL_ADD:EPOLL_CTL_MOD);
		epollEvent.events = 0;
		epollEvent.data.fd = fd;

		mask |= fileEvent.mask;
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

	NetworkEvent fileEvent = eventLoop->networkEventList[fd];
	int operator = (fileEvent.mask==EVENT_NONE?EPOLL_CTL_DEL:EPOLL_CTL_MOD);
	int mask = fileEvent.mask & (~delMask);
	
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

static void processAllEvents(EventLoop *eventLoop) {
	processFileEvent(eventLoop);
}

static void processFileEvent(EventLoop *eventLoop) {
	if (eventLoop!=NULL) {
		int eventNum = processEpollEvent(eventLoop);
		if (eventNum>0) {
			int i = 0, mask = 0;
			for (i=0;i<eventNum;++i) {
				FireEvent fireEvent = eventLoop->fireEventList[i];
				NetworkEvent fileEvent = eventLoop->networkEventList[fireEvent.fd];
				if (fileEvent.mask & fireEvent.mask & EVENT_READ_ABLE) {
					fileEvent.readProcessor(eventLoop, fireEvent.fd, fireEvent.mask, fileEvent.client);
				}

				if (fileEvent.mask & fireEvent.mask & EVENT_WRITE_ABLE) {
					fileEvent.writeProcessor(eventLoop, fireEvent.fd, fireEvent.mask, fileEvent.client);
				} 
			}
		}
	}
}
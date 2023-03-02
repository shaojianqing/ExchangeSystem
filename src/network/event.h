
#define EVENT_SIZE					1024

#define MAX_EVENT_NUM				1024

#define EVENT_NONE					0
#define EVENT_READ_ABLE				1
#define EVENT_WRITE_ABLE			2

typedef struct EventLoop EventLoop;

typedef struct NetworkEvent NetworkEvent;

typedef struct FireEvent FireEvent;

typedef struct epoll_event EpollEvent;

typedef struct EpollState EpollState;

typedef void (*NetworkProcessor)(EventLoop *eventLoop, int fd, int mask, Object *client);

struct NetworkEvent {

	int fd;

	int mask;

	Object *client;

	NetworkProcessor readProcessor;
	
	NetworkProcessor writeProcessor;
};  

struct FireEvent {

	int fd;

	int mask;
};

struct EpollState {

	int epollFd;

	EpollEvent epollEvents[EVENT_SIZE];
};

struct EventLoop {

	EpollState *epollState;
	
	NetworkEvent networkEventList[EVENT_SIZE];

	FireEvent fireEventList[EVENT_SIZE];
};

EventLoop *createEventLoop();

void executeEventLoop(EventLoop *eventLoop);

void createNetworkEvent(EventLoop *eventLoop, int fd, int mask, NetworkProcessor processor, Object *client);

void deleteNetworkEvent(EventLoop *eventLoop, int fd, int mask);
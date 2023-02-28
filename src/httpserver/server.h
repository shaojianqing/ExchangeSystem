
#define MAX_FILTER_COUNT 16

typedef struct EventLoop EventLoop;

typedef struct epoll_event EpollEvent;

typedef struct HttpServer HttpServer;

typedef struct HttpRequest HttpRequest;

typedef struct HttpResponse HttpResponse;

typedef void (*HttpFilter)(HttpRequest *request, HttpResponse *response);

typedef void (*HttpProcessor)(HttpRequest *request, HttpResponse *response);

struct HttpRequest {

    char* method;
};

struct HttpResponse {

    u16 statusCode;

    char* contentType;

    char* response;
};

struct HttpServer {

    u16 serverPort;

    u16 filterCount;

    int serverSocketFd;

    EventLoop *eventLoop;

    HttpProcessor processor;

    HttpFilter filters[MAX_FILTER_COUNT];

    bool (*addHttpFilter)(HttpServer *this, HttpFilter filter);

    bool (*setHttpProcessor)(HttpServer *this, HttpProcessor processor);

    bool (*start)(HttpServer *this);
};

HttpServer* initHttpServer(u16 port);

typedef struct HttpRequest HttpRequest;

typedef struct HttpResponse HttpResponse;

typedef struct HttpServer HttpServer;

typedef void (*Filter)(HttpRequest *request, HttpResponse *response);

typedef void (*Processor)(HttpRequest *request, HttpResponse *response);

struct HttpRequest {

};

struct HttpResponse {

};

struct HttpServer {

    int (*start)(HttpServer *this);

};

void initHttpServer();

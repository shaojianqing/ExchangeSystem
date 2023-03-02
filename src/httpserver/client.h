typedef struct HttpClient HttpClient;

typedef struct HttpServer HttpServer;

typedef void (*Reply)(HttpServer *server, HttpClient *client, String *info);

struct HttpClient {

	int clientFd;

	String *requestBody;

	String *responseBody;

	Reply reply;
};

HttpClient *createClient(int clientFd);

void acceptConnHandler(EventLoop *eventLoop, int fd, int mask, Object *client);

void processClientRequest(EventLoop *eventLoop, int fd, int mask, Object *client);
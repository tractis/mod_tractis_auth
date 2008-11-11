#include <curl/curl.h>

typedef struct NetworkClient{
	CURL *curl;
	struct MemoryStruct *chunk;	
}NetworkClient;

typedef struct PostResult{
	int code;
	char* result;
	struct NetworkClient *client;
}PostResult;

void close_network_client(struct NetworkClient* client);
struct NetworkClient *initialize_network_client(char* url);
struct PostResult* perform_post(char* username, char*pass,char* post_content, char* url, struct  NetworkClient* client);


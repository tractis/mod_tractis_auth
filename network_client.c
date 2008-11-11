#include "inmemory-response-manager.c"

void close_network_client(struct NetworkClient* client){
	if (client != NULL){
		if (client->chunk){
			if (client->chunk->memory){
				free(client->chunk->memory);
				log_debug("Freeing memory buffer");
			}
			free(client->chunk);
			log_debug("Freeing chunk");
		}
		free(client);
		log_debug("Freeing client");		
	}
}

struct PostResult* perform_post(char* username, char*pass,char* post_content, char* url, struct  NetworkClient* client){

	if (client == NULL) {
		client = (struct NetworkClient*)initialize_network_client(url);	
	}

	char* credentials;
	//If credentials are provided they're used
	if (username != NULL && pass!= NULL){
		credentials = malloc(sizeof(char) * strlen(username) + strlen(pass) + 1);
		sprintf(credentials,"%s:%s", username, pass);
		log_debug(credentials);
		curl_easy_setopt(client->curl, CURLOPT_USERPWD , credentials);
	}	

	curl_easy_setopt(client->curl, CURLOPT_POSTFIELDS, post_content);

	CURLcode res;
	res = curl_easy_perform(client->curl);

	//Be clean
	if (credentials) free(credentials);
        curl_easy_cleanup(client->curl);	
	
	int return_code;
	struct PostResult *result = malloc(sizeof(struct PostResult));
	memset (result, 0, sizeof (struct PostResult));
	result->client = client;	

	//If an error happened this error is returned
	if (res) {
		log_error(res, "HTTP request returned with error");
		result->code = SERVER_ERROR;	
	}
	else {
		result->code = 0;
		result->result = client->chunk->memory;
	}
	
	return result;
}

struct NetworkClient *initialize_network_client(char* url){

	CURL *curl = curl_easy_init();

	if (curl){
		 struct MemoryStruct *chunk = malloc(sizeof(struct MemoryStruct));
  	         memset (chunk, 0, sizeof (struct MemoryStruct));

 		 chunk->memory=NULL; /* we expect realloc(NULL, size) to work */
		 chunk->size = 0;    /* no data at this point */	 

		 struct NetworkClient *client= malloc(sizeof(struct NetworkClient));;
  	         memset (client, 0, sizeof (struct NetworkClient));
		 client->chunk = chunk;
		 client->curl = curl;	

		 //Configure a callback that will store the result in memory instead of writing a file
                 curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
                 curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)client->chunk);

		 curl_easy_setopt(curl, CURLOPT_URL, url);

		return client;
	}else{
		//No init could be performed
		return NULL;
	}
}

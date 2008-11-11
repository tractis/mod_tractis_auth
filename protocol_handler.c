#include "protocol_handler.h"
#include "string_manager.h"
#include "string_manager.c"
#include "network_client.h"
#include "network_client.c"
#include "result_parser.h"
#include "result_parser.c"
#include "protocol_result_analizer.c"

//Performs a certificate validation process
int validate_certificate(request_rec* r, char* certificate, char* user, char* password, char* api_key){
	 //Default username and pass are used
	
	 char* url=malloc(sizeof(char) * (strlen(TRACTIS_URL) + strlen(api_key) + 10));
	 sprintf(url,"%s?api_key=%s",TRACTIS_URL,api_key);
	 struct PostResult* post_result = perform_post(user, password, certificate, url, NULL);
	 
         //Clean the url
	 free(url);

	 int return_code;

	 //Something failed
	 if (post_result->code != 0){
	 	return_code = SERVER_ERROR;
		log_error(return_code, "Server returned error code");
	 }else{
	 	return_code = check_validation_result(post_result->result);
		if (return_code == VALID){
			recover_attributes(r, post_result->result);
		}
	 }

	 if (is_debug_enabled()) log_debug("Closing channel");
	 close_network_client(post_result->client);
	 free(post_result);
	
	 return return_code;
}

char* get_tag_text(char* result, char* tag_open, char *tag_close);

void recover_attributes(request_rec * r,char* result){
	//recover the attribute identifiers and values (NULL terminated array)
	struct CertificateAttribute** attributes = (struct CertificateAttribute**)get_certificate_attributes(result); 

	struct CertificateAttribute* current;
	int index = 0;
	while((current = attributes[index])!= NULL){
		if (is_debug_enabled()){
			char message[256];
			sprintf(message, "Recovering attribute with id: %s and value:%s", current->attribute_name, current->attribute_value);	

			log_debug(message);	
		}
		
		apr_table_add(r->subprocess_env, current->attribute_name, current->attribute_value);
		
		index++;
	}
	
        cleanAttributes(attributes);	
}






const char* TRACTIS_URL = "https://www.tractis.com/certificate_verification";
const char* resultStartTag = "<validationStatus>";
const char* resultEndTag = "</validationStatus>";

char* get_tag_text(char* result, char* tag_open, char *tag_close);

int check_validation_result(char* result){

  if (is_debug_enabled()){
  	log_debug("Received result");
	log_debug(result);
  }

  char* res = get_tag_text(result,(char*) resultStartTag,(char*) resultEndTag);  
  if (res == NULL) {
	log_error(-1, "Server reported invalid response \n");
	log_debug(result);
  	return SERVER_ERROR;	
  }

  if (is_debug_enabled()){
  	log_debug("Validation result is");	
	log_debug(res);	
	log_debug("\n");
   } 

  int result_code;
  if (res == NULL){
  	log_error(-1, "Invalid message format");
	result_code = SERVER_ERROR;
  }else{
  	

	//Result analisys
	if (strcmp(res,"VALID")==0){
		result_code = VALID;
	}else if (strcmp(res,"INVALID")==0){
		result_code = INVALID;
	}else if (strcmp(res,"UNKNOWN")==0){
		result_code = UNKNOWN;
	}else{
		result_code = SERVER_ERROR;
	}
  }

  free(res);
  return result_code;

}

//Returns the text value of xml tag
char* get_tag_text(char* result, char* tag_open, char *tag_close){
	int startToken = find_token_at_string(result, tag_open) + strlen(tag_open);

	int endToken;
  	if (startToken >= 0) endToken = find_token_at_string_from(result, tag_close, startToken);
	else return NULL;
	
	if (startToken < 0 || endToken < 0){
		log_error(-1,"Cannot find requested tokens on string");
		return NULL;
  	}

	size_t size = endToken - startToken;
	char *res = malloc(size); 
	substring(result, res, startToken, size);

	return res;
}	


void log_error(int code, char* message){
	fprintf(stderr,"ERROR code:%d, \n message %s\n",code,message);
	fflush(stderr);
}

void log_info(char* message){
	if (LOG_LEVEL >= LOG_ON_INFO) {
		fprintf(stderr, "INFO: %s\n",message);
		fflush(stderr);
	}
}

void log_debug(char* message){
	if (LOG_LEVEL >= LOG_ON_INFO) {
		fprintf(stderr, "DEBUG: %s\n",message);
		fflush(stderr);
	}
}

int is_debug_enabled(){
	return LOG_LEVEL >= LOG_ON_DEBUG;
}
int is_info_enabled(){
	return LOG_LEVEL >= LOG_ON_INFO;	
}




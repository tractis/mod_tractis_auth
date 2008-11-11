//The log levels
#define LOG_ON_ERROR  0
#define LOG_ON_INFO  1
#define LOG_ON_DEBUG 2

#define MESSAGE_LEN 256

//Define on compilation time in order to allow better debugging options
#ifndef LOG_LEVEL
	const int LOG_LEVEL = LOG_ON_ERROR;
#endif


void log_error(int code, char* message);
void log_info(char* message);
void log_debug(char* message);

int is_debug_enabled();
int is_info_enabled();

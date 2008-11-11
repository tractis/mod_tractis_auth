#include <httpd.h>
#include <http_protocol.h>
#include <http_config.h>
#include <stdio.h>

//#define PROFILE 1

#ifdef PROFILE
	#include <mcheck.h> 
#endif

//Holds the data required to connect to the service
struct tractis_auth_module_config{
	char* username;
	char* pass;
	char* api_key;	
	char* enabled;
};



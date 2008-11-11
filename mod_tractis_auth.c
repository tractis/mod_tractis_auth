#include "mod_tractis_auth.h"
#include "constants.h"
#include "logger.h"
#include "logger.c"
#include "protocol_handler.h"
#include "protocol_handler.c"


//First module definition in order to be used by the following methods refering it
module AP_MODULE_DECLARE_DATA mod_tractis_auth;

//Recovers the username from the config
static char* get_username(request_rec* r){
	struct tractis_auth_module_config *s_cfg = ap_get_module_config(r->server->module_config, &mod_tractis_auth);
	return s_cfg->username;
}

//Recovers the password from the config
static char* get_password(request_rec* r){
	struct tractis_auth_module_config *s_cfg = ap_get_module_config(r->server->module_config, &mod_tractis_auth);
	return s_cfg->pass;
}

//Recovers the api_key identifier from the config
static char* get_api_key(request_rec* r){
	struct tractis_auth_module_config *s_cfg = ap_get_module_config(r->server->module_config, &mod_tractis_auth);
	return s_cfg->api_key;
}

//Recovers if the module is enabled or not
static char* is_enabled(request_rec* r){
	struct tractis_auth_module_config *s_cfg = ap_get_module_config(r->server->module_config, &mod_tractis_auth);
	return s_cfg->enabled;
}


static int process_request(request_rec* r){

	//Used on memory profiling tests
   	#ifdef PROFILE 
		mtrace();
	#endif

	log_debug("Starting Tractis auth module process");

    char *enabled = is_enabled(r);
	//Not enabled
	if (enabled == NULL || strcmp(enabled,"true") !=0){
		char buff[MESSAGE_LEN];
		sprintf(buff,"Module not enabled for given url");
		log_debug(buff);
		return DECLINED;
	}

	//Recover credentials
	char *user = get_username(r);
	char *password = get_password(r);
	char *api_key = get_api_key(r);
	

	if (is_debug_enabled()){
		char debug_message[MESSAGE_LEN];
		sprintf(debug_message, "Credentials are %s:%s and api key is %s", user,password,api_key);
		log_debug(debug_message);
	}

	char* certificate = (char*)apr_table_get(r->subprocess_env, "SSL_CLIENT_CERT");
	
 	//The certificate is not present so the client is not allowed to use the service
	if (!certificate) {
		char error_message[MESSAGE_LEN];
		log_error(401, "No certificate has been provided");
		return 401; //HTTP unauthorized	
	}else{
		if (is_debug_enabled()){
			char message[MESSAGE_LEN + strlen(certificate)];
			sprintf(message, "User provided certificate \n->%s<-",certificate);
			log_debug(message);
		}
	}

	int certificate_status = validate_certificate(r,certificate, user, password, api_key);

	#ifdef PROFILE 
		mtrace();
	#endif	

	if (certificate_status == VALID) return OK;
	else {
		if(is_info_enabled()){
			char validation_result[MESSAGE_LEN];			
			sprintf(validation_result, "The provided certificate reported unsuccessful validation, result code is %d", certificate_status);
			log_info(validation_result);
		}
		//Non valid auth		
		return 401;	
	}
}

//Handles the authentication request
static int handle_authentication_request(ap_filter_t* f, apr_bucket_brigade* bb, ap_input_mode_t mode, apr_read_type_e block, apr_off_t readbytes) {
	request_rec* r = f->r;
	process_request(r);
}

//register the hooks at the required level. Using ap_hooks_fixups this hook will be called inmediatelly before staring dynamic code generation process
static void register_hooks(apr_pool_t* pool)
{
	log_debug("Starting Tractis authentication module!");
	ap_hook_fixups(process_request, NULL, NULL, APR_HOOK_MIDDLE);
	log_debug("Tractis authentication module started!");
}

//Creates the service config struct, not performs filling but could be used to provide default parameters
static void *create_mod_config(apr_pool_t *p, server_rec *s)
{  
  struct tractis_auth_module_config *newcfg;
  newcfg = (struct tractis_auth_module_config *) apr_pcalloc(p, sizeof(struct tractis_auth_module_config));
  return (void *) newcfg;
}

static const char *set_tractis_username(cmd_parms *parms, void *mconfig, const char *arg)
{	struct tractis_auth_module_config *s_cfg = ap_get_module_config(parms->server->module_config, &mod_tractis_auth);
	s_cfg->username = (char *) arg;	
	return NULL;
}

static const char *set_tractis_password(cmd_parms *parms, void *mconfig, const char *arg)
{	struct tractis_auth_module_config *s_cfg = ap_get_module_config(parms->server->module_config, &mod_tractis_auth);
	s_cfg->pass = (char *) arg;	
	return NULL;
}

static const char *set_tractis_api_key(cmd_parms *parms, void *mconfig, const char *arg)
{	struct tractis_auth_module_config *s_cfg = ap_get_module_config(parms->server->module_config, &mod_tractis_auth);
	s_cfg->api_key = (char *) arg;	
	return NULL;
}

static const char *set_enabled(cmd_parms *parms, void *mconfig, const char *arg)
{	struct tractis_auth_module_config *s_cfg = ap_get_module_config(parms->server->module_config, &mod_tractis_auth);
	s_cfg->enabled = (char *) arg;	
	return NULL;
}

//Tha available config commands
static const command_rec mod_commands[] =
{
  AP_INIT_TAKE1("TractisUsername", set_tractis_username, NULL, ACCESS_CONF,"TractisUsername (string) The full username of the tractis account."),
  AP_INIT_TAKE1("TractisPassword", set_tractis_password, NULL, ACCESS_CONF,"TractisPassword (string) The password of the tractis account."),
  AP_INIT_TAKE1("TractisAPIKey", set_tractis_api_key, NULL, ACCESS_CONF,"TractisAPIKey (string) The configured and active api key holding the configured service"),
AP_INIT_TAKE1("TractisEnabled", set_enabled, NULL, ACCESS_CONF,"Defines if the module is enabled for the given context or not, default is not"),
  {NULL}
};


module AP_MODULE_DECLARE_DATA mod_tractis_auth = {
	STANDARD20_MODULE_STUFF,
	NULL,
	NULL,
	create_mod_config, //Config
	NULL,
	mod_commands, //Commands
	register_hooks
};

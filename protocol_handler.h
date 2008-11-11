#include <httpd.h>
#include <http_protocol.h>
#include <http_config.h>
#include "constants.h"

int validate_certificate(request_rec* r, char* certificate, char* user, char* password, char* api_key);
void recover_attributes(request_rec * r,char* result);

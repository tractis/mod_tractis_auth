
struct CertificateAttribute** get_certificate_attributes(char *message){
	if (message == NULL) return NULL;
	int index = 0;
	
	char *id = "id";
	char *attribute_tag = "<attribute";
	char *attribute_closing_tag = "</attribute>";

	if (is_debug_enabled()){
		log_debug("Server returned message:");
		log_debug(message);
	}

	int current_index = 0;

	//Attribute counting
	int attribute_count = 0;
	while((current_index = (find_token_at_string_from(message,  attribute_tag, current_index))+1)>0){
		attribute_count++;	
	}

	if(is_debug_enabled()){
		char message[MESSAGE_LEN];
		sprintf(message, "Found %d attributes", attribute_count);
		log_debug(message);
	}	

	//Allocating the array
	struct CertificateAttribute** result = malloc(attribute_count * sizeof(struct CertificateAttribute*) + 1);

	if (attribute_count > 0){
	int current_attribute_index = 0;

		//Finding the next attributute
		while((current_index = find_token_at_string_from(message,  attribute_tag, current_index))>=0){
			//find the id, the +2 means adding =' to seek to the name start
			int id_start_pos = find_token_at_string_from(message,  id, current_index) + 4;
			int id_end_pos = find_token_at_string_from(message,  "'", id_start_pos);

			int len = id_end_pos - id_start_pos;
			char* attribute_name = malloc(sizeof(char) * len);
			substring(message, attribute_name,  id_start_pos, len);
			if (is_debug_enabled()) log_debug(attribute_name);

			//Now seek to the value start
			int value_start_pos = find_token_at_string_from(message,  ">", id_end_pos) + 1;
			int value_end_pos = find_token_at_string_from(message,  "<", value_start_pos);

			int value_len = value_end_pos - value_start_pos;
			char* attribute_value = malloc(sizeof(char) * value_len);
			substring(message, attribute_value,  value_start_pos, value_len);

			if (is_debug_enabled()) log_debug(attribute_value);

			struct CertificateAttribute* current_attribute = malloc(sizeof(struct CertificateAttribute));
			current_attribute->attribute_name = attribute_name;
			current_attribute->attribute_value = attribute_value;
			
			result[current_attribute_index] = current_attribute;
			current_attribute_index++;
			
			//Seek to the end of the current attribute
			current_index = find_token_at_string_from(message,  attribute_closing_tag, value_end_pos);
		}
	}
	
	//NULL terminated array
	result[attribute_count] = NULL;
	return result;
}

void cleanAttributes(struct CertificateAttribute** to_be_cleaned){
	struct CertificateAttribute* current;
	
	if (to_be_cleaned == NULL) {
		if (is_debug_enabled()) log_debug("Structure already freed");
		return;	
	}
	
	
	int i = 0;
	
	while(to_be_cleaned[i] != NULL){
		if (to_be_cleaned[i]) {
			if (to_be_cleaned[i]->attribute_name) {
				free(to_be_cleaned[i]->attribute_name); 	
				if (is_debug_enabled()) log_debug("Cleaning name");
			}
			if (to_be_cleaned[i]->attribute_value) {
				free(to_be_cleaned[i]->attribute_value); 	
				if (is_debug_enabled()) log_debug("Cleaning value");
			}
			free(to_be_cleaned[i]);
		}
		i++;
	}

	free(to_be_cleaned);
}


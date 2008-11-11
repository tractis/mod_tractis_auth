#include "string_manager.h"

void substring(char* in, char* out,  int start, int size){
	int i;
	for(i = 0 ; i < size; i++){
		out[i] = in[start + i];
	}
	out[i] = '\0';
}

int find_token_at_string(char* in,  char *pattern){
	find_token_at_string_from(in,pattern,0);
}

int find_token_at_string_from(char* in,  char *pattern, int start){
	int i = start,j = 0;

	for(; i < strlen(in); i++){
		//Start comparation
		if (in[i] == pattern[0]){
			for(j = 0; j < strlen(pattern); j++){
				if (in[i + j] != pattern[j]){
					break;
				}
			}
			//Pattern found
			if (j == strlen(pattern)){
				return i;
			}
		}
	}
	return -1;
}



struct CertificateAttribute{
	char* attribute_name;
	char* attribute_value;	
};

//Returns all the returned attributes, NULL if no attribute has been returned
struct CertificateAttribute** get_certificate_attributes(char *message);

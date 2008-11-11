//Holds inmemory the stream returned by the server
struct MemoryStruct {
 char *memory;
 size_t size;
};

static void *myrealloc(void *ptr, size_t size);

static void *myrealloc(void *ptr, size_t size)
{
   /* There might be a realloc() out there that doesn't like reallocing
      NULL pointers, so we take care of it here */
   if(ptr)
     return realloc(ptr, size);
   else
     return malloc(size);
 }
 
 static size_t
 WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
 {
   size_t realsize = size * nmemb;
   struct MemoryStruct *mem = (struct MemoryStruct *)data;
 
   mem->memory = myrealloc(mem->memory, mem->size + realsize + 1);
   if (mem->memory) {
     memcpy(&(mem->memory[mem->size]), ptr, realsize);
     mem->size += realsize;
     mem->memory[mem->size] = 0;
   }
   return realsize;
 }

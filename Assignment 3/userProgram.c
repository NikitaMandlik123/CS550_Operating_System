#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 256000 
static char receive[BUFFER_LENGTH];

int main()
{
  //variables
   int ret, fdi;

   //test code starts
   printf("The device test code begins...\n");
   fd = open("/dev/process_list", O_RDWR);
   if (fdi < 0)
   {
      perror("ERROR:Failed to open the device...");
      return errno;
   }
  //Reading
   printf("Reading initiated...\n");
   ret = read(fdi, receive, BUFFER_LENGTH);
   if (ret < 0)
   {
      perror("ERROR:Failed to read the message from the device.");
      return errno;
   }
    //The message
    printf("The obtained message: [%s]", receive);
    
    //close the file
    close(fdi);
    printf("Program Terminated\n");
    return 0;
}

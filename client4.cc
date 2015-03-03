#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sstream>
#define HOST_BUFFER 256
using namespace std;


int main(int argc, char * argv[]) // <addr> <port#> [-b]
{
   if (argc != 3 && argc != 4) {
      cerr << "Inappropriate number of arguments"  << endl;
      exit(0);
   }



   int portNumber;
   //struct sockaddr_in socketInfo;
   char host[HOST_BUFFER + 1]; // hostname of this computuer
   struct hostent *hPtr;
   struct sockaddr_in remoteSocketInfo;


   istringstream ss(argv[1]);
   if (! (htonl(ss) >> remoteSocketInfo.sin_addr.s_addr)) {
      cerr << "Invalid address: " << argv[1] << "\n";
      exit(EXIT_FAILURE);
   }
   istringstream ss2(argv[2]);
   if (! (ss2 >> portNumber)) {
      cerr << "Invalid port number: " << argv[2] << "\n";
      exit(EXIT_FAILURE);
   }


   //struct hostent *hPtr;
   int socketHandle;
   const char *remoteHost = "localhost";
   //int portNumber = 8080;

   bzero(&remoteSocketInfo, sizeof(sockaddr_in));  // Clear structure memory

   // Get system information

   if((hPtr = gethostbyname(remoteHost)) == NULL)
   {
      cerr << "System DNS name resolution not configured properly." << endl;
      cerr << "Error number: " << ECONNREFUSED << endl;
      exit(EXIT_FAILURE);
   }

   // create socket
   if((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
      close(socketHandle);
      exit(EXIT_FAILURE);
   }

   // Load system information into socket data structures

   memcpy((char *)&remoteSocketInfo.sin_addr, hPtr->h_addr, hPtr->h_length);
   remoteSocketInfo.sin_family = AF_INET;
   remoteSocketInfo.sin_port = htons((u_short)portNumber);

   if(connect(socketHandle, (struct sockaddr *)&remoteSocketInfo, sizeof(sockaddr_in)) < 0)
   {
      close(socketHandle);
      exit(EXIT_FAILURE);
   }


   int rc = 0;  // Actual number of bytes read by function read()
   char buf[512];

   strcpy(buf,"Message to send");
   send(socketHandle, buf, strlen(buf)+1, 0);


   const char * file_name = "test5354.txt";
   FILE * f = fopen(file_name, "a");
   if (f == NULL) {
      cout << "File not received" << endl;
   }
   else {
      cout << "Received: " << file_name << endl;
   }



}
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
   memset(host, '0', sizeof(host));
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

   cout << ":)" << endl;

   // create socket
   if((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
      cout << "socket failed :(" << endl;
      close(socketHandle);
      exit(EXIT_FAILURE);
   }

   cout << ":)" << endl;

   // Load system information into socket data structures

   memcpy((char *)&remoteSocketInfo.sin_addr, hPtr->h_addr, hPtr->h_length);
   remoteSocketInfo.sin_family = AF_INET;
   remoteSocketInfo.sin_port = htons((u_short)portNumber);

   if(connect(socketHandle, (struct sockaddr *)&remoteSocketInfo, sizeof(sockaddr_in)) < 0)
   {
      cout << "connect failed :(" << endl;
      close(socketHandle);
      exit(EXIT_FAILURE);
   }

   cout << ":)" << endl;


   int rc = 0;  // Actual number of bytes read by function read()
   char buf[256];

   //strcpy(buf,"Hello there");
   //send(socketHandle, buf, strlen(buf)+1, 0);

   
   // create file where data is stored
   FILE * fp = fopen("received.txt", "w");
   if (fp == NULL) {
      cout << "File not received" << endl;
      exit(1);
   }

   // receive file
   int bytesReceived;
   while ((bytesReceived = read(socketHandle, buf, 256) > 0)) { //?
      cout << "Bytes received: " << bytesReceived << endl;
      fwrite(buf, 1, bytesReceived, fp);
   }
   if(bytesReceived < 0)
    {
        printf("\n Read Error \n");
         fprintf(fp,"%s",buf);
    }

  

   else {
      cout << "Bytes received: " << bytesReceived << endl;
   cout << "the file was received successfully" << endl;
   cout << "the new file created is received.tx" << endl;
}
   
   /*
   // received data in 256 bytes
   int bytesReceived;
   while (bytesReceived == read(socketHandle, host, 256)) { //?
      cout << "Bytes received: " << bytesReceived;
      fwrite(host, 1, bytesReceived, fp);
   }
   */

   
   

}
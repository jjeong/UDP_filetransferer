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
#define BUFFER_SIZE 256
using namespace std;


int main(int argc, char * argv[]) // <addr> <port#> [-b]
{
   if (argc != 3 && argc != 4) {
      cerr << "Inappropriate number of arguments"  << endl;
      exit(0);
   }

   int portNumber;
   char host[HOST_BUFFER + 1]; // hostname of this computuer
   memset(host, '0', sizeof(host));
   struct hostent *hPtr;
   struct sockaddr_in remoteSocketInfo;
   int blink = 0;


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
   if ((argc == 4) && (strcmp(argv[3], "-b") == 0))
      blink = 1;

   int socketHandle;
   const char *remoteHost = "localhost";

   // clear structure memory
   bzero(&remoteSocketInfo, sizeof(sockaddr_in));

   // Get system information
   if((hPtr = gethostbyname(remoteHost)) == NULL) {
      cerr << "System DNS name resolution not configured properly." << endl;
      cerr << "Error number: " << ECONNREFUSED << endl;
      exit(EXIT_FAILURE);
   }

   // create socket
   if((socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
   {
      cout << "socket failed" << endl;
      close(socketHandle);
      exit(EXIT_FAILURE);
   }

   // Load system information into socket data structures
   memcpy((char *)&remoteSocketInfo.sin_addr, hPtr->h_addr, hPtr->h_length);
   remoteSocketInfo.sin_family = AF_INET;
   remoteSocketInfo.sin_port = htons((u_short)portNumber);

   /*
   if(connect(socketHandle, (struct sockaddr *)&remoteSocketInfo, sizeof(sockaddr_in)) < 0)
   {
      cout << "connect failed :(" << endl;
      close(socketHandle);
      exit(EXIT_FAILURE);
   }
   */

   // bind
        //ret_val = ;
   // if (bind(socketHandle, (struct sockaddr *)&socketInfo, sizeof(struct sockaddr_in)) != 0) {
   //    cout << "bind failure" << endl;
   //    close(socketHandle);
   //    return -1;
   // }


   //int rc = 0;  // Actual number of bytes read by function read()
   char buf[BUFFER_SIZE];

   FILE * fp = fopen("received.txt", "w");
   if (fp == NULL) {
      cout << "File not received" << endl;
      exit(EXIT_FAILURE);
   }

   int bytesReceived;
   time_t start, end;
   double diff;
   int beginning = 1;

   cout << "ok3" << endl;

   // receive data
   time(&start);
   while ((bytesReceived = recv(socketHandle, buf, BUFFER_SIZE, 0)) > 0) { //error
      
      //fwrite(buf, 1, bytesReceived, fp);
      cout << "receiving.." << endl;
      time(&end);
      diff = difftime(end,start);
      if (!beginning)
         cout << "time elapsed: " << diff << " seconds" << endl;
      beginning = 0;
      cout << "data block received: " << bytesReceived << endl;
      time(&start);
      if (blink) {
         //sleep(5);
         cout << "blink!" << endl;
      }
   }
   if(bytesReceived < 0) {
      printf("\n Read Error \n");
      fprintf(fp,"%s",buf);
   } else {
      cout << "the file was received successfully" << endl;
      cout << "the new file \"received.txt\" is created" << endl;
   }
}
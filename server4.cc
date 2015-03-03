#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sstream>
#define MAX_BUFFER 1025 //?
using namespace std;



int main(int argc, char * argv[])
{
	if (argc < 1) {
		cerr << "Port number missing in the argument" << endl;
		exit(0);
	}

	int portNumber;
	istringstream ss(argv[1]);
	if (! (ss >> portNumber))
		cerr << "Invalid port number" << argv[1] << "\n";
	

	struct sockaddr_in socketInfo;
	char host[MAX_BUFFER + 1]; // hostname of this computuer
	struct hostent *hPtr;

	int socketHandle;

	// clear struct memory
	bzero(&socketInfo, sizeof(sockaddr_in));

	//get system info
	gethostname(host, MAX_BUFFER); // name of this computer
	if ((hPtr = gethostbyname(host)) == NULL) {
		cerr << "System hostname misconfigured." << endl;
		exit(EXIT_FAILURE);
	}

	// create socket
	if ((socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0)
	{
		close(socketHandle);
		exit(EXIT_FAILURE);

	}

	// load system info into socket data
	socketInfo.sin_family = AF_INET;
	socketInfo.sin_addr.s_addr = htonl(INADDR_ANY); //use any addr available
	socketInfo.sin_port = htons(portNumber);




	//bind socket to local socket addr
	if (bind(socketHandle, (struct sockaddr *) &socketInfo, sizeof(socketInfo)) < 0) {
		close(socketHandle);
		perror("bind");
		exit(EXIT_FAILURE);
	}

	// listening mode
	listen(socketHandle, 5);

	int socketConnection;
	if ((socketConnection = accept(socketHandle, NULL, NULL)) < 0) {
		exit(EXIT_FAILURE);
	}
	close(socketHandle);

	int rc = 0; // actual number of bytes read
	char buf[512];

	// rc: # of char returned
	// note this is not typical
	rc = recv(socketConnection, buf, 512, 0);
	buf[rc] = (char) NULL; // null termination

	cout << "Number of bytes read: " << rc << endl;
	cout << "Received: " << buf << endl;

	bzero(buf, 512);

	   // create a file

    FILE * f;
    f = fopen("test5354.txt", "abc");
    if (f == NULL) {
      printf("ERROR: file %s not found\n", "test.txt");
      exit(EXIT_FAILURE);
    }


    int file_size;
    while ((file_size = fread(buf, sizeof(char), 512, f)) > 0) {
    	if (send(socketHandle, buf, file_size, 0) < 0) {
    		cerr << "ERROR: file cannot be sent" << endl;
    		break;
    	}
    	bzero(buf, 512);
    }
    printf("File sent to the client!\n");
    close(socketHandle);
   
    return 0;
}


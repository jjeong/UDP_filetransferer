#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sstream>
#define HOST_BUFFER 1025 //?
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
	char host[HOST_BUFFER + 1]; // hostname of this computuer
	struct hostent *hPtr;

	int socketHandle;

	// clear struct memory
	bzero(&socketInfo, sizeof(sockaddr_in));

	//get system info
	gethostname(host, HOST_BUFFER); // name of this computer
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

	cout << "listening..." << endl;

	// listening mode
	listen(socketHandle, 5);


	int rc = 0; // actual number of bytes read
	char buf[256];
	memset(&socketInfo, '0', sizeof(socketInfo));
    memset(buf, '0', sizeof(buf));

	// rc: # of char returned
	// note this is not typical
	
	//rc = recv(socketConnection, buf, 512, 0);
	//buf[rc] = (char) NULL; // null termination

	//cout << "Number of bytes read: " << rc << endl;
	//cout << "Received: " << buf << endl;

	
	cout << "ok" << endl;
	// works fine up to here

	
    int connfd; //?
    int done = 0;
    while (true) {
    	connfd = accept(socketHandle, (struct sockaddr *) NULL, NULL);
    	FILE * f = fopen("test1.txt", "r"); //
    	if (f == NULL) {
    		cout << "file open error" << endl;
    		exit(1);
    	}

    	cout << "ok" << endl;

    	while (true) {
    		unsigned char buffer[256] = {0};
    		int nread = fread(buffer, 1, 256, f);
    		cout << "Bytes read: " << nread << endl;

    		//send data
    		if (nread > 0) {
    			cout << "sending..." << endl;
    			write(connfd, buffer, nread);
    		}
    		// if error, or reached end of file
    		if (nread < 256) {
    			if (feof(f))
    				cout << "end of file" << endl;
    			if (ferror(f))
    				cout << "error reading" << endl;
    			done = 1;
    			break;
    		}

    	}
    	if (done)
    		break;

    	close (connfd);
    	sleep(1);
    }
    
    /*
    FILE *f;
	f = fopen("add.txt","r");
	if (f == NULL) {
    	cout << "file open error" << endl; //error
    	exit(1);
    }


	fscanf(f,"%s",buf); //segmentation error
	write(socketHandle, buf, 256);

	cout << "the file was sent successfully" << endl;
    bzero(buf, 256);

    
    close(socketHandle);
    */
   
    return 0;
}


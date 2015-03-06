#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sstream>
#include <time.h>
#define HOST_BUFFER 256
#define BUFFER_SIZE 256
#define MESSAGE "message to be sent "
using namespace std;


int main(int argc, char * argv[])
{

	//check parameter
	if (argc < 1) {
		cerr << "Port number missing in the argument" << endl;
		exit(0);
	}
	int portNumber;
	istringstream ss(argv[1]);
	if (! (ss >> portNumber))
		cerr << "Invalid port number" << argv[1] << "\n";
	
	//initialize random generator
	srand(time(NULL));

	struct sockaddr_in socketInfo;
	char host[HOST_BUFFER + 1]; // hostname of this computuer
	struct hostent *hPtr;
	int socketHandle;

	// clear struct memory
	bzero(&socketInfo, sizeof(sockaddr_in));

	//get system info
	/*
	gethostname(host, HOST_BUFFER); // name of this computer
	if ((hPtr = gethostbyname(host)) == NULL) {
		cerr << "System hostname misconfigured." << endl;
		exit(EXIT_FAILURE);
	}
	*/

	// create socket
	if ((socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		cout << "cannot create a socket" << endl;
		close(socketHandle);
		exit(EXIT_FAILURE);

	}

	// initialize socket data
	socketInfo.sin_family = AF_INET;
	socketInfo.sin_addr.s_addr = htonl(INADDR_ANY); //use any addr available
	socketInfo.sin_port = htons(portNumber);

	//loop through all the results and bind to the first we can


	//bind socket to local socket addr
	if (bind(socketHandle, (struct sockaddr *) &socketInfo, sizeof(socketInfo)) < 0) {
		close(socketHandle);
		perror("bind failure");
		exit(EXIT_FAILURE);
	}
	

	// freeaddrinfo(servinfo);

	//int rc = 0; // number of bytes read
	char buf[BUFFER_SIZE];
	memset(&socketInfo, '0', sizeof(socketInfo));
    memset(buf, '0', sizeof(buf));

	
    int connfd; //?
    int done = 0;
    char filename[10];
    strcpy(filename, "test1"); //10MB
	strcat(filename, ".txt");

	int randtime;
	int totaltime = 0;
	int blocksent = 0; // counter for data block


    while (true) {

    	//connfd = accept(socketHandle, (struct sockaddr *) NULL, NULL);
    	FILE * f = fopen(filename, "r");
    	if (f == NULL) {
    		cout << "cannot open file: " << filename << endl;
    		exit(1);
    	}
    	bzero(f, BUFFER_SIZE); 

    	cout << "trying to send " << endl;

    	//char *message = "message to send";

    	while (true) {
    		unsigned char buffer[BUFFER_SIZE] = {0};

    		//int nread = fread(buffer, 1, BUFFER_SIZE, f);
    		int nread = send(socketHandle, MESSAGE, strlen(MESSAGE) + 1, 0); 

    		cout << "Bytes read: " << nread << endl;

    		//send data
    		if (nread > 0) {
    			cout << "sending..." << endl;
    			//write(connfd, buffer, nread);
    		}


    		blocksent += 1;

    		/*
    		// error, or reached end of file
    		if (nread < BUFFER_SIZE) {

    			if (feof(f)) {
    				cout << "end of file" << endl;
    				done = 1;
    				break;
    			}
    			if (ferror(f)) {
    				cout << "ok" << endl;
    				cout << "error reading" << endl;
    				break;
    			}
    			//break;
    		}
    		*/
    		

    		// create random delay
    		randtime = rand() % 10; // 0~9
    		cout << "delaying " << randtime << " seconds" << endl;
    		sleep(randtime);
    		cout << "done! moving on " << endl;
    		totaltime += randtime;
    	}

    	//close (connfd);
    	if (done)
    		break;
    	//sleep(1);
    }

    close (socketHandle);
    
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


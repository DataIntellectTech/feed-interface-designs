#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include "k.h"
#include "blog.h"

int clientSocket;
char buffer[1024];
struct sockaddr_in serverAddr;
socklen_t addr_size;
int callbackcounter;

K callback(I d) {
	logmessage(__FUNCTION__, "Callback called");
	recv(clientSocket, buffer, 1024, 0);
	logmessage(__FUNCTION__, buffer);
	k(0, ss(callbackname), kj(callbackcounter), kp(buffer), (K) 0);
	callbackcounter += 1;
	return (K) 0;
}

//start client listening to server
K startclient(K x) {
	logmessage(__FUNCTION__, "starting to connect to server");
	int port = x -> j;
	callbackcounter = 0;
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(port);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
	addr_size = sizeof serverAddr;
	connect(clientSocket, (struct sockaddr * ) & serverAddr, addr_size);
	sd1(clientSocket, callback);
	return (K) 0;
}

//return dictionary of messages sent and received
K getstats(K x) {
	K keys;
	K values;
	K dict;
	keys = knk(2, ks((S) "sent"),ks((S) "processed"));
	values = knk(2, kj(sent), kj(callbackcounter));
	return xD(keys, values);
}

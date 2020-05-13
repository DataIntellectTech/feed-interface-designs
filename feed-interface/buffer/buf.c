#include "k.h"
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/socket.h>
#include <assert.h>

#include "cb.h"

int fd[2];
static cb queue;
long loops = 0;
int callbacks = 0;

void* pub(void* s) {

	int *h = (int *)s;
	char dot = '.';
	struct timespec tp;
	uint8_t msgbuff[1000];
	long ctr = 0;
	
	K syms = ktn(KS,3);
	
	kS(syms)[0] = ss("IBM");
	kS(syms)[1] = ss("MSFT");
	kS(syms)[2] = ss("GOOG");

	while(2) {

		loops += 1;
		ctr = 1;
		clock_gettime(CLOCK_REALTIME, &tp);

		K trade = knk(4, ktj(-KP, tp.tv_nsec + 1e9*(tp.tv_sec - 946684800)),
				ks(kS(syms)[rand()%3]),
				kf(0.01*(rand()%10000)),
				kj(rand()%1000));

		K kmsg = b9(-1,trade);

		long msize = kmsg->n;
		memcpy(msgbuff, &msize, sizeof(long));
		memcpy(msgbuff + sizeof(long), kG(kmsg), msize);

		r0(kmsg);
		cb_write(&queue, msgbuff, msize + sizeof(long), 1);
		write(*h, &dot, 1);

	}

}

uint8_t rbuf[1000000];

K callback2(I d) {

	char c;
	K msg, dmsgs;
	long qsize, msize;

	recv(d,&c,1,0);
	qsize = cb_size(&queue);

        callbacks++;

	if(qsize > 0) {

		dmsgs = ktn(0,0);
		cb_read(&queue, rbuf, qsize, 0);
		long offset = 0;

		while( offset < qsize ) {

			memcpy(&msize, rbuf + offset, sizeof(long));
			assert((msize > 0) && (msize < 500));
			msg = ktn(KG, msize);
			memcpy(kG(msg), rbuf + offset + sizeof(long), msize);
			jk(&dmsgs, d9(msg));
			r0(msg);
			offset += msize + sizeof(long);

		}

		K rtn = k(0, "f", dmsgs, (K)0);
	}

	return (K) 0;

}

void* noop(void* x) {

	while(1) usleep(1000);

}

K start(K x) {

	callbacks = 0;
	pthread_t tid;
	cb_init(&queue, 1000000);
	socketpair(AF_LOCAL, SOCK_STREAM, 0, fd);
	fcntl(fd[0], F_SETFL, O_NONBLOCK);
	sd1(fd[1], callback2);
	pthread_create(&tid, NULL, &pub, (void*) &fd[0]);
	return (K)0;

}

K init(K x) {

	cb_init(&queue, 1000000);
	return (K) 0;

}

K push(K x) {

	if(cb_write(&queue, kG(x), x->n, 0)) {

		krr("full");
		return (K)0;

	}

	return (K)0;

}

K pop(K x) {

	K r = ktn(KG, x->j);
	if(cb_read(&queue, kG(r), x->j, 0)) {

		r0(r);
		krr("empty");
		return (K)0;

	}

	return r;

}

K stats(K x) {

	K keys = ktn(KS,4);
	K vals = ktn(KJ,4);

	kS(keys)[0] = ss("h");
	kS(keys)[1] = ss("t");
	kS(keys)[2] = ss("n");
	kS(keys)[3] = ss("size");

	kJ(vals)[0] = queue.h;
	kJ(vals)[1] = queue.t;
	kJ(vals)[2] = queue.n;
	kJ(vals)[3] = cb_size(&queue);

	K dict = xD(keys,vals);

	return dict;

}

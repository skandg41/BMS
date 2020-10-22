#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/resource.h>
#include <string.h>
#include <time.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while(0);

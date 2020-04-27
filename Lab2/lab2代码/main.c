#include "httpd.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include<string.h>
//初始化
int startup(const char* ip, int port)
{
	assert(ip);

	int sock=socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		print_log("socket failed!  ",FATAL);
		return 1;
	}

	int opt=1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);
	socklen_t len = sizeof(local);

	if(bind(sock, (struct sockaddr*)&local, len) < 0)
	{
		print_log("bind_failed!  ",FATAL);
		return 2;
	}

	if(listen(sock,5) < 0)
	{
		print_log("listen failed!  ",FATAL);
		return 3;
	}
	return sock;
}

void Usage(const char* proc)
{
	printf("%s [local_ip] [local_port]",proc);
}

void* accept_request(void* arg)
{
	int sock = (int)arg;
	pthread_detach(pthread_self());
	return (void*)handle(sock);
}

int main(int argc, char **argv)
{
	

   int opt;
   int digit_optind = 0;
   int option_index = 0;
   char *optstring = "ip:port:number-thread";

   static struct option long_options[] = {

       {"ip", required_argument, NULL, 'i'},

      {"number-thread",  required_argument, NULL, 'n'},

       {"port", required_argument, NULL, 'p'},
       
       {0, 0, 0, 0}

   };

    char *ip,*port,*num;

   while ( (opt = getopt_long(argc, argv, optstring, long_options, &option_index)) != -1)
   {
        
        if(opt=='i')
        {
        	ip=optarg;
		}
		if(opt=='p')
        {
            port=optarg;
		}
		if(opt=='n')
		{
			num=optarg;
		}
   }
	
	
	int listen_sock = startup(ip,atoi(port));
//	daemon(1,0);
	while(1)
	{
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
		int sock = accept(listen_sock,(struct sockaddr*)&client,&len);
		if(sock < 0)
		{
			print_log("accept failed!",FATAL);
			continue ;
		}

		pthread_t tid;
		pthread_create(&tid, NULL, accept_request, (void*)sock);
	}
	close(listen_sock);
	return 0;
}

#ifndef __REDIS_SERVER_H__
#define __REDIS_SERVER_H__
#include <WINSOCK2.H>
#include "redisDb.h"

typedef struct redisServer{
	SOCKET sListen;
	SOCKADDR_IN local;
	//�˿ں�
	int port;
	//һ�����飬�����ŷ�����������������ݿ�
	redisDb**db;
	//�����������ݿ�����
	int dbnum;
	//...
}redisServer;

//�ͷŷ����
void redisServerRelease(redisServer*SERVER){
	for(int i=0;i<SERVER->dbnum;i++){
		//if(SERVER->db[i]->dict->ht->table[0]==NULL)
		//printf("%d\n",i);
		
		redisDbRelease(SERVER->db[i]);
	}
	//free(server->db);
	//free(server);
}

#endif

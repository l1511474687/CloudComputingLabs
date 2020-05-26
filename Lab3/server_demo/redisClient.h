#ifndef __REDIS_CLIENT_H__
#define __REDIS_CLIENT_H__
#include <WINSOCK2.H>
#include <stdio.h>
#include "redisDb.h"
using namespace std;

typedef struct redisClient{
	SOCKET sClient;
	SOCKADDR_IN my_client;
	//��¼�ͻ��˵�ǰ����ʹ�õ����ݿ�
	redisDb*db;
	//...
}redisClient;

//��ʼ���ͻ���
redisClient* redisClientCreate(){
	redisClient*client=(redisClient*)malloc(sizeof(redisClient));
	return client;
}


//�л����ݿ�Ĳ���
bool redisClientChangeDB(redisServer*server,int dbId,redisClient*&client){
	//�ж�Ҫ�л������ݿ�Id�Ƿ�Ϸ�
	if(dbId>=server->dbnum||dbId<0){
		printf("PIG Redis ERROR : DbId is Illegal.\n");
		return false;
	}
	client->db=server->db[dbId];
	return true;
}


//��ǰʹ�õ����ݿ��в���/����
bool redisClientInsertDB(redisClient*client,char* key,char* val){
	return redisDbInsert(client->db,key,val);
}


//��ǰʹ�õ����ݿ���ɾ��ָ��key��Ӧ�ļ�ֵ��
void redisClientDeleteDB(redisClient*client,char* key){
	redisDbDelete(client->db,key);
	return;
}


//��ǰʹ�õ����ݿ��л�ȡָ��key��Ӧ��value
char* redisClientFetchDB(redisClient*client,char* key){
	//printf("===%s\n",(char*)(redisDbFetchValue(client->db,key)));
	return redisDbFetchValue(client->db,key);
}

#endif


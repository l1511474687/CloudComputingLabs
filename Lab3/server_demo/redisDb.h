#ifndef __REDIS_H__
#define __REDIS_H__
#include "dict.h"
#include <string>
using namespace std;
//���ݿ�
typedef struct redisDb{
	//���ݿ��ڷ���˶�Ӧ��Id
	int id;
	//���ݿ�������������ݵ��ֵ�����
	//�ֵ������е�hashTable������ż�ֵ��
	dict*dict;
}redisDb;

//��ʼ��һ�����ݿ�
redisDb* redisDbCreate(dictType*type,int hashSize,int id);

//��ʼ��һ�����ݿ�
redisDb* redisDbCreate(dictType*type,int hashSize);

//�����ݿ������/�����½�
bool redisDbInsert(redisDb*db,char* key,char* val);

//�����ݿ���ɾ����
void redisDbDelete(redisDb*db,char* key);

//�����ݿ��л�ȡ����key��Ӧ��value
char* redisDbFetchValue(redisDb*db,char* key);

//�ͷ����ݿ�
void redisDbRelease(redisDb*db);

#endif
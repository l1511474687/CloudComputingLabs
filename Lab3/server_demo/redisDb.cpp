#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "redisDb.h"

/*//��ϣ��Ĵ�С
#define HASHSIZE 10

//����Թ�ϣ�������ز����ĺ�����
//�����ϣֵ�ĺ���
unsigned int myHashFunction(void *key,int size){
	char* charkey=(char*)key;
	unsigned int hash=0;
	for(;*charkey;++charkey){
		hash=hash*33+*charkey;
	}
	return hash%size;
}
//���Ƽ��ĺ���
void *myKeyDup(void *key){
	return key;
}
//����ֵ�ĺ���
void *myValDup(void *obj){
	return obj;
}
//�Աȼ��ĺ���
int myKeyCompare(void *key1, void *key2){
	char*charkey1=(char*)key1;
	char*charkey2=(char*)key2;
	return strcmp(charkey1,charkey2);
}
//���ټ��ĺ���
void myKeyDestructor(void *key){
	//free(key);
}
//����ֵ�ĺ���
void myValDestructor(void *obj){
	//free(obj);
}


*/
//��ʼ��һ�����ݿ�
redisDb* redisDbCreate(dictType*type,int hashSize,int id){
	redisDb*db=(redisDb*)malloc(sizeof(redisDb));
	db->dict=dictCreate(type,hashSize);
	db->id=id;
	return db;
}

//��ʼ��һ�����ݿ�
redisDb* redisDbCreate(dictType*type,int hashSize){
	redisDb*db=(redisDb*)malloc(sizeof(redisDb));
	db->dict=dictCreate(type,hashSize);
	db->id=NULL;
	printf("PIG Redis WARNING : redisDb's Id is NULL.\n");
	return db;
}

//�����ݿ������/�����½�
bool redisDbInsert(redisDb*db,char* key,char* val){
/*	if(db==NULL){
		printf("NULL\n");
		return false;
	}else if(db->dict==NULL){
		printf("NULL\n");
		return false;
	}*/
	int flag=dictInsert(db->dict,key,val);
	if(flag){
		printf("PIG Redis : redisDb insert/refresh success.\n");
		return true;
	}else{
		printf("PIG Redis : redisDb insert/refresh fail.\n");
		return false;
	}
}

//�����ݿ���ɾ����
void redisDbDelete(redisDb*db,char* key){
	dictDelete(db->dict,key);
	printf("PIG Redis : redisDb delete OK.\n");
	return ;
}

//�����ݿ��л�ȡ����key��Ӧ��value
char* redisDbFetchValue(redisDb*db,char* key){
	char* result=dictFetchValue(db->dict,key);
	printf("result %s\n",result);
	if(result==""||result==NULL){
		printf("PIG Redis WARNING: redisDb fetch NULL.\n");
	}else{
		printf("PIG Redis : redisDb fetch success.\n");
	}
	return result;
}

//�ͷ����ݿ�
void redisDbRelease(redisDb*db){
	if(db==NULL)return ;
	dictRelease(db->dict);
	free(db);
}


/*int main(){
	dictType*type=(dictType*)malloc(sizeof(dictType));
	type->hashFunction=myHashFunction;
	type->keyDup=myKeyDup;
	type->valDup=myValDup;
	type->keyCompare=myKeyCompare;
	type->keyDestructor=myKeyDestructor;
	type->valDestructor=myValDestructor;

	redisDb*db=redisDbCreate(type,HASHSIZE);
	
	char*k1="aaa";
	char*v1="121";
	redisDbInsert(db,k1,v1);
	char*v2=(char*)redisDbFetchValue(db,k1);
	redisDbDelete(db,k1);
	
	printf("%s\n",v2);

	redisDbRelease(db);
	system("pause");
	return 0;
}*/

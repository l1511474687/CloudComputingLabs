#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "dict.h"
using namespace std;
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
}*/

//����һ���µ��ֵ�
dict *dictCreate(dictType *type,int hashSize){
	dict* d=(dict*)malloc(sizeof(dict));
	if(d==NULL){
		printf("PIG Redis ERROR : Dict malloc fail.\n");
		return NULL;
	}
	//��hashTable������ز������ض�������
	if(type==NULL){
		printf("PIG Redis WARNING : Type is NULL.\n");
	}
	d->type=type;
	//��ϣ���ʼ��
	d->ht=(dictht*)malloc(sizeof(dictht));
	d->ht->size=hashSize;
	d->ht->used=0;
	d->ht->table=(dictEntry**)malloc(sizeof(dictEntry*)*hashSize);
	//ȫ����㶼��ΪNULL
	for(int i=0;i<hashSize;i++){
		d->ht->table[i]=NULL;
	}
	return d;
}

//����keyѰ������hashTable�ж�Ӧ�Ľ��
dictEntry* lookup(dict *d,char* key){
	dictEntry* node;
	//��key��hashTable�ж�Ӧ���±�
	unsigned int index;
	index=d->type->hashFunction(key,d->ht->size);
	//printf("%s------%d\n",key,index);
	if(d->ht->table[index]==NULL){
		//printf("NULL\n");
		return NULL;
	}
	for(node=d->ht->table[index];node;node=node->next){
		if(!(d->type->keyCompare(key,node->key))){
			return node;
		}
	}
	return NULL;
}

//�������ļ�ֵ����ӵ��ֵ�����
bool dictInsert(dict *d, char* key, char* val){
	unsigned int index;
	dictEntry* node;
	//�����ڡ������в������
	if(!(node=lookup(d,key))){
		index=d->type->hashFunction(key,d->ht->size);
		//printf("key : %s\n",key);
		//printf("index :%d\n",index);
		node=(dictEntry*)malloc(sizeof(dictEntry));
		if(node==NULL)return false;
		//node->key=d->type->keyDup(key);
		//strcpy(node->key,key);
		//TODO
		node->key=(char*)malloc(sizeof(char)*(strlen(key)+1));
		strcpy(node->key,key);
		//node->value="";
		node->next=d->ht->table[index];
		d->ht->table[index]=node;
	}
	//�����ڡ������и��²���,ֱ���޸����Ӧ��valueֵ
	//TODO
	node->value=(char*)malloc(sizeof(char)*(strlen(val)+1));
	strcpy(node->value,val);
	return true;
}

//���ظ����ļ���ֵ
char* dictFetchValue(dict *d, char* key){
	dictEntry* node;
	//�Ҳ���������
	if(!(node=lookup(d,key))){
		return "";
	}
	//cout<<node->value<<"coutcoutcout"<<endl;
	//printf("printf : %s\n",node->value);
	return node->value;
}

//���ֵ���ɾ������������Ӧ�ļ�ֵ��
void dictDelete(dict *d, char* key){
	dictEntry* node;
	dictEntry* temp;
	//��key��hashTable�ж�Ӧ���±�
	unsigned int index;
	index=d->type->hashFunction(key,d->ht->size);
	node=d->ht->table[index];
	//key��ͬ
	if(!(d->type->keyCompare(key,node->key))){
		d->ht->table[index]=node->next;
		d->type->keyDestructor(node->key);
		d->type->valDestructor(node->value);
		free(node);
		return;
	}
	temp=node;
	node=node->next;
	while(node){
		if(!(d->type->keyCompare(key,node->key))){
			temp->next=node->next;
			d->type->keyDestructor(node->key);
			d->type->valDestructor(node->value);			
			free(node);
			return;
		}
		temp=node;
		node=node->next;
	}
	return;
}
//�ͷŸ����ֵ䣬�Լ��ֵ��а��������м�ֵ��
void dictRelease(dict *d){
	if(d==NULL)return;
	dictEntry* node;
	dictEntry* temp;
	for(int i=0;i<d->ht->size;i++){
		node=d->ht->table[i];
		while(node!=NULL){
			//printf("hh\n");
			//char* t=(char*)node->value;
			temp=node;
			node=node->next;
			d->type->keyDestructor(temp->key);
			d->type->valDestructor(temp->value);
			free(temp);
		}
	}
	free(d->ht);
/*	free(d->type);*/
	free(d);
}

/*int main(){
	dictType*type=(dictType*)malloc(sizeof(dictType));
	type->hashFunction=myHashFunction;
	type->keyDup=myKeyDup;
	type->valDup=myValDup;
	type->keyCompare=myKeyCompare;
	type->keyDestructor=myKeyDestructor;
	type->valDestructor=myValDestructor;
	dict* d=dictCreate(type,HASHSIZE);
	
	char*key1="sss";
	char*value1="111";
	bool result=dictInsert(d,key1,value1);
	if(result){
		printf("insert1 success\n");
	}else{
		printf("insert1 fail\n");
	}

	char*key2="3sd";
	char*value2="ddd";
	result=dictInsert(d,key2,value2);
	if(result){
		printf("insert2 success\n");
	}else{
		printf("insert2 fail\n");
	}

	char*key3="ddds";
	char*value3="1ss";
	result=dictInsert(d,key3,value3);
	if(result){
		printf("insert3 success\n");
	}else{
		printf("insert3 fail\n");
	}
	
	char *value4=(char*)dictFetchValue(d,key3);
	printf("---%s\n",value4);

	dictDelete(d,key3);
	value4=(char*)dictFetchValue(d,key3);
	printf("---%s\n",value4);

	dictRelease(d);
	system("pause");
	return 0;
}*/
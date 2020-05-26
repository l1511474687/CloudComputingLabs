#pragma warning(disable: 4786)
#pragma comment(lib, "ws2_32.lib")  
#include <WINSOCK2.H>   
#include <stdio.h>     
#include <string>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <process.h>
#include <vector>
#include "redisServer.h"
#include "redisClient.h"
using namespace std;

//��������ݿ����������������У�16�����ݿⶼ�Ǹ�ͬһ���û�ʹ�õ�
#define DBNUM 16
//�˿ں�                    
#define PORT 5155    
//������Ϣ������ֽڳ���
#define MSGSIZE 1024 
#define MAXCLIENTNUM 10

//��ϣ��Ĵ�С
#define HASHSIZE 10

//�����
redisServer*server;

//�ָ��ַ����ķ���
vector<string> split(string str,string ch){
	vector<string>vec;
	if(""==str){
		return vec;
	}
	string strs=str+ch;

	size_t pos=strs.find(ch);
	size_t size=strs.size();
	
	//nposΪ����λ��
	while(pos!=std::string::npos){
		string x=strs.substr(0,pos);
		vec.push_back(x);
		strs=strs.substr(pos+1,size);
		pos=strs.find(ch);
	}
	return vec;
}

//����Թ�ϣ�������ز����ĺ�����
//�����ϣֵ�ĺ���
unsigned int myHashFunction(char* key,int size){
	//char* charkey=(char*)key.data();
	unsigned int hash=0;
	for(;*key;++key){
		hash=hash*33+*key;
	}
	return hash%size;
}
//���Ƽ��ĺ���
char* myKeyDup(char* key){
	//strcpy
	return key;
}
//����ֵ�ĺ���
char* myValDup(char* obj){
	return obj;
}
//�Աȼ��ĺ���
int myKeyCompare(char* key1, char* key2){
	//char*charkey1=(char*)key1.data();
	//char*charkey2=(char*)key2.data();
	
	return strcmp(key1,key2);
}
//���ټ��ĺ���
void myKeyDestructor(char* key){
	key=NULL;
	free(key);
}
//����ֵ�ĺ���
void myValDestructor(char* obj){
	obj=NULL;
	free(obj);
}

//�����߳�
DWORD WINAPI WorkerThread(LPVOID lpParam);

int main(){
	dictType*type=(dictType*)malloc(sizeof(dictType));
	type->hashFunction=myHashFunction;
	type->keyDup=myKeyDup;
	type->valDup=myValDup;
	type->keyCompare=myKeyCompare;
	type->keyDestructor=myKeyDestructor;
	type->valDestructor=myValDestructor;

	WSADATA wsaData;  
	int iaddrSize = sizeof(SOCKADDR_IN); 
	DWORD dwThreadId;
 
	//��ʼ��Windows�׽��ֿ�
    WSAStartup(0x0202, &wsaData);
	
	//��ʼ�������
	//server=redisServerCreate(PORT,DBNUM,type,HASHSIZE);
	server=(redisServer*)malloc(sizeof(redisServer));

	//�����ͻ��˵��׽���
	server->sListen=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//��
	(server->local).sin_family=AF_INET;  
	(server->local).sin_port=htons(PORT); 
	(server->local).sin_addr.s_addr = htonl(INADDR_ANY); 
	bind(server->sListen, (struct sockaddr *) &(server->local), sizeof(SOCKADDR_IN));

	server->dbnum=DBNUM;

	//��ʼ�����ݿ�����
	server->db=(redisDb**)malloc(sizeof(redisDb*)*server->dbnum);
	for(int k=0;k<server->dbnum;k++){
		server->db[k]=redisDbCreate(type,HASHSIZE,k);
	}

	//����
	listen(server->sListen, 1);   

    //����̵߳ľ��
    HANDLE hThread[MAXCLIENTNUM];
	
	for(int i=0;i<MAXCLIENTNUM;i++){
		//�ȴ��ͻ�������
		redisClient*client=redisClientCreate();
		client->sClient=accept(server->sListen, (struct sockaddr *) &(client->my_client), &iaddrSize);  
		//���ͻ��˶Է�����ж�Ӧ�����ݿ����ӡ���Ĭ�ϴӵ�0�����ݿ⿪ʼʹ��
		client->db=server->db[0];
		printf("Accepted client:%s:%d\n", inet_ntoa((client->my_client).sin_addr),      
            ntohs((client->my_client).sin_port));

		//���������߳�
		hThread[i]=CreateThread(NULL, 0, WorkerThread, client, 0, NULL);
	}


	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
    //�ر��׽���  
    closesocket(server->sListen);
	printf("PIG Redis : Threads are fully executed\n");

	redisServerRelease(server);

	system("pause");
	return 0;
}

//�����߳�
DWORD WINAPI WorkerThread(LPVOID lpParam){
	char szMessage[MSGSIZE];      
	int ret;
	redisClient*client=(redisClient*)lpParam;
	client->db->dict;
	while(true){
		//ret�Ƿ�����Ϣ���ֽڳ���
		//szMessage�Ƿ�����Ϣ������
		ret = recv(client->sClient, szMessage, MSGSIZE, 0); 
		szMessage[ret] = '\0'; 
		printf("PIG Redis : Received [%d bytes]: '%s'\n", ret, szMessage); 
		
		//����ת��
		string message(szMessage);

		//���û����͵���������ж�
		//�û�����汾��Ϣ
		if(message=="-v"||message=="--version"){
			char *str="Redis by Shimmer Pig v=0.0.2000 <00000000/0> 64 bit";
			//���û������������Ӧ
			send(client->sClient,str,strlen(str)+sizeof(char),NULL);
			printf("PIG Redis : Send version information success.\n");
		//�û���������ĵ�
		}else if(message=="-h"||message=="--help"){
			char *str="help document not yet perfect";
			send(client->sClient,str,strlen(str)+sizeof(char),NULL);
			printf("PIG Redis : Send help document success.\n");	
		}else{
			//�Է��͵���Ϣ���зָ����Ƿ�������������ʽ
			//�ָ��������ո�
			string ch=" ";
			vector<string>vec=split(message,ch);
			
			//�������ʽ�����ж�
			if(vec[0]=="set"){
				//��������
				if(vec.size()==3){
					//��ü�ֵ
					vector<string>::iterator t;
					t=vec.begin();
					t++;
					string key=*t;
					t++;
					string value=*t;
					char*charkey=(char*)key.data();
					char*charvalue=(char*)value.data();
					//cout<<"DEBUG:"<<key<<" "<<value<<endl;
					//ִ��set����
					bool flag=redisClientInsertDB(client,charkey,charvalue);
					//ִ���Ƿ�ɹ�
					if(flag){
						char*str1="set ok";
						send(client->sClient,str1,strlen(str1)+sizeof(char),NULL);
						printf("PIG Redis : Send set ok\n");
					}else{
						char*str2="set error";
						send(client->sClient,str2,strlen(str2)+sizeof(char),NULL);
						printf("PIG Redis : Send set error\n");
					}

				//���Ȳ�ƥ��
				}else{
					char *str="PIG Redis ERROR : Wrong number of arguments for 'set' command";
					send(client->sClient,str,strlen(str)+sizeof(char),NULL);
					printf("PIG Redis : Send set error.\n");
				}
			}
			
			else if(vec[0]=="get"){
				//��������
				if(vec.size()==2){
					//��ü�ֵ
					vector<string>::iterator t;
					t=vec.begin();
					t++;
					string key=*t;
					char*charkey=(char*)key.data();
					char* getValue=redisClientFetchDB(client,charkey);
					
					printf("value:");
					printf("%s\n",getValue);
					//�Ҳ��������ֵ��
					if(getValue==""||getValue==NULL){
						
						char *str="PIG Redis ERROR : Can't find such key-value pairs";
						send(client->sClient,str,strlen(str)+sizeof(char),NULL);
						printf("PIG Redis : Send get error\n");
					//�ҵ��˼�ֵ�ԣ���ͻ��˷���get����ֵ
					}
					else{
						send(client->sClient,getValue,strlen(getValue)+sizeof(char),NULL);
						printf("PIG Redis : Send get ok\n");
					}

				//����Ȳ�ƥ��
				}else{
					char *str="PIG Redis ERROR : Wrong number of arguments for 'get' command";
					send(client->sClient,str,strlen(str)+sizeof(char),NULL);
					printf("PIG Redis : Send get error.\n");
				}
			}
			
			else if(vec[0]=="del"){
				   
				    vector<string>::iterator t;
					t=vec.begin();
					t++;
					string key=*t;
					char* charkey=(char*)key.data();
					//char* charvalue=(char*)value.data();
					
					//char* getValue=redisClientFetchDB(client,charkey);
					//myKeyDestructor((char*)key.data());
					//myValDestructor(getValue);
					redisDbDelete(client->db,charkey);
					//redisClientDeleteDB(client,charkey);
					
					char *str="del ok";
					send(client->sClient,str,strlen(str)+sizeof(char),NULL);
					printf("PIG Redis : Send del ok\n");
					
			}
			
			
			
				//��ǰ������л�û���������
			else{
				char *str="PIG Redis ERROR : No such command";
				send(client->sClient,str,strlen(str)+sizeof(char),NULL);
				printf("PIG Redis : Send command error.\n");
			}

		}

	}
	return 0;
}

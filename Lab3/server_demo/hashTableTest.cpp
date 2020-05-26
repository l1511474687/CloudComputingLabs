//������ʵ��hashTableС��ϰ
#include <iostream>
#include <string>
#include <malloc.h>
#include <string.h>

using namespace std;

//��ϣ��Ĵ�С
#define HASHSIZE 10
typedef unsigned int uint;

//�����ϣ����
typedef struct Node{
	const char*key;
	const char*value;
	Node*next;
}Node;

class HashTable{
private:
	Node*node[HASHSIZE];
public:
	//���췽��
	HashTable();
	//�Լ�����hash�ķ���
	uint hash(const char*key);
	//���ظ�key��Ӧ�Ľ��
	Node*lookup(const char*key);
	//���ϣ���в���һ�����ķ���
	bool insert(const char*key,const char*value);
	//�ӹ�ϣ���л�ȡĳ��key�Ľ���value�ķ���
	const char*get(const char*key);
	//��ӡ����ϣ��ķ���
	void display();
};


//���췽��
HashTable::HashTable() {
	for (int i = 0; i < HASHSIZE; i++) {
		//��HashTable�е�ÿ������ʼ��ΪNULL
		node[i] = NULL;
	}
}

//�����ϣ���hash�㷨,����ʹ��time33
//hash�㷨--��keyת���ɽڵ������ж�Ӧ��index
uint HashTable::hash(const char*key){
	uint hash=0;
	//��key�е�ÿ���ַ����в���
	for(;*key;++key){
		hash=hash*33+*key;
	}
	return hash%HASHSIZE;
}

//����keyѰ�����Ӧ�Ľ���������е�λ�õķ���
//�õ��˽�㣬���Ǿ���֪���������е�value
//ʵ���˸���keyѰ��value�Ĺ���
//HashTableҪʵ�ֵľ���һ������keyѰ��value�Ĺ���
//������ǽ�keyת�������鷶Χ�ڵ�index
//���ڿ��ܳ��ֵĶ��key��Ӧһ��index�����������ʹ��������������
//��Ȼ��ͬһ��index���������node��ָ����һ��node���γ�һ������
//�������ǿ��Ը��ݶ�����ı������õ�key��ͬ��node���Ӷ��õ������value
Node*HashTable::lookup(const char*key){
	Node*np;
	uint index;
	//��������key��Ӧ��index
	index=hash(key);
	for(np=node[index];np;np=np->next){
		//�ҵ���������key��ͬ��node
		if(!strcmp(key,np->key)){
			return np;
		}
	}
	return NULL;
}

//��hashTabel�й�������ķ���
bool HashTable::insert(const char*key, const char*value) {
	uint index;
	Node*np;
	//���key��Ӧ�Ľ�㣨��Ψһ�ģ��ڱ��в����ڣ�
	//���½�һ��node�������Ӧ��index������ͷ
	//��������������Ԫ��--���ָ��һֱ�ڱ�
	if(!(np=lookup(key))){
		index=hash(key);
		np=(Node*)malloc(sizeof(Node));
		if(!np)return false;
		np->key=key;
		//�����ͷ
		np->next=node[index];
		node[index]=np;
	}
	//�����ڣ�����key������һһ��Ӧ�ģ���������
	//ֱ���޸����Ӧ��valueֵ����
	np->value=value;
	return true;
}

void HashTable::display() {
	Node*temp;
	for (int i = 0; i < HASHSIZE; i++) {
		if (!node[i]) {
			cout << "[]" << endl;
		}
		else {
			cout << "[";
			for (temp = node[i]; temp; temp = temp->next) {
				cout << "[" << temp->key << "]" << "[" << temp->value << "]";
			}
			cout << "]" << endl;
		}
	}
}

/*
int main() {
	
	HashTable *ht = new HashTable();
	const char*key[] = { "a","b" ,"c","asdhu","asd","asdsad"};
	const char*value[] = { "value1","value2" ,"zhu","asf","sfdsf","asfdsagfsg"};
	for (int i = 0; i < 6; i++) {
		ht->insert(key[i], value[i]);
	}
	ht->display();
	system("pause");
	return 0;
}
*/

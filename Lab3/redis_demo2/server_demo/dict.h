#ifndef __DICT_H
#define __DICT_H
#include <string>
using namespace std;
//��ϣ��Ľ��ʹ��dictEntry�ṹ����ʾ
//ÿ��dictEntry�ṹ��������һ��key-value
typedef struct dictEntry{
	//��
	char* key;
	//ֵ
	char* value;
	//ָ���¸���ϣ���㣬�γ��������������ͻ
	struct dictEntry *next;
}dictEntry;

//����һ�����ڲ����ض����ͼ�ֵ�Եĺ���
typedef struct dictType {
	//�����ϣֵ�ĺ���
    unsigned int (*hashFunction)(char* key,int ssize);
    //���Ƽ��ĺ���
	char* (*keyDup)(char* key);
    //����ֵ�ĺ���
	char* (*valDup)(char* obj);
    //�Աȼ��ĺ���
	int (*keyCompare)(char* key1, char* key2);
    //���ټ��ĺ���
	void (*keyDestructor)(char* key);
    //����ֵ�ĺ���
	void (*valDestructor)(char* obj);
} dictType;

//��ϣ��
typedef struct dictht {
	//��ϣ������
    dictEntry **table;
	//��ϣ���С
    int size;
	//�ù�ϣ�����н�������
    int used;
} dictht;

//�ֵ�
//��ʵ�ֵ���Ƕ���ͨ�Ĺ�ϣ������һ���װ
//������һЩ����
typedef struct dict {
	//�����ض�����
    dictType *type;
	//��ϣ��
    dictht *ht;
} dict;

//����һ���µ��ֵ�
//��Ҫ�����ϣ��Ĵ�С
dict *dictCreate(dictType *type,int hashSize);
//����keyѰ������hashTable�ж�Ӧ�Ľ��
dictEntry* lookup(dict *d,char* key);
//�������ļ�ֵ����ӵ��ֵ�����
//�������ļ�ֵ����ӵ��ֵ����棬������Ѿ��������ֵ䣬
//��ô����ֵȡ��ԭ�е�ֵ
bool dictInsert(dict *d, char* key, char* val);
//���ظ����ļ���ֵ
char* dictFetchValue(dict *d, char* key);
//���ֵ���ɾ������������Ӧ�ļ�ֵ��
void dictDelete(dict *d, char* key);
//�ͷŸ����ֵ䣬�Լ��ֵ��а��������м�ֵ��
void dictRelease(dict *d);

#endif
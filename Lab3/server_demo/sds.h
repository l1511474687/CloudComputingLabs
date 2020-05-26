#ifndef SDS_H
#define SDS_H
//ʵ��Redis�еĶ�̬�ַ���
//SDS:simple dynamic string


typedef struct sdshdr{
	//��¼buf��������ʹ���ֽڵ�����
	//����SDS�������ַ����ĳ���,��
	//��������'\0';
	int len;
	//��¼buf������δʹ���ֽڵ�����
	int free;
	//�ֽ����飬���ڱ����ַ�������
	//'\0'����
	char* buf;
}*sds;

//����sds��ʹ�ÿռ���ֽ���:len
static inline int sdslen(const sds sh){
	return sh->len;
}

//����sdsδʹ�ÿռ���ֽ���:free
static inline int sdsavail(const sds sh){
	return sh->free;
}


//����һ����������c�ַ�����sds
sds sdsnew(char *);

//Ϊsds(Ҳ����buf����)����ָ���ռ�/len
sds sdsnewlen(sds,int);

//����һ���������κ����ݵĿ��ַ���
sds sdsempty(void);

//�ͷŸ�����sds
void sdsfree(sds);

//����һ������sds�ĸ���
sds sdsdup(sds);

//���sds������ַ�������
sds sdsclear(sds);

//������c�ַ���ƴ�ӵ���һ��sds�ַ�����ĩβ
sds sdscat(sds,char *);

//������sds�ַ���ƴ�ӵ���һ��sds�ַ�����ĩβ
sds sdscatsds(sds,sds);

//��������c�ַ������Ƶ�sds���棬����ԭ�е��ַ���
sds sdscpy(sds,char *);


//����sds���������ڵ����ݣ����������ڵ����ݻᱻ���ǻ����
//s = sdsnew("Hello World");
//sdsrange(s,1,-1); => "ello World"
sds sdsrange(sds,int,int);

//����һ��sds��һ��c�ַ�����Ϊ��������sds���Ƴ�������c�ַ����г��ֹ����ַ�
//s = sdsnew("AA...AA.a.aa.aHelloWorld     :::");
//s = sdstrim(s,"A. :");
//printf("%s\n", s);
//Output will be just "Hello World".
//��Сд������
sds sdstrim(sds,const char *);

//�Ա�����sds�ַ����Ƿ���ͬ
bool sdscmp(sds,sds);

#endif
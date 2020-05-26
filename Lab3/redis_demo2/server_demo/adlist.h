#ifndef __ADLIST_H__
#define __ADLIST_H__

//˫������

//˫������Ľ��
typedef struct listNode{
	struct listNode *prev;//ָ���һ������ָ��
	struct listNode *next;//ָ����һ������ָ��
	void *value;//����ŵ�ֵ
}listNode;

//����
typedef struct list{
	listNode *head;//ͷ���
	listNode *tail;//β���
	int len;//����ĳ���

	//����ʵ�ֶ�̬������������͵��ض�����
	//����ָ��
	//���ڸ����������������ֵ
	void *(*dup)(void *ptr);
	//�����ͷ��������������ֵ
	void (*free)(void *ptr);
	//���ڶԱ�
	int (*match)(void *ptr, void *key);
}list;

//�����������в����ĺ�
//��ȡ������
#define listLength(l) ((l)->len)
//��ȡ�����ͷ���
#define listFirst(l) ((l)->head)
//��ȡ�����β���
#define listLast(l) ((l)->tail)
//��ȡǰһ�����
#define listPrevNode(n) ((n)->prev)
//��ȡ��һ�����
#define listNextNode(n) ((n)->next)
//��ȡ�ý���ֵ
#define listNodeValue(n) ((n)->value)
//���ø��Ʋ����ĺ���ָ��
#define listSetDupMethod(l,m) ((l)->dup = (m))
//�����ͷŲ����ĺ���ָ��
#define listSetFreeMethod(l,m) ((l)->free = (m))
//���öԱȲ����ĺ���ָ��
#define listSetMatchMethod(l,m) ((l)->match = (m))
//��ȡ����������ĺ���ָ��
#define listGetDupMethod(l) ((l)->dup)
//��ȡ�ͷ�������ĺ���ָ��
#define listGetFree(l) ((l)->free)
//��ȡ�Ƚ�������ĺ���ָ��
#define listGetMatchMethod(l) ((l)->match)

//����һ���������κν���������
list *listCreate(void);
//�ͷŸ��������Լ������е����н��
void listRelease(list *l);
//��һ����������ֵ���½ڵ���ӵ���������ı�ͷ
list *listAddNodeHead(list *l, void *value);
//��һ����������ֵ���½ڵ���ӵ���������ı�β
list *listAddNodeTail(list *l, void *value);
//��һ����������ֵ���½ڵ���ӵ���������֮ǰ��֮��
list *listInsertNode(list *l, listNode *old_node, void *value, int after);
//��������ɾ���������
list *listDelNode(list *l, listNode *node);
//����һ����������ĸ���
list *listDup(list *orig);
//���Ҳ����������а�������ֵ�Ľ��
listNode *listSearchKey(list *l, void *key);
//���������ڸ��������ϵĽ��
listNode *listIndex(list *l, long index);
//��������ı�λ��㵯����Ȼ�󽫱������Ľ���
//�뵽����ı�ͷ����Ϊ�µı�ͷ���
void listRotate(list *l);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adlist.h"

//����һ���������κν���������
list *listCreate(void){
	list* l=(list*)malloc(sizeof(list));
	//û�н��
	l->head=NULL;
	l->tail=NULL;
	l->len=0;
	l->dup=NULL;
	l->free=NULL;
	l->match=NULL;
	return l;
}
//�ͷŸ��������Լ������е����н��
void listRelease(list *l){
	if(l==NULL){
		return ;
	}
	//û��head(û�н��)
	if(l->head==NULL){
		return ;
	}
	//��֤���������н����ڵ�
	//�����ƶ���ָ�룬ָ���һ�����
	listNode*temp=l->head;
	while(temp->next!=NULL){
		temp=temp->next;
		//ʹ�������Ӧ�ͷ�value��free���ͷ�value��ֵ
		if(l->free!=NULL){
			l->free(temp->value);
		}else{
			printf("PIG Redis WARNING : List->free is not define.\n");
		}
		free(temp->prev);
	}
	free(temp);
	l->head=NULL;
	l->tail=NULL;
	free(l);
	l=NULL;
	return;
}


//��һ����������ֵ���½ڵ���ӵ���������ı�ͷ
list *listAddNodeHead(list *l, void *value){
	if(l==NULL){
		printf("PIG Redis ERROR : List NULL.\n");
		return NULL;
	}
	//������û�н��
	if(l->head==NULL){
		l->head=(listNode*)malloc(sizeof(listNode));
		l->head->next=NULL;
		l->head->prev=NULL;
		//ʹ�������Ӧ����value��dup������value��ֵ
		if(l->dup!=NULL){
			l->head->value=l->dup(value);
		}else{
			printf("PIG Redis WARNING : List->dup is not define.\n");
			l->head->value=value;
		}
/*		int *c=(int*)(l->head->value);
		printf("%d====\n",*c);*/
		l->len=1;
		//ͷβָ�붼ָ���µĽ��
		l->tail=l->head;
		return l;
	}else{
		listNode*newone=(listNode*)malloc(sizeof(listNode));
		//newone->value=value;
		//ʹ�������Ӧ����value��dup������value��ֵ
		if(l->dup!=NULL){
			newone->value=l->dup(value);
		}else{
			printf("PIG Redis WARNING : List->dup is not define.\n");
			newone->value=value;
		}
/*		int *cc=(int*)(newone->value);
		printf("%d====\n",*cc);*/

		newone->next=l->head;
		l->head->prev=newone;
		//�½ڵ���Ϊͷ���
		l->head=newone;
		newone->prev=NULL;
		l->len++;
		return l;
	}
}

//��һ����������ֵ���½ڵ���ӵ���������ı�β
list *listAddNodeTail(list *l, void *value){
	if(l==NULL){
		printf("PIG Redis ERROR : List NULL.\n");
		return NULL;
	}
	//������û�н��
	if(l->head==NULL){
		l->head=(listNode*)malloc(sizeof(listNode));
		//l->head->value=value;
		//ʹ�������Ӧ����value��dup������value��ֵ
		if(l->dup!=NULL){
			l->head->value=l->dup(value);
		}else{
			printf("PIG Redis WARNING : List->dup is not define.\n");
			l->head->value=value;
		}		
		l->head->next=NULL;
		l->head->prev=NULL;
		l->tail=l->head;
		l->len=1;
		return l;
	}else{
		listNode*temp=(listNode*)malloc(sizeof(listNode));
		//temp->value=value;
		//ʹ�������Ӧ����value��dup������value��ֵ
		if(l->dup!=NULL){
			temp->value=l->dup(value);
		}else{
			printf("PIG Redis WARNING : List->dup is not define.\n");
			temp->value=value;
		}
		temp->next=NULL;
		temp->prev=l->tail;
		l->tail->next=temp;
		l->tail=temp;
		l->len++;
		return l;
	}
}

//��һ����������ֵ���½ڵ���ӵ���������֮ǰ��֮��
//afterΪ1��ʾ֮��afterΪ0��ʾ֮ǰ
list *listInsertNode(list *l, listNode *old_node, void *value, int after){
	listNode *newone=(listNode*)malloc(sizeof(listNode));
	//newone->value=value;
	//ʹ�������Ӧ����value��dup������value��ֵ
	if(l->dup!=NULL){
		newone->value=l->dup(value);
	}else{
		printf("PIG Redis WARNING : List->dup is not define.\n");
		newone->value=value;
	}
	l->len++;
	if(after){
		newone->next=old_node->next;
		newone->prev=old_node;
		old_node->next->prev=newone;
		old_node->next=newone;
		//���ԭ����temp�Ƿ�Ϊtail
		if(l->tail==old_node){
			l->tail=newone;
		}
		return l;
	}else{
		newone->next=old_node;
		newone->prev=old_node->prev;
		old_node->prev->next=newone;
		old_node->prev=newone;
		//���ԭ����temp�Ƿ�Ϊͷ���
		if(l->head==old_node){
			l->head=newone;
		}
		return l;		
	}
}	

//��������ɾ���������
list *listDelNode(list *l, listNode *node){
	l->len--;
	//ʹ�������Ӧ�ͷ�value��free���ͷ�value��ֵ
	if(l->free!=NULL){
		l->free(node->value);
	}else{
		printf("PIG Redis WARNING : List->free is not define.\n");
	}
	//Ҫɾ���������һ�����
	if(l->head==node&&l->tail==node){
		free(node);
		l->head=NULL;
		l->tail=NULL;
		return l;
	}else if(l->head==node){
		printf("head\n");
		l->head=node->next;
		l->head->prev=NULL;
		free(node);
		return l;
	}else if(l->tail==node){
		l->tail=node->prev;
		l->tail->next=NULL;
		free(node);
		return l;
	}
	node->prev->next=node->next;
	node->next->prev=node->prev;
	free(node);
	return l;
}

//����һ����������ĸ���
list *listDup(list *orig){
	if(orig==NULL){
		return NULL;
	}
	//������û�н��
	if(orig->head==NULL){
		list*l=listCreate();
		return l;
	}else{
		list*l=listCreate();
		listNode*temp=orig->head;
		while(temp!=NULL){
			//���β����
			l=listAddNodeTail(l,temp->value);
			temp=temp->next;
		}
		return l;
	}
}

//���Ҳ����������а�������ֵ�Ľ��
listNode *listSearchKey(list *l, void *key){
	if(l==NULL){
		printf("PIG Redis ERROR : List NULL.\n");
		return NULL;
	//������û�н��
	}else if(l->head==NULL){
		printf("PIG Redis ERROR : List does't have nodes.\n");
		return NULL;
	}else{
		listNode*temp=l->head;
		//����Ƿ����˱Ƚ�value�ĺ���
		if(l->match==NULL){
			printf("PIG Redis ERROR : List->match is not define.\n");
			return NULL;
		}
		//match�������������ʱ����1
		while(temp!=NULL&&!(l->match(key,temp->value))){
			temp=temp->next;
		}
		if(temp==NULL){
			printf("PIG Redis ERROR : List doesn't have this node.\n");
			return NULL;
		}else{
			return temp;
		}
	}
}	

//���������ڸ��������ϵĽ��,index��0��ʼ
listNode *listIndex(list *l, long index){
	if(l==NULL){
		printf("PIG Redis ERROR : List NULL.\n");
		return NULL;
	}else if(l->head==NULL){
		printf("PIG Redis ERROR : List doesn't have node.\n");
		return NULL;
	}
	listNode*temp=l->head;
	for(int i=0;i<index&&temp!=NULL;i++){
		temp=temp->next;
	}
	if(temp==NULL){
		printf("PIG Redis ERROR : Subscript out of range.\n");
		return NULL;	
	}
	return temp;
}

//��������ı�β��㵯����Ȼ�󽫱������Ľ���
//�뵽����ı�ͷ����Ϊ�µı�ͷ���
void listRotate(list *l){
	if(l==NULL){
		printf("PIG Redis ERROR : List NULL.\n");
		return ;
	}else if(l->head==NULL){
		printf("PIG Redis ERROR : List doesn't have node.\n");
		return ;
	}else if(l->head==l->tail){
		printf("PIG Redis ERROR : List only have one node.\n");
		return ;
	}	

	listNode*node=l->tail->prev;
	l->tail->prev->next=NULL;
	l->tail->next=l->head;
	l->head->prev=l->tail;
	l->head=l->tail;
	l->tail=node;
	l->head->prev=NULL;
}



/*
int intMatch(void *ptr, void *key){
	int *a=(int *)ptr;
	int *b=(int *)key;
	return (*a==*b)?1:0;
}

void *intDup(void *ptr){
	return ptr;
}
int main(){
	printf("listCreate()\n");
	list*l=listCreate();
	printf("%d\n",l->len);
	listSetDupMethod(l,&intDup);

	int b=111;
	int *a=&b;
	l=listAddNodeHead(l,a);
	printf("%d\n",l->len);
	//ʹ��void*ָ���ʱ����Ҫǿ��ת��
	int *c=(int *)(l->head->value);
	printf("%d\n",*c);

	int bb=12;
	int *aa=&bb;
	l=listAddNodeHead(l,aa);
	//listInsertNode(l,l->head,a,1);
	//l=listAddNodeTail(l,aa);
	//printf("%d\n",l->len);
	//l=listDelNode(l,l->head);
	//l=listDelNode(l,l->tail);
	//printf("%d\n",l->len);
	listRotate(l);
	//ʹ��void*ָ���ʱ����Ҫǿ��ת��
	int *cc=NULL;
	listNode*temp=l->tail;
	while(temp){
		cc=(int *)(temp->value);
		printf("%d\n",*cc);
		temp=temp->prev;
	}
	
	list*l2=listDup(l);
	temp=l2->tail;
	while(temp){
		cc=(int *)(temp->value);
		printf("%d\n",*cc);
		temp=temp->prev;
	}

	//listSetMatchMethod(l,&intMatch);
	listNode*node=listIndex(l,1);
	int *zhu=(int*)node->value;
	printf("*zhu:%d\n",*zhu);

	listRelease(l);
	//listRelease(l2);

	system("pause");
	return 0;
}*/
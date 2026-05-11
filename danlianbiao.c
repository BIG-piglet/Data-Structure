#include <stdio.h>
#include <stdlib.h>

//1 定义单链表结构体
typedef struct Node
{
	int data;
	struct Node *next;
} Node, *Linked_list;

//2 创建一个新结点
Node *createNode(int data)
{
	Node *newNode = (Node *)malloc(sizeof(Node));
	if(newNode == NULL)
	{
		perror("malloc failed");
        exit(EXIT_FAILURE);
	}
	newNode->data = data;
	newNode->next = NULL;
	
	return newNode;
}

//3 初始化链表（创建头节点、空链表）
Linked_list initLinkedList()
{
	Node *head=createNode(0);
	return head; 
}

//4 尾插法
void insertTail(Linked_list head, int data)
{
	Node *p=head;
	while(p->next!=NULL)
	{
		p=p->next;
	}
	p->next=createNode(data);
} 

//5 头插法
void insertHead(Linked_list head, int data)
{
	Node *p=createNode(data);
	p->next=head->next;
	head->next=p;
} 

//6 遍历链表
void Traverselist(Linked_list head)
{
	Node *p=head->next;
	while(p!=NULL)
	{
		printf("%d ",p->data);
		p=p->next;
	}
} 

//7 查找指定节点 返回节点地址（查找不到返回NULL）
Linked_list findNode(Linked_list head, int data)
{
	Node *p=head->next;
	while(p!=NULL)
	{
		if(p->data==data)
		{
			return p;
		}
		p=p->next;
	}
	return NULL;
} 

//8 删除第一个值为data的节点
void deleteNode(Linked_list head, int data)
{
	Node *p=head; 
	Node *q=head->next;
	while(q!=NULL && q->data!=data)
	{
		p=q; 
		q=q->next;
	}
	p->next = q->next;
	free(q);
	q=NULL;
	printf("已删除第一个值为：%d 的节点\n",data);
} 

//9 销毁链表 释放内存
void destroylist(Linked_list head)
{
	Node *p = head;
	Node *q = NULL;
	
	while(p != NULL)
	{
		q = p->next;
		free(p);
		p = q;
	}
	printf("链表已销毁，内存完全释放\n"); 
} 
  
//10 测试
int main()
{
	Node *p=initLinkedList();
	
	insertTail(p, 1);
	insertTail(p, 2);
	insertTail(p, 3);
	insertTail(p, 4);
	insertHead(p, 7);
	insertHead(p, 8);
	insertHead(p, 9);
	
	Traverselist(p);
	printf("\n");
	
	Node *q=findNode(p, 10);
	printf("查找节点地址：%p\n",q);
	
	deleteNode(p,3);
	
	Traverselist(p);
	printf("\n");
	
	destroylist(p);
	p=NULL;
	
	return 0;
 } 
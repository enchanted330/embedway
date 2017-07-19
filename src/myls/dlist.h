/*************************************************************************
    > File Name: dlist.h
    > Author: 
    > Mail: 
    > Created Time: Mon 03 Jul 2017 11:30:58 PM PDT
 ************************************************************************/
#ifndef DLIST_H
#define DLIST_H

typedef  void * item;  
typedef struct s_node * pnode;  

/*定义节点类型*/  
typedef struct s_node  
{  
    item  data;      /*数据域*/  
    pnode previous; /*指向前驱*/  
    pnode next;     /*指向后继*/  
}node;  

/*定义链表类型*/  
typedef struct  
{  
    pnode head;     /*指向头节点*/  
    pnode tail;     /*指向尾节点*/  
    int size;  
}d_list;  
  
/*分配值为i的节点，并返回节点地址*/  
pnode make_node(item i);  
  
/*构造一个空的双向链表*/  
d_list* init_list();  
  
/*摧毁一个双向链表*/  
void destroy_list(d_list *plist,void(*call_back_free)(pnode ));  
  
/*将一个链表置为空表，释放原链表节点空间*/  
void clear_list(d_list *plist,void(*call_back_free)(pnode ));  
  
/*返回头节点地址*/  
pnode get_head(d_list *plist);  
  
/*返回尾节点地址*/  
pnode get_tail(d_list *plist);  
  
/*返回链表大小*/  
int get_size(d_list *plist);  
  
/*返回p的直接后继位置*/  
pnode get_next(pnode p);  
  
/*返回p的直接前驱位置*/  
pnode get_previous(pnode p);  
  
/*删除节点并返回其地址*/  
pnode del_node(d_list *plist,pnode p);
  
/*获得节点的数据项*/  
item get_item(pnode p);  
  
/*设置节点的数据项*/  
void set_item(pnode p,item i);  

/*删除链表中的尾节点并返回其地址，改变链表的尾指针指向新的尾节点*/  
pnode Remove(d_list *plist);  
  
/*在链表中p位置之前插入新节点S*/  
pnode ins_before(d_list *plist,pnode p,pnode s);  
  
/*在链表中p位置之后插入新节点s*/  
pnode ins_after(d_list *plist,pnode p,pnode s);  
  
/*返回在链表中第i个节点的位置*/  
pnode locate_pos(d_list *plist,int i);  

/*返回在链表中数据项为i的节点的位置*/  
pnode search_item(d_list *plist,item i,int(*call_back_cmp)(void const* , const void* ));  
  
/*依次对链表中每个元素调用函数call_back_print*/  
void list_traverse(d_list *plist,void(*call_back_print)(const void *));  

/*链表排序*/
void sort_list(d_list *plist,int (*call_back_cmp)(const void*, const void*));

#endif

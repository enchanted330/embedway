/*************************************************************************
    > File Name: dlist.c
    > Author: 
    > Mail: 
    > Created Time: Mon 03 Jul 2017 11:30:42 PM PDT
 ************************************************************************/
#include "dlist.h"

#include <malloc.h>  
#include <stdlib.h>  
#include <string.h>  

pnode make_node(item i,int data_size)  
{  
    pnode  p = (pnode)malloc(sizeof(node));  
	if(p!=NULL && data_size == 0)
	{
        p->data = NULL;
        p->previous = NULL;  
        p->next = NULL;  
		p->data_size = 0;
		p->times = 0;
		return p;
	}
	item data = (item)malloc(data_size+1);
    if(p!=NULL && data!=NULL)  
    {  
		strcpy(data,i);
        p->data = data;  
        p->previous = NULL;  
        p->next = NULL;  
		p->data_size = data_size;
		p->times = 1;
        return p;  
    }     

	else
	{
	    free(p);
	    free(data);
	    return NULL;
	}
}  

void free_node(pnode p)  
{  
     free(p->data);  
	 p->data = NULL;
     free(p);  
}  

d_list * init_list()  
{  
	d_list *plist = NULL;
    plist = (d_list *)malloc(sizeof(d_list));  
    if(plist==NULL)  
	{
		print("malloc failed!\n");
		return NULL;
	}
	//memset(plist,0,sizeof(d_list));
    pnode head = make_node(NULL,0);   
    if(head!=NULL)  
    {  
        plist->head = head;  
        plist->tail = head;  
        plist->size = 0;  
    }  
    else  
        return NULL;  

    return plist;  
}  
  
void destroy_list(d_list *plist)  
{  
	if(!is_empty())
		clear_list(plist);  
    pnode h = get_head(plist);  
	free(h);
    free(plist);  
}  
  
int is_empty(d_list *plist)  
{  
    if(get_size(plist)==0 && get_tail(plist)==get_head(plist))  
        return 1;  
    else  
        return 0;  
}  

void clear_list(d_list *plist)  
{  
	pnode h = get_head(plist);
	pnode p = h->next;
	while(plist->size)
	{
		if(plist->size == 1)
		{
            free_node(p);
			return;
		}
		p = p->next;
		free_node(p->previous);
		plist->size--;
	}
	h->next = h;
}  
  
pnode get_head(d_list *plist)  
{  
    return plist->head;  
}  
  
pnode get_tail(d_list *plist)  
{  
    return plist->tail;  
}  
  
int get_size(d_list *plist)  
{  
    return plist->size;  
}  
  
pnode get_next(pnode p)  
{  
    return p->next;  
}  
  
pnode get_previous(pnode p)  
{  
    return p->previous;  
}  
/*  
pnode InsFirst(d_list *plist,pnode s)  
{  
    pnode head = get_head(plist);  
  
    if(is_empty(plist))  
        plist->tail = s;  
    plist->size++;  
  
    s->next = head->next;  
    s->previous = head;  
  
    if(head->next!=NULL)  
        head->next->previous = s;  
    head->next = s;  
      
    return s;   
}  
  
pnode DelFirst(d_list *plist)  
{  
    pnode head = get_head(plist);  
    pnode p=head->next;  
    if(p!=NULL)  
    {  
        if(p==get_tail(plist))  
            plist->tail = p->previous;  
        head->next = p->next;  
		if(head->next==NULL)
		{
			plist->size--;
			return p;
		}
        head->next->previous = head;  
        plist->size--;  
          
    }     
    return p;  
}  
*/
pnode del_node(d_list *plist,pnode p)
{
	pnode s = get_previous(p);
	s->next = p->next;
	if(p == get_tail(plist))
	{
		plist->tail = s;
		plist->size--;
		return p;
	}
	p->next->previous = s;
	plist->size--;
	return p;
}
  
item get_item(pnode p)  
{  
	if(p!=NULL)
        return p->data;  
	else
		return NULL;
}  
  
void set_item(pnode p,item i)  
{  
    p->data = i;  
}  

void add_times(pnode p)  
{  
    p->times++ ;  
}  
  
/*在链表中p位置之前插入新节点s*/  
pnode ins_before(d_list *plist,pnode p,pnode s)  
{  
    s->previous = p->previous;  
    s->next = p;  
    p->previous->next = s;      
    p->previous = s;  
  
    plist->size++;  
    return s;  
}  
/*在链表中p位置之后插入新节点s*/  
pnode ins_after(d_list *plist,pnode p,pnode s)  
{  
    s->next = p->next;  
    s->previous = p;  
      
    if(p->next != NULL)  
        p->next->previous = s;  
    p->next = s;  
      
    if(p == get_tail(plist))  
        plist->tail = s;  
      
    plist->size++;  
    return s;  
}  
  
/*返回在链表中第i个节点的位置*/  
pnode locate_pos(d_list *plist,int i)  
{  
    int cnt = 0;  
    pnode p = get_head(plist);  
    if(i>get_size(plist)||i<1)  
        return NULL;  
  
    while(++cnt<=i)  
    {  
        p=p->next;  
    }  
  
    return p;  
}  
  
/*返回在链表中数据项为i的节点的位置*/  
pnode search_item(d_list *plist,item i)  
{
	pnode p = get_head(plist);
	p = p->next;
	while(p!=NULL)
	{
		if(0==strcmp(i,p->data) && p->data_size==(int)strlen(i))
		{
			return p;
		}
		else
		{
			p = p->next;
		}
	}
	return NULL;
}

/*依次对链表中每个元素调用函数visit()*/  
void list_traverse(d_list *plist,void (*visit)())  
{  
    pnode p = get_head(plist);  
    if(is_empty(plist))  
        return;  
    else  
    {  
          
        while(p->next!=NULL)  
        {  
            p = p->next;  
            visit(p->data,p->times);            
        }         
    }  
}  

/*排序*/
void sort_list(d_list *plist,int (*str_cmp)())
{
	if(is_empty(plist))
		return;
	item temp_data = NULL;
	int  temp_data_size = 0;
	int  temp_times = 0; 

	pnode h = get_head(plist);
	pnode p = h->next;

	int count = get_size(plist);
	while(count>0)
	{
		int i = count - 1;
		while(i>0)
		{
			if(str_cmp(p->data,p->next->data)>0)
			{
				temp_data = p->data;
				temp_data_size = p->data_size;
				temp_times = p->times;

				p->data = p->next->data;
				p->data_size = p->next->data_size;
				p->times = p->next->times;

				p->next->data = temp_data;
				p->next->data_size = temp_data_size;
				p->next->times = temp_times;
			}
			p = p->next;
			i--;
		}
		p = h->next;
		count--;
	}
}


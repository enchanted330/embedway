/*************************************************************************
    > File Name: test.c
    > Author: 
    > Mail: 
    > Created Time: Tue 04 Jul 2017 12:28:09 AM PDT
 ************************************************************************/

#include"dlist.h"  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<ctype.h>  

#define BUFF_SIZE  1024
#define UA_CODE    65
#define LA_CODE    97
#define AZ_LEN     26
#define ASCII_LEN  128

char ascii[ASCII_LEN];
char flag[ASCII_LEN];

char * read_file(FILE *fp);
char * word_job(char *str,int len);
void print(item data,int times);  
int  str_cmp(item data1,item data2);  

int main(int argc,char **argv)  
{  
	if(argc!=2)
	{
		printf("Usage: %s filename\n",argv[0]);
		return 1;
	}

	memset(ascii,1,ASCII_LEN);
	memset(ascii+UA_CODE,0,AZ_LEN);
	memset(ascii+LA_CODE,0,AZ_LEN);

	memset(flag,0,ASCII_LEN);
	int i=0;
	for(i=1;i<27;i++)
		memset(flag + LA_CODE-1+i,i,1);

	char *path = argv[1];
	FILE *fp = fopen(path,"r");
	if(NULL == fp)
	{
		printf("Can't open file!\n");
		return 1;
	}
	
    d_list *plist[27] = {NULL};  
	plist[0] = NULL;
	for(i=1;i<27;i++)
		plist[i] = init_list();  
	char *buff = NULL;
	buff = read_file(fp);

	char *val = word_job(buff,(int)strlen(buff));
	char *temp = strtok(val," ");
	while(NULL != temp)
	{
		unsigned char i = temp[0];
		unsigned char j = flag[i];

		pnode q = search_item(plist[j],temp);
		if(NULL == q)
		{
			ins_after(plist[j],get_head(plist[j]),make_node(temp,strlen(temp)));
		} 
		else
		{
			add_times(q);
		}
		temp = strtok(NULL," ");
	}

	for(i=1;i<27;i++)
		sort_list(plist[i],str_cmp);

	for(i=1;i<27;i++)
		list_traverse(plist[i],print);  

	int count = 0;
	for(i=1;i<27;i++)
	{
		count = count + get_size(plist[i]);
	}
    printf("共有%d个不同单词\n",count);

	for(i=1;i<27;i++)
		destroy_list(plist[i]);  

	free(buff);
	fclose(fp);

	return 0;
}  

char * word_job(char *str,int len)
{
	if(NULL == str)
		return NULL;
   int i=0;
   for(i=0;i<len;i++)
   {
	   int j = str[i];
	   if(isupper(str[i]))
		   str[i] = tolower(str[i]);
	   if(ascii[j])
	   {
		   if(isalpha(str[i-1]) && isalpha(str[i+1]) && j!=10 && j!= 13)
			   ;
		   else
			   str[i] = ' ';
	   }
	   
   }
   return str;
}

void print(item data,int times)  
{  
    printf("[%-20s] ",(char*)data);  
    printf("出现 %-5d 次\n",times);  
}  

int str_cmp(item data1,item data2)  
{  
    return strcmp(data1,data2);
}  

char * read_file(FILE *fp)
{
	char *data = NULL;
	int len = 0;

	fseek(fp,0,SEEK_END);
	len = ftell(fp);

	data = (char*)malloc(len+1);
	if(data==NULL)
	{
		return NULL;
	}

	rewind(fp);
	len = fread(data,1,len,fp);
	data[len] = '\0';
	
	return data;
}

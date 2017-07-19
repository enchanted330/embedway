/*************************************************************************
    > File Name: myls.c
    > Author: 
    > Mail: 
    > Created Time: Sun 16 Jul 2017 06:40:09 PM PDT
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <getopt.h>

#include "dlist.h"
#include "myls.h"

#define  ERROR  -1
#define  OK     0

#define NONE    "\e[0m"
#define BLUE    "\e[1;34m"
#define GREEN   "\e[1;32m"

static int aflag,lflag,Rflag,Uflag,iflag,rflag,Sflag;
const char    *short_opt = "alRUirS";
struct option long_opt[] = {
	                           {"all",0,NULL,'a'},
                           	   {   "",0,NULL,'l'},
                           	   {   "",0,NULL,'R'},
                           	   {   "",0,NULL,'U'},
                           	   {"inode",0,NULL,'i'},
                           	   {"reverse",0,NULL,'r'},
                           	   {   "",0,NULL,'S'},
                               {0,0,0,0}
                           };

int main(int argc, char *argv[])
{
	int ret;
	while(-1 != (ret = getopt_long(argc, argv, short_opt, long_opt, NULL)))
	{
		switch(ret)
		{
			case 'a': /* 列出所有文件 */
				aflag = 1;
				break;
			case 'l': /* 列出详细信息 */
				lflag = 1;
				break;
			case 'R': /* 递归遍历目录 */
				Rflag = 1;
				break;
			case 'U': /* 不进行排序 */
				Uflag = 1;
				break;
			case 'i': /* 列出inode节点号 */
				iflag = 1;
				break;
			case 'r': /* 反向排序 */
				rflag = 1;
				break;
			case 'S': /* 按文件大小排序 */
				Sflag = 1;
				break;
			default:
				break;
		}
	}
	argc = argc - optind;
	d_list *plist = init_list();
	if(0 == argc)
		save_to_list(".",plist);
	else
	{
		int i;
		for(i=0;i<argc;i++)
		{
	        save_to_list(argv[i+optind],plist);
		}
	}
	if(0 == Uflag)
		sort_list(plist,str_cmp);
	list_traverse(plist,print);
	destroy_list(plist,free_node);
	return OK;
}

/* 将文件信息保存到file_info结构体 */
file_info *save_to_info(struct stat* pst,const char *name)
{
	file_info *pf= (file_info*)malloc(sizeof(file_info));
	if(NULL == pf)
	{
		perror("malloc");
		return NULL;
	}
	char *str = (char*)malloc(strlen(name)+1);
	if(NULL == str)
	{
		perror("malloc");
		free(pf);
		return NULL;
	}
	strcpy(str,name);
	pf->uid   = pst->st_uid;
	pf->gid   = pst->st_gid;
	pf->nlink = pst->st_nlink;
	pf->size  = pst->st_size;
	pf->inode = pst->st_ino;
	pf->mtime = pst->st_mtime;
	pf->mode  = pst->st_mode;
	pf->name  = str;
	return pf;
}

/* 保存至链表 */
int save_to_list(const char *path, d_list *plist)
{
	char   fpath[4096];
	struct stat stat_buff; 
	if(-1 == lstat(path,&stat_buff))
	{
		perror("lstat");
		return ERROR;
	}

	if(S_ISDIR(stat_buff.st_mode))
	{
		DIR           *pd;
		struct dirent *pdir;
		if(NULL == (pd = opendir(path)))
		{
			perror("opendir");
			return ERROR;
		}
		chdir(path);
		while(NULL != (pdir = readdir(pd)))
		{
			if(-1 == lstat(pdir->d_name,&stat_buff))
			{
				perror("lstat");
				closedir(pd);
				return ERROR;
			}
			if(S_ISDIR(stat_buff.st_mode) && 1 == Rflag) /*是否指定-R选项*/
			{
				if(1 == aflag) /* 是否指定-a选项 */
				{
					if(0 == strcmp(".",pdir->d_name) || 0 == strcmp("..",pdir->d_name)) /*   "."、".."目录不递归  */
					{
						file_info *pf = save_to_info(&stat_buff,pdir->d_name);
						ins_after(plist,get_head(plist),make_node(pf));
					}
					else
						save_to_list(pdir->d_name,plist); /* 递归调用 */
				}
				else /* 保存非隐藏目录 */
				{
					if('.' != pdir->d_name[0])
						save_to_list(pdir->d_name,plist); /* 递归调用 */
				}
			}
			else  /* 保存文件信息 */
		    {
				if(1 == aflag)
				{
					file_info *pf = save_to_info(&stat_buff,pdir->d_name);
					ins_after(plist,get_head(plist),make_node(pf));
				}
				else
				{
					if('.' != pdir->d_name[0])
					{
						file_info *pf = save_to_info(&stat_buff,pdir->d_name);
						ins_after(plist,get_head(plist),make_node(pf));
					}
				}
            }
		}
		chdir("..");
		closedir(pd);
		return OK; 
	}
	else
	{
		file_info *pf = save_to_info(&stat_buff,path);
		ins_after(plist,get_head(plist),make_node(pf));
		return OK;
	}
}

/* 将uid转换成username */
char *uid_str(uid_t uid)
{
	struct passwd *ppwd;
	if(NULL == (ppwd = getpwuid(uid)))
	{
		perror("getpwuid");
        return NULL;
	}
	else
		return ppwd->pw_name;
}

/* 将gid转换成groupname */
char *gid_str(gid_t gid)
{
	struct group *pgr;
	if(NULL == (pgr = getgrgid(gid)))
	{
		perror("getgrgid");
        return NULL;
	}
	else
		return pgr->gr_name;
}

/* 获取文件权限位 */
char *mode_str(int mode,char *str)
{
	strcpy(str,"----------");

	if(S_ISDIR(mode))
		str[0] = 'd';
	if(S_ISCHR(mode))
		str[0] = 'c';
	if(S_ISBLK(mode))
		str[0] = 'b';

	if(S_IRUSR & mode)
		str[1] = 'r';
	if(S_IWUSR & mode)
		str[2] = 'w';
	if(S_IXUSR & mode)
		str[3] = 'x';

	if(S_IRGRP & mode)
		str[4] = 'r';
	if(S_IWGRP & mode)
		str[5] = 'w';
	if(S_IXGRP & mode)
		str[6] = 'x';

	if(S_IROTH & mode)
		str[7] = 'r';
	if(S_IWOTH & mode)
		str[8] = 'w';
	if(S_IXOTH & mode)
		str[9] = 'x';

	return str;
}

int str_cmp(const void* p1, const void* p2)
{
	file_info *pf1 = (file_info*)p1;
	file_info *pf2 = (file_info*)p2;
	file_info *tmp;
	if(1 == rflag)
	{
		tmp = pf1;
		pf1 = pf2;
		pf2 = tmp;
	}
	if(1 == Sflag)
		return (pf2->size) > (pf1->size) ? 1 : 0; /* 大小排序 */
	return strcmp(pf1->name,pf2->name);           /* 文件名排序 */
}

void free_node(pnode p)
{
	file_info *pf = (file_info*)(p->data);
	free(pf->name);
	free(pf);
	free(p);
}

void print(const void *p)
{
	file_info *pf = (file_info*)p;
	if(1 == lflag)
	{
		if(1 == iflag)
			printf("%d ",pf->inode);  /* inode 节点号 */
		print_info(pf);
		print_name(pf);
	}
	else
	{
		if(1 == iflag)
			printf("%d ",pf->inode);
		print_name(pf);
	}
	return;
}

void print_info(file_info *pf)
{
	char str[11];
	printf("%s ",mode_str(pf->mode,str));  /* 权限 */
	printf("%2d ",(int)pf->nlink);         /* 连接数 */
	printf("%-10s ",uid_str(pf->uid));     /* 用户 */
	printf("%-10s ",gid_str(pf->gid));     /* 组 */
	printf("%6d ",(int)pf->size);          /* 大小 */
	printf("%.12s ",ctime(&pf->mtime)+4);  /* 最后修改时间 */
}

void print_name(file_info *pf)
{
	if(S_ISDIR(pf->mode))
		printf(""BLUE"%s\n"NONE,pf->name); /* 目录-蓝色 */
	else if((S_IXOTH & pf->mode) || (S_IXUSR & pf->mode) || (S_IXGRP & pf->mode))
		printf(""GREEN"%s\n"NONE,pf->name);/* 可执行文件-绿色 */
	else
		printf("%s\n",pf->name);           /* 普通文件 */
}

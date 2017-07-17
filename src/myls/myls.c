/*************************************************************************
    > File Name: myls.c
    > Author: 
    > Mail: 
    > Created Time: Sun 16 Jul 2017 06:40:09 PM PDT
 ************************************************************************/
#include "dlist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>

#define  ERROR  -1
#define  OK     0

typedef struct s_file_info
{
	int    uid;
	int    gid;
	int    nlink;
	int    mode;
	int    size;
	time_t mtime;
	char   *name;
}file_info;

file_info *save_to_info(struct stat*, const char*);
int save_to_list(const char *,d_list *);
char *uid_str(uid_t);
char *gid_str(gid_t);
char *mode_str(int, char *);
void free_node(pnode);
void print_info(const void*);

int main(int argc, char *argv[])
{
	d_list *plist = init_list();
	if(1 == argc)
		save_to_list(".",plist);
	else
	{
		int i;
		for(i=1;i<argc;i++)
		{
			if('-' == argv[i][0] && isalpha(argv[i][1]))
				continue;
			else
			    save_to_list(argv[i],plist);
		}
	}
	list_traverse(plist,print_info);
	destroy_list(plist,free_node);
	return 0;
}

file_info *save_to_info(struct stat* pst,const char *name)
{
	file_info *pinfo= (file_info*)malloc(sizeof(file_info));
	char *str = (char*)malloc(strlen(name)+1);
	strcpy(str,name);
	pinfo->uid   = pst->st_uid;
	pinfo->gid   = pst->st_gid;
	pinfo->nlink = pst->st_nlink;
	pinfo->size  = pst->st_size;
	pinfo->mtime = pst->st_mtime;
	pinfo->mode = pst->st_mode;
	pinfo->name  = str;
	return pinfo;
}
int save_to_list(const char *path, d_list *plist)
{
	char fpath[4096];
	struct stat *pst = (struct stat*)malloc(sizeof(struct stat));
	if(NULL == pst)
	{
		perror("malloc");
		return ERROR;
	}
	if(-1 == stat(path,pst))
	{
		perror("stat");
		return ERROR;
	}
	if(S_ISDIR(pst->st_mode))
	{
		DIR *pd;
		struct dirent *pdir;
		if(NULL == (pd = opendir(path)))
		{
			perror("opendir");
			return ERROR;
		}
		while(NULL != (pdir = readdir(pd)))
		{
			sprintf(fpath,"%s/%s",path,pdir->d_name);
			if(NULL == pst)
			{
				perror("malloc");
				return ERROR;
			}
			if(-1 == stat(fpath,pst))
            {
				perror("stat");
				return ERROR;
			}
			file_info *pinfo = save_to_info(pst,pdir->d_name);
			ins_after(plist,get_head(plist),make_node(pinfo));
		}
		closedir(pd);
		free(pst);
		return OK; 
	}
	else
	{
		file_info *pinfo = save_to_info(pst,path);
		ins_after(plist,get_head(plist),make_node(pinfo));
		free(pst);
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

void free_node(pnode p)
{
	file_info *pinfo = (file_info*)(p->data);
	free(pinfo->name);
	free(pinfo);
	free(p);
}
void print_info(const void *p)
{
	file_info *pinfo = (file_info*)p;
	char str[11];
	char time_str[25];
	snprintf(time_str,24,"%s",ctime(&pinfo->mtime)),
	time_str[24] = '\0';
	printf("%s %2d %s %s %6d %.12s %s\n",mode_str(pinfo->mode,str),
			                    (int)pinfo->nlink,
			                    uid_str(pinfo->uid),
			                    gid_str(pinfo->gid),
						        (int)pinfo->size,
								4 + ctime(&pinfo->mtime),
								pinfo->name
								);
	return;
}

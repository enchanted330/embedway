/*************************************************************************
    > File Name: myls.h
    > Author: 
    > Mail: 
    > Created Time: Mon 17 Jul 2017 07:56:47 PM PDT
 ************************************************************************/
#ifndef MYLS_H
#define MYLS_H

#include "dlist.h"
#include <sys/stat.h>

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
int  save_to_list(const char *,d_list*);
char *uid_str(uid_t);
char *gid_str(gid_t);
char *mode_str(int, char*);
int  str_cmp(const void*, const void*);
void free_node(pnode);
void print(const void*);
void print_info(file_info*);
void print_name(file_info*);
#endif

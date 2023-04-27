#ifndef _SHELL_HD_
#define _SHELL_HD_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* Def for read/write buffers */
#define RD_BUFF_SIZE 1024
#define WT_BUFF_SIZE 1024
#define FLUSH_BUF -1

#define _CMD_NRM	0
#define CD_OR		1
#define CD_AND		2
#define CD_CHAIN	3


/* Def for convert_num() */
#define CONV_LC	1
#define CONV_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GTLINE 0
#define USE_STROK 0

#define FILE_HST	".simple_shell_history"
#define MAX_HST	4096

extern char **environ;


/**
 * struct lststring - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct lststring
{
	int num;
	char *str;
	struct lststring *next;
} list_t;

/**
 *struct pass_info - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@d_path: a string d_path for the current command
 *@argc: the argument count
 *@count_line: the error count
 *@error_numb: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_bufr: address of pointer to cmd_bufr, on if chaining
 *@cmd_bufr_typ: CMD_type ||, &&, ;
 *@read_fd: the fd from which to read line input
 *@count_hist: the history line number count
 */
typedef struct pass_info
{
	char *arg;
	char **argv;
	char *d_path;
	int argc;
	unsigned int count_line;
	int error_numb;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_bufr; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_bufr_typ; /* CMD_type ||, &&, ; */
	int read_fd;
	int count_hist;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}


/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

int hsh(info_t *, char **);
int find_builtin(info_t *);
void fnd_cmd(info_t *);
void fork_cmd(info_t *);

int is_cmd(info_t *, char *);
char *dup_chars(char *, int, int);
char *fnd_path(info_t *, char *, char *);

int loophsh(char **);

void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);


char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);


char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);


char **strtow(char *, char *);
char **strtow2(char *, char);


char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);


int bfree(void **);


int sh_interactive(info_t *);
int is_delimeter(char, char *);
int chk_isalph(int);
int _atoi(char *);


int _erratoi(char *);
void print_err(info_t *, char *);
int print_d(int, int);
char *convert_num(long int, int, int);
void rm_comments(char *);


int _myexit(info_t *);
int _mycd(info_t *);
int _myhelp(info_t *);


int _myhistory(info_t *);
int _myalias(info_t *);


ssize_t get_input(info_t *);
int _getline(info_t *, char **, size_t *);
void sigintHandler(int);


void clear_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);


char *_getenv(info_t *, const char *);
int _myenv(info_t *);
int _mysetenv(info_t *);
int _myunsetenv(info_t *);
int populate_env_list(info_t *);


char **get_environ(info_t *);
int _unsetenv(info_t *, char *);
int _setenv(info_t *, char *, char *);


char *get_history_file(info_t *_Info_);
int write_history(info_t *_Info_);
int read_history(info_t *_Info_);
int build_history_list(info_t *_Info_, char *buf, int linecount);
int re_num_hist(info_t *_Info_);


list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);


size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

int is_chain(info_t *, char *, size_t *);
void check_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int replace_vars(info_t *);
int replace_str(char **, char *);


#endif

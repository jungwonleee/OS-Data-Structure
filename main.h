#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "list.h"
#include "hash.h"
#include "bitmap.h"
#include "limits.h"
#include "round.h"
#include "debug.h"

struct list_item {
	struct list_elem elem;
	int data;
};

struct LIST {
	struct list list;
	char listname[20];
};

struct hash_item {
	struct hash_elem elem;
	int data;
};

struct HASH {
	struct hash hash;
	char hashname[20];
};

struct BITMAP {
	struct bitmap *bitmap;
	char bitmapname[20];
};

struct LIST lists[10];
struct HASH hashs[10];
struct BITMAP bitmaps[10];

int listnum=0;
int hashnum=0;
int bitmapnum=0;

bool llf(const struct list_elem *a, const struct list_elem *b, void *aux);
void list_shuffle(struct list *list);
unsigned hhf(const struct hash_elem *e, void *aux);
bool hlf(const struct hash_elem *a, const struct hash_elem *b, void *aux);
void destructor(struct hash_elem *e, void *aux);
void square(struct hash_elem *e, void *aux);
void triple(struct hash_elem*e, void *aux);

int command(char *cmd);
void create(char *arg1, char *arg2, char *arg3);
void delete(char *arg1);
void dumpdata(char *arg1);
int getindex(int *type, char *name);
void listcmd(char *cmd, char *arg1, char *arg2, char *arg3, char *arg4, char *arg5);
void hashcmd(char *cmd, char *arg1, char *arg2, char *arg3, char *arg4, char *arg5);
void bitmapcmd(char *cmd, char *arg1, char *arg2, char *arg3, char *arg4, char *arg5);

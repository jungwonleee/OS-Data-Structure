#include "main.h"

int main(void) {
	char str[100];
	char cmd[20], arg1[20], arg2[20], arg3[20], arg4[20], arg5[20];
	int n, cmd_num;

	while (true) {
		strcpy(cmd, "");
		strcpy(arg1, "");
		strcpy(arg2, "");
		strcpy(arg3, "");
		fgets(str, sizeof(str), stdin);
		n=sscanf(str, "%s %s %s %s %s %s", cmd, arg1, arg2, arg3, arg4, arg5); 
		if (n==1&&strcmp(cmd, "quit")==0) break;
		cmd_num=command(cmd);
		switch(cmd_num) {
			case 1: create(arg1, arg2, arg3); break;
			case 2: delete(arg1); break;
			case 3: dumpdata(arg1); break;
			case 4: listcmd(cmd, arg1, arg2, arg3, arg4, arg5); break;
			case 5: hashcmd(cmd, arg1, arg2, arg3, arg4, arg5); break;
			case 6: bitmapcmd(cmd, arg1, arg2, arg3, arg4, arg5); break;
			default: printf("유효하지 않은 명령어입니다.\n");
		}
	}
	return 0;
}

int command(char *cmd) {
	char *s, copy[20];
	
	if (strcmp(cmd, "create")==0) return 1;
	if (strcmp(cmd, "delete")==0) return 2;
	if (strcmp(cmd, "dumpdata")==0) return 3;
	strcpy(copy, cmd);
	s=strtok(copy, "_");
	if (strcmp(s, "list")==0) return 4;
	if (strcmp(s, "hash")==0) return 5;
	if (strcmp(s, "bitmap")==0) return 6;
	else return 0;
}

void create(char *arg1, char *arg2, char *arg3) {
	if (strcmp(arg1, "list")==0) {
		strcpy(lists[listnum].listname, arg2);
		list_init(&(lists[listnum].list));
		listnum++;
	}
	else if (strcmp(arg1, "hashtable")==0) {
		strcpy(hashs[hashnum].hashname, arg2);
		hash_init(&(hashs[hashnum].hash), hhf, hlf, NULL);
		hashnum++;
	}
	else if (strcmp(arg1, "bitmap")==0) {
		strcpy(bitmaps[bitmapnum].bitmapname, arg2);
		bitmaps[bitmapnum].bitmap=bitmap_create(atoi(arg3));
		bitmapnum++;
	}
	else printf("유효하지 않은 자료구조입니다.\n");
}

void delete(char *arg1) {
	int i, j, id, type=0;
	struct list_elem *temp;

	id=getindex(&type, arg1);

	if (type==1) {
		for (j=id; j<listnum-1; j++) {
			lists[j]=lists[j+1];
		}
		strcpy(lists[listnum-1].listname, "");
		listnum--;
		return;
	}

	if (type==2) {
		for (j=id; j<hashnum-1; j++) {
			hashs[j]=hashs[j+1];
		}
		hash_clear(&(hashs[hashnum-1].hash), NULL);
		strcpy(hashs[hashnum-1].hashname, "");
		hashnum--;
		return;
	}
		
	if (type==3) {
		for (j=id; j<bitmapnum-1; j++) {
			bitmaps[j]=bitmaps[j+1];
		}
		bitmap_destroy(bitmaps[bitmapnum-1].bitmap);
		strcpy(bitmaps[bitmapnum-1].bitmapname, "");
		bitmapnum--;
		return;
	}
	printf("%s (이)라는 자료구조는 존재하지 않습니다.\n");
}

void dumpdata(char *arg1) {
	int i, j, id, type=0;
	struct list_elem *p;
	struct hash_iterator it;

	id=getindex(&type, arg1);

	if (type==1) {
		for (p=list_begin(&(lists[id].list)); p!=list_end(&(lists[id].list)); p=list_next(p)) {
			printf("%d ", list_entry(p, struct list_item, elem)->data);
		}
		printf("\n");
		return;
	}

	if (type==2) {
		hash_first(&it, &(hashs[id].hash));
		while (hash_next(&it)) {
			printf("%d ", hash_entry(hash_cur(&it), struct hash_item, elem)->data);
		}
		printf("\n");
		return;
	}

	if (type==3) {
		for (j=0; j<(int)bitmap_size(bitmaps[id].bitmap); j++) {
			printf("%d", bitmap_test(bitmaps[id].bitmap, j));
		}
		printf("\n");
		return;
	}
	printf("%s (이)라는 자료구조는 존재하지 않습니다.\n");
}

int getindex(int *type, char *name) {
	int i;

	if (*type==0 || *type==1) {
		for (i=0; i<listnum; i++) {
			if (strcmp(lists[i].listname, name)==0) {
				if (*type==0) *type=1;
				return i;
			}
		}
	}

	if (*type==0 || *type==2) {
		for (i=0; i<hashnum; i++) {
			if (strcmp(hashs[i].hashname, name)==0) {
				if (*type==0) *type=2;
				return i;
			}
		}
	}

	if (*type==0 || *type==3) {
		for (i=0; i<bitmapnum; i++) {
			if (strcmp(bitmaps[i].bitmapname, name)==0) {
				if (*type==0) *type=3;
				return i;
			}
		}
	}
	return -1;
}

bool llf(const struct list_elem *a, const struct list_elem *b, void *aux) {
	if ((list_entry(a, struct list_item, elem)->data) < (list_entry(b, struct list_item, elem)->data)) return true;
	else return false;
}

void list_shuffle(struct list *list) {
	int i, j, a, b, temp;
	struct list_elem *p, *q;
	
	size_t size=list_size(list);
	srand((unsigned int)time(NULL));

	for (i=0; i<100; i++) {
		a=rand()%size;
		b=rand()%size;
		if (a>b) {
			temp=a;
			a=b;
			b=temp;
		}
		p=list_begin(list);
		for (j=0; j<a; j++) p=list_next(p);
		q=list_begin(list);
		for (j=0; j<b; j++) q=list_next(q);
		list_swap(p, q);
	}
}

void listcmd(char *cmd, char *arg1, char *arg2, char *arg3, char *arg4, char *arg5) {
	int id, id2, type=1, i;
	struct list_item *temp;
	struct list_elem *p, *q, *first, *last;
	
	if (strcmp(cmd, "list_insert")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			temp=(struct list_item*)malloc(sizeof(struct list_item));
			temp->data=atoi(arg3);
			p=list_begin(&(lists[id].list));
			for (i=0; i<atoi(arg2); i++) p=list_next(p);
			list_insert(p, &(temp->elem));
			return;
		}
	}

	if (strcmp(cmd, "list_splice")==0) {
		id=getindex(&type, arg1);
		id2=getindex(&type, arg3);
		if (id>=0 && id2>=0) {
			p=list_begin(&(lists[id].list));
			for (i=0; i<atoi(arg2); i++) p=list_next(p);
			first=list_begin(&(lists[id2].list));
			for (i=0; i<atoi(arg4); i++) first=list_next(first);
			last=list_begin(&(lists[id2].list));
			for (i=0; i<atoi(arg5); i++) last=list_next(last);
			list_splice(p, first, last);
			return;
		}
	}		

	if (strcmp(cmd, "list_push_front")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			temp=(struct list_item*)malloc(sizeof(struct list_item));
			temp->data=atoi(arg2);
			list_push_front(&(lists[id].list), &(temp->elem));
			return;
		}
	}

	if (strcmp(cmd, "list_push_back")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			temp=(struct list_item*)malloc(sizeof(struct list_item));
			temp->data=atoi(arg2);
			list_push_back(&(lists[id].list), &(temp->elem));
			return;
		}
	}
	
	if (strcmp(cmd, "list_remove")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			p=list_begin(&(lists[id].list));
			for (i=0; i<atoi(arg2); i++) p=list_next(p);
			list_remove(p);
			return;
		}
	}

	if (strcmp(cmd, "list_pop_front")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			list_pop_front(&(lists[id].list));
			return;
		}
	}

	if (strcmp(cmd, "list_pop_back")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			list_pop_back(&(lists[id].list));
			return;
		}
	}

	if (strcmp(cmd, "list_front")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			p=list_front(&(lists[id].list));
			printf("%d\n",  list_entry(p, struct list_item, elem)->data);
			return;
		}
	}
	
	if (strcmp(cmd, "list_back")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			p=list_back(&(lists[id].list));
			printf("%d\n", list_entry(p, struct list_item, elem)->data);
			return;
		}
	}

	if (strcmp(cmd, "list_size")==0) {
		id=getindex(&type, arg1);
		if (id>=0) printf("%d\n", (int)list_size(&(lists[id].list)));
		return;
	}

	if (strcmp(cmd, "list_empty")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			if (list_empty(&(lists[id].list))) printf("true\n");
			else printf("false\n");
			return;
		}
	}

	if (strcmp(cmd, "list_reverse")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			list_reverse(&(lists[id].list));
			return;
		}
	}

	if (strcmp(cmd, "list_sort")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			list_sort(&(lists[id].list), llf, NULL);
			return;
		}
	}

	if (strcmp(cmd, "list_insert_ordered")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			temp=(struct list_item*)malloc(sizeof(struct list_item));
			temp->data=atoi(arg2);
			list_insert_ordered(&(lists[id].list), &(temp->elem), llf, NULL);
			return;
		}
	}

	if (strcmp(cmd, "list_unique")==0) {
		id=getindex(&type, arg1);
		id2=getindex(&type, arg2);
		if (id>=0) {
			if (id2>=0) list_unique(&(lists[id].list), &(lists[id2].list), llf, NULL);
			else list_unique(&(lists[id].list), NULL, llf, NULL);
			return;
		}
	}

	if (strcmp(cmd, "list_max")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			p=list_max(&(lists[id].list), llf, NULL);
			printf("%d\n", list_entry(p, struct list_item, elem)->data);
			return;
		}
	}

	if (strcmp(cmd, "list_min")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			p=list_min(&(lists[id].list), llf, NULL);
			printf("%d\n", list_entry(p, struct list_item, elem)->data);
			return;
		}
	}

	if (strcmp(cmd, "list_swap")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			p=list_begin(&(lists[id].list));
			for (i=0; i<atoi(arg2); i++) p=list_next(p);
			q=list_begin(&(lists[id].list));
			for (i=0; i<atoi(arg3); i++) q=list_next(q);
			list_swap(p, q);
			return;
		}
	}

	if (strcmp(cmd, "list_shuffle")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			list_shuffle(&(lists[id].list));
			return;
		}
	}
}

unsigned hhf(const struct hash_elem *e, void *aux) {
	return hash_int(hash_entry(e, struct hash_item, elem)->data);
}

bool hlf(const struct hash_elem *a, const struct hash_elem *b, void *aux) {
	if ((hash_entry(a, struct hash_item, elem)->data) < (hash_entry(b, struct hash_item, elem)->data)) return true;
	else return false;
}

void destructor(struct hash_elem *e, void *aux) {
	free (hash_entry(e, struct hash_item, elem));
}

void square(struct hash_elem *e, void *aux) {
	int a;
	a=hash_entry(e, struct hash_item, elem)->data;
	hash_entry(e, struct hash_item, elem)->data=a*a;
}

void triple(struct hash_elem*e, void *aux) {
	int a;
	a=hash_entry(e, struct hash_item, elem)->data;
	hash_entry(e, struct hash_item, elem)->data=a*a*a;
}

void hashcmd(char *cmd, char *arg1, char *arg2, char *arg3, char *arg4, char *arg5) {
	int type=2, id;
	struct hash_item* temp;
	
	if (strcmp(cmd, "hash_insert")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			temp=(struct hash_item*)malloc(sizeof(struct hash_item));
			temp->data=atoi(arg2);
			hash_insert(&(hashs[id].hash), &(temp->elem));
			return;
		}
	}

	if (strcmp(cmd, "hash_replace")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			temp=(struct hash_item*)malloc(sizeof(struct hash_item));
			temp->data=atoi(arg2);
			hash_replace(&(hashs[id].hash), &(temp->elem));
			return;
		}
	}

	if (strcmp(cmd, "hash_find")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			temp=(struct hash_item*)malloc(sizeof(struct hash_item));
			temp->data=atoi(arg2);
			if (hash_find(&(hashs[id].hash), &(temp->elem))) printf("%d\n", temp->data);
			return;
		}
	}

	if (strcmp(cmd, "hash_delete")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			temp=(struct hash_item*)malloc(sizeof(struct hash_item));
			temp->data=atoi(arg2);
			hash_delete(&(hashs[id].hash), &(temp->elem));
			return;
		}
	}
	
	if (strcmp(cmd, "hash_clear")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			hash_clear(&(hashs[id].hash), destructor);
			return;
		}
	}

	if (strcmp(cmd, "hash_size")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			printf("%d\n", (int)hash_size(&(hashs[id].hash)));
			return;
		}
	}

	if (strcmp(cmd, "hash_empty")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			if (hash_empty(&(hashs[id].hash))) printf("true\n");
			else printf("false\n");
			return;
		}
	}

	if (strcmp(cmd, "hash_apply")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			if (strcmp(arg2, "square")==0) hash_apply(&(hashs[id].hash), square);
			if (strcmp(arg2, "triple")==0) hash_apply(&(hashs[id].hash), triple);
			return;
		}
	}
}

void bitmapcmd(char *cmd, char *arg1, char *arg2, char *arg3, char *arg4, char *arg5) {
	int type=3, id;
	bool value;

	if (strcmp(cmd, "bitmap_size")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			printf("%d\n", (int)bitmap_size(bitmaps[id].bitmap));
			return;
		}
	}

	if (strcmp(cmd, "bitmap_set")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			if (strcmp(arg3, "true")==0) value=true;
			else value=false;
			bitmap_set(bitmaps[id].bitmap, atoi(arg2), value);
			return;
		}
	}

	if (strcmp(cmd, "bitmap_mark")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			bitmap_mark(bitmaps[id].bitmap, atoi(arg2));
			return;
		}
	}

	if (strcmp(cmd, "bitmap_reset")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			bitmap_reset(bitmaps[id].bitmap, atoi(arg2));
			return;
		}
	}

	if (strcmp(cmd, "bitmap_flip")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			bitmap_flip(bitmaps[id].bitmap, atoi(arg2));
			return;
		}
	}

	if (strcmp(cmd, "bitmap_test")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			if (bitmap_test(bitmaps[id].bitmap, atoi(arg2))==true) printf("true\n");
			else printf("false\n");
			return;
		}
	}

	if (strcmp(cmd, "bitmap_set_all")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			if (strcmp(arg2, "true")==0) value=true;
			else value=false;
			bitmap_set_all(bitmaps[id].bitmap, value);
			return;
		}
	}

	if (strcmp(cmd, "bitmap_set_multiple")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			if (strcmp(arg4, "true")==0) value=true;
			else value=false;
			bitmap_set_multiple(bitmaps[id].bitmap, atoi(arg2), atoi(arg3), value);
			return;
		}
	}

	if (strcmp(cmd, "bitmap_count")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			if (strcmp(arg4, "true")==0) value=true;
			else value=false;
			printf("%d\n", bitmap_count(bitmaps[id].bitmap, atoi(arg2), atoi(arg3), value));
			return;
		}
	}
	
	if (strcmp(cmd, "bitmap_contains")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			if (strcmp(arg4, "true")==0) value=true;
			else value=false;
			if (bitmap_contains(bitmaps[id].bitmap, atoi(arg2), atoi(arg3), value)) printf("true\n");
			else printf("false\n");
			return;
		}
	}

	if (strcmp(cmd, "bitmap_any")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			if (bitmap_any(bitmaps[id].bitmap, atoi(arg2), atoi(arg3))) printf("true\n");
			else printf("false\n");
			return;
		}
	}

	if (strcmp(cmd, "bitmap_none")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			if (bitmap_none(bitmaps[id].bitmap, atoi(arg2), atoi(arg3))) printf("true\n");
			else printf("false\n");
			return;
		}
	}

	if (strcmp(cmd, "bitmap_all")==0) {
		id=getindex(&type, arg1);
		if (id>=0) {
			if (bitmap_all(bitmaps[id].bitmap, atoi(arg2), atoi(arg3))) printf("true\n");
			else printf("false\n");
			return;
		}
	}

	if (strcmp(cmd, "bitmap_scan")==0) {
		id=getindex(&type, arg1);
		if (strcmp(arg4, "true")==0) value=true;
		else value=false;
		if (id>=0) {
			printf("%u\n", bitmap_scan(bitmaps[id].bitmap, atoi(arg2), atoi(arg3), value));
			return;
		}
	}

	if (strcmp(cmd, "bitmap_scan_and_flip")==0) {
		id=getindex(&type, arg1);
		if (strcmp(arg4, "true")==0) value=true;
		else value=false;
		if (id>=0) {
			printf("%u\n", bitmap_scan_and_flip(bitmaps[id].bitmap, atoi(arg2), atoi(arg3), value));
			return;
		}
	}
	
	if (strcmp(cmd, "bitmap_dump")==0) {
		id=getindex(&type, arg1);
		if (id>=0) bitmap_dump(bitmaps[id].bitmap);
		return;
	}

	if (strcmp(cmd, "bitmap_expand")==0) {
		id=getindex(&type, arg1);
		if (id>=0) bitmap_expand(bitmaps[id].bitmap, atoi(arg2));
		return;
	}
}

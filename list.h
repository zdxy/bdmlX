#ifndef LIST_H
#define LIST_H

// 链表节点的数据结构定义
typedef struct node {
	// 节点的数据域指针，之所以定义为void类型，主要是考虑到该节点的数据类型还不确定
	void* val;
	// 下一个节点的指针
	struct node* next;
	// 上一个节点的指针
	struct node* prev;
} node;

// 双向链表头指针节点数据结构定义
typedef struct list {
    // 节点个数
    int size;
    // 头指针
    node* front;
    // 尾指针
    node* back;
} list;

#ifdef __cplusplus
extern "C" {
#endif
list *make_list();
int list_find(list *l, void *val);

void list_insert(list *, void *);

void **list_to_array(list *l);

void free_list_val(list *l);
void free_list(list *l);
void free_list_contents(list *l);
void free_list_contents_kvp(list *l);

#ifdef __cplusplus
}
#endif
#endif

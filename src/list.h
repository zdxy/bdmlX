#ifndef LIST_H
#define LIST_H

// ����ڵ�����ݽṹ����
typedef struct node {
	// �ڵ��������ָ�룬֮���Զ���Ϊvoid���ͣ���Ҫ�ǿ��ǵ��ýڵ���������ͻ���ȷ��
	void* val;
	// ��һ���ڵ��ָ��
	struct node* next;
	// ��һ���ڵ��ָ��
	struct node* prev;
} node;

// ˫������ͷָ��ڵ����ݽṹ����
typedef struct list {
    // �ڵ����
    int size;
    // ͷָ��
    node* front;
    // βָ��
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

#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "utils.h"
#include "option_list.h"

/*
                                        list.c文件说明
    一、所依赖的数据结构
        链表节点的数据结构定义
        typedef struct node {	        
	        void* val;          节点的数据域指针，之所以定义为void类型，主要是考虑到该节点的数据类型还不确定
	        struct node* next;	下一个节点的指针
	        struct node* prev;  上一个节点的指针
        } node;

        // 双向链表头指针节点数据结构定义
        typedef struct list {
            int size;           链表的节点个数
            node* front;        链表的第一个节点指针
            node* back;         链表的尾指针
        } list;
        注意：基于数据结构的基本我们知道，一个链表是由指向链表的指针所表示的，如 list *L,则L指向了这个链表，
              但通过这个L只是知道了链表的头节点，头节点中必须要有对应的指针域找到存放链表节点链的第一个节点，
              在list中使用node *类型的front指向，说明了L所指向链表的节点类型只能为node类型的。在list中还有
              一个node *类型的back变量，该变量用于指向链表的尾节点，也就是最后一个节点。链表list中的size记录
              着链表的长度，根据对链表front和back的不同赋值，可以实现循环双向链表，结合链表的头插法和pop方法
              可以实现栈的数据结构。

    二、函数列表和简介
        1、list *make_list()                     构建并初始化双向链表，初始化链表的第一个节点和尾节点指向空，链表节点个数为0，返回链表头指针
        2、void *list_pop(list *l)               返回链表l尾节点的值，并从链表l中删除和释放掉尾节点，更新链表的尾指针指向新的尾节点
        3、void list_insert(list *l, void *val)  将val变量作为node的val域，使用尾插法插入链表l中
        4、void free_node(node *n)               释放以n节点开始的后续链表节点，在执行此函数之前需要先将节点的val域指向的内存空间释放掉
        5、void free_list_val(list *l)           释放链表l中所有节点的val域所指向的内存空间
        6、void free_list(list *l)               先释放链表l的所有存储空间，再释放链表头指针l
        7、void free_list_contents(list *l)      释放链表l中所有节点的val域所指向的内存空间，同void free_list_val(list *l)
        8、void free_list_contents_kvp(list *l)  释放链表l中所有节点的val域中kvp节点数据类型的key和val键值对，具体见函数说明
        9、void **list_to_array(list *l)         将链表l中的节点内容组织成二维数组并返回
*/

/**************************************************************
*  brief：创建双向链表头节点，返回链表头指针                  *
*  details：完成头节点的初始化，节点个数为0，L的front指向第   *
*           一个节点，back指向最后一个节点，初始化指向空节点  *
**************************************************************/
list *make_list()
{
    // list数据类型定义在 list.h中 定义格式如下所示
    //typedef struct node {
    //    void *val;
    //    struct node *next;
    //    struct node *prev;
    //} node;

    //typedef struct list {
    //    int size;
    //    node *front;
    //    node *back;
    //} list;
    // 生成链表头指针，开辟头指针空间，开辟空间失败时会保存并退出
    list* l = (list*)xmalloc(sizeof(list));
    // 链表的初始节点数为0个
    l->size = 0;
    // 链表的第一个节点指针和尾节点指向空，数字0在asscil中指代NULL
    l->front = 0;
    l->back = 0;
    // 返回头节点的指针
    return l;
}

/*
void transfer_node(list *s, list *d, node *n)
{
    node *prev, *next;
    prev = n->prev;
    next = n->next;
    if(prev) prev->next = next;
    if(next) next->prev = prev;
    --s->size;
    if(s->front == n) s->front = next;
    if(s->back == n) s->back = prev;
}
*/

/********************************************************
*  brief：从链表l中取出尾节点的值返回，然后删除尾节点   *
*  args l: 欲释放的链表头指针                           *
********************************************************/
void *list_pop(list *l){
    // 如果链表的尾指针为空，则返回
    if(!l->back) return 0;
    // 找到链表尾指针所指向的尾节点
    node *b = l->back;
    // 获得尾节点的值
    void *val = b->val;
    // 找到尾节点的上一节点，作为新的尾节点
    l->back = b->prev;
    // 如果尾节点不为空，则让尾节点的下一节点指向空
    if(l->back) l->back->next = 0;
    // 释放节点
    free(b);
    // 链表长度减1
    --l->size;
    // 返回值
    return val;
}

/**************************************************************
*  brief：开辟新节点node，将val作为节点node的val域，以尾插法  *
*         插入双向链表l中，l作为引用传递改变被保留            *
*  args l: 双向链表头指针                                     *
*  args val: node节点的val域值，数据类型灵活变换              *
*  details：每次插入完成后，l的节点数量会增1                  *
**************************************************************/
void list_insert(list *l, void *val)
{
    // node数据结构定义在list.h中
    node* newnode = (node*)xmalloc(sizeof(node));
    // 节点的值域指向val，节点的next域和prev域都应该为空才完善，不过后面指定了
    // 从节点的next域始终为NULL就可以看出此处使用的是尾插法
    newnode->val = val;
    // 由此可以看出该链表不是循环链表
    newnode->next = 0;

    // 下面代码就是经典的双向链表初始化的步骤
    // 当链表中没有节点时，第一个插入节点需要特殊处理
    if(!l->back){
        l->front = newnode;
        newnode->prev = 0;
    }else{
        l->back->next = newnode;
        newnode->prev = l->back;
    }
    // 在尾插法中，插入节点后要记得修改头节点的尾指针
    l->back = newnode;
    // 每插入一个节点，计数加1，实现节点计数
    ++l->size;
}

/**************************************************
*  brief：释放从节点n开始至最后的链表的所有节点   *
*         调用此函数有一个前提条件，那就是必须先  *
*         释放掉node节点的val域所指向的空间，否   *
*         则会造成内存泄漏                        *
*  args n: 开始释放的节点指针                     *
*  details：可以从链表的中间节点开始释放，据n而定 *
**************************************************/
void free_node(node *n)
{
    node *next;
    // free函数为系统库函数，释放由malloc或alloc等动态
    // 申请的内存等空间，在释放当前节点之前需要中间变量保存下一个节点的指针
    while(n) {
        // 先获取到下一个节点的地址，不然直接调用free后会使指针断开
        next = n->next;
        free(n);
        n = next;
    }
}

/************************************************
*  brief：释放链表l中每个节点node的val域        *
*         此函数和free_list_contents基本一致    *
*  args l: 欲释放的链表头指针                   *
************************************************/
void free_list_val(list *l)
{
    // 获取链表的第一个节点
    node *n = l->front;
    node *next;
    // 循环释放节点的val域
    while (n) {
        next = n->next;
        free(n->val);
        n = next;
    }
}

/************************************************
*  brief：释放链表l，先调用free_node释放所有节  *
*         点，然后释放l所指向的头指针的空间     *
*  args l: 欲释放的链表头指针                   *
************************************************/
void free_list(list *l)
{
    // free_node函数：本文件定义。释放链表中的节点node。
    free_node(l->front);
    // 释放完每一个节点后再释放头指针的空间
    free(l);
}

/*********************************************************
*  brief：释放链表l中所有node val域的空间注意并没有释放  *
*         node节点。此函数限定了node节点的val域是简单的  *
*         数据类型的指针，以便能直接free                 *
*  args l: 欲操作的链表头指针                            *
*********************************************************/
void free_list_contents(list *l)
{
    // 找到双向链表的第一个节点
    node *n = l->front;
    // 循环处理链表的每一个节点
    while(n){
        // 此处的处理有点草率，如果Node的val域不是简单的指针类型，而是复杂的数据结构
        // 那么直接调用free是有问题的.当然，此函数也可以作为专用函数，专门释放针对
        // node.val是简单数据的是没有问题的。作者也正是这么个意图
        free(n->val);
        n = n->next;
    }
}

/******************************************************
*  brief：释放链表中的节点的数据信息，本函数限定了节  *
*         点的val域必须指向kvp类型的数据。注意此函数  *
*         只是释放了节点node中val域所指kvp的key和val  *
*         甚至连kvp节点都没有进行释放                 *
*  args l: 欲释放的链表头指针                         *
******************************************************/
void free_list_contents_kvp(list *l)
{
    // 找到双向链表的第一个节点，数据类型为node类型
    node *n = l->front;
    // 循环处理链表中的每一个节点
    while (n) {
        // 读取节点的val域，此时节点val域指向kvp类型的指针
        kvp* p = (kvp*)n->val;
        // 释放kvp结构体指向的key和val，这两者一般为一对键值对
        free(p->key);
        free(n->val);
        // 循环指向下一个节点
        n = n->next;
    }
}

/*************************************************
*  brief：将链表l中的节点的val提取组织成数组     *
*  args l: 欲提取的链表头指针                    *
*  details：node的val域类型为void，根据实际而定  *
*************************************************/
void **list_to_array(list *l)
{
    // l->size记录了双向链表中的节点个数，在此是为每一个节点开辟一个void *类型的存储空间
    void** a = (void**)xcalloc(l->size, sizeof(void*));
    int count = 0;
    // 找到双向链表的第一个节点
    node *n = l->front;
    // 循环处理双向链表的节点
    while(n){
        // n的数据类型为node,node中包含一个void *类型的val成员变量，这个成员变量的类型不定。但通常在调用
        // 此函数时，该成员变量的实际值一般为字符串
        // 挨个处理，a中每一个元素均为char类型的指针，所以a为二级指针
        a[count++] = n->val;
        n = n->next;
    }
    // 返回转换好的二维数组的指针。注意需要在函数调用处进行强制数据类型转换
    return a;
}
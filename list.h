#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>

#define LIST_START_HEAD 0
#define LIST_START_TAIL 1

typedef struct listNode {
  struct listNode *prev;
  struct listNode *next;
  void *value;
} listNode;

typedef struct listIterator {
  listNode *next;
  int direction;
} listIterator;

typedef struct list {
  listNode *head;
  listNode *tail;
  void (*free)(void *value);
  int (*equal)(void *value, void *comparedValue);
  size_t len;
} list;

#define listLengthGet(l) ((l)->len)

#define listNodeGetHead(l) ((l)->head)
#define listNodeGetTail(l) ((l)->tail)
#define listNodeGetPrev(n) ((n)->prev)
#define listNodeGetNext(n) ((n)->next)
#define listNodeGetValue(n) ((n)->value)

#define listMethodSetFree(l, m) ((l)->free = (m))
#define listMethodSetEqual(l, m) ((l)->equal = (m))

list *listCreate(void);
void listDestroy(list *list);

listNode *listNodeAddHead(list *list, void *value);
listNode *listNodeAddTail(list *list, void *value);
void listNodeDestroy(list *list, listNode *node);

listIterator *listIteratorCreate(list *list, int direction);
void listIteratorDestroy(listIterator *iter);
listNode *listIteratorNext(listIterator *iter);

void listIteratorRewind(list *list, listIterator *iter);
void listIteratorRewindReverse(list *list, listIterator *iter);

listNode *listNodeFind(list *list, void *comparedValue);

#endif // __LIST_H__

#include <stdlib.h>
#include <assert.h>

#include "list.h"

list *listCreate(void) {
  struct list *list;

  if ((list = malloc(sizeof(*list))) == NULL)
    return NULL;
  list->head = list->tail = NULL;
  list->len = 0;
  list->free = NULL;
  list->equal = NULL;
  return list;
}

void listDestroy(list *list) {
  assert(list);

  size_t len;
  listNode *current, *next;

  current = list->head;
  len = list->len;
  while (len--) {
    next = current->next;
    if (list->free)
      list->free(current->value);
    free(current);
    current = next;
  }

  free(list);
}

listNode *listNodeAddHead(list *list, void *value) {
  assert(list);
  assert(value);

  listNode *node;

  if ((node = malloc(sizeof(*node))) == NULL)
    return NULL;

  node->value = value;

  if (list->len == 0) {
    list->head = list->tail = node;
    node->prev = node->next = NULL;
  } else {
    node->prev = NULL;
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
  }
  list->len++;

  return node;
}

listNode *listNodeAddTail(list *list, void *value) {
  assert(list);
  assert(value);

  listNode *node;

  if ((node = malloc(sizeof(*node))) == NULL)
    return NULL;

  node->value = value;

  if (list->len == 0) {
    list->head = list->tail = node;
    node->prev = node->next = NULL;
  } else {
    node->prev = list->tail;
    node->next = NULL;
    list->tail->next = node;
    list->tail = node;
  }
  list->len++;

  return node;
}

void listNodeDestroy(list *list, listNode *node) {
  assert(list);
  assert(node);

  if (node->prev)
    node->prev->next = node->next;
  else
    list->head = node->next;
  if (node->next)
    node->next->prev = node->prev;
  else
    list->tail = node->prev;

  if (list->free)
    list->free(node->value);

  free(node);

  list->len--;
}

listIterator *listIteratorCreate(list *list, int direction) {
  assert(list);
  assert(direction == LIST_START_HEAD || direction == LIST_START_TAIL);

  listIterator *iter;

  if ((iter = malloc(sizeof(*iter))) == NULL)
    return NULL;

  if (direction == LIST_START_HEAD)
    iter->next = list->head;
  else
    iter->next = list->tail;
  iter->direction = direction;

  return iter;
}

void listIteratorDestroy(listIterator *iter) {
  assert(iter);

  free(iter);
}

listNode *listIteratorNext(listIterator *iter) {
  listNode *current = iter->next;

  if (current != NULL) {
    if (iter->direction == LIST_START_HEAD)
      iter->next = current->next;
    else
      iter->next = current->prev;
  }
  return current;
}

void listIteratorRewind(list *list, listIterator *iter) {
  iter->next = list->head;
  iter->direction = LIST_START_HEAD;
}

void listIteratorRewindReverse(list *list, listIterator *iter) {
  iter->next = list->tail;
  iter->direction = LIST_START_TAIL;
}

listNode *listNodeFind(list *list, void *comparedValue) {
  assert(list);
  assert(comparedValue);
  assert(list->equal);

  listIterator iter;
  listNode *node;

  listIteratorRewind(list, &iter);
  while ((node = listIteratorNext(&iter)) != NULL) {
    if (list->equal(node->value, comparedValue)) {
      return node;
    }
  }

  return NULL;
}

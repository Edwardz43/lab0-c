#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

static void merge_sort(list_ele_t **headRef);
static void front_back_split(list_ele_t *source,
                             list_ele_t **frontRef,
                             list_ele_t **backRef);
/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q || sizeof(queue_t) == 0)
        return NULL;
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q) {
        return;
    }
    for (list_ele_t *cur = q->head; cur;) {
        list_ele_t *tmp = cur;
        cur = cur->next;
        free(tmp->value);
        free(tmp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    newh->value = malloc(sizeof(char) * strlen(s) + 1);
    if (!newh->value) {
        free(newh->value);
        free(newh);
        return false;
    }
    memset(newh->value, 0, strlen(s) + 1);
    strncpy(newh->value, s, strlen(s));
    if (q->size == 0)
        q->tail = newh;

    newh->next = q->head;
    q->head = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    newt->value = malloc(sizeof(char) * strlen(s) + 1);
    if (!newt->value) {
        free(newt);
        return false;
    }
    memset(newt->value, 0, strlen(s) + 1);
    strncpy(newt->value, s, strlen(s));
    newt->next = NULL;
    if (q->size == 0) {
        q->head = newt;
    } else {
        q->tail->next = newt;
    }
    q->tail = newt;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || q->size == 0)
        return false;
    if (sp) {
        size_t cpySize = bufsize > strlen(q->head->value)
                             ? strlen(q->head->value)
                             : bufsize - 1;
        strncpy(sp, q->head->value, cpySize);
        sp[cpySize] = '\0';
    }
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    if (!q->head)
        q->tail = NULL;
    free(tmp->value);
    free(tmp);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size == 0 || q->size == 1)
        return;
    list_ele_t *pre, *cur, *next;
    pre = NULL;
    cur = q->head;
    next = q->head->next;
    q->tail = q->head;
    while (next) {
        cur->next = pre;
        pre = cur;
        cur = next;
        next = next->next;
    }
    cur->next = pre;
    q->head = cur;
    q->tail->next = NULL;
}

/* merge sort method */
static void merge_sort(list_ele_t **headRef)
{
    list_ele_t *slow, *fast;

    if (!(*headRef) || !(*headRef)->next)
        return;
    slow = *headRef;
    fast = (*headRef)->next;

    front_back_split(*headRef, &slow, &fast);
    merge_sort(&slow);
    merge_sort(&fast);
    *headRef = NULL;
    list_ele_t **tmp = headRef;

    while (slow && fast) {
        if (strcmp(slow->value, fast->value) < 0) {
            *tmp = slow;
            slow = slow->next;
        } else {
            *tmp = fast;
            fast = fast->next;
        }
        tmp = &(*tmp)->next;
    }

    *tmp = slow ? slow : fast;
}

/* split list to two partition */
static void front_back_split(list_ele_t *source,
                             list_ele_t **frontRef,
                             list_ele_t **backRef)
{
    list_ele_t *slow = source;
    list_ele_t *fast = source->next;
    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size == 0 || q->size == 1)
        return;
    merge_sort(&q->head);
    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}
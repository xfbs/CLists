#include "dlist.h"


dlist *dlist_alloc()
{
    // allocate new list
    dlist *list = malloc(sizeof(dlist));

    // zeroize list
    memset(list, 0, sizeof(dlist));

    return list;
}


dlist *dlist_copy(dlist *orig)
{
    // new dlist to copy into
    dlist *copy = dlist_alloc();
    
    // get pointer to original node for copying
    dlist_node *orig_node = orig->head;
    // copy node over
    dlist_node *copy_node;
    if (orig_node)
        copy_node = dlist_node_copy(orig_node);
    
    // add node as head of list
    copy->head = copy_node;
    
    while (orig_node = orig_node->next) {
        // copy node
        copy_node->next = dlist_node_copy(orig_node);
        
        // insert node into list
        copy_node->next->prev = copy_node;
        
        // go to next node
        copy_node = copy_node->next;
    }
    
    // set last copied node to be the tail of the list
    copy->tail = copy_node;
    
    // copy size over
    copy->size = orig->size;
}


dlist_node *dlist_node_alloc()
{
    // allocate memory for node
    dlist_node *node = malloc(sizeof(dlist_node));

    // zeroize node
    memset(node, 0, sizeof(dlist_node));

    return node;
}


dlist_node *dlist_node_copy(dlist_node *node)
{
    // don't attempt to copy an empty node
    if(!node)
        return NULL;

    // allocate memory for the copy
    dlist_node *copy = malloc(sizeof(dlist_node));

    // copy contents over
    memcpy(copy, node, sizeof(dlist_node));

    return copy;
}


dlist_node *dlist_node_get(dlist *list, size_t pos)
{
    // return NULL if the list is empty
    if (!list->size)
        return NULL;

    // return null if pos doesn't exist in list
    if (pos >= list->size)
        return NULL;

    // deliberately uninitialized
    dlist_node *node;

    // check where the node is
    if (pos < (list->size/2)) {
        // pos is before the middle if the list,
        // thus we loop from the beginning
        node = list->head;

        while (node && pos) {
            // decrement pos
            pos--;

            // go to next node
            node = node->next;
        }
    } else {
        // pos is after the middle of the list,
        // thus we loop from the end of the list

        node = list->tail;
        pos = (list->size-pos)-1;

        while (node && pos) {
            // decrement pos
            pos--;

            // go to previous node
            node = node->prev;
        }
    }

    return node;
}


void dlist_free(dlist *list, int free_data)
{
    // remove all nodes
    dlist_purge(list, free_data);

    // free dlist
    free(list);
}


void dlist_purge(dlist *list, int free_data)
{
    // get pointer to head of list
    dlist_node *node = list->head;

    while(node)
    {
        // save data pointer for freeing later
        void *data = node->data;

        // free node
        free(node);

        // free data if asked to
        if(free_data)
            free(data);

        // traverse to next node
        node = node->next;
    }
}


void dlist_merge(dlist *dest, dlist *source)
{
    // can't copy from source if source is empty
    if(!source->size)
        return;

    // if dest is empty, just copy the memory over
    if(!dest->size) {
        memcpy(dest, source, sizeof(dlist));
    } else {
        // connect dest->tail and source->head
        dest->tail->next = source->head;
        source->head->prev = dest->tail;

        // increment the size
        dest->size += source->size;
    }

    // zeroise source
    memset(source, 0, sizeof(dlist));
}


void dlist_append(dlist *list, void *data)
{
    // the ndoe that needs to be appended
    dlist_node *node = dlist_node_alloc();

    // set data
    node->data = data;

    // check if list is empty
    if (list->size) {
        // append after current tail of list
        // set the previous node (current tail)
        node->prev = list->tail;

        // set current tail's next ndoe (this one)
        list->tail->next = node;

        // set node as new tail
        list->tail = node;
    } else {
        // set node as first item of list
        list->head = node;

        // set node as last item of list
        list->tail = node;
    }

    // increae size by one
    list->size++;
}


void dlist_push(dlist *list, void *data)
{
    // node that needs to be pushed
    dlist_node *node = dlist_node_alloc();

    // set data
    node->data = data;

    // check if list is empty
    if (list->size) {
        // set node's next to the current head of the list
        node->next = list->head;

        // set current head's prev to this node
        list->head->prev = node;

        // set this node as head
        list->head = node;
    } else {
        // set this node as the only list member
        list->head = node;
        list->tail = node;
    }

    // increase list size by one
    list->size++;
}


void dlist_insert(dlist *list, void *data, size_t pos)
{
    // if asked to insert at pos 0, use push instead
    if (pos == 0) {
        // add to top of list
        dlist_push(list, data);
    } else {
        // new node that needs to be inserted
        dlist_node *new = dlist_node_alloc();

        // set new node's data
        new->data = data;

        // get the node before
        dlist_node *node = dlist_node_get(list, pos-1);

        if (node) {
            // tell the next node about the new one
            node->next->prev = new;

            // set the next node
            new->next = node->next;

            // insert this node
            node->next = new;
        } else {
            // set this node as first node
            list->head = new;
            list->tail = new;
        }

        // increase list size
        list->size++;
    }
}


void dlist_set(dlist *list, void *data, size_t pos)
{
    // get a reference to the node that needs to be altered
    dlist_node *node = dlist_node_get(list, pos);

    // only change node if it exists
    if (node)
        node->data = data;
}


void *dlist_first(dlist *list)
{
    // get pointer to first node
    dlist_node *node = list->head;

    // return it's data or NULL
    return((node) ? node->data : NULL);
}


void *dlist_last(dlist *list)
{
    // get pointer to last node
    dlist_node *node = list->tail;

    // return it's data or NULL
    return((node) ? node->data : NULL);
}


void *dlist_get(dlist *list, size_t pos)
{
    // get the requested node
    dlist_node *node = dlist_node_get(list, pos);

    // return data or NULL
    return((node) ? node->data : NULL);
}



#include "cu/cu.h"
#include "clists/dlist.h"

static int ret;
static dlist_t *list;
static dlist_node_t *node;

TEST(freeingEmpty)
{
    list = dlist_new();
    assertNotEquals(list, NULL);

    ret = dlist_free(list);
    assertEquals(ret, 0);
}

TEST(freeingSingle)
{
    list = dlist_new();
    assertNotEquals(list, NULL);

    node = malloc(sizeof(dlist_node_t));
    assertNotEquals(node, NULL);

    memset(node, 0, sizeof(dlist_node_t));
    list->head = node;
    list->tail = node;
    list->size = 1;

    ret = dlist_free(list);
    assertEquals(ret, 0);
}

TEST(freeingFull)
{
    list = dlist_new();
    assertNotEquals(list, NULL);

    node = malloc(sizeof(dlist_node_t));
    assertNotEquals(node, NULL);

    memset(node, 0, sizeof(dlist_node_t));
    list->head = node;

    node->next = malloc(sizeof(dlist_node_t));
    assertNotEquals(node->next, NULL);
    node->next->prev = node;

    memset(node, 0, sizeof(dlist_node_t));
    list->tail = node;
    list->size = 2;

    ret = dlist_free(list);
    assertEquals(ret, 0);
}
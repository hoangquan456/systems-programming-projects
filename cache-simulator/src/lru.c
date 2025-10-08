#include "lru.h"
#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

void lru_init_queue(Set *set) {
  LRUNode *s = NULL;
  LRUNode **pp = &s;  // place to chain in the next node
  for (int i = 0; i < set->line_count; i++) {
    Line *line = &set->lines[i];
    LRUNode *node = (LRUNode *)(malloc(sizeof(LRUNode)));
    node->line = line;
    node->next = NULL;
    (*pp) = node;
    pp = &((*pp)->next);
  }
  set->lru_queue = s;
}

void lru_init(Cache *cache) {
  Set *sets = cache->sets;
  for (int i = 0; i < cache->set_count; i++) {
    lru_init_queue(&sets[i]);
  }
}

void lru_destroy(Cache *cache) {
  Set *sets = cache->sets;
  for (int i = 0; i < cache->set_count; i++) {
    LRUNode *p = sets[i].lru_queue;
    LRUNode *n = p;
    while (p != NULL) {
      p = p->next;
      free(n);
      n = p;
    }
    sets[i].lru_queue = NULL;
  }
}

void lru_fetch(Set *set, unsigned int tag, LRUResult *result) {
  // TODO:
  // Implement the LRU algorithm to determine which line in
  // the cache should be accessed.
  //
  result->access = CONFLICT_MISS;
  char flag = 0;
  for(int i = 0; i < set->line_count; ++i) {
    Line* line = &set->lines[i];
    if (line->valid && line->tag == tag) {
      for(int j = i; j > 0; --j) {
        set->lines[j] = set->lines[j - 1];
      }
      flag = 1;
      result->access = HIT;
      break;
    }
    if (line->valid == 0) {
      result->access = COLD_MISS;
      break;
    }
  }

  if (flag == 0) {
    for(int j = set->line_count - 1; j > 0; --j) {
      set->lines[j] = set->lines[j - 1];
    }
  }
  set->lines[0].valid = 1;
  set->lines[0].tag = tag;
  result->line = &set->lines[0];
}
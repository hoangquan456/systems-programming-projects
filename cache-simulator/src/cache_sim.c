#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache.h"
#include "cpu.h"
#include "lru.h"

void test_wc_trace() {
    int sets = 2;
    int lines = 4;
    int bytes = 8;
    
    Cache* cache = make_cache(sets, lines, bytes);
    if (cache == NULL) {
        printf("cache is NULL\n");
        assert(cache != NULL);
    }
    
    CPU* cpu = make_cpu(cache, "/home/quan/repos/CS230/cache/test/wc.trace");
    if (cpu == NULL) {
        printf("cpu is NULL\n");
        assert(cpu != NULL);
    }

    run_cpu(cpu);

    int diff_misses = abs(cpu->cold + cpu->conflict - 22104);
    if (!(cpu->cold + cpu->conflict > 22050 && cpu->cold + cpu->conflict < 22154)) {
        printf("the number of misses (cpu->cold+cpu->conflict) was not "
               "within the range of the expected result of 22104. "
               "You were off by %d.\n", diff_misses);
        assert(cpu->cold + cpu->conflict > 22050 && cpu->cold + cpu->conflict < 22154);
    }

    int diff_hits = abs(cpu->hits - 787666);
    if (!(cpu->hits > 787616 && cpu->hits < 787716)) {
        printf("the number of hits (cpu->hits) was not "
               "within the range of the expected result of 787666. "
               "You were off by %d.\n", diff_hits);
        assert(cpu->hits > 787616 && cpu->hits < 787716);
    }

    int diff_conflict = abs(cpu->conflict - 22088);
    if (!(cpu->conflict > 22038 && cpu->conflict < 22138)) {
        printf("the number of conflicts (cpu->conflict) was not "
               "within the range of the expected result of 22088. "
               "You were off by %d.\n", diff_conflict);
        assert(cpu->conflict > 22038 && cpu->conflict < 22138);
    }
}

void test2() {
    int sets = 2;
    int lines = 3;
    int bytes = 8;
    
    Cache* cache = make_cache(sets, lines, bytes);
    if (cache == NULL) {
        printf("cache is NULL\n");
        assert(cache != NULL);
    }

    // unsigned int s = get_set(cache, trace_line->address);
    // unsigned int t = get_line(cache, trace_line->address);
    // unsigned int b = get_byte(cache, trace_line->address);

    // Get the set:
    Set *set = &cache->sets[0];

    // Get the line:
    for(int i = 1; i <= 4; ++i) {
        LRUResult result;
        lru_fetch(set, 0, &result);
        Line *line = result.line;
        if (result.access != HIT) {
            for (int i = 0; i < cache->block_size; i++) {
                line->accessed[i] = 0;
            }
        }
      // Then set the accessed byte to 1:
        // line->accessed[b] = 1;
    }
    // Get the line:
    for(int i = 1; i <= 4; ++i) {
        LRUResult result;
        lru_fetch(set, 1, &result);
        Line *line = result.line;
        if (result.access != HIT) {
            for (int i = 0; i < cache->block_size; i++) {
                line->accessed[i] = 0;
            }
        }
    }

    for(int i = 1; i <= 4; ++i) {
        LRUResult result;
        lru_fetch(set, 2, &result);
        Line *line = result.line;
        if (result.access != HIT) {
            for (int i = 0; i < cache->block_size; i++) {
                line->accessed[i] = 0;
            }
        }
    }

    LRUResult result;
    lru_fetch(set, 0, &result);
    lru_fetch(set, 0, &result);
    
    
    // If it was a miss we will clear the accessed bits:

}  

int main(int argc, char *argv[]) {
    test2();
    printf("test ok");
}
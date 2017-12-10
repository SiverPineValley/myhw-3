#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "run.h"
#include "util.h"

void *base = 0;

p_meta find_meta(p_meta *last, size_t size) {
  p_meta previous = base;
  p_meta index = base;
  p_meta result = base;
  p_meta pbrk = sbrk(0);
  struct rlimit limit;
  if( getrlimit(RLIMIT_DATA, &limit) != 0 ) exit(0);
  
  switch(fit_flag){
    case FIRST_FIT:
    {
     bool i = true;
     while(i) {
      previous = index;
      if( index->free == true ) {
       if( index == pbrk ) {
	pbrk = sbrk(size);
	index->free = false;
	index->size = size;
	index->next = pbrk;
	index->prev = previous;
	result = index;
        i = false;
       }
       else if( size <= index->size ) {
        index->free = false;
	index->size = size;
	index->next = index + index->size + sizeof(struct metadata);
	index->prev = previous;
        result = index;
        i = false;
       }
       else index = index + index->size + sizeof(struct metadata);
       }
      else if( index >= limit.rlim_max ) return NULL;
      else index = index + index->size + sizeof(struct metadata);
     }
    }
    break;

    case BEST_FIT:
    {
      //BEST_FIT CODE
    }
    break;

    case WORST_FIT:
    {
      //WORST_FIT CODE
    }
    break;

  }
  return result;
}

void *m_malloc(size_t size) {

}

void m_free(void *ptr) {

}

void*
m_realloc(void* ptr, size_t size)
{

}

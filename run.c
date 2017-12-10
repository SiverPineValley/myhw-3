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
	result = index;
        i = false;
       }
       else if( size <= index->size ) {
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
  last = &previous;
  return result;
}

void *m_malloc(size_t size) {
  p_meta last = base;
  p_meta previous = base;
  p_meta pbrk = sbrk(0);
  struct rlimit limit;
  p_meta hlimit = ( base + limit.rlim_max );
  
  last = find_meta(&previous,size);
  if( last >= hlimit ) return NULL;
  if( last == pbrk ) {
   if( ( hlimit - pbrk ) < size ) return NULL;
   else {
    pbrk = sbrk(size);
    last->free = false;
    last->size = size;
    last->next = pbrk;
    last->prev = previous;
   }
  } else {
   last->free = false;
   last->size = size;
   last->next = last + ( size + sizeof(struct metadata));
   last->prev = previous;
  }
  return (last + sizeof(struct metadata));
}

void m_free(void *ptr) {

}

void*
m_realloc(void* ptr, size_t size)
{

}

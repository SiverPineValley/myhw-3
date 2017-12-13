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
  getrlimit(RLIMIT_DATA, &limit);
  long hsize = limit.rlim_max/(long)sizeof(void*);
  void* hend = base + hsize;
  
  switch(fit_flag){
    case FIRST_FIT:
    {
     bool i = true;
     while(i) {
      if( index->free == true ) {
       if( index == pbrk ) {
	last = &previous;
	result = index;
        i = false;
       }
       else if( size <= index->size ) {
        last = &previous;
        result = index;
        i = false;
       }
       else { 
        previous = index;
        index = index + index->size + sizeof(struct metadata);
       }
      }
      else if( (void*)index >= hend ) return NULL;
      else { 
       previous = index;
       index = index + index->size + sizeof(struct metadata);
      }
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
  p_meta last = base;
  p_meta now = base;
  p_meta pbrk = sbrk(0);
  struct rlimit limit;
  getrlimit(RLIMIT_DATA ,&limit);
  long hsize = limit.rlim_max/(long)sizeof(void*);
  void* hend = base + hsize;
  
  now = find_meta(&last,size);
  if( now >= hsize ) return NULL;
  if( now == pbrk ) {
   if( ( (p_meta)hend - pbrk ) < size ) return NULL;
   else {
    pbrk = sbrk(size);
    now->free = false;
    now->size = size;
    now->next = pbrk;
    now->prev = last;
   }
  } else {
   now->free = false;
   now->size = size;
   now->next = now + ( size + sizeof(struct metadata));
   now->prev = last;
   last->next = now;
   now->next->prev = now;
  }
  return (last + sizeof(struct metadata));
}

void m_free(void *ptr) {

}

void*
m_realloc(void* ptr, size_t size)
{

}

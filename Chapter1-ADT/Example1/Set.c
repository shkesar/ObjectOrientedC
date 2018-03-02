#include <assert.h>
#include <stdlib.h>

#include "new.h"
#include "Object.h"
#include "Set.h"

// Redefine extern Object and Set to keep the compiler happy
static const size_t _Set = sizeof(int);
static const size_t _Object = sizeof(int);
const void * Object = & _Object;
const void * Set = & _Set;

/* ADT implementation using singular heap*/
// new.c
#if ! defined MANY || MANY < 1
#define MANY 10
#endif

static int heap[MANY];

void * new (const void * type, ...) {
  int *p;

  for (p = heap+1; p < heap + MANY; ++p) {
    if (!*p) break;
  }
  assert(p < heap + MANY);
  *p = MANY;
  return p;
}

void delete (void * _item) {
  int * item = _item;
  if (item) {
    assert(item > heap && item < heap + MANY);
    *item = 0;
  }
}

// Object.c
int differ(const void * a, const void *b) {
  return a != b;
}

// Set.c
void * add(void * _set, const void * _element) {
  int * set = _set;
  const int * element = _element;

  assert(set > heap && set < heap + MANY);
  assert(* set == MANY);
  assert(element > heap && element < heap + MANY);

  if (*element == MANY) {
    * (int *)element = heap - set;
  } else {
    assert(* element == heap - set);
  }

  return (void *) element;
}

void * find (const void * _set, const void * _element) {
  const int * set = _set;
  const int * element = _element;

  assert(set > heap && set < heap + MANY);
  assert(* set == MANY);
  assert(element > heap && element < heap + MANY);
  assert(* element);

  return * element == (set - heap) ? (void *) element : 0;
}

int contains (const void * _set, const void * _element) {
  return find(_set, _element) != 0;
}

void * drop (void * _set, const void * _element) {
  int * element = _element;

  if (element)
    * element = MANY;

  return element;
}


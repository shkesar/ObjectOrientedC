#include <assert.h>
#include <stdlib.h>

#include "new.h"
#include "Object.h"
#include "Set.h"

typedef struct {
  int count;
} Set_t;

typedef struct {
  int count;
  Set_t * in;
} Object_t;

// Redefine extern Object and Set to keep the compiler happy
static const size_t _Set = sizeof(Set_t);
static const size_t _Object = sizeof(Object_t);

const void * Set = & _Set;
const void * Object = & _Object;

/* ADT implementation using singular heap*/
// new.c

void * new (const void * type, ...) {
  const size_t size = sizeof(type);
  void *p = calloc(1, size);

  assert(p);
  return p;
}

void delete (void * _item) {
  free(_item);
}

// Object.c
int differ(const void * a, const void *b) {
  return a != b;
}

// Set.c
void * add(void * _set, const void * _element) {
  Set_t * set = _set;
  Object_t * element = (void *) _element;

  assert(set);
  assert(element);

  if (!element->in) {
    element->in = set;
  } else {
    assert(element->in == set);
  }

  ++element->count, ++set->count;

  return (void *) element;
}

void * find (const void * _set, const void * _element) {
  const Set_t * set = _set;
  const Object_t * element = _element;

  assert(set);
  assert(element);

  return element->in == set ? (void *) element : 0;
}

int contains (const void * _set, const void * _element) {
  return find(_set, _element) != 0;
}

void * drop (void * _set, const void * _element) {
  Set_t * set = _set;
  Object_t * element = (Object_t *) _element;

  if (element) {
    if (-- element->count == 0) {
      element->in = 0;
    }
    -- set->count;
  }

  return element;
}

unsigned count (const void * _set) {
  const Set_t * set = _set;
  assert(set);
  return set->count;
}


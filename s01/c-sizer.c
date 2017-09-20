#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>


/// spec_size(spec)
///    Return the size of the C struct specified by the type specification `spec`,
///    as compiled on an x86-64 machine. A type specification is a string of
///    characters where each character corresponds to a struct member of some
///    fundamental type:
///
///         `c`    char (or signed char, unsigned char)
///         `s`    short (or unsigned short)
///         `i`    int (or unsigned int)
///         `l`    long (or unsigned long)
///         `z`    size_t
///         `f`    float
///         `d`    double
///         `p`    pointer
///
///    See `README.md` or the Section 1 wiki page for hints and advanced features.

size_t spec_size(const char* spec) {
    (void) spec;
    int i = 0;
    int size = 0;
    while (spec[i] != '\0') {
      char c = spec[i];
      if (c == 'c') {
        // determine if padding is necessary. If it is, add it
        if (size % __alignof__(char) != 0) {
            size += sizeof(char) - size % __alignof__(char);
        }
        size += sizeof(char);
      }
      else if (c == 's') {
        if (size % __alignof__(short) != 0) {
            size += sizeof(short) - size % __alignof__(short);
        }
        size += sizeof(short);
      }
      else if (c == 'i') {
        if (size % __alignof__(int) != 0) {
            size += sizeof(int) - size % __alignof__(int);
        }
        size += sizeof(int);
      }
      else if (c == 'l') {
        if (size % __alignof__(long) != 0) {
            size += sizeof(long) - size % __alignof__(long);
        }
        size += sizeof(long);
      }
      else if (c == 'z') {
        if (size % __alignof__(size_t) != 0) {
            size += sizeof(size_t) - size % __alignof__(size_t);
        }
        size += sizeof(size_t);
      }
      else if (c == 'f') {
        if (size % __alignof__(float) != 0) {
            size += sizeof(float) - size % __alignof__(float);
        }
        size += sizeof(float);
      }
      else if (c == 'd') {
        if (size % __alignof__(double) != 0) {
            size += sizeof(double) - size % __alignof__(double);
        }
        size += sizeof(double);
      }
      else if (c == 'p') {
        if (size % __alignof__(void*) != 0) {
            size += sizeof(void*) - size % __alignof__(void*);
        }
          size += sizeof(void*);
      }
      i++;
    }
    return size;
}


/// spec_print(spec, ptr)
///    Print the contents of the memory located at `ptr` as interpreted
///    by type specification `spec`. For example:
///
///        int i = 10;
///        spec_print("i", &i);
///
///    might print something like
///
///        7fffffabc int 10
///
///    (assuming `i` is located at address 0x7fffffabc), and
///
///        struct { char c; int d; } x = { 'A', 24 };
///        spec_print("ci", &x);
///
///    might print something like
///
///        7ffffffabc char A
///        7ffffffac0 int 24
///
///    You'll need to add your own tests of `spec_print`.

void spec_print(const char* spec, const void* data) {
    (void) spec, (void) data;
    int i = 0;
    int progress = 0;
    // arbitrarily supports structs up to 64 elements in size
    char* checker = calloc(64, sizeof(char));
    while (spec[i] != '\0') {
      char c = spec[i];
      if (c == 'c') {
        printf("%p char %c\n", &data + progress, *((char*)data + progress));
        // track how far along in the struct we are
        checker[i] = spec[i];
        progress += spec_size(checker);
      }
      else if (c == 's') {
        printf("%p short %hu\n", &data + progress, *((short*)data + progress));
        // track how far along in the struct we are
        checker[i] = spec[i];
        progress += spec_size(checker);
      }
      else if (c == 'i') {
        printf("%p int %i\n", &data + progress, *((int*)data + progress));
        // track how far along in the struct we are
        checker[i] = spec[i];
        progress += spec_size(checker);
      }
      else if (c == 'l') {
        printf("%p long %lu\n", &data + progress, *((long*)data + progress));
        // track how far along in the struct we are
        checker[i] = spec[i];
        progress += spec_size(checker);
      }
      else if (c == 'z') {
        printf("%p size_t %zu\n", &data + progress, *((size_t*)data + progress));
        // track how far along in the struct we are
        checker[i] = spec[i];
        progress += spec_size(checker);
      }
      else if (c == 'f') {
        printf("%p float %f\n", &data + progress, *((float*)data + progress));
        // track how far along in the struct we are
        checker[i] = spec[i];
        progress += spec_size(checker);
      }
      else if (c == 'd') {
        printf("%p double %lf\n", &data + progress, *((double*)data + progress));
        // track how far along in the struct we are
        checker[i] = spec[i];
        progress += spec_size(checker);
      }
      else if (c == 'p') {
        printf("%p pointer %p\n", &data + progress, ((char*)data + progress));
        // track how far along in the struct we are
        checker[i] = spec[i];
        progress += spec_size(checker);
      }
      i++;
    }
}


int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        printf("%8zu %s\n", spec_size(argv[i]), argv[i]);
        struct { char c; char a; int d; double e; } x = { 'A', 'B', 24, 32.4, };
        spec_print("ci", &x);
    }
}

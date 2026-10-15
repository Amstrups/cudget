#include <stdint.h>
#include <string.h>

typedef uint32_t u32;

#define _BUFFER_LEN 4096

typedef struct {
  u32 cursor;
  char buffer[_BUFFER_LEN];
} write_buffer;

void newline(write_buffer *wb) {
  if (wb->cursor + 2 >= _BUFFER_LEN) {
    // write to error, or?
    return;
  }

  wb->buffer[wb->cursor] = 0x0D;     // carriage return
  wb->buffer[wb->cursor + 1] = 0x0A; // line feed
  wb->cursor += 2;
}

void buf_write(write_buffer *wb, const char *str, int n) {
  if (wb->cursor + n >= _BUFFER_LEN) {
    // write to error, or?
    return;
  }

  memcpy(&wb->buffer[wb->cursor], str, n * sizeof(char));
  wb->cursor += n;
}

void write_str(write_buffer *wb, const char *str) {
  buf_write(wb, str, strlen(str));
}

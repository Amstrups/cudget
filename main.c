#include "file.c"
#include <math.h>
#include <stdio.h>

static const char currency[3] = "dkk";
static const char *user = "chris";
static const char *name = "test budget";

void write_config(write_buffer *wb) {
  write_str(wb, "[config]");
  newline(wb);

  write_str(wb, "currency = ");
  buf_write(wb, currency, 3);
  newline(wb);

  write_str(wb, "user = ");
  write_str(wb, user);
  newline(wb);

  write_str(wb, "name = ");
  write_str(wb, name);
  newline(wb);
}

#define _MAX_ACCOUNTS 8
#define _MAX_EXPENSES 16
#define _ROW_PAGE_SIZE 16
#define _PAGE_PER_EXPENSE 16
#define _STUPIDLY_LARGE_BUFFER                                                 \
  (_MAX_EXPENSES * _PAGE_PER_EXPENSE * _ROW_PAGE_SIZE)

typedef struct {
  char _str[64];
} name_str;

typedef struct {
  char deleted;
  long date;
  float_t amount;
} row;

typedef struct {
  row rows[_ROW_PAGE_SIZE];
  int len;
} row_page;

typedef row_page *row_page_prt;

static const row_page global_row_arr[_STUPIDLY_LARGE_BUFFER] = {};

typedef struct {
  name_str name;
  char deleted;
  row_page_prt row_pages[_PAGE_PER_EXPENSE];
} expense;

void write_expense(write_buffer *wb, name_str acc_name, expense *exp) {
  write_str(wb, "[expense]\n");

}

typedef struct {
  expense *expenses[_MAX_EXPENSES];
} expense_arr;

static const expense global_expense_arr[_MAX_EXPENSES] = {};

typedef struct {
  name_str name;
  expense_arr expenses;
} account;

void write_account(write_buffer *wb, account *acc) {
  write_str(wb, "[account]");
}

void save(account accounts[], int n) {
  write_buffer wb = {0};
  write_config(&wb);

  for (int i = 0; i < n; i++) {
    write_account(&wb, &accounts[i]);
  }

  // Open a file in writing mode
  FILE *f = fopen("./examples/test.brk", "w");
  printf("amount of chars in buffer: %d\n", wb.cursor);
  fprintf(f, "%s", wb.buffer);
  fclose(f);
}

void setup_example(account accounts[5]) {
  char *name_checkings = "checkings";
  memcpy(&(&accounts[0])->name, name_checkings, strlen(name_checkings));
}

int main(void) {
  printf("hello world\n");
  int acc_count = 0;
  account accounts[5] = {};
  setup_example(accounts);

  printf("first account name: %s\n", accounts[0].name._str);
  printf("second account name: %s\n", accounts[1].name._str);
  printf("account size: %zu\n", sizeof(expense_arr));

  return 0;
}

/*
 * file 1-N account
 * account 1-N expense
 * expense 1-N row
 *
 *
 * [ accounts ]
 * [ expenses { row_page_ptr } ]
 * [ row pages ]
 *
 *
 *
 *
 */

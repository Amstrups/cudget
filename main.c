#include "file.c"
#include <math.h>
#include <stdio.h>

static const char currency[3] = "dkk";
static const char *user = "chris";
static const char *name = "test budget";

void write_config(write_buffer *wb) {
  write_str(wb, "[config]");
  newline(wb);

  write_str(wb, "currency = \"");
  buf_write(wb, currency, 3);
  write_str(wb, "\"\n");

  write_str(wb, "user = \"");
  write_str(wb, user);
  write_str(wb, "\"\n");

  write_str(wb, "name = \"");
  write_str(wb, name);
  write_str(wb, "\"\n\n");
}

#define _MAX_ACCOUNTS 8
#define _MAX_EXPENSES 32
#define _ROW_PAGE_SIZE 16
#define _PAGE_PER_EXPENSE 16
#define _STUPIDLY_LARGE_BUFFER (_MAX_EXPENSES * _PAGE_PER_EXPENSE)

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

static const row_page global_row_page_arr[_STUPIDLY_LARGE_BUFFER] = {};

typedef struct {
  name_str name;
  char deleted;
  row_page_prt row_pages[_PAGE_PER_EXPENSE];
} expense;

void write_expense(write_buffer *wb, name_str acc_name, expense *exp) {
  write_str(wb, "[expense]\n");
  write_str(wb, "name = \"");
  write_str(wb, exp->name._str);
  write_str(wb, "\"\n");
}

typedef struct {
  expense *expenses[_MAX_EXPENSES / 4];
} expense_arr;

static const struct {
  int len;
  expense expenses[_MAX_EXPENSES];
} global_expense_arr = {};

typedef struct {
  name_str name;
  expense_arr expenses;
} account;

void write_account(write_buffer *wb, account *acc) {
  write_str(wb, "[account]\n");
  write_str(wb, "name = \"");
  write_str(wb, acc->name._str);
  write_str(wb, "\"\n\n");
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

#define EXAMPLE_ACC_COUNT 2
void setup_example(account accounts[EXAMPLE_ACC_COUNT]) {
  char *names[EXAMPLE_ACC_COUNT] = {
      "checkings",
      "savings",
  };

  for (int i = 0; i < EXAMPLE_ACC_COUNT; i++) {
    memcpy(&(&accounts[i])->name, names[i], strlen(names[i]));
  }
}

int main(void) {
  printf("hello world\n");
  int acc_count = 0;
  account accounts[EXAMPLE_ACC_COUNT] = {};

  setup_example(accounts);
  save(accounts, EXAMPLE_ACC_COUNT);

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

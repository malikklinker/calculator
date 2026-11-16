#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int read_line(FILE *file, char line[128]) {
  if (!fgets(line, 128, file))
    return 0;
  if (!strchr(line, '\n') && !feof(file))
    return -1;
  line[strcspn(line, "\r\n")] = '\0';
  return 1;
}

static int number(char *line, long *value) {
  errno = 0;
  char *end;
  *value = strtol(line, &end, 10);
  if (line == end || errno)
    return 0;
  while (isspace((unsigned char)*end))
    ++end;
  return *end == '\0';
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s input.txt\n", argv[0]);
    return 1;
  }
  FILE *file = fopen(argv[1], "r");
  if (!file) {
    perror(argv[1]);
    return 1;
  }
  char line[128];
  long value, operand, result;
  if (read_line(file, line) != 1 || !number(line, &value))
    goto error;
  int state;
  while ((state = read_line(file, line)) != 0) {
    if (state != 1 || strlen(line) != 1 || !strchr("+-*/", line[0]))
      goto error;
    char op = line[0];
    if (read_line(file, line) != 1 || !number(line, &operand))
      goto error;
    if (op == '+') {
      if (__builtin_add_overflow(value, operand, &result))
        goto error;
    } else if (op == '-') {
      if (__builtin_sub_overflow(value, operand, &result))
        goto error;
    } else if (op == '*') {
      if (__builtin_mul_overflow(value, operand, &result))
        goto error;
    } else {
      if (!operand || (value == LONG_MIN && operand == -1))
        goto error;
      result = value / operand;
    }
    value = result;
  }
  if (ferror(file))
    goto error;
  fclose(file);
  printf("%ld\n", value);
  return 0;
error:
  fclose(file);
  puts("ERROR");
  return 1;
}

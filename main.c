#define _GNU_SOURCE

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char *home = secure_getenv("HOME");

  if (home == NULL) {
    printf("HOME is unset\n");
  } else {
    printf("HOME=%s\n", home);
  }

  char *new_argv[argc+1];
  for (int i = 0; i < argc; i++) {
    new_argv[i] = strdup(argv[i]);
    printf("argv[%d]: %s\n", i, new_argv[i]);
  }
  new_argv[argc] = NULL;

  char *shell = "bash";
  new_argv[0] = shell;
  int ret = execvp(shell, new_argv);
  if (ret == -1) {
    perror("error executing shell");
    return 1;
  }

  return 0;
}

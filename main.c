#define _GNU_SOURCE
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

bool usable(char const *path) {
  char real[PATH_MAX];
  if (realpath(path, real) == NULL) {
    return false;
  }

  if (access(real, X_OK) == 0) {
    struct stat sb = {};
    if (stat(real, &sb) == 0 && S_ISREG(sb.st_mode)) {
      return true;
    } else {
      return false;
    }
  } else {
    char *msg;
    int errsv = errno;

    switch (errsv) {
    case EACCES:
      break;
    default:
      asprintf(&msg, "WARN: Checking %s", path);
      perror(msg);
    }

    return false;
  }
}

char *getshell(void) {
  char *relpath = "shell";
  char *xdg_config_home = getenv("XDG_CONFIG_HOME");

  char *xdg_result = NULL;
  if (xdg_config_home != NULL) {
    asprintf(&xdg_result, "%s/%s", xdg_config_home, relpath);
    if (usable(xdg_result)) {
      return xdg_result;
    }
  }

  char *home = getenv("HOME");
  if (home != NULL) {
    char *home_result;
    asprintf(&home_result, "%s/.config/%s", home, relpath);
    if (strcmp(home_result, xdg_result) && usable(home_result)) {
      return home_result;
    };
  }

  char *fallback_result = "/bin/sh";
  fprintf(stderr, "WARN: Falling back to %s\n", fallback_result);
  return fallback_result;
}

int main(int argc, char *argv[]) {
  char *shell = getshell();
  if (shell == NULL) {
    fprintf(stderr, "ERROR: Failed to detect shell");
    return EXIT_FAILURE;
  }
  fprintf(stderr, "INFO: Using %s\n", shell);

  char *new_argv[argc + 1];
  for (int i = 0; i < argc; i++) {
    new_argv[i] = strdup(argv[i]);
  }
  new_argv[argc] = NULL;

  new_argv[0] = shell;
  int ret = execvp(shell, new_argv);
  if (ret == -1) {
    perror("error executing shell");
    return 1;
  }

  return EXIT_FAILURE; // unreachable
}

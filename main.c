#define _GNU_SOURCE
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "noshell.h"

bool usable(char const* path) {
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
    char* msg;
    int errsv = errno;

    switch (errsv) {
    case EACCES:
      break;
    default:
      asprintf(&msg, "WARN: Checking %s", path);
      perror(msg);
      free(msg);
    }

    return false;
  }
}

char* getshell(void) {
  char* relpath = "shell";
  char* xdg_config_home = getenv("XDG_CONFIG_HOME");

  char* xdg_result = NULL;
  if (xdg_config_home != NULL) {
    asprintf(&xdg_result, "%s/%s", xdg_config_home, relpath);
    if (usable(xdg_result)) {
      return xdg_result;
    }
  }

  char* home = getenv("HOME");
  if (home != NULL) {
    char* home_result;
    asprintf(&home_result, "%s/.config/%s", home, relpath);
    if (xdg_config_home == NULL) {
      if (usable(home_result)) {
        return home_result;
      }
    } else {
      if (strcmp(home_result, xdg_result) && usable(home_result)) {
        return home_result;
      };
    }
  }

  fprintf(stderr, "WARN: Using fallback shell\n");
  return strdup(DEFAULT_SHELL);
}

/// Changes argv0 depending on the filetype
/// - Regular file: uses the basename
/// - Symlink: dereferences the symlink once and uses the basename
void argv0_deref(char** argv0_p) {
  struct stat sb = {};
  if (lstat(*argv0_p, &sb) != 0) {
    fprintf(stderr, "WARN: Failed to stat the shell\n");
    return;
  }

  if (S_ISREG(sb.st_mode)) {
    *argv0_p = strdup(basename(*argv0_p));
    return;
  }

  if (S_ISLNK(sb.st_mode)) {
    char buf[PATH_MAX + 1];
    if (readlink(*argv0_p, buf, PATH_MAX) == -1) {
      fprintf(stderr, "Failed to readlink\n");
      return;
    }
    *argv0_p = strdup(basename(buf));
    return;
  }
}

int main(int argc, char* argv[]) {
  (void)argc;
  char* shell = getshell();

  if (shell == NULL) {
    fprintf(stderr, "ERROR: Failed to detect shell");
    return EXIT_FAILURE;
  }

  fprintf(stderr, "INFO: Using %s\n", shell);

  bool login_dash = argv[0][0] == '-';

  argv[0] = shell;
  argv0_deref(&argv[0]);

  if (login_dash) {
    asprintf(&argv[0], "-%s", argv[0]);
  }

  execvp(shell, argv);
  free(shell);

  perror("ERROR: Failed to execute shell");
  return EXIT_FAILURE;
}

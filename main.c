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

// Get argument that should be passed as zeroth
// argument to the shell executable.
void get_shell_arg(char* buffer, char* argv, char* shell)
{
  bool written = false, read_status = true;

  if (argv[0] == '-') {
    (*buffer++) = '-';
  }

  struct stat buf;
  if (lstat(shell, &buf) != 0) {
    perror("ERROR: shell file status cannot be read");
    read_status = false;
  }

  if (read_status && S_ISLNK(buf.st_mode)) {
    ssize_t len = readlink(shell, buffer, PATH_MAX);
    if (len < 0) {
      perror("ERROR: Failed to read shell link value");
    } else {
      written = true;
      buffer[len] = '\0';
    }
  }

  if (!written) {
    strcpy(buffer, shell);
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

int main(int argc, char* argv[]) {
  (void)argc;
  char* shell = getshell();

  if (shell == NULL) {
    fprintf(stderr, "ERROR: Failed to detect shell");
    return EXIT_FAILURE;
  }

  fprintf(stderr, "INFO: Using %s\n", shell);

  char arg_zero_buf[PATH_MAX + 1];
  get_shell_arg(arg_zero_buf, argv[0], shell);
  argv[0] = arg_zero_buf;

  execvp(shell, argv);
  free(shell);

  perror("ERROR: Failed to execute shell");
  return EXIT_FAILURE;
}

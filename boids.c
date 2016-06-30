#include "version.h"
#include <stdio.h>

int main (int argc, char *argv[], char **env_var_ptr) {

	printf ("%s v%s.%s-%s\n", gVERSION, gVERSION_MAJOR, gVERSION_MINOR,
		gVERSION_REST);
	return 0;
}

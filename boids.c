#include "version.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[], char **env_var_ptr) {
	printf ("%s v%s.%s-%s\n", gVERSION, gVERSION_MAJOR, gVERSION_MINOR,
		gVERSION_REST);
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		fprintf (stderr, "Unable to initialize SDL:  %s\n",
			 SDL_GetError ());
		exit(1);
	}
	atexit (SDL_Quit);
	/* ... */
	exit(0);
}

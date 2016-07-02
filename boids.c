#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "version.h"
#include "cleanlist.h"

#ifdef DEBUG
#  warning Building debug version.
#endif

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#  pragma message "Compiling on unix-compatible system..."
#  include <unistd.h>
#elif defined(_WIN32)
#  pragma message "Compiling on windows..."
#  include <windows.h>
#  define sleep(x) Sleep((x)*1000)
#endif

int main (int argc, char *argv[], char **env_var_ptr) {
	list_item *head;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtrampolines"
	void cleanatexit (void) {
		clean (head);
	}
#pragma GCC diagnostic pop
	atexit (cleanatexit);

#ifdef DEBUG
	push (&head, (cleaner) printf, "done\n");
#endif

	printf ("%s v%s.%s-%s\n", gVERSION, gVERSION_MAJOR, gVERSION_MINOR,
		gVERSION_REST);
	// init SDL
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		fprintf (stderr, "Unable to initialize SDL:  %s\n",
			 SDL_GetError ());
		exit (1);
	}
	atexit (SDL_Quit);
	/* ... */

	sleep (1);
	exit (0);
}

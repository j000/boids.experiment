#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_macros.h"
#include "version.h"
#include "cleanlist.h"

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#  pragma message "Compiling for unix-compatible system..."
#  include <unistd.h>
#elif defined(_WIN32)
#  pragma message "Compiling for windows..."
#  include <windows.h>
#  define sleep(x) Sleep((x)*1000)
#endif

int main (int argc, char *argv[], char **env_var_ptr) {
	ERROR ("start");
	pListItem head = NULL;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtrampolines"
	void cleaningatexit (void) {
		cleaning (&head);
	}
#pragma GCC diagnostic pop
	atexit (cleaningatexit);

#ifdef DEBUG
	push_cleaner (&head, (fpCleaner) printf, "done\n");
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

	// create and show window
	SDL_Window *win;

	if ((win = SDL_CreateWindow ("Hello World!", 100, 100, 640, 480,
				     SDL_WINDOW_SHOWN)) == NULL) {
		fprintf (stderr, "Unable to create window:  %s\n",
			 SDL_GetError ());
		exit (1);
	}
	push_cleaner (&head, (fpCleaner) SDL_DestroyWindow, window);
#ifdef DEBUG
	push_cleaner (&head, (fpCleaner) printf, "SDL_DestroyWindow\n");
#endif

	// create renderer
	SDL_Renderer *ren;

	if ((ren = SDL_CreateRenderer (win, -1,
				       SDL_RENDERER_ACCELERATED |
				       SDL_RENDERER_PRESENTVSYNC)) == NULL) {
		fprintf (stderr, "Unable to create renderer:  %s\n",
			 SDL_GetError ());
		exit (1);
	}
	push_cleaner (&head, (fpCleaner) SDL_DestroyRenderer, renderer);
#ifdef DEBUG
	push_cleaner (&head, (fpCleaner) printf, "SDL_DestroyRenderer\n");
#endif

	/* ... */

	sleep (1);
	exit (0);
}

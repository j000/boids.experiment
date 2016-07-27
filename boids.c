#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#  pragma message ("Compiling for unix-style system")
#  include <unistd.h>
#elif defined(_WIN32)
#  pragma message ("Compiling for windows")
#  include <windows.h>
#  undef ERROR			// defined in windows.h, we want ours
#  define sleep(x) Sleep((x)*1000)
#endif

#include "my_macros.h"
#include "version.h"
#include "cleanlist.h"

SDL_PUSH_WARNINGS
#include <SDL.h>
SDL_POP_WARNINGS

// declaraions
// TODO: move to *.h
static pListItem head;
void cleaningatexit (void);
void bail (const char *, const char *) NORETURN;

// globals
static pListItem head = NULL;

void cleaningatexit (void) {
	cleaning (&head);
}

// prints error and exits application
// TODO: show warning window
void bail (const char *message, const char *error) {
	fprintf (stderr, "%s:  %s\n", message, error);
	exit (EXIT_FAILURE);
}

int main ( /* int argc, char *argv[], char **env_var_ptr */ ) {
	ERROR ("start");
	atexit (cleaningatexit);

#ifdef DEBUG
	push_cleaner (&head, (fpCleaner) printf, "done\n");
#endif

	printf ("%s v%s.%s.%s-%s\n", gVERSION, gVERSION_MAJOR, gVERSION_MINOR,
		gVERSION_PATCH, gVERSION_PRERELEASE);

	// init SDL
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		bail ("Unable to initialize SDL", SDL_GetError());
	}
	atexit (SDL_Quit);

	// create and show window
	SDL_Window *window;

	if ((window =
	     SDL_CreateWindow ("Hello World!", SDL_WINDOWPOS_CENTERED,
			       SDL_WINDOWPOS_CENTERED, 640, 480,
			       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)) ==
	    NULL) {
		bail ("Unable to create window", SDL_GetError());
	}
	push_cleaner (&head, (fpCleaner) SDL_DestroyWindow, window);
#ifdef DEBUG
	push_cleaner (&head, (fpCleaner) printf, "SDL_DestroyWindow\n");
#endif

	// create renderer
	SDL_Renderer *renderer;

	if ((renderer = SDL_CreateRenderer (window, -1,
					    SDL_RENDERER_ACCELERATED |
					    SDL_RENDERER_PRESENTVSYNC)) ==
	    NULL) {
		/*fprintf (stderr, "Unable to create renderer:  %s\n",
		   SDL_GetError ());
		   exit (EXIT_FAILURE); */
		bail ("Unable to create renderer", SDL_GetError ());
	}
	push_cleaner (&head, (fpCleaner) SDL_DestroyRenderer, renderer);
#ifdef DEBUG
	push_cleaner (&head, (fpCleaner) printf, "SDL_DestroyRenderer\n");
#endif

	// loop flag
#ifdef DEBUG
	int keepgoing = 30;
#else
	bool keepgoing = true;
#endif

	// event handler
	SDL_Event e;

	while (keepgoing) {
		while (SDL_PollEvent (&e) != 0) {
			if (e.type == SDL_QUIT) {
				ERROR ("quit");
				keepgoing = false;
			}
		}

		// Clear screen
		SDL_SetRenderDrawColor (renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear (renderer);

		// Render red filled quad
		SDL_Rect fillRect = { 640 / 4, 480 / 4, 640 / 2, 480 / 2 };
		SDL_SetRenderDrawColor (renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect (renderer, &fillRect);
		SDL_RenderPresent (renderer);

		// 17 ms ~~ 1/60 s <=> 17 ms * 60 = 1.02 s
		// FIXME: That's not how you do fixed framerate
		SDL_Delay (17);
#ifdef DEBUG
		--keepgoing;
#endif
	}
	/* ... */

	// wait
	//SDL_Delay (1000);
	//sleep (1);

	exit (EXIT_SUCCESS);
}

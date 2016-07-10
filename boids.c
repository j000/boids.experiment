//#include <SDL2/SDL.h>
#include <SDL.h>
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
	SDL_Window *window;

	if ((window =
	     SDL_CreateWindow ("Hello World!", SDL_WINDOWPOS_CENTERED,
			       SDL_WINDOWPOS_CENTERED, 640, 480,
			       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)) ==
	    NULL) {
		fprintf (stderr, "Unable to create window:  %s\n",
			 SDL_GetError ());
		exit (1);
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
		fprintf (stderr, "Unable to create renderer:  %s\n",
			 SDL_GetError ());
		exit (1);
	}
	push_cleaner (&head, (fpCleaner) SDL_DestroyRenderer, renderer);
#ifdef DEBUG
	push_cleaner (&head, (fpCleaner) printf, "SDL_DestroyRenderer\n");
#endif

	//Clear screen
	SDL_SetRenderDrawColor (renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear (renderer);
	SDL_RenderPresent (renderer);

	//Render red filled quad
	SDL_Rect fillRect = { 640 / 4, 480 / 4, 640 / 2, 480 / 2 };
	SDL_SetRenderDrawColor (renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect (renderer, &fillRect);
	SDL_RenderPresent (renderer);

	// loop flag
	bool keepgoing = true;

	// event handler
	SDL_Event e;

	while (keepgoing) {
		while (SDL_PollEvent (&e) != 0) {
			if (e.type == SDL_QUIT) {
				ERROR ("quit");
				keepgoing = false;
			}
		}
		SDL_RenderPresent (renderer);

		//17 ms ~~ 1/60 s <=> 17 ms * 60 = 1.02 s
		SDL_Delay (17);
	}
	/* ... */

	// wait
	//SDL_Delay (1000);
	//sleep (1);

	exit (0);
}

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Test", 0, 0, 500, 250, 0);

  // Fix: capture the return value
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Event event;
  int running = 1;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
      }
    }

    // Clear with white background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Draw something so you can see it worked
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    SDL_Rect rect = {100, 50, 100, 100};
    SDL_RenderFillRect(renderer, &rect);

    // Actually show what you drew
    SDL_RenderPresent(renderer);

    SDL_Delay(16);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
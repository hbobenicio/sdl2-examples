#include "sdl2-wrapper.h"

// g++ -std=c++14 -Wall -pedantic `sdl2-config --cflags --libs` -I sdl2-wrapper/ sdl2-wrapper/sdl2-wrapper.o sdl2-wrapper-example.cpp -lSDL2 -o sdl2-wrapper-example
int main(int argc, char* argv[])
{
  try {

    // Initialize SDL
    SDL2::SDL sdl{SDL_INIT_EVERYTHING};

    // Create window
    SDL2::Window window{"Hello, World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN};

    // Create a Renderer for the window
    SDL2::Renderer renderer{window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC};

    SDL2::Surface image{SDL_LoadBMP("basketball-texture.bmp")};
    SDL2::Texture texture{SDL_CreateTextureFromSurface(renderer.get(), image.get())};

    // clear the current rendering target with the drawing color.
    renderer.renderClear();
    //  SDL_RenderCopy(ren.get(), texture.get(), NULL, NULL);
    // update the screen with any rendering performed since the previous call
    renderer.renderPresent();

    //While application is running
    bool quit{false};
    SDL_Event e;
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = 50;
    r.h = 50;
    while (!quit) {
      //Handle events on queue
      while (SDL_PollEvent(&e) != 0 ) {
        //User requests quit
        if (e.type == SDL_QUIT) {
          quit = true;
        }

        SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &r);
        renderer.renderPresent();
      }
    }
  }
  catch(SDL2::SDLError& e) {
    e.print();
    return e.return_code();
  }

  return 0;
}

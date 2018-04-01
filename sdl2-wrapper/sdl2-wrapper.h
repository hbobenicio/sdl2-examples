#ifndef SDL2_WRAPPER_H
#define SDL2_WRAPPER_H

#include <stdexcept>
#include <memory>
#include <SDL2/SDL.h>

namespace SDL2 {

  class SDLError : public std::runtime_error {
    protected:
      std::string what_message;
      int rc;

    public:
      SDLError(const std::string& what, int rc = 1);
      SDLError(const char* what, int rc = 1);

      virtual int return_code() const noexcept;
      virtual void print() const;
  };

  class SDL {
    public:
      SDL(Uint32 flags);
      virtual ~SDL();
  };

  /**
   * Wrapper Functor for SDL_DestroyWindow. Useful to use with smart pointers.
   */
  struct DestroyWindow {
    void operator()(SDL_Window* window);
  };

  /**
   * Wrapper class for SDL_Window.
   */
  class Window {
    protected:
      std::unique_ptr<SDL_Window, DestroyWindow> _window;

    public:
      Window(const char* title, int x, int y, int w, int h, Uint32 flags);

      SDL_Window* release() noexcept;
      SDL_Window* get() const noexcept;
  };

  /**
   * Wrapper Functor for SDL_DestroyRenderer. Useful to use with smart pointers.
   */
  struct DestroyRenderer {
    void operator()(SDL_Renderer* renderer);
  };

  /**
   * Wrapper class for SDL_Renderer.
   */
  class Renderer {
    protected:
      std::unique_ptr<SDL_Renderer, DestroyRenderer> _renderer;

    public:
      Renderer(SDL_Window* window, int index, Uint32 flags);
      Renderer(const Window& window, int index, Uint32 flags);

      /**
       * Wrapper for SDL_RenderClear(SDL_Renderer*).
       * Clear the current rendering target with the drawing color.
       */
      void renderClear() const;

      /**
       * Wrapper for SDL_RenderPresent(SDL_Renderer*).
       * Update the screen with any rendering performed since the previous call.
       */
      void renderPresent() const;

      SDL_Renderer* release() noexcept;
      SDL_Renderer* get() const noexcept;
  };

  /**
   * Wrapper Functor for SDL_FreeSurface. Useful to use with smart pointers.
   */
  struct DestroySurface {
    void operator()(SDL_Surface* surface);
  };

  using Surface = std::unique_ptr<SDL_Surface, DestroySurface>;

  struct DestroyTexture {
    void operator()(SDL_Texture* texture);
  };

  using Texture = std::unique_ptr<SDL_Texture, DestroyTexture>;
}

#endif

//g++ -std=c++14 -Wall -pedantic `sdl2-config --cflags --libs` -c sdl2-wrapper.cpp

#include "sdl2-wrapper.h"
#include <iostream>

namespace SDL2 {

// SDLError

SDLError::SDLError(const std::string& what, int rc)
  : std::runtime_error(what), what_message(what), rc(rc)
{
}

SDLError::SDLError(const char* what, int rc)
  : std::runtime_error(what), what_message(what), rc(rc)
{
}

int SDLError::return_code() const noexcept {
  return rc;
}

void SDLError::print() const {
  std::cerr << "SDLError[" << rc << "] - " << what_message;
  auto cause = SDL_GetError();
  if (cause != nullptr && strlen(cause) > 0) {
    std::cerr << ": " << cause;
  }
  std::cerr << '\n';
}

// SDL

SDL::SDL(Uint32 flags) {
  if (SDL_Init(flags) != 0) {
    throw SDLError{"SDL failed to initialize", 1};
  }
}

SDL::~SDL() {
  SDL_Quit();
}

void DestroyWindow::operator()(SDL_Window* window) {
  SDL_DestroyWindow(window);
}

// Window

Window::Window(const char* title, int x, int y, int w, int h, Uint32 flags)
  : _window(SDL_CreateWindow(title, x, y, w, h, flags))
{
  if (_window == nullptr) {
    throw SDLError{"SDL Window could not be created", 2};
  }
}

SDL_Window* Window::release() noexcept {
  return _window.release();
}

SDL_Window* Window::get() const noexcept {
  return _window.get();
}

// Renderer

void DestroyRenderer::operator()(SDL_Renderer* renderer) {
  SDL_DestroyRenderer(renderer);
}

Renderer::Renderer(SDL_Window* window, int index, Uint32 flags)
  : _renderer(SDL_CreateRenderer(window, index, flags))
{
  if (_renderer == nullptr) {
    throw SDLError{"Could not create renderer for window", 3};
  }
}

Renderer::Renderer(const Window& window, int index, Uint32 flags)
  : _renderer(SDL_CreateRenderer(window.get(), index, flags))
{
  if (_renderer == nullptr) {
    throw SDLError{"Could not create renderer for window", 3};
  }
}

void Renderer::renderClear() const {
  SDL_RenderClear(_renderer.get());
}

void Renderer::renderPresent() const {
  SDL_RenderPresent(_renderer.get());
}

SDL_Renderer* Renderer::release() noexcept {
  return _renderer.release();
}

SDL_Renderer* Renderer::get() const noexcept {
  return _renderer.get();
}

// Surface

void DestroySurface::operator()(SDL_Surface* surface) {
  SDL_FreeSurface(surface);
}

using Surface = std::unique_ptr<SDL_Surface, DestroySurface>;

// Texture

void DestroyTexture::operator()(SDL_Texture* texture) {
  SDL_DestroyTexture(texture);
}

using Texture = std::unique_ptr<SDL_Texture, DestroyTexture>;

} // namespace SDL2

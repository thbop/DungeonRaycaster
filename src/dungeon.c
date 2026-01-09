/*
* MIT License
* 
* Copyright (c) 2026 Thbop
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <SDL3/SDL.h>


#define WINDOW_TITLE  "Dungeon"
#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 800

#define SDL_ASSERT( x ) \
    if ( !( x ) ) \
        SDL_Log( "ERROR: %s", SDL_GetError() )


static struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} state;

int main() {
    SDL_ASSERT( SDL_Init( SDL_INIT_VIDEO ) );

    SDL_ASSERT( SDL_CreateWindowAndRenderer(
        WINDOW_TITLE,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0,
        &state.window,
        &state.renderer
    ) );

    bool running = true;
    while ( running ) {
        SDL_Event event;
        while ( SDL_PollEvent( &event ) ) {
            if ( event.type == SDL_EVENT_QUIT )
                running = false;
        }
    }


    SDL_DestroyRenderer( state.renderer );
    SDL_DestroyWindow( state.window );

}
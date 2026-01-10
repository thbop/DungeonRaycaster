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

#include <stdint.h>
#include <SDL3/SDL.h>
#include <chelp/vector_t.h>

#include "vec2.h"
#include "ray.h"
#include "camera.h"


#define WINDOW_TITLE          "Dungeon"
#define WINDOW_WIDTH          1280
#define WINDOW_HEIGHT         720
#define SCREEN_WIDTH          320
#define SCREEN_HEIGHT         180
#define MAP_WIDTH             3
#define MAP_HEIGHT            3
#define CAMERA_ROTATION_DELTA 0.01f

const static char char_map[] = 
    "   "
    " # "
    "    ";

#define SDL_ASSERT( x ) \
    if ( !( x ) ) \
        SDL_Log( "ERROR: %s", SDL_GetError() )



static struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *screen;
    uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];

    vector_t map;
    camera_t camera;
} state;



uint32_t float_to_byte( float value ) {
    return SDL_clamp( value, 0.0f, 1.0f ) * 255;
}

void set_pixel( int x, int y, float r, float g, float b ) {
    uint32_t color =
        0x000000FF               |
        float_to_byte( r ) << 24 |
        float_to_byte( g ) << 16 |
        float_to_byte( b ) << 8;
    
    state.pixels[ y * SCREEN_WIDTH + x ] = color;
}

char char_map_get_tile( int x, int y ) {
    if ( x < 0 || x >= MAP_WIDTH )  return ' ';
    if ( y < 0 || y >= MAP_HEIGHT ) return ' ';

    return char_map[ y * MAP_WIDTH + x ];
}

ray_t get_ray_line( vec2 a, vec2 b ) {
    return (ray_t){
        .direction = vec2_sub( &b, &a ),
        .origin    = a,
    };
}

void generate_map() {
    state.map = new_vector( ray_t );

    for ( int j = 0; j < MAP_HEIGHT; j++ ) {
        for ( int i = 0; i < MAP_WIDTH; i++ ) {

            if ( char_map_get_tile( i - 1, j ) == ' ' ) {
                ray_t ray_line = get_ray_line( (vec2){ i, j }, (vec2){ i, j + 1 } );
                vector_append( state.map, ray_line );
            }
            if ( char_map_get_tile( i, j - 1 ) == ' ' ) {
                ray_t ray_line = get_ray_line( (vec2){ i, j }, (vec2){ i + 1, j } );
                vector_append( state.map, ray_line );
            }
            if ( char_map_get_tile( i + 1, j ) == ' ' ) {
                ray_t ray_line = get_ray_line( (vec2){ i + 1, j }, (vec2){ i + 1, j + 1 } );
                vector_append( state.map, ray_line );
            }
            if ( char_map_get_tile( i, j + 1 ) == ' ' ) {
                ray_t ray_line = get_ray_line( (vec2){ i + 1, j }, (vec2){ i + 1, j + 1 } );
                vector_append( state.map, ray_line );
            }

        }
    }
}

int main() {
    // Initialize
    SDL_ASSERT( SDL_Init( SDL_INIT_VIDEO ) );

    SDL_ASSERT( SDL_CreateWindowAndRenderer(
        WINDOW_TITLE,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0,
        &state.window,
        &state.renderer
    ) );

    SDL_ASSERT( state.screen = SDL_CreateTexture(
        state.renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    ) );

    generate_map();
    
    camera_new( &state.camera, VEC2_ZERO, CAMERA_ROTATION_DELTA );


    // Loop
    bool running = true;
    while ( running ) {
        // Handle events
        SDL_Event event;
        while ( SDL_PollEvent( &event ) ) {
            if ( event.type == SDL_EVENT_QUIT )
                running = false;
        }



        // Render pixels
        SDL_UpdateTexture( state.screen, NULL, state.pixels, SCREEN_WIDTH * sizeof( uint32_t ) );
        SDL_RenderTexture( state.renderer, state.screen, NULL, NULL );
        SDL_RenderPresent( state.renderer );

    }

    // Cleanup
    vector_free( state.map );

    SDL_DestroyTexture( state.screen );
    SDL_DestroyRenderer( state.renderer );
    SDL_DestroyWindow( state.window );

}
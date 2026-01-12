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

#include <stdio.h>
#include <float.h>
#include <stdint.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <chelp/vector_t.h>

#include "vec2.h"
#include "vec3.h"
#include "ray.h"
#include "matrix.h"
#include "camera.h"
#include "map.h"


#define WINDOW_TITLE       "Dungeon"
#define WINDOW_WIDTH       1280
#define WINDOW_HEIGHT      720
#define WINDOW_FPS         60

#define SCREEN_WIDTH       320
#define SCREEN_HEIGHT      180
#define SCREEN_HALF_WIDTH  ( SCREEN_WIDTH >> 1 )
#define SCREEN_HALF_HEIGHT ( SCREEN_HEIGHT >> 1 )

#define ASCII_MAP_WIDTH    16
#define ASCII_MAP_HEIGHT   16

#define START_POS          (vec2){ 1.5f, 1.5f }
#define FOV                90.0f
#define TURN_SPEED         2.0f
#define MOUSE_SENSITIVITY  0.34f
#define MOVE_SPEED         2.0f

#ifdef __INTELLISENSE__
#define constexpr
#endif

const static char char_map[] = 
    "###/############"
    "#     ##       #"
    "#     ##       #"
    "####  ######   #"
    "####           #"
    "#       ####   #"
    "#       ####   #"
    "#   ####       #"
    "#   ####       #"
    "#       ####   #"
    "#       ####   #"
    "####           #"
    "####  ######   #"
    "#       ##     #"
    "#       ##     #"
    "################";

#define SDL_ASSERT( x ) \
    if ( !( x ) ) \
        SDL_Log( "ERROR: %s", SDL_GetError() )



static struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *screen;
    uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];

    SDL_Surface *textures[TEXTURE_COUNT];

    map_t map;
    camera_t camera;

    bool mouse_grabbed;
} state;



uint32_t float_to_byte( float value ) {
    return SDL_clamp( value, 0.0f, 1.0f ) * 255;
}

float byte_to_float( uint8_t value ) {
    return (float)value * 0.0039215686f; // / 255.0f
}

void set_pixel( int x, int y, float r, float g, float b ) {
    if ( x < 0 || x > SCREEN_WIDTH - 1 )  return;
    if ( y < 0 || y > SCREEN_HEIGHT - 1 ) return;

    uint32_t color =
        float_to_byte( r ) << 16 |
        float_to_byte( g ) << 8  |
        float_to_byte( b );
    
    state.pixels[ y * SCREEN_WIDTH + x ] = color;
}


vec3 sample_texture( int texture_id, float u, float v ) {
    SDL_Surface *texture = state.textures[texture_id];

    int x = u * texture->w;
    int y = v * texture->h;

    uint32_t pixel = ( (uint32_t*)texture->pixels )[ y * texture->w + x ];
    return (vec3){
        byte_to_float( ( pixel       ) & 0xFF ),
        byte_to_float( ( pixel >> 8  ) & 0xFF ),
        byte_to_float( ( pixel >> 16 ) & 0xFF ),
    };
}

float calculate_light( wall_t *wall ) {
    return vec2_dot( &state.camera.view.direction, &wall->normal ) * 0.5f + 0.5f;
}

void rasterize_ray_cast( int x, float view_t, float wall_u, wall_t *wall ) {
    int half_wall_height = SCREEN_HALF_HEIGHT / view_t;

    float v_half_delta = 0.5f / half_wall_height;
    float v_half = 0.0f;

    float u = SDL_fmodf( wall_u * vec2_length( &wall->line.direction ), 1.0f );

    float light = calculate_light( wall );

    for ( int j = 0; j < half_wall_height; j++ ) {
        vec3
            color_high = sample_texture( wall->texture_id, u, 0.5f - v_half ),
            color_low  = sample_texture( wall->texture_id, u, 0.5f + v_half );
        color_high = vec3_mul_value( &color_high, light );
        color_low = vec3_mul_value( &color_low, light );
        
        set_pixel( x, SCREEN_HALF_HEIGHT + j, color_low.x, color_low.y, color_low.z );
        set_pixel( x, SCREEN_HALF_HEIGHT - j, color_high.x, color_high.y, color_high.z );

        v_half += v_half_delta;
    }
}

void ray_cast( int x, ray_t *ray ) {
    wall_t *closest_wall = NULL;
    float
        closest_t = FLT_MAX,
        wall_u;
    for ( int i = 0; i < state.map.walls.elementCount; i++ ) {
        wall_t *wall = _vector_at( &state.map.walls, i );

        vec2 intersect_result;
        if ( !ray_intersect( ray, &wall->line, &intersect_result ) ) continue;
        if ( intersect_result.x < 1.0e-12f ) continue;
        if ( intersect_result.y < 0.0f ) continue;
        if ( intersect_result.y > 1.0f ) continue;

        if ( intersect_result.x < closest_t ) {
            closest_wall = wall;
            closest_t = intersect_result.x;
            wall_u = intersect_result.y;
        }
    }

    if ( closest_wall != NULL ) { // If ray hit the nearest wall
        rasterize_ray_cast( x, closest_t, wall_u, closest_wall );
    }
}

void cast_rays() {
    vec2 sensor_left, sensor_right; {
        vec2
            _sensor_center = vec2_add( &state.camera.view.origin, &state.camera.view.direction ),
            _left_edge_delta = vec2_mul_value( &state.camera.perpendicular_direction, state.camera.half_sensor_width ),
            _right_edge_delta = vec2_mul_value( &state.camera.perpendicular_direction, -state.camera.half_sensor_width );
        sensor_left = vec2_add( &_sensor_center, &_left_edge_delta ),
        sensor_right = vec2_add( &_sensor_center, &_right_edge_delta );
    }

    float
        lerp_t_delta = 1.0f / SCREEN_WIDTH,
        lerp_t = 0.0f;
    
    ray_t ray = {
        .origin = state.camera.view.origin,
    };

    for ( int i = 0; i < SCREEN_WIDTH; i++ ) {
        vec2 sensor_target = vec2_lerp( &sensor_left, &sensor_right, lerp_t );
        ray.direction = vec2_sub( &sensor_target, &state.camera.view.origin );

        ray_cast( i, &ray );

        lerp_t += lerp_t_delta;
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

    SDL_SetWindowRelativeMouseMode( state.window, true );
    state.mouse_grabbed = true;

    SDL_ASSERT( state.screen = SDL_CreateTexture(
        state.renderer,
        SDL_PIXELFORMAT_XRGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    ) );
    SDL_SetTextureScaleMode( state.screen, SDL_SCALEMODE_NEAREST );

    state.textures[TEXTURE_BRICK] = IMG_Load( "../assets/textures/brick.png" );
    state.textures[TEXTURE_DOOR] = IMG_Load( "../assets/textures/door.png" );

    map_initialize( &state.map );
    ascii_map_t ascii_map = {
        .data   = char_map,
        .width  = ASCII_MAP_WIDTH,
        .height = ASCII_MAP_HEIGHT,
    };
    map_generate_from_ascii( &state.map, &ascii_map );
    // map_generate_polygon( &state.map, VEC2_ZERO, 5.0f, 8 );

    camera_settings_t camera_settings = {
        .position       = START_POS,
        .rotation_speed = TURN_SPEED,
        .fov            = FOV,
        .move_speed     = MOVE_SPEED,
    };
    camera_new( &state.camera, &camera_settings );

    uint64_t time, last_time = SDL_GetTicks();
    constexpr uint64_t frame_time_target = 1000 / WINDOW_FPS;
    float delta_time = 0.001f;

    // Loop
    bool running = true;
    while ( running ) {
        // Handle events
        SDL_Event event;
        while ( SDL_PollEvent( &event ) ) {
            switch ( event.type ) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if ( event.key.scancode == SDL_SCANCODE_ESCAPE ) {
                        SDL_SetWindowRelativeMouseMode( state.window, false );
                        state.mouse_grabbed = false;
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if ( !state.mouse_grabbed ) {
                        SDL_SetWindowRelativeMouseMode( state.window, true );
                        state.mouse_grabbed = true;
                    }
                    break;
            }
        }

        // Handle input
        float mouse_delta_x;
        SDL_GetRelativeMouseState( &mouse_delta_x, NULL );
        if ( SDL_fabsf( mouse_delta_x ) > 0.001 && state.mouse_grabbed )
            camera_rotate( &state.camera, mouse_delta_x * MOUSE_SENSITIVITY, delta_time );

        const bool *keys = SDL_GetKeyboardState( NULL );
        if ( keys[SDL_SCANCODE_LEFT]  ) camera_turn_left( &state.camera, delta_time ); 
        if ( keys[SDL_SCANCODE_RIGHT] ) camera_turn_right( &state.camera, delta_time );
        if ( keys[SDL_SCANCODE_W] )     camera_move_forward( &state.camera, delta_time );
        if ( keys[SDL_SCANCODE_S] )     camera_move_backward( &state.camera, delta_time );
        if ( keys[SDL_SCANCODE_A] )     camera_move_left( &state.camera, delta_time );
        if ( keys[SDL_SCANCODE_D] )     camera_move_right( &state.camera, delta_time );

        memset( state.pixels, 0, sizeof( state.pixels ) ); // Clear screen
        cast_rays();

        // Render pixels
        SDL_UpdateTexture( state.screen, NULL, state.pixels, SCREEN_WIDTH * sizeof( uint32_t ) );
        SDL_RenderTexture( state.renderer, state.screen, NULL, NULL );
        SDL_RenderPresent( state.renderer );

        time = SDL_GetTicks();
        uint64_t frame_time = time - last_time;
        delta_time = frame_time / 1000.0f;
        uint64_t delay = frame_time_target - SDL_min( frame_time, frame_time_target );
        SDL_Delay( delay );
        last_time = time;

    }

    // Cleanup
    for ( int i = 0; i < TEXTURE_COUNT; i++ )
        SDL_DestroySurface( state.textures[i] );

    map_free( &state.map );

    SDL_DestroyTexture( state.screen );
    SDL_DestroyRenderer( state.renderer );
    SDL_DestroyWindow( state.window );
    SDL_Quit();

    return 0;

}
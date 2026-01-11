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

#include "map.h"

uint32_t _wall_ascii_to_texture( char ascii ) {
    switch ( ascii ) {
        case '#': return TEXTURE_BRICK;
    }

    return ' ';
}

void _wall_new( vec2 a, vec2 b, uint32_t texture_id, wall_t *wall ) {
    vec2 
        direction = vec2_sub( &b, &a ),
        perpendicular = (vec2){ -direction.y, direction.x };
    
    *wall = (wall_t){
        .line = (ray_t){
            .direction = direction,
            .origin    = a,
        },
        .normal = vec2_normalize( &perpendicular ),
        .texture_id = texture_id,
    };
}

char _ascii_map_get_tile( ascii_map_t *ascii_map, int x, int y ) {
    if ( x < 0 || x > ascii_map->width - 1 )  return ' ';
    if ( y < 0 || y > ascii_map->height - 1 ) return ' ';

    return ascii_map->data[ y * ascii_map->width + x ];
}

void map_generate_from_ascii( ascii_map_t *ascii_map, map_t *map ) {
    map->walls = new_vector( wall_t );

    for ( int j = 0; j < ascii_map->height; j++ ) {
        for ( int i = 0; i < ascii_map->width; i++ ) {
            wall_t wall;
            char ascii = _ascii_map_get_tile( ascii_map, i, j );
            uint32_t texture_id =  _wall_ascii_to_texture( ascii );
            if ( ascii != ' ' ) {
                if ( _ascii_map_get_tile( ascii_map, i - 1, j ) == ' ' ) {
                    _wall_new( (vec2){ i, j + 1 }, (vec2){ i, j }, texture_id, &wall );
                    vector_append( map->walls, wall );
                }
                if ( _ascii_map_get_tile( ascii_map, i, j - 1 ) == ' ' ) {
                    _wall_new( (vec2){ i, j }, (vec2){ i + 1, j }, texture_id, &wall );
                    vector_append( map->walls, wall );
                }
                if ( _ascii_map_get_tile( ascii_map, i + 1, j ) == ' ' ) {
                    _wall_new( (vec2){ i + 1, j }, (vec2){ i + 1, j + 1 }, texture_id, &wall );
                    vector_append( map->walls, wall );
                }
                if ( _ascii_map_get_tile( ascii_map, i, j + 1 ) == ' ' ) {
                    _wall_new( (vec2){ i + 1, j + 1 }, (vec2){ i, j + 1 }, texture_id, &wall );
                    vector_append( map->walls, wall );
                }
            }
        }
    }
}

void map_free( map_t *map ) {
    vector_free( map->walls );
}
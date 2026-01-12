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

#include "camera.h"

void camera_new( camera_t *camera, camera_settings_t *settings ) {
    float deg2rad   = SDL_PI_F / 180.0f;


    *camera = (camera_t){
        .view = (ray_t){
            .origin = settings->position,
            .direction = (vec2){ 1.0f, 0.0f },
        },

        .perpendicular_direction = (vec2){ 0.0f, 1.0f },

        .half_sensor_width = SDL_tanf( settings->fov * deg2rad * 0.5f ),

        ._rotation_speed = settings->rotation_speed,
        ._move_speed = settings->move_speed,
    };
}

void camera_rotate( camera_t *camera, float delta, float delta_time ) {
    float
        sin_theta = SDL_sinf( delta * delta_time ), 
        cos_theta = SDL_cosf( delta * delta_time );
    
    float rotate[] = {
         cos_theta,  sin_theta,
        -sin_theta,  cos_theta,
    };
    camera->view.direction = mat2_transform( rotate, &camera->view.direction );
    camera->perpendicular_direction = mat2_transform( rotate, &camera->perpendicular_direction );
}

void camera_turn_right( camera_t *camera, float delta_time ) {
    camera_rotate( camera, camera->_rotation_speed, delta_time );
}

void camera_turn_left( camera_t *camera, float delta_time ) {
    camera_rotate( camera, -camera->_rotation_speed, delta_time );
}

void camera_move_forward( camera_t *camera, float delta_time ) {
    vec2 delta = vec2_mul_value( &camera->view.direction, camera->_move_speed * delta_time );
    camera->view.origin = vec2_add( &camera->view.origin, &delta );
}

void camera_move_backward( camera_t *camera, float delta_time ) {
    vec2 delta = vec2_mul_value( &camera->view.direction, -camera->_move_speed * delta_time );
    camera->view.origin = vec2_add( &camera->view.origin, &delta );
}

void camera_move_left( camera_t *camera, float delta_time ) {
    vec2 delta = vec2_mul_value( &camera->perpendicular_direction, camera->_move_speed * delta_time );
    camera->view.origin = vec2_add( &camera->view.origin, &delta );
}

void camera_move_right( camera_t *camera, float delta_time ) {
    vec2 delta = vec2_mul_value( &camera->perpendicular_direction, -camera->_move_speed * delta_time );
    camera->view.origin = vec2_add( &camera->view.origin, &delta );
}
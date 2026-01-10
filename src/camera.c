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

void camera_new( camera_t *camera, vec2 position, float rotation_delta ) {
    float
        sin_theta = SDL_sinf( rotation_delta ), // sin(-x) = -sin(x) 
        cos_theta = SDL_cosf( rotation_delta ); // cos(-x) =  cos(x)
    
    *camera = (camera_t){
        .view = (ray_t){
            .origin = position,
            .direction = (vec2){ 1.0f, 0.0f },
        },

        ._rotate_left = {
             cos_theta, -sin_theta,
             sin_theta,  cos_theta,
        },
        ._rotate_right = {
             cos_theta,  sin_theta,
            -sin_theta,  cos_theta,
        },
    };
}

void camera_rotate_right( camera_t *camera ) {
    mat2_transform( camera->_rotate_right, &camera->view.direction );
}

void camera_rotate_left( camera_t *camera ) {
    mat2_transform( camera->_rotate_left, &camera->view.direction );
}
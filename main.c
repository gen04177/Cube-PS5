#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CUBE_SIZE 1.0f
#define ROTATE_SPEED 0.01f

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float x, y;
} Vec2;

void draw_line(SDL_Renderer* renderer, Vec2 p1, Vec2 p2) {
    SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
}

void draw_cube(SDL_Renderer* renderer, Vec2* projected) {
    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    for (int i = 0; i < 12; ++i) {
        draw_line(renderer, projected[edges[i][0]], projected[edges[i][1]]);
    }
}

void project(Vec3* vertices, Vec2* projected, int count, float fov, float viewer_distance, int window_width, int window_height) {
    for (int i = 0; i < count; ++i) {
        float factor = fov / (viewer_distance + vertices[i].z);
        projected[i].x = vertices[i].x * factor + window_width / 2;
        projected[i].y = -vertices[i].y * factor + window_height / 2;
    }
}

void rotate(Vec3* vertices, int count, float angleY) {
    for (int i = 0; i < count; ++i) {
        // Rotação em Y
        float x = vertices[i].x * cos(angleY) + vertices[i].z * sin(angleY);
        float z = -vertices[i].x * sin(angleY) + vertices[i].z * cos(angleY);
        vertices[i].x = x;
        vertices[i].z = z;
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Rotating Cube",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          0, 0,
                                          SDL_WINDOW_FULLSCREEN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);

    Vec3 vertices[8] = {
        {-CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE},
        { CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE},
        { CUBE_SIZE,  CUBE_SIZE, -CUBE_SIZE},
        {-CUBE_SIZE,  CUBE_SIZE, -CUBE_SIZE},
        {-CUBE_SIZE, -CUBE_SIZE,  CUBE_SIZE},
        { CUBE_SIZE, -CUBE_SIZE,  CUBE_SIZE},
        { CUBE_SIZE,  CUBE_SIZE,  CUBE_SIZE},
        {-CUBE_SIZE,  CUBE_SIZE,  CUBE_SIZE}
    };

    float rotationY = 0.0f;
    int running = 1;
    SDL_Event event;

    Uint32 start_time = SDL_GetTicks(), end_time;
    int frame_count = 0;
    float fps = 0.0f;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        Vec3 transformed_vertices[8];
        memcpy(transformed_vertices, vertices, sizeof(vertices));

        rotate(transformed_vertices, 8, rotationY);

        Vec2 projected_vertices[8];
        project(transformed_vertices, projected_vertices, 8, 256, 4, window_width, window_height);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        draw_cube(renderer, projected_vertices);

        SDL_RenderPresent(renderer);

        frame_count++;
        end_time = SDL_GetTicks();
        if (end_time - start_time >= 1000) {
            fps = frame_count / ((end_time - start_time) / 1000.0f);
            frame_count = 0;
            start_time = end_time;
        }

        rotationY += ROTATE_SPEED;

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

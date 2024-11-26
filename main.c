#include <GL/gl.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "graphics.h"

#define ITEM_COUNT 50

int windowWidth = 800;
int windowHeight = 600;

float carX = 0.0f; // Car's initial position (centered on the road)
float carZ = 10.0f;
bool finished = 0;

const float leftPosition = -2.0f;
const float centerPosition = 0.0f;
const float rightPosition = 2.0f;

float roadWidth = 6.0f;    // Width of the road to fit three lanes
float roadLength = 1000.0f; // Length of the road to extend along the z-axis
float laneWidth = 2.0f;    // Width of each lane (same as car’s x-move distance)

typedef struct {
    float x, y, z;
    bool collected;
} Item;

Item items[ITEM_COUNT];

float roadOffset = 0.0f; // Controls the road's scrolling effect
float roadSpeed = 0.35f; // Speed at which the road scrolls backward
float maxSpeed = 0.65f;
float minSpeed = 0.25f;

float cameraPosition[] = {50.0f, 50.0f, 50.0f};
GLfloat lightPos[4] = {5.0f, 10.0f, 5.0f, 1.0f}; 
GLfloat roadPlane[4] = {0.0f, 1.0f, 0.0f, 0.5f};

Mix_Chunk *coinSound = NULL;  
Mix_Chunk *hornSound = NULL;  
Mix_Chunk *endSound = NULL;  

void initGame() {
    srand(time(NULL));
    int itemPosition = -40.0f;

    for (int i = 0; i < ITEM_COUNT; i++) {
        int diceValue = (rand() % 6) + 1;

        if (diceValue > 5) {
            continue;
        }

        if (diceValue <= 2) {
            items[i].x = -2.0f;
        } else if (diceValue <= 4) {
            items[i].x = 0.0f;
        } else {
            items[i].x = 2.0f;
        }

        itemPosition -= diceValue * (3.0f + (float) diceValue);
        items[i].z = itemPosition;

        items[i].y = 1.0f;
        items[i].collected = false;
    }
}

int checkCollision(float carX, float carZ, float itemX, float itemZ) {
    float tolerance = 0.5f;

    if (carZ <= itemZ + tolerance && carZ >= itemZ - tolerance) {
        if (carX <= itemX + tolerance && carX >= itemX - tolerance) {
            return 1;
        }
    }

    return 0;
}

void updateRoadOffset() {
    roadOffset += roadSpeed;

    if (roadOffset >= roadLength - 5.0f) {

        finished = true;
        Mix_PlayChannel(-1, endSound, 0); // Play sound on collision
    }
}

void idle() {
    glutPostRedisplay(); // Continuously request redraw
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2], 0.0, 0.0,
              0.0,          // Look-at point
              0.0, 1.0, 0.0 // Up vector
    );

    if (!finished) {
        updateRoadOffset();
    }

    drawGround(roadWidth, roadOffset, roadLength);
    drawRoad(roadWidth, roadOffset, roadLength);
    drawLaneLines(roadOffset, roadLength);
    drawFinishLine(roadWidth, roadOffset, roadLength);

    for (int i = 0; i < ITEM_COUNT; i++) {
        if (checkCollision(carX, carZ, items[i].x, items[i].z)) {
            if (!items[i].collected) {
                Mix_PlayChannel(-1, coinSound, 0); // Play sound on collision
                items[i].collected = true;
            }
        }

        if (items[i].collected) {
            continue;
        }

        items[i].z += roadSpeed;
        drawFloatingSphere(items[i].x, items[i].y, items[i].z);
    }

    drawCar(carX, carZ);

    glutSwapBuffers();
}

void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case 'a': // Move car to the left
        if (carX > leftPosition) {
            carX -= 2.0f; // Shift to the left position
        }
        break;
    case 'd': // Move car to the right
        if (carX < rightPosition) {
            carX += 2.0f; // Shift to the right position
        }
        break;
    case 'w':
        if (roadSpeed < maxSpeed) {
            roadSpeed += 0.2f;
        }
        break;
    case 's':
        if (roadSpeed > minSpeed) {
            roadSpeed -= 0.2f;
        }
        break;
    case 'r':
        cameraPosition[0] = -cameraPosition[0];
        break;
    }
    glutPostRedisplay(); // Redraw the scene with updated car position
}

void initAudio() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Failed to initialize SDL audio: %s\n", SDL_GetError());
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        exit(1);
    }

    coinSound = Mix_LoadWAV("assets/audio/ponto.mp3");
    if (!coinSound) {
        fprintf(stderr, "Failed to load coin sound effect: %s\n", Mix_GetError());
        exit(1);
    }

    hornSound = Mix_LoadWAV("assets/audio/buzina-carro.mp3");
    if (!hornSound) {
        fprintf(stderr, "Failed to load horn sound effect: %s\n", Mix_GetError());
        exit(1);
    }

    endSound = Mix_LoadWAV("assets/audio/fim.mp3");
    if (!endSound) {
        fprintf(stderr, "Failed to load horn sound effect: %s\n", Mix_GetError());
        exit(1);
    }
}

void cleanupAudio() {
    Mix_FreeChunk(coinSound);
    Mix_FreeChunk(hornSound);
    coinSound = NULL;
    hornSound = NULL;
    Mix_Quit();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Jogo 3D");

    initGame();
    initOpenGL();
    setProjection();
    initAudio();

    Mix_PlayChannel(-1, hornSound, 0); // Play the horn sound at the start

    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeypress);
    glutIdleFunc(idle); // Register idle function for continuous movement

    atexit(cleanupAudio); // Ensure resources are freed on exit

    glutMainLoop();
    return 0;
}

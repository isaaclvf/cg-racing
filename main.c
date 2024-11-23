#include <GL/gl.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
GLfloat lightPos[4] = {5.0f, 10.0f, 5.0f, 1.0f}; // Position of the light source
GLfloat roadPlane[4] = {0.0f, 1.0f, 0.0f,
                        0.5f}; // The plane equation for y = -0.5

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
  // Update roadSpeed to create acceleration effect

  if (roadOffset >= roadLength - 5.0f) {
    finished = true;
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
      items[i].collected = true;
    }

    if (items[i].collected)
      continue;

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
  x++;
  y++;
  glutPostRedisplay(); // Redraw the scene with updated car position
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Jogo 3D");

  initGame();
  initOpenGL();
  setProjection();

  // Register callback functions
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(handleKeypress);
  glutIdleFunc(idle); // Register idle function for continuous movement

  glutMainLoop();
  return 0;
}

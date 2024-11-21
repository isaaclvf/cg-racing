#include <GL/gl.h>
#include <GL/glut.h>
#include <stdbool.h>

#include "graphics.h"

// typedef struct {
//   float x, y, z;
//   float radius;
//   bool collected;
// } Item;
//
// #define MAX_ITEMS 100
// Item items[MAX_ITEMS];
// int itemCount = 0;

int windowWidth = 800;
int windowHeight = 600;

float carX = 0.0f; // Car's initial position (centered on the road)
float carZ = 10.0f;

const float leftPosition = -2.0f;
const float centerPosition = 0.0f;
const float rightPosition = 2.0f;

float roadWidth = 6.0f;    // Width of the road to fit three lanes
float roadLength = 500.0f; // Length of the road to extend along the z-axis
float laneWidth = 2.0f;    // Width of each lane (same as car’s x-move distance)

typedef struct {
  float x, y, z;
  bool collected;
} Item;

Item items[3] = {{.x = 0.0f, .y = 1.0f, .z = -40.0f, .collected = false},
                 {.x = 2.0f, .y = 1.0f, .z = -65.0f, .collected = false},
                 {.x = -2.0f, .y = 1.0f, .z = -90.0f, .collected = false}};

float roadOffset = 0.0f; // Controls the road's scrolling effect
float roadSpeed = 0.35f; // Speed at which the road scrolls backward
float maxSpeed = 0.65f;
float minSpeed = 0.25f;

GLfloat lightPos[4] = {5.0f, 10.0f, 5.0f, 1.0f}; // Position of the light source
GLfloat roadPlane[4] = {0.0f, 1.0f, 0.0f,
                        0.5f}; // The plane equation for y = -0.5

// int checkCollision(float px, float py, float pz, float pSize, Item item) {
//   if (item.collected)
//     return 0; // Skip if already collected
//
//   float dx = px - item.x;
//   float dy = py - item.y;
//   float dz = pz - item.z;
//
//   float distance = sqrt(dx * dx + dy * dy + dz * dz);
//   return distance < (pSize / 2.0f + item.radius);
// }

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
    roadOffset = 0.0f;
  }
}

void idle() {
  glutPostRedisplay(); // Continuously request redraw
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  gluLookAt(5.0, 5.0, 10.0, // Camera position
            0.0, 0.0, 0.0,  // Look-at point
            0.0, 1.0, 0.0   // Up vector
  );

  updateRoadOffset();

  drawGround(roadWidth, roadOffset, roadLength);
  drawRoad(roadWidth, roadOffset, roadLength);
  drawLaneLines(roadOffset, roadLength);

  for (int i = 0; i < 3; i++) {
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
  }
  glutPostRedisplay(); // Redraw the scene with updated car position
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Jogo 3D");

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

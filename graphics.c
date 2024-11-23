#include <GL/gl.h>
#include <GL/glut.h>

#include "graphics.h"

void initOpenGL() {
  // Set the background color (RGBA)
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background

  // Enable depth testing for z-buffering
  glEnable(GL_DEPTH_TEST);

  // Enable backface culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Enable lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0); // Default light source

  // Configure light properties
  GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat diffuseLight[] = {0.8f, 0.8f, 0.8f, 1.0f};
  GLfloat specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat lightPosition[] = {0.0f, 10.0f, 5.0f, 1.0f};

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void setRoadMaterial() {
  GLfloat ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
  GLfloat diffuse[] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
  GLfloat shininess = 10.0f;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void setLaneLineMaterial() {
  GLfloat ambient[] = {0.25f, 0.25f, 0.0f, 1.0f};
  GLfloat diffuse[] = {1.0f, 1.0f, 0.0f, 1.0f};
  GLfloat specular[] = {0.5f, 0.5f, 0.4f, 1.0f};
  GLfloat shininess = 30.0f;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void setFinishLineMaterial() {
  GLfloat ambient[] = {0.8f, 0.8f, 0.8f, 1.0f};
  GLfloat diffuse[] = {0.9f, 0.9f, 0.9f, 1.0f};
  GLfloat specular[] = {0.9f, 0.9f, 0.9f, 1.0f};
  GLfloat shininess = 10.0f;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void setGroundMaterial() {
  GLfloat ambient[] = {0.1f, 0.5f, 0.1f, 1.0f};
  GLfloat diffuse[] = {0.2f, 0.9f, 0.2f, 1.0f};
  GLfloat specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat shininess = 15.0f;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void setGoldMaterial() {
  GLfloat ambient[] = {0.24725f, 0.1995f, 0.0745f, 1.0f};
  GLfloat diffuse[] = {0.75164f, 0.60648f, 0.22648f, 1.0f};
  GLfloat specular[] = {0.628281f, 0.555802f, 0.366065f, 1.0f};
  GLfloat shininess = 51.2f;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void setCarMaterial() {
  GLfloat ambient[] = {0.3f, 0.0f, 0.0f, 1.0f};
  GLfloat diffuse[] = {0.8f, 0.0f, 0.0f, 1.0f};
  GLfloat specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat shininess = 50.0f;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void drawGround(float roadWidth, float roadOffset, float roadLength) {
  setGroundMaterial();
  glBegin(GL_QUADS);
  glVertex3f(8 * (-roadWidth / 2), -0.6f, 2 * (20.0f + roadOffset));
  glVertex3f(8 * (roadWidth / 2), -0.6f, 2 * (20.0f + roadOffset));
  glVertex3f(8 * (roadWidth / 2), -0.6f,
             2 * (-20.0f + -roadLength + roadOffset));
  glVertex3f(8 * (-roadWidth / 2), -0.6f,
             2 * (-20.0f + -roadLength + roadOffset));
  glEnd();
}

void drawRoad(float roadWidth, float roadOffset, float roadLength) {
  setRoadMaterial();
  glBegin(GL_QUADS);
  glVertex3f(-roadWidth / 2, -0.5f, 30.0f + roadOffset);
  glVertex3f(roadWidth / 2, -0.5f, 30.0f + roadOffset);
  glVertex3f(roadWidth / 2, -0.5f, -30.0f + -roadLength + roadOffset);
  glVertex3f(-roadWidth / 2, -0.5f, -30.0f + -roadLength + roadOffset);
  glEnd();
}

void drawLaneLines(float roadOffset, float roadLength) {
  setLaneLineMaterial();
  for (float z = 30.0f + roadOffset; z > -30.0f + -roadLength + roadOffset;
       z -= 5.0f) {
    glLineWidth(12.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, -0.49f, z);
    glVertex3f(0.0f, -0.49f, z - 2.0f);
    glEnd();
  }
}

void drawFinishLine(float roadWidth, float roadOffset, float roadLength) {
  setFinishLineMaterial();

  glBegin(GL_QUADS);
  // (temporary) 10.0f is the carZ offset
  glVertex3f(-roadWidth / 2, -0.47f, -(roadLength - 5.0f) + 1.0f + 10.0f + roadOffset); 
  glVertex3f(roadWidth / 2, -0.47f, -(roadLength - 5.0f) + 1.0f + 10.0f + roadOffset);
  glVertex3f(roadWidth / 2, -0.47f, -(roadLength - 5.0f) -1.0f + 10.0f + roadOffset);
  glVertex3f(-roadWidth / 2, -0.47f, -(roadLength - 5.0f) -1.0f + 10.0f + roadOffset);
  glEnd();
}

void drawFloatingSphere(float sphereX, float sphereY, float sphereZ) {
  glPushMatrix();
  setGoldMaterial();
  glTranslatef(sphereX, sphereY, sphereZ); // Position the sphere
  // glutSolidSphere(0.5f, 32, 32);        // Draw sphere with radius 0.5
  glutSolidOctahedron();
  glPopMatrix();
}

void drawCar(float carX, float carZ) {
  glPushMatrix();
  setCarMaterial();
  glTranslatef(carX, 0.2f, carZ);
  glutSolidCube(1.0);
  glPopMatrix();
}

void setProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  setProjection();
}

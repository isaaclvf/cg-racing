#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "graphics.h"

typedef struct {
  float x, y, z;
} Vertex;

typedef struct {
  int v1, v2, v3;
} Face;

typedef struct {
  Vertex *vertices;
  Face *faces;
  int vertexCount;
  int faceCount;
} Model;

Model loadOBJ(const char *filename) {
  Model model = {0};
  FILE *file = fopen(filename, "r");

  if (!file) {
    fprintf(stderr, "Error: Cannot open file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  char line[128];
  int vertexCount = 0, faceCount = 0;

  // Passo 1: Contar vértices e faces
  while (fgets(line, sizeof(line), file)) {
    if (strncmp(line, "v ", 2) == 0) {
      vertexCount++;
    } else if (strncmp(line, "f ", 2) == 0) {
      faceCount++;
    }
  }

  model.vertices = (Vertex *)malloc(sizeof(Vertex) * vertexCount);
  if (!model.vertices) {
    fprintf(stderr, "Error: Could not allocate memory for vertices\n");
    exit(EXIT_FAILURE);
  }

  model.faces = (Face *)malloc(sizeof(Face) * faceCount);
  if (!model.faces) {
    fprintf(stderr, "Error: Could not allocate memory for faces\n");
    exit(EXIT_FAILURE);
  }

  model.vertexCount = vertexCount;
  model.faceCount = faceCount;

  rewind(file);
  int vIndex = 0, fIndex = 0;

  while (fgets(line, sizeof(line), file)) {
    if (strncmp(line, "v ", 2) == 0) {
      sscanf(line, "v %f %f %f", &model.vertices[vIndex].x,
             &model.vertices[vIndex].y, &model.vertices[vIndex].z);
      vIndex++;
    } else if (strncmp(line, "f ", 2) == 0) {
      int v1, v2, v3;
      // Tentando ler as faces em vários formatos possíveis
      if (sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &v1, &v2, &v3) ==
              3 || // Formato com textura e normal
          sscanf(line, "f %d//%*d %d//%*d %d//%*d", &v1, &v2, &v3) ==
              3 || // Formato com normal apenas
          sscanf(line, "f %d %d %d", &v1, &v2, &v3) == 3) { // Formato simples
        if (v1 <= 0 || v1 > model.vertexCount || v2 <= 0 ||
            v2 > model.vertexCount || v3 <= 0 || v3 > model.vertexCount) {
          fprintf(stderr, "Error: Invalid face index in OBJ file\n");
          exit(EXIT_FAILURE);
        }
        model.faces[fIndex].v1 = v1;
        model.faces[fIndex].v2 = v2;
        model.faces[fIndex].v3 = v3;
        fIndex++;
      } else {
        fprintf(stderr, "Error: Invalid face format in OBJ file: %s\n", line);
        exit(EXIT_FAILURE);
      }
    }
  }

  fclose(file);
  return model;
}

Vertex calculateFaceNormal(Vertex v1, Vertex v2, Vertex v3) {
    // Compute two edges
    float edge1x = v2.x - v1.x;
    float edge1y = v2.y - v1.y;
    float edge1z = v2.z - v1.z;

    float edge2x = v3.x - v1.x;
    float edge2y = v3.y - v1.y;
    float edge2z = v3.z - v1.z;

    // Cross product
    Vertex normal;
    normal.x = edge1y * edge2z - edge1z * edge2y;
    normal.y = edge1z * edge2x - edge1x * edge2z;
    normal.z = edge1x * edge2y - edge1y * edge2x;

    // Vertexize the result
    float length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    if (length != 0) {
        normal.x /= length;
        normal.y /= length;
        normal.z /= length;
    }

    return normal;
}


void renderModel(const Model *model) {
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < model->faceCount; i++) {
    Face face = model->faces[i];

    // OBJ usa índices baseados em 1, ajustar para 0
    Vertex v1 = model->vertices[face.v1 - 1];
    Vertex v2 = model->vertices[face.v2 - 1];
    Vertex v3 = model->vertices[face.v3 - 1];

    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);
  }
  glEnd();
}

void renderModelFlatShading(const Model *model) {
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < model->faceCount; i++) {
        Face face = model->faces[i];

        Vertex v1 = model->vertices[face.v1 - 1];
        Vertex v2 = model->vertices[face.v2 - 1];
        Vertex v3 = model->vertices[face.v3 - 1];

        // Calculate the face normal
        Vertex faceNormal = calculateFaceNormal(v1, v2, v3);

        // Use the same normal for all vertices
        glNormal3f(faceNormal.x, faceNormal.y, faceNormal.z);
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
    }
    glEnd();
}

void initOpenGL() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
  glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-buffering
  glEnable(GL_CULL_FACE);               // Enable backface culling
  glCullFace(GL_BACK);

  glEnable(GL_LIGHTING); // Enable lighting
  glEnable(GL_LIGHT0);   // Default light source

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
  // Material rosa
  GLfloat ambient[] = {0.3f, 0.0f, 0.3f, 1.0f};  // Rosa suave como ambiente
  GLfloat diffuse[] = {1.0f, 0.0f, 1.0f, 1.0f};  // Cor rosa para o material
  GLfloat specular[] = {0.5f, 0.0f, 0.5f, 1.0f}; // Reflexão especular rosa
  GLfloat shininess = 50.0f;                     // Brilho do material

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
  glVertex3f(-roadWidth / 2, -0.47f,
             -(roadLength - 5.0f) + 1.0f + 10.0f + roadOffset);
  glVertex3f(roadWidth / 2, -0.47f,
             -(roadLength - 5.0f) + 1.0f + 10.0f + roadOffset);
  glVertex3f(roadWidth / 2, -0.47f,
             -(roadLength - 5.0f) - 1.0f + 10.0f + roadOffset);
  glVertex3f(-roadWidth / 2, -0.47f,
             -(roadLength - 5.0f) - 1.0f + 10.0f + roadOffset);
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
  static Model carModel = {0};
  static int isModelLoaded = 0;

  if (!isModelLoaded) {
    carModel =
        loadOBJ("assets/model/vaca.obj"); // Carregar o modelo apenas uma vez
    if (carModel.vertexCount == 0 || carModel.faceCount == 0) {
      fprintf(stderr, "Error: Car model not loaded correctly!\n");
      exit(EXIT_FAILURE);
    }
    isModelLoaded = 1;
  }

  glPushMatrix();
  setCarMaterial();               // Aplicando o material rosa
  glTranslatef(carX, 1.2f, carZ); // Ajustando a posição do carro
  glScalef(0.5f, 0.5f, 0.5f);     // Aumentando o tamanho do carro para testes
  glRotatef(90, 0, 1, 0);
  // renderModel(&carModel); // Renderizando o modelo do carro
  renderModelFlatShading(&carModel);
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

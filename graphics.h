#ifndef GRAPHICS_H
#define GRAPHICS_H

void initOpenGL();
void setRoadMaterial();
void setLaneLineMaterial();
void setGroundMaterial();
void setGoldMaterial();
void setCarMaterial();
void drawGround(float roadWidth, float roadOffset, float roadLength);
void drawRoad(float roadWidth, float roadOffset, float roadLength);
void drawLaneLines(float roadOffset, float roadLength);
void drawFloatingSphere(float sphereX, float sphereY, float sphereZ);
void drawCar(float carX, float carZ);
void setProjection();
void reshape(int width, int height);

#endif

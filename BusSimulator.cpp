#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

bool shadowEnabled = true;

float angle = 0.0, deltaAngle = 0.0, ratio;
float x = 0.0f, y = 2.0f, z = 10.0f;
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
int deltaMove = 0, h, w;

float busX = 0.0f;
float busZ = 0.0f;
float busSpeed = 0.2f;
float busRotation = 0.0f;

int gameOver = 0;
int gameScore = 0;
float gameStartTime = 0;

float busStopX = 5.0f;
float busStopZ = -3.5f;
GLfloat busStopRoof[] = {0.7f, 0.1f, 0.1f};
GLfloat busStopSupport[] = {0.3f, 0.3f, 0.3f};
GLfloat busStopBench[] = {0.5f, 0.35f, 0.05f};
float worldSize = 100.0f;
float roadSegmentLength = 10.0f;

#define MAX_PEDESTRIANS 10
typedef struct {
    float x, z;
    float destX, destZ;
    float rotation;
    float speed;
    float size;
    int moveState;
    int waitTime;
    int waitCounter;
    GLfloat shirtColor[3];
    GLfloat pantsColor[3];
} Pedestrian;
Pedestrian pedestrians[MAX_PEDESTRIANS];

#define MAX_OBSTACLES 15
typedef struct {
    float x, z;
    float width, height, depth;
    int type;
    GLfloat color[3];
    int active;
} Obstacle;
Obstacle obstacles[MAX_OBSTACLES];

GLfloat skyBlue[] = {0.5f, 0.7f, 1.0f};
GLfloat roadGray[] = {0.2f, 0.2f, 0.2f};
GLfloat roadYellow[] = {1.0f, 1.0f, 0.0f};
GLfloat buildingColor[] = {0.5f, 0.5f, 0.7f};
GLfloat windowColor[] = {0.7f, 0.85f, 0.9f};
GLfloat trunkColor[] = {0.5f, 0.3f, 0.1f};
GLfloat leafColor[] = {0.0f, 0.8f, 0.0f};
GLfloat busBlue[] = {0.0f, 0.0f, 1.0f};
GLfloat busGlass[] = {0.3f, 0.3f, 0.5f, 0.7f};
GLfloat tireBlack[] = {0.1f, 0.1f, 0.1f};
GLfloat tireSilver[] = {0.8f, 0.8f, 0.8f};

const GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 10.0f, 10.0f, 10.0f, 0.0f };

void initPedestrians() {
    for (int i = 0; i < MAX_PEDESTRIANS; i++) {
        pedestrians[i].x = -20.0f + (rand() % 40);
        pedestrians[i].z = -20.0f + (rand() % 30);
        pedestrians[i].destX = -20.0f + (rand() % 40);
        pedestrians[i].destZ = -20.0f + (rand() % 30);
        pedestrians[i].rotation = (float)(rand() % 360);
        pedestrians[i].speed = 0.03f + ((float)rand() / RAND_MAX) * 0.04f;
        pedestrians[i].size = 0.8f + ((float)rand() / RAND_MAX) * 0.4f;
        pedestrians[i].moveState = 1;
        pedestrians[i].waitTime = 100 + (rand() % 300);
        pedestrians[i].waitCounter = 0;
        pedestrians[i].shirtColor[0] = 0.2f + ((float)rand() / RAND_MAX) * 0.8f;
        pedestrians[i].shirtColor[1] = 0.2f + ((float)rand() / RAND_MAX) * 0.8f;
        pedestrians[i].shirtColor[2] = 0.2f + ((float)rand() / RAND_MAX) * 0.8f;
        pedestrians[i].pantsColor[0] = ((float)rand() / RAND_MAX) * 0.5f;
        pedestrians[i].pantsColor[1] = ((float)rand() / RAND_MAX) * 0.5f;
        pedestrians[i].pantsColor[2] = ((float)rand() / RAND_MAX) * 0.5f;
    }
}

void drawPedestrian(Pedestrian *ped) {
    glPushMatrix();
    glTranslatef(ped->x, 0.0f, ped->z);
    glRotatef(ped->rotation, 0.0f, 1.0f, 0.0f);
    glScalef(ped->size, ped->size, ped->size);  
    
    glColor3f(0.95f, 0.85f, 0.7f);
    glPushMatrix();
    glTranslatef(0.0f, 1.6f, 0.0f);
    glutSolidSphere(0.15f, 10, 10);
    glPopMatrix();   
    
    glColor3fv(ped->shirtColor);
    glPushMatrix();
    glTranslatef(0.0f, 1.2f, 0.0f);
    glScalef(0.25f, 0.4f, 0.15f);
    glutSolidCube(1.0f);
    glPopMatrix();  
    
    glColor3fv(ped->pantsColor);
    glPushMatrix();
    glTranslatef(-0.07f, 0.5f, 0.0f);
    glScalef(0.08f, 0.7f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.07f, 0.5f, 0.0f);
    glScalef(0.08f, 0.7f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glColor3fv(ped->shirtColor);
    glPushMatrix();
    glTranslatef(-0.2f, 1.2f, 0.0f);
    glScalef(0.08f, 0.4f, 0.08f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.2f, 1.2f, 0.0f);
    glScalef(0.08f, 0.4f, 0.08f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPopMatrix();
}

void drawBusStop() {
    glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix();
    glTranslatef(busStopX, 0.05f, busStopZ);
    glScalef(4.0f, 0.1f, 1.5f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glColor3f(0.1f, 0.1f, 0.8f);
    glPushMatrix();
    glTranslatef(busStopX + 1.8f, 1.5f, busStopZ - 0.6f);
    glScalef(0.1f, 1.0f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(busStopX + 1.85f, 1.7f, busStopZ - 0.6f);
    glScalef(0.05f, 0.3f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glColor3fv(busStopRoof);
    glPushMatrix();
    glTranslatef(busStopX, 2.2f, busStopZ);
    glScalef(3.0f, 0.1f, 1.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glColor3fv(busStopSupport);
    glPushMatrix();
    glTranslatef(busStopX - 1.4f, 1.1f, busStopZ - 0.5f);
    glScalef(0.1f, 2.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(busStopX + 1.4f, 1.1f, busStopZ - 0.5f);
    glScalef(0.1f, 2.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(busStopX - 1.4f, 1.1f, busStopZ + 0.5f);
    glScalef(0.1f, 2.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(busStopX + 1.4f, 1.1f, busStopZ + 0.5f);
    glScalef(0.1f, 2.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glColor4f(0.8f, 0.8f, 0.9f, 0.6f);
    glPushMatrix();
    glTranslatef(busStopX, 1.1f, busStopZ - 0.55f);
    glScalef(2.9f, 1.1f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(busStopX - 1.45f, 1.1f, busStopZ);
    glScalef(0.05f, 1.1f, 1.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(busStopX + 1.45f, 1.1f, busStopZ);
    glScalef(0.05f, 1.1f, 1.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glColor3fv(busStopBench);
    glPushMatrix();
    glTranslatef(busStopX, 0.7f, busStopZ - 0.3f);
    glScalef(2.0f, 0.1f, 0.4f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glColor3f(0.4f, 0.4f, 0.4f);
    glPushMatrix();
    glTranslatef(busStopX - 0.9f, 0.4f, busStopZ - 0.3f);
    glScalef(0.1f, 0.6f, 0.4f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(busStopX + 0.9f, 0.4f, busStopZ - 0.3f);
    glScalef(0.1f, 0.6f, 0.4f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(busStopX + 1.81f, 2.0f, busStopZ - 0.6f);
    glScalef(0.02f, 0.4f, 0.4f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void updatePedestrians() {
    for (int i = 0; i < MAX_PEDESTRIANS; i++) {
        Pedestrian *ped = &pedestrians[i];       
        
        switch (ped->moveState) {
            case 0:
                ped->waitCounter++;
                if (ped->waitCounter >= ped->waitTime) {
                    ped->moveState = 1;
                    ped->waitCounter = 0;
                    if (rand() % 100 < 30) {
                        ped->destX = busStopX + ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
                        ped->destZ = busStopZ + ((float)rand() / RAND_MAX) * 1.0f - 0.5f;
                    } else {
                        ped->destX = -15.0f + (rand() % 30);
                        ped->destZ = -15.0f + (rand() % 25);
                    }
                }
                break;               
                
            case 1:
                {
                    float dx = ped->destX - ped->x;
                    float dz = ped->destZ - ped->z;
                    float distance = sqrt(dx*dx + dz*dz);                    
                    
                    if (distance < 0.2f) {
                        float busStopDist = sqrt(pow(ped->x - busStopX, 2) + pow(ped->z - busStopZ, 2));
                        if (busStopDist < 2.0f) {
                            ped->moveState = 2;
                            ped->waitTime = 200 + (rand() % 300);
                            ped->waitCounter = 0;                            
                            
                            ped->x = busStopX + ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
                            ped->z = busStopZ + ((float)rand() / RAND_MAX) * 0.8f - 0.4f;
                            ped->rotation = 270.0f + ((float)rand() / RAND_MAX) * 40.0f - 20.0f;
                        } else {
                            ped->moveState = 0;
                            ped->waitTime = 50 + (rand() % 100);
                            ped->waitCounter = 0;
                        }
                    } else {
                        if (distance > 0) {
                            dx /= distance;
                            dz /= distance;
                        }                       
                        
                        ped->x += dx * ped->speed;
                        ped->z += dz * ped->speed;                      
                        
                        ped->rotation = atan2(dx, -dz) * 180.0f / M_PI;
                    }
                }
                break;                
                
            case 2:
                ped->waitCounter++;
                if (ped->waitCounter >= ped->waitTime) {
                    ped->moveState = 1;
                    ped->waitCounter = 0;
                    ped->destX = -15.0f + (rand() % 30);
                    ped->destZ = -15.0f + (rand() % 25);
                }               
                
                float busDistance = sqrt(pow(busX - busStopX, 2) + pow(busZ - busStopZ, 2));
                if (busDistance < 3.0f) {
                    if (rand() % 100 < 50 && ped->waitCounter > 30) {
                        ped->x = -15.0f + (rand() % 30);
                        ped->z = -15.0f + (rand() % 25);
                        ped->moveState = 0;
                        ped->waitCounter = 0;
                        ped->waitTime = 20 + (rand() % 40);
                    }
                }
                break;
        }
    }
}

void drawPedestrians() {
    for (int i = 0; i < MAX_PEDESTRIANS; i++) {
        drawPedestrian(&pedestrians[i]);
    }
}

void reshape(int w1, int h1) {
    if (h1 == 0) h1 = 1;
    w = w1;
    h = h1;
    ratio = 1.0f * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

void orientMe(float ang) {
    lx = sin(ang);
    lz = -cos(ang);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

void moveMeFlat(int i) {
    x = x + i * (lx) * 0.1;
    z = z + i * (lz) * 0.1;
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

void drawCylinder(float radius, float height, int slices) {
    float angle, x, y;
    int i;
    
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= slices; i++) {
        angle = 2.0f * M_PI * i / slices;
        x = radius * cos(angle);
        y = radius * sin(angle);
        
        glVertex3f(x, y, 0.0f);
        glVertex3f(x, y, height);
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    for (i = 0; i < slices; i++) {
        angle = 2.0f * M_PI * i / slices;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    for (i = 0; i < slices; i++) {
        angle = 2.0f * M_PI * i / slices;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height);
    }
    glEnd();
}

void drawRoad() {
    float roadWidth = 7.0f;
    float visibleRoadLength = 200.0f;
    
    float startX = busX - visibleRoadLength/2;
    float endX = busX + visibleRoadLength/2;
    
    glColor3fv(roadGray);
    glBegin(GL_QUADS);
    glVertex3f(startX, 0.01f, -roadWidth/2);
    glVertex3f(endX, 0.01f, -roadWidth/2);
    glVertex3f(endX, 0.01f, roadWidth/2);
    glVertex3f(startX, 0.01f, roadWidth/2);
    glEnd();
    
    glColor3fv(roadYellow);
    
    float markerSpacing = 2.0f;
    float markerLength = 1.0f;
    
    float firstMarkerPos = startX - fmodf(startX, markerSpacing);
    if (firstMarkerPos < startX) firstMarkerPos += markerSpacing;
    
    for (float x = firstMarkerPos; x < endX; x += markerSpacing) {
        glBegin(GL_QUADS);
        glVertex3f(x, 0.02f, -0.1f);
        glVertex3f(x + markerLength, 0.02f, -0.1f);
        glVertex3f(x + markerLength, 0.02f, 0.1f);
        glVertex3f(x, 0.02f, 0.1f);
        glEnd();
    }
}

void drawBuilding(float x, float z, float width, float height, float depth) {
    glColor3fv(buildingColor);
    glBegin(GL_QUADS);
    
    glVertex3f(x, 0.0f, z);
    glVertex3f(x + width, 0.0f, z);
    glVertex3f(x + width, height, z);
    glVertex3f(x, height, z);
    
    glVertex3f(x, 0.0f, z + depth);
    glVertex3f(x + width, 0.0f, z + depth);
    glVertex3f(x + width, height, z + depth);
    glVertex3f(x, height, z + depth);
    
    glVertex3f(x, 0.0f, z);
    glVertex3f(x, 0.0f, z + depth);
    glVertex3f(x, height, z + depth);
    glVertex3f(x, height, z);  
    
    glVertex3f(x + width, 0.0f, z);
    glVertex3f(x + width, 0.0f, z + depth);
    glVertex3f(x + width, height, z + depth);
    glVertex3f(x + width, height, z); 
    
    glVertex3f(x, height, z);
    glVertex3f(x + width, height, z);
    glVertex3f(x + width, height, z + depth);
    glVertex3f(x, height, z + depth);
    glEnd();    
    
    glColor3fv(windowColor);
    float windowSize = 0.5f;
    float windowSpacing = 0.8f;    
    
    for (float wx = x + 0.5f; wx < x + width; wx += windowSpacing) {
        for (float wy = 1.0f; wy < height; wy += windowSpacing) {
            glBegin(GL_QUADS);
            glVertex3f(wx, wy, z - 0.01f);
            glVertex3f(wx + windowSize, wy, z - 0.01f);
            glVertex3f(wx + windowSize, wy + windowSize, z - 0.01f);
            glVertex3f(wx, wy + windowSize, z - 0.01f);
            glEnd();
        }
    }    
    
    for (float wx = x + 0.5f; wx < x + width; wx += windowSpacing) {
        for (float wy = 1.0f; wy < height; wy += windowSpacing) {
            glBegin(GL_QUADS);
            glVertex3f(wx, wy, z + depth + 0.01f);
            glVertex3f(wx + windowSize, wy, z + depth + 0.01f);
            glVertex3f(wx + windowSize, wy + windowSize, z + depth + 0.01f);
            glVertex3f(wx, wy + windowSize, z + depth + 0.01f);
            glEnd();
        }
    }
}

void drawBuildings() {
    float buildingSpacing = 8.0f;
    float visibleDistance = 100.0f;
    
    float startX = busX - visibleDistance;
    float endX = busX + visibleDistance;
    
    float firstBuildingPosLeft = startX - fmodf(startX, buildingSpacing);
    if (firstBuildingPosLeft < startX) firstBuildingPosLeft += buildingSpacing;
    
    for (float x = firstBuildingPosLeft; x < endX; x += buildingSpacing) {
        float height = 5.0f + fmodf(x * 1.7f, 5.0f);
        float width = 3.0f + fmodf(x * 2.3f, 2.0f);
        
        drawBuilding(x, -15.0f, width, height, 4.0f);
    }
    
    float firstBuildingPosRight = startX - fmodf(startX + 4.0f, buildingSpacing);
    if (firstBuildingPosRight < startX) firstBuildingPosRight += buildingSpacing;
    for (float x = firstBuildingPosRight; x < endX; x += buildingSpacing) {
        float height = 5.0f + fmodf(x * 1.3f, 5.0f);
        float width = 3.0f + fmodf(x * 1.9f, 2.0f);        
        drawBuilding(x, 8.0f, width, height, 4.0f);
    }
}

void drawTree(float x, float z) {
    glColor3fv(trunkColor);
    glPushMatrix();
    glTranslatef(x, 0.0f, z);    
    
    float trunkHeight = 2.0f;
    float trunkRadius = 0.2f;
    int segments = 16;    
    
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        float xPos = trunkRadius * cos(angle);
        float zPos = trunkRadius * sin(angle);
        
        glVertex3f(xPos, 0.0f, zPos);
        glVertex3f(xPos, trunkHeight, zPos);
    }
    glEnd();    
    
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        float xPos = trunkRadius * cos(angle);
        float zPos = trunkRadius * sin(angle);
        glVertex3f(xPos, trunkHeight, zPos);
    }
    glEnd();   
    glPopMatrix();   
    
    glColor3fv(leafColor);
    glPushMatrix();
    glTranslatef(x, 2.3f, z);
    glutSolidSphere(1.0f, 16, 16);
    glPopMatrix();
}

void drawTrees() {
    float treeSpacing = 4.0f;
    float visibleDistance = 120.0f;
    
    float startX = busX - visibleDistance;
    float endX = busX + visibleDistance;
    
    float firstTreePosLeft = startX - fmodf(startX, treeSpacing);
    if (firstTreePosLeft < startX) firstTreePosLeft += treeSpacing;
    
    for (float x = firstTreePosLeft; x < endX; x += treeSpacing) {
        drawTree(x, -10.0f);
    }
    
    float firstTreePosRight = startX - fmodf(startX + 2.0f, treeSpacing);
    if (firstTreePosRight < startX) firstTreePosRight += treeSpacing;
    for (float x = firstTreePosRight; x < endX; x += treeSpacing) {
        drawTree(x, 4.0f);
    }
}

void drawBus() {
    glPushMatrix();
    glTranslatef(busX, 0.5f, busZ);
    glRotatef(busRotation, 0.0f, 1.0f, 0.0f);   
    
    glColor3fv(busBlue);  
    
    glBegin(GL_QUADS);
    glVertex3f(-3.0f, 0.0f, -1.0f);
    glVertex3f(3.0f, 0.0f, -1.0f);
    glVertex3f(3.0f, 0.0f, 1.0f);
    glVertex3f(-3.0f, 0.0f, 1.0f); 
    
    glVertex3f(-3.0f, 1.5f, -1.0f);
    glVertex3f(3.0f, 1.5f, -1.0f);
    glVertex3f(3.0f, 1.5f, 1.0f);
    glVertex3f(-3.0f, 1.5f, 1.0f); 
    
    glVertex3f(3.0f, 0.0f, -1.0f);
    glVertex3f(3.0f, 0.0f, 1.0f);
    glVertex3f(3.0f, 1.5f, 1.0f);
    glVertex3f(3.0f, 1.5f, -1.0f);
    
    glVertex3f(-3.0f, 0.0f, -1.0f);
    glVertex3f(-3.0f, 0.0f, 1.0f);
    glVertex3f(-3.0f, 1.5f, 1.0f);
    glVertex3f(-3.0f, 1.5f, -1.0f);
    
    glVertex3f(-3.0f, 0.0f, 1.0f);
    glVertex3f(3.0f, 0.0f, 1.0f);
    glVertex3f(3.0f, 1.5f, 1.0f);
    glVertex3f(-3.0f, 1.5f, 1.0f);    
    
    glVertex3f(-3.0f, 0.0f, -1.0f);
    glVertex3f(3.0f, 0.0f, -1.0f);
    glVertex3f(3.0f, 1.5f, -1.0f);
    glVertex3f(-3.0f, 1.5f, -1.0f);
    glEnd();    
    
    glColor4fv(busGlass);
    glBegin(GL_QUADS);
    glVertex3f(3.01f, 0.3f, -0.8f);
    glVertex3f(3.01f, 0.3f, 0.8f);
    glVertex3f(3.01f, 1.3f, 0.8f);
    glVertex3f(3.01f, 1.3f, -0.8f);
    glEnd();    
    
    glBegin(GL_QUADS);
    glVertex3f(-3.01f, 0.3f, -0.8f);
    glVertex3f(-3.01f, 0.3f, 0.8f);
    glVertex3f(-3.01f, 1.3f, 0.8f);
    glVertex3f(-3.01f, 1.3f, -0.8f);
    glEnd();    
    
    float windowSpacing = 1.0f;   
    
    for (float wx = -2.5f; wx < 2.5f; wx += windowSpacing) {
        glBegin(GL_QUADS);
        glVertex3f(wx, 0.6f, 1.01f);
        glVertex3f(wx + 0.8f, 0.6f, 1.01f);
        glVertex3f(wx + 0.8f, 1.3f, 1.01f);
        glVertex3f(wx, 1.3f, 1.01f);
        glEnd();
    }    
    
    for (float wx = -2.5f; wx < 2.5f; wx += windowSpacing) {
        glBegin(GL_QUADS);
        glVertex3f(wx, 0.6f, -1.01f);
        glVertex3f(wx + 0.8f, 0.6f, -1.01f);
        glVertex3f(wx + 0.8f, 1.3f, -1.01f);
        glVertex3f(wx, 1.3f, -1.01f);
        glEnd();
    }   
    
    glColor3fv(tireBlack);    
    
    glPushMatrix();
    glTranslatef(2.0f, 0.3f, 1.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    drawCylinder(0.3f, 0.2f, 20);
    glPopMatrix();    
    
    glPushMatrix();
    glTranslatef(2.0f, 0.3f, -1.2f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    drawCylinder(0.3f, 0.2f, 20);
	glPopMatrix();    
	
	glPushMatrix();
	glTranslatef(-2.0f, 0.3f, 1.0f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	drawCylinder(0.3f, 0.2f, 20);
	glPopMatrix();    
	
	glPushMatrix();
	glTranslatef(-2.0f, 0.3f, -1.2f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	drawCylinder(0.3f, 0.2f, 20);
	glPopMatrix();    
	
	glColor3fv(tireSilver);    
	
	glPushMatrix();
	glTranslatef(2.0f, 0.3f, 1.2f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glutSolidSphere(0.1f, 10, 10);
	glPopMatrix();    
	
	glPushMatrix();
	glTranslatef(2.0f, 0.3f, -1.2f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glutSolidSphere(0.1f, 10, 10);
	glPopMatrix();    
	
	glPushMatrix();
	glTranslatef(-2.0f, 0.3f, 1.2f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glutSolidSphere(0.1f, 10, 10);
	glPopMatrix();    
	
	glPushMatrix();
	glTranslatef(-2.0f, 0.3f, -1.2f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glutSolidSphere(0.1f, 10, 10);
	glPopMatrix();    
	
	glPopMatrix();
}

void drawBusShadow() {
    glColor4f(0.0f, 0.0f, 0.0f, 0.3f);      
    glPushMatrix();
    glTranslatef(busX, 0.01f, busZ);
    glRotatef(busRotation, 0.0f, 1.0f, 0.0f);   
    
    glPushMatrix();
    glScalef(3.5f, 1.0f, 1.5f);
    glutSolidSphere(0.8f, 16, 16);
    glPopMatrix();    
    
    glPopMatrix();
}

void toggleBusShadow() {
    shadowEnabled = !shadowEnabled;
    printf("Bus shadow: %s\n", shadowEnabled ? "ON" : "OFF");
}
GLuint obstacleTextures[3];

void loadObstacleTextures() {
    glGenTextures(3, obstacleTextures);
    
    glBindTexture(GL_TEXTURE_2D, obstacleTextures[0]);
    unsigned char boxTexture[64*64*3];
    for(int i = 0; i < 64*64; i++) {
        int x = i % 64;
        int y = i / 64;
        if((x/8 + y/8) % 2) {
            boxTexture[i*3] = 139;
            boxTexture[i*3+1] = 69;
            boxTexture[i*3+2] = 19;
        } else {
            boxTexture[i*3] = 160;
            boxTexture[i*3+1] = 82;
            boxTexture[i*3+2] = 45;
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, boxTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, obstacleTextures[1]);
    unsigned char coneTexture[64*64*3];
    for(int i = 0; i < 64*64; i++) {
        int y = i / 64;
        if(y % 8 < 2) {
            coneTexture[i*3] = 255;
            coneTexture[i*3+1] = 255;
            coneTexture[i*3+2] = 255;
        } else {
            coneTexture[i*3] = 255;
            coneTexture[i*3+1] = 127;
            coneTexture[i*3+2] = 0;
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, coneTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, obstacleTextures[2]);
    unsigned char barrelTexture[64*64*3];
    for(int i = 0; i < 64*64; i++) {
        int y = i / 64;
        if(y % 4 == 0) {
            barrelTexture[i*3] = 80;
            barrelTexture[i*3+1] = 80;
            barrelTexture[i*3+2] = 80;
        } else {
            barrelTexture[i*3] = 160;
            barrelTexture[i*3+1] = 160;
            barrelTexture[i*3+2] = 160;
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, barrelTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void initObstacles() {
    float obstacleSpacing = 15.0f;
    float roadCenterZ = 0.0f;
    float roadWidth = 7.0f;
    
    loadObstacleTextures();
    
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].x = busX + 10.0f + (i * obstacleSpacing);
        if (rand() % 2 == 0) {
            obstacles[i].z = -roadWidth/2 + 0.5f + ((float)rand() / RAND_MAX) * 1.5f;
        } else {
            obstacles[i].z = roadWidth/2 - 0.5f - ((float)rand() / RAND_MAX) * 1.5f;
        }      
        obstacles[i].type = rand() % 3;
        obstacles[i].active = 1;
        switch (obstacles[i].type) {
            case 0:
                obstacles[i].width = 0.8f + ((float)rand() / RAND_MAX) * 0.4f;
                obstacles[i].height = 0.5f + ((float)rand() / RAND_MAX) * 0.5f;
                obstacles[i].depth = 0.8f + ((float)rand() / RAND_MAX) * 0.4f;
                obstacles[i].color[0] = 0.4f + ((float)rand() / RAND_MAX) * 0.4f;
                obstacles[i].color[1] = 0.3f + ((float)rand() / RAND_MAX) * 0.3f;
                obstacles[i].color[2] = 0.2f + ((float)rand() / RAND_MAX) * 0.3f;
                break;
            case 1:
                obstacles[i].width = 0.6f;
                obstacles[i].height = 1.0f;
                obstacles[i].depth = 0.6f;
                obstacles[i].color[0] = 1.0f;
                obstacles[i].color[1] = 0.5f;
                obstacles[i].color[2] = 0.0f;
                break;
            case 2:
                obstacles[i].width = 0.5f;
                obstacles[i].height = 1.2f;
                obstacles[i].depth = 0.5f;
                obstacles[i].color[0] = 0.6f + ((float)rand() / RAND_MAX) * 0.3f;
                obstacles[i].color[1] = 0.6f + ((float)rand() / RAND_MAX) * 0.3f;
                obstacles[i].color[2] = 0.6f + ((float)rand() / RAND_MAX) * 0.3f;
                break;
        }
    }
}

void drawTexturedCube(float size) {
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size/2, -size/2,  size/2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( size/2, -size/2,  size/2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( size/2,  size/2,  size/2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size/2,  size/2,  size/2);
    
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size/2, -size/2, -size/2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size/2,  size/2, -size/2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( size/2,  size/2, -size/2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( size/2, -size/2, -size/2);
    
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size/2,  size/2, -size/2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size/2,  size/2,  size/2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( size/2,  size/2,  size/2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( size/2,  size/2, -size/2);
    
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size/2, -size/2, -size/2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( size/2, -size/2, -size/2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( size/2, -size/2,  size/2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size/2, -size/2,  size/2);
    
    glTexCoord2f(1.0f, 0.0f); glVertex3f( size/2, -size/2, -size/2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( size/2,  size/2, -size/2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( size/2,  size/2,  size/2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( size/2, -size/2,  size/2);
    
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size/2, -size/2, -size/2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size/2, -size/2,  size/2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size/2,  size/2,  size/2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size/2,  size/2, -size/2);
    glEnd();
}

void drawObstacle(Obstacle *obs) {
    if (!obs->active) return;    
    
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glPushMatrix();
    glTranslatef(obs->x, 0.0f, obs->z);    
    
    switch (obs->type) {
        case 0:
            glBindTexture(GL_TEXTURE_2D, obstacleTextures[0]);
            glPushMatrix();
            glTranslatef(0.0f, obs->height/2, 0.0f);
            glScalef(obs->width, obs->height, obs->depth);
            drawTexturedCube(1.0f);
            glPopMatrix();
            break;
        case 1: {
            glBindTexture(GL_TEXTURE_2D, obstacleTextures[1]);
            glPushMatrix();
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            GLUquadric* coneQuad = gluNewQuadric();
            gluQuadricTexture(coneQuad, GL_TRUE);
            gluCylinder(coneQuad, obs->width, 0.0f, obs->height, 16, 16);
            gluDeleteQuadric(coneQuad);
            glPopMatrix();
            break;
        }
        case 2: {
            glBindTexture(GL_TEXTURE_2D, obstacleTextures[2]);
            glPushMatrix();
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            GLUquadric* barrelQuad = gluNewQuadric();
            gluQuadricTexture(barrelQuad, GL_TRUE);
            gluCylinder(barrelQuad, obs->width, obs->width, obs->height, 16, 16);
            gluDeleteQuadric(barrelQuad);
            glPopMatrix();
            break;
        }
    }
    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawObstacles() {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        drawObstacle(&obstacles[i]);
    }
}

void cleanupObstacleTextures() {
    glDeleteTextures(3, obstacleTextures);
}

int checkObstacleCollision() {
    if (gameOver) return -1;
    
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active) continue;
        
        float busLeft = busX - 3.0f;
        float busRight = busX + 3.0f;
        float busFront = busZ - 1.0f;
        float busBack = busZ + 1.0f;
        
        float obsLeft = obstacles[i].x - obstacles[i].width/2;
        float obsRight = obstacles[i].x + obstacles[i].width/2;
        float obsFront = obstacles[i].z - obstacles[i].depth/2;
        float obsBack = obstacles[i].z + obstacles[i].depth/2;
        
        if (busRight > obsLeft && busLeft < obsRight &&
            busBack > obsFront && busFront < obsBack) {
            gameOver = 1;
            printf("GAME OVER! Final Score: %d\n", gameScore);
            return i;
        }
    }
    return -1;
}

void resetGame() {
    gameOver = 0;
    gameScore = 0;
    busX = 0.0f;
    busZ = 0.0f;
    busRotation = 0.0f;
    
    x = 0.0f;
    y = 2.0f;
    z = 10.0f;
    lx = 0.0f;
    ly = 0.0f;
    lz = -1.0f;
    angle = 0.0f;
    
    initObstacles();
    initPedestrians();
    
    printf("Game Reset! New game started.\n");
}

void drawGameOverText() {
    if (!gameOver) return;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(w, 0);
    glVertex2f(w, h);
    glVertex2f(0, h);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2i(w / 2 - 50, h / 2 + 30);
    const char* gameOverText = "GAME OVER";
    for (const char* c = gameOverText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(w / 2 - 120, h / 2 - 30);
    const char* restartText = "Tekan SPASI untuk mulai ulang";
    for (const char* c = restartText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void keyboardWithCollision(int key, int xx, int yy) {
    if (gameOver) return;
    if (key == GLUT_KEY_F1) {
        toggleBusShadow();
        glutPostRedisplay();
        return;
    }

    float originalX = busX;
    float originalZ = busZ;
    float radians = (busRotation * M_PI) / 180.0f;

    switch (key) {
        case GLUT_KEY_LEFT:
            busX += busSpeed * sin(radians);
            busZ -= busSpeed * cos(radians);
            break;
        case GLUT_KEY_RIGHT:
            busX -= busSpeed * sin(radians);
            busZ += busSpeed * cos(radians);
            break;
        case GLUT_KEY_UP:
            busX += busSpeed * cos(radians);
            busZ += busSpeed * sin(radians);
            break;
        case GLUT_KEY_DOWN:
            busX -= busSpeed * cos(radians);
            busZ -= busSpeed * sin(radians);
            break;
    }

    int collisionIndex = checkObstacleCollision();
    if (collisionIndex >= 0) {
        busX = originalX;
        busZ = originalZ;
        printf("Collision with obstacle %d!\n", collisionIndex);
    }
}

void updateObstacles() {
    float obstacleSpacing = 15.0f;
    float roadWidth = 7.0f;

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].x < busX - 20.0f) {
            float maxX = busX;
            for (int j = 0; j < MAX_OBSTACLES; j++) {
                if (obstacles[j].x > maxX) {
                    maxX = obstacles[j].x;
                }
            }
            obstacles[i].x = maxX + obstacleSpacing;

            if (rand() % 2 == 0) {
                obstacles[i].z = -roadWidth/2 + 0.5f + ((float)rand() / RAND_MAX) * 1.5f;
            } else {
                obstacles[i].z = roadWidth/2 - 0.5f - ((float)rand() / RAND_MAX) * 1.5f;
            }

            obstacles[i].type = rand() % 3;
            switch (obstacles[i].type) {
                case 0:
                    obstacles[i].width = 0.8f + ((float)rand() / RAND_MAX) * 0.4f;
                    obstacles[i].height = 0.5f + ((float)rand() / RAND_MAX) * 0.5f;
                    obstacles[i].depth = 0.8f + ((float)rand() / RAND_MAX) * 0.4f;
                    obstacles[i].color[0] = 0.4f + ((float)rand() / RAND_MAX) * 0.4f;
                    obstacles[i].color[1] = 0.3f + ((float)rand() / RAND_MAX) * 0.3f;
                    obstacles[i].color[2] = 0.2f + ((float)rand() / RAND_MAX) * 0.3f;
                    break;
                case 1:
                    obstacles[i].width = 0.6f;
                    obstacles[i].height = 1.0f;
                    obstacles[i].depth = 0.6f;
                    obstacles[i].color[0] = 1.0f;
                    obstacles[i].color[1] = 0.5f;
                    obstacles[i].color[2] = 0.0f;
                    break;
                case 2:
                    obstacles[i].width = 0.5f;
                    obstacles[i].height = 1.2f;
                    obstacles[i].depth = 0.5f;
                    obstacles[i].color[0] = 0.6f + ((float)rand() / RAND_MAX) * 0.3f;
                    obstacles[i].color[1] = 0.6f + ((float)rand() / RAND_MAX) * 0.3f;
                    obstacles[i].color[2] = 0.6f + ((float)rand() / RAND_MAX) * 0.3f;
                    break;
            }
        }
    }
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);

    float groundExtension = 200.0f;
    glColor3f(0.2f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(busX - groundExtension, 0.0f, -50.0f);
    glVertex3f(busX + groundExtension, 0.0f, -50.0f);
    glVertex3f(busX + groundExtension, 0.0f, 50.0f);
    glVertex3f(busX - groundExtension, 0.0f, 50.0f);
    glEnd();

    drawRoad();
    drawBuildings();
    drawTrees();
    drawObstacles();

    if (shadowEnabled) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawBusShadow();
        glDisable(GL_BLEND);
    }

    drawBus();
    drawBusShadow();
    drawBusStop();
    drawPedestrians();
    drawGameOverText();

    glutSwapBuffers();
}

int cameraMode = 1;

void updateCamera() {
    if (cameraMode == 1) {
        float distance = 10.0f;
        float height = 3.5f;
        float radians = (busRotation * M_PI) / 180.0f;
        x = busX - distance * cos(radians);
        z = busZ - distance * sin(radians);
        y = height;
        lx = cos(radians);
        lz = sin(radians);
        ly = -0.3f;
        glLoadIdentity();
        gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
    }
}

void update(int value) {
    if (!gameOver) {
        gameScore++;
    }
    if (cameraMode == 0) {
        if (deltaMove) {
            x += deltaMove * lx * 0.1f;
            z += deltaMove * lz * 0.1f;
        }
        if (deltaAngle) {
            angle += deltaAngle;
            lx = sin(angle);
            lz = -cos(angle);
        }
        glLoadIdentity();
        gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
    }
    updateCamera();
    updatePedestrians();
    updateObstacles();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void normalKeys(unsigned char key, int xx, int yy) {
    switch (key) {
        case 'w':
            deltaMove = 1;
            break;
        case 's':
            deltaMove = -1;
            break;
        case 'a':
            deltaAngle = -0.01f;
            break;
        case 'd':
            deltaAngle = 0.01f;
            break;
        case 'c':
            cameraMode = 1 - cameraMode;
            break;
        case 'r':
            x = 0.0f;
            y = 2.0f;
            z = 10.0f;
            lx = 0.0f;
            ly = 0.0f;
            lz = -1.0f;
            angle = 0.0f;
            break;
        case ' ':
            if (gameOver) {
                resetGame();
            }
            break;
        case 27:
            exit(0);
            break;
    }
}

void normalKeysUp(unsigned char key, int xx, int yy) {
    switch (key) {
        case 'h':
        case 'H':
            toggleBusShadow();
            glutPostRedisplay();
            break;
        case 'w':
        case 's':
            deltaMove = 0;
            break;
        case 'a':
        case 'd':
            deltaAngle = 0.0f;
            break;
    }
}

void lighting() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);   
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void init() {
    glClearColor(skyBlue[0], skyBlue[1], skyBlue[2], 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    initPedestrians();
    initObstacles();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("3D Bus Simulator");
    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    glutIdleFunc(renderScene);
    glutTimerFunc(16, update, 0);
    glutSpecialFunc(keyboardWithCollision);
    glutKeyboardFunc(normalKeys);
    glutKeyboardUpFunc(normalKeysUp);
    lighting();
    init();
    glutMainLoop();
    return 0;
}
#include"Sphere.h"
#include"Constants.h"
#include<GL/freeglut.h>
#include<iostream>
using namespace std;

void Sphere::settime(int t) {
    time = t;
}

int Sphere::gettime() {
    return time;
}

Sphere::Sphere() {
    radius = 0; slice = 0; stack = 0;
    idxPos[0] = 0; idxPos[1] = 0;
    bInxPosUpdated = "true";
    currDirection = NONE;
    nextDirection = NONE;
}

Sphere::Sphere(float r, int sl, int st) {
    radius = r; slice = sl; stack = st;
    idxPos[0] = 0; idxPos[1] = 0;
    bInxPosUpdated = "False";

    currDirection = NONE;
    nextDirection = NONE;
}
void Sphere::setRadius(float r) {
    radius = r;
}
float Sphere::getRadius() const {
    return radius;
}
void Sphere::setSlice(int sl) {
    slice = sl;
}
void Sphere::setStack(int st) {
    stack = st;
}
int Sphere::getXIndex()const {
    return idxPos[0];
}
int Sphere::getYIndex()const {
    return idxPos[1];
}
void Sphere::setIndexPosition(int x, int y) {
    idxPos[0] = x;
    idxPos[1] = y;
    center[0] = LEFT_BOUNDARY + idxPos[0] * BLOCK_SIZE + BLOCK_SIZE / 2;
    center[1] = BOTTOM_BOUNDARY + idxPos[1] * BLOCK_SIZE + BLOCK_SIZE / 2;
}
bool Sphere::isIndexPositionUpdated() {
    return bInxPosUpdated;
}

void Sphere::updateDirection() {
    currDirection = nextDirection;

}

void Sphere::move() {
    center[0] += velocity[0];
    center[1] += velocity[1];
    center[2] += velocity[2];

    bInxPosUpdated = false;

    if (currDirection == RIGHT) {
        if (center[0] - radius * 2.0f >= LEFT_BOUNDARY + idxPos[0] * BLOCK_SIZE + BLOCK_SIZE / 2) {
            if (idxPos[0] < NUM_COL - 2) {
                setIndexPosition(idxPos[0] + 1, idxPos[1]);
            }
            else {
                setIndexPosition(1, idxPos[1]);
            }
            bInxPosUpdated = true;
        }
    }
    else if (currDirection == LEFT) {
        if (center[0] + radius * 2.0f <= LEFT_BOUNDARY + idxPos[0] * BLOCK_SIZE + BLOCK_SIZE / 2) {
            if (idxPos[0] > 1) {
                setIndexPosition(idxPos[0] - 1, idxPos[1]);
            }
            else {
                setIndexPosition(NUM_COL - 2, idxPos[1]);
            }
            bInxPosUpdated = true;
        }
    }
    else if (currDirection == UP) {
        if (center[1] - radius * 2.0f >= BOTTOM_BOUNDARY + idxPos[1] * BLOCK_SIZE + BLOCK_SIZE / 2) {
            if (idxPos[1] < NUM_ROW - 2) {
                setIndexPosition(idxPos[0], idxPos[1] + 1);
            }
            else {
                setIndexPosition(idxPos[0], 0);
            }
            bInxPosUpdated = true;
        }
    }
    else if (currDirection == DOWN) {
        if (center[1] + radius * 2.0f <= BOTTOM_BOUNDARY + idxPos[1] * BLOCK_SIZE + BLOCK_SIZE / 2) {
            if (idxPos[1] > 1)
                setIndexPosition(idxPos[0], idxPos[1] - 1);
            else
                setIndexPosition(idxPos[0], NUM_ROW - 1);
            bInxPosUpdated = true;
        }
    }
    else
        bInxPosUpdated = true;
}

void Sphere::setCurrentDirection(DIRECTION d) {
    currDirection = d;
}
void Sphere::setNextDirection(DIRECTION d) {
    nextDirection = d;
}
Sphere::DIRECTION Sphere::getCurrentDirection() const {
    return currDirection;
}
Sphere::DIRECTION Sphere::getNextDirection() const {
    return nextDirection;
}
void Sphere::draw() const {
    GLfloat emissionArray[4] = { mtl.getEmission()[0],mtl.getEmission()[1],mtl.getEmission()[2],mtl.getEmission()[3] };
    GLfloat ambientArray[4] = { mtl.getAmbient()[0],mtl.getAmbient()[1],mtl.getAmbient()[2],mtl.getAmbient()[3] };
    GLfloat diffuseArray[4] = { mtl.getDiffuse()[0],mtl.getDiffuse()[1],mtl.getDiffuse()[2],mtl.getDiffuse()[3] };
    GLfloat specularArray[4] = { mtl.getSpecular()[0],mtl.getSpecular()[1],mtl.getSpecular()[2],mtl.getSpecular()[3] };
    GLfloat shininess = mtl.getShininess();

    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_EMISSION, emissionArray);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArray);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArray);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularArray);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glutSolidSphere(radius, slice, stack);
    glPopMatrix();
}


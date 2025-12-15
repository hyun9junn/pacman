#include"Ghost.h"
#include"Constants.h"
#include<GL/freeglut.h>
#include<iostream>
using namespace std;

Ghost::Ghost(float r, int sl, int st, STATE s, int t) {
	radius = r;
	slice = sl;
	stack = st;
	state = s;
	frighttime = t;
}
void Ghost::setfright_time(int t) {
	frighttime = t;
}

int Ghost::getfright_time() {
	return frighttime;
}

void Ghost::setState(STATE s) {
	state = s;
}
Ghost::STATE Ghost::getState() const {
	return state;
}


void Ghost::draw() const {
	/*
	// CHASE,SCATTER color
	GLfloat emissionArray[4] = { mtl.getEmission()[0],mtl.getEmission()[1],mtl.getEmission()[2],mtl.getEmission()[3] };
	GLfloat ambientArray[4] = { mtl.getAmbient()[0],mtl.getAmbient()[1],mtl.getAmbient()[2],mtl.getAmbient()[3] };
	GLfloat diffuseArray[4] = { mtl.getDiffuse()[0],mtl.getDiffuse()[1],mtl.getDiffuse()[2],mtl.getDiffuse()[3] };
	GLfloat specularArray[4] = { mtl.getSpecular()[0],mtl.getSpecular()[1],mtl.getSpecular()[2],mtl.getSpecular()[3] };
	GLfloat shininess = mtl.getShininess();
	//FRIGHTENED color
	GLfloat ambientArray1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuseArray1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularArray1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat emissionArray1[] = { 0.1f, 0.0f, 0.0f, 1.0f };
	GLfloat shininess1 = 50.0f;
	//EATEN color
	GLfloat ambientArray2[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat diffuseArray2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularArray2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat emissionArray2[] = { 0.1f, 0.0f, 0.0f, 1.0f };
	GLfloat shininess2 = 50.0f;
	glPushMatrix();
	glTranslatef(center[0], center[1], center[2]);
	glShadeModel(GL_SMOOTH);

	if (state == CHASE||state==SCATTER) {
		glMaterialfv(GL_FRONT, GL_EMISSION, emissionArray);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArray);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArray);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularArray);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}
	else if(state==FRIGHTENED){
		if (frighttime < 120 && frighttime % 20 >= 0 && frighttime % 20 < 10) {
			glMaterialfv(GL_FRONT, GL_EMISSION, emissionArray);
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArray);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArray);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specularArray);
			glMaterialf(GL_FRONT, GL_SHININESS, shininess);
		}
		else {
			glMaterialfv(GL_FRONT, GL_EMISSION, emissionArray1);
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArray1);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArray1);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specularArray1);
			glMaterialf(GL_FRONT, GL_SHININESS, shininess1);
		}
	}
	else {
		glMaterialfv(GL_FRONT, GL_EMISSION, emissionArray2);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArray2);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArray2);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularArray2);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess2);
	}
	glutSolidSphere(radius, slice, stack);
	glPopMatrix();
	*/

	glPushMatrix();
	glTranslatef(center[0], center[1], center[2]);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_TEXTURE_2D);

	// 현재 상태에 따라 적절한 텍스처를 바인딩
	if (state == CHASE || state == SCATTER) {
		if (time % 20 >= 0 && time % 20 < 10) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
		}
	}
	else if (state == FRIGHTENED) {
		if (frighttime < 120 && frighttime % 20 >= 0 && frighttime % 20 < 10) {
			if (time % 20 >= 0 && time % 20 < 10) {
				glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
			}
		}
		else {
			glBindTexture(GL_TEXTURE_2D, textureIDs[2]);
		}
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textureIDs[3]);
	}

	// 텍스처 매핑
	float size = radius * 2; // Ghost 크기
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2, -size / 2, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2, -size / 2, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2, size / 2, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2, size / 2, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Ghost::loadTextures(const std::vector<std::string>& textureFiles) {
	for (size_t i = 0; i < textureFiles.size(); ++i) {
		textureIDs[i] = TextureHandler::createTexture(textureFiles[i]);
	}
}


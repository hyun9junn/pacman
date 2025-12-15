#include"PacMan.h"
#include"Constants.h"
#include<GL/freeglut.h>
#include<iostream>
#include"CollisionHandler.h"
using namespace std;

PacMan::PacMan(float r, int sl, int st, bool bCol, int l) {
	radius = r;
	slice = sl;
	stack = st;
	bCollided = bCol;
	life = l;
}

void PacMan:: setlife(int l) {
	life = l;
}
int PacMan::getlife(){
	return life;
}
void PacMan::setCollided(bool bCol) {
	bCollided = bCol;
}
void PacMan::setghost_time(int t) {
	ghosttime = t;
}

int PacMan::getghost_time() {
	return ghosttime;
}
void PacMan::loadTextures(const std::vector<std::string>& textureFiles) {
	for (size_t i = 0; i < textureFiles.size(); ++i) {
		textureIDs[i] = TextureHandler::createTexture(textureFiles[i]);
	}
}

void PacMan::draw() const {
	/*
	// not collided color
	GLfloat emissionArray[4] = { mtl.getEmission()[0],mtl.getEmission()[1],mtl.getEmission()[2],mtl.getEmission()[3] };
	GLfloat ambientArray[4] = { mtl.getAmbient()[0],mtl.getAmbient()[1],mtl.getAmbient()[2],mtl.getAmbient()[3] };
	GLfloat diffuseArray[4] = { mtl.getDiffuse()[0],mtl.getDiffuse()[1],mtl.getDiffuse()[2],mtl.getDiffuse()[3] };
	GLfloat specularArray[4] = { mtl.getSpecular()[0],mtl.getSpecular()[1],mtl.getSpecular()[2],mtl.getSpecular()[3] };
	GLfloat shininess = mtl.getShininess();
	//collided color
	GLfloat ambientArray1[] = { 0.5f, 0.0f, 0.5f, 1.0f };
	GLfloat diffuseArray1[] = { 0.7f, 0.0f, 0.7f, 1.0f };
	GLfloat specularArray1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat emissionArray1[] = { 0.2f, 0.0f, 0.2f, 1.0f };
	GLfloat shininess1 = 60.0f;
	glPushMatrix();
	glTranslatef(center[0], center[1], center[2]);
	glShadeModel(GL_SMOOTH);
	//cout << "collision_counter " << collision_counter << endl;
	
	if (collision_counter == 0) {
		glMaterialfv(GL_FRONT, GL_EMISSION, emissionArray);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArray);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArray);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularArray);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}
	else if (collision_counter > 0 && (collision_counter % 20 >= 0 && collision_counter % 20 < 10)) {
		glMaterialfv(GL_FRONT, GL_EMISSION, emissionArray1);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArray1);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArray1);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularArray1);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess1);
	}
	else {
		glMaterialfv(GL_FRONT, GL_EMISSION, emissionArray);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArray);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArray);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularArray);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}
	*/

	glPushMatrix();
	glTranslatef(center[0], center[1], center[2]);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_TEXTURE_2D);

	int time_ratio = 180;
	// 현재 상태에 따라 적절한 텍스처를 바인딩
	switch (getCurrentDirection()) {
	case Sphere::UP:
		if (time % time_ratio >= 0 && time % time_ratio < time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
		}
		else if (time % time_ratio >= time_ratio / 4 && time % time_ratio < 2 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
		}
		else if (time % time_ratio >= 2 * time_ratio / 4 && time % time_ratio < 3 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[2]);
		}
		else if (time % time_ratio >= 3 * time_ratio / 4 && time % time_ratio < 4 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[3]);
		}
		break;
	case Sphere::LEFT:
		if (time % time_ratio >= 0 && time % time_ratio < time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[4]);
		}
		else if (time % time_ratio >= time_ratio / 4 && time % time_ratio < 2 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[5]);
		}
		else if (time % time_ratio >= 2 * time_ratio / 4 && time % time_ratio < 3 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[6]);
		}
		else if (time % time_ratio >= 3 * time_ratio / 4 && time % time_ratio < 4 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[7]);
		}
		break;
	case Sphere::DOWN:
		if (time % time_ratio >= 0 && time % time_ratio < time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[8]);
		}
		else if (time % time_ratio >= time_ratio / 4 && time % time_ratio < 2 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[9]);
		}
		else if (time % time_ratio >= 2 * time_ratio / 4 && time % time_ratio < 3 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[10]);
		}
		else if (time % time_ratio >= 3 * time_ratio / 4 && time % time_ratio < 4 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[11]);
		}
		break;
	case Sphere::RIGHT:
		if (time % time_ratio >= 0 && time % time_ratio < time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[12]);
		}
		else if (time % time_ratio >= time_ratio / 4 && time % time_ratio < 2 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[13]);
		}
		else if (time % time_ratio >= 2 * time_ratio / 4 && time % time_ratio < 3 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[14]);
		}
		else if (time % time_ratio >= 3 * time_ratio / 4 && time % time_ratio < 4 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[15]);
		}
		break;
	case Sphere::NONE:
		if (time % time_ratio >= 0 && time % time_ratio < time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[12]);
		}
		else if (time % time_ratio >= time_ratio / 4 && time % time_ratio < 2 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[13]);
		}
		else if (time % time_ratio >= 2 * time_ratio / 4 && time % time_ratio < 3 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[14]);
		}
		else if (time % time_ratio >= 3 * time_ratio / 4 && time % time_ratio < 4 * time_ratio / 4) {
			glBindTexture(GL_TEXTURE_2D, textureIDs[15]);
		}
		break;
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


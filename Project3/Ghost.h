#pragma once
#include "Sphere.h"
#include "TextureHandler.h"
#include <vector>
#include <string>

class Ghost : public Sphere
{
public:
	enum STATE { CHASE, SCATTER, FRIGHTENED, EATEN};
	Ghost(float r, int sl, int st, STATE s, int t);

	void setState(STATE s);
	void setfright_time(int t);
	int getfright_time();
	STATE getState() const;
	virtual void draw() const;


	void loadTextures(const std::vector<std::string>& textureFiles);

private:
	STATE state;
	int frighttime;

	GLuint textureIDs[4];
};

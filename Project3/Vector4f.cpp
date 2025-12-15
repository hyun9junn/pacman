#include"Vector4f.h"

Vector4f::Vector4f() {
	pos[0] = 0;	pos[1] = 0; pos[2] = 0; pos[3] = 0;
}
Vector4f::Vector4f(float a, float b, float c, float d) {
	pos[0] = a; pos[1] = b; pos[2] = c; pos[3] = d;
}
void Vector4f::setPos(float a, float b, float c, float d) {
	pos[0] = a; pos[1] = b; pos[2] = c; pos[3] = d;
}
float& Vector4f::operator[](const int i) {
	return pos[i];
}
float Vector4f::operator[](const int i) const {
	return pos[i];
}


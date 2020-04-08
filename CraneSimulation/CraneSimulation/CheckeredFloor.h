#include "GL/glew.h"

class CheckeredFloor
{
public:
	CheckeredFloor() = default;
	CheckeredFloor(int size, int nsquare, float yPos);

	~CheckeredFloor() = default;

	void draw();

private:
	int _floor_size;
	int _nsquare;

	int _nb_floor_vetices;

	float _yPos;

	std::vector<glm::vec3> _floor_vertices;
	std::vector<glm::vec3> _floor_colors;

	GLuint _vaoHandle;
	GLuint _vbo_floor_vertices, _vbo_floor_colors;

	void setupFloor();
	void setupGl();
};


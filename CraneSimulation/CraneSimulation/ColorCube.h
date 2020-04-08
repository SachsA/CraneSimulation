#include <iostream>
#include <vector>

#include "GL/glew.h"

class ColorCube
{
public:

	ColorCube();
	~ColorCube() = default;

	void draw();

private:

	std::vector<glm::vec4> _cube_vertices;
	std::vector<glm::vec3> _cube_colors;

	GLuint _vaoHandle;
	GLuint _vbo_cube_vertices, _vbo_cube_colors;
	GLuint _ibo_cube_elements;

	void setupColorCube();
	void setupVertices();
	void setupColor();

	void setupGl();
};
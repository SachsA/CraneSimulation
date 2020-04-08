#include <iostream>
#include <vector>

#include "GL/glew.h"

class WireCube
{
public:

	WireCube();
	~WireCube() = default;

	void draw();

private:

	std::vector<glm::vec4> _cube_vertices;
	std::vector<glm::vec3> _cube_colors;

	GLuint _vaoHandle;
	GLuint _vbo_cube_vertices, _vbo_cube_colors;

	void setupWireCube();
	void setupVertices();
	void setupColor();

	void setupGl();
};
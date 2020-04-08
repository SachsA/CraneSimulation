#include <iostream>
#include <vector>

#include "GL/glew.h"

class Cable
{
public:
	Cable(glm::vec4 begin, glm::vec4 end);
	~Cable() = default;

	void draw();

private:

	std::vector<glm::vec4> _cable_vertices;
	std::vector<glm::vec3> _cable_colors;

	GLuint _vaoHandle;
	GLuint _vbo_cable_vertices, _vbo_cable_colors;

	glm::vec4 _m_cable_begin;
	glm::vec4 _m_cable_end;

	void setupCable();
	void setupVertices();
	void setupColor();

	void setupGl();
};
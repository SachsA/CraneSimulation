#include <glm/gtc/type_ptr.hpp>

#include "Cable.h"

Cable::Cable(glm::vec4 begin, glm::vec4 end) :
	_m_cable_begin(begin), _m_cable_end(end)
{
	setupCable();
	setupGl();
}

void Cable::setupCable()
{
	setupVertices();
	setupColor();
}

void Cable::setupVertices()
{
	_cable_vertices.push_back(_m_cable_begin);
	_cable_vertices.push_back(_m_cable_end);
}

void Cable::setupColor()
{
	_cable_colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	_cable_colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
}

// Call from constructor
void Cable::setupGl()
{
	// Create VAO
	glGenVertexArrays(1, &_vaoHandle); // 1 : of VAOs, vaoHandle : vao handler
	glBindVertexArray(_vaoHandle); // Bind means that "i want to do something" : activate VAO

	// Create VBO 0 : vertex
	glGenBuffers(1, &_vbo_cable_vertices);

	// GL_ARRAY_BUFFER : type = buffer for containing vertex attribute
	// Bind : activate VBO, "I want to do something on this vbo"
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_cable_vertices);

	// Allocate memory on GPU and copy data
	// GL_STATIC_DRAW : This data are not going to be changed
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _cable_vertices.size() * 4, _cable_vertices.data(), GL_STATIC_DRAW);

	// Let GPU know how to interpret this data...
	glVertexAttribPointer(
		0, // Attribute.number : "I'm going to set 0 for vertex positions"
		4, // Data for vertex (x,y,z,w)
		GL_FLOAT, // Data type
		GL_FALSE, // "Is it normalized?"
		0, // Offset between real data
		0 // Offset from the starting of buffer
	);
	glEnableVertexAttribArray(0); //Activate vertex attribute 0

	glGenBuffers(1, &_vbo_cable_colors);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_cable_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _cable_colors.size() * 3, _cable_colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Close/Desactivate VAO
}

void Cable::draw()
{
	glBindVertexArray(_vaoHandle);

	glLineWidth(2.0);

	glDrawArrays(GL_LINES, 0, 2);
}
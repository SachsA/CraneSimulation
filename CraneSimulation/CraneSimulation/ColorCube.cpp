#include <glm/gtc/type_ptr.hpp>

#include "ColorCube.h"

ColorCube::ColorCube()
{
	setupColorCube();
	setupGl();
}

void ColorCube::setupColorCube()
{
	setupVertices();
	setupColor();
}

void ColorCube::setupVertices()
{
	_cube_vertices.push_back(glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, -0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, 0.5f, -0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f));
}

void ColorCube::setupColor()
{
	_cube_colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	_cube_colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	_cube_colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	_cube_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	_cube_colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	_cube_colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	_cube_colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	_cube_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
}

// Call from constructor
void ColorCube::setupGl()
{
	// Create VAO
	glGenVertexArrays(1, &_vaoHandle); // 1 : of VAOs, vaoHandle : vao handler
	glBindVertexArray(_vaoHandle); // Bind means that "i want to do something" : activate VAO

	// Create VBO 0 : vertex
	glGenBuffers(1, &_vbo_cube_vertices);

	// GL_ARRAY_BUFFER : type = buffer for containing vertex attribute
	// Bind : activate VBO, "I want to do something on this vbo"
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_cube_vertices);

	// Allocate memory on GPU and copy data
	// GL_STATIC_DRAW : This data are not going to be changed
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _cube_vertices.size() * 4, _cube_vertices.data(), GL_STATIC_DRAW);

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

	glGenBuffers(1, &_vbo_cube_colors);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_cube_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _cube_colors.size() * 3, _cube_colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glEnableVertexAttribArray(1);

	GLushort cube_elements[] = {
	0, 1, 2, 2, 3, 0, 1, 5, 6,
	6, 2, 1, 7, 6, 5, 5, 4, 7,
	4, 0, 3, 3, 7, 4, 4, 5, 1,
	1, 0, 4, 3, 2, 6, 6, 7, 3,
	};

	glGenBuffers(1, &_ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

	glBindVertexArray(0); // Close/Desactivate VAO
}

void ColorCube::draw()
{
	int size;

	glBindVertexArray(_vaoHandle);
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}
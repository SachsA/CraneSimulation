#include <vector>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "CheckeredFloor.h"

CheckeredFloor::CheckeredFloor(int size, int nsquare, float yPos) :
	_nb_floor_vetices(0), _floor_size(size), _nsquare(nsquare), _yPos(yPos)
{
	setupFloor();
	setupGl();
}

void CheckeredFloor::setupFloor()
{
	glm::vec3 floor_colors1 = { .7f, .7f, .7f };
	glm::vec3 floor_colors2 = { .3f, .3f, .3f };

	float maxX = _floor_size / 2, maxY = _floor_size / 2;
	float minX = -_floor_size / 2, minY = -_floor_size / 2;

	int x, y, i = 0;

	float xp, yp;

	float xd = (maxX - minX) / ((float)_nsquare);
	float yd = (maxY - minY) / ((float)_nsquare);

	for (x = 0, xp = minX; x < _nsquare; x++, xp += xd) {
		for (y = 0, yp = minY, i = x; y < _nsquare; y++, i++, yp += yd) {

			for (int c = 0; c < 6; c++)
				i % 2 == 1 ? _floor_colors.push_back(floor_colors1) : _floor_colors.push_back(floor_colors2);

			_floor_vertices.push_back(glm::vec3(xp, _yPos, yp));
			_floor_vertices.push_back(glm::vec3(xp, _yPos, yp + yd));
			_floor_vertices.push_back(glm::vec3(xp + xd, _yPos, yp + yd));
			_floor_vertices.push_back(glm::vec3(xp, _yPos, yp));
			_floor_vertices.push_back(glm::vec3(xp + xd, _yPos, yp));
			_floor_vertices.push_back(glm::vec3(xp + xd, _yPos, yp + yd));

			_nb_floor_vetices += 6;
		}
	}// end of for i
}

// Call from constructor
void CheckeredFloor::setupGl()
{	
	// Create VAO
	glGenVertexArrays(1, &_vaoHandle); // 1 : of VAOs, vaoHandle : vao handler
	glBindVertexArray(_vaoHandle); // Bind means that "i want to do something" : activate VAO

	// Create VBO 0 : vertex
	glGenBuffers(1, &_vbo_floor_vertices);

	// GL_ARRAY_BUFFER : type = buffer for containing vertex attribute
	// Bind : activate VBO, "I want to do something on this vbo"
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_floor_vertices);

	// Allocate memory on GPU and copy data
	// GL_STATIC_DRAW : This data are not going to be changed
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _floor_vertices.size() * 3, _floor_vertices.data(), GL_STATIC_DRAW);

	// Let GPU know how to interpret this data...
	glVertexAttribPointer(
		0, // Attribute.number : "I'm going to set 0 for vertex positions"
		3, // Data for vertex (x,y,z,w)
		GL_FLOAT, // Data type
		GL_FALSE, // "Is it normalized?"
		0, // Offset between real data
		0 // Offset from the starting of buffer
	);
	glEnableVertexAttribArray(0); //Activate vertex attribute 0

	glGenBuffers(1, &_vbo_floor_colors);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_floor_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _floor_colors.size() * 3, _floor_colors.data(), GL_STATIC_DRAW);
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

void CheckeredFloor::draw()
{
	glBindVertexArray(_vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, _nb_floor_vetices);
}
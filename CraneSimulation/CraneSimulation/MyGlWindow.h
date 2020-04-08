//#define  FREEGLUT_LIB_PRAGMAS  0

#pragma warning(push)

// Convert void* to long
#pragma warning(disable:4311)

// Convert long to void*
#pragma warning(disable:4312)

#include <iostream>
#include "GL/glew.h"
#include <string>
#include <stack>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#pragma warning(pop)

#include "Loader.h"

#include "Model.h"
#include "Viewer.h"

#include "ColorCube.h"
#include "WireCube.h"
#include "CheckeredFloor.h"

class MyGlWindow {
public:
	Viewer *m_viewer;

	MyGlWindow() = default;
	~MyGlWindow() = default;

	MyGlWindow(int w, int h);

	void setSize(int w, int h);
	void setAspect(float r);
	void setAngle(float angle);

	void setCamera(bool camera);

	void setHorizontalCubes(int horizontalCubes);
	void setVerticalCubes(int veticalCubes);

	void setHorizontalTranslation(float horizontalTranslation);
	void setVerticalTranslation(float verticalTranslation);

	void draw();

private:
	int _m_width;
	int _m_height;

	int _nb_horizontal_cubes;
	int _nb_vertical_cubes;

	bool _camera;

	float _horizontal_translation;
	float _vertical_translation;

	float _angle;

	glm::mat4 _cubeModel;

	glm::vec4 _m_cable_begin;
	glm::vec4 _m_cable_end;

	ColorCube *_m_cube;
	WireCube *_m_wire_cube;
	CheckeredFloor *_m_floor;

	Model _m_model;

	ShaderProgram *_shaderProgram;

	void setupShader();
	void setupViewer();

	void initializeData(int w, int h);

	glm::mat4 getMVP();
};
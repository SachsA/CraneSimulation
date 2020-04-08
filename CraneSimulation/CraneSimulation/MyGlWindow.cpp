#include <glm/gtc/type_ptr.hpp>

#include "Cable.h"
#include "MyGlWindow.h"
#include "MyGlUtiles.h"

static float DEFAULT_VIEW_POINT[3] = { 5, 10, -20 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 1, 5 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

MyGlWindow::MyGlWindow(int w, int h)
{
	setupShader();
	setupViewer();
	initializeData(w, h);
}

void MyGlWindow::setupShader()
{
	_shaderProgram = new ShaderProgram();
	_shaderProgram->initFromFiles("simple.vert", "simple.frag");

	_shaderProgram->addUniform("mvp");
}

void MyGlWindow::setupViewer()
{
	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (float)(_m_width / _m_height);
	
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);
}

void MyGlWindow::initializeData(int w, int h)
{
	_m_width = w;
	_m_height = h;

	_m_cube = new ColorCube();
	_m_wire_cube = new WireCube();
	_m_floor = new CheckeredFloor(50, 16, -0.52);
}

void MyGlWindow::setSize(int w, int h)
{
	_m_width = w;
	_m_height = h;
}

void MyGlWindow::setCamera(bool camera)
{
	_camera = camera;
}

void MyGlWindow::setAspect(float r)
{
	m_viewer->setAspectRatio(r);
}

void MyGlWindow::setAngle(float angle)
{
	_angle = glm::degrees(angle);
}

void MyGlWindow::setHorizontalCubes(int horizontalCubes)
{
	_nb_horizontal_cubes = horizontalCubes;
}

void MyGlWindow::setVerticalCubes(int verticalCubes)
{
	_nb_vertical_cubes = verticalCubes;
}

void MyGlWindow::setHorizontalTranslation(float horizontalTranslation)
{
	_horizontal_translation = horizontalTranslation;
}

void MyGlWindow::setVerticalTranslation(float verticalTranslation)
{
	_vertical_translation = verticalTranslation;
}

glm::mat4 MyGlWindow::getMVP()
{
	glm::vec3 eye;
	glm::vec3 look;
	glm::mat4 model = _m_model.getMatrix();

	if (_camera == true) {
		eye = m_viewer->getViewPoint();
		look = m_viewer->getViewCenter();
	}
	else
	{
		eye = _cubeModel * glm::vec4(0, 0, 0, 1);
		look = _cubeModel * glm::vec4(1, 0, 0, 1);
	}

	glm::vec3 up = m_viewer->getUpVector();
	glm::mat4 view = glm::lookAt(eye, look, up);
	glm::mat4 projection = glm::perspective(45.0f, 1.0f * _m_width / _m_height, 0.1f, 500.0f);

	return projection * view * model;
}

void MyGlWindow::draw()
{
	//Call our shader
	_shaderProgram->use();

	glm::mat4 mvp = getMVP();
	glUniformMatrix4fv(_shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
	if (_m_floor)
		_m_floor->draw();

	_m_model.glPushMatrix();

	_m_model.glRotate(_angle, 0, 1, 0);

	_m_model.glTranslate(1, _nb_vertical_cubes - 1, 0);

	_m_model.glTranslate(
		_horizontal_translation * (_nb_horizontal_cubes - 3),
		-_vertical_translation * (_nb_vertical_cubes - 1),
		0
	);

	mvp = getMVP();
	_cubeModel = _m_model.getMatrix();
	glUniformMatrix4fv(_shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
	if (_m_cube && _camera == true)
		_m_cube->draw();

	_m_model.glPopMatrix();

	_m_model.glPushMatrix();

	for (size_t i = 0; i < _nb_vertical_cubes; i++)
	{
		mvp = getMVP();
		glUniformMatrix4fv(_shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
		if (_m_wire_cube)
			_m_wire_cube->draw();

		_m_model.glTranslate(0, 1, 0);
	}

	_m_model.glRotate(_angle, 0, 1, 0);

	mvp = getMVP();
	glUniformMatrix4fv(_shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
	if (_m_wire_cube)
		_m_wire_cube->draw();

	_m_model.glTranslate(-1, 0, 0);

	for (size_t i = 0; i < _nb_horizontal_cubes; i++)
	{
		mvp = getMVP();
		glUniformMatrix4fv(_shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
		if (_m_wire_cube)
			_m_wire_cube->draw();

		_m_model.glTranslate(1, 0, 0);
	}

	_m_model.glPopMatrix();

	_m_model.glPushMatrix();

	_m_model.glRotate(_angle, 0, 1, 0);

	_m_cable_begin = glm::vec4(_horizontal_translation * (_nb_horizontal_cubes - 3) + 1, _nb_vertical_cubes - 0.5, 0, 1);
	_m_cable_end = glm::vec4(_horizontal_translation * (_nb_horizontal_cubes - 3) + 1, (_nb_vertical_cubes - 1) - (_vertical_translation * (_nb_vertical_cubes - 1)), 0, 1);
	Cable *cable = new Cable(_m_cable_begin, _m_cable_end);
	
	mvp = getMVP();
	glUniformMatrix4fv(_shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
	if (cable)
		cable->draw();
	
	_m_model.glPopMatrix();

	_shaderProgram->disable();
}
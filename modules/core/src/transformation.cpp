#include "transformation.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "logger.h"
#define LOG_TAG "Transformation"

using namespace gre;

Transformation::Transformation()
{

}

Transformation::~Transformation()
{

}

void Transformation::setScale(glm::vec3 scale)
{
	m_scale = scale;
	computeLocalMatrix();
}

void Transformation::setRotation(float angle, glm::vec3 axis)
{
	m_rotationAngle = angle;
	m_rotationAxis = axis;
	computeLocalMatrix();
}

void Transformation::setTranslation(glm::vec3 translation)
{
	m_translation = translation;
	computeLocalMatrix();
}

void Transformation::computeLocalMatrix()
{
	glm::mat4 t = glm::mat4(1.0);
	t = glm::translate(t, m_translation);
	t = glm::rotate(t, m_rotationAngle, m_rotationAxis);
	t = glm::scale(t, m_scale);
	this->setLocalTranslation(t);
}

void Transformation::scale(glm::vec3 scale)
{
	m_scale += scale;
	computeLocalMatrix();
}

void Transformation::rotate(float angle, glm::vec3 axis)
{
	m_rotationAngle += angle;
	m_rotationAxis += axis;
	computeLocalMatrix();
}

void Transformation::translate(glm::vec3 translation)
{
	m_translation += translation;
	computeLocalMatrix();
}

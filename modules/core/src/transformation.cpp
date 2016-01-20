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
    glm::mat4 s = glm::scale(glm::mat4(1.0), m_scale);
    setLocalScale(s);
    //computeLocalMatrix();
}

void Transformation::setRotation(float angle, glm::vec3 axis)
{
	m_rotationAngle = angle;
	m_rotationAxis = axis;
    glm::mat4 r = glm::rotate(glm::mat4(1.0), m_rotationAngle, m_rotationAxis);
    setLocalRotation(r);
    //computeLocalMatrix();
}

void Transformation::setTranslation(glm::vec3 translation)
{
	m_translation = translation;
    glm::mat4 t = glm::translate(glm::mat4(1.0), m_translation);
    setLocalTranslation(t);
    //computeLocalMatrix();
}

//void Transformation::computeLocalMatrix()
//{
//	glm::mat4 t = glm::mat4(1.0);
//    t = glm::translate(t, m_translation);
//    t = glm::rotate(t, m_rotationAngle, m_rotationAxis);
//    t = glm::scale(t, m_scale);
//    this->setLocalTranslation(t);
//}

void Transformation::scale(glm::vec3 scale)
{
    setScale(m_scale + scale);
    //computeLocalMatrix();
}

void Transformation::rotate(float angle, glm::vec3 axis)
{
    setRotation(m_rotationAngle+angle, m_rotationAxis+axis);
    //computeLocalMatrix();
}

void Transformation::translate(glm::vec3 translation)
{
    setTranslation(m_translation+translation);
    //computeLocalMatrix();
}

#ifndef ORTHOCAMERA_H
#define ORTHOCAMERA_H

#include "camera.hpp"

#include <glm/glm.hpp>
namespace gre
{

enum CameraConfigPrior{
    GRE_USE_WIDTH,
    GRE_USE_HEIGHT
};

class OrthoCamera : public Camera
{
public:
    OrthoCamera();
    ~OrthoCamera();

    void setConfiguration(float left, float right, float bottom, float top, float near, float far);
    void setConfiguration(float refSizeValue, float near, float far, CameraConfigPrior refDim=GRE_USE_WIDTH);
    void setLocation( const glm::vec3& position,           // Camera is here
                      const glm::vec3& target, // and looks here : at the same position, plus "direction"
                      const glm::vec3& up);

    float getLeftValue()    {return m_left;}
    float getRightValue()   {return m_right;}
    float getBottomValue()  {return m_bottom;}
    float getTopValue()     {return m_top;}
    float getNearValue()    {return m_near;}
    float getFarValue()     {return m_far;}

private:
    float m_left;
    float m_right;
    float m_bottom;
    float m_top;
    float m_near;
    float m_far;

};

}

#endif

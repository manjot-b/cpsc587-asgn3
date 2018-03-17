#include "Camera.h"

Camera::Camera(glm::mat4 view, glm::mat4 perspective)
: viewMatrix_(view), projectionMatrix_(perspective)
{

}

const glm::mat4& Camera::getProjectionMatrix() const 
{
    return projectionMatrix_;
}

const glm::mat4& Camera::getViewMatrix() const 
{
    return viewMatrix_;
}

glm::mat4 Camera::getProjectionViewMatrix() const 
{
    return projectionMatrix_ * viewMatrix_;
}
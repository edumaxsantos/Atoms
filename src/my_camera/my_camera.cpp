#include "my_camera.h"

MyCamera::MyCamera(Vector3 position, Vector3 target, float fovY) {
    this->camera.position = position;
    this->camera.target = target;
    this->camera.up = Vector3{0.0f, 1.0f, 0.0f};
    this->camera.fovy = fovY;
    this->camera.projection = CAMERA_PERSPECTIVE;
}

Camera3D *MyCamera::getCamera() {
    return &this->camera;
}
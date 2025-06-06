#ifndef MYCAMERA_H
#define MYCAMERA_H
#include "raylib.h"

class MyCamera {
    Camera3D camera;

    public:
        MyCamera(Vector3 position, Vector3 target, float fovY);
        Camera3D *getCamera();
};

#endif // MYCAMERA_H
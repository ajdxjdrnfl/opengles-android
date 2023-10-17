#ifndef HW2_SCENE_H
#define HW2_SCENE_H

#include "global.h"
#include "program.h"
#include "camera.h"
#include "object.h"
#include "texture.h"
#include "material.h"
#include "light.h"

class Scene {
public:
    static void setup(AAssetManager* aAssetManager);
    static void screen(int width, int height);
    static void update(float deltaTime);

    static void mouseUpEvents(float x, float y, bool doubleTouch);
    static vec4 inverseTransform(float x, float y, bool isPoint);
    static vec3 calculateArcballvec(float x, float y);
    static void mouseDownEvents(float x, float y, bool doubleTouch);
    static void mouseMoveEvents(float x, float y, bool doubleTouch);

private:
    static Shader* vertexShader;
    static Shader* fragmentShader;
    static Program* program;
    static Camera* camera;
    static Object* teapot;
    static Texture* diffuse;
    static Texture* dissolve;
    static Material* material;
    static Light* light;
    static int _width;
    static int _height;
    static vec3 oldVec;
};

#endif //HW2_SCENE_H

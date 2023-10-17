#include "main.h"

void surfaceCreated(AAssetManager* aAssetManager) {

    glClearDepthf(1.0f);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Scene::setup(aAssetManager);
}

void surfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
    Scene::screen(width, height);
}

void drawFrame(float deltaTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Scene::update(deltaTime);
}

void mouseDownEvents(float x, float y, bool doubleTouch)
{
    Scene::mouseDownEvents(x,y,doubleTouch);
}
void mouseMoveEvents(float x, float y, bool doubleTouch)
{
    Scene::mouseMoveEvents(x,y,doubleTouch);
}

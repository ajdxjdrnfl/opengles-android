#include "global.h"
#include "scene.h"

void surfaceCreated(AAssetManager* aAssetManager);
void surfaceChanged(int width, int height);
void drawFrame(float deltaTime);
void mouseDownEvents(float x, float y, bool doubleTouch);
void mouseMoveEvents(float x, float y, bool doubleTouch);

extern "C"
JNIEXPORT void JNICALL
Java_korea_media_test_MainActivity_surfaceCreated(JNIEnv* env, jobject instance, jobject assetManager) {
    surfaceCreated(AAssetManager_fromJava(env, assetManager));
}

extern "C"
JNIEXPORT void JNICALL
Java_korea_media_test_MainActivity_surfaceChanged(JNIEnv* env, jobject instance, jint width, jint height) {
    surfaceChanged(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_korea_media_test_MainActivity_drawFrame(JNIEnv* env, jobject instance, jfloat deltaTime) {
    drawFrame(deltaTime);
}

extern "C"
JNIEXPORT void JNICALL
Java_korea_media_test_MainActivity_mouseDownEvents(JNIEnv* env, jobject instance, jfloat x, jfloat y, jboolean doubleTouch) {
    mouseDownEvents(x,y,doubleTouch);
}

extern "C"
JNIEXPORT void JNICALL
Java_korea_media_test_MainActivity_mouseMoveEvents(JNIEnv* env, jobject instance, jfloat x, jfloat y, jboolean doubleTouch) {
    mouseMoveEvents(x,y,doubleTouch);
}

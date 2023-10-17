#include "scene.h"
#include "binary/teapot.h"
#include "binary/floral.h"
#include "binary/cloud.h"
#include "checker.h"

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Light* Scene::light = nullptr;
Object* Scene::teapot = nullptr;
Material* Scene::material = nullptr;
Texture* Scene::diffuse = nullptr;
Texture* Scene::dissolve = nullptr;

int Scene::_width = 0;
int Scene::_height = 0;

void Scene::setup(AAssetManager* aAssetManager) {

    Asset::setManager(aAssetManager);

    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(10.0f, 20.0f, 10.0f);

    Scene::light = new Light(program);
    light->direction = vec3(-3.0f, 9.0f, 9.0f);

    Scene::diffuse  = new Texture(Scene::program, 0, "textureDiff", floralTexels, floralSize);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    Scene::dissolve = new Texture(Scene::program, 1, "textureDissolve", cloudTexels, cloudSize);
    Scene::material = new Material(Scene::program, diffuse, dissolve);
    Scene::teapot = new Object(program, material, teapotVertices, teapotIndices);
    Scene::teapot->origin = vec3(0.0f, 0.0f, 0.0f);
    Scene::teapot->worldMat =  translate(teapot->origin) * rotate(radians(60.0f), vec3(1.0, 1.0, 0.0));
}

void Scene::mouseUpEvents(float x, float y, bool doubleTouch)
{
}

void Scene::screen(int width, int height) {

    _width = width;
    _height = height;

    // set camera aspect ratio
    camera->aspect = (float) width / height;
}

void Scene::update(float deltaTime) {

    // use program
    program->use();

    camera->update();
    light->update();

    // draw teapot

    teapot->origin = vec3(teapot->worldMat[3][0],
                                 teapot->worldMat[3][1], teapot->worldMat[3][2]);
    teapot->draw();

}

vec3 Scene::oldVec = vec3(0.0f, 0.0f, 0.0f);

vec4 Scene::inverseTransform(float x, float y, bool isPoint)
{
    float camera_x = camera->zNear*camera->aspect*tan(radians(camera->fovy/2))*((2*x)/(_width)-1);
    float camera_y = -camera->zNear*tan(radians(camera->fovy/2))*((2*y)/(_height)-1);

    vec4 camera_startpoint=vec4(camera_x, camera_y, -camera->zNear, 1);
    vec4 camera_direct=vec4(camera_x/camera->zNear, camera_y/camera->zNear, -1, 0);

    vec4 startpoint = inverse(camera->viewMat)*camera_startpoint;
    vec4 direct = inverse(camera->viewMat)*camera_direct;

    if (isPoint==true){
        return startpoint;
    }
    else{
        return direct;
    }
}

vec3 Scene::calculateArcballvec(float x, float y)
{
    float temp_x = (2*x)/_width-1;
    float temp_y = -(2*y)/_height+1;

    vec3 arcvec;

    if(pow(temp_x, 2)+pow(temp_y,2)>1){
        arcvec= normalize(vec3(temp_x,temp_y,0));
    }
    else{
        arcvec=vec3(temp_x, temp_y, sqrt(1-pow(temp_x,2)-pow(temp_y,2)));
    }

    return arcvec;
}

void Scene::mouseDownEvents(float x, float y, bool doubleTouch) {
    vec4 world_startpoint = inverseTransform(x, y, true);
    vec4 world_direct = inverseTransform(x, y, false);
    vec4 startpoint;
    vec4 direct;
    vec3 arcvec;
    LOG_PRINT_DEBUG("mouseDownEvent!\n");
    oldVec=vec3(world_startpoint[0], world_startpoint[1], world_startpoint[2]);
    float radius = 13.0f;
    float minT=10000000.0f;
    float discri;

    if (doubleTouch == false) {

            startpoint = inverse(teapot->worldMat) * world_startpoint;
            direct = inverse(teapot->worldMat) * world_direct;


            float a = pow(direct[0], 2) + pow(direct[1], 2) + pow(direct[2], 2);
            float b = 2 *(startpoint[0]) * direct[0] + 2 * (startpoint[1]) * direct[1] + 2 * (startpoint[2]) * direct[2];
            float c = pow(startpoint[0],2)+pow(startpoint[1],2)+pow(startpoint[2],2)-pow(radius, 2);

            discri = pow(b, 2) - 4 * a * c;



            if (discri >= 0) {
                float t = (-b - sqrt(pow(b, 2) - 4 * a * c))/(2*a);

                if (minT > t) {
                    minT = t;
                }
            }

    }

    else if (doubleTouch==true){
        startpoint = inverse(teapot->worldMat) * world_startpoint;
        direct = inverse(teapot->worldMat) * world_direct;


        float a = pow(direct[0], 2) + pow(direct[1], 2) + pow(direct[2], 2);
        float b = 2 *(startpoint[0]) * direct[0] + 2 * (startpoint[1]) * direct[1] + 2 * (startpoint[2]) * direct[2];
        float c = pow(startpoint[0],2)+pow(startpoint[1],2)+pow(startpoint[2],2)-pow(radius, 2);

        discri = pow(b, 2) - 4 * a * c;




        if (discri >= 0) {
            float t = (-b - sqrt(pow(b, 2) - 4 * a * c))/(2*a);

            if (minT > t) {
                minT = t;
                }
            }

        oldVec = calculateArcballvec(x, y);
    }
}

void Scene::mouseMoveEvents(float x, float y, bool doubleTouch)
{
    vec4 world_startpoint = inverseTransform(x, y, true);

    LOG_PRINT_DEBUG("mouseMoveevent!\n");
    if (doubleTouch==false){

        float coefficient = (camera->projMat*camera->viewMat*vec4(teapot->origin, 1.0f))[2]/camera->zNear;

        teapot->worldMat=translate(vec3(coefficient*(world_startpoint[0]-oldVec[0]), coefficient*(world_startpoint[1]-oldVec[1]), coefficient*(world_startpoint[2]-oldVec[2])))*teapot->worldMat;
        oldVec=vec3(world_startpoint[0], world_startpoint[1], world_startpoint[2]);
    }

    else if (doubleTouch==true){
        vec3 cos_angle = oldVec*calculateArcballvec(x,y);
        float angle = acos(cos_angle[0]+cos_angle[1]+cos_angle[2]);
        vec3 rot_axis=cross(oldVec, calculateArcballvec(x,y));
        vec4 world_axis=inverse(camera->viewMat)*vec4(rot_axis, 0.0f);
        vec4 object_axis=inverse(teapot->worldMat)*world_axis;

        vec3 axis= vec3(object_axis[0], object_axis[1], object_axis[2]);
        teapot->worldMat=teapot->worldMat*rotate(angle, axis);
        oldVec = calculateArcballvec(x, y);
    }
}











#pragma once

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

//define camera class
class camera {

public:
    //variables for opengl
    vec3 eye; //center position
    vec3 at; //point where this camera is pointing out
    vec3 up; // up direction (normalized)

    //other variables
    vec3 fw; //vector pointing forward direction
    float dist = 1; //distance from eye to at
    vec3 right; //vector indicating right direction from the point of camera

    void InitCamera(vec3 _center, vec3 _at, vec3 _up)
    {
        //set three variables and update others
        eye = _center;
        at = _at;
        up = _up;
        UpdateCamera();
    }

    void MoveCamera(vec3 move)
    {
        eye += move;
        at += move;
    }

    void RotateCamera(float angle)
    {
        //rotate direc and up vector
        vec4 tempForward = vec4(fw, 0);
        vec4 tempUp = vec4(up, 0);
        vec4 tempEye = vec4(eye, 0);
        vec4 tempAt = vec4(at, 0);
        mat4 rot = rotate(mat4(1), radians(angle), vec3(0, 0, 1));
        mat4 trans1 = translate(mat4(1), vec3(-0.2, 0, 0));
        mat4 trans2 = translate(mat4(1), vec3(0.2, 0, 0));

        //원점으로 이동
        at = trans1 * tempAt;

        up = rot * tempUp;
        eye = rot * tempEye;

        UpdateCamera();
        //다시 원래위치로 이동
        tempAt = vec4(at, 0);
        at = trans2 * tempAt;
    }

    //update variables accordingly
    void UpdateCamera()
    {
        up = normalize(up);
        dist = length(at - eye);
        fw = normalize(at - eye);
        right = normalize(cross(fw, up));
    }
};
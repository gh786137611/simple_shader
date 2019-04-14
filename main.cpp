#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>


#include "Shader.h"
#include "VBO.h"
#include "Material.h"
#include "Texture2D.h"
#include "EBO.h"
#include "stb_image.h"
#include "point.h"
typedef  Geometry::Point<float> Point;
using namespace std;
/********************************************************/
static const string & vs = R"(
#version 130
in vec3 Vertex;
in vec2 Tex;
uniform mat4 mat;


out vec2 v_Tex;
void main() {
    gl_Position = mat * vec4(Vertex, 1);
    v_Tex = Tex;
}
)";

static const string &fs = R"(
#version 130
in vec2 v_Tex;
uniform sampler2D mySampler2D_1;
uniform sampler2D mySampler2D_2;
uniform float alpha;
void main() {
    gl_FragColor = texture(mySampler2D_1, v_Tex) *alpha + texture(mySampler2D_2, v_Tex) *(1 - alpha);
}
)";
/********************************************************/
shared_ptr<GLU::Shader> commonShader;
shared_ptr<GLU::VBO> vbo;
shared_ptr<GLU::VBO> tbo;
shared_ptr<GLU::EBO> ebo;
shared_ptr<GLU::Material> material;
shared_ptr<GLU::Texture2D> texture2dBeijing3;
shared_ptr<GLU::Texture2D> texture2dBeijing2;
/********************************************************/

/********************************************************/
glm::mat4 ModelView(1.f), Projection (1.f), Transformation(1.f);
/********************************************************/

/********************************************************/
int WinWidth = 800, WinHeight = 600;
int MouseState, MouseButton;
int MouseX, MouseY;
/********************************************************/
void initializeGL() {
    glClearColor(0.5, 0.5, 0.5, 1);
    glEnable(GL_DEPTH_TEST);


    commonShader.reset(new GLU::Shader(vs, fs));
    vbo.reset(new GLU::VBO);
    tbo.reset(new GLU::VBO);
    ebo.reset(new GLU::EBO);
    material.reset(new GLU::Material);
    material->SetAttrib("Vertex", vbo.get());
    material->SetAttrib("Tex", tbo.get());
    texture2dBeijing3.reset(new GLU::Texture2D);
    int w, h, channel;
    unsigned  char *im = stbi_load("../scene.jpeg", &w, &h, &channel, 4);
    assert(im);
    texture2dBeijing3->SetData(w, h, im);
    stbi_image_free(im);

    texture2dBeijing2.reset(new GLU::Texture2D);
    im = stbi_load("../scene2.jpeg", &w, &h, &channel, 4);
    assert(im);
    texture2dBeijing2->SetData(w, h, im);
    stbi_image_free(im);
}

void paintGL() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    vector<Point> vertices={{0,0,0},{1,0,0}, {1,1,0}, {0,0,0},{1,1,0},{0,1,0}};
    vector<float> tex = {0,0,1,0,1,1, 0,0,1,1,0,1};
    vector<unsigned> elements={0,1,2,3,4,5};
    static float alpha = 0;

    glm::mat4 mvp = Projection * ModelView * Transformation;
    material->SetUniform("mat", &mvp[0][0], sizeof(float)*16);
    material->SetUniform("mySampler2D_1", &texture2dBeijing3->Handle(), sizeof(unsigned));
    material->SetUniform("mySampler2D_2", &texture2dBeijing2->Handle(), sizeof(unsigned));
    float t = (float)(sin(alpha) + 1.0f)/2.f;
    material->SetUniform("alpha", &t, sizeof(float));
    vbo->SetData(vertices.data(), vertices.size() * sizeof(Point));
    tbo->SetData(tex.data(), tex.size() * sizeof(float));
    ebo->SetData(elements.data(), elements.size()*sizeof(unsigned));

    commonShader->DrawElements(GL_TRIANGLES, 6, elements.data(), material.get());
    //commonShader->DrawElements(GL_TRIANGLES, 6, ebo.get(), material.get());
    //commonShader->DrawArrays(GL_TRIANGLES, 0, 6, material.get());

    alpha += 0.05;
    if (alpha >=1000) alpha = 0;

    glutSwapBuffers();
}

void resizeGL(int w, int h){
    WinWidth = w;
    WinHeight = h;
    glViewport(0,0,w,h);
    ModelView = glm::lookAt(glm::vec3(0.f, 0.f, 100.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.1f, 1.f, 0.f));
    Projection = glm::perspective(atan(1.f/100.f)*2.f, float(w)/float(h), 1.f, 1000.f);
}

void mouse(int btn, int state, int x, int y){
    if (3 == btn){
        glm::mat4 scale(0.9f);
        scale[3][3] = 1;
        Transformation *= scale;
    }else if (4 == btn) {
        glm::mat4 scale(1.1f);
        scale[3][3] = 1;
        Transformation *= scale;
    }
    MouseState = state;
    MouseX = x;
    MouseY = y;
    MouseButton = btn;
    glutPostRedisplay();
}


void motion(int x, int y){
    if (MouseButton == GLUT_RIGHT_BUTTON && GLUT_DOWN == MouseState) {
        float startx = float(MouseX) / WinWidth * 2.0f- 1;
        float starty = float(WinHeight - MouseY - 1)/WinHeight * 2.f -1;
        float startz = startx *startx + starty * starty;
        if (startz >= 1)
            startz=1;
        else
            startz = sqrt(1 - startz);

        float endx = (float)x/ WinWidth * 2.0f- 1;
        float endy = float(WinHeight - y - 1)/WinHeight * 2.f -1;
        float endz = endx *endx + endy * endy;
        if (endz >= 1)
            endz = 1;
        else
            endz = sqrt(1 - endz);

        Point start(startx, starty, startz), end(endx, endy, endz);
        Point axis = start %end;
        if (axis.length() > 1e-6) {
            axis= axis / axis.length();
            float t = (start | end);
            if (t >1)
                t = 1;
            else if (t <-1)
                t = -1;
            t = acos(t);
            glm::mat4 rotation = glm::rotate(glm::mat4(1.f),t, glm::vec3(axis[0], axis[1],axis[2]));
            Transformation = rotation * Transformation;
            MouseX = x;
            MouseY = y;
        }
    }
    glutPostRedisplay();
}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB| GLUT_DOUBLE);
    glutInitWindowSize(WinWidth, WinHeight);
    glutCreateWindow("GLAB");

    if (GLEW_OK != glewInit()){
        cout <<"failed to init glew" <<endl;
        return 0;
    }
    initializeGL();
    glutDisplayFunc(paintGL);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutReshapeFunc(resizeGL);
    glutMainLoop();
    return 0;
}

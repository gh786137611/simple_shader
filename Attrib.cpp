//
// Created by mm on 19-4-4.
//
#include <cassert>
#include "Attrib.h"
#include "VBO.h"
namespace GLU{

Attrib::Attrib(int location, int size, int type, const std::string &name){
    m_Location = location;
    m_Size = size;
    m_Type = type;
    m_Name = name;
}

void Attrib::bind(const VBO *vbo) {
    vbo->Bind();
    glVertexAttribPointer(m_Location, m_Size, m_Type, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(m_Location);
}

int Attrib::component_size(int type) {
    switch (type){
        case GL_FLOAT:
            return 1;
        case GL_FLOAT_VEC2:
            return 2;
        case GL_FLOAT_VEC3:
            return 3;
        case GL_FLOAT_VEC4:
            return 4;
        default:
            assert(false);
            break;
    }
    return 0;
}

int Attrib::component_type(int type) {
    switch (type){
        case GL_FLOAT:
        case GL_FLOAT_VEC2:
        case GL_FLOAT_VEC3:
        case GL_FLOAT_VEC4:
            return GL_FLOAT;
        default:
            assert(false);
            break;
    }
    return 0;
}
}
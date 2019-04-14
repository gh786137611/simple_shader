//
// Created by mm on 19-4-4.
//

#include "VBO.h"
#include <cassert>

namespace GLU{
VBO::VBO() {
    glGenBuffers(1, &m_Handle); assert(m_Handle);
}
VBO::~VBO(){
    glDeleteBuffers(1, &m_Handle);
}

void VBO::SetData(const void *data, int size) {
    glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
}

}
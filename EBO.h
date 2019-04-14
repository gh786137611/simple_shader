//
// Created by mm on 19-4-8.
//

#ifndef GLAB_EBO_H
#define GLAB_EBO_H

#include <cassert>
#include <GL/glew.h>

namespace GLU{
class Shader;

class EBO{
    friend Shader;
public:
    EBO(){
        glGenBuffers(1, &m_Handle);
        assert(m_Handle);
    }

    void SetData(const void *data, int size) {
        glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
private:
    unsigned m_Handle;
};



}


#endif //GLAB_EBO_H

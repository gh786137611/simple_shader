//
// Created by mm on 19-4-4.
//
#include <iostream>
using namespace std;
#include "Uniforms.h"
namespace GLU {

template<int type>
class UniformT : public Uniform {
    friend Shader;
    friend Uniform;
private:
    UniformT(int loc, const std::string &name) {
        m_Location = loc;
        m_Auxiliary = -1;
        m_Name = name;
    }

    void bind(const void *data) override {
        assert(false);
    }

private:
};


Uniform *Uniform::create(int type, int loc, const std::string &name) {
    switch (type) {
        case GL_FLOAT_MAT4:
            return new UniformT<GL_FLOAT_MAT4>(loc, name);
        case GL_SAMPLER_2D:
            return new UniformT<GL_SAMPLER_2D>(loc, name);
        case GL_FLOAT:
            return new UniformT<GL_FLOAT>(loc, name);
        default:
            assert(false);
            break;
    }
    return nullptr;
}


template<>
void UniformT<GL_FLOAT_MAT4>::bind(const void *data) {
    glUniformMatrix4fv(m_Location, 1, GL_FALSE, (const float *) data);
}

template<>
void UniformT<GL_SAMPLER_2D>::bind(const void *data) {
    int * p = (int*)data;
    glActiveTexture(GL_TEXTURE0 + m_Auxiliary);
    glBindTexture(GL_TEXTURE_2D, *p);
    glUniform1i(m_Location, m_Auxiliary);
}

template <>
void UniformT<GL_FLOAT>::bind(const void *data) {
    glUniform1fv(m_Location, 1, (const float *)data);
}

} // namespace GLU
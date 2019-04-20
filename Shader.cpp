//
// Created by mm on 19-4-4.
//
#include <cassert>

#include "Shader.h"
#include "Attrib.h"
#include "Material.h"
#include "Uniforms.h"
#include "EBO.h"

using namespace std;

namespace GLU{


/********************************************************/
/********************************************************/

Shader::Shader(const std::string &vs, const std::string & fs) {
    create_from_source(vs, fs);
}

void Shader::create_from_source(const std::string&vfile, const std::string &ffile)
{
    unsigned vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    {
        const std::string & source = vfile;
        const char * p = source.c_str();
        glShaderSource(vertex_shader, 1, &p, nullptr);
        glCompileShader(vertex_shader);
        int status;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
        if (!status) {
            char log[512];
            int length;
            glGetShaderInfoLog(vertex_shader, 511, &length, log);
            log[511] = 0;
            std::cout << log<<std::endl;
            return;
        }
    }

    unsigned frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    {
        const std::string &source = ffile;
        const char * p = source.c_str();
        glShaderSource(frag_shader, 1, &p, nullptr);
        glCompileShader(frag_shader);
        int status;
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);
        if (!status) {
            char log[512];
            int length;
            glGetShaderInfoLog(frag_shader, 511, &length, log);
            log[511] = 0;
            std::cout << log<<std::endl;
            return;
        }
    }

    m_Program = glCreateProgram();
    glAttachShader(m_Program, vertex_shader);
    glAttachShader(m_Program, frag_shader);
    glLinkProgram(m_Program);
    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    get_active_attribs();
    get_active_uniforms();
}

void Shader::get_active_attribs() {
    int num = 0;
    int length;
    char name[512];
    int size;
    GLenum  type;
    glGetProgramiv(m_Program, GL_ACTIVE_ATTRIBUTES, &num);
    for (int i=0; i<num; ++i) {
        glGetActiveAttrib(m_Program, i, 512, &length, &size, &type, name);
        m_Attribs.push_back(new Attrib(glGetAttribLocation(m_Program, name),
                Attrib::component_size(type),
                Attrib::component_type(type), name));
    }
}

void Shader::get_active_uniforms() {
    int num = 0;
    int length;
    char name[512];
    int size;
    GLenum  type;
    glGetProgramiv(m_Program, GL_ACTIVE_UNIFORMS, &num);
    int samplerNum = 0;
    for (int i= 0; i<num; ++i){
        glGetActiveUniform(m_Program, i, 512, &length, &size, &type, name);
        Uniform *uniform = Uniform::create(type, glGetUniformLocation(m_Program, name), name);
        if (is_sampler(type)) {
            uniform->m_Auxiliary = samplerNum;
            samplerNum++;
        }
        m_Uniforms.push_back(uniform);
    }
}

void Shader::clear() {
    for (auto &e: m_Attribs)
        delete e;
    m_Attribs.clear();
    for (auto &e:m_Uniforms) {
        delete e;
    }
    m_Uniforms.clear();
    glDeleteProgram(m_Program);
}

Shader::~Shader() {
    clear();
}

void Shader::DrawArrays(GLenum element, int first, int count, const Material *material) {
    glUseProgram(m_Program);
    for (Attrib* e: m_Attribs) {
        auto it = material->m_Attribs.find(e->m_Name);
        assert(it != material->m_Attribs.end() && it->second);
        e->bind(it->second);
    }

    for (Uniform* e : m_Uniforms) {
        auto it = material->m_Uniforms.find(e->m_Name);
        assert (it != material->m_Uniforms.end() && it->second.data);
        e->bind(it->second.data);
    }
    glDrawArrays(element, first,count);
    glUseProgram(0);
}

void Shader::DrawElements(GLenum element, int count, const GLU::EBO *ebo, const GLU::Material *material) {
    glUseProgram(m_Program);
    for (Attrib* e: m_Attribs) {
        auto it = material->m_Attribs.find(e->m_Name);
        assert(it != material->m_Attribs.end() && it->second);
        e->bind(it->second);
    }

    for (Uniform* e : m_Uniforms) {
        auto it = material->m_Uniforms.find(e->m_Name);
        assert (it != material->m_Uniforms.end() && it->second.data);
        e->bind(it->second.data);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->m_Handle);
    glDrawElements(element, count, GL_UNSIGNED_INT, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void Shader::DrawElements(GLenum element, int count, const void* indices, const GLU::Material *material) {
    glUseProgram(m_Program);
    for (Attrib* e: m_Attribs) {
        auto it = material->m_Attribs.find(e->m_Name);
        assert(it != material->m_Attribs.end() && it->second);
        e->bind(it->second);
    }

    for (Uniform* e : m_Uniforms) {
        auto it = material->m_Uniforms.find(e->m_Name);
        assert (it != material->m_Uniforms.end() && it->second.data);
        e->bind(it->second.data);
    }
    glDrawElements(element, count, GL_UNSIGNED_INT, indices);
    glUseProgram(0);
}

bool Shader::is_sampler(int type) {
    return type == GL_SAMPLER_2D  ||
            type == GL_SAMPLER_1D ||
            type == GL_SAMPLER_3D ||
            type == GL_SAMPLER_CUBE;
}

}

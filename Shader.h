

#ifndef GLAB_SHADER_H
#define GLAB_SHADER_H

#include <string>
#include <map>
#include <iostream>
#include <list>
#include <GL/glew.h>


namespace GLU {

class Attrib;
class Material;
class Uniform;
class EBO;

class Shader {
public:
    Shader(const std::string & vs, const std::string & fs);
    ~Shader();
    void DrawArrays(GLenum element, int first, int count, const Material *material);
    void DrawElements(GLenum element, int count, const EBO * ebo, const Material *material);
    void DrawElements(GLenum element, int count, const void * indices, const Material *material);
private:
    void create_from_source(const std::string & vs, const std::string & fs);
    void get_active_attribs();
    void get_active_uniforms();
    void clear();
    static bool is_sampler(int type);
private:
    unsigned m_Program;
    std::list<Attrib*> m_Attribs;
    std::list<Uniform*> m_Uniforms;
};

}
#endif //GLAB_SHADER_H

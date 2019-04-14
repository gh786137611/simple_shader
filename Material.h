//
// Created by mm on 19-4-4.
//

#ifndef GLAB_MATERIAL_H
#define GLAB_MATERIAL_H

#include <iostream>
#include <map>
#include <string>
#include <cstring>

using namespace std;

namespace GLU {
class VBO;
class Shader;

class Material {
    friend Shader;
    struct Buffer{
        char * data;
        int length;
        Buffer(char *d, int len){
            data = d;
            length = len;
        }
    };
public:
    Material(){}
    void SetAttrib(const std::string &attrib, const VBO *vbo);
    void SetUniform(const string &uniform, const void *value, int len) {
        auto it = m_Uniforms.find(uniform);
        if (it != m_Uniforms.end()){
            Buffer & buffer = it->second;
            if (buffer.length >= len ){
                memcpy(buffer.data, value, len);
            }else{
                delete[] buffer.data;
                buffer.data = new char[len];
                memcpy(buffer.data, &value, len);
            }
        } else{
            char * data = new char[len];
            memcpy(data, value, len);
            m_Uniforms.insert(std::make_pair(uniform, Buffer(data, len)));
        }
    }
    ~Material();

private:
    Material(const Material &) = delete;
    Material &operator=(const Material&) = delete;
private:
    std::map<std::string, const VBO*> m_Attribs;
    std::map<std::string, Buffer> m_Uniforms;
};

}
#endif //GLAB_MATERIAL_H

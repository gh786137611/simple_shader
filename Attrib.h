//
// Created by mm on 19-4-4.
//

#ifndef GLAB_ATTRIB_H
#define GLAB_ATTRIB_H

#include <GL/glew.h>

#include <string>

namespace GLU {
class Shader;
class VBO;

class Attrib{
    friend class Shader;
private:
    Attrib(int location, int size, int type, const std::string &name);
    Attrib(const Attrib&) = delete;
    Attrib&operator = (const Attrib&) = delete;
    void bind(const VBO*vbo);

private:
    static int component_size(int type);
    static int component_type(int type);
private:
    int m_Location;
    int m_Size;
    int m_Type;
    std::string m_Name;
};

}
#endif //GLAB_ATTRIB_H

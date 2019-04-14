//
// Created by mm on 19-4-4.
//

#include "Material.h"
namespace GLU{
void Material::SetAttrib(const std::string &attrib, const GLU::VBO *vbo) {
    m_Attribs[attrib] = vbo;
}

Material::~Material() {
    m_Attribs.clear();

    for (auto & e: m_Uniforms){
        delete[] e.second.data;
    }
    m_Uniforms.clear();
}

}
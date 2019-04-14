//
// Created by mm on 19-4-4.
//

#ifndef GLAB_UNIFORMS_H
#define GLAB_UNIFORMS_H

#include <GL/glew.h>
#include <cstring>
#include <cassert>
#include <string>


namespace GLU {
class Shader;



class Uniform {
    friend Shader;

protected:
    virtual void  bind(const void *data){
        assert(false);
    }
    Uniform(){}
    static Uniform * create(int type, int loc, const std::string &name);
private:
    Uniform(const Uniform&) = delete;
    Uniform&operator=(const Uniform&) = delete;
protected:
    int m_Location;
    int m_Auxiliary;
    std::string m_Name;
};




}
#endif //GLAB_UNIFORMS_H

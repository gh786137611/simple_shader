//
// Created by mm on 19-4-4.
//

#ifndef GLAB_TEXTURE2D_H
#define GLAB_TEXTURE2D_H

#include <GL/glew.h>
#include <cassert>

namespace GLU{
class Shader;
class Texture2D {
public:
    Texture2D(){
        glGenTextures(1, &m_Handle);
        assert(m_Handle);

        glBindTexture(GL_TEXTURE_2D, m_Handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    ~Texture2D(){
        glDeleteTextures(1, &m_Handle);
        m_Handle = 0;
    }

    const unsigned & Handle()const {
        return m_Handle;
    }

    void SetData(int w, int h, const void *data) {
        glBindTexture(GL_TEXTURE_2D, m_Handle);
        glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
private:
    Texture2D(const Texture2D&) = delete;
    Texture2D&operator =(const Texture2D&) = delete;
private:
    unsigned m_Handle;
};

}
#endif //GLAB_TEXTURE2D_H

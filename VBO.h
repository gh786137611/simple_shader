//
// Created by mm on 19-4-4.
//

#ifndef GLAB_VBO_H
#define GLAB_VBO_H

#include <GL/glew.h>

namespace GLU {
class VBO {
public:
    VBO();
    ~VBO();
    void SetData(const void *data, int size);
    void Bind()const;

private:
    VBO(const VBO&) = delete;
    VBO&operator=(const VBO&) = delete;
private:
    unsigned m_Handle;
};

}
#endif //GLAB_VBO_H

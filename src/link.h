#ifndef LINK_H
#define LINK_H

#include <QOpenGLFunctions>

class Link{


public:
    Link(float ep_link, float r_link, float edge, float col_r, float col_g, float col_b);
    void createVertice(int index, float x, float y, float z);
    void createColor(int index, float col_r, float col_g, float col_b);
    GLfloat* getVertices();
    GLfloat* getColors();
    GLfloat vertices[20*3];
    GLfloat colors[20*3];

};

#endif // LINK_H

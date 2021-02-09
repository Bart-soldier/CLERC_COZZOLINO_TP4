#ifndef LINK_H
#define LINK_H

#include <QOpenGLFunctions>

class Link{


public:
    Link(float ep_link, float r_link, float edge, float col_r, float col_g, float col_b);
    void createVertice(int index, float x, float y, float z);
    void createSide(int number, int index, float x, float y, float z);
    void createColor(int index, float col_r, float col_g, float col_b);
    GLfloat* getVertices();
    GLfloat* getVertices(int numberFace);
    GLfloat* getColors();


    GLfloat vertices[20*3];

    GLfloat side1[4*3];
    GLfloat side2[4*3];
    GLfloat side3[4*3];
    GLfloat side4[4*3];
    GLfloat side5[4*3];
    GLfloat side6[4*3];
    GLfloat side7[4*3];
    GLfloat side8[4*3];

    GLfloat temp[20*3];
    GLfloat colors[20*3];

};

#endif // LINK_H

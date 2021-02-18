/**
  CLERC Billy, COZZOLINO Christine
  Programmation Graphique
  Projet
  28/02/2021
**/

#ifndef LINK_H
#define LINK_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <math.h>
#include <QDebug>
#include <QMatrix4x4>
#include <QVector3D>
#include <QColor>

class Link{

public:

    Link(float W, float H, float edge, float ep, float m_col_r, float m_col_g, float m_col_b);
    ~Link();
    void buildVertData(QVector<GLfloat> &data);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);
    void add(QVector3D o, QVector3D a, QVector3D b);

private:
    QOpenGLBuffer m_vbo;
    float W;
    float H;
    float edge;
    float ep;
    float m_col_r;
    float m_col_g;
    float m_col_b;
};

#endif // LINK_H

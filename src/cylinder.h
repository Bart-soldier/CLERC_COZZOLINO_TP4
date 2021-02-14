/**
  CLERC Billy, COZZOLINO Christine
  Programmation Graphique
  Projet
  28/02/2021
**/

#ifndef CYLINDER_H
#define CYLINDER_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <math.h>

class Cylinder
{
public:
    Cylinder(float ep_cyl, float r_cyl, int nb_fac, float col_r, float col_g, float col_b);
    ~Cylinder();
    void buildVertData(QVector<GLfloat> &data);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);

private:
    QOpenGLBuffer m_vbo;
    float m_ep_cyl;
    float m_r_cyl;
    int m_nb_fac;
    float m_col_r;
    float m_col_g;
    float m_col_b;
};

#endif // CYLINDER_H

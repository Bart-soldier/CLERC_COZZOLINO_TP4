/**
  CLERC Billy, COZZOLINO Christine
  Programmation Graphique
  Projet
  28/02/2021
**/

#ifndef GEAR_H
#define GEAR_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <math.h>

class Gear
{
public:
    Gear(float ep_roue, float r_roue, float h_dent, int nb_dent, float col_r, float col_g, float col_b);
    ~Gear();
    void buildVertData(QVector<GLfloat> &data);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);

private:
    QOpenGLBuffer m_vbo;
    float m_ep_roue;
    float m_r_roue;
    float m_h_dent;
    int m_nb_dent;
    float m_col_r;
    float m_col_g;
    float m_col_b;
};

#endif // GEAR_H

// CC-BY Edouard.Thiel@univ-amu.fr - 06/02/2021

#ifndef KITE_H
#define KITE_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class Kite
{
public:
    Kite(bool phong_shading = false);
    ~Kite();
    double radius();
    void buildVertData(QVector<GLfloat> &data);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);

private:
    bool m_phong_shading = false;
    QOpenGLBuffer m_vbo;
};

#endif // KITE_H

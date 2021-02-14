// CC-BY Edouard.Thiel@univ-amu.fr - 06/02/2021

#include "kite.h"
#include <QDebug>
#include <QMatrix4x4>
#include <QVector3D>
#include <QColor>

// Cette classe doit être instanciée lorsqu'il y a un contexte GL courant,
// par exemple dans GLArea::makeGLObject.

Kite::Kite(bool phong_shading)
{
    m_phong_shading = phong_shading;

    m_vbo.create();
    m_vbo.bind();
    QVector<GLfloat> vertData;
    buildVertData(vertData);
    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    m_vbo.release();
}


Kite::~Kite()
{
    m_vbo.destroy();
}


double Kite::radius()
{
    // on renvoie ce qu'on veut, ça servira pour les placement dans GLArea::paintGL.
    return 0.8;
}


void Kite::buildVertData(QVector<GLfloat> &data)
{
    QVector<QVector3D> vs = {       // vertices
        QVector3D( 0.2,  0.5,  0.2),     // 0 A
        QVector3D( 0.1, -0.6,  0.1),     // 1 B
        QVector3D(-0.8,  0.1, -0.3),     // 2 C
        QVector3D( 0.8, -0.1, -0.1)      // 3 D
    };
                   // A  C  B  A  B  D
    int ind_ver[] = { 0, 2, 1, 0, 1, 3 };

    QVector<QColor> colors = {
        QColor::fromRgbF(1.0, 0.6, 0.6),     // triangle 0
        QColor::fromRgbF(0.7, 1.0, 0.5)      // triangle 1
    };
                   // A  C  B  A  B  D
    int ind_col[] = { 0, 0, 0, 1, 1, 1 };

    QVector3D vAB = vs[1]-vs[0],
              vAC = vs[2]-vs[0],
              vAD = vs[3]-vs[0];

    // normal() calcule la normale normalisée
    QVector3D nACB = QVector3D::normal(vAC, vAB);
    QVector3D nABD = QVector3D::normal(vAB, vAD);

    QVector<QVector3D> normals;
    QVector<int> ind_nor;

    if (m_phong_shading) {
        // normale interpolée
        QVector3D nAB = (nACB + nABD) / 2.0;
        normals = { nACB, nABD, nAB };
                 // A  C  B  A  B  D
        ind_nor = { 2, 0, 2, 2, 2, 1 };

    } else {
        normals = { nACB, nABD };
                 // A  C  B  A  B  D
        ind_nor = { 0, 0, 0, 1, 1, 1 };
    }

    for (int i = 0; i < 6; ++i) {
        // coordonnées sommets
        data.append(vs[ind_ver[i]].x());
        data.append(vs[ind_ver[i]].y());
        data.append(vs[ind_ver[i]].z());
        // couleurs sommets
        data.append(colors[ind_col[i]].redF());
        data.append(colors[ind_col[i]].greenF());
        data.append(colors[ind_col[i]].blueF());
        // normales sommets
        data.append(normals[ind_nor[i]].x());
        data.append(normals[ind_nor[i]].y());
        data.append(normals[ind_nor[i]].z());
    }
}


void Kite::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs)
{
    m_vbo.bind();

    program->setAttributeBuffer("posAttr",
        GL_FLOAT, 0 * sizeof(GLfloat), 3, 9 * sizeof(GLfloat));
    program->setAttributeBuffer("colAttr",
        GL_FLOAT, 3 * sizeof(GLfloat), 3, 9 * sizeof(GLfloat));
    program->setAttributeBuffer("norAttr",
        GL_FLOAT, 6 * sizeof(GLfloat), 3, 9 * sizeof(GLfloat));
    program->enableAttributeArray("posAttr");
    program->enableAttributeArray("colAttr");
    program->enableAttributeArray("norAttr");

    // Pour des questions de portabilité, hors de la classe GLArea, tous les appels
    // aux fonctions glBidule doivent être préfixés par glFuncs->.
    glFuncs->glDrawArrays(GL_TRIANGLES, 0, 6);

    program->disableAttributeArray("posAttr");
    program->disableAttributeArray("colAttr");
    program->disableAttributeArray("norAttr");

    m_vbo.release();
}

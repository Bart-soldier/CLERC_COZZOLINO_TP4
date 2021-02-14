/**
  CLERC Billy, COZZOLINO Christine
  Programmation Graphique
  Projet
  28/02/2021
**/

#include "cylinder.h"
#include <QDebug>
#include <QMatrix4x4>
#include <QVector3D>
#include <QColor>

Cylinder::Cylinder(float ep_cyl, float r_cyl, int nb_fac, float col_r, float col_g, float col_b)
{
    m_ep_cyl = ep_cyl;
    m_r_cyl = r_cyl;
    m_nb_fac = nb_fac;
    m_col_r = col_r;
    m_col_g = col_g;
    m_col_b = col_b;

    m_vbo.create();
    m_vbo.bind();
    QVector<GLfloat> vertData;
    buildVertData(vertData);
    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    m_vbo.release();
}


Cylinder::~Cylinder()
{
    m_vbo.destroy();
}

void Cylinder::buildVertData(QVector<GLfloat> &data)
{
    float alpha = 360/m_nb_fac;

    // On a nb_fac triangles par face, 3 sommets par triangle, deux faces pour un cylindre
    // On a quatre sommets par quadrilatère et nb_fac quadrilatère pour le côté
    int nb_vertices = m_nb_fac * 3 * 2 + m_nb_fac * 4;

    QVector<QVector3D> vertices; // Sommets
    QVector<QColor> colors = { // Couleurs
        QColor::fromRgbF(m_col_r, m_col_g, m_col_b),                  // Faces
        QColor::fromRgbF(0.8 * m_col_r, 0.8 * m_col_g, 0.8 * m_col_b) // Côté
    };
    QVector<QVector3D> normals; // Normales

    int vertices_indices[nb_vertices]; // Index des sommets
    int colors_indices[nb_vertices]; // Index des couleurs
    int normals_indices[nb_vertices]; // Index des normales
    int table_index = 0; // Index du prochain sommet à ajouter (pour la construction des formes)
    int vertex_index = -1; // Index du dernier sommet ajouté (unicité de chaque sommet)
    int normal_index = -1; // Index pour le tableau des normales

    QVector3D vOA; // Premier vecteur
    QVector3D vOB; // Second vecteur
    QVector3D nOAB; // Normale des deux vecteurs

    /**
     * Face éloigné
     **/

    // Origine de la face
    vertices.append(QVector3D(0, 0, -m_ep_cyl/2));
    vertex_index++;
    // Premier sommet de la face
    vertices.append(QVector3D(cos(0 * (3.14/180)) * m_r_cyl, sin(0 * (3.14/180)) * m_r_cyl, -m_ep_cyl/2));
    vertex_index++;

    // On fait le tour de la face éloignée du cylindre
    for(int angle = alpha; angle < 360; angle += alpha) {
        vertices.append(QVector3D(cos(angle * (3.14/180)) * m_r_cyl, sin(angle * (3.14/180)) * m_r_cyl, -m_ep_cyl/2));
        vertex_index++;

        // Normale
        vOA = vertices[vertex_index - 1] - vertices[0];
        vOB = vertices[vertex_index] - vertices[0];
        nOAB = QVector3D::normal(vOA, vOB);

        // On ajoute la normal à la liste des normales
        normals.append(nOAB);
        normal_index++;

        // O
        vertices_indices[table_index] = 0;
        colors_indices[table_index] = 0;
        normals_indices[table_index] = normal_index;
        table_index++;
        // A
        vertices_indices[table_index] = vertex_index - 1;
        colors_indices[table_index] = 0;
        normals_indices[table_index] = normal_index;
        table_index++;
        // B
        vertices_indices[table_index] = vertex_index;
        colors_indices[table_index] = 0;
        normals_indices[table_index] = normal_index;
        table_index++;
    }

    // Dernier triangle de la face
    // Normale
    vOA = vertices[vertex_index] - vertices[0];
    vOB = vertices[1] - vertices[0];
    nOAB = QVector3D::normal(vOA, vOB);

    // On ajoute la normal à la liste des normales
    normals.append(nOAB);
    normal_index++;

    // O
    vertices_indices[table_index] = 0;
    colors_indices[table_index] = 0;
    normals_indices[table_index] = normal_index;
    table_index++;
    // A
    vertices_indices[table_index] = vertex_index;
    colors_indices[table_index] = 0;
    normals_indices[table_index] = normal_index;
    table_index++;
    // B
    vertices_indices[table_index] = 1;
    colors_indices[table_index] = 0;
    normals_indices[table_index] = normal_index;
    table_index++;

    /**
     * Face proche
     **/

    // Origine de la face
    vertices.append(QVector3D(0, 0, m_ep_cyl/2));
    vertex_index++;
    // Premier sommet de la face
    vertices.append(QVector3D(cos(0 * (3.14/180)) * m_r_cyl, sin(0 * (3.14/180)) * m_r_cyl, m_ep_cyl/2));
    vertex_index++;

    // On fait le tour de la face éloignée du cylindre
    for(int angle = alpha; angle < 360; angle += alpha) {
        vertices.append(QVector3D(cos(angle * (3.14/180)) * m_r_cyl, sin(angle * (3.14/180)) * m_r_cyl, m_ep_cyl/2));
        vertex_index++;

        // Normale
        vOA = vertices[vertex_index - 1] - vertices[m_nb_fac + 1];
        vOB = vertices[vertex_index] - vertices[m_nb_fac + 1];
        nOAB = QVector3D::normal(vOA, vOB);

        // On ajoute la normal à la liste des normales
        normals.append(nOAB);
        normal_index++;

        // O
        vertices_indices[table_index] = m_nb_fac + 1;
        colors_indices[table_index] = 0;
        normals_indices[table_index] = normal_index;
        table_index++;
        // A
        vertices_indices[table_index] = vertex_index - 1;
        colors_indices[table_index] = 0;
        normals_indices[table_index] = normal_index;
        table_index++;
        // B
        vertices_indices[table_index] = vertex_index;
        colors_indices[table_index] = 0;
        normals_indices[table_index] = normal_index;
        table_index++;
    }

    // Dernier triangle de la face
    // Normale
    vOA = vertices[vertex_index] - vertices[m_nb_fac + 1];
    vOB = vertices[m_nb_fac + 2] - vertices[m_nb_fac + 1];
    nOAB = QVector3D::normal(vOA, vOB);

    // On ajoute la normal à la liste des normales
    normals.append(nOAB);
    normal_index++;

    // O
    vertices_indices[table_index] = m_nb_fac + 1;
    colors_indices[table_index] = 0;
    normals_indices[table_index] = normal_index;
    table_index++;
    // A
    vertices_indices[table_index] = vertex_index;
    colors_indices[table_index] = 0;
    normals_indices[table_index] = normal_index;
    table_index++;
    // B
    vertices_indices[table_index] = m_nb_fac + 2;
    colors_indices[table_index] = 0;
    normals_indices[table_index] = normal_index;
    table_index++;

    /**
     * Côté
     **/

    // Chaque quadrilatère
    for(int face_index = 1; face_index <= m_nb_fac; face_index++) {
        int second_face_index = face_index + 1;
        if(face_index == m_nb_fac) {
            second_face_index = 1;
        }

        // Normale
        vOA = vertices[second_face_index] - vertices[face_index];
        vOB = vertices[m_nb_fac + 1 + face_index] - vertices[face_index];
        nOAB = QVector3D::normal(vOA, vOB);

        // On ajoute la normal à la liste des normales
        normals.append(nOAB);
        normal_index++;

        // Est composé de deux sommets de la face éloignée
        // A
        vertices_indices[table_index] = face_index;
        colors_indices[table_index] = 1;
        normals_indices[table_index] = normal_index;
        table_index++;
        // B
        vertices_indices[table_index] = second_face_index;
        colors_indices[table_index] = 1;
        normals_indices[table_index] = normal_index;
        table_index++;

        // Et de deux sommets de la face proche
        // B'
        vertices_indices[table_index] = m_nb_fac + 1 + second_face_index;
        colors_indices[table_index] = 1;
        normals_indices[table_index] = normal_index;
        table_index++;
        // A'
        vertices_indices[table_index] = m_nb_fac + 1 + face_index;
        colors_indices[table_index] = 1;
        normals_indices[table_index] = normal_index;
        table_index++;
    }

    // Pour chaque sommet
    for(int i = 0; i < nb_vertices; i++) {
        // Coordonnées du sommet
        data.append(vertices[vertices_indices[i]].x());
        data.append(vertices[vertices_indices[i]].y());
        data.append(vertices[vertices_indices[i]].z());
        // Couleur du sommet
        data.append(colors[colors_indices[i]].redF());
        data.append(colors[colors_indices[i]].greenF());
        data.append(colors[colors_indices[i]].blueF());
        // Normales du sommet
        data.append(normals[normals_indices[i]].x());
        data.append(normals[normals_indices[i]].y());
        data.append(normals[normals_indices[i]].z());
    }
}


void Cylinder::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs)
{
    m_vbo.bind();

    program->setAttributeBuffer("posAttr", GL_FLOAT, 0 * sizeof(GLfloat), 3, 9 * sizeof(GLfloat));
    program->setAttributeBuffer("colAttr", GL_FLOAT, 3 * sizeof(GLfloat), 3, 9 * sizeof(GLfloat));
    program->setAttributeBuffer("norAttr", GL_FLOAT, 6 * sizeof(GLfloat), 3, 9 * sizeof(GLfloat));
    program->enableAttributeArray("posAttr");
    program->enableAttributeArray("colAttr");
    program->enableAttributeArray("norAttr");

    // Pour des questions de portabilité, hors de la classe GLArea, tous les appels
    // aux fonctions glBidule doivent être préfixés par glFuncs->.
    // Face éloignée
    glFuncs->glDrawArrays(GL_TRIANGLES, 0, 3 * m_nb_fac);
    // Face proche
    glFuncs->glDrawArrays(GL_TRIANGLES, 3 * m_nb_fac, 3 * m_nb_fac);
    // Côté
    glFuncs->glDrawArrays(GL_QUADS, 3 * 2 * m_nb_fac, 4 * m_nb_fac);

    program->disableAttributeArray("posAttr");
    program->disableAttributeArray("colAttr");
    program->disableAttributeArray("norAttr");

    m_vbo.release();
}

#include "link.h"

Link::Link(float W, float H, float edge, float ep, float m_col_r, float m_col_g, float m_col_b){
    this->W = W;
    this->H = H;
    this->edge = edge;
    this->ep = ep;
    this->m_col_r = m_col_r;
    this->m_col_g = m_col_g;
    this->m_col_b = m_col_b;

    m_vbo.create();
    m_vbo.bind();
    QVector<GLfloat> vertData;
    buildVertData(vertData);
    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    m_vbo.release();
}

Link::~Link(){ m_vbo.destroy(); }

void Link::add(QVector3D o, QVector3D a, QVector3D b){


}

void Link::buildVertData(QVector<GLfloat> &data){
    //int nb_vertices = 18;
    int nb_vertices = 3*8;

    // points (x, y) sans l'origine
    //float p[9][2] = { {W/2, H/2}, {0, H - edge/2}, {edge/2, H}, {W - edge/2, H}, {W, H-edge/2}, {W, edge/2}, {W - edge/2, 0}, {edge/2, 0}, {0, edge/2} };

    float p[8][2] = { {0, H - edge/2}, {edge/2, H}, {W - edge/2, H}, {W, H-edge/2}, {W, edge/2}, {W - edge/2, 0}, {edge/2, 0}, {0, edge/2} };

    QVector<QVector3D> vertices; // Sommets
    QVector<QColor> colors = { // Couleurs
        QColor::fromRgbF(m_col_r, m_col_g, m_col_b),                  // Faces
        //QColor::fromRgbF(0.8 * m_col_r, 0.8 * m_col_g, 0.8 * m_col_b) // Côté
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

    float currZ = -this->ep;

    // Origine
    vertices.append(QVector3D(W/2, H/2, currZ));
    vertex_index++;


    // face 1
    for (int i=0; i<8 ; i++){

        vertices.append(QVector3D(p[i][0], p[i][1], currZ));
        vertex_index++;

        if (i>=1){
            // On calcule la normale
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


void Link::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs){

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
    glFuncs->glDrawArrays(GL_TRIANGLES, 0, 3*8);

    program->disableAttributeArray("posAttr");
    program->disableAttributeArray("colAttr");
    program->disableAttributeArray("norAttr");

    m_vbo.release();

}

/*
// Origine de la face
// O
vertices.append(QVector3D(W/2, H/2, currZ));
vertex_index++;

// Premier sommet de la face
// A
vertices.append(QVector3D(0, H - edge/2, currZ));
vertex_index++;

// 2em sommet de la face
// B
vertices.append(QVector3D(edge/2, H, currZ));
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

*/

/**
  CLERC Billy, COZZOLINO Christine
  Programmation Graphique
  TP4
  04/02/2021
**/

#include "glarea.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include <math.h>

static const QString vertexShaderFile   = ":/basic.vsh";
static const QString fragmentShaderFile = ":/basic.fsh";


GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    qDebug() << "init GLArea" ;

    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);  // nb de sample par pixels : suréchantillonnage por l'antialiasing, en décalant à chaque fois le sommet
                        // cf https://www.khronos.org/opengl/wiki/Multisampling et https://stackoverflow.com/a/14474260
    setFormat(sf);
    qDebug() << "Depth is"<< format().depthBufferSize();

    setEnabled(true);  // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus); // accepte focus
    setFocus();                      // donne le focus

    m_timer = new QTimer(this);
    m_timer->setInterval(20);  // msec
    connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect (this, SIGNAL(radiusChanged(double)), this, SLOT(setRadius(double)));
    connect (this, SIGNAL(frustumNearChanged(double)), this, SLOT(setFrustumNear(double)));
    connect (this, SIGNAL(frustumFarChanged(double)), this, SLOT(setFrustumFar(double)));
    connect (this, SIGNAL(cameraDistanceChanged(double)), this, SLOT(setCameraDistance(double)));
    connect (this, SIGNAL(cameraAngleChanged(double)), this, SLOT(setCameraAngle(double)));
}

GLArea::~GLArea()
{
    qDebug() << "destroy GLArea";

    delete m_timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();

    // ici destructions de ressources GL

    doneCurrent();
}


void GLArea::initializeGL()
{
    qDebug() << __FUNCTION__ ;
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    // shaders
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);  // compile
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);
    if (! m_program->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << m_program->log();
    }

    // récupère identifiants de "variables" dans les shaders
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
}

void GLArea::resizeGL(int w, int h)
{
    qDebug() << __FUNCTION__ << w << h;

    // C'est fait par défaut
    glViewport(0, 0, w, h);

    m_ratio = (double) w / h;
    // doProjection();
}

void GLArea::paintGL()
{
    qDebug() << __FUNCTION__ ;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind(); // active le shader program

    QMatrix4x4 matrix;
    GLfloat hr = m_radius, wr = hr * m_ratio;            // = glFrustum
    matrix.frustum(-wr, wr, -hr, hr, m_frustumNear, m_frustumFar);
    //matrix.ortho(-wr, wr, -hr, hr, 1.0, 5.0);
    //matrix.perspective(60.0f, m_ratio, 0.1f, 100.0f);  // = gluPerspective

    // Remplace gluLookAt (0, 0, 3.0, 0, 0, 0, 0, 1, 0);
    matrix.translate(3, 0, m_cameraDistance);
    matrix.rotate(m_cameraAngle, 0, 1, 0);

    //paintTP3(matrix);

    m_program->release();
}

void GLArea::keyPressEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
        case Qt::Key_Space :
            m_angle += 1;
            if (m_angle >= 360) m_angle -= 360;
            update();
            break;
        case Qt::Key_I :
            if (m_timer->isActive())
                m_timer->stop();
            else m_timer->start();
            break;
        case Qt::Key_Z :
            if (ev->text() == "z")
                 setCameraDistance(m_cameraDistance-0.05);
            else setCameraDistance(m_cameraDistance+0.05);
            break;
        case Qt::Key_R :
            if (ev->text() == "r")
                 setRadius(m_radius-0.05);
            else setRadius(m_radius+0.05);
            break;
        case Qt::Key_N :
            if (ev->text() == "n")
                 setFrustumNear(m_frustumNear-0.05);
            else setFrustumNear(m_frustumNear+0.05);
            break;
        case Qt::Key_F :
            if (ev->text() == "f")
                 setFrustumFar(m_frustumFar-0.05);
            else setFrustumFar(m_frustumFar+0.05);
            break;
        case Qt::Key_A :
            if (ev->text() == "a")
                 setCameraAngle(m_cameraAngle-1);
            else setCameraAngle(m_cameraAngle+1);
            break;
    }
}

void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}

void GLArea::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y();
}

void GLArea::onTimeout()
{
    qDebug() << __FUNCTION__ ;
    m_angle += 1;
    if (m_angle >= 360) m_angle -= 360;
    update();
}

void GLArea::setRadius(double radius)
{
    qDebug() << __FUNCTION__ << radius << sender();
    if (radius != m_radius && radius > 0.01 && radius <= 10) {
        m_radius = radius;
        qDebug() << "  emit radiusChanged()";
        emit radiusChanged(radius);
        update();
    }
}

void GLArea::setFrustumNear(double frustumNear)
{
    qDebug() << __FUNCTION__ << frustumNear << sender();
    if (frustumNear != m_frustumNear && frustumNear > 0.01 && frustumNear <= 10) {
        m_frustumNear = frustumNear;
        qDebug() << "  emit frustumNearChanged()";
        emit frustumNearChanged(frustumNear);
        update();
    }
}

void GLArea::setFrustumFar(double frustumFar)
{
    qDebug() << __FUNCTION__ << frustumFar << sender();
    if (frustumFar != m_frustumFar && frustumFar > 0.01 && frustumFar <= 10) {
        m_frustumFar = frustumFar;
        qDebug() << "  emit frustumFarChanged()";
        emit frustumFarChanged(frustumFar);
        update();
    }
}

void GLArea::setCameraDistance(double cameraDistance)
{
    qDebug() << __FUNCTION__ << cameraDistance << sender();
    if (cameraDistance != m_cameraDistance && cameraDistance >= -20 && cameraDistance <= 20) {
        m_cameraDistance = cameraDistance;
        qDebug() << "  emit cameraDistanceChanged()";
        emit cameraDistanceChanged(cameraDistance);
        update();
    }
}

void GLArea::setCameraAngle(double cameraAngle)
{
    qDebug() << __FUNCTION__ << cameraAngle << sender();
    if (cameraAngle != m_cameraAngle) {
        if(cameraAngle == -1) cameraAngle = 359;
        if(cameraAngle == 360) cameraAngle = 0;
        m_cameraAngle = cameraAngle;
        qDebug() << "  emit cameraAngleChanged()";
        emit cameraAngleChanged(cameraAngle);
        update();
    }
}

/**
 * @brief GLArea::paintTP3 Créer le piston et la bielle telle que décrite dans le TP3
 * @param matrix La matrice de transformation initiale
 */
void GLArea::paintTP3(QMatrix4x4 matrix)
{
    float ep_cyl = 1;
    float r_cyl = 1.5;
    int nb_fac = 20;

    // O
    QMatrix4x4 matrixCopy_1 = matrix; // Push
    matrixCopy_1.rotate(-m_angle, 0, 0, 1);
    m_program->setUniformValue(m_matrixUniform, matrixCopy_1); // On applique la matrice
    paintCylinder(ep_cyl, r_cyl, nb_fac, 0, 0, 1);

    matrixCopy_1.translate(0, 0, -ep_cyl/2);
    paintCylinder(ep_cyl*2.1, r_cyl/10, nb_fac, 0, 0, 0.5);
    m_program->setUniformValue(m_matrixUniform, matrix); // Pop

    // H
    float gh = 2 * r_cyl / 3;
    float xH = -gh * cos(m_angle * 3.14/180);
    float yH = gh * sin(m_angle * 3.14/180);

    matrixCopy_1 = matrix; // Push
    matrixCopy_1.translate(xH, yH, ep_cyl/2);
    m_program->setUniformValue(m_matrixUniform, matrixCopy_1); // On applique la matrice
    paintCylinder(ep_cyl*2.5, r_cyl/10, nb_fac, 0, 0.5, 0);

    matrixCopy_1.translate(0, 0, ep_cyl/2);
    paintCylinder(ep_cyl, r_cyl/4, nb_fac, 0, 1, 0);
    m_program->setUniformValue(m_matrixUniform, matrix); // Pop

    // J
    float xJ = xH - 1.5*r_cyl;

    matrixCopy_1 = matrix; // Push
    matrixCopy_1.translate(xJ, 0, ep_cyl);
    m_program->setUniformValue(m_matrixUniform, matrixCopy_1); // On applique la matrice
    paintCylinder(ep_cyl, r_cyl/3, nb_fac, 0, 1, 0);

    matrixCopy_1.translate(0, 0, ep_cyl/2);
    m_program->setUniformValue(m_matrixUniform, matrixCopy_1); // On applique la matrice
    paintCylinder(ep_cyl*2.5, r_cyl/10, nb_fac, 0, 0.5, 0);

    matrixCopy_1.translate(0, 0, ep_cyl/2);
    m_program->setUniformValue(m_matrixUniform, matrixCopy_1); // On applique la matrice
    paintCylinder(ep_cyl, r_cyl/4, nb_fac, 1, 0, 1);
    m_program->setUniformValue(m_matrixUniform, matrix); // Pop

    // JH

    matrixCopy_1 = matrix; // Push
    matrixCopy_1.translate(xH + (xJ - xH)/2, yH/2, ep_cyl);
    matrixCopy_1.rotate(90, 0, 1, 0);
    matrixCopy_1.rotate(-atan(yH/abs(xJ - xH)) * 180/3.14, 1, 0, 0);
    m_program->setUniformValue(m_matrixUniform, matrixCopy_1); // On applique la matrice
    paintCylinder(xJ - xH, r_cyl/6, nb_fac, 0, 1, 0);
    m_program->setUniformValue(m_matrixUniform, matrix); // Pop

    // K
    float xK = -4*r_cyl;

    // Piston
    matrixCopy_1 = matrix; // Push
    matrixCopy_1.translate(xJ + ((xK - xJ)/2), 0, 2*ep_cyl);
    matrixCopy_1.rotate(90, 0, 1, 0);
    m_program->setUniformValue(m_matrixUniform, matrixCopy_1); // On applique la matrice
    paintCylinder(xK - xJ, r_cyl/6, nb_fac, 1, 0, 1);
    m_program->setUniformValue(m_matrixUniform, matrix); // Pop

    // KJ
    matrixCopy_1 = matrix; // Push
    matrixCopy_1.translate(xK - r_cyl/2, 0, 2*ep_cyl);
    matrixCopy_1.rotate(90, 0, 1, 0);
    m_program->setUniformValue(m_matrixUniform, matrixCopy_1); // On applique la matrice
    paintCylinder(3*ep_cyl, r_cyl/2, nb_fac, 1, 0.5, 0);
    m_program->setUniformValue(m_matrixUniform, matrix); // Pop
}

/**
 * @brief GLArea::paintCylinder Dessine un cylindre
 * @param ep_cyl L'épaisseur du cylindre
 * @param r_cyl Le rayon de la face du cylinder
 * @param nb_fac Le nombre de parties constituant la face
 * @param col_r La valeur R de la couleur en RGB
 * @param col_g La valeur G de la couleur en RGB
 * @param col_b La valeur B de la couleur en RGB
 */
void GLArea::paintCylinder(float ep_cyl, float r_cyl, int nb_fac, float col_r, float col_g, float col_b)
{
    float alpha = 360/nb_fac;

    // On a deux faces par cylindre et nb_fac quadrilatères pour le côté
    // On a nb_fac sommets par face
    // On a 4 sommets par quadrilatère pour le côté
    // On a trois coordonnée par sommet
    int nb_vertices = 2 * nb_fac * 3 + nb_fac * 4 * 3;

    // On initialise le tableau des sommets
    GLfloat vertices[nb_vertices];


    // On intialise le tableau des couleurs
    GLfloat colors[nb_vertices];

    int index = 0; // Index des tableaux

    // On fait le tour du cylindre
    for(int angle = 0; angle < 360; angle += alpha) {
        vertices[3 * index] = static_cast<GLfloat>(cos(angle * (3.14/180)) * r_cyl);
        vertices[3 * index + 1] = static_cast<GLfloat>(sin(angle * (3.14/180)) * r_cyl);
        vertices[3 * index + 2] = -ep_cyl/2;
        colors[3 * index] = col_r;
        colors[3 * index + 1] = col_g;
        colors[3 * index + 2] = col_b;
        index++;
    }

    int secondFaceIndex = index; // Index du premier élément pour le dessin de la seconde face

    // On fait le tour du cylindre
    for(int angle = 0; angle < 360; angle += alpha) {
        vertices[3 * index] = static_cast<GLfloat>(cos(angle * (3.14/180)) * r_cyl);
        vertices[3 * index + 1] = static_cast<GLfloat>(sin(angle * (3.14/180)) * r_cyl);
        vertices[3 * index + 2] = ep_cyl/2;
        colors[3 * index] = col_r;
        colors[3 * index + 1] = col_g;
        colors[3 * index + 2] = col_b;
        index++;
    }

    int sideIndex = index; // Index du premier élément pour le dessin du côté

    // On fait le tour du cylindre
    for(int angle = 0; angle < 360; angle += alpha) {
        vertices[3 * index] = static_cast<GLfloat>(cos(angle * (3.14/180)) * r_cyl);
        vertices[3 * index + 1] = static_cast<GLfloat>(sin(angle * (3.14/180)) * r_cyl);
        vertices[3 * index + 2] = ep_cyl/2;
        colors[3 * index] = 0.8 * col_r;
        colors[3 * index + 1] = 0.8 * col_g;
        colors[3 * index + 2] = 0.8 * col_b;
        index++;

        vertices[3 * index] = static_cast<GLfloat>(cos(angle * (3.14/180)) * r_cyl);
        vertices[3 * index + 1] = static_cast<GLfloat>(sin(angle * (3.14/180)) * r_cyl);
        vertices[3 * index + 2] = -ep_cyl/2;
        colors[3 * index] = 0.8 * col_r;
        colors[3 * index + 1] = 0.8 * col_g;
        colors[3 * index + 2] = 0.8 * col_b;
        index++;

        vertices[3 * index] = static_cast<GLfloat>(cos((angle + alpha) * (3.14/180)) * r_cyl);
        vertices[3 * index + 1] = static_cast<GLfloat>(sin((angle + alpha) * (3.14/180)) * r_cyl);
        vertices[3 * index + 2] = -ep_cyl/2;
        colors[3 * index] = 0.8 * col_r;
        colors[3 * index + 1] = 0.8 * col_g;
        colors[3 * index + 2] = 0.8 * col_b;
        index++;

        vertices[3 * index] = static_cast<GLfloat>(cos((angle + alpha) * (3.14/180)) * r_cyl);
        vertices[3 * index + 1] = static_cast<GLfloat>(sin((angle + alpha) * (3.14/180)) * r_cyl);
        vertices[3 * index + 2] = ep_cyl/2;
        colors[3 * index] = 0.8 * col_r;
        colors[3 * index + 1] = 0.8 * col_g;
        colors[3 * index + 2] = 0.8 * col_b;
        index++;
    }

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(m_posAttr);  // rend le VAA accessible pour glDrawArrays
    glEnableVertexAttribArray(m_colAttr);

    glDrawArrays(GL_POLYGON, 0, nb_fac); // Première face
    glDrawArrays(GL_POLYGON, secondFaceIndex, nb_fac); // Deuxième face
    // Côté
    for(int i = 0; i < nb_fac; i++) {
        glDrawArrays(GL_QUADS, sideIndex + i * 4, 4);
    }

    glDisableVertexAttribArray(m_posAttr);
    glDisableVertexAttribArray(m_colAttr);
}





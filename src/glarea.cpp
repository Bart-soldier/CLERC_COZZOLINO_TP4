/**
  CLERC Billy, COZZOLINO Christine
  Programmation Graphique
  Projet
  28/02/2021
**/

#include "glarea.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include <QVector3D>
#include <math.h>

static const QString vertexShaderFile   = ":/vertex.glsl";
static const QString fragmentShaderFile = ":/fragment.glsl";


GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    //qDebug() << "init GLArea" ;

    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);  // suréchantillonnage pour l'antialiasing
    setFormat(sf);
    //qDebug() << "Depth is"<< format().depthBufferSize();

    setEnabled(true);  // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus); // accepte focus
    setFocus();                      // donne le focus

    // Animation timer
    m_timer = new QTimer(this);
    m_timer->setInterval(20);  // msec

    // CONNECT
    connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect (this, SIGNAL(radiusChanged(double)), this, SLOT(setRadius(double)));
    connect (this, SIGNAL(frustumNearChanged(double)), this, SLOT(setFrustumNear(double)));
    connect (this, SIGNAL(frustumFarChanged(double)), this, SLOT(setFrustumFar(double)));
    connect (this, SIGNAL(cameraDistanceChanged(double)), this, SLOT(setCameraDistance(double)));
    connect (this, SIGNAL(cameraAngleChanged(double)), this, SLOT(setCameraAngle(double)));
}


GLArea::~GLArea()
{
    //qDebug() << "destroy GLArea";

    delete m_timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();
    tearGLObjects();
    doneCurrent();
}


void GLArea::initializeGL()
{
    //qDebug() << __FUNCTION__ ;

    // Initialise le wrapper des fonctions OpenGL pour des questions de portabilité.
    // Les fontions glBidule pourront être directement appelées dans cette classe
    // car elle hérite de QOpenGLFunctions. Par contre pour les autres classes qui
    // font du OpenGL, il faudra appeler les fonctions glBidule en faisant :
    //     glFuncs->glBidule(...);
    // en leur transmettant glFuncs, récupéré par (cf paintGL) :
    //     QOpenGLFunctions *glFuncs = context()->functions();
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    makeGLObjects();

    // shaders
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);  // compile
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);
    if (! m_program->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << m_program->log();
    }
}


void GLArea::makeGLObjects()
{
    // Initialisation des propriétés des éléments graphique
    ep_roue = 0.5;
    r_roue = 1.5;
    h_dent = 0.3;
    nb_dent = 20;
    alphaBig = 360/nb_dent;
    alphaSmall = alphaBig*2;

    ep_cyl = ep_roue;
    r_cyl = 0.15;
    nb_fac = 20;

    m_cylinder = new Cylinder(ep_cyl, r_cyl, nb_fac, 0, 0, 1);
    m_bigGear = new Gear(ep_roue, r_roue, h_dent, nb_dent, 0, 1, 0);
    m_smallGear = new Gear(ep_roue, r_roue/2, h_dent, nb_dent/2, 0, 1, 0);
}


void GLArea::tearGLObjects()
{
    delete m_cylinder;
    delete m_bigGear;
    delete m_smallGear;
}


void GLArea::resizeGL(int w, int h)
{
    //qDebug() << __FUNCTION__ << w << h;

    // C'est fait par défaut
    glViewport(0, 0, w, h);

    m_ratio = (double) w / h;
}


void GLArea::paintGL()
{
    //qDebug() << __FUNCTION__ ;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QOpenGLFunctions *glFuncs = context()->functions();  // cf initializeGL

    m_program->bind(); // active le shader program

    // Projection
    QMatrix4x4 proj_mat;
    GLfloat hr = m_radius, wr = hr * m_ratio;
    //proj_mat.frustum(-wr, wr, -hr, hr, m_frustumNear, m_frustumFar); // = glFrustum
    proj_mat.ortho(-wr, wr, -hr, hr, m_frustumNear, m_frustumFar);
    //proj_mat.perspective(60.0f, m_ratio, 0.1f, 100.0f);  // = gluPerspective
    m_program->setUniformValue("projMatrix", proj_mat);

    // Caméra
    QMatrix4x4 cam_mat;
    //cam_mat.translate(0, 0, -3.0);
    cam_mat.translate(3, 0, m_cameraDistance);

    // Orientation de la scène
    QMatrix4x4 world_mat;
    world_mat.rotate(m_cameraAngle, 0, 1, 0);

    // Initialisation des matrices
    QMatrix4x4 matrixCopy_1;
    QMatrix4x4 matrixCopy_2;

    // Dessin du gros engrenage à gauche
    matrixCopy_1 = world_mat;
    matrixCopy_1.translate(-2.5, 0, 0); // Translation en x
    matrixCopy_1.rotate(-alphaBig/8, 0, 0, 1); // Pour aligner avec les maillons
    matrixCopy_1.rotate(fmod(m_angle, alphaBig), 0, 0, 1); // Rotation axe z
    setTransforms(cam_mat, matrixCopy_1); // On applique la matrice
    m_bigGear->draw(m_program, glFuncs); // On dessine

    // Dessin du petit engrenage à droite
    matrixCopy_1 = world_mat;
    matrixCopy_1.translate(2.50, 0, 0); // Translation en x
    matrixCopy_1.rotate(-alphaSmall/8, 0, 0, 1); // Pour aligner avec les maillons
    matrixCopy_1.rotate(2 * fmod(m_angle, alphaSmall), 0, 0, 1); // Rotation axe z
    setTransforms(cam_mat, matrixCopy_1); // On applique la matrice
    m_smallGear->draw(m_program, glFuncs); // On dessine

    // Dessin des maillons
    matrixCopy_1 = world_mat;
    matrixCopy_1.translate(-2.5, 0, 0); // Translation au centre du gros engrenage

    // Maillons A à B (B exclu)
    for(int i = 4; i < 16; i++) {
        float xi = (r_roue + h_dent/4) * cos((fmod(m_angle, alphaBig) + i * alphaBig) * 3.14/180);
        float yi = (r_roue + h_dent/4) * sin((fmod(m_angle, alphaBig) + i * alphaBig) * 3.14/180);

        matrixCopy_2 = matrixCopy_1;
        matrixCopy_2.translate(xi, yi, 0);
        setTransforms(cam_mat, matrixCopy_2); // On applique la matrice
        m_cylinder->draw(m_program, glFuncs); // On dessine
    }

    float pas = 0.60/alphaBig; // Pour l'animation des maillons sur les tangentes

    // Trouvons l'équation de la tangeante au point B : y = ax + b
    // a
    float xB = (r_roue + 3 * h_dent/16) * cos((-4 * alphaBig - 17 * alphaBig/32) * 3.14/180);
    float yB = (r_roue + 3 * h_dent/16) * sin((-4 * alphaBig - 17 * alphaBig/32) * 3.14/180);
    float a = -xB/yB;
    // b
    float b = yB - a * xB;

    // Maillons B à C (C exclu)
    for(float i = 0.25; i <= 4.45; i+=0.60) {
        float xi = xB + i + fmod(m_angle, alphaBig) * pas;
        float yi = a * xi + b;

        matrixCopy_2 = matrixCopy_1;
        matrixCopy_2.translate(xi, yi, 0);
        setTransforms(cam_mat, matrixCopy_2); // On applique la matrice
        m_cylinder->draw(m_program, glFuncs); // On dessine
    }

    // Trouvons l'équation de la tangeante au point A : y = ax + b
    // a
    float xA = (r_roue + 3 * h_dent/16) * cos((4 * alphaBig + 17 * alphaBig/32) * 3.14/180);
    float yA = (r_roue + 3 * h_dent/16) * sin((4 * alphaBig + 17 * alphaBig/32) * 3.14/180);
    a = -xA/yA;
    // b
    b = yA - a * xA;

    // Maillons A à D (A exclu)
    for(float i = 0.85; i <= 5.05; i+=0.6) {
        float xi = xA + i - fmod(m_angle, alphaBig) * pas;
        float yi = a * xi + b;

        matrixCopy_2 = matrixCopy_1;
        matrixCopy_2.translate(xi, yi, 0);
        setTransforms(cam_mat, matrixCopy_2); // On applique la matrice
        m_cylinder->draw(m_program, glFuncs); // On dessine
    }

    matrixCopy_1 = world_mat; // Push
    matrixCopy_1.translate(2.50, 0, 0); // Translation au centre du gros engrenage

    // Maillons C à D (D exclu)
    for(int i = -2; i < 2; i++) {
        float xi = (r_roue/2 + h_dent/4) * cos((2 * fmod(m_angle, alphaBig) + i * alphaSmall) * 3.14/180);
        float yi = (r_roue/2 + h_dent/4) * sin((2 * fmod(m_angle, alphaBig) + i * alphaSmall) * 3.14/180);

        matrixCopy_2 = matrixCopy_1;
        matrixCopy_2.translate(xi, yi, 0);
        setTransforms(cam_mat, matrixCopy_2); // On applique la matrice
        m_cylinder->draw(m_program, glFuncs); // On dessine
    }

    /*
    // Dessin kite1 à droite, tourné et plus petit
    shape_mat = world_mat;
    shape_mat.translate(m_kite1->radius(), -0.2, 0);
    //shape_mat.rotate(5*m_angle, 1, 0, 0);
    //double s = 0.5 + fabs(m_anim-0.5);
    //shape_mat.scale(s, s, s);
    setTransforms(cam_mat, shape_mat);
    m_gear->draw(m_program, glFuncs);

    // Dessin kite2 au dessus
    shape_mat = world_mat;
    shape_mat.translate(0, m_kite2->radius(), 0);
    setTransforms(cam_mat, shape_mat);
    m_kite2->draw(m_program, glFuncs);*/

    m_program->release();
}


void GLArea::setTransforms(QMatrix4x4 &cam_mat, QMatrix4x4 &shape_mat)
{
    QMatrix4x4 mv_mat = cam_mat*shape_mat;
    m_program->setUniformValue("mvMatrix", mv_mat);

    QMatrix3x3 nor_mat = shape_mat.normalMatrix();
    m_program->setUniformValue("norMatrix", nor_mat);
}


void GLArea::keyPressEvent(QKeyEvent *ev)
{
    //qDebug() << __FUNCTION__ << ev->text();

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
    (void) ev;
    //qDebug() << __FUNCTION__ << ev->text();
}


void GLArea::mousePressEvent(QMouseEvent *ev)
{
    //qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}


void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    //qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}


void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    //qDebug() << __FUNCTION__ << ev->x() << ev->y();
}


void GLArea::onTimeout()
{
    //qDebug() << __FUNCTION__ ;
    m_angle += 1;
    if (m_angle >= 360) m_angle -= 360;
    update();
}


void GLArea::setRadius(double radius)
{
    //qDebug() << __FUNCTION__ << radius << sender();
    if (radius != m_radius && radius > 0.01 && radius <= 10) {
        m_radius = radius;
        //qDebug() << "  emit radiusChanged()";
        emit radiusChanged(radius);
        update();
    }
}

void GLArea::setFrustumNear(double frustumNear)
{
    //qDebug() << __FUNCTION__ << frustumNear << sender();
    if (frustumNear != m_frustumNear && frustumNear > 0.01 && frustumNear <= 10) {
        m_frustumNear = frustumNear;
        //qDebug() << "  emit frustumNearChanged()";
        emit frustumNearChanged(frustumNear);
        update();
    }
}

void GLArea::setFrustumFar(double frustumFar)
{
    //qDebug() << __FUNCTION__ << frustumFar << sender();
    if (frustumFar != m_frustumFar && frustumFar > 0.01 && frustumFar <= 10) {
        m_frustumFar = frustumFar;
        //qDebug() << "  emit frustumFarChanged()";
        emit frustumFarChanged(frustumFar);
        update();
    }
}

void GLArea::setCameraDistance(double cameraDistance)
{
    //qDebug() << __FUNCTION__ << cameraDistance << sender();
    if (cameraDistance != m_cameraDistance && cameraDistance >= -20 && cameraDistance <= 20) {
        m_cameraDistance = cameraDistance;
        //qDebug() << "  emit cameraDistanceChanged()";
        emit cameraDistanceChanged(cameraDistance);
        update();
    }
}

void GLArea::setCameraAngle(double cameraAngle)
{
    //qDebug() << __FUNCTION__ << cameraAngle << sender();
    if (cameraAngle != m_cameraAngle) {
        if(cameraAngle == -1) cameraAngle = 359;
        if(cameraAngle == 360) cameraAngle = 0;
        m_cameraAngle = cameraAngle;
        //qDebug() << "  emit cameraAngleChanged()";
        emit cameraAngleChanged(cameraAngle);
        update();
    }
}


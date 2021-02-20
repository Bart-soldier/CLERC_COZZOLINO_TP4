/**
  CLERC Billy, COZZOLINO Christine
  Programmation Graphique
  Projet
  28/02/2021
**/

#ifndef GLAREA_H
#define GLAREA_H

#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include "kite.h"
#include "cylinder.h"
#include "gear.h"
#include "link.h"

class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = 0);
    ~GLArea();

public slots:
    void setRadius(double radius);
    void setFrustumNear(double frustumNear);
    void setFrustumFar(double frustumFar);
    void setCameraDistance(double cameraDistance);
    void setCameraAngle(double cameraAngle);

signals:  // On ne les implémente pas, elles seront générées par MOC ;
          // les paramètres seront passés aux slots connectés.
    void radiusChanged(double newRadius);
    void frustumNearChanged(double newFrustumNear);
    void frustumFarChanged(double newFrustumFar);
    void cameraDistanceChanged(double newCameraDistance);
    void cameraAngleChanged(double newCameraAngle);

protected slots:
    void onTimeout();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

private:
    double m_angle = 0;
    QTimer *m_timer = nullptr;
    double m_anim = 0;
    double m_radius = 3.25;
    double m_ratio = 1;
    double m_frustumNear = 1.0;
    double m_frustumFar = 7.5;
    double m_cameraDistance = -5.0;
    double m_cameraAngle = 0;

    QOpenGLShaderProgram *m_program;

    void makeGLObjects();
    void tearGLObjects();
    void setTransforms(QMatrix4x4 &cam_mat, QMatrix4x4 &shape_mat);

    Cylinder *m_cylinder = nullptr;
    Gear *m_bigGear = nullptr,
         *m_smallGear = nullptr;
    Link *m_link = nullptr;
    float ep_roue,
          r_roue,
          h_dent,
          alphaBig,
          alphaSmall,
          ep_cyl,
          r_cyl;
    int nb_dent,
        nb_fac;
};

#endif // GLAREA_H

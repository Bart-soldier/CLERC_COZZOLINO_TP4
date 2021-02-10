/**
  CLERC Billy, COZZOLINO Christine
  Programmation Graphique
  TP4
  04/02/2021
**/

#ifndef GLAREA_H
#define GLAREA_H

#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

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
    void doProjection();
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

    void paintTP3(QMatrix4x4 matrix);
    void paintTP4(QMatrix4x4 matrix);
    void paintCylinder(float ep_cyl, float r_cyl, int nb_fac, float col_r, float col_g, float col_b);
    
    void paintLink(float ep_link, float r_link, float edge, float col_r, float col_g, float col_b);

    void paintGear(float ep_roue, float r_roue, float h_dent, int nb_dent, float col_r, float col_g, float col_b);

private:
    double m_angle = 0;
    QTimer *m_timer = nullptr;
    double m_anim = 0;
    double m_radius = 0.5;
    double m_ratio = 1;
    double m_frustumNear = 1.0;
    double m_frustumFar = 5.0;
    double m_cameraDistance = -5.0;
    double m_cameraAngle = 10;

    // Pour utiliser les shaders
    QOpenGLShaderProgram *m_program;
    int m_posAttr;
    int m_colAttr;
    int m_matrixUniform;
};

#endif // GLAREA_H

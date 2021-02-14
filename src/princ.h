/**
  CLERC Billy, COZZOLINO Christine
  Programmation Graphique
  Projet
  28/02/2021
**/

#ifndef PRINC_H
#define PRINC_H

#include "ui_princ.h"

class Princ : public QMainWindow, private Ui::Princ
{
    Q_OBJECT

public:
    explicit Princ(QWidget *parent = 0);

public slots:
    void setRadiusValue(double fRadius);
    void setFrustumNearValue(double fNear);
    void setFrustumFarValue(double fFar);
    void setCameraDistanceValue(double cDistance);
    void setCameraAngleValue(double cAngle);

protected slots:
    void onRadiusValue(int value);
    void onFrustumNearValue(int value);
    void onFrustumFarValue(int value);
    void onCameraDistanceValue(int value);
    void onCameraAngleValue(int value);
private slots:
    void on_cameraButton_clicked();
};

#endif // PRINC_H

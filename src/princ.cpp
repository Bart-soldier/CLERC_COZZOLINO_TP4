/**
  CLERC Billy, COZZOLINO Christine
  Programmation Graphique
  TP4
  04/02/2021
**/

#include "princ.h"
#include <QDebug>

Princ::Princ(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    // Par défaut, on cache les boutons de propriétés de la caméra
    frustumRadius->setVisible(false);
    frustumRadiusLabel->setVisible(false);
    frustumNear->setVisible(false);
    frustumNearLabel->setVisible(false);
    frustumFar->setVisible(false);
    frustumFarLabel->setVisible(false);
    cameraDistance->setVisible(false);
    cameraDistanceLabel->setVisible(false);
    cameraAngle->setVisible(false);
    cameraAngleLabel->setVisible(false);
    oneFrame->setVisible(false);
    launchAnimation->setVisible(false);

    // On définit les valeurs intiales
    frustumRadius->setValue(0.5 * 20);
    frustumNear->setValue(1.0 * 20);
    frustumFar->setValue(5.0 * 20);
    cameraDistance->setValue(-5.0 * 20);
    cameraAngle->setValue(10);

    // On configure les signaux
    connect (glarea, SIGNAL(radiusChanged(double)), this, SLOT(setRadiusValue(double)));
    connect (glarea, SIGNAL(frustumNearChanged(double)), this, SLOT(setFrustumNearValue(double)));
    connect (glarea, SIGNAL(frustumFarChanged(double)), this, SLOT(setFrustumFarValue(double)));
    connect (glarea, SIGNAL(cameraDistanceChanged(double)), this, SLOT(setCameraDistanceValue(double)));
    connect (glarea, SIGNAL(cameraAngleChanged(double)), this, SLOT(setCameraAngleValue(double)));

    connect(frustumRadius, SIGNAL(valueChanged(int)), this, SLOT(onRadiusValue(int)));
    connect(frustumNear, SIGNAL(valueChanged(int)), this, SLOT(onFrustumNearValue(int)));
    connect(frustumFar, SIGNAL(valueChanged(int)), this, SLOT(onFrustumFarValue(int)));
    connect(cameraDistance, SIGNAL(valueChanged(int)), this, SLOT(onCameraDistanceValue(int)));
    connect(cameraAngle, SIGNAL(valueChanged(int)), this, SLOT(onCameraAngleValue(int)));
}

void Princ::on_cameraButton_clicked()
{
    frustumRadius->setVisible(!frustumRadius->isVisible());
    frustumRadiusLabel->setVisible(!frustumRadiusLabel->isVisible());
    frustumNear->setVisible(!frustumNear->isVisible());
    frustumNearLabel->setVisible(!frustumNearLabel->isVisible());
    frustumFar->setVisible(!frustumFar->isVisible());
    frustumFarLabel->setVisible(!frustumFarLabel->isVisible());
    cameraDistance->setVisible(!cameraDistance->isVisible());
    cameraDistanceLabel->setVisible(!cameraDistanceLabel->isVisible());
    cameraAngle->setVisible(!cameraAngle->isVisible());
    cameraAngleLabel->setVisible(!cameraAngleLabel->isVisible());
    oneFrame->setVisible(!oneFrame->isVisible());
    launchAnimation->setVisible(!launchAnimation->isVisible());
}

void Princ::setRadiusValue(double fRadius)
{
    qDebug() << __FUNCTION__ << fRadius << sender();
    int value = fRadius * 20;
    if (frustumRadius->value() != value) {
        qDebug() << "  frustumRadius->setvalue()";
        frustumRadius->setValue(value);
    }
}

void Princ::setFrustumNearValue(double fNear)
{
    qDebug() << __FUNCTION__ << fNear << sender();
    int value = fNear * 20;
    if (frustumNear->value() != value) {
        qDebug() << "  frustumNear->setvalue()";
        frustumNear->setValue(value);
    }
}

void Princ::setFrustumFarValue(double fFar)
{
    qDebug() << __FUNCTION__ << fFar << sender();
    int value = fFar * 20;
    if (frustumFar->value() != value) {
        qDebug() << "  frustumFar->setvalue()";
        frustumFar->setValue(value);
    }
}

void Princ::setCameraDistanceValue(double cDistance)
{
    qDebug() << __FUNCTION__ << cDistance << sender();
    int value = cDistance * 20;
    if (cameraDistance->value() != value) {
        qDebug() << "  cameraDistance->setvalue()";
        cameraDistance->setValue(value);
    }
}

void Princ::setCameraAngleValue(double cAngle)
{
    qDebug() << __FUNCTION__ << cAngle << sender();
    if (cameraAngle->value() != cAngle) {
        qDebug() << "  cameraAngle->setvalue()";
        cameraAngle->setValue(cAngle);
    }
}

void Princ::onRadiusValue(int value)
{
    qDebug() << __FUNCTION__ << value << sender();
    qDebug() << "  emit radiusChanged()";
    emit glarea->radiusChanged(value/20.0);
}

void Princ::onFrustumNearValue(int value)
{
    qDebug() << __FUNCTION__ << value << sender();
    qDebug() << "  emit frustumNearChanged()";
    emit glarea->frustumNearChanged(value/20.0);
}

void Princ::onFrustumFarValue(int value)
{
    qDebug() << __FUNCTION__ << value << sender();
    qDebug() << "  emit frustumFarChanged()";
    emit glarea->frustumFarChanged(value/20.0);
}

void Princ::onCameraDistanceValue(int value)
{
    qDebug() << __FUNCTION__ << value << sender();
    qDebug() << "  emit cameraDistanceChanged()";
    emit glarea->cameraDistanceChanged(value/20.0);
}

void Princ::onCameraAngleValue(int value)
{
    qDebug() << __FUNCTION__ << value << sender();
    qDebug() << "  emit cameraAngleChanged()";
    emit glarea->cameraAngleChanged(value);
}

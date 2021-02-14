/**
  CLERC Billy, COZZOLINO Christine
  Programmation Graphique
  Projet
  28/02/2021
**/

#include "princ.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Princ w;
    w.show();

    return a.exec();
}

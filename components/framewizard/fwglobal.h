#ifndef FWGLOBAL_H
#define FWGLOBAL_H

#include <qstring.h>

int proxInt(double d);

int cancelledPixels(int n);

QString absolutize(QString dummySrc,QString path);
QString relativize(QString dummySrc,QString path);

#endif

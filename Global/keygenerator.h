#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <QString>
#include "lunhalgorithm.h"

class KeyGenerator
{
public:
    static QString generateKey (QString time, QString serial, QString constant);
private:
    static QString getKey (QString input);
};

#endif // KEYGENERATOR_H

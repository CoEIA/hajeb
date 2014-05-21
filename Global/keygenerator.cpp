#include "keygenerator.h"

QString KeyGenerator::generateKey(QString time, QString serial, QString constant) {
    QString variables = time + serial + constant ;
    QString result = variables + LunhAlgorithm::lunhGenerator(variables);
    QString key = getKey(result.toAscii().toBase64());
    return key;
}

QString KeyGenerator::getKey(QString input) {
    QString result = "" ;

    for (int i=0; i<input.size(); i++) {
        if ( i % 5 != 0 || i == 0)
            result += input[i];
        else if ( i%5 == 0){
            result += "-" + input[i];
        }
    }

    return (result);
}

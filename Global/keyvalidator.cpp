#include "keyvalidator.h"

bool KeyValidator::validate (QString key) {
    key = key.replace("-", "");
    key = key.toAscii().fromBase64(key.toAscii());
    const QString constantNumber = "853023";

    if ( key.contains(constantNumber))
        return LunhAlgorithm::lunhValidation(key);
    else
        return false;
}

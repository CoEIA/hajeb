#ifndef LICENSE_H
#define LICENSE_H

class License {
public:
    static bool verifySerialNumber(QString);
    static bool verifySerialNumberOnline(QString);
private:
    static void saveSerialNumber(QString);
};

#endif // LICENSE_H

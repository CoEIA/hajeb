#ifndef XMLLISTWRITER_H
#define XMLLISTWRITER_H

#include <QMap>
#include <QString>
#include "xmlconstants.h"

class XmlListWriter
{
public:
    XmlListWriter();
    void write(ListType listType);
    bool checkHash(ListType listType);
private:
    void writeFileHash(ListType listType);
    QMap<ListType, QString> m_xmlFileMap;
    QMap<ListType, QString> m_md5FileMap;
    QMap<ListType, QString> m_prefixMap;
};

#endif // XMLLISTWRITER_H

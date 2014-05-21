#ifndef XMLCONSTANTS_H
#define XMLCONSTANTS_H

#include <QString>

enum ListType {BLACK_LIST, WHITE_LIST};

enum DownloadOperation {
    DOWNLOAD_HASH,
    DOWNLOAD_LIST
};

enum UpdateResult {
    NO_UPDATE,
    UPDATE
};

const QString XML_TAG_LIST      =       "LIST";
const QString XML_TAG_CAT       =       "CAT";
const QString XML_TAG_WEB       =       "WEB";

const QString XML_ATTR_ID       =       "ID";
const QString XML_ATTR_NAME     =       "NAME";
const QString XML_ATTR_URL      =       "URL";
const QString XML_ATTR_HOST     =       "HOST";

#endif // XMLCONSTANTS_H

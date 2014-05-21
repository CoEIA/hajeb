/*
* Copyright (C) 2014 - Center of Excellence in Information Assurance
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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

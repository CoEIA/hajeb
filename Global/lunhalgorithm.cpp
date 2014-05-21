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
#include "lunhalgorithm.h"

LunhAlgorithm::LunhAlgorithm()
{
}

char LunhAlgorithm:: lunhGenerator (QString input) {
    int factor = 2 ;
    int sum = 0;
    int n = 128;

    for (int i=input.size()-1 ; i>= 0;  i--) {
        int codePoint = (int) input[i].toAscii();
        int addend = factor * codePoint;

        factor = (factor == 2 ) ? 1 : 2;

        addend = (addend/n) + (addend%n);
        sum += addend;
    }

    int remainder = sum%n;
    int checkCodePoint = n - remainder;
    checkCodePoint %= n;

    return (char) checkCodePoint;
}

bool LunhAlgorithm:: lunhValidation (QString input) {
    int factor = 1 ;
    int sum = 0;
    int n = 128;

    for (int i=input.size() -1; i>=0; i--) {
        int codePoint = (int) input[i].toAscii();
        int addend = factor * codePoint;

        factor = (factor == 2 ) ? 1 : 2;

        addend = (addend/n) + (addend%n);
        sum += addend;
    }

    int remainder = sum % n;
    return ( remainder == 0 );
}

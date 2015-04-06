//
//  Structure.cpp
//  putoputo
//
//  Created by Riya.Liel on 2015/04/04.
//
//

#include "Structure.h"

bool PosIndex::operator==(const PosIndex&& rhs){
    return x == rhs.x && y == rhs.y;
}

PosIndex PosIndex::operator+(const PosIndex&& rhs){
    return PosIndex(x+rhs.x,y+rhs.y);
}

PosIndex PosIndex::operator-(const PosIndex&& rhs){
    return PosIndex(x-rhs.x,y-rhs.y);
}
PosIndex PosIndex::operator*(int rhs){
    PosIndex(rhs*x,rhs*y);
}
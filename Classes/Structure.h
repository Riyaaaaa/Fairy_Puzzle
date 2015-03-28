//
//  Structure.h
//  putoputo
//
//  Created by Riya.Liel on 2015/03/26.
//
//

#ifndef putoputo_Structure_h
#define putoputo_Structure_h

using PosIndex = cocos2d::Vec2;

const cocos2d::Size DESIGN_SIZE(1136, 640);
const int WIDTH_PUTO_NUM = 6;
const int HEIGHT_PUTO_NUM = 11;

enum class VECTOR{ //scoped enumeration
    UP=1,
    RIGHT,
    DOWN,
    LEFT,
    NONE,
};

enum ZOrder{
    WINDOW=0,
    PUTO,
    
};

const double PI = 3.14;
#endif

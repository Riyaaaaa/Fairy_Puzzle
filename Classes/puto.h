//
//  puto.h
//  putoputo
//
//  Created by Riya.Liel on 2015/03/22.
//
//

#ifndef __putoputo__puto__
#define __putoputo__puto__

#include"cocos2d.h"
#include "Structure.h"

class puto : public cocos2d::Sprite{
public:
    enum class TYPE{ //puto type
        RED=0,
        GREEN,
        BLUE,
        YELLOW,
        PURPLE,
        OZYANA,
        NONE,
    };
    
    virtual bool init(TYPE type);
    static puto* create(TYPE type);
    
    void setPosIndex(PosIndex);
private:
    TYPE _type;
    PosIndex _index;
};

#endif /* defined(__putoputo__puto__) */

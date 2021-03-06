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
    static puto* create();
    
    static TYPE getRandom();
    static void rndInit();
    
    void setPosIndex(PosIndex);
    
    CC_SYNTHESIZE(int, _removeNo, RemoveNo);
    CC_SYNTHESIZE_READONLY(TYPE, _type, Type);
private:
    PosIndex _index;
    
    static std::mt19937 _engine;
    static std::uniform_int_distribution<> _distribution;
};

#endif /* defined(__putoputo__puto__) */

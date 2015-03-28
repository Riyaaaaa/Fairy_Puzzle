//
//  connectedPuto.h
//  putoputo
//
//  Created by Riya.Liel on 2015/03/25.
//
//

#ifndef __putoputo__connectedPuto__
#define __putoputo__connectedPuto__

#include "puto.h"
#include "Structure.h"

class connectedPuto{
public:
    connectedPuto(cocos2d::Node*);
    
    void rotateRight();
    void rotateLeft ();
    
    virtual void progress(int movement);
    
    /*
    void setPositionOrigin();
    void getPositionOrigin();
    
    void setPositionOptional();
    void getPositionOptional();
     */
    
    void setPositionOriginByPosIndex(PosIndex);
    void setPositionOptionalByPosIndex(PosIndex);
    
    void moveDelta(cocos2d::Vec2 delta);
    
    CC_SYNTHESIZE_READONLY(puto*, _origin, originPuto);
    CC_SYNTHESIZE_READONLY(puto*, _opt, optionalPuto);
    
    puto* getOriginPuto  ();
    puto* getOptionalPuto();
    
    bool newPuto(puto::TYPE,puto::TYPE);
    
    void getSemaphore    (){_semaphore=false;}
    void releaceSemaphore(){_semaphore=true;};
    bool isCanAccess(){return _semaphore;};
private:
    
    cocos2d::Node* _owner;
    
    bool _semaphore=true;
    
    VECTOR OPT_POSITION;
    
    const int TAG_ORIGIN=100,TAG_OPT=101;
};

#endif /* defined(__putoputo__connectedPuto__) */

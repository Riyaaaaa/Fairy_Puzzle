//
//  PuzzleFiled.h
//  putoputo
//
//  Created by Riya.Liel on 2015/03/27.
//
//

#ifndef __putoputo__PuzzleFiled__
#define __putoputo__PuzzleFiled__

#include "connectedPuto.h"
#include "Structure.h"
#include "puto.h"
#include <array>

class PuzzleFiled : public cocos2d::DrawNode{
public:
    void pushNewPuto(puto::TYPE,puto::TYPE);
    void progress();
    void progressWithMovement(int);
    
    virtual bool init(cocos2d::Size);
    static PuzzleFiled* create(cocos2d::Size);
    
    puto* getPutoMapCell(PosIndex);
    
    enum class STATUS{
        WAITING=0,
        PLAYING,
        ACTING,
        LOSED,
        WON,
    };
    
    CC_SYNTHESIZE_READONLY(STATUS, _status, Status);
    
protected:
    PuzzleFiled();
    virtual ~PuzzleFiled() = default;
    
    PosIndex convertPosIndex(cocos2d::Vec2);
    
    /* CallBack Touch Event*/
    virtual bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    virtual void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    virtual void onTouchCancelled(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    
    void fallPuto(puto*);
    
    int _movement     =-20;
    
    std::array<std::array<puto*,WIDTH_PUTO_NUM>,HEIGHT_PUTO_NUM> _putoMap;
    
    connectedPuto _mainPuto;
    
    cocos2d::DrawNode* _mainWindow;
    
    cocos2d::Vec2 _oldLocation,_stdPos;
    VECTOR SWIPE_PARAM;

};

#endif /* defined(__putoputo__PuzzleFiled__) */

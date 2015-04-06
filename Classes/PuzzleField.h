//
//  PuzzleField.h
//  putoputo
//
//  Created by Riya.Liel on 2015/03/27.
//
//

#ifndef __putoputo__PuzzleField__
#define __putoputo__PuzzleField__

#include "connectedPuto.h"
#include "Structure.h"
#include "puto.h"
#include <array>

class PuzzleField : public cocos2d::DrawNode{
public:
    void pushNewPuto(puto::TYPE,puto::TYPE);    //basically, plz call while _status is WAITING. this set instance to  _mainPuto
    void progress();                            //game progress
    void progressWithMovement(int);             //fall processing
    
    virtual bool init(cocos2d::Size); //initialize by window size
    static PuzzleField* create(cocos2d::Size);
    
    bool isPutoContain(puto*);
    
    puto* getPutoMapCell(PosIndex);
    void setPutoPosIndex(puto*,PosIndex);
    
    void rotate(VECTOR);
    
    enum class STATUS{  //express status of game
        WAITING=0,      //_mainPuto is null
        PLAYING,        //in progress
        ACTING,         //while animation,or gravity processing
        LOSED,          //game ended
        WON,            //game ended
    };
    
    CC_SYNTHESIZE_READONLY(STATUS, _status, Status); //status of game
    
protected:
    PuzzleField(); //constructor protected. plz use static function create
    virtual ~PuzzleField() = default;
    
    cocos2d::Size _window_size;
    
    PosIndex convertPosIndex(cocos2d::Vec2); //Vec2 -> PosIndex
    
    /* CallBack Touch Event*/
    virtual bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    virtual void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    virtual void onTouchCancelled(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    
    void fallPuto(puto*); //gravity processing (after set origin or optional puto)
    void patchGravity();
    
    int _movement     =-20; //movement of puto per frame
    
    std::array<std::array<puto*,WIDTH_PUTO_NUM>,HEIGHT_PUTO_NUM> _putoMap; //puto list
    
    connectedPuto _mainPuto; //puto falling
    
    /* member variable to swipe processing */
    cocos2d::Vec2 _oldLocation,_stdPos;
    VECTOR SWIPE_PARAM;
    
    void removePuto();

};

#endif /* defined(__putoputo__PuzzleField__) */

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
    void pushPuto(puto::TYPE,puto::TYPE);
    puto* createPuto(puto::TYPE);    //basically, plz call while _status is WAITING. this set instance to  _mainPuto
    
    void popPuto();
    
    void progress();                            //game progress
    void progressWithMovement(int);             //fall processing
    
    virtual bool init(cocos2d::Size,std::array<puto::TYPE,HEIGHT_PUTO_NUM>); //initialize by window size
    bool initPutoList(std::array<puto::TYPE,HEIGHT_PUTO_NUM>);
    
    static PuzzleField* create(cocos2d::Size,std::array<puto::TYPE,HEIGHT_PUTO_NUM>);
    
    bool isLive();
    
    puto* getPutoMapCell(PosIndex);
    void setPutoPosIndex(puto*,PosIndex);
    
    void rotate(VECTOR);
    
    //bool isCanMove(puto*,VECTOR);
    bool isCanMove(connectedPuto&,VECTOR);
    
    cocos2d::Size getPutoSize(){return cocos2d::Size(_window_size.width/WIDTH_PUTO_NUM,
                                                     _window_size.height/HEIGHT_PUTO_NUM);};
    
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
    std::deque<puto*> next_puto_list;
    
    connectedPuto _mainPuto; //puto falling
    
    /* member variable to swipe processing */
    cocos2d::Vec2 _oldLocation,_stdPos;
    VECTOR SWIPE_PARAM;
    
    void removePuto();
    void recursive_search(PosIndex,puto::TYPE,std::vector<PosIndex>&);

};

#endif /* defined(__putoputo__PuzzleField__) */

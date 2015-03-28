//
//  PuzzleFiled.cpp
//  putoputo
//
//  Created by Riya.Liel on 2015/03/27.
//
//

#include "PuzzleFiled.h"

USING_NS_CC;

PuzzleFiled::PuzzleFiled() : _mainPuto(this){
    
}

PuzzleFiled* PuzzleFiled::create(Size window_size){
    PuzzleFiled* pRef = new PuzzleFiled;
    if(pRef && pRef->init(window_size)){
        pRef->autorelease();
    }
    else{
        delete pRef;
        pRef = nullptr;
    }
    return pRef;
}

bool PuzzleFiled::init(Size window_size){
    if(!DrawNode::init())return false;
    
    std::vector<Vec2> vecs={Vec2(window_size),Vec2(0,window_size.height),Vec2(0,0),Vec2(window_size.width,0)};
    const int SQUARE_POINT_NUM=4;
    
    _mainWindow->drawPolygon(&vecs[0], SQUARE_POINT_NUM, Color4F(.5f, .6f, 1.0f, 1.0f), 5, Color4F::BLACK);
    _mainWindow->setOpacity(128);
    
    _status = STATUS::WAITING;
    
    for(auto cell: _putoMap){
        cell.fill(nullptr);
    }
    
    return true;
}


bool PuzzleFiled::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* unused_event){
    _stdPos = touch->getLocation();
    SWIPE_PARAM = VECTOR::NONE;
    
    return true;
}

void PuzzleFiled::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* unused_event){
    Vec2 pos = touch->getLocation();
    
    if(SWIPE_PARAM == VECTOR::NONE && (_stdPos.x - pos.x) > 50){
        SWIPE_PARAM = VECTOR::LEFT;
    }
    else if(SWIPE_PARAM == VECTOR::NONE && (pos.x - _stdPos.x) > 50){
        SWIPE_PARAM = VECTOR::RIGHT;
    }
    
    if(fabs(touch->getDelta().x) < fabs(touch->getDelta().y) && touch->getDelta().y < 0){
        _mainPuto.moveDelta(Vec2(0,touch->getDelta().y/2));
    }
    
    _oldLocation = pos;
    
}
void PuzzleFiled::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* unused_event){
    if(SWIPE_PARAM == VECTOR::RIGHT){
        _mainPuto.rotateRight();
    }
    else if(SWIPE_PARAM == VECTOR::LEFT){
        _mainPuto.rotateLeft();
    }
}
void PuzzleFiled::onTouchCancelled(cocos2d::Touch* touch,cocos2d::Event* unused_event){
    onTouchEnded(touch, unused_event);
}

puto* PuzzleFiled::getPutoMapCell(PosIndex index){
    CC_ASSERT(index.x < WIDTH_PUTO_NUM && index.y < HEIGHT_PUTO_NUM && index.x >=0 && index.y >=0);
    return _putoMap[index.y][index.x];
}

PosIndex PuzzleFiled::convertPosIndex(Vec2 pos){
    PosIndex index;
    
    index.x = pos.x / (DESIGN_SIZE.width/WIDTH_PUTO_NUM);
    index.y = pos.y / (DESIGN_SIZE.height/HEIGHT_PUTO_NUM);
    
    return index;
}

void PuzzleFiled::fallPuto(puto* target){
    PosIndex index = convertPosIndex(target->getPosition());
    _status = STATUS::ACTING;
    
    for(int i = 0 ; i <= index.y ; i++){
        if(getPutoMapCell(PosIndex(index.x , i)) == nullptr){
            target->setPosIndex(PosIndex(index.x , i));
            _status = STATUS::WAITING;
        }
    }
    
}

void PuzzleFiled::progressWithMovement(int movement){
    PosIndex originIndex = convertPosIndex(_mainPuto.getOriginPuto()->getPosition());
    PosIndex optIndex = convertPosIndex(_mainPuto.getOptionalPuto()->getPosition());
    
    _mainPuto.moveDelta(Vec2(0,movement));
    if(_mainPuto.isCanAccess()){
        if(getPutoMapCell(originIndex) != nullptr){
            _mainPuto.getOriginPuto()->setPosIndex(originIndex+PosIndex(0,1));
            fallPuto(_mainPuto.getOptionalPuto());
        }
        else if(getPutoMapCell(optIndex) != nullptr){
            _mainPuto.getOptionalPuto()->setPosIndex(originIndex+PosIndex(0,1));
            fallPuto(_mainPuto.getOriginPuto());
        }
    }
}

void PuzzleFiled::pushNewPuto(puto::TYPE origin_type,puto::TYPE opt_type){
    _mainPuto.newPuto(origin_type,opt_type);
    
    _status = STATUS::PLAYING;
    
    /*
     switch(type){
     case puto::TYPE::RED:
     break;
     case puto::TYPE::GREEN:
     break;
     case puto::TYPE::BLUE:
     break;
     case puto::TYPE::YELLOW:
     break;
     case puto::TYPE::PURPLE:
     break;
     default:
     break;
     }
    
     */
    
    
}
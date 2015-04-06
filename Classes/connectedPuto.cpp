//
//  connectedPuto.cpp
//  putoputo
//
//  Created by Riya.Liel on 2015/03/25.
//
//

#include "connectedPuto.h"

USING_NS_CC;

connectedPuto::connectedPuto(cocos2d::Node* owner){
    _owner = owner;
    
}

void connectedPuto::progress(int movement){
    
    _origin->setPosition(_origin->getPosition()+Vec2(0,movement));
    _opt->setPosition(_opt->getPosition()+Vec2(0,movement));

}

bool connectedPuto::newPuto(puto::TYPE originType,puto::TYPE optType){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _origin = puto::create(originType);
    _opt = puto::create(optType);
    
    _origin->setScale(_puto_size.width / _origin->getContentSize().width);
    _opt->setScale(_puto_size.width / _opt->getContentSize().width);
    
    _owner->addChild(_origin,PUTO,TAG_ORIGIN);
    _owner->addChild(_opt,PUTO,TAG_OPT);
    
    OPT_POSITION = VECTOR::UP;
}

void connectedPuto::moveDelta(cocos2d::Vec2 delta){
    _origin->setPosition(_origin->getPosition()+delta);
    _opt->setPosition(_opt->getPosition()+delta);
}

void connectedPuto::rotateRight(){
    double rad;
    int radius = _origin->getContentSize().width;
    switch (OPT_POSITION) {
        case VECTOR::UP:
            rad = PI/2;
            OPT_POSITION = VECTOR::RIGHT;
            break;
        case VECTOR::RIGHT:
            rad = 0;
            OPT_POSITION = VECTOR::DOWN;
            break;
        case VECTOR::LEFT:
            rad = PI;
            OPT_POSITION = VECTOR::UP;
            break;
        case VECTOR::DOWN:
            rad = PI*3/2;
            OPT_POSITION = VECTOR::LEFT;
            break;
        default:
            break;
    }
    Vector<FiniteTimeAction*> action_arr;
    this->getSemaphore();
    for(int i=0;i<4;i++){
        rad -= PI/8;
        action_arr.pushBack(CallFuncN::create([=](Node* node){
                node->setPosition(_origin->getPosition() + radius * Vec2(cos(rad),sin(rad)));
                }));
        action_arr.pushBack(DelayTime::create(0.1f));
    }
        action_arr.pushBack(CallFunc::create(CC_CALLBACK_0(connectedPuto::releaceSemaphore, this)));
        
    _opt->runAction(Sequence::create(action_arr));
    
}

void connectedPuto::rotateLeft (){
    int radius = _origin->getContentSize().width;
    double rad;
    Vec2 originPos = _origin->getPosition();
    switch (OPT_POSITION) {
        case VECTOR::UP:
            rad = PI/2;
            OPT_POSITION = VECTOR::LEFT;
            break;
        case VECTOR::RIGHT:
            rad = 0;
            OPT_POSITION = VECTOR::UP;
            break;
        case VECTOR::LEFT:
            rad = PI;
            OPT_POSITION = VECTOR::DOWN;
            break;
        case VECTOR::DOWN:
            rad = PI*3/2;
            OPT_POSITION = VECTOR::RIGHT;
            break;
        default:
            break;
    }
    Vector<FiniteTimeAction*> action_arr;
    this->getSemaphore();
    for(int i=0;i<4;i++){
        rad += PI/8;
        action_arr.pushBack(CallFuncN::create([=](Node* node){
            node->setPosition(_origin->getPosition() + radius * Vec2(cos(rad),sin(rad)));
        }));
        action_arr.pushBack(DelayTime::create(0.1f));
    }
    action_arr.pushBack(CallFunc::create(CC_CALLBACK_0(connectedPuto::releaceSemaphore, this)));
    
    _opt->runAction(Sequence::create(action_arr));

}

void connectedPuto::rotate(VECTOR N){
    int radius = _origin->getContentSize().width;
    double rad;
    Vec2 originPos = _origin->getPosition();
    
    switch (OPT_POSITION) {
        case VECTOR::UP:
            rad = PI/2;
            if(N==VECTOR::LEFT)OPT_POSITION = VECTOR::LEFT;
            else if(N==VECTOR::RIGHT)OPT_POSITION = VECTOR::RIGHT;
            break;
        case VECTOR::RIGHT:
            rad = 0;
            if(N==VECTOR::LEFT)OPT_POSITION = VECTOR::UP;
            else if(N==VECTOR::RIGHT)OPT_POSITION = VECTOR::DOWN;
            break;
        case VECTOR::LEFT:
            rad = PI;
            if(N==VECTOR::LEFT)OPT_POSITION = VECTOR::DOWN;
            else if(N==VECTOR::RIGHT)OPT_POSITION = VECTOR::UP;
            break;
        case VECTOR::DOWN:
            rad = PI*3/2;
            if(N==VECTOR::LEFT)OPT_POSITION = VECTOR::RIGHT;
            else if(N==VECTOR::RIGHT)OPT_POSITION = VECTOR::LEFT;
            break;
        default:
            break;
    }
    Vector<FiniteTimeAction*> action_arr;
    this->getSemaphore();
    for(int i=0;i<4;i++){
        if(N == VECTOR::LEFT)rad += PI/8;
        else if(N==VECTOR::RIGHT)rad -= PI/8;
        action_arr.pushBack(CallFuncN::create([=](Node* node){
            node->setPosition(_origin->getPosition() + radius * Vec2(cos(rad),sin(rad)));
        }));
        action_arr.pushBack(DelayTime::create(0.03f));
    }
    action_arr.pushBack(CallFunc::create(CC_CALLBACK_0(connectedPuto::releaceSemaphore, this)));
    
    _opt->runAction(Sequence::create(action_arr));

}
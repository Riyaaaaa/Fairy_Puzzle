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
    
    _origin->setPosition(Vec2(visibleSize.width/2,visibleSize.height));
    _opt->setPosition(Vec2(visibleSize.width/2,visibleSize.height + _origin->getContentSize().height));
    
    _owner->addChild(_origin,PUTO,TAG_ORIGIN);
    _owner->addChild(_opt,PUTO,TAG_OPT);
    
    OPT_POSITION = VECTOR::UP;
}

void connectedPuto::moveDelta(cocos2d::Vec2 delta){
    _origin->setPosition(_origin->getPosition()+Vec2(0,delta.y));
    _opt->setPosition(_opt->getPosition()+Vec2(0,delta.y));
}

void connectedPuto::rotateRight(){
    double rad;
    Vec2 originPos = _origin->getPosition();
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
    CallFuncN* func = CallFuncN::create([=](Node* _node){
        Vector<FiniteTimeAction*> action_arr;
        double _rad = rad;
        this->getSemaphore();
        for(int i=0;i<4;i++){
            _rad-=PI/8;
            action_arr.pushBack(MoveTo::create(0.1f,(originPos+_node->getContentSize().width*Vec2(cos(_rad),sin(_rad)))));
        }
        action_arr.pushBack(CallFunc::create(CC_CALLBACK_0(connectedPuto::releaceSemaphore, this)));
        _node->runAction(Sequence::create(action_arr));
    });

    _opt->runAction(func);
    
    CCLOG("rotate right");
}

void connectedPuto::rotateLeft (){
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
    CallFuncN* func = CallFuncN::create([=](Node* _node){
        Vector<FiniteTimeAction*> action_arr;
        double _rad = rad;
        this->getSemaphore();
        for(int i=0;i<4;i++){
            _rad+=PI/8;
            action_arr.pushBack(MoveTo::create(0.1f,(originPos+_node->getContentSize().width*Vec2(cos(_rad),sin(_rad)))));
        }
        action_arr.pushBack(CallFunc::create(CC_CALLBACK_0(connectedPuto::releaceSemaphore, this)));
        _node->runAction(Sequence::create(action_arr));
    });
    
    _opt->runAction(func);

    CCLOG("rotate left");
}
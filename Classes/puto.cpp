//
//  puto.cpp
//  putoputo
//
//  Created by Riya.Liel on 2015/03/22.
//
//

#include "puto.h"

USING_NS_CC;

std::mt19937 puto::_engine;
std::uniform_int_distribution<> puto::_distribution;

puto* puto::create(TYPE type){
    puto* pRef = new puto;
    if(pRef && pRef->init(type)){
        pRef->autorelease();
    }
    else{
        delete pRef;
        pRef = nullptr;
    }
    return pRef;
}

bool puto::init(TYPE type){
    Size design_size = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
    std::string filePath("drop");
    filePath += std::to_string(static_cast<int>(type)) += ".png";
    if(!Sprite::initWithFile(filePath))return false;
    
    setAnchorPoint(Vec2(0.0f,0.0f));
    
    _type = type;
    return true;
}

puto* puto::create(){
    puto* pRef = new puto;
    if(pRef && pRef->init(getRandom())){
        pRef->autorelease();
    }
    else{
        delete pRef;
        pRef = nullptr;
    }
    return pRef;
}

void puto::setPosIndex(PosIndex index){
    setPosition(index.x * DESIGN_SIZE.width/WIDTH_PUTO_NUM , index.y * DESIGN_SIZE.height/HEIGHT_PUTO_NUM);
    _index = index;
}

void puto::rndInit(){
    std::random_device rnd;
    _engine =std::mt19937(rnd());
    _distribution=std::uniform_int_distribution<>(1,5);
}

puto::TYPE puto::getRandom(){
    return static_cast<TYPE>(_distribution(_engine));
}

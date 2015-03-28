//
//  puto.cpp
//  putoputo
//
//  Created by Riya.Liel on 2015/03/22.
//
//

#include "puto.h"

USING_NS_CC;

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
    std::string filePath("puto");
    filePath += std::to_string(static_cast<int>(type)) += ".png";
    if(!Sprite::initWithFile(filePath))return false;
    
    setScale(design_size.width/6/getContentSize().width);
    setAnchorPoint(Vec2(0.5f,0.5f));
    
    _type = type;
    return true;
}

void puto::setPosIndex(PosIndex index){
    setPosition(index.x * DESIGN_SIZE.width/WIDTH_PUTO_NUM , index.y * DESIGN_SIZE.height/HEIGHT_PUTO_NUM);
    _index = index;
}

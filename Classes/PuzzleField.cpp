//
//  PuzzleField.cpp
//  putoputo
//
//  Created by Riya.Liel on 2015/03/27.
//
//

#include "PuzzleField.h"

USING_NS_CC;

PuzzleField::PuzzleField() : _mainPuto(this){
    
}

PuzzleField* PuzzleField::create(Size window_size){
    PuzzleField* pRef = new PuzzleField;
    if(pRef && pRef->init(window_size)){
        pRef->autorelease();
    }
    else{
        delete pRef;
        pRef = nullptr;
    }
    return pRef;
}

bool PuzzleField::init(Size window_size){
    if(!DrawNode::init())return false;
    
    Vec2 vecs[]={Vec2(window_size),Vec2(0,window_size.height),Vec2(0,0),Vec2(window_size.width,0)};
    const int SQUARE_POINT_NUM=4;
    
    _window_size = window_size;
    
    setContentSize(_window_size);
    
    drawPolygon(&vecs[0], SQUARE_POINT_NUM, Color4F(.5f, .6f, 1.0f, 0.5f), 5, Color4F::BLACK);
    
    setOpacity(128);
    
    _status = STATUS::WAITING;
    
    for(auto &cell: _putoMap){
        cell.fill(nullptr);
    }
    
    auto touchListner = EventListenerTouchOneByOne::create();
    touchListner->setSwallowTouches(true);
    touchListner->onTouchBegan = CC_CALLBACK_2(PuzzleField::onTouchBegan,this);
    touchListner->onTouchMoved = CC_CALLBACK_2(PuzzleField::onTouchMoved,this);
    touchListner->onTouchEnded = CC_CALLBACK_2(PuzzleField::onTouchEnded,this);
    touchListner->onTouchCancelled = CC_CALLBACK_2(PuzzleField::onTouchCancelled,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListner, this);
    
    
    return true;
}


bool PuzzleField::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* unused_event){
    _stdPos = touch->getLocation();
    SWIPE_PARAM = VECTOR::NONE;
    
    return true;
}

void PuzzleField::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* unused_event){
    Vec2 pos = touch->getLocation();
    
    if(SWIPE_PARAM == VECTOR::NONE && (_stdPos.x - pos.x) > 50){
        SWIPE_PARAM = VECTOR::LEFT;
    }
    else if(SWIPE_PARAM == VECTOR::NONE && (pos.x - _stdPos.x) > 50){
        SWIPE_PARAM = VECTOR::RIGHT;
    }
    
    if(SWIPE_PARAM == VECTOR::NONE){
        if(fabs(touch->getDelta().x) < fabs(touch->getDelta().y) && touch->getDelta().y < 0){
            progressWithMovement(touch->getDelta().y/2);
        }
    }
    
    _oldLocation = pos;
    
}
void PuzzleField::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* unused_event){
    Vec2 pos = touch->getLocation();
    
    if(SWIPE_PARAM == VECTOR::RIGHT){
        if(fabs(_stdPos.y - pos.y) < 50){
            if(convertPosIndex(_mainPuto.getOptionalPuto()->getPosition()).x != WIDTH_PUTO_NUM-1 &&
               convertPosIndex(_mainPuto.getOriginPuto()->getPosition()).x != WIDTH_PUTO_NUM-1)
                _mainPuto.moveDelta(Vec2(_window_size.width/WIDTH_PUTO_NUM,0));
        }
        else rotate(VECTOR::RIGHT);
    }
    else if(SWIPE_PARAM == VECTOR::LEFT){
        if(fabs(_stdPos.y - pos.y) < 50){
            if(convertPosIndex(_mainPuto.getOptionalPuto()->getPosition()).x != 0 &&
               convertPosIndex(_mainPuto.getOriginPuto()->getPosition()).x != 0)
            _mainPuto.moveDelta(Vec2(-1*_window_size.width/WIDTH_PUTO_NUM,0));
        }
        else rotate(VECTOR::LEFT);
    }
}
void PuzzleField::onTouchCancelled(cocos2d::Touch* touch,cocos2d::Event* unused_event){
    onTouchEnded(touch, unused_event);
}

puto* PuzzleField::getPutoMapCell(PosIndex index){
    CC_ASSERT(index.x < WIDTH_PUTO_NUM && index.x >=0 && index.y >=0);
    if(index.y >= HEIGHT_PUTO_NUM){
        return nullptr;
    }
    return _putoMap[index.y][index.x];
}

PosIndex PuzzleField::convertPosIndex(Vec2 pos){
    PosIndex index;
    
    index.x = (pos.x+1) / (_window_size.width/WIDTH_PUTO_NUM);
    index.y = pos.y / (_window_size.height/HEIGHT_PUTO_NUM);
    
    if(pos.y - _mainPuto.getOriginPuto()->getContentSize().height/2 < 0) {index.y = -1;}
    
    return index;
}

void PuzzleField::fallPuto(puto* target){
    PosIndex index = convertPosIndex(target->getPosition());
    _status = STATUS::ACTING;
    
    for(int i = 0 ; i <= index.y + 1 ; i++){
        if(getPutoMapCell(PosIndex(index.x , i)) == nullptr){
            setPutoPosIndex(target,PosIndex(index.x , i));
            _status = STATUS::WAITING;
            break;
        }
    }
    
    removePuto();
}

void PuzzleField::patchGravity(){
    int count;
    puto* target;
    
    for(int j = 0 ; j < WIDTH_PUTO_NUM ; j++){
        count=0;
        for(int i = 0 ; i < HEIGHT_PUTO_NUM ; i++){
            target = getPutoMapCell(PosIndex(j,i));
            if(target != nullptr){
                setPutoPosIndex(target, PosIndex(j,count));
                _putoMap[count][j] = target;
                count++;
            }
        }
        for(int k=count;k<HEIGHT_PUTO_NUM;k++){
            _putoMap[k][j] = nullptr;
        }
    }
}

void PuzzleField::setPutoPosIndex(puto* target,PosIndex index){
    CCLOG("%d %d",WIDTH_PUTO_NUM,HEIGHT_PUTO_NUM);
    target->setPosition(Vec2(static_cast<int>(index.x) * _window_size.width/WIDTH_PUTO_NUM,
                             static_cast<int>(index.y) * _window_size.height/HEIGHT_PUTO_NUM));
    _putoMap[index.y][index.x] = target;
}

void PuzzleField::progress(){
    progressWithMovement(_movement);
}

void PuzzleField::progressWithMovement(int movement){
    PosIndex originIndex = convertPosIndex(_mainPuto.getOriginPuto()->getPosition());
    PosIndex optIndex = convertPosIndex(_mainPuto.getOptionalPuto()->getPosition());
    
    _mainPuto.moveDelta(Vec2(0,movement));
    
    //if(_mainPuto.isCanAccess()){
    if(originIndex.y < 0 || getPutoMapCell(originIndex) != nullptr){
        setPutoPosIndex(_mainPuto.getOriginPuto(), originIndex+PosIndex(0,1));
        fallPuto(_mainPuto.getOptionalPuto());
    }
    else if(optIndex.y < 0 || getPutoMapCell(optIndex) != nullptr){
        setPutoPosIndex(_mainPuto.getOptionalPuto(), optIndex+PosIndex(0,1));
        fallPuto(_mainPuto.getOriginPuto());
    }
    else return;
    //}
    _eventDispatcher->setEnabled(false);
}

void PuzzleField::pushNewPuto(puto::TYPE origin_type,puto::TYPE opt_type){
    _mainPuto.newPuto(origin_type,opt_type);
    
    puto* origin = _mainPuto.getOriginPuto();
    puto* opt = _mainPuto.getOptionalPuto();
    
    origin->setScale((_window_size.width/WIDTH_PUTO_NUM) / origin->getContentSize().width,
                     (_window_size.height/HEIGHT_PUTO_NUM) / origin->getContentSize().height);
    opt->setScale((_window_size.width/WIDTH_PUTO_NUM) / opt->getContentSize().width,
                     (_window_size.height/HEIGHT_PUTO_NUM) / opt->getContentSize().height);

    
    origin->setContentSize( Size(_window_size.width/WIDTH_PUTO_NUM,
                           _window_size.height/HEIGHT_PUTO_NUM) );
    opt->setContentSize( Size(_window_size.width/WIDTH_PUTO_NUM,
                        _window_size.height/HEIGHT_PUTO_NUM) );
    
     CCLOG("%f %f",_window_size.width/WIDTH_PUTO_NUM,origin->getContentSize().width);
    
    origin->setPosition(Vec2(0 * _window_size.width/WIDTH_PUTO_NUM,
                             5 * _window_size.height/HEIGHT_PUTO_NUM));
    opt->setPosition(Vec2(0 * _window_size.width/WIDTH_PUTO_NUM,
                          6 * _window_size.height/HEIGHT_PUTO_NUM));
    
    //origin->setPosition(Vec2(_window_size.width/2,_window_size.height));
    //_mainPuto.getOptionalPuto()->setPosition(Vec2(_windowyuyu_size.width/2,_window_size.height + origin->getContentSize().height));
    
    _status = STATUS::PLAYING;
    _eventDispatcher->setEnabled(true);
    
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

void PuzzleField::rotate(VECTOR N){

    _mainPuto.rotate(N);
    PosIndex index = convertPosIndex(_mainPuto.getOriginPuto()->getPosition());
    VECTOR _opt_pos = _mainPuto.getOptPos();
    
    if(_opt_pos == VECTOR::DOWN && (index.y == 0 || getPutoMapCell(PosIndex(index.x,index.y-1))!=nullptr)){
        _mainPuto.moveDelta(Vec2(0,_mainPuto.getOptionalPuto()->getContentSize().height));
    }
    else if(_opt_pos == VECTOR::LEFT && (index.x == 0 || (getPutoMapCell(PosIndex(index.x-1,index.y))!=nullptr))){
        if(getPutoMapCell(PosIndex(index.x+1,index.y)) == nullptr){
            _mainPuto.moveDelta(Vec2(_mainPuto.getOptionalPuto()->getContentSize().width,0));
        }
        else rotate(N);
    }
    else if(_opt_pos == VECTOR::RIGHT && (index.x == WIDTH_PUTO_NUM-1 || getPutoMapCell(PosIndex(index.x+1,index.y))!=nullptr)){
        if(getPutoMapCell(PosIndex(index.x-1,index.y)) == nullptr){
            _mainPuto.moveDelta(Vec2(-1*_mainPuto.getOptionalPuto()->getContentSize().width,0));
        }
        else rotate(N);
    }
    
}

void PuzzleField::removePuto(){
    std::vector<std::vector<int>> removeMap;
    std::vector<std::vector<puto**>> removeList;
    puto::TYPE target_type;
    
    int removeNo=0;
    
    _status = STATUS::ACTING;
    
    removeMap.resize(HEIGHT_PUTO_NUM);
    for(auto &map: removeMap){
        map.resize(WIDTH_PUTO_NUM);
        std::fill(map.begin(),map.end(),-1);
    }
    
    for(int i=0;i<HEIGHT_PUTO_NUM-1;i++){
        for(int j=0;j<WIDTH_PUTO_NUM-1;j++){
            if(_putoMap[i][j] != nullptr){
                target_type = _putoMap[i][j]->getType();
                if(j!=0 && _putoMap[i][j-1] != nullptr && target_type == _putoMap[i][j-1]->getType()){
                    removeList[removeMap[i][j-1]].push_back(&_putoMap[i][j]);
                    removeMap[i][j]=removeMap[i][j-1];
                    continue;
                }
                if(i!=0 && _putoMap[i-1][j] != nullptr && target_type == _putoMap[i-1][j]->getType()){
                    removeList[removeMap[i-1][j]].push_back(&_putoMap[i][j]);
                    removeMap[i][j]=removeMap[i-1][j];
                    continue;
                }
                /*
                if(j!=WIDTH_PUTO_NUM-1 && _putoMap[i][j+1] != nullptr && target_type == _putoMap[i][j+1]->getType()){
                    removeList[removeMap[i][j+1]].push_back(&_putoMap[i][j]);
                    removeMap[i][j]=removeMap[i][j+1];
                    continue;
                }
                if(i!=HEIGHT_PUTO_NUM-1 && _putoMap[i+1][j] != nullptr && target_type == _putoMap[i+1][j]->getType()){
                    removeList[removeMap[i+1][j]].push_back(&_putoMap[i][j]);
                    removeMap[i][j]=removeMap[i+1][j];
                    continue;
                }
                */
                if(removeMap[i][j]==-1){
                    removeMap[i][j] = removeNo;
                    removeList.push_back(std::vector<puto**>{&_putoMap[i][j]});
                    removeNo++;
                    continue;
                }
            }
        }
    }
    
    removeList.erase(std::remove_if(removeList.begin(),removeList.end(),[](std::vector<puto**> v){return v.size() < 4;}),removeList.end());
    
    if(removeList.size()==0){
        _status = STATUS::WAITING;
        return;
    }
    
    auto fade = FadeTo::create(0.5f, 0.0f);
    auto remove = RemoveSelf::create();
    auto recursive = CallFunc::create(CC_CALLBACK_0(PuzzleField::removePuto, this));
    auto gravity = CallFunc::create(CC_CALLBACK_0(PuzzleField::patchGravity, this));
    auto delay = DelayTime::create(0.5f);
    
    for(auto list: removeList){
        for(auto& _puto: list){
            (*_puto)->runAction(Sequence::create(fade->clone(),remove->clone(), nullptr));
            *_puto=nullptr;
        }
    }
    runAction(Sequence::create(delay,gravity,recursive,nullptr));
}

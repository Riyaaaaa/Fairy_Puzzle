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

PuzzleField* PuzzleField::create(Size window_size,std::array<puto::TYPE,HEIGHT_PUTO_NUM> typeList){
    PuzzleField* pRef = new PuzzleField;
    if(pRef && pRef->init(window_size,typeList)){
        pRef->autorelease();
    }
    else{
        delete pRef;
        pRef = nullptr;
    }
    return pRef;
}

bool PuzzleField::init(Size window_size,std::array<puto::TYPE,HEIGHT_PUTO_NUM> typeList){
    if(!DrawNode::init())return false;
    
    const int SQUARE_POINT_NUM=4;
    
    _puto_size = Size(window_size.width/(WIDTH_PUTO_NUM+1),window_size.height/HEIGHT_PUTO_NUM);
    _window_size = Size(window_size.width-_puto_size.width,window_size.height);
    
    Sprite* sprite = Sprite::create("Alice.png");
    sprite->setScale(_window_size.width/sprite->getContentSize().width,
                     _window_size.height/sprite->getContentSize().height);
    sprite->setAnchorPoint(Vec2(0,0));
    sprite->setPosition(Vec2(0,0));
    sprite->setOpacity(128);
    
    addChild(sprite,Z_BACKGROUND);
    
    Vec2 vecs[]={Vec2(_window_size),Vec2(0,_window_size.height),Vec2(0,0),Vec2(_window_size.width,0)};
    drawPolygon(&vecs[0], SQUARE_POINT_NUM, Color4F(.5f, .6f, 1.0f, 0.5f), 1, Color4F::BLACK);
    
    setContentSize(Size(window_size.width,window_size.height));
    
    _status = STATUS::WAITING;
    
    initPutoList(typeList);
    
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
    
    _eventDispatcher->setEnabled(false);
    
    
    return true;
}

bool PuzzleField::initPutoList(std::array<puto::TYPE,HEIGHT_PUTO_NUM> typeList){
    next_puto_list.resize(HEIGHT_PUTO_NUM);
    
    for(int i=HEIGHT_PUTO_NUM;i>0;i--){
        next_puto_list[HEIGHT_PUTO_NUM - i] = createPuto(typeList[HEIGHT_PUTO_NUM - i]);
        next_puto_list[HEIGHT_PUTO_NUM - i]->setPosition(Vec2(getContentSize().width-getPutoSize().width
                                                              ,getPutoSize().height*(i-1)));
        addChild(next_puto_list[HEIGHT_PUTO_NUM - i],NEXT_DROP+i);
    }
    
    return true;
}

bool PuzzleField::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* unused_event){
    Rect targetBox = this->getBoundingBox();
     _stdPos = touch->getLocation();
    
    if (targetBox.containsPoint(_stdPos))
    {
        SWIPE_PARAM = VECTOR::NONE;
        return true;
    }
    
    return false;
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
            if(isCanMove(_mainPuto,VECTOR::RIGHT))
                _mainPuto.moveDelta(Vec2(_window_size.width/WIDTH_PUTO_NUM,0));
        }
        else rotate(VECTOR::RIGHT);
    }
    else if(SWIPE_PARAM == VECTOR::LEFT){
        if(fabs(_stdPos.y - pos.y) < 50){
            if(isCanMove(_mainPuto, VECTOR::LEFT))
                _mainPuto.moveDelta(Vec2(-1*_window_size.width/WIDTH_PUTO_NUM,0));
        }
        else rotate(VECTOR::LEFT);
    }
}
void PuzzleField::onTouchCancelled(cocos2d::Touch* touch,cocos2d::Event* unused_event){
    onTouchEnded(touch, unused_event);
}

void PuzzleField::popPuto(){
    auto move_up = MoveBy::create(0.5f, Vec2(0,getPutoSize().height));
    auto move_left = MoveBy::create(0.5f, Vec2(-getPutoSize().width,0));
    auto move_down = MoveBy::create(0.5f, Vec2(0,-getPutoSize().height));
    
    CCLOG("%f",getPutoSize().width);
    
    _status = STATUS::ACTING;
    
    next_puto_list[0]->runAction(Sequence::create(move_left,move_down,nullptr));
    next_puto_list[1]->runAction(Sequence::create(move_up,move_left->clone(),nullptr));
    
    _mainPuto.initPuto(next_puto_list[0], next_puto_list[1]);
    
    next_puto_list.pop_front();
    next_puto_list.pop_front();
}

void PuzzleField::pushPuto(puto::TYPE origin_type,puto::TYPE opt_type){
    auto move_up = MoveBy::create(1.0f, Vec2(0,getPutoSize().height*2));
    
    popPuto();
    
    next_puto_list.push_back(createPuto(origin_type));
    next_puto_list.back()->setPosition(Vec2(getContentSize().width-getPutoSize().width
                                            ,getPutoSize().height*(-1)));
    addChild(next_puto_list.back());
    
    next_puto_list.push_back(createPuto(opt_type));
    next_puto_list.back()->setPosition(Vec2(getContentSize().width-getPutoSize().width
                                            ,getPutoSize().height*(-2)));
    
    addChild(next_puto_list.back());
    
    for(int i=0;i<HEIGHT_PUTO_NUM;i++){
        next_puto_list[i]->runAction(move_up->clone());
    }
    //next_puto_list.back()->runAction(MoveBy::create(1.0f, Vec2(0,getPutoSize().height)));
    runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([&](){_status = STATUS::PLAYING; _eventDispatcher->setEnabled(true);}),nullptr));
}

puto* PuzzleField::getPutoMapCell(PosIndex index){
    if(index.x >= WIDTH_PUTO_NUM || index.x < 0 || index.y < 0 )throw nullptr;
    if(index.y >= HEIGHT_PUTO_NUM){
        return nullptr;
    }
    return _putoMap[index.y][index.x];
}

PosIndex PuzzleField::convertPosIndex(Vec2 pos){
    PosIndex index;
    
    index.x = (pos.x+getPutoSize().width/2) / (_window_size.width/WIDTH_PUTO_NUM);
    index.y = pos.y / (_window_size.height/HEIGHT_PUTO_NUM);
    
    if(pos.y - _mainPuto.getOriginPuto()->getContentSize().height/2 < 0) {index.y = -1;}
    
    return index;
}

void PuzzleField::fallPuto(puto* target){
    PosIndex index = convertPosIndex(target->getPosition());
    _status = STATUS::ACTING;
    
    for(int i = 0 ; i <= index.y + 1 ; i++){
        if(index.y == i){
            setPutoPosIndex(target,PosIndex(index.x , i));
            SpriteFallFinished();
            break;
        }
        if(getPutoMapCell(PosIndex(index.x , i)) == nullptr){
            setPutoPosIndexWithAnimation(target,PosIndex(index.x , i));
            break;
        }
    }
}

void PuzzleField::SpriteFallFinished(){
    removePuto();
    _status = STATUS::WAITING;
    if(!isLive()){
        _status = STATUS::LOSED;
        _eventDispatcher->setEnabled(false);
        std::vector<Vec2> vecs({Vec2(getContentSize()),Vec2(0,getContentSize().height),Vec2(0,0),Vec2(getContentSize().width,0)});
        drawPolygon(&vecs[0], 4, Color4F(.0f, .0f, .0f, 0.5f), 0, Color4F::BLACK);
    }
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
    target->setPosition(Vec2(static_cast<int>(index.x) * _window_size.width/WIDTH_PUTO_NUM,
                             static_cast<int>(index.y) * _window_size.height/HEIGHT_PUTO_NUM));
    _putoMap[index.y][index.x] = target;
}

void PuzzleField::setPutoPosIndexWithAnimation(puto* target,PosIndex index){
    Vec2 pos = Vec2(static_cast<int>(index.x) * _window_size.width/WIDTH_PUTO_NUM,
                    static_cast<int>(index.y) * _window_size.height/HEIGHT_PUTO_NUM);
    auto move = MoveTo::create(0.5f, pos);
    auto callback = CallFunc::create(CC_CALLBACK_0(PuzzleField::SpriteFallFinished, this));
    
    target->runAction(Sequence::create(move,callback,nullptr));
    _putoMap[index.y][index.x] = target;
}

void PuzzleField::progress(){
    progressWithMovement(_movement);
}

void PuzzleField::progressWithMovement(int movement){
    PosIndex originIndex = convertPosIndex(_mainPuto.getOriginPuto()->getPosition() + Vec2(0,movement));
    PosIndex optIndex = convertPosIndex(_mainPuto.getOptionalPuto()->getPosition() + Vec2(0,movement));
    
    //if(_mainPuto.isCanAccess()){
    if(originIndex.y < 0 || getPutoMapCell(originIndex) != nullptr){
        setPutoPosIndex(_mainPuto.getOriginPuto(), originIndex+PosIndex(0,1));
        fallPuto(_mainPuto.getOptionalPuto());
    }
    else if(optIndex.y < 0 || getPutoMapCell(optIndex) != nullptr){
        setPutoPosIndex(_mainPuto.getOptionalPuto(), optIndex+PosIndex(0,1));
        fallPuto(_mainPuto.getOriginPuto());
    }
    else {
        _mainPuto.moveDelta(Vec2(0,movement));
        return;
    }
    //}
    _eventDispatcher->setEnabled(false);
}

puto* PuzzleField::createPuto(puto::TYPE type){
    
    puto* new_puto = puto::create(type);
    new_puto->setScale((_window_size.width/WIDTH_PUTO_NUM) / new_puto->getContentSize().width,
                       (_window_size.height/HEIGHT_PUTO_NUM) / new_puto->getContentSize().height);
    
    new_puto->setContentSize( Size(_window_size.width/WIDTH_PUTO_NUM,
                                   _window_size.height/HEIGHT_PUTO_NUM) );
    
    return new_puto;
}

void PuzzleField::rotate(VECTOR N){
    
    _mainPuto.rotate(N);
    PosIndex index = convertPosIndex(_mainPuto.getOriginPuto()->getPosition());
    VECTOR _opt_pos = _mainPuto.getOptPos();
    
    if(_opt_pos == VECTOR::DOWN && (index.y == 0 || getPutoMapCell(PosIndex(index.x,index.y-1))!=nullptr)){
        _mainPuto.moveDelta(Vec2(0,_mainPuto.getOptionalPuto()->getContentSize().height));
    }
    else if(_opt_pos == VECTOR::LEFT && (index.x == 0 || (getPutoMapCell(PosIndex(index.x-1,index.y))!=nullptr))){
        if(index.x != WIDTH_PUTO_NUM-1 && getPutoMapCell(PosIndex(index.x+1,index.y)) == nullptr){
            _mainPuto.moveDelta(Vec2(_mainPuto.getOptionalPuto()->getContentSize().width,0));
        }
        else rotate(N);
    }
    else if(_opt_pos == VECTOR::RIGHT && (index.x == WIDTH_PUTO_NUM-1 || getPutoMapCell(PosIndex(index.x+1,index.y))!=nullptr)){
        if(index.x != 0 && getPutoMapCell(PosIndex(index.x-1,index.y)) == nullptr){
            _mainPuto.moveDelta(Vec2(-1*_mainPuto.getOptionalPuto()->getContentSize().width,0));
        }
        else rotate(N);
    }
    
}

void PuzzleField::removePuto(){
    puto* subject;
    std::vector<std::vector<bool>> removeMap;
    std::vector<PosIndex> from_list;
    std::vector<std::vector<PosIndex>> removeList;
    
    removeMap.resize(HEIGHT_PUTO_NUM);
    for(auto &v: removeMap){
        v.resize(WIDTH_PUTO_NUM);
        std::fill(v.begin(),v.end(),false);
    }
    
    for(int i=0;i<HEIGHT_PUTO_NUM;i++){
        for(int j=0;j<WIDTH_PUTO_NUM;j++){
            from_list.clear();
            if(!removeMap[i][j]){
                subject = _putoMap[i][j];
                if(!subject)continue;
                from_list.push_back(PosIndex(j,i));
                recursive_search(PosIndex(j,i), subject->getType() , from_list);
                for(auto& index: from_list){
                    removeMap[index.y][index.x] = true;
                }
                if(from_list.size() > 3)removeList.push_back(from_list);
            }
        }
    }
    
    if(removeList.size() == 0)return;
    
    //removeList.erase(std::remove_if(removeList.begin(),removeList.end(),[](std::vector<PosIndex> v){return v.size()<4;}),removeList.end());
    
    auto fade = FadeTo::create(0.5f, 0.0f);
    auto remove = RemoveSelf::create();
    auto recursive = CallFunc::create(CC_CALLBACK_0(PuzzleField::removePuto, this));
    auto gravity = CallFunc::create(CC_CALLBACK_0(PuzzleField::patchGravity, this));
    auto delay = DelayTime::create(0.5f);
    
    for(auto &list: removeList){
        for(auto& index: list){
            _putoMap[index.y][index.x]->runAction(Sequence::create(fade->clone(),remove->clone(),NULL));
            _putoMap[index.y][index.x] = nullptr;
        }
    }
    runAction(Sequence::create(delay,gravity,recursive,nullptr));
}

void PuzzleField::recursive_search(PosIndex pos,puto::TYPE type,std::vector<PosIndex>& destination){
    std::vector<PosIndex> remove_list,from_list;
    
    
    if(pos.y != HEIGHT_PUTO_NUM-1 && _putoMap[pos.y+1][pos.x] && type == _putoMap[pos.y+1][pos.x]->getType()){
        remove_list.push_back(PosIndex(pos.x,pos.y+1));
    }
    if(pos.x != WIDTH_PUTO_NUM-1 && _putoMap[pos.y][pos.x+1] && type == _putoMap[pos.y][pos.x+1]->getType()){
        remove_list.push_back(PosIndex(pos.x+1,pos.y));
    }
    if(pos.y != 0 && _putoMap[pos.y-1][pos.x] && type == _putoMap[pos.y-1][pos.x]->getType()){
        remove_list.push_back(PosIndex(pos.x,pos.y-1));
    }
    if(pos.x != 0 && _putoMap[pos.y][pos.x-1] && type == _putoMap[pos.y][pos.x-1]->getType()) {
        remove_list.push_back(PosIndex(pos.x-1,pos.y));
    }
    
    for(auto &index: remove_list){
        if(destination.end() == std::find(destination.begin(), destination.end(), index)){
            destination.push_back(index);
            recursive_search(index, type, destination);
        }
    }
    
    return;
}

bool PuzzleField::isCanMove(connectedPuto& _puto, VECTOR N){
    PosIndex origin_index = convertPosIndex(_puto.getOriginPuto()->getPosition());
    PosIndex opt_index;
    
    puto* origin = nullptr;
    puto* opt = nullptr;
    
    switch (_puto.getOptPos()) {
        case VECTOR::UP:
            opt_index = PosIndex(origin_index.x,origin_index.y+1);
            break;
        case VECTOR::RIGHT:
            opt_index = PosIndex(origin_index.x+1,origin_index.y);
            break;
        case VECTOR::DOWN:
            opt_index = PosIndex(origin_index.x,origin_index.y-1);
            break;
        case VECTOR::LEFT:
            opt_index = PosIndex(origin_index.x-1,origin_index.y);
            break;
        default:
            break;
    }
    
    if(opt_index.x < 0 || opt_index.x >= WIDTH_PUTO_NUM || opt_index.y < 0 || opt_index.y >= HEIGHT_PUTO_NUM ){
        return false;
    }
    
    try{
        
        switch (N) {
            case VECTOR::UP:
                origin = getPutoMapCell(PosIndex(origin_index.x,origin_index.y+1));
                opt = getPutoMapCell(PosIndex(opt_index.x,opt_index.y+1));
                break;
            case VECTOR::RIGHT:
                origin = getPutoMapCell(PosIndex(origin_index.x+1,origin_index.y));
                opt = getPutoMapCell(PosIndex(opt_index.x+1,opt_index.y));
                break;
            case VECTOR::DOWN:
                origin = getPutoMapCell(PosIndex(origin_index.x,origin_index.y-1));
                opt = getPutoMapCell(PosIndex(opt_index.x,opt_index.y-1));
                break;
            case VECTOR::LEFT:
                origin = getPutoMapCell(PosIndex(origin_index.x-1,origin_index.y));
                opt = getPutoMapCell(PosIndex(opt_index.x-1,opt_index.y));
                break;
            default:
                break;
                
        }
        
    }
    
    catch(void* ptr){
        return false;
    }
    
    if(!origin && !opt){
        return true;
    }
    
    return false;
}

bool PuzzleField::isLive(){
    for(int i=0;i<WIDTH_PUTO_NUM;i++){
        if(_putoMap[HEIGHT_PUTO_NUM-1][i] != nullptr){
            return false;
        }
    }
    return true;
}
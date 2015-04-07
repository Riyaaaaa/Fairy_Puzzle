#include "GameScene.h"

USING_NS_CC;

using STATUS = PuzzleField::STATUS;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B::WHITE) )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    std::random_device rnd;
    _engine =std::mt19937(rnd());
    _distribution=std::uniform_int_distribution<>(1,5);
    
    initWindow();
    initPutoList();
    
    schedule(schedule_selector(GameScene::progress),_progressDelay);
    
    return true;
}

bool GameScene::initWindow(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Size mainWindowSize(550,1000);
    
    Sprite* player = Sprite::create("Alice.png");
    _mainWindow = PuzzleField::create(mainWindowSize);
    _mainWindow->setOpacity(0.0);
 
        //drawPolygon(points data,point num,color,outline size,outline color)
    
    player->setAnchorPoint(Vec2(0,0));
    player->setPosition(Vec2(0,0));
    
    player->addChild(_mainWindow,WINDOW);
    addChild(player);
    
    return true;
}

bool GameScene::initPutoList(){
    int _type;
    
    for(int i=0;i<HEIGHT_PUTO_NUM;i++){
        _type = _distribution(_engine);
        next_puto_list.push_back(std::pair<Sprite*,puto::TYPE>(Sprite::create("drop" + std::to_string(_type) + ".png"),
                                                               static_cast<puto::TYPE>(_type)));
        next_puto_list.back().first->setScale(90 / next_puto_list.back().first->getContentSize().width);
        next_puto_list.back().first->setPosition(Vec2(605,90*i +45));
        addChild(next_puto_list.back().first,NEXT_DROP+i);
    }
    
    return true;
}

void GameScene::onEnter(){
    /*Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();*/
    Layer::onEnter();
}


void GameScene::progress(float frame){
    
    switch (_mainWindow->getStatus()) {
        case STATUS::PLAYING:
            _mainWindow->progress();
            break;
        case STATUS::WAITING:
            _mainWindow->pushNewPuto(static_cast<puto::TYPE>(_distribution(_engine)),
                                     static_cast<puto::TYPE>(_distribution(_engine)));
            break;
        case STATUS::ACTING:
            break;
        case STATUS::LOSED:
            //TODO:
            break;
        case STATUS::WON:
            //TODO;
            break;
            
        default:
            break;
    }
}



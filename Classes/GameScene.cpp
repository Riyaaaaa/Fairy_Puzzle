#include "GameScene.h"

USING_NS_CC;

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
    
    initWindow();
    
    std::random_device rnd;
    _engine =std::mt19937(rnd());
    _distribution=std::uniform_int_distribution<>(1,5);
    
    auto touchListner = EventListenerTouchOneByOne::create();
    touchListner->setSwallowTouches(true);
    touchListner->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan,this);
    touchListner->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved,this);
    touchListner->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded,this);
    touchListner->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListner, this);
    
    schedule(schedule_selector(GameScene::progress),_progressDelay);
    
    return true;
}

bool GameScene::initWindow(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Size mainWindowSize(550,1000);
    
    _mainWindow = PuzzleFiled::create(mainWindowSize);
 
        //drawPolygon(points data,point num,color,outline size,outline color)
    
    addChild(_mainWindow,WINDOW);
    
    return true;
}

void GameScene::onEnter(){
    /*Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();*/
    Layer::onEnter();
}


void GameScene::progress(float frame){
    _mainWindow->progress();
}



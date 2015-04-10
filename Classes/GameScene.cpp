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
    
    schedule(schedule_selector(GameScene::progress),_progressDelay);
    
    return true;
}

bool GameScene::initWindow(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Size mainWindowSize(320,500);
    std::array<puto::TYPE, HEIGHT_PUTO_NUM> typeList;
    
    for(int i=0;i<HEIGHT_PUTO_NUM;i++)typeList[i] = static_cast<puto::TYPE>(_distribution(_engine));
    
    //Sprite* player = Sprite::create("Alice.png");
    _window_list.push_back(PuzzleField::create(mainWindowSize,typeList));
    addChild(_window_list.back());

    _window_list.push_back(PuzzleField::create(mainWindowSize,typeList));
    _window_list.back()->setPosition(Vec2(mainWindowSize.width,0));
    addChild(_window_list.back());
    
    _window_list.push_back(PuzzleField::create(mainWindowSize,typeList));
    _window_list.back()->setPosition(Vec2(0,mainWindowSize.height+50));
    addChild(_window_list.back());
    
    _window_list.push_back(PuzzleField::create(mainWindowSize,typeList));
    _window_list.back()->setPosition(Vec2(mainWindowSize.width,mainWindowSize.height+50));
    addChild(_window_list.back());
    
    return true;
}



void GameScene::onEnter(){
    /*Size visibleSize = Director::getInstance()->getVisibleSize();
     Vec2 origin = Director::getInstance()->getVisibleOrigin();*/
    Layer::onEnter();
}


void GameScene::progress(float frame){
    
    for(auto Window: _window_list){
        switch (Window->getStatus()) {
            case STATUS::PLAYING:
                Window->progress();
                break;
            case STATUS::WAITING:
                Window->pushPuto(static_cast<puto::TYPE>(_distribution(_engine)),
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
}


#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "puto.h"
#include "PuzzleField.h"
#include "Structure.h"

class GameScene : public cocos2d::LayerColor
{
public:
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnter();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

protected:
    GameScene() = default;
    virtual ~GameScene() = default;

    std::vector<PuzzleField*> _window_list;
    
    bool initWindow();
    
    void progress            (float);
    void progressWithMovement(float);
    
    void rotateRight();
    void rotateLeft ();
        
    std::mt19937 _engine;
    std::uniform_int_distribution<> _distribution;
    
    float _progressDelay= 0.75f;
    
    const int TAG_ORIGIN=0,TAG_OPT=1;
    
};

#endif // __HELLOWORLD_SCENE_H__

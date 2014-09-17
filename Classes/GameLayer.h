//
//  GameLayer.h
//  BirdTest
//
//  Created by XiongJian on 14-9-11.
//
//

#ifndef __BirdTest__GameLayer__
#define __BirdTest__GameLayer__

#include "Constance.h"
#include "GameScene.h"
#include "BirdSprite.h"
#include "BackgroundLayer.h"
#include "PopLayer.h"
#include "HomeScene.h"

const int TAG_UP_PIPE = 2001;
const int TAG_DOWN_PIPE = 2002;
const int TAG_PASS_PIPE = 2003;
const int TAG_NEW_PIPE = 2004;

const int PIPE_HEIGHT = 320;
const int PIPE_WIDTH = 52;
const int PIPE_DISTANCE = 100;
const int PIPE_INTERVAL = 180;
const int WAIT_DISTANCE = 100;

typedef enum {
    GAME_STATUS_READY,
    GAME_STATUS_START,
    GAME_STATUS_OVER,
    GAME_STATUS_PAUSE
} GameStatus;

#define KEY_FIRST_SCORE "first_score"
#define KEY_SECOND_SCORE "second_score"
#define KEY_THIRD_SCORE "third_score"

class GameLayer : public Layer {
public:
    GameLayer();
    ~GameLayer();
    
    virtual bool init();

    CREATE_FUNC(GameLayer);
    void restart();
    void backToHome();

    void setPhyWorld(PhysicsWorld* pWorld) { world = pWorld; };

    bool onTouchBegan(Touch* touch, Event* pEvent);
    void menuCallBack(Node* pSender);

    void pauseOrResumeGame();

private:
    PhysicsWorld* world;
    BirdSprite* bird;
    Vector<Node*> pipes;
    Label* scoreLabel;
    Sprite* land1;
    Sprite* land2;
    Node* readyNode;

    int score;
    GameStatus gameStatus;

    MenuItemSprite* pauseItem;
    EventListenerPhysicsContact* contactListener;
    EventListenerTouchOneByOne* touchListener;
    SEL_SCHEDULE moveSchedule;
    Vec2 speed;

    void createPipes();
    void movePipesAndLand(float dt);
    bool onContactBegin(PhysicsContact& contact);
    void gameOver();
    void showOverPop();

    int sortScore(int score);
};

#endif /* defined(__BirdTest__GameLayer__) */

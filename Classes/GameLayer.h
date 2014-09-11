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
#include "BirdSprite.h"
#include "BackgroundLayer.h"

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
    GAME_STATUS_OVER
} GameStatus;

class GameLayer : public Layer {
public:
    GameLayer();
    ~GameLayer();
    
    virtual bool init();

    CREATE_FUNC(GameLayer);
    void restart();

    void setPhyWorld(PhysicsWorld* pWorld) { world = pWorld; };

    bool onTouchBegan(Touch* touch, Event* pEvent);

private:
    PhysicsWorld* world;
    Node* groundNode;
    BackgroundLayer* bgLayer;
    BirdSprite* bird;
    Vector<Node*> pipes;

    int score;
    GameStatus gameStatus;

    void createPipes();

    bool onContactBegin(PhysicsContact& contact);
    void gameOver();
    void movePipes(float dt);
};

#endif /* defined(__BirdTest__GameLayer__) */

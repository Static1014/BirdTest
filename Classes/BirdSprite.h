//
//  BirdSprite.h
//  BirdTest
//
//  Created by XiongJian on 14-9-10.
//
//

#ifndef __BirdTest__BirdSprite__
#define __BirdTest__BirdSprite__

#include "Constance.h"

typedef enum {
    ACTION_STATE_IDLE,
    ACTION_STATE_FLY,
    ACTION_STATE_DIE
} ActionState;

typedef enum {
    BIRD_YELLOW = 0,
    BIRD_BLUE,
    BIRD_RED
} BirdType;

const int Tag_shared_bird = 10001;
const int BIRD_RADIUS = 13;

class BirdSprite : public Sprite {
public:
    BirdSprite();
    ~BirdSprite();
    
    static BirdSprite* getInstance();
    virtual bool init();
    bool createBird();
    
    void idle();
    void fly();
    void die();
    void resume();

protected: 
    Animation* createAnimation(int count, float fps);
    
private:
    static BirdSprite* sharedBird;
    BirdType birdType;
    ActionState currentState;

    Action* swingAction;
    Action* idleAction;

};

#endif /* defined(__BirdTest__BirdSprite__) */

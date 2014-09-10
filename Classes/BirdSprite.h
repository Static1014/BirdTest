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

class BirdSprite : public Sprite {
public:
    BirdSprite();
    ~BirdSprite();

    virtual bool init();
    static BirdSprite* getInstance();

    bool createBird();

public:
    void idle();
    void fly();
    void die();

private:
    static BirdSprite* sharedBird;
    ActionState currentState;

    Action* swingAction;
    Action* idleAction;

    Animation* createAnimation(const char* format, int count, float fps);
};

#endif /* defined(__BirdTest__BirdSprite__) */

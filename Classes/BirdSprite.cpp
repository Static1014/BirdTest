//
//  BirdSprite.cpp
//  BirdTest
//
//  Created by XiongJian on 14-9-10.
//
//

#include "BirdSprite.h"

BirdSprite::BirdSprite(){}
BirdSprite::~BirdSprite(){}

BirdSprite* BirdSprite::sharedBird = NULL;

BirdSprite* BirdSprite::getInstance() {
    if (sharedBird == NULL) {
        sharedBird = new BirdSprite();
        if (!sharedBird->init()) {
            delete(sharedBird);
            sharedBird = NULL;
            CCLOG("Create Bird ERROR!!!");
        }
    }
    return sharedBird;
}

bool BirdSprite::init() {
    return true;
}

bool BirdSprite::createBird() {
    string name;
    switch((int)rand()%3) {
        case 0:
            name = "bird0_0";
            birdType = BIRD_RED;
            break;
        case 1:
            name = "bird1_0";
            birdType = BIRD_BLUE;
            break;
        case 2:
            name = "bird2_0";
            birdType = BIRD_YELLOW;
            break;
        default:
            name = "bird2_0";
            birdType = BIRD_YELLOW;
            break;
    }
    
    if (!Sprite::initWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName(name))) {
        return false;
    } else {
        //  When Resume the sharedBird,Reset the Bird Rotation
        this->setRotation(0);

        Animate* animate = Animate::create(createAnimation(3, 10));
        idleAction = RepeatForever::create(animate);

        ActionInterval* up = MoveBy::create(0.4f,Vec2(0, 8));
        ActionInterval* down = up->reverse();
        swingAction = RepeatForever::create(Sequence::create(up, down, NULL));

        return true;
    }
}

void BirdSprite::idle() {
    currentState = ACTION_STATE_IDLE;

    this->runAction(idleAction);
    this->runAction(swingAction);
}

void BirdSprite::fly() {
    currentState = ACTION_STATE_FLY;

    this->getPhysicsBody()->setGravityEnable(true);
}

void BirdSprite::resume() {
    currentState = ACTION_STATE_FLY;

    Animate* animate = Animate::create(createAnimation(3, 10));
    idleAction = RepeatForever::create(animate);
    this->runAction(idleAction);
}

void BirdSprite::die() {
    currentState = ACTION_STATE_DIE;

    this->stopAllActions();
}

Animation* BirdSprite::createAnimation(int count, float fps) {
    Animation* animation = Animation::create();
    animation->setDelayPerUnit(1/fps);

    for (int i = 0; i < count; i++) {
        SpriteFrame* frame = AtlasLoader::getInstance()->getSpriteFrameByName(String::createWithFormat("bird%d_%d", birdType, i)->getCString());
        animation->addSpriteFrame(frame);
    }
    return animation;
}
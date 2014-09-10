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
    string name, nameFormat;
    switch((int)rand()%3) {
        case 0:
            name = "bird0_0";
            nameFormat = "bird0_%d";
            break;
        case 1:
            name = "bird1_0";
            nameFormat = "bird1_%d";
            break;
        case 2:
            name = "bird2_0";
            nameFormat = "bird2_%d";
            break;
        default:
            name = "bird2_0";
            nameFormat = "bird2_%d";
            break;
    }
    
    if (!Sprite::initWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName(name))) {
        return false;
    } else {
        Animate* animate = Animate::create(createAnimation(nameFormat.c_str(), 3, 10));
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

    this->runAction(idleAction);
    this->getPhysicsBody()->setGravityEnable(true);
}

void BirdSprite::die() {
    currentState = ACTION_STATE_DIE;

    this->stopAllActions();
}

Animation* BirdSprite::createAnimation(const char* format, int count, float fps) {
    Animation* animation = Animation::create();
    animation->setDelayPerUnit(1/fps);

    for (int i = 0; i < count; i++) {
        SpriteFrame* frame = AtlasLoader::getInstance()->getSpriteFrameByName(String::createWithFormat(format, i)->getCString());
        animation->addSpriteFrame(frame);
    }
    return animation;
}
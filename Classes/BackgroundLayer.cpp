//
//  BackgroundLayer.cpp
//  BirdTest
//
//  Created by XiongJian on 14-9-10.
//
//

#include "BackgroundLayer.h"

BackgroundLayer::BackgroundLayer(){}
BackgroundLayer::~BackgroundLayer(){}

bool BackgroundLayer::init() {
    if (!Layer::init()) return false;

    time_t t = time(NULL);
    tm* lt = localtime(&t);
    int hour = lt->tm_hour;

    auto bg = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName((hour>=6&&hour<=17)?"bg_day":"bg_night"));
    bg->setPosition(Vec2(WIN_SIZE.width/2,WIN_SIZE.height/2));
    this->addChild(bg);

    land1 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
    land1->setAnchorPoint(Vec2::ZERO);
    land1->setPosition(Vec2::ZERO);
    this->addChild(land1);

    land2 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
    land2->setAnchorPoint(Vec2::ZERO);
    land2->setPosition(Vec2(land1->getContentSize().width-2.0f, 0));
    this->addChild(land2);

    return true;
}

float BackgroundLayer::getLandHeight() {
    return Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"))->getContentSize().height;
}

void BackgroundLayer::scrollLand(float dt) {
    land1->setPositionX(land1->getPositionX()-2.0f);
    land2->setPositionX(land1->getPositionX()+land1->getContentSize().width-2.0f);
    if (land2->getPositionX() == 0) {
        land1->setPositionX(0);
        land2->setPositionX(land1->getPositionX()+land1->getContentSize().width-2.0f);
    }
}

void BackgroundLayer::setScrollLand(bool pScroll) {
    if (pScroll) {
        this->schedule(schedule_selector(BackgroundLayer::scrollLand), 0.01f);
    } else {
        this->unscheduleAllSelectors();
    }
}
//
//  GameScene.cpp
//  BirdTest
//
//  Created by XiongJian on 14-9-11.
//
//

#include "GameScene.h"
#include "GameLayer.h"
#include "BackgroundLayer.h"

GameScene::GameScene(){}
GameScene::~GameScene(){}

bool GameScene::init() {
    if (!Scene::initWithPhysics()) {
        return false;
    }

    this->getPhysicsWorld()->setGravity(Vec2(0, -900));

//    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto bgLayer = BackgroundLayer::create();
    this->addChild(bgLayer);
    bgLayer->setScrollLand(false);

    auto gameLayer = GameLayer::create();
    this->addChild(gameLayer);

    return true;
}

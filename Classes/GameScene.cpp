//
//  GameScene.cpp
//  BirdTest
//
//  Created by XiongJian on 14-9-11.
//
//

#include "GameScene.h"
#include "GameLayer.h"

GameScene::GameScene(){}
GameScene::~GameScene(){}

bool GameScene::init() {
    if (!Scene::initWithPhysics()) { return false; }
    this->getPhysicsWorld()->setGravity(Vec2(0, -900));

    auto gameLayer = GameLayer::create();
    this->addChild(gameLayer);

    return true;
}

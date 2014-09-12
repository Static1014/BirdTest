//
//  GameOverLayer.cpp
//  BirdTest
//
//  Created by XiongJian on 14-9-12.
//
//

#include "GameOverLayer.h"

GameOverLayer::GameOverLayer(){}
GameOverLayer::~GameOverLayer(){}

bool GameOverLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    

    return true;
}

void GameOverLayer::menuCallBack(Ref* pSender) {
    
}
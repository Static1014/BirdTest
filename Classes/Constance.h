//
//  Constance.h
//  BirdTest
//
//  Created by Static on 14-8-31.
//
//

#ifndef HelloTest_Constance_h
#define HelloTest_Constance_h

#include "SimpleAudioEngine.h"
#include "extensions/cocos-ext.h"
#include "AtlasLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
using namespace std;

//  Common Property
#pragma mark -- Common Property ---
#define VISIBLE_SIZE Director::getInstance()->getVisibleSize()
#define VISIBLE_ORIGIN Director::getInstance()->getVisibleOrigin()
#define WIN_SIZE Director::getInstance()->getWinSize()
#define WIN_CENTER Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2)

#define min(X, Y) ((X)<(Y) ? (X) : (Y))
#define max(X, Y) ((X)>(Y) ? (X) : (Y))

#define MOVE_INTERVAL ((CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)?1.5f:2.0f)

#define VOICE_VALUE 0.8

#define KEY_FIRST_SCORE "first_score"
#define KEY_SECOND_SCORE "second_score"
#define KEY_THIRD_SCORE "third_score"

#endif

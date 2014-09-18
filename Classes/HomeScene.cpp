//
//  HomeScene.cpp
//  BirdTest
//
//  Created by XiongJian on 14-9-10.
//
//

#include "HomeScene.h"
#include "BackgroundLayer.h"
#include "BirdSprite.h"
#include "GameScene.h"

enum {
    TagStartBtn,
};

HomeScene::HomeScene(){}
HomeScene::~HomeScene(){}

Scene* HomeScene::createScene() {
    auto scene = Scene::create();
    auto layer = HomeScene::create();
    scene->addChild(layer);

    return scene;
}

bool HomeScene::init() {
    if (!Layer::init()) return false;

    auto bg = BackgroundLayer::create();
    this->addChild(bg);
    bg->setScrollLand(true);

    Sprite *title  = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("title"));
	title->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height*3/4));
	this->addChild(title);

    auto bird = BirdSprite::getInstance();
    bird->createBird();
    bird->setTag(Tag_shared_bird);
    bird->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2));
    bird->idle();
    this->addChild(bird);

    auto startSprite1 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
    auto startSprite2 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
    startSprite2->setPositionY(-5);
    auto startItem = MenuItemSprite::create(startSprite1, startSprite2, CC_CALLBACK_1(HomeScene::menuCallBack, this));
    auto menu = Menu::create(startItem, NULL);
    startItem->setTag(TagStartBtn);
    menu->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/4+20));
    this->addChild(menu);

    auto copyRight = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("brand_copyright"));
    copyRight->setPosition(Vec2(WIN_SIZE.width/2, copyRight->getContentSize().height));
    this->addChild(copyRight);

    return true;
}

void HomeScene::menuCallBack(Ref* pSender) {
    Node* btn = (Node*)pSender;
    if (btn->getTag() == TagStartBtn) {
        SimpleAudioEngine::getInstance()->playEffect("sounds/sfx_swooshing.ogg");
        this->removeChildByTag(Tag_shared_bird);
        auto gameScene = GameScene::create();
        auto gameSceneTrans = TransitionFadeTR::create(0.6, gameScene);
        Director::getInstance()->replaceScene(gameSceneTrans);
    }
}
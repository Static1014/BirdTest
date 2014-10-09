//
//  HomeScene.cpp
//  BirdTest
//
//  Created by XiongJian on 14-9-10.
//
//

#include "HomeScene.h"

enum {
    TagStartBtn,
    TagSortBtn,
    TagOkBtnInSortPop
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
    startItem->setScale(0.8);
    startItem->setPosition(Vec2(WIN_SIZE.width*2/7, WIN_SIZE.height/4));

    auto sortSprite1 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_score"));
    auto sortSprite2 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_score"));
    sortSprite2->setPositionY(-5);
    auto sortItem = MenuItemSprite::create(sortSprite1, sortSprite2, CC_CALLBACK_1(HomeScene::menuCallBack, this));
    sortItem->setScale(0.8);
    sortItem->setPosition(Vec2(WIN_SIZE.width*5/7, WIN_SIZE.height/4));


    menu = Menu::create(startItem, sortItem, NULL);
    startItem->setTag(TagStartBtn);
    sortItem->setTag(TagSortBtn);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    auto copyRight = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("brand_copyright"));
    copyRight->setPosition(Vec2(WIN_SIZE.width/2, copyRight->getContentSize().height));
    this->addChild(copyRight);

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyReleased = CC_CALLBACK_2(HomeScene::onKeyReleased, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    return true;
}

void HomeScene::onKeyReleased(EventKeyboard::KeyCode code, Event* pEvent) {
    if (code == EventKeyboard::KeyCode::KEY_BACK) {
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo minfo;
        if (JniHelper::getStaticMethodInfo(minfo,
                                           CLASS_NAME,
                                           "showExitDialog",
                                           "()V")) {
            minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        }
        #endif
    }
}

void HomeScene::menuCallBack(Ref* pSender) {
    Node* btn = (Node*)pSender;
    if (btn->getTag() == TagStartBtn) {

        SimpleAudioEngine::getInstance()->playEffect("sounds/sfx_swooshing.ogg");
        SimpleAudioEngine::getInstance()->setEffectsVolume(VOICE_VALUE);
        this->removeChildByTag(Tag_shared_bird);
        auto gameScene = GameScene::create();
        auto gameSceneTrans = TransitionFadeTR::create(0.6, gameScene);
        Director::getInstance()->replaceScene(gameSceneTrans);

    } else if (btn->getTag() == TagSortBtn) {

        menu->setEnabled(false);

        auto sortPop = PopListLayer::create();
        sortPop->setBackground("bg_night", 0.75);
        sortPop->initTableView();
        sortPop->setCallBackFunc(this, callfuncN_selector(HomeScene::removeTouchListener));
        sortPop->addButton("button_ok", "", "", TagOkBtnInSortPop);
        
        this->addChild(sortPop, 2);
    }
}

void HomeScene::removeTouchListener(Node* pNode) {
    if (pNode->getTag() == TagOkBtnInSortPop) {
        menu->setEnabled(true);
    }
}
//
//  GameLayer.cpp
//  BirdTest
//
//  Created by XiongJian on 14-9-11.
//
//

#include "GameLayer.h"

GameLayer* GameLayer::sharedGameLayer = NULL;
GameStatus GameLayer::gameStatus = GAME_STATUS_READY;

GameLayer::GameLayer(){
    sharedGameLayer = this;
}
GameLayer::~GameLayer(){}

GameLayer * GameLayer::getSharedGameLayer(){
    if (sharedGameLayer == NULL) {
        sharedGameLayer = new GameLayer();
        sharedGameLayer->init();
    }
    return sharedGameLayer;
}

bool GameLayer::init() {
    if (!Layer::init()){
       return false;
    }

    gameStatus = GAME_STATUS_READY;
    score = 0;

    auto groundNode = Node::create();
    auto groundBody = PhysicsBody::create();
    groundBody->addShape(PhysicsShapeBox::create(Size(288, BackgroundLayer::getLandHeight())));
    groundBody->setDynamic(false);
    groundBody->setLinearDamping(0.0f);
    groundBody->setCategoryBitmask(1);
    groundBody->setCollisionBitmask(1);
    groundBody->setContactTestBitmask(1);
    groundNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    groundNode->setPosition(Vec2(144, BackgroundLayer::getLandHeight()/2));
    groundNode->setPhysicsBody(groundBody);
    this->addChild(groundNode, 0);

    auto skyNode = Node::create();
    auto skyBody = PhysicsBody::create();
    skyBody->addShape(PhysicsShapeEdgeBox::create(Size(288, 1)));
    skyBody->setDynamic(false);
    skyBody->setLinearDamping(0.0f);
    skyBody->setCategoryBitmask(1);
    skyBody->setCollisionBitmask(1);
    skyBody->setContactTestBitmask(1);
    skyNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    skyNode->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height));
    skyNode->setPhysicsBody(skyBody);
    this->addChild(skyNode, 0);

    readyNode = Node::create();
    auto readySprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("text_ready"));
    readySprite->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2+readySprite->getContentSize().height));
    auto tutorialSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("tutorial"));
    tutorialSprite->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2-tutorialSprite->getContentSize().height/2));
    readyNode->addChild(readySprite);
    readyNode->addChild(tutorialSprite);
    this->addChild(readyNode, 2);

    bird = BirdSprite::getInstance();
    bird->createBird();
    PhysicsBody* body = PhysicsBody::create();
    body->addShape(PhysicsShapeCircle::create(BIRD_RADIUS));
    body->setDynamic(true);
    body->setLinearDamping(0.0f);
    body->setGravityEnable(false);
    body->setCategoryBitmask(1);
    body->setCollisionBitmask(1);
    body->setContactTestBitmask(1);
    bird->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bird->setPosition(WIN_CENTER);
    bird->setPhysicsBody(body);
    bird->idle();
    bird->setRotation(0);

    scoreLabel = Label::createWithSystemFont("score:0", "fonts/Marker Felt.ttf", 24);
    scoreLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    scoreLabel->setPosition(Vec2(10, WIN_SIZE.height-5));
    this->addChild(scoreLabel, 3);

    auto pauseNormalSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_pause"));
    auto pauseSelectedSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_pause"));
    pauseSelectedSprite->setScale(0.8);
    pauseItem = MenuItemSprite::create(pauseNormalSprite, pauseSelectedSprite, CC_CALLBACK_0(GameLayer::pauseOrResumeGame, this));
    pauseItem->setPosition(Vec2(WIN_SIZE.width-25, WIN_SIZE.height-25));
    auto menu = Menu::create(pauseItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 3);
    pauseItem->setEnabled(false);
    pauseItem->setScale(0);


    land1 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
    land1->setAnchorPoint(Vec2::ZERO);
    land1->setPosition(Vec2::ZERO);
    this->addChild(land1, 3);

    land2 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
    land2->setAnchorPoint(Vec2::ZERO);
    land2->setPosition(Vec2(land1->getContentSize().width-2.0f, 0));
    this->addChild(land2, 3);

    contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(false);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool GameLayer::onContactBegin(PhysicsContact& contact) {
    SimpleAudioEngine::getInstance()->playEffect("sounds/sfx_hit.ogg");
    gameOver();
    return true;
}

void GameLayer::pauseOrResumeGame() {
    string btnImage;
    if (gameStatus != GAME_STATUS_PAUSE) {
        gameStatus = GAME_STATUS_PAUSE;
        btnImage = "button_resume";

        this->getEventDispatcher()->removeEventListener(contactListener);
//        this->getEventDispatcher()->removeEventListener(touchListener);
        this->unschedule(moveSchedule);

        speed = bird->getPhysicsBody()->getVelocity();
        bird->getPhysicsBody()->setVelocity(Vec2(0, 0));
        bird->getPhysicsBody()->setEnable(false);
        bird->stopAllActions();

        // show ads
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo minfo;

        if (JniHelper::getStaticMethodInfo(minfo,
                                           CLASS_NAME,
                                           "isConnectionAvaliable",
                                           "()Z")) {
            if (minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID)) {
                pauseItem->setEnabled(false);
                JniMethodInfo minfoi;
                if (JniHelper::getStaticMethodInfo(minfoi,
                                                   CLASS_NAME,
                                                   "showInterAd",
                                                   "()V")) {
                    minfoi.env->CallStaticVoidMethod(minfoi.classID, minfoi.methodID);
                    minfoi.env->DeleteLocalRef(minfoi.classID);
                }
            } else {
                CCLOG("Network is unavailable!");
            }
            minfo.env->DeleteLocalRef(minfo.classID);
        }
        #endif
    } else {
        btnImage = "button_pause";

        timeNum = 3;
        showResumeTimer();
        pauseItem->setEnabled(false);
    }
    
    auto pauseNormalSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName(btnImage));
    auto pauseSelectedSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName(btnImage));
    pauseSelectedSprite->setScale(0.8);
    pauseItem->setNormalImage(pauseNormalSprite);
    pauseItem->setSelectedImage(pauseSelectedSprite);
}

void GameLayer::resumeGame() {
    gameStatus = GAME_STATUS_START;

    contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    //    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    this->schedule(moveSchedule, 0.01f);
    bird->resume();
    bird->getPhysicsBody()->setEnable(true);
    bird->getPhysicsBody()->setVelocity(speed);

    pauseItem->setEnabled(true);
}

void GameLayer::showResumeTimer() {
//    string name = String::createWithFormat("number_socre_0%d", timeNum)->getCString();
    string name;
    switch(timeNum) {
        case 3:
            name = "number_score_03";
            break;

        case 2:
            name = "number_score_02";
            break;

        case 1:
            name = "number_score_01";
            break;

        default:
            name = "number_score_01";
            break;
    }
    if (timeNum == 3) {
        timeSprite3 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName(name));
        timeSprite3->setPosition(WIN_CENTER);
        timeSprite3->setOpacity(80);
        this->addChild(timeSprite3, 4);
    } else {
        timeSprite3->setSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName(name));
    }
    timeSprite3->setScale(0);

    auto aScale = ScaleTo::create(0.8, 2.4);
    auto removeCall = CallFuncN::create(CC_CALLBACK_1(GameLayer::changeTimeSprite, this));
    auto resumeCall = CallFunc::create(CC_CALLBACK_0(GameLayer::resumeGame, this));
    timeSprite3->runAction(Sequence::create(aScale, removeCall, (timeNum == 1)? resumeCall:NULL, NULL));
}

void GameLayer::changeTimeSprite(Node* pNode) {
    if (timeNum == 1) {
        pNode->removeFromParentAndCleanup(true);
    } else {
        timeNum--;
        showResumeTimer();
    }
}

// Java->cpp
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C" {
    void Java_org_cocos2dx_lib_Cocos2dxActivity_resumeGame(JNIEnv * jenv,jobject jobject, jint succ){
        if (succ == 1) {
            //  关闭广告，直接进行开始倒计时
            GameLayer::getSharedGameLayer()->pauseOrResumeGame();
        } else {
            //  广告显示失败，让pauseItem可点
            GameLayer::getSharedGameLayer()->pauseItem->setEnabled(true);
        }
    }
}
#endif

bool GameLayer::onTouchBegan(Touch* touch, Event* pEvent) {
    if (gameStatus == GAME_STATUS_OVER || gameStatus == GAME_STATUS_PAUSE) {
        return false;
    }

	SimpleAudioEngine::getInstance()->playEffect("sounds/sfx_wing.ogg");
    if (gameStatus == GAME_STATUS_READY) {
        gameStatus = GAME_STATUS_START;
        this->removeChild(readyNode);
        this->addChild(bird, 2);
        bird->setPosition(WIN_CENTER);
        bird->fly();
        createPipes();

        moveSchedule = schedule_selector(GameLayer::movePipesAndLand);
        this->schedule(moveSchedule, 0.01f);

        pauseItem->setScale(1);
        pauseItem->setEnabled(true);
    } else {
        bird->getPhysicsBody()->setVelocity(Vect(0,260));
    }

    return true;
}

void GameLayer::createPipes() {
    pipes.clear();
    for (int i = 0; i < 2; i++) {
        Sprite* pipeDown = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("pipe_down"));
        Sprite* pipeUp = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("pipe_up"));
        Node* pipe = Node::create();
        pipe->addChild(pipeDown, 0, TAG_DOWN_PIPE);
        pipe->addChild(pipeUp, 0, TAG_UP_PIPE);
        pipeUp->setPosition(Vec2(0, PIPE_HEIGHT + PIPE_DISTANCE));

        auto pipeBody = PhysicsBody::create();
        pipeBody->addShape(PhysicsShapeBox::create(pipeUp->getContentSize(),PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(0, PIPE_HEIGHT + PIPE_DISTANCE)));
        pipeBody->addShape(PhysicsShapeBox::create(pipeDown->getContentSize()));
        pipeBody->setDynamic(false);
        pipeBody->setCategoryBitmask(1);
        pipeBody->setCollisionBitmask(1);
        pipeBody->setContactTestBitmask(1);
        pipe->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        pipe->setPosition(Vec2(WIN_SIZE.width + i*PIPE_INTERVAL + WAIT_DISTANCE, rand()%(int)(2*PIPE_HEIGHT + PIPE_DISTANCE - WIN_SIZE.height)));
        pipe->setTag(TAG_NEW_PIPE);

        pipe->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        pipe->setPhysicsBody(pipeBody);

        this->addChild(pipe, 2);
        pipes.pushBack(pipe);
    }
}

void GameLayer::movePipesAndLand(float dt) {
    float speed = bird->getPhysicsBody()->getVelocity().y;
    bird->setRotation(min(max(-60, (speed*0.2 + 60)), 30));

    land1->setPositionX(land1->getPositionX()-MOVE_INTERVAL);
    land2->setPositionX(land1->getPositionX()+land1->getContentSize().width-2.0f);
    if (land2->getPositionX() <= 0) {
        land1->setPositionX(0);
        land2->setPositionX(land1->getPositionX()+land1->getContentSize().width-2.0f);
    }

    for (auto pipe : pipes) {
        pipe->setPositionX(pipe->getPositionX() - MOVE_INTERVAL);

        if (pipe->getTag() == TAG_NEW_PIPE && ( pipe->getPositionX()+PIPE_WIDTH/2 <= bird->getPositionX()-BIRD_RADIUS)) {
            pipe->setTag(TAG_PASS_PIPE);
            score++;
            scoreLabel->setString(String::createWithFormat("score:%d", score)->getCString());
            SimpleAudioEngine::getInstance()->playEffect("sounds/sfx_point.ogg");

            Sprite* up = (Sprite*)pipe->getChildByTag(TAG_UP_PIPE);
            up->setDisplayFrame(AtlasLoader::getInstance()->getSpriteFrameByName("pipe2_up"));
            up->setRotation(180);
            Sprite* down = (Sprite*)pipe->getChildByTag(TAG_DOWN_PIPE);
            down->setDisplayFrame(AtlasLoader::getInstance()->getSpriteFrameByName("pipe2_down"));

        } else {
            if (pipe->getPositionX()+PIPE_WIDTH <= 0) {
                pipe->setPositionX(WIN_SIZE.width+PIPE_WIDTH/2);
                pipe->setTag(TAG_NEW_PIPE);
                pipe->setPositionY(rand()%(int)(2*PIPE_HEIGHT + PIPE_DISTANCE - WIN_SIZE.height));

                Sprite* up = (Sprite*)pipe->getChildByTag(TAG_UP_PIPE);
                up->setDisplayFrame(AtlasLoader::getInstance()->getSpriteFrameByName("pipe_down"));
                Sprite* down = (Sprite*)pipe->getChildByTag(TAG_DOWN_PIPE);
                down->setDisplayFrame(AtlasLoader::getInstance()->getSpriteFrameByName("pipe_down"));
            }
        }
    }
}

void GameLayer::gameOver() {
    pauseItem->setScale(0);

    gameStatus = GAME_STATUS_OVER;
    this->unscheduleAllSelectors();
    bird->die();
    pauseItem->setEnabled(false);

    this->getEventDispatcher()->removeAllEventListeners();

    SimpleAudioEngine::getInstance()->playEffect("sounds/sfx_die.ogg");
    showOverPop();
}

void GameLayer::showOverPop() {
    string str = String::createWithFormat("medals_%d",3-sortScore(score))->getCString();
    auto sortSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName(str));

    auto currentScoreLabel = Label::createWithSystemFont(String::createWithFormat("%d", score)->getCString(), "fonts/Marker Felt.ttf", 20);
    currentScoreLabel->setColor(Color3B::RED);
    currentScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);

    string bestScoreStr = String::createWithFormat("%d", UserDefault::getInstance()->getIntegerForKey(KEY_FIRST_SCORE))->getCString();
    auto bestScoreLabel = Label::createWithSystemFont(bestScoreStr, "fonts/Marker Felt.ttf", 20);
    bestScoreLabel->setColor(Color3B::RED);
    bestScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);

    auto overPop = PopLayer::create();
    overPop->setBackground("score_panel");
    overPop->setCallBackFunc(this, callfuncN_selector(GameLayer::menuCallBack));
    overPop->addButton("button_menu", "", "", TAG_BACK_HOME_BUTTON);
    overPop->addButton("button_ok", "", "", TAG_RESTART_BUTTON);

    overPop->addCustomNode(sortSprite, Vec2(55, 60));
    overPop->addCustomNode(currentScoreLabel, Vec2(210, 80));
    overPop->addCustomNode(bestScoreLabel, Vec2(210, 40));

    this->addChild(overPop, 4);
}

int GameLayer::sortScore(int cur) {
    int first = UserDefault::getInstance()->getIntegerForKey(KEY_FIRST_SCORE);
    int second = UserDefault::getInstance()->getIntegerForKey(KEY_SECOND_SCORE);
    int third = UserDefault::getInstance()->getIntegerForKey(KEY_THIRD_SCORE);

//    CCLOG("%d----%d----%d---%d", first, second, third, cur);

    if (cur == first) {
        return 0;
    } else if (cur == second) {
        return 1;
    } else if (cur == third) {
        return 2;
    } else if (cur > first) {
        third = second;
        second = first;
        first = cur;

        UserDefault::getInstance()->setIntegerForKey(KEY_FIRST_SCORE, first);
        UserDefault::getInstance()->setIntegerForKey(KEY_SECOND_SCORE, second);
        UserDefault::getInstance()->setIntegerForKey(KEY_THIRD_SCORE, third);
        UserDefault::getInstance()->flush();
        return 0;
    } else if (cur > second) {
        third = second;
        second = cur;

        UserDefault::getInstance()->setIntegerForKey(KEY_SECOND_SCORE, second);
        UserDefault::getInstance()->setIntegerForKey(KEY_THIRD_SCORE, third);
        UserDefault::getInstance()->flush();
        return 1;
    } else if (cur > third) {
        third = cur;

        UserDefault::getInstance()->setIntegerForKey(KEY_THIRD_SCORE, third);
        UserDefault::getInstance()->flush();
        return 2;
    } else {
        return 3;
    }
}

void GameLayer::menuCallBack(Node* pSender) {
    //    this->removeAllChildrenWithCleanup(true);
	SimpleAudioEngine::getInstance()->playEffect("sounds/sfx_swooshing.ogg");

    if (pSender->getTag() == TAG_BACK_HOME_BUTTON) {
        backToHome();
    } else if (pSender->getTag() == TAG_RESTART_BUTTON) {
        restart();
    }
}

void GameLayer::restart() {
    auto game = GameScene::create();
    auto gameTrans = TransitionFlipAngular::create(0.6, game);
    Director::getInstance()->replaceScene(gameTrans);
}

void GameLayer::backToHome() {
    this->removeChild(bird);
    auto home = HomeScene::createScene();
    auto homeTrans = TransitionFadeTR::create(0.7, home);
    Director::getInstance()->replaceScene(homeTrans);
}
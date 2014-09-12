//
//  GameLayer.cpp
//  BirdTest
//
//  Created by XiongJian on 14-9-11.
//
//

#include "GameLayer.h"

GameLayer::GameLayer(){}
GameLayer::~GameLayer(){}

bool GameLayer::init() {
    if (!Layer::init()) return false;

    gameStatus = GAME_STATUS_READY;
    score = 0;

    bgLayer = BackgroundLayer::create();
    this->addChild(bgLayer, 0);
    bgLayer->setScrollLand(false);
//    bgLayer->setScale(0);
//    bgLayer->runAction(ScaleTo::create(2.0f, 1));

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
    bird->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2));
    bird->setPhysicsBody(body);
    bird->idle();
    bird->setRotation(0);

    scoreLabel = Label::createWithSystemFont("score:0", "fonts/Marker Felt.ttf", 24);
    scoreLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    scoreLabel->setPosition(Vec2(10, WIN_SIZE.height-5));
    this->addChild(scoreLabel, 3);

    auto restartItem = MenuItemFont::create("restart", CC_CALLBACK_0(GameLayer::restart, this));
    restartItem->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    restartItem->setFontSizeObj(24);
    auto menu = Menu::create(restartItem, NULL);
    menu->setPosition(Vec2(WIN_SIZE.width-10, WIN_SIZE.height-5));
    this->addChild(menu, 3);

    land1 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
    land1->setAnchorPoint(Vec2::ZERO);
    land1->setPosition(Vec2::ZERO);
    this->addChild(land1, 3);

    land2 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
    land2->setAnchorPoint(Vec2::ZERO);
    land2->setPosition(Vec2(land1->getContentSize().width-2.0f, 0));
    this->addChild(land2, 3);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool GameLayer::onContactBegin(PhysicsContact& contact) {
    gameOver();
    return true;
}


bool GameLayer::onTouchBegan(Touch* touch, Event* pEvent) {
    if (gameStatus == GAME_STATUS_OVER) {
        return false;
    }

    if (gameStatus == GAME_STATUS_READY) {
        gameStatus = GAME_STATUS_START;
        this->removeChild(readyNode);
        this->addChild(bird, 2);
        bird->fly();
        createPipes();
        this->schedule(schedule_selector(GameLayer::movePipes), 0.01f);
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
//        pipeBody->setCategoryBitmask(1);
//        pipeBody->setCollisionBitmask(1);
//        pipeBody->setContactTestBitmask(1);
        pipe->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        pipe->setPosition(Vec2(WIN_SIZE.width + i*PIPE_INTERVAL + WAIT_DISTANCE, rand()%(int)(2*PIPE_HEIGHT + PIPE_DISTANCE - WIN_SIZE.height)));
        pipe->setTag(TAG_NEW_PIPE);

        pipe->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        pipe->setPhysicsBody(pipeBody);

        this->addChild(pipe, 2);
        pipes.pushBack(pipe);
    }
}

void GameLayer::movePipes(float dt) {
    float speed = bird->getPhysicsBody()->getVelocity().y;
    bird->setRotation(min(max(-60, (speed*0.2 + 60)), 30));

    land1->setPositionX(land1->getPositionX()-2.0f);
    land2->setPositionX(land1->getPositionX()+land1->getContentSize().width-2.0f);
    if (land2->getPositionX() == 0) {
        land1->setPositionX(0);
        land2->setPositionX(land1->getPositionX()+land1->getContentSize().width-2.0f);
    }

    for (auto pipe : pipes) {
        pipe->setPositionX(pipe->getPositionX() - 2.0f);

        if (pipe->getTag() == TAG_NEW_PIPE && ( pipe->getPositionX()+PIPE_WIDTH/2 <= bird->getPositionX()-BIRD_RADIUS)) {
            pipe->setTag(TAG_PASS_PIPE);
            score++;
            scoreLabel->setString(String::createWithFormat("score:%d", score)->getCString());

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
    gameStatus = GAME_STATUS_OVER;
    bgLayer->setScrollLand(false);
    this->unscheduleAllSelectors();
    bird->die();
}

void GameLayer::restart() {
    this->removeAllChildrenWithCleanup(true);
    init();
}

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

    bgLayer = BackgroundLayer::create();
    this->addChild(bgLayer);
    bgLayer->setScrollLand(true);

    groundNode = Node::create();
    auto groundBody = PhysicsBody::create();
    groundBody->addShape(PhysicsShapeBox::create(Size(288, BackgroundLayer::getLandHeight())));
    groundBody->setDynamic(false);
    groundBody->setLinearDamping(0.0f);
    groundNode->setPhysicsBody(groundBody);
    groundNode->setPosition(Vec2(144, BackgroundLayer::getLandHeight()/2));
    this->addChild(groundNode);

    bird = BirdSprite::getInstance();
    bird->createBird();
    bird->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2));
    PhysicsBody* body = PhysicsBody::create();
    body->addShape(PhysicsShapeCircle::create(BIRD_RADIUS));
    body->setDynamic(true);
    body->setLinearDamping(0.0f);
    body->setGravityEnable(false);
    body->setCategoryBitmask(1);
    body->setCollisionBitmask(1);
    body->setContactTestBitmask(1);
    bird->setPhysicsBody(body);

    bird->idle();
    this->addChild(bird);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    gameStatus = GAME_STATUS_READY;
    score = 0;

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
        bird->fly();
        createPipes();
        this->schedule(schedule_selector(GameLayer::movePipes), 0.01f);
    } else {
        bird->getPhysicsBody()->setVelocity(Vect(0,260));
    }

    return true;
}

void GameLayer::createPipes() {
    for (int i = 0; i < 2; i++) {
        Sprite* pipeDown = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("pipe_down"));
        Sprite* pipeUp = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("pipe_up"));
        Node* pipe = Node::create();
        pipe->addChild(pipeDown, 0, TAG_UP_PIPE);
        pipe->addChild(pipeUp, 0, TAG_DOWN_PIPE);
        pipeUp->setPosition(Vec2(0, PIPE_HEIGHT + PIPE_DISTANCE));

        pipe->setAnchorPoint(Vec2::ZERO);
        pipe->setPosition(Vec2(WIN_SIZE.width + i*PIPE_INTERVAL + WAIT_DISTANCE, rand()%(int)(2*PIPE_HEIGHT + PIPE_DISTANCE - WIN_SIZE.height)));
        auto pipeBody = PhysicsBody::create();
        pipeBody->addShape(PhysicsShapeBox::create(pipeUp->getContentSize(),PHYSICSSHAPE_MATERIAL_DEFAULT, Point(0, PIPE_HEIGHT + PIPE_DISTANCE)));
        pipeBody->addShape(PhysicsShapeBox::create(pipeDown->getContentSize()));
        pipeBody->setDynamic(false);
        pipeBody->setTag(TAG_NEW_PIPE);
        pipeBody->setCategoryBitmask(1);
        pipeBody->setCollisionBitmask(1);
        pipeBody->setContactTestBitmask(1);
        pipe->setPhysicsBody(pipeBody);

        this->addChild(pipe);
        pipes.pushBack(pipe);
    }
}

void GameLayer::movePipes(float dt) {
    for (auto pipe : pipes) {
        pipe->setPositionX(pipe->getPositionX() - 2.0f);
        if (pipe->getTag() == TAG_NEW_PIPE && pipe->getPositionX() < bird->getPositionX()) {
            pipe->setTag(TAG_PASS_PIPE);
            score++;
            CCLOG("------%d", score);
        } else {
            if (pipe->getPositionX() < PIPE_WIDTH) {
                pipe->setTag(TAG_NEW_PIPE);
                pipe->setPositionX(WIN_SIZE.width);
                pipe->setPositionY(rand()%(int)(2*PIPE_HEIGHT + PIPE_DISTANCE - WIN_SIZE.height));
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

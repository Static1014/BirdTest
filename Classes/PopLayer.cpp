//
//  TestLayer.cpp
//  PopLayer
//
//  Created by XiongJian on 14-9-16.
//
//

#include "PopLayer.h"

PopLayer::PopLayer(){}
PopLayer::~PopLayer(){}

bool PopLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    auto bg = LayerColor::create(Color4B(0, 0, 0, 0));
    bg->setOpacity(80);
    bg->setContentSize(WIN_SIZE);
    bg->setPosition(Vec2::ZERO);
    this->addChild(bg);
    
    setCallBack = false;
    defaultScale = 1.0;

    return true;
}

void PopLayer::menuCallback(Ref* pSender) {
    Node* btn = static_cast<Node*>(pSender);

    if (setCallBack && mCallFuncN && mCallBackListener) {
        (mCallBackListener->*mCallFuncN)(btn);
    }
    this->removeFromParentAndCleanup(true);
}

void PopLayer::setBackground(string bgImage, float scale) {
    defaultScale = scale;

    mBackground = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName(bgImage));
    mBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    mBackground->setPosition(WIN_CENTER);
    this->addChild(mBackground);
    mBackground->setScale(0);

    mMenu = Menu::create();
    mMenu->setPosition(Vec2::ZERO);
    mBackground->addChild(mMenu);

}

void PopLayer::setTitle(string title) {

}

void PopLayer::addCustomNode(Node* node, Vec2 location) {
    mBackground->addChild(node);
    node->setPosition(location);
}

void PopLayer::setCallBackFunc(Ref* target, SEL_CallFuncN callFunc) {
    setCallBack = true;
    mCallBackListener = target;
    mCallFuncN = callFunc;
}

void PopLayer::addButton(string normalImage, string selectImage, string title, int tag) {
    auto normalSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName(normalImage));

    auto selectedSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName(("" == selectImage)?normalImage:selectImage));
    selectedSprite->setScale(("" == selectImage)?0.92:1.0);

    auto item = MenuItemSprite::create(normalSprite, selectedSprite, CC_CALLBACK_1(PopLayer::menuCallback, this));
    item->setTag(tag);

    mMenu->addChild(item);
}

void PopLayer::onEnter() {
    Layer::onEnter();

    float bgWidth = mBackground->getContentSize().width;
    int btnCount = mMenu->getChildrenCount();
    Node* temp = mMenu->getChildren().at(0);
    float btnWidth = temp->getContentSize().width;

    for (int i = 0; i < btnCount; i++) {
        auto btn = mMenu->getChildren().at(i);
        btn->setPosition(Vec2(btnWidth/2 + i*btnWidth + (i+1)*(bgWidth-btnWidth*btnCount)/(btnCount+1), 10));
    }

    Action* action = Sequence::create(ScaleTo::create(0.20, defaultScale + 0.06),
                                      ScaleTo::create(0.08, defaultScale - 0.05),
                                      ScaleTo::create(0.08, defaultScale), NULL);
    mBackground->runAction(action);
}
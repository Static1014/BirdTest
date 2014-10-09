//
//  PopListLayer.cpp
//  BirdTest
//
//  Created by XiongJian on 14-9-25.
//
//

#include "PopListLayer.h"

enum TagInCell {
    TagItemBg,
    TagItemSortSprite,
    TagItemScore,
    TagItemBtn,
};

PopListLayer::PopListLayer(){}
PopListLayer::~PopListLayer(){}

bool PopListLayer::init() {
    if (!PopLayer::init()) {
        return false;
    }

    return true;
}

void PopListLayer::initTableView() {
    if (mBackground != NULL) {
        auto itemBg = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("score_panel"));

        tableView = TableView::create(this, Size(itemBg->getContentSize().width, itemBg->getContentSize().height*2));
        tableView->setDelegate(this);
        tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
        tableView->setPosition(Vec2((WIN_SIZE.width-itemBg->getContentSize().width)/2, itemBg->getContentSize().height));
        mBackground->addChild(tableView, 1);
        tableView->reloadData();
    }
}

void PopListLayer::onEnter() {
    PopLayer::onEnter();

}

void PopListLayer::tableCellUnhighlight(TableView* table, TableViewCell* cell) {
    Sprite* itemBg = (Sprite*)cell->getChildByTag(TagItemBg);
    itemBg->setOpacity(255);
}
void PopListLayer::tableCellHighlight(TableView* table, TableViewCell* cell) {
    Sprite* itemBg = (Sprite*)cell->getChildByTag(TagItemBg);
    itemBg->setOpacity(50);
}

void PopListLayer::tableCellTouched(TableView* table, TableViewCell* cell) {

}

void PopListLayer::clickItem(ssize_t idx) {

}

ssize_t PopListLayer::numberOfCellsInTableView(TableView *table) {
    return 3;
}

Size PopListLayer::tableCellSizeForIndex(TableView *table, ssize_t idx) {
    auto itemBg = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("score_panel"));
    return itemBg->getContentSize();
}

TableViewCell* PopListLayer::tableCellAtIndex(TableView *table, ssize_t idx) {
    string spriteName = String::createWithFormat("medals_%d",3-(int)idx)->getCString();
    string key;
    switch ((int)idx) {
        case 0:
            key = KEY_FIRST_SCORE;
            break;
        case 1:
            key = KEY_SECOND_SCORE;
            break;
        case 2:
            key = KEY_THIRD_SCORE;
            break;

        default:
            key = KEY_FIRST_SCORE;
            break;
    }
    string score = String::createWithFormat("%d", UserDefault::getInstance()->getIntegerForKey(key.c_str()))->getCString();


    TableViewCell* cell = table->dequeueCell();
    if (!cell) {
        cell = new TableViewCell();
        cell->autorelease();

        auto itemBg = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("score_panel"));
        itemBg->setAnchorPoint(Vec2::ZERO);
        itemBg->setPosition(Vec2::ZERO);
        itemBg->setTag(TagItemBg);
        cell->addChild(itemBg);

        auto sortSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName(spriteName));
        sortSprite->setPosition(Vec2(55, 60));
        sortSprite->setTag(TagItemSortSprite);
        itemBg->addChild(sortSprite);

        auto currentScoreLabel = Label::createWithSystemFont(score, "fonts/Marker Felt.ttf", 20);
        currentScoreLabel->setColor(Color3B::RED);
        currentScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        currentScoreLabel->setPosition(Vec2(210, 80));
        currentScoreLabel->setTag(TagItemScore);
        itemBg->addChild(currentScoreLabel);

        auto button = ControlButton::create(String::createWithFormat("NO.%ld",idx+1)->getCString(), "fonts/Marker Felt.ttf", 20);
        button->setName(String::createWithFormat("%ld",idx)->getCString());
        button->setTitleColorForState(Color3B::RED, Control::State::NORMAL);
        button->setTitleColorForState(Color3B::RED, Control::State::HIGH_LIGHTED);
        button->setTag(TagItemBtn);
        button->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        button->setPosition(Vec2(210, 40));
        button->addTargetWithActionForControlEvents(this, cccontrol_selector(PopListLayer::touchUpInsideAction), Control::EventType::TOUCH_UP_INSIDE);
        itemBg->addChild(button);
    } else {
        auto sortSprite = (Sprite*)cell->getChildByTag(TagItemBg)->getChildByTag(TagItemSortSprite);
        sortSprite->setSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName(spriteName));

        auto currentScoreLabel = (Label*)cell->getChildByTag(TagItemBg)->getChildByTag(TagItemScore);
        currentScoreLabel->setString(score);

        auto button = (ControlButton*)cell->getChildByTag(TagItemBg)->getChildByTag(TagItemBtn);
        button->setTitleForState(String::createWithFormat("NO.%ld",idx+1)->getCString(), Control::State::NORMAL);
        button->setTitleForState(String::createWithFormat("NO.%ld",idx+1)->getCString(), Control::State::HIGH_LIGHTED);
        button->setName(String::createWithFormat("%ld",idx)->getCString());
    }

    return cell;
}

void PopListLayer::touchUpInsideAction(Ref *sender, Control::EventType controlEvent) {
    auto btn = (ControlButton*)sender;

    String * adaf = String::create(btn->getName());

    CCLOG("click------%d",adaf->intValue());
}
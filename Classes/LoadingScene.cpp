#include "LoadingScene.h"
#include "HomeScene.h"

LoadingScene::LoadingScene(){}
LoadingScene::~LoadingScene(){}

Scene* LoadingScene::createScene() {
    auto scene = Scene::create();
    auto layer = LoadingScene::create();
    scene->addChild(layer);

    return scene;
}

bool LoadingScene::init() {
    return !Layer::init() ? false : true;
}

void LoadingScene::onEnter() {
    auto bg = Sprite::create("image/splash.png");
    bg->setAnchorPoint(Vec2::ZERO);
    bg->setPosition(Vec2::ZERO);
    this->addChild(bg);

    Director::getInstance()->getTextureCache()->addImageAsync("image/atlas.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
}

void LoadingScene::loadingCallback(Texture2D *texture) {
    AtlasLoader::getInstance()->loadAtlas("image/atlas.txt", texture);

    SimpleAudioEngine::getInstance()->preloadEffect("sounds/sfx_die.ogg");
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/sfx_hit.ogg");
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/sfx_point.ogg");
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/sfx_swooshing.ogg");
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/sfx_wing.ogg");

    auto home = HomeScene::createScene();
    auto homeTrans = TransitionFadeTR::create(0.6, home);
    Director::getInstance()->replaceScene(homeTrans);
}

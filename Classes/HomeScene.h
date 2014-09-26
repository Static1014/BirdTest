//
//  HomeScene.h
//  BirdTest
//
//  Created by XiongJian on 14-9-10.
//
//

#ifndef __BirdTest__HomeScene__
#define __BirdTest__HomeScene__

#include "Constance.h"
#include "BackgroundLayer.h"
#include "BirdSprite.h"
#include "GameScene.h"
#include "PopListLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif

#define CLASS_NAME "org/cocos2dx/lib/Cocos2dxActivity"

class HomeScene : public Layer {
public:
    HomeScene();
    ~HomeScene();

    static Scene* createScene();
    virtual bool init();

    CREATE_FUNC(HomeScene);

    void menuCallBack(Ref* pSender);
    void onKeyReleased(EventKeyboard::KeyCode code, Event* pEvent);
    void removeTouchListener(Node* pNode);

    Menu* menu;
};

#endif /* defined(__BirdTest__HomeScene__) */

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

class HomeScene : public Layer {
public:
    HomeScene();
    ~HomeScene();

    static Scene* createScene();
    virtual bool init();

    CREATE_FUNC(HomeScene);

    void menuCallBack(Ref* pSender);
    void onKeyReleased(EventKeyboard::KeyCode code, Event* pEvent);
};

#endif /* defined(__BirdTest__HomeScene__) */

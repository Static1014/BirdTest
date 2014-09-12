//
//  GameOverLayer.h
//  BirdTest
//
//  Created by XiongJian on 14-9-12.
//
//

#ifndef __BirdTest__GameOverLayer__
#define __BirdTest__GameOverLayer__

#include "Constance.h"
enum {
    TAG_RESTART_BTN,
    TAG_BACK_MENU_BTN
};

class GameOverLayer : public Layer {
public:
    GameOverLayer();
    ~GameOverLayer();

    virtual bool init();
    CREATE_FUNC(GameOverLayer);

    void menuCallBack(Ref* pSender);
};

#endif /* defined(__BirdTest__GameOverLayer__) */

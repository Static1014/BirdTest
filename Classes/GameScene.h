//
//  GameScene.h
//  BirdTest
//
//  Created by XiongJian on 14-9-11.
//
//

#ifndef __BirdTest__GameScene__
#define __BirdTest__GameScene__

#include "Constance.h"

class GameScene : public Scene {
public:
    GameScene();
    ~GameScene();

    virtual bool init();

    CREATE_FUNC(GameScene);

};

#endif /* defined(__BirdTest__GameScene__) */

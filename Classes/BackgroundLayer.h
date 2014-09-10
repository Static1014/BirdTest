//
//  BackgroundLayer.h
//  BirdTest
//
//  Created by XiongJian on 14-9-10.
//
//

#ifndef __BirdTest__BackgroundLayer__
#define __BirdTest__BackgroundLayer__

#include "Constance.h"

class BackgroundLayer : public Layer {
public:
    BackgroundLayer();
    ~BackgroundLayer();

    virtual bool init();

    CREATE_FUNC(BackgroundLayer);

public:
    Sprite* land1;
    Sprite* land2;

    static float getLandHeight();
    void scrollLand(float dt);
    void setScrollLand(bool pScroll);
};

#endif /* defined(__BirdTest__BackgroundLayer__) */

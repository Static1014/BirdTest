//
//  TestLayer.h
//  PopLayer
//
//  Created by XiongJian on 14-9-16.
//
//

#ifndef __BirdTest__PopLayer__
#define __BirdTest__PopLayer__

#include "Constance.h"

class PopLayer : public Layer {
public:
    PopLayer();
    ~PopLayer();
    
    virtual bool init();
    CREATE_FUNC(PopLayer);

    void menuCallback(Ref* pSender);

    void setBackground(string bgImage, float scale = 1.0);

    void setTitle(string title = "");

    void addCustomNode(Node* node, Vec2 location);

    void setCallBackFunc(Ref* target, SEL_CallFuncN callFunc);

    void addButton(string normalImage, string selectImage, string title = "", int tag = 0);

    void onEnter();

public:
    Sprite* mBackground;
    Menu* mMenu;
    float defaultScale;
    bool setCallBack;

    Ref* mCallBackListener;
    SEL_CallFuncN mCallFuncN;

};

#endif /* defined(__BirdTest__PopLayer__) */

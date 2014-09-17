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

    void setBackground(string bgImage);

    void setTitle(string title = "");

    void addCustomNode(Node* node, Vec2 location);

    void setCallBackFunc(Ref* target, SEL_CallFuncN callFunc);

    void addButton(string normalImage, string selectImage, string title = "", int tag = 0);

    void onEnter();

private:
    Sprite* mBackground;
    Menu* mMenu;

    Ref* mCallBackListener;
    SEL_CallFuncN mCallFuncN;

};

#endif /* defined(__BirdTest__PopLayer__) */

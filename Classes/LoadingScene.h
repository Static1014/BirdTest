#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "Constance.h"

class LoadingScene : public Layer
{
public:
    LoadingScene();
    ~LoadingScene();

    static Scene* createScene();
    virtual bool init();

    CREATE_FUNC(LoadingScene);

    void onEnter() override;

    void loadingCallback(Texture2D *texture);
};

#endif // __LOADING_SCENE_H__

#pragma once

#include "cocos2d.h"
#include "Player.h"

class MoodEventDemoScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual void update(float dt) override;

    CREATE_FUNC(MoodEventDemoScene);

private:
    void setPlayerVisible(bool visible);
    void refreshHUD();

    // 事件驱动：模拟“移动”和“受击”
    void triggerMoveStart();
    void triggerMoveStop();
    void triggerHit(int damage);

private:
    Player* _player = nullptr;
    cocos2d::Label* _moodLabel = nullptr;

    float _t = 0.0f;   // 状态计时
    int _step = 0;     // 当前步骤
};
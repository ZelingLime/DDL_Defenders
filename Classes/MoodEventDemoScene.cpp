#include "MoodEventDemoScene.h"

USING_NS_CC;

Scene* MoodEventDemoScene::createScene()
{
    return MoodEventDemoScene::create();
}

bool MoodEventDemoScene::init()
{
    if (!Scene::init())
        return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 创建 Player，但不需要图标（imagePath 为空）
    _player = Player::create(
        "Player",
        "", // 不显示图标；后面你有角色图再改这里
        Vec2(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y),
        100,   // maxHp
        200.0f,// baseSpeed
        0      // defense
    );

    if (!_player)
        return false;

    // 不显示/不addChild都行；但为了安全，addChild后隐藏
    addChild(_player, 1);
    setPlayerVisible(false);

    _moodLabel = Label::createWithTTF("Mood: Normal", "fonts/Marker Felt.ttf", 22);
    _moodLabel->setPosition(Vec2(origin.x + 110, origin.y + visibleSize.height - 30));
    addChild(_moodLabel, 2);

    _t = 0.0f;
    _step = 0;

    return true;
}

void MoodEventDemoScene::setPlayerVisible(bool visible)
{
    if (_player)
    {
        _player->setVisible(visible);
    }
}

void MoodEventDemoScene::triggerMoveStart()
{
    if (!_player) return;
    _player->setInputDirection(Vec2(1, 0)); // 触发：移动事件 → Focus
}

void MoodEventDemoScene::triggerMoveStop()
{
    if (!_player) return;
    _player->setInputDirection(Vec2::ZERO);
}

void MoodEventDemoScene::triggerHit(int damage)
{
    if (!_player) return;
    _player->takeDamage(damage); // 触发：受击事件 → Irritable/Exhausted
}

void MoodEventDemoScene::refreshHUD()
{
    if (!_player || !_moodLabel) return;
    _moodLabel->setString("Mood: " + _player->getCurrentMoodName());
}

void MoodEventDemoScene::update(float dt)
{
    if (!_player) return;

    // 驱动 Player：更新 mood 计时、无敌计时等
    _player->updatePlayer(dt);

    // 事件脚本（每步间隔 >= 1.0s，避免无敌挡住 takeDamage）
    _t += dt;

    switch (_step)
    {
    case 0:
        // 等 0.8s，开始“移动”
        if (_t >= 0.8f) { triggerMoveStart(); _t = 0.0f; _step = 1; }
        break;

    case 1:
        // 持续 0.7s，停止移动
        if (_t >= 0.7f) { triggerMoveStop(); _t = 0.0f; _step = 2; }
        break;

    case 2:
        // 受击：从 100hp 先打 40 -> 60hp（>50%）=> Irritable
        if (_t >= 1.1f) { triggerHit(40); _t = 0.0f; _step = 3; }
        break;

    case 3:
        // 受击：再打 20 -> 40hp（<=50%）=> Exhausted
        if (_t >= 1.1f) { triggerHit(20); _t = 0.0f; _step = 4; }
        break;

    default:
        // 后面不再触发，只看 Mood 自动回 Normal
        break;
    }

    refreshHUD();
}
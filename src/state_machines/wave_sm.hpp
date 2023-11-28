#pragma once
#include "../util/state_machine.hpp"

class GameManager;

struct BaseWaveS : public State {

    BaseWaveS(RReader<GameManager> manager);

    virtual const Transition* processEvent(Event& event) override;

protected:
    RReader<GameManager> manager_;
};

struct OngoingWaveS : public BaseWaveS {
    OngoingWaveS(RReader<GameManager> manager) : BaseWaveS{manager} {};

    virtual void onEnter();

    virtual void onExit();

    virtual void tick();
};

struct OngoingWavePausedS : public BaseWaveS {
    OngoingWavePausedS(RReader<GameManager> manager) : BaseWaveS{manager} {};

    virtual void tick();
};

struct BetweenWavesS : public BaseWaveS {
    BetweenWavesS(RReader<GameManager> manager) : BaseWaveS{manager} {};

    virtual void tick();
};

struct LostWaveS : public BaseWaveS {
    LostWaveS(RReader<GameManager> manager) : BaseWaveS{manager} {};

    virtual void onEnter();

    virtual void onExit();

    virtual void tick();
};

struct WaveSM : public StateMachine {
    WaveSM(RReader<GameManager> manager);

    virtual void tick() override;

    virtual ~WaveSM() = default;

private:
    RReader<GameManager> manager_;
};

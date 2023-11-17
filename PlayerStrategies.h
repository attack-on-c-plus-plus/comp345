/**
 ************************************
 * COMP 345 Professor Hakim Mellah
 ************************************
 * @author Team 5 Attack on C++
 * @author Daniel Soldera
 * @author Carson Senthilkumar
 * @author Joe El-Khoury
 * @author Henri Stephane Carbon
 * @author Haris Mahmood
 */

#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H
#include <vector>

#include "GameEngine.h"

class CommandProcessor;
class Territory;
class Player;

enum class Strategy {
    Neutral,
    Cheater,
    Human,
    Aggressive,
    Benevolent
};

/**
 * \brief Abstract player strategy in order to implement different kinds of player behaviours
 */
class PlayerStrategy {
public:
    explicit PlayerStrategy(Player &player, GameEngine& gameEngine);
    PlayerStrategy(const PlayerStrategy &playerStrategy);
    virtual ~PlayerStrategy();
    PlayerStrategy &operator=(const PlayerStrategy &playerStrategy);
    virtual void issueOrder() = 0;
    [[nodiscard]] virtual std::vector<const Territory *> toAttack() const = 0;
    [[nodiscard]] virtual std::vector<const Territory *> toDefend() const = 0;
protected:
    Player *player_;            // weak pointer
    GameEngine* gameEngine_;    // weak pointer
private:
};

class HumanPlayerStrategy final : public PlayerStrategy {
public:
    explicit HumanPlayerStrategy(Player& player, GameEngine& gameEngine);
    HumanPlayerStrategy(const HumanPlayerStrategy& humanPlayerStrategy);
    ~HumanPlayerStrategy() override;
    HumanPlayerStrategy &operator=(const HumanPlayerStrategy& humanPlayerStrategy);
    void issueOrder() override;
    [[nodiscard]] std::vector<const Territory*> toAttack() const override;
    [[nodiscard]] std::vector<const Territory*> toDefend() const override;
private:
    CommandProcessor *commandProcessor_;
};

class NeutralPlayerStrategy final : public PlayerStrategy {
public:
    explicit NeutralPlayerStrategy(Player& player, GameEngine& gameEngine);
    NeutralPlayerStrategy(const NeutralPlayerStrategy& neutralPlayerStrategy);
    ~NeutralPlayerStrategy() override = default;
    NeutralPlayerStrategy &operator=(const NeutralPlayerStrategy& neutralPlayerStrategy);
    void issueOrder() override;
    [[nodiscard]] std::vector<const Territory*> toAttack() const override;
    [[nodiscard]] std::vector<const Territory*> toDefend() const override;
};

#endif //PLAYERSTRATEGIES_H

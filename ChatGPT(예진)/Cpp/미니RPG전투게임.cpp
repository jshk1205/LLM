#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <random>

class Character {
public:
    std::string name;
    int level;
    int maxHealth;
    int health;
    int attack;
    int experience;
    std::map<std::string, int> items;
    std::vector<Character*> allies;

    Character(std::string name, int level) : name(name), level(level) {
        maxHealth = rand() % 51 + 50 + (10 * level);
        health = maxHealth;
        attack = rand() % 16 + 5 + (2 * level);
        experience = 0;
        items = { {"health_potion", 3}, {"mana_potion", 2} };
    }

    virtual void takeDamage(int damage) {
        health -= damage;
        if (health < 0) {
            health = 0;
        }
    }

    bool isAlive() {
        return health > 0;
    }

    void levelUp() {
        level++;
        maxHealth += 10;
        attack += 2;
        health = maxHealth;
        std::cout << name << " leveled up to level " << level << "!" << std::endl;
    }

    void gainExperience(int exp) {
        experience += exp;
        std::cout << name << " gained " << exp << " experience points!" << std::endl;
        if (experience >= level * 100) {
            levelUp();
        }
    }

    void useItem(std::string item) {
        if (items.find(item) != items.end() && items[item] > 0) {
            if (item == "health_potion") {
                int healthToRestore = std::min(30, maxHealth - health);
                health += healthToRestore;
                items[item]--;
                std::cout << name << " used a health potion and restored " << healthToRestore << " health!" << std::endl;
            }
            else if (item == "mana_potion") {
                items[item]--;
                std::cout << name << " used a mana potion!" << std::endl;
                // Add mana restoring logic here if needed
            }
        }
    }

    std::string status() {
        return name + "'s health: " + std::to_string(health) + "/" + std::to_string(maxHealth) +
            ", Level: " + std::to_string(level) + ", Experience: " + std::to_string(experience) +
            "/" + std::to_string(level * 100) + ", Items: {health_potion: " + std::to_string(items["health_potion"]) +
            ", mana_potion: " + std::to_string(items["mana_potion"]) + "}";
    }

    void joinAlly(Character* ally) {
        allies.push_back(ally);
        std::cout << ally->name << " joined " << name << "'s party!" << std::endl;
    }
};

class Player : public Character {
public:
    int gold;

    Player(std::string name, int level) : Character(name, level), gold(100) {}

    void takeDamage(int damage) {
        Character::takeDamage(damage);
        if (attack < 0) {
            attack = 0;
        }
    }
};

class Enemy : public Character {
public:
    std::string specialAbility;

    Enemy(std::string name, int level, std::string specialAbility) : Character(name, level), specialAbility(specialAbility) {}

    void takeDamage(int damage) {
        if (specialAbility == "block") {
            std::cout << name << " blocks the attack!" << std::endl;
        }
        else {
            Character::takeDamage(damage);
        }
    }

    void attackTarget(Character* target) {
        if (specialAbility == "stun") {
            std::cout << name << " uses stun ability!" << std::endl;
            target->takeDamage(attack * 2);
        }
        else {
            target->takeDamage(std::max(1, attack));
        }
    }
};

void randomEvent(Player* player, Enemy* enemy) {
    double eventChance = static_cast<double>(rand()) / RAND_MAX; // Generate a random number between 0 and 1
    if (eventChance < 0.2) { // 20% chance of an event occurring
        int event = rand() % 5 + 1;
        switch (event) {
        case 1:
            std::cout << "A friendly wizard appears and heals you." << std::endl;
            player->health = std::min(player->maxHealth, player->health + 20);
            break;
        case 2:
            std::cout << "You found a treasure chest! You gained 50 experience points." << std::endl;
            player->gainExperience(50);
            break;
        case 3:
            std::cout << "A curse befalls you, reducing your attack power!" << std::endl;
            if (player->attack >= 5) {
                player->attack -= 5;
            }
            else {
                player->attack = 0;
            }
            break;
        case 4:
            std::cout << "You stumbled upon a hidden passage and found a health potion!" << std::endl;
            player->useItem("health_potion");
            break;
        default:
            std::cout << "A mysterious mist surrounds you, confusing the enemy!" << std::endl;
            if (enemy->attack >= 5) {
                enemy->attack -= 5;
            }
            else {
                enemy->attack = 0;
            }
            break;
        }
    }
}

void battle(Player* player, Enemy* enemy) {
    std::cout << "Battle Start!" << std::endl;
    while (player->isAlive() && enemy->isAlive()) {
        randomEvent(player, enemy);
        int playerDamage = rand() % std::max(1, player->attack) + 1; // Ensure player's attack is at least 1
        int enemyDamage = rand() % std::max(1, enemy->attack) + 1; // Ensure enemy's attack is at least 1

        std::cout << player->name << " attacks " << enemy->name << " for " << playerDamage << " damage." << std::endl;
        enemy->takeDamage(playerDamage);
        std::cout << enemy->name << " attacks " << player->name << " for " << enemyDamage << " damage." << std::endl;
        player->takeDamage(enemyDamage);

        std::cout << player->status() << std::endl;
        std::cout << enemy->status() << std::endl;
        std::cout << std::endl;
    }

    if (!player->isAlive()) {
        std::cout << enemy->name << " wins!" << std::endl;
    }
    else if (!enemy->isAlive()) {
        std::cout << player->name << " wins!" << std::endl;
    }

    std::cout << "Game Over!" << std::endl;
}

int main() {
    srand(time(0));

    std::string playerName;
    std::cout << "Enter player name: ";
    std::cin >> playerName;
    Player player(playerName, 1);

    std::string enemyName = "Evil Wizard";
    Enemy enemy(enemyName, rand() % 3 + 3, "block");

    player.joinAlly(new Character("Knight", player.level));
    player.joinAlly(new Character("Archer", player.level));

    battle(&player, &enemy);

    std::cout << "Game Over!" << std::endl;
    std::cout << player.status() << std::endl;
    std::cout << enemy.status() << std::endl;

    return 0;
}

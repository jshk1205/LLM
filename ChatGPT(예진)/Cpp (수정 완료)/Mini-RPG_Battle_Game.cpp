#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

#define MAX_ITEMS 2
#define MAX_ALLIES 2

using namespace std;

class Character {
public:
    char name[100];
    int level;
    int maxHealth;
    int health;
    int attack;
    int experience;
    int items[MAX_ITEMS];
    Character* allies[MAX_ALLIES];

    Character() {
        for (int i = 0; i < MAX_ALLIES; i++) {
            allies[i] = nullptr;
        }
    }

    void initialize(const char* name, int level, int maxHealth, int health, int attack, int experience, int healthPotion, int manaPotion) {
        strcpy(this->name, name);
        this->level = level;
        this->maxHealth = maxHealth;
        this->health = health;
        this->attack = attack;
        this->experience = experience;
        this->items[0] = healthPotion;
        this->items[1] = manaPotion;
    }

    void takeDamage(int damage) {
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
        cout << name << " has leveled up to " << level << "!" << endl;
    }

    void gainExperience(int exp) {
        experience += exp;
        cout << name << " gained " << exp << " experience!" << endl;
        if (experience >= level * 100) {
            levelUp();
        }
    }

    void useItem(const char* item) {
        if (strcmp(item, "health_potion") == 0) {
            int healthToRestore = maxHealth - health;
            health += healthToRestore;
            cout << name << " used a health potion, restored " << healthToRestore << " health!" << endl;
        }
        else if (strcmp(item, "mana_potion") == 0) {
            cout << name << " used a mana potion!" << endl;
            // Mana recovery logic can be added here
        }
    }

    void joinAlly(Character* ally) {
        for (int i = 0; i < MAX_ALLIES; i++) {
            if (allies[i] == nullptr) {
                allies[i] = ally;
                cout << ally->name << " has joined " << name << "'s party!" << endl;
                break;
            }
        }
    }
};

class Player : public Character {
public:
    int gold;

    void takeDamage(int damage) {
        Character::takeDamage(damage);
        if (attack < 0) {
            attack = 0;
        }
    }
};

class Enemy : public Character {
public:
    char specialAbility[20];

    void takeDamage(int damage) {
        // 50% chance to block the attack
        if (strcmp(specialAbility, "block") == 0 && ((double)rand() / RAND_MAX) < 0.5) {
            cout << name << " blocked the attack!" << endl;
        }
        else {
            Character::takeDamage(damage);
        }
    }

    void attackTarget(Character* target) {
        if (strcmp(specialAbility, "stun") == 0) {
            cout << name << " used the stun ability!" << endl;
            target->health -= attack * 2;
        }
        else {
            target->takeDamage(attack);
        }
    }
};

void randomEvent(Player* player, Enemy* enemy) {
    double eventChance = (double)rand() / RAND_MAX; // Generate a random number between 0 and 1
    if (eventChance < 0.2) { // 20% chance of an event occurring
        int event = rand() % 5 + 1;
        switch (event) {
        case 1:
            cout << "A kind wizard appears and heals you." << endl;
            player->health = player->maxHealth;
            break;
        case 2:
            cout << "You found a treasure chest! Gained 50 experience." << endl;
            player->gainExperience(50);
            break;
        case 3:
            cout << "You are cursed and your attack decreases!" << endl;
            if (player->attack >= 5) {
                player->attack -= 5;
            }
            else {
                player->attack = 0;
            }
            break;
        case 4:
            cout << "You discovered a hidden passage and found a health potion!" << endl;
            player->useItem("health_potion");
            break;
        default:
            cout << "A mysterious fog confuses the enemy!" << endl;
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
    cout << "Battle begins!" << endl;
    while (player->isAlive() && enemy->isAlive()) {
        randomEvent(player, enemy);
        int playerDamage = rand() % player->attack + 1; // Set player's minimum attack to 1
        int enemyDamage = rand() % enemy->attack + 1; // Set enemy's minimum attack to 1

        cout << player->name << " attacked " << enemy->name << " for " << playerDamage << " damage." << endl;
        enemy->takeDamage(playerDamage);

        if (enemy->isAlive()) { // Attack only if the enemy is alive
            cout << enemy->name << " attacked " << player->name << " for " << enemyDamage << " damage." << endl;
            player->takeDamage(enemyDamage);
        }

        cout << player->name << ": " << player->health << "/" << player->maxHealth << " HP" << endl;
        cout << enemy->name << ": " << enemy->health << "/" << enemy->maxHealth << " HP" << endl << endl;
    }

    if (!player->isAlive()) {
        cout << enemy->name << " wins!" << endl;
    }
    else if (!enemy->isAlive()) {
        cout << player->name << " wins!" << endl;
    }

    cout << "The End" << endl;
}

int main() {
    srand(time(nullptr));

    char playerName[100];
    cout << "Enter player's name: ";
    cin >> playerName;
    Player player;
    player.initialize(playerName, 1, rand() % 51 + 50, rand() % 51 + 50, rand() % 16 + 5, 0, 3, 2);
    player.gold = 100;

    char enemyName[] = "Evil Wizard";
    Enemy enemy;
    enemy.initialize(enemyName, rand() % 3 + 3, rand() % 51 + 50, rand() % 51 + 50, rand() % 16 + 5, 0, 0, 0);
    strcpy(enemy.specialAbility, "block");

    player.joinAlly(&enemy);
    Character knight, archer;
    knight.initialize("Knight", player.level, rand() % 51 + 50, rand() % 51 + 50, rand() % 16 + 5, 0, 0, 0);
    player.joinAlly(&knight);
    archer.initialize("Archer", player.level, rand() % 51 + 50, rand() % 51 + 50, rand() % 16 + 5, 0, 0, 0);
    player.joinAlly(&archer);

    battle(&player, &enemy);

    return 0;
}

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITEMS 2
#define MAX_ALLIES 2

typedef struct {
    char name[100];
    int level;
    int maxHealth;
    int health;
    int attack;
    int experience;
    int items[MAX_ITEMS];
    struct Character* allies[MAX_ALLIES];
} Character;

typedef struct {
    Character base;
    int gold;
} Player;

typedef struct {
    Character base;
    char specialAbility[20];
} Enemy;

void initializeCharacter(Character* character, const char* name, int level, int maxHealth, int health, int attack, int experience, int healthPotion, int manaPotion) {
    strcpy(character->name, name);
    character->level = level;
    character->maxHealth = maxHealth;
    character->health = health;
    character->attack = attack;
    character->experience = experience;
    character->items[0] = healthPotion;
    character->items[1] = manaPotion;
    int i;
    for (i = 0; i < MAX_ALLIES; i++) {
        character->allies[i] = NULL;
    }
}

void takeDamage(Character* character, int damage) {
    character->health -= damage;
    if (character->health < 0) {
        character->health = 0;
    }
}

int isAlive(Character* character) {
    return character->health > 0;
}

void levelUp(Character* character) {
    character->level++;
    character->maxHealth += 10;
    character->attack += 2;
    character->health = character->maxHealth;
    printf("%s leveled up to level %d!\n", character->name, character->level);
}

void gainExperience(Character* character, int exp) {
    character->experience += exp;
    printf("%s gained %d experience points!\n", character->name, exp);
    if (character->experience >= character->level * 100) {
        levelUp(character);
    }
}

void useItem(Character* character, const char* item) {
    if (strcmp(item, "health_potion") == 0) {
        int healthToRestore = character->maxHealth - character->health;
        character->health += healthToRestore;
        printf("%s used a health potion and restored %d health!\n", character->name, healthToRestore);
    }
    else if (strcmp(item, "mana_potion") == 0) {
        printf("%s used a mana potion!\n", character->name);
        // Add mana restoring logic here if needed
    }
}

void joinAlly(Character* character, Character* ally) {
    int i;
    for (i = 0; i < MAX_ALLIES; i++) {
        if (character->allies[i] == NULL) {
            character->allies[i] = ally;
            printf("%s joined %s's party!\n", ally->name, character->name);
            break;
        }
    }
}

void Player_takeDamage(Player* player, int damage) {
    takeDamage((Character*)player, damage);
    if (player->base.attack < 0) {
        player->base.attack = 0;
    }
}

void Enemy_takeDamage(Enemy* enemy, int damage) {
    if (strcmp(enemy->specialAbility, "block") == 0) {
        printf("%s blocks the attack!\n", enemy->base.name);
    }
    else {
        takeDamage((Character*)enemy, damage);
    }
}

void Enemy_attackTarget(Enemy* enemy, Character* target) {
    if (strcmp(enemy->specialAbility, "stun") == 0) {
        printf("%s uses stun ability!\n", enemy->base.name);
        target->health -= enemy->base.attack * 2;
    }
    else {
        takeDamage(target, enemy->base.attack);
    }
}

void randomEvent(Player* player, Enemy* enemy) {
    double eventChance = (double)rand() / RAND_MAX; // Generate a random number between 0 and 1
    if (eventChance < 0.2) { // 20% chance of an event occurring
        int event = rand() % 5 + 1;
        switch (event) {
        case 1:
            printf("A friendly wizard appears and heals you.\n");
            player->base.health = player->base.maxHealth;
            break;
        case 2:
            printf("You found a treasure chest! You gained 50 experience points.\n");
            gainExperience((Character*)player, 50);
            break;
        case 3:
            printf("A curse befalls you, reducing your attack power!\n");
            if (player->base.attack >= 5) {
                player->base.attack -= 5;
            }
            else {
                player->base.attack = 0;
            }
            break;
        case 4:
            printf("You stumbled upon a hidden passage and found a health potion!\n");
            useItem((Character*)player, "health_potion");
            break;
        default:
            printf("A mysterious mist surrounds you, confusing the enemy!\n");
            if (enemy->base.attack >= 5) {
                enemy->base.attack -= 5;
            }
            else {
                enemy->base.attack = 0;
            }
            break;
        }
    }
}

void battle(Player* player, Enemy* enemy) {
    printf("Battle Start!\n");
    while (isAlive((Character*)player) && isAlive((Character*)enemy)) {
        randomEvent(player, enemy);
        int playerDamage = rand() % (player->base.attack) + 1; // Ensure player's attack is at least 1
        int enemyDamage = rand() % (enemy->base.attack) + 1; // Ensure enemy's attack is at least 1

        printf("%s attacks %s for %d damage.\n", player->base.name, enemy->base.name, playerDamage);
        Enemy_takeDamage(enemy, playerDamage);
        printf("%s attacks %s for %d damage.\n", enemy->base.name, player->base.name, enemyDamage);
        Player_takeDamage(player, enemyDamage);

        printf("%s\n", player->base.name);
        printf("%s\n\n", enemy->base.name);
    }

    if (!isAlive((Character*)player)) {
        printf("%s wins!\n", enemy->base.name);
    }
    else if (!isAlive((Character*)enemy)) {
        printf("%s wins!\n", player->base.name);
    }

    printf("Game Over!\n");
}

int main() {
    srand(time(NULL));

    char playerName[100];
    printf("Enter player name: ");
    scanf("%s", playerName);
    Player player;
    initializeCharacter(&(player.base), playerName, 1, rand() % 51 + 50, rand() % 51 + 50, rand() % 16 + 5, 0, 3, 2);
    player.gold = 100;

    char enemyName[] = "Evil Wizard";
    Enemy enemy;
    initializeCharacter(&(enemy.base), enemyName, rand() % 3 + 3, rand() % 51 + 50, rand() % 51 + 50, rand() % 16 + 5, 0, 0, 0);
    strcpy(enemy.specialAbility, "block");

    joinAlly(&(player.base), &(enemy.base));
    Character knight, archer;
    initializeCharacter(&knight, "Knight", player.base.level, rand() % 51 + 50, rand() % 51 + 50, rand() % 16 + 5, 0, 0, 0);
    joinAlly(&(player.base), &knight);
    initializeCharacter(&archer, "Archer", player.base.level, rand() % 51 + 50, rand() % 51 + 50, rand() % 16 + 5, 0, 0, 0);
    joinAlly(&(player.base), &archer);

    battle(&player, &enemy);

    printf("Game Over!\n");
    printf("%s\n", player.base.name);
    printf("%s\n", enemy.base.name);

    return 0;
}

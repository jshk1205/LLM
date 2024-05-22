import random

MAX_ITEMS = 2
MAX_ALLIES = 2

class Character:
    def __init__(self, name, level, max_health, health, attack, experience, health_potion, mana_potion):
        self.name = name
        self.level = level
        self.max_health = max_health
        self.health = health
        self.attack = attack
        self.experience = experience
        self.items = [health_potion, mana_potion]
        self.allies = [None] * MAX_ALLIES

    def take_damage(self, damage):
        self.health -= damage
        if self.health < 0:
            self.health = 0

    def is_alive(self):
        return self.health > 0

    def level_up(self):
        self.level += 1
        self.max_health += 10
        self.attack += 2
        self.health = self.max_health
        print(f"{self.name} has leveled up to {self.level}!")

    def gain_experience(self, exp):
        self.experience += exp
        print(f"{self.name} gained {exp} experience!")
        if self.experience >= self.level * 100:
            self.level_up()

    def use_item(self, item):
        if item == "health_potion":
            health_to_restore = self.max_health - self.health
            self.health += health_to_restore
            print(f"{self.name} used a health potion, restored {health_to_restore} health!")
        elif item == "mana_potion":
            print(f"{self.name} used a mana potion!")
            # Mana recovery logic can be added here

    def join_ally(self, ally):
        for i in range(MAX_ALLIES):
            if self.allies[i] is None:
                self.allies[i] = ally
                print(f"{ally.name} has joined {self.name}'s party!")
                break

class Player(Character):
    def __init__(self, name, level, max_health, health, attack, experience, health_potion, mana_potion, gold):
        super().__init__(name, level, max_health, health, attack, experience, health_potion, mana_potion)
        self.gold = gold

class Enemy(Character):
    def __init__(self, name, level, max_health, health, attack, experience, special_ability):
        super().__init__(name, level, max_health, health, attack, experience, 0, 0)
        self.special_ability = special_ability

    def take_damage(self, damage):
        if self.special_ability == "block" and random.random() < 0.5:
            print(f"{self.name} blocked the attack!")
        else:
            super().take_damage(damage)

    def attack_target(self, target):
        if self.special_ability == "stun":
            print(f"{self.name} used the stun ability!")
            target.take_damage(self.attack * 2)
        else:
            target.take_damage(self.attack)

def random_event(player, enemy):
    event_chance = random.random()
    if event_chance < 0.2:
        event = random.randint(1, 5)
        if event == 1:
            print("A kind wizard appears and heals you.")
            player.health = player.max_health
        elif event == 2:
            print("You found a treasure chest! Gained 50 experience.")
            player.gain_experience(50)
        elif event == 3:
            print("You are cursed and your attack decreases!")
            if player.attack >= 5:
                player.attack -= 5
            else:
                player.attack = 0
        elif event == 4:
            print("You discovered a hidden passage and found a health potion!")
            player.use_item("health_potion")
        else:
            print("A mysterious fog confuses the enemy!")
            enemy.attack = max(1, enemy.attack - 5)

def battle(player, enemy):
    print("Battle begins!")
    while player.is_alive() and enemy.is_alive():
        random_event(player, enemy)
        player_damage = random.randint(1, player.attack)
        enemy_damage = random.randint(1, enemy.attack)

        print(f"{player.name} attacked {enemy.name} for {player_damage} damage.")
        enemy.take_damage(player_damage)

        if enemy.is_alive():
            print(f"{enemy.name} attacked {player.name} for {enemy_damage} damage.")
            player.take_damage(enemy_damage)

        print(f"{player.name}: {player.health}/{player.max_health} HP")
        print(f"{enemy.name}: {enemy.health}/{enemy.max_health} HP\n")

    if not player.is_alive():
        print(f"{enemy.name} wins!")
    elif not enemy.is_alive():
        print(f"{player.name} wins!")

    print("The End")

def main():
    random.seed()

    player_name = input("Enter player's name: ")
    player = Player(player_name, 1, random.randint(50, 100), random.randint(50, 100), random.randint(5, 20), 0, 3, 2, 100)

    enemy_name = "Evil Wizard"
    enemy = Enemy(enemy_name, random.randint(3, 5), random.randint(50, 100), random.randint(50, 100), random.randint(5, 20), 0, "block")

    player.join_ally(enemy)
    knight = Character("Knight", player.level, random.randint(50, 100), random.randint(50, 100), random.randint(5, 20), 0, 0, 0)
    player.join_ally(knight)
    archer = Character("Archer", player.level, random.randint(50, 100), random.randint(50, 100), random.randint(5, 20), 0, 0, 0)
    player.join_ally(archer)

    battle(player, enemy)

if __name__ == "__main__":
    main()

import random

class Character:
    def __init__(self, name, level=1):
        self.name = name
        self.level = level
        self.max_health = random.randint(50, 100) + (10 * self.level)
        self.health = self.max_health
        self.attack = random.randint(5, 20) + (2 * self.level)
        self.experience = 0
        self.items = {"health_potion": 3, "mana_potion": 2}
        self.allies = []

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
        print(f"{self.name} leveled up to level {self.level}!")

    def gain_experience(self, exp):
        self.experience += exp
        print(f"{self.name} gained {exp} experience points!")
        if self.experience >= self.level * 100:
            self.level_up()

    def use_item(self, item):
        if self.items.get(item, 0) > 0:
            if item == "health_potion":
                health_to_restore = min(30, self.max_health - self.health)
                self.health += health_to_restore
                self.items[item] -= 1
                print(f"{self.name} used a health potion and restored {health_to_restore} health!")
            elif item == "mana_potion":
                self.items[item] -= 1
                print(f"{self.name} used a mana potion!")
                # Add mana restoring logic here if needed

    def status(self):
        return f"{self.name}'s health: {self.health}/{self.max_health}, Level: {self.level}, Experience: {self.experience}/{self.level * 100}, Items: {self.items}"

    def join_ally(self, ally):
        self.allies.append(ally)
        print(f"{ally.name} joined {self.name}'s party!")

class Player(Character):
    def __init__(self, name, level=1):
        super().__init__(name, level)
        self.gold = 100  # Initial gold amount for the player

    def take_damage(self, damage):
        super().take_damage(damage)
        if self.attack < 0:
            self.attack = 0

class Enemy(Character):
    def __init__(self, name, level=1, special_ability=None):
        super().__init__(name, level)
        self.special_ability = special_ability

    def take_damage(self, damage):
        if self.special_ability == "block":
            print(f"{self.name} blocks the attack!")
        else:
            super().take_damage(damage)

    def attack_target(self, target):
        if self.special_ability == "stun":
            print(f"{self.name} uses stun ability!")
            target.take_damage(self.attack * 2)
        else:
            target.take_damage(max(1, self.attack))

def random_event(player, enemy):
    event = random.randint(1, 5)
    if event == 1:
        print("A friendly wizard appears and heals you.")
        player.health = min(player.max_health, player.health + 20)
    elif event == 2:
        print("You found a treasure chest! You gained 50 experience points.")
        player.gain_experience(50)
    elif event == 3:
        print("A curse befalls you, reducing your attack power!")
        if player.attack >= 5:
            player.attack -= 5
        else:
            player.attack = 0
    elif event == 4:
        print("You stumbled upon a hidden passage and found a health potion!")
        player.use_item("health_potion")
    else:
        print("A mysterious mist surrounds you, confusing the enemy!")
        if enemy.attack >= 5:
            enemy.attack -= 5
        else:
            enemy.attack = 0


def battle(player, enemy):
    print("Battle Start!")
    while player.is_alive() and enemy.is_alive():
        random_event(player, enemy)
        player_damage = random.randint(1, max(1, player.attack))  # Ensure player's attack is at least 1
        enemy_damage = random.randint(1, max(1, enemy.attack))    # Ensure enemy's attack is at least 1

        print(f"{player.name} attacks {enemy.name} for {player_damage} damage.")
        enemy.take_damage(player_damage)
        print(f"{enemy.name} attacks {player.name} for {enemy_damage} damage.")
        player.take_damage(enemy_damage)

        print(player.status())
        print(enemy.status())
        print()

        if not player.is_alive():
            print(f"{enemy.name} wins!")
            return
        elif not enemy.is_alive():
            print(f"{player.name} wins!")
            return

    print("Game Over!")




def main():
    player_name = input("Enter player name: ")
    player = Player(player_name)

    enemy_name = "Evil Wizard"
    enemy = Enemy(enemy_name, random.randint(3, 5), special_ability="block")

    player.join_ally(Character("Knight", level=player.level))
    player.join_ally(Character("Archer", level=player.level))

    battle(player, enemy)

    print("Game Over!")
    print(player.status())
    print(enemy.status())

if __name__ == "__main__":
    main()

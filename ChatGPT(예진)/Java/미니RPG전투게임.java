import java.util.HashMap;
import java.util.Map;
import java.util.Random;

class Character {
    String name;
    int level;
    int maxHealth;
    int health;
    int attack;
    int experience;
    Map<String, Integer> items;
    Character[] allies;

    Character(String name, int level) {
        this.name = name;
        this.level = level;
        this.maxHealth = new Random().nextInt(51) + 50 + (10 * this.level);
        this.health = this.maxHealth;
        this.attack = new Random().nextInt(16) + 5 + (2 * this.level);
        this.experience = 0;
        this.items = new HashMap<>();
        this.items.put("health_potion", 3);
        this.items.put("mana_potion", 2);
        this.allies = new Character[]{};
    }

    void takeDamage(int damage) {
        this.health -= damage;
        if (this.health < 0) {
            this.health = 0;
        }
    }

    boolean isAlive() {
        return this.health > 0;
    }

    void levelUp() {
        this.level++;
        this.maxHealth += 10;
        this.attack += 2;
        this.health = this.maxHealth;
        System.out.println(this.name + " leveled up to level " + this.level + "!");
    }

    void gainExperience(int exp) {
        this.experience += exp;
        System.out.println(this.name + " gained " + exp + " experience points!");
        if (this.experience >= this.level * 100) {
            levelUp();
        }
    }

    void useItem(String item) {
        if (this.items.containsKey(item) && this.items.get(item) > 0) {
            if (item.equals("health_potion")) {
                int healthToRestore = Math.min(30, this.maxHealth - this.health);
                this.health += healthToRestore;
                this.items.put(item, this.items.get(item) - 1);
                System.out.println(this.name + " used a health potion and restored " + healthToRestore + " health!");
            } else if (item.equals("mana_potion")) {
                this.items.put(item, this.items.get(item) - 1);
                System.out.println(this.name + " used a mana potion!");
                // Add mana restoring logic here if needed
            }
        }
    }

    String status() {
        return this.name + "'s health: " + this.health + "/" + this.maxHealth + ", Level: " + this.level +
                ", Experience: " + this.experience + "/" + (this.level * 100) + ", Items: " + this.items;
    }

    void joinAlly(Character ally) {
        // Assuming that allies array will have fixed length
        Character[] temp = new Character[this.allies.length + 1];
        System.arraycopy(this.allies, 0, temp, 0, this.allies.length);
        temp[temp.length - 1] = ally;
        this.allies = temp;
        System.out.println(ally.name + " joined " + this.name + "'s party!");
    }
}

class Player extends Character {
    int gold;

    Player(String name, int level) {
        super(name, level);
        this.gold = 100;
    }

    @Override
    void takeDamage(int damage) {
        super.takeDamage(damage);
        if (this.attack < 0) {
            this.attack = 0;
        }
    }
}

class Enemy extends Character {
    String specialAbility;

    Enemy(String name, int level, String specialAbility) {
        super(name, level);
        this.specialAbility = specialAbility;
    }

    @Override
    void takeDamage(int damage) {
        if ("block".equals(this.specialAbility)) {
            System.out.println(this.name + " blocks the attack!");
        } else {
            super.takeDamage(damage);
        }
    }

    void attackTarget(Character target) {
        if ("stun".equals(this.specialAbility)) {
            System.out.println(this.name + " uses stun ability!");
            target.takeDamage(this.attack * 2);
        } else {
            target.takeDamage(Math.max(1, this.attack));
        }
    }
}

public class 미니RPG전투게임 {
    static Random random = new Random();

    static void randomEvent(Player player, Enemy enemy) {
        double eventChance = random.nextDouble(); // Generate a random number between 0 and 1
        if (eventChance < 0.2) { // 20% chance of an event occurring
            int event = random.nextInt(5) + 1;
            switch (event) {
                case 1:
                    System.out.println("A friendly wizard appears and heals you.");
                    player.health = Math.min(player.maxHealth, player.health + 20);
                    break;
                case 2:
                    System.out.println("You found a treasure chest! You gained 50 experience points.");
                    player.gainExperience(50);
                    break;
                case 3:
                    System.out.println("A curse befalls you, reducing your attack power!");
                    if (player.attack >= 5) {
                        player.attack -= 5;
                    } else {
                        player.attack = 0;
                    }
                    break;
                case 4:
                    System.out.println("You stumbled upon a hidden passage and found a health potion!");
                    player.useItem("health_potion");
                    break;
                default:
                    System.out.println("A mysterious mist surrounds you, confusing the enemy!");
                    if (enemy.attack >= 5) {
                        enemy.attack -= 5;
                    } else {
                        enemy.attack = 0;
                    }
                    break;
            }
        }
    }
    

    static void battle(Player player, Enemy enemy) {
        System.out.println("Battle Start!");
        while (player.isAlive() && enemy.isAlive()) {
            randomEvent(player, enemy);
            int playerDamage = random.nextInt(Math.max(1, player.attack)) + 1; // Ensure player's attack is at least 1
            int enemyDamage = random.nextInt(Math.max(1, enemy.attack)) + 1; // Ensure enemy's attack is at least 1
    
            System.out.println(player.name + " attacks " + enemy.name + " for " + playerDamage + " damage.");
            enemy.takeDamage(playerDamage);
            System.out.println(enemy.name + " attacks " + player.name + " for " + enemyDamage + " damage.");
            player.takeDamage(enemyDamage);
    
            System.out.println(player.status());
            System.out.println(enemy.status());
            System.out.println();
        }
    
        if (!player.isAlive()) {
            System.out.println(enemy.name + " wins!");
        } else if (!enemy.isAlive()) {
            System.out.println(player.name + " wins!");
        }
    
        System.out.println("Game Over!");
    }
    

    public static void main(String[] args) {
        java.util.Scanner scanner = new java.util.Scanner(System.in);
        System.out.print("Enter player name: ");
        String playerName = scanner.nextLine();
        Player player = new Player(playerName, 1);

        String enemyName = "Evil Wizard";
        Enemy enemy = new Enemy(enemyName, random.nextInt(3) + 3, "block");

        player.joinAlly(new Character("Knight", player.level));
        player.joinAlly(new Character("Archer", player.level));

        battle(player, enemy);

        System.out.println("Game Over!");
        System.out.println(player.status());
        System.out.println(enemy.status());

        scanner.close();
    }
}

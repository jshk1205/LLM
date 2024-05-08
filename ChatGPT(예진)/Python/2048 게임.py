import pygame
import random

# 화면 설정
WIDTH = 400
HEIGHT = 400
GRID_SIZE = 4
CELL_SIZE = WIDTH // GRID_SIZE
BACKGROUND_COLOR = (187, 173, 160)
GRID_COLOR = (205, 193, 180)
FONT_COLOR = (255, 255, 255)

# 게임 상태
class GameState:
    def __init__(self):
        self.grid = [[0] * GRID_SIZE for _ in range(GRID_SIZE)]
        self.add_new_tile()
        self.add_new_tile()

    def add_new_tile(self):
        empty_cells = [(x, y) for x in range(GRID_SIZE) for y in range(GRID_SIZE) if self.grid[x][y] == 0]
        if empty_cells:
            x, y = random.choice(empty_cells)
            self.grid[x][y] = 2 if random.random() < 0.9 else 4

    def move(self, direction):
        if direction == "up":
            self.grid = [list(row) for row in zip(*self.grid)]
            self.grid = [self.merge(row) for row in self.grid]
            self.grid = [list(row) for row in zip(*self.grid)]
        elif direction == "down":
            self.grid = [list(row[::-1]) for row in zip(*self.grid)]
            self.grid = [self.merge(row) for row in self.grid]
            self.grid = [list(row[::-1]) for row in zip(*self.grid)]
        elif direction == "left":
            self.grid = [self.merge(row) for row in self.grid]
        elif direction == "right":
            self.grid = [row[::-1] for row in self.grid]
            self.grid = [self.merge(row) for row in self.grid]
            self.grid = [row[::-1] for row in self.grid]

    def merge(self, row):
        new_row = [val for val in row if val != 0]
        for i in range(len(new_row) - 1):
            if new_row[i] == new_row[i + 1]:
                new_row[i] *= 2
                new_row[i + 1] = 0
        new_row = [val for val in new_row if val != 0]
        return new_row + [0] * (GRID_SIZE - len(new_row))

    def is_game_over(self):
        for x in range(GRID_SIZE):
            for y in range(GRID_SIZE):
                if self.grid[x][y] == 0:
                    return False
                if y < GRID_SIZE - 1 and self.grid[x][y] == self.grid[x][y + 1]:
                    return False
                if x < GRID_SIZE - 1 and self.grid[x][y] == self.grid[x + 1][y]:
                    return False
        return True

# 화면 업데이트
def draw_text(surface, text, size, x, y, color):
    font = pygame.font.SysFont(None, size)
    text_surface = font.render(text, True, color)
    text_rect = text_surface.get_rect()
    text_rect.center = (x, y)
    surface.blit(text_surface, text_rect)

def draw_grid(surface, grid):
    for x in range(GRID_SIZE):
        for y in range(GRID_SIZE):
            pygame.draw.rect(surface, GRID_COLOR, (x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE))
            value = grid[x][y]
            if value != 0:
                text = str(value)
                text_size = 36 if len(text) == 1 else 24
                draw_text(surface, text, text_size, x * CELL_SIZE + CELL_SIZE // 2, y * CELL_SIZE + CELL_SIZE // 2, FONT_COLOR)

def main():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("2048 Game")
    clock = pygame.time.Clock()
    game_state = GameState()

    running = True
    while running:
        screen.fill(BACKGROUND_COLOR)
        draw_grid(screen, game_state.grid)
        pygame.display.flip()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_UP:
                    game_state.move("up")
                    game_state.add_new_tile()
                elif event.key == pygame.K_DOWN:
                    game_state.move("down")
                    game_state.add_new_tile()
                elif event.key == pygame.K_LEFT:
                    game_state.move("left")
                    game_state.add_new_tile()
                elif event.key == pygame.K_RIGHT:
                    game_state.move("right")
                    game_state.add_new_tile()

        if game_state.is_game_over():
            running = False

        clock.tick(10)

    pygame.quit()

if __name__ == "__main__":
    main()

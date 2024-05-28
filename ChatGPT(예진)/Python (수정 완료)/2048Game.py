import pygame
import random

# 게임 자동 종료 기능 없음
# 게임 종료하는 기능을 축하하려고 하면 게임 실행 과정에서 오류가 계속 생기고, 오류를 수정해달라고 해도 제대로 반환해주지 않음

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
GRAY = (100, 100, 100)
DARK_GRAY = (40, 40, 40)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
YELLOW = (255, 255, 0)

SCREEN_WIDTH = 400
SCREEN_HEIGHT = 500
TILE_SIZE = 100
MARGIN = 10
GRID_SIZE = 4

def initialize_grid():
    return [[0] * GRID_SIZE for _ in range(GRID_SIZE)]

def add_new_tile(grid):
    empty_cells = [(i, j) for i in range(GRID_SIZE) for j in range(GRID_SIZE) if grid[i][j] == 0]
    if empty_cells:
        i, j = random.choice(empty_cells)
        grid[i][j] = random.choices([2, 4], [0.9, 0.1])[0]

def draw_grid(screen, grid):
    screen.fill(WHITE)
    for i in range(GRID_SIZE):
        for j in range(GRID_SIZE):
            pygame.draw.rect(screen, GRAY, (j * (TILE_SIZE + MARGIN), i * (TILE_SIZE + MARGIN), TILE_SIZE, TILE_SIZE))
            if grid[i][j] != 0:
                font = pygame.font.SysFont(None, 48)
                text = font.render(str(grid[i][j]), True, BLACK)
                text_rect = text.get_rect(center=(j * (TILE_SIZE + MARGIN) + TILE_SIZE / 2,
                                                   i * (TILE_SIZE + MARGIN) + TILE_SIZE / 2))
                screen.blit(text, text_rect)

def move_tiles(grid, direction):
    if direction == 'up':
        for j in range(GRID_SIZE):
            for i in range(1, GRID_SIZE):
                if grid[i][j] != 0:
                    for k in range(i, 0, -1):
                        if grid[k - 1][j] == 0:
                            grid[k - 1][j], grid[k][j] = grid[k][j], grid[k - 1][j]
                        elif grid[k - 1][j] == grid[k][j]:
                            grid[k - 1][j] *= 2
                            grid[k][j] = 0
                            break
    elif direction == 'down':
        for j in range(GRID_SIZE):
            for i in range(GRID_SIZE - 2, -1, -1):
                if grid[i][j] != 0:
                    for k in range(i, GRID_SIZE - 1):
                        if grid[k + 1][j] == 0:
                            grid[k + 1][j], grid[k][j] = grid[k][j], grid[k + 1][j]
                        elif grid[k + 1][j] == grid[k][j]:
                            grid[k + 1][j] *= 2
                            grid[k][j] = 0
                            break
    elif direction == 'left':
        for i in range(GRID_SIZE):
            for j in range(1, GRID_SIZE):
                if grid[i][j] != 0:
                    for k in range(j, 0, -1):
                        if grid[i][k - 1] == 0:
                            grid[i][k - 1], grid[i][k] = grid[i][k], grid[i][k - 1]
                        elif grid[i][k - 1] == grid[i][k]:
                            grid[i][k - 1] *= 2
                            grid[i][k] = 0
                            break
    elif direction == 'right':
        for i in range(GRID_SIZE):
            for j in range(GRID_SIZE - 2, -1, -1):
                if grid[i][j] != 0:
                    for k in range(j, GRID_SIZE - 1):
                        if grid[i][k + 1] == 0:
                            grid[i][k + 1], grid[i][k] = grid[i][k], grid[i][k + 1]
                        elif grid[i][k + 1] == grid[i][k]:
                            grid[i][k + 1] *= 2
                            grid[i][k] = 0
                            break

def main():
    pygame.init()
    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
    pygame.display.set_caption("2048 Game")
    clock = pygame.time.Clock()

    grid = initialize_grid()
    add_new_tile(grid)
    add_new_tile(grid)

    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_UP:
                    move_tiles(grid, 'up')
                    add_new_tile(grid)
                elif event.key == pygame.K_DOWN:
                    move_tiles(grid, 'down')
                    add_new_tile(grid)
                elif event.key == pygame.K_LEFT:
                    move_tiles(grid, 'left')
                    add_new_tile(grid)
                elif event.key == pygame.K_RIGHT:
                    move_tiles(grid, 'right')
                    add_new_tile(grid)

        draw_grid(screen, grid)
        pygame.display.flip()
        clock.tick(60)

    pygame.quit()

if __name__ == "__main__":
    main()

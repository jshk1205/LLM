# Python으로 테트티스 게임을 만들어줘
# 1. 게임판은 10x20
# 2. 블록은 7개
# 3. 블록은 회전 가능
# 4. 블록이 바닥에 닿으면 새로운 블록이 나타남
# 5. 가로로 꽉 차면 그 줄이 사라짐
# 6. 레벨은 10단계
# 7. 레벨이 올라갈수록 블록이 떨어지는 속도가 빨라짐
# 8. 레벨이 올라갈수록 점수가 높아짐
# 9. 게임 오버가 되면 게임 종료
# 10. 게임 종료 후 최고 점수를 보여줌
# 11. 게임 종료 후 다시 시작하거나 종료할 수 있음
# 12. 게임 종료 후 최고 점수를 파일로 저장함
# 13. 게임 시작 시 최고 점수를 파일에서 불러옴

import pygame
import random
import time
import sys
import os
import json
from pygame.locals import *

BLOCKS = [
    [[1, 1, 1, 1]],  # I
    [[1, 1, 1, 0], [0, 0, 1, 0]],  # J
    [[1, 1, 1, 0], [1, 0, 0, 0]],  # L
    [[1, 1, 0, 0], [1, 1, 0, 0]],  # O
    [[0, 1, 1, 0], [1, 1, 0, 0]],  # S
    [[0, 1, 0, 0], [1, 1, 1, 0]],  # T
    [[1, 1, 0, 0], [0, 1, 1, 0]],  # Z
]

COLORS = [
    (0, 255, 255),  # I
    (0, 0, 255),  # J
    (255, 165, 0),  # L
    (255, 255, 0),  # O
    (0, 255, 0),  # S
    (128, 0, 128),  # T
    (255, 0, 0),  # Z
]

WIDTH = 10
HEIGHT = 20
START_X = WIDTH // 2 - 2
START_Y = 0
START_R = 0

class Tetris:
    def __init__(self):
        pygame.init()
        self.screen = pygame.display.set_mode((WIDTH * 30, HEIGHT * 30))
        pygame.display.set_caption('Tetris')
        self.clock = pygame.time.Clock()
        self.font = pygame.font.Font(None, 36)
        self.load_score()
        self.init_game()

    def init_game(self):
        self.board = [[0 for _ in range(WIDTH)] for _ in range(HEIGHT)]
        self.score = 0
        self.level = 1
        self.lines = 0
        self.new_block()
        self.playing = True

    def new_block(self):
        self.x = START_X
        self.y = START_Y
        self.r = START_R
        self.block = random.choice(BLOCKS)
        self.color = random.choice(COLORS)

        if self.check_collision(self.x, self.y, self.r):
            self.playing = False

    def check_collision(self, x, y, r):
        for dy in range(len(self.block)):
            for dx in range(len(self.block[0])):
                if self.block[dy][dx]:
                    nx, ny = x + dx, y + dy
                    if nx < 0 or nx >= WIDTH or ny >= HEIGHT or self.board[ny][nx]:
                        return True
        return False

    def rotate_block(self):
        # 블록의 형태를 회전시킨 결과를 계산합니다.
        rotated_block = [list(row) for row in zip(*self.block[::-1])]
    
        # 회전된 블록이 충돌하는지 확인합니다.
        if not self.check_collision(self.x, self.y, rotated_block):
        # 충돌하지 않는 경우, 블록을 업데이트합니다.
            self.block = rotated_block
        # 충돌하는 경우, 회전을 취소하고 아무것도 하지 않습니다.
    def move(self, dx, dy):
        if not self.check_collision(self.x + dx, self.y + dy, self.r):
            self.x += dx
            self.y += dy
        else:
            if dy:
                self.merge()

    def merge(self):
        for dy in range(len(self.block)):
            for dx in range(len(self.block[0])):
                if self.block[dy][dx]:
                    nx, ny = self.x + dx, self.y + dy
                    self.board[ny][nx] = self.color

        lines = 0
        for y, row in enumerate(self.board):
            if all(row):
                del self.board[y]
                self.board.insert(0, [0 for _ in range(WIDTH)])
                lines += 1

        self.score += lines ** 2
        self.lines += lines
        self.level = 1 + self.lines // 10

        self.new_block()

    def load_score(self):
        if os.path.exists('score.json'):
            with open('score.json', 'r') as f:
                self.high_score = json.load(f)
        else:
            self.high_score = 0

    def save_score(self):
        if self.score > self.high_score:
            self.high_score = self.score
            with open('score.json', 'w') as f:
                json.dump(self.high_score, f)

    def run(self):
        while self.playing:
            self.update()
            self.draw()
            pygame.display.flip()
            self.clock.tick(10 * self.level)
        self.game_over()

    def update(self):
        for event in pygame.event.get():
            if event.type == QUIT:
                self.save_score()
                pygame.quit()
                sys.exit()
            elif event.type == KEYDOWN:
                if event.key == K_UP:
                    self.rotate_block()
                elif event.key == K_DOWN:
                    self.move(0, 1)
                elif event.key == K_LEFT:
                    self.move(-1, 0)
                elif event.key == K_RIGHT:
                    self.move(1, 0)

        self.move(0, 1)

    def draw(self):
        self.screen.fill((0, 0, 0))
        for y, row in enumerate(self.board):
            for x, cell in enumerate(row):
                if cell:
                    pygame.draw.rect(self.screen, cell, (x * 30, y * 30, 30, 30))

        for dy in range(len(self.block)):
            for dx in range(len(self.block[0])):
                if self.block[dy][dx]:
                    nx, ny = self.x + dx, self.y + dy
                    pygame.draw.rect(self.screen, self.color, (nx * 30, ny * 30, 30, 30))

        score_text = self.font.render(f'Score: {self.score}', True, (255, 255, 255))
        self.screen.blit(score_text, (10, 10))

        high_score_text = self.font.render(f'High Score: {self.high_score}', True, (255, 255, 255))
        self.screen.blit(high_score_text, (10, 40))

        level_text = self.font.render(f'Level: {self.level}', True, (255, 255, 255))
        self.screen.blit(level_text, (10, 70))

        lines_text = self.font.render(f'Lines: {self.lines}', True, (255, 255, 255))
        self.screen.blit(lines_text, (10, 100))

    def game_over(self):
        self.save_score()

        game_over_text = self.font.render('Game Over', True, (255, 255, 255))
        self.screen.blit(game_over_text, (WIDTH * 15 - 100, HEIGHT * 15 - 20))

        score_text = self.font.render(f'Score: {self.score}', True, (255, 255, 255))
        self.screen.blit(score_text, (WIDTH * 15 - 100, HEIGHT * 15 + 20))

        high_score_text = self.font.render(f'High Score: {self.high_score}', True, (255, 255, 255))
        self.screen.blit(high_score_text, (WIDTH * 15 - 100, HEIGHT * 15 + 60))

        pygame.display.flip()

        time.sleep(3)

        self.init_game()

if __name__ == '__main__':
    Tetris().run()
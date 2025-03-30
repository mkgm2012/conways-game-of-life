#include "raylib.h"
#include <vector>

const int screenWidth = 800;
const int screenHeight = 800;
int cellSize = 5;
int cols = screenWidth / cellSize;
int rows = screenHeight / cellSize;
bool isRunning = false;
bool isPaused = true;
Vector2 cameraOffset = {0, 0};
float zoomFactor = 1.0f;

std::vector<std::vector<bool>> grid;
std::vector<std::vector<bool>> nextGrid;

int simulationFPS = 12;
float simulationTimer = 0.0f;
float timePerSimulationStep = 1.0f / simulationFPS;

void InitializeGrid() {
    grid.assign(rows, std::vector<bool>(cols, false));
    nextGrid.assign(rows, std::vector<bool>(cols, false));
}

void RandomizeGrid() {
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            grid[y][x] = GetRandomValue(0, 1);
        }
    }
}

void ClearGrid() {
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            grid[y][x] = false;
        }
    }
}

int CountNeighbors(int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int nx = x + j;
            int ny = y + i;
            if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
                count += grid[ny][nx];
            }
        }
    }
    return count;
}

void UpdateGrid() {
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            int neighbors = CountNeighbors(x, y);
            if (grid[y][x]) {
                nextGrid[y][x] = (neighbors == 2 || neighbors == 3);
            } else {
                nextGrid[y][x] = (neighbors == 3);
            }
        }
    }
    grid.swap(nextGrid);
}

void DrawGrid() {
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (grid[y][x]) {
                DrawRectangle(x * cellSize * zoomFactor + cameraOffset.x, y * cellSize * zoomFactor + cameraOffset.y, cellSize * zoomFactor, cellSize * zoomFactor, GREEN);
            }
            DrawRectangleLines(x * cellSize * zoomFactor + cameraOffset.x, y * cellSize * zoomFactor + cameraOffset.y, cellSize * zoomFactor, cellSize * zoomFactor, Color{50, 50, 50, 255});
        }
    }
}

int main() {
    InitWindow(screenWidth, screenHeight, "Game of Life");
    SetTargetFPS(60);
    InitializeGrid();
    RandomizeGrid();

    while (!WindowShouldClose()) {
        simulationTimer += GetFrameTime();

        if (simulationTimer >= timePerSimulationStep) {
            simulationTimer = 0.0f;

            if (isRunning) {
                UpdateGrid();
            }
        }

        if (IsKeyPressed(KEY_SPACE)) isRunning = !isRunning;
        if (IsKeyPressed(KEY_C)) ClearGrid();
        if (IsKeyPressed(KEY_R)) RandomizeGrid();

        Vector2 mousePos = GetMousePosition();
        int x = (mousePos.x - cameraOffset.x) / (cellSize * zoomFactor);
        int y = (mousePos.y - cameraOffset.y) / (cellSize * zoomFactor);

        if (x >= 0 && x < cols && y >= 0 && y < rows) {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && isPaused) {
                grid[y][x] = true;
            }
            if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && isPaused) {
                grid[y][x] = false;
            }
        }

        if (IsKeyPressed(KEY_UP)) zoomFactor *= 1.1f;
        if (IsKeyPressed(KEY_DOWN)) zoomFactor *= 0.9f;
        if (zoomFactor < 0.5f) zoomFactor = 0.5f;
        if (zoomFactor > 5.0f) zoomFactor = 5.0f;

        if (IsKeyDown(KEY_W)) cameraOffset.y += 10;
        if (IsKeyDown(KEY_S)) cameraOffset.y -= 10;
        if (IsKeyDown(KEY_A)) cameraOffset.x += 10;
        if (IsKeyDown(KEY_D)) cameraOffset.x -= 10;

        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawGrid();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

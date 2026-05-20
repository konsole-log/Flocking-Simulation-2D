#include "flock.h"
#include <ctime>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

static void DrawSlider(const char *label, float *value, float minVal,
                       float maxVal, int panelX, int row) {
  const int PADDING = 20;
  const int LABEL_H = 22;
  const int SLIDER_H = 20;
  const int VALUE_W = 52;
  const int ROW_HEIGHT = 70;

  int y = 30 + row * ROW_HEIGHT;
  int sliderW = PANEL_WIDTH - PADDING * 2 - VALUE_W - 6;

  GuiLabel({(float)(panelX + PADDING), (float)y,
            (float)(PANEL_WIDTH - PADDING * 2), (float)LABEL_H},
           label);

  GuiSliderBar({(float)(panelX + PADDING), (float)(y + LABEL_H + 4),
                (float)sliderW, (float)SLIDER_H},
               NULL, NULL, value, minVal, maxVal);

  DrawText(TextFormat("%.1f", *value), panelX + PADDING + sliderW + 8,
           y + LABEL_H + 6, 16, LIGHTGRAY);
}

int main() {
  InitWindow(width, height, "FLOCKING SYSTEM 2D");
  SetTargetFPS(60);
  SetRandomSeed((unsigned int)time(NULL));

  GuiLoadStyle("assets/style_dark.rgs");

  Flock flock;
  for (int i = 0; i < 300; i++)
    flock.addBoid(Boid({viewportWidth / 2.0f, height / 2.0f}));

  while (!WindowShouldClose()) {
    flock.run();

    BeginDrawing();
    ClearBackground({24, 24, 24, 255});

    BeginScissorMode(0, 0, viewportWidth, height);
    DrawRectangle(0, 0, viewportWidth, height, BLACK);
    flock.draw();
    EndScissorMode();

    int panelX = width - PANEL_WIDTH;
    DrawRectangle(panelX, 0, PANEL_WIDTH, height, {28, 28, 30, 255});
    DrawRectangle(panelX, 0, 1, height, {60, 60, 60, 255});

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    GuiLabel({(float)(panelX + 20), 6, (float)(PANEL_WIDTH - 40), 26},
             "FLOCK SETTINGS");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);

    DrawLine(panelX + 20, 32, width - 20, 32, {60, 60, 60, 255});

    DrawSlider("Alignment Weight", &flock.params.alignmentWeight, 0.0f, 3.0f,
               panelX, 0);
    DrawSlider("Separation Radius", &flock.params.desiredSeparation, 5.0f,
               80.0f, panelX, 1);
    DrawSlider("Neighbour Distance", &flock.params.neighborDistance, 10.0f,
               150.0f, panelX, 2);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
#include <functional>
#include <iostream>
#include <random>
#include <vector>

#include "my_camera/my_camera.h"
#include "raylib.h"
#include "raymath.h"
#include "element/element.h"

using namespace std;
using std::string;

struct ElementRenderData {
  const Element *element;
  Texture2D label;
  Rectangle srcRect;
};

Color getRandomColor();

void drawElement(const Element *element,
                 Vector3 spherePosition,
                 Color color);

void drawElementBillboard(const Camera3D *camera,
                          const ElementRenderData *data,
                          Vector3 labelPosition);

int main() {
  constexpr int screenWidth = 800;
  constexpr int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");

  SetTraceLogLevel(LOG_ERROR);

  auto *myCam = new MyCamera(
    Vector3{10.0f, 10.0f, 10.0f},
    Vector3{0.0f, 0.0f, 0.0f},
    45.0f);

  Font font = GetFontDefault();

  std::vector<Element> elements = Element::processElements();

  std::vector<ElementRenderData> renderData;

  const size_t TITLE_FONT_SIZE = 50;
  const size_t SUBTITLE_FONT_SIZE = 25;

  auto imgSize = Vector2{150, 150};

  auto initialPosition = Vector2{5, 5};
  auto horizontalCenter = Vector2{imgSize.x / 2, 0};
  auto verticalCenter = Vector2{0, imgSize.y / 2};
  auto center = Vector2Add(horizontalCenter, verticalCenter);

  for (int i = 0; i < elements.size(); i++) {
    auto &element = elements.at(i);

    Image img = GenImageColor(static_cast<int>(imgSize.x), static_cast<int>(imgSize.y), WHITE);
    ImageDrawTextEx(&img, font, to_string(element.getAtomicNumber()).c_str(), initialPosition, SUBTITLE_FONT_SIZE, 1,
                    BLACK);
    ImageDrawTextEx(&img, font, element.getSymbol().c_str(), Vector2SubtractValue(center, SUBTITLE_FONT_SIZE),
                    TITLE_FONT_SIZE, 1, BLACK);
    ImageDrawTextEx(&img, font, element.getName().c_str(), Vector2Add(verticalCenter, Vector2{2, TITLE_FONT_SIZE}),
                    SUBTITLE_FONT_SIZE, 1, BLACK);
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);

    renderData.push_back(ElementRenderData{
      &element, texture, {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)}
    });

    auto pos = Vector3{0, 0, 0};

    if (i > 0) {
      auto &previous_element = elements.at(i - 1);
      float previous_radius = previous_element.getRadius();
      float current_radius = element.getRadius();
      pos.x = previous_element.getPosition().x + current_radius + previous_radius + 0.5f;
    }

    element.setPosition(pos);
    element.setColor(getRandomColor());
  }

  DisableCursor();

  SetTargetFPS(70);

  while (!WindowShouldClose()) {
    UpdateCamera(myCam->getCamera(), CAMERA_FREE);

    if (IsKeyPressed('Z')) {
      myCam->getCamera()->target = Vector3{0.0f, 0.0f, 0.0f};
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(*myCam->getCamera());
    for (auto element: elements) {
      element.draw();
      //ElementRenderData data = renderData.at(i);
      //drawElementBillboard(myCam->getCamera(), &data, Vector3Add(spherePosition, Vector3{0, 1.5, 0}));
    }

    // DrawGrid(10, 1.0f);
    EndMode3D();

    DrawRectangle(10, 10, 320, 93, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(10, 10, 320, 93, BLUE);

    DrawText("Free camera default controls:", 20, 20, 10, BLACK);
    DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
    DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
    DrawText("- Z to zoom to (0, 0, 0)", 40, 80, 10, DARKGRAY);
    EndDrawing();
  }

  for (const auto &data: renderData) {
    UnloadTexture(data.label);
  }
  CloseWindow();

  return 0;
}

void drawElement(const Element *element,
                 const Vector3 spherePosition,
                 const Color color) {
  DrawSphere(spherePosition, element->getRadius(), color);
}

void drawElementBillboard(const Camera3D *camera,
                          const ElementRenderData *data,
                          const Vector3 labelPosition) {
  DrawBillboardRec(*camera, data->label, data->srcRect, labelPosition, Vector2{1.0f, 1.0f}, GREEN);
}

Color getRandomColor() {
  constexpr int min = 0;
  constexpr int max = 255;

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<> dist(min, max);
  return Color{
    static_cast<unsigned char>(dist(mt)),
    static_cast<unsigned char>(dist(mt)),
    static_cast<unsigned char>(dist(mt)),
    255
  };
}

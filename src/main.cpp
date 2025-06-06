#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

#include "my_camera/my_camera.h"
#include "raylib.h"
#include "raymath.h"
#include "element/element.h"

using namespace std;
using std::string;

struct ElementRenderData
{
    const Element *element;
    Texture2D label;
    Rectangle srcRect;
};

Color getRandomColor();
void drawElement(const Element *element,
                 const Vector3 spherePosition,
                 Color color);

void drawElementBillboard(const Camera3D *camera,
                          const ElementRenderData *data, 
                          const Vector3 labelPosition);

int main()
{
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");

    SetTraceLogLevel(LOG_ERROR);

    MyCamera *myCam = new MyCamera(
        Vector3{10.0f, 10.0f, 10.0f},
        Vector3{0.0f, 0.0f, 0.0f},
        45.0f);

    Font font = GetFontDefault();

    std::vector<Element> elements = Element::processElements();

    std::vector<ElementRenderData> renderData;
    std::vector<Vector3> positions;

    const size_t TITLE_FONT_SIZE = 50;
    const size_t SUBTITLE_FONT_SIZE = 25;

    auto imgSize = Vector2{150, 150};

    auto initialPosition = Vector2{5, 5};
    auto horizontalCenter = Vector2{(float)imgSize.x / 2, 0};
    auto verticalCenter = Vector2{0, (float)imgSize.y / 2};
    auto center = Vector2Add(horizontalCenter, verticalCenter);

    for (uint8_t i = 0; i < elements.size(); i++)
    {
        auto &element = elements.at(i);

        Image img = GenImageColor(imgSize.x, imgSize.y, WHITE);
        ImageDrawTextEx(&img, font, to_string(element.getAtomicNumber()).c_str(), initialPosition, SUBTITLE_FONT_SIZE, 1, BLACK);
        ImageDrawTextEx(&img, font, element.getSymbol().c_str(), Vector2SubtractValue(center, SUBTITLE_FONT_SIZE), TITLE_FONT_SIZE, 1, BLACK);
        ImageDrawTextEx(&img, font, element.getName().c_str(), Vector2Add(verticalCenter, Vector2{2, TITLE_FONT_SIZE}), SUBTITLE_FONT_SIZE, 1, BLACK);
        Texture2D texture = LoadTextureFromImage(img);
        UnloadImage(img);

        renderData.push_back(ElementRenderData{&element, texture, {0, 0, (float)texture.width, (float)texture.height}});
        
        float previous_pos = 0;
        auto pos = Vector3{0, 0, 0};

        if (!positions.empty())
        {
            float previous_radius = elements.at(i-1).getRadius();
            float current_radius = element.getRadius();
            pos.x = positions.at(i-1).x + current_radius + previous_radius + 0.5; 
        }

        element.setPosition(pos);
        element.setColor(getRandomColor());

        //auto pos = Vector3{element.getRadius() + previous_pos, 0, 0};
        positions.push_back(pos);
    }

    for (uint8_t i = 0; i < positions.size(); i++)
    {
        const auto element = elements.at(i);
        const auto pos = positions.at(i);
        cout << "Element: " << element.getName() << ", x: " << pos.x << endl;
    }

    DisableCursor();

    SetTargetFPS(70);

    while (!WindowShouldClose())
    {
        UpdateCamera(myCam->getCamera(), CAMERA_FREE);

        if (IsKeyPressed('Z'))
        {
            myCam->getCamera()->target = Vector3{0.0f, 0.0f, 0.0f};
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(*(myCam->getCamera()));
        int x = 0;
        for (uint8_t i = 0; i < elements.size(); i++)
        {
            auto element = elements.at(i);
            ElementRenderData data = renderData.at(i);
            //auto x = static_cast<float>(element.getAtomicNumber()) + (element.getAtomicRadius() / 100) * i;
            element.draw();
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

    for (const auto &data : renderData)
    {
        UnloadTexture(data.label);
    }
    CloseWindow();

    return 0;
}

void drawElement(const Element *element,
                 const Vector3 spherePosition,
                 Color color)
{
    DrawSphere(spherePosition, element->getRadius(), color);
}

void drawElementBillboard(const Camera3D *camera,
                          const ElementRenderData *data, 
                          const Vector3 labelPosition)
{
    DrawBillboardRec(*camera, data->label, data->srcRect, labelPosition, Vector2{1.0f, 1.0f}, GREEN);
}

Color getRandomColor()
{
    int min = 0;
    int max = 255;
    return Color{
        static_cast<unsigned char>((unsigned char)rand() % (max - min + 1) + min),
        static_cast<unsigned char>((unsigned char)rand() % (max - min + 1) + min),
        static_cast<unsigned char>((unsigned char)rand() % (max - min + 1) + min),
        255};
}
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "imgui.h"
#include "imgui_impl_raylib.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Try Hard Wings");
    SetTargetFPS(144);

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//    ImGui::StyleColorsDark();

    ImGui_ImplRaylib_Init(screenWidth, screenHeight);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        //ImGui
        ImGui_ImplRaylib_NewFrame();
        ImGui_ImplRaylib_ProcessEvent();
        ImGui::NewFrame();

        ClearBackground(WHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        ImGui::ShowDemoWindow();

        ImGui::Render();
        raylib_render_cimgui(ImGui::GetDrawData());
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
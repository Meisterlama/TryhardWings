#ifndef CIMGUI_RAYLIB_H
#define CIMGUI_RAYLIB_H

#include <imconfig.h>
#include <imgui.h>
#include <float.h>
#include <raylib.h>

static double g_Time = 0.0;

static const char* ImGui_ImplRaylib_GetClipboardText(void* _)
{
    return GetClipboardText();
}

static void ImGui_ImplRaylib_SetClipboardText(void* _, const char* text)
{
    SetClipboardText(text);
}

bool ImGui_ImplRaylib_Init(int width, int height)
{
    rlEnableScissorTest();

    ImGui::GetIO().BackendPlatformName = "imgui_impl_raylib";

    ImGui::GetIO().KeyMap[ImGuiKey_Tab] = KEY_TAB;
    ImGui::GetIO().KeyMap[ImGuiKey_LeftArrow] = KEY_LEFT;
    ImGui::GetIO().KeyMap[ImGuiKey_RightArrow] = KEY_RIGHT;
    ImGui::GetIO().KeyMap[ImGuiKey_UpArrow] = KEY_UP;
    ImGui::GetIO().KeyMap[ImGuiKey_DownArrow] = KEY_DOWN;
    ImGui::GetIO().KeyMap[ImGuiKey_PageUp] = KEY_PAGE_DOWN;
    ImGui::GetIO().KeyMap[ImGuiKey_PageDown] = KEY_PAGE_UP;
    ImGui::GetIO().KeyMap[ImGuiKey_Home] = KEY_HOME;
    ImGui::GetIO().KeyMap[ImGuiKey_End] = KEY_END;
    ImGui::GetIO().KeyMap[ImGuiKey_Insert] = KEY_INSERT;
    ImGui::GetIO().KeyMap[ImGuiKey_Delete] = KEY_DELETE;
    ImGui::GetIO().KeyMap[ImGuiKey_Backspace] = KEY_BACKSPACE;
    ImGui::GetIO().KeyMap[ImGuiKey_Space] = KEY_SPACE;
    ImGui::GetIO().KeyMap[ImGuiKey_Enter] = KEY_ENTER;
    ImGui::GetIO().KeyMap[ImGuiKey_Escape] = KEY_ESCAPE;
    ImGui::GetIO().KeyMap[ImGuiKey_KeyPadEnter] = KEY_KP_ENTER;
    ImGui::GetIO().KeyMap[ImGuiKey_A] = KEY_A;
    ImGui::GetIO().KeyMap[ImGuiKey_C] = KEY_C;
    ImGui::GetIO().KeyMap[ImGuiKey_V] = KEY_V;
    ImGui::GetIO().KeyMap[ImGuiKey_X] = KEY_X;
    ImGui::GetIO().KeyMap[ImGuiKey_Y] = KEY_Y;
    ImGui::GetIO().KeyMap[ImGuiKey_Z] = KEY_Z;

    ImGui::GetIO().MousePos = {-FLT_MAX, -FLT_MAX};
    ImGui::GetIO().SetClipboardTextFn = ImGui_ImplRaylib_SetClipboardText;
    ImGui::GetIO().GetClipboardTextFn = ImGui_ImplRaylib_GetClipboardText;
    ImGui::GetIO().ClipboardUserData = NULL;

    unsigned char *pixels = NULL;

    ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, NULL);
    // At this point you've got the texture data and you need to upload that your your graphic system:
    // After we have created the texture, store its pointer/identifier (_in whichever format your engine uses_) in 'io.Fonts->TexID'.
    // This will be passed back to your via the renderer. Basically ImTextureID == void*. Read FAQ for details about ImTextureID.

    Image image = GenImageColor(width, height, {1, 0, 0, 1});
//    for (int i = 0; i < width * height; i += 4)
//    {
//        ImageDrawPixel(&image, i/width, i%width, {pixels[i], pixels[i+1], pixels[i+2], pixels[i+3]});
//    }

//    Image image = LoadImageRaw((Color *)pixels, width, height);
    Texture2D texture = LoadTextureFromImage(image); //MyEngine::CreateTextureFromMemoryPixels(pixels, width, height, TEXTURE_TYPE_RGBA32)
    ImGui::GetIO().Fonts->SetTexID((ImTextureID)&texture.id);

    return true;
}

void ImGui_ImplRaylib_Shutdown()
{
    g_Time = 0.0;
}

static void ImGui_ImplRaylib_UpdateMouseCursor()
{
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (ImGui::GetIO().MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
    {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        HideCursor();
    }
    else
    {
        // Show OS mouse cursor
        ShowCursor();
    }
}

static void ImGui_ImplRaylib_UpdateMousePosAndButtons()
{
    // Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
    if (ImGui::GetIO().WantSetMousePos)
        SetMousePosition(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    else
        ImGui::GetIO().MousePos = {-FLT_MAX, -FLT_MAX};

    ImGui::GetIO().MouseDown[0] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    ImGui::GetIO().MouseDown[1] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
    ImGui::GetIO().MouseDown[2] = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);

    if (!IsWindowMinimized()){
        ImGui::GetIO().MousePos = {static_cast<float>(GetTouchX()), static_cast<float>(GetTouchY())};
    }
}

void ImGui_ImplRaylib_NewFrame()
{
    ImGui::GetIO().DisplaySize = {(float) GetScreenWidth(), (float) GetScreenHeight()};

    double current_time = GetTime();
     ImGui::GetIO().DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f/60.0f);
    g_Time = current_time;

     ImGui::GetIO().KeyCtrl = IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL);
     ImGui::GetIO().KeyShift = IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT);
     ImGui::GetIO().KeyAlt = IsKeyDown(KEY_RIGHT_ALT) || IsKeyDown(KEY_LEFT_ALT);
     ImGui::GetIO().KeySuper = IsKeyDown(KEY_RIGHT_SUPER) || IsKeyDown(KEY_LEFT_SUPER);

    ImGui_ImplRaylib_UpdateMousePosAndButtons();
    ImGui_ImplRaylib_UpdateMouseCursor();

    if (GetMouseWheelMove() > 0)
         ImGui::GetIO().MouseWheel += 1;
    else if (GetMouseWheelMove() < 0)
         ImGui::GetIO().MouseWheel -= 1;
}

#define FOR_ALL_KEYS(X) \
    X(KEY_APOSTROPHE); \
    X(KEY_COMMA); \
    X(KEY_MINUS); \
    X(KEY_PERIOD); \
    X(KEY_SLASH); \
    X(KEY_ZERO); \
    X(KEY_ONE); \
    X(KEY_TWO); \
    X(KEY_THREE); \
    X(KEY_FOUR); \
    X(KEY_FIVE); \
    X(KEY_SIX); \
    X(KEY_SEVEN); \
    X(KEY_EIGHT); \
    X(KEY_NINE); \
    X(KEY_SEMICOLON); \
    X(KEY_EQUAL); \
    X(KEY_A); \
    X(KEY_B); \
    X(KEY_C); \
    X(KEY_D); \
    X(KEY_E); \
    X(KEY_F); \
    X(KEY_G); \
    X(KEY_H); \
    X(KEY_I); \
    X(KEY_J); \
    X(KEY_K); \
    X(KEY_L); \
    X(KEY_M); \
    X(KEY_N); \
    X(KEY_O); \
    X(KEY_P); \
    X(KEY_Q); \
    X(KEY_R); \
    X(KEY_S); \
    X(KEY_T); \
    X(KEY_U); \
    X(KEY_V); \
    X(KEY_W); \
    X(KEY_X); \
    X(KEY_Y); \
    X(KEY_Z); \
    X(KEY_SPACE); \
    X(KEY_ESCAPE); \
    X(KEY_ENTER); \
    X(KEY_TAB); \
    X(KEY_BACKSPACE); \
    X(KEY_INSERT); \
    X(KEY_DELETE); \
    X(KEY_RIGHT); \
    X(KEY_LEFT); \
    X(KEY_DOWN); \
    X(KEY_UP); \
    X(KEY_PAGE_UP); \
    X(KEY_PAGE_DOWN); \
    X(KEY_HOME); \
    X(KEY_END); \
    X(KEY_CAPS_LOCK); \
    X(KEY_SCROLL_LOCK); \
    X(KEY_NUM_LOCK); \
    X(KEY_PRINT_SCREEN); \
    X(KEY_PAUSE); \
    X(KEY_F1); \
    X(KEY_F2); \
    X(KEY_F3); \
    X(KEY_F4); \
    X(KEY_F5); \
    X(KEY_F6); \
    X(KEY_F7); \
    X(KEY_F8); \
    X(KEY_F9); \
    X(KEY_F10); \
    X(KEY_F11); \
    X(KEY_F12); \
    X(KEY_LEFT_SHIFT); \
    X(KEY_LEFT_CONTROL); \
    X(KEY_LEFT_ALT); \
    X(KEY_LEFT_SUPER); \
    X(KEY_RIGHT_SHIFT); \
    X(KEY_RIGHT_CONTROL); \
    X(KEY_RIGHT_ALT); \
    X(KEY_RIGHT_SUPER); \
    X(KEY_KB_MENU); \
    X(KEY_LEFT_BRACKET); \
    X(KEY_BACKSLASH); \
    X(KEY_RIGHT_BRACKET); \
    X(KEY_GRAVE); \
    X(KEY_KP_0); \
    X(KEY_KP_1); \
    X(KEY_KP_2); \
    X(KEY_KP_3); \
    X(KEY_KP_4); \
    X(KEY_KP_5); \
    X(KEY_KP_6); \
    X(KEY_KP_7); \
    X(KEY_KP_8); \
    X(KEY_KP_9); \
    X(KEY_KP_DECIMAL); \
    X(KEY_KP_DIVIDE); \
    X(KEY_KP_MULTIPLY); \
    X(KEY_KP_SUBTRACT); \
    X(KEY_KP_ADD); \
    X(KEY_KP_ENTER); \
    X(KEY_KP_EQUAL);

#define SET_KEY_DOWN(KEY) ImGui::GetIO().KeysDown[KEY] = IsKeyDown(KEY)

bool ImGui_ImplRaylib_ProcessEvent()
{
    FOR_ALL_KEYS(SET_KEY_DOWN);

    int keyPressed = GetKeyPressed();
    if (keyPressed > 0)
    {
        ImGui::GetIO().AddInputCharacter(keyPressed);
    }
 

    return true;
}

void draw_triangle_vertex(ImDrawVert idx_vert)
{
    Color *c;
    c = (Color *)&idx_vert.col;
    rlColor4ub(c->r, c->g, c->b, c->a);
    rlTexCoord2f(idx_vert.uv.x, idx_vert.uv.y);
    rlVertex2f(idx_vert.pos.x, idx_vert.pos.y);
}

void raylib_render_draw_triangles(unsigned int count, const ImDrawIdx *idx_buffer, const ImDrawVert *idx_vert, unsigned int texture_id)
{
    // Draw the imgui triangle data
    for (unsigned int i = 0; i <= (count - 3); i += 3)
    {
        rlPushMatrix();
        rlBegin(RL_TRIANGLES);
        rlEnableTexture(texture_id);

        ImDrawIdx index;
        ImDrawVert vertex;

        index = idx_buffer[i];
        vertex = idx_vert[index];
        draw_triangle_vertex(vertex);

        index = idx_buffer[i + 2];
        vertex = idx_vert[index];
        draw_triangle_vertex(vertex);

        index = idx_buffer[i + 1];
        vertex = idx_vert[index];
        draw_triangle_vertex(vertex);
        rlDisableTexture();
        rlEnd();
        rlPopMatrix();
    }
}

void raylib_render_cimgui(ImDrawData *draw_data)
{
    rlDisableBackfaceCulling();
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];
        const ImDrawVert *vtx_buffer = cmd_list->VtxBuffer.Data; // vertex buffer generated by Dear ImGui
        const ImDrawIdx *idx_buffer = cmd_list->IdxBuffer.Data;  // index buffer generated by Dear ImGui
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd *pcmd = &(cmd_list->CmdBuffer.Data)[cmd_i]; // cmd_list->CmdBuffer->data[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                ImVec2 pos = draw_data->DisplayPos;
                int rectX = (int)(pcmd->ClipRect.x - pos.x);
                int rectY = (int)(pcmd->ClipRect.y - pos.y);
                int rectW = (int)(pcmd->ClipRect.z - rectX);
                int rectH = (int)(pcmd->ClipRect.w - rectY);
                BeginScissorMode(rectX, rectY, rectW, rectH);
                {
                    unsigned int *ti = (unsigned int*)(pcmd->TextureId);
                    raylib_render_draw_triangles(pcmd->ElemCount, idx_buffer, vtx_buffer, *ti);
                }
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
    EndScissorMode();
    rlEnableBackfaceCulling();
}

#endif
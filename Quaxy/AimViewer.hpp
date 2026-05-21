namespace Texture {
    extern ID3D11ShaderResourceView* previewSlow;
    extern ID3D11ShaderResourceView* rect;
}

class AimViewer {
public:
    void Draw() {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 boxPos, boxSize;
        boxPos.x = ImGui::GetWindowPos().x + 35;
        boxPos.y = ImGui::GetWindowPos().y + 73;
        boxSize.x = 210;
        boxSize.y = 345;

        ImVec2 imagePos(ImGui::GetWindowPos().x + 15, ImGui::GetWindowPos().y + 70);
        ImVec2 imageSize(imagePos.x + 250, imagePos.y + 350);
        drawList->AddImage(Images::PreviewSlow, imagePos, imageSize, ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(MenuSetting::TextActive));

        struct TargetPoint {
            ImVec2 pos;
            const char* label;
        };

        TargetPoint targetPoints[] = {
            { {boxPos.x + boxSize.x / 2, boxPos.y + 20},  XorString("Head") },
            { {boxPos.x + boxSize.x / 2, boxPos.y + 50},  XorString("Neck") },
            { {boxPos.x + boxSize.x / 2, boxPos.y + 100}, XorString("Chest") },
            { {boxPos.x + 15,             boxPos.y + 120}, XorString("Left Arm") },
            { {boxPos.x + boxSize.x - 15, boxPos.y + 120}, XorString("Right Arm") },
            { {boxPos.x + 65,             boxPos.y + boxSize.y - 20}, XorString("Left Leg") },
            { {boxPos.x + boxSize.x - 70, boxPos.y + boxSize.y - 20}, XorString("Right Leg") }
        };

        bool silentOn = Features::AimAssist::Silent::Enabled;
        bool aimbotOn = Features::AimAssist::Aimbot::Enabled;
        int silentMask = Features::AimAssist::Silent::SelectedArea;
        int aimbotMask = Features::AimAssist::Aimbot::SelectedArea;

        ImU32 silentColor = ImGui::GetColorU32({ 1,0,0,0.5f });
        ImU32 aimbotColor = ImGui::GetColorU32({ 0,0,1,0.5f });
        ImU32 textColor = ImGui::GetColorU32({ 1,1,1,1 });

        for (int i = 0; i < IM_ARRAYSIZE(targetPoints); i++) {
            bool silentSel = (silentMask & (1 << i)) != 0;
            bool aimbotSel = (aimbotMask & (1 << i)) != 0;
            if (!silentSel && !aimbotSel) continue;

            ImVec2 p = targetPoints[i].pos;

            if (silentOn && aimbotOn && silentSel && aimbotSel) {
                drawList->PathArcTo(p, 32, IM_PI / 2, 3 * IM_PI / 2, 32);
                drawList->PathLineTo(p);
                drawList->PathFillConvex(silentColor);
                drawList->PathArcTo(p, 32, -IM_PI / 2, IM_PI / 2, 32);
                drawList->PathLineTo(p);
                drawList->PathFillConvex(aimbotColor);

                ImVec2 ts = ImGui::CalcTextSize(XorString("Silent"));
                ImVec2 ta = ImGui::CalcTextSize(XorString("Aimbot"));
                drawList->AddText({ p.x - ts.x / 2, p.y - ts.y }, textColor, XorString("Silent"));
                drawList->AddText({ p.x - ta.x / 2, p.y }, textColor, XorString("Aimbot"));
            }
            else {
                if (silentOn && silentSel) {
                    drawList->PathArcTo(p, 32, 0, IM_PI * 2, 32);
                    drawList->PathFillConvex(silentColor);
                    ImVec2 t = ImGui::CalcTextSize(XorString("Silent"));
                    drawList->AddText({ p.x - t.x / 2, p.y - t.y / 2 }, textColor, XorString("Silent"));
                }
                if (aimbotOn && aimbotSel) {
                    drawList->PathArcTo(p, 32, 0, IM_PI * 2, 32);
                    drawList->PathFillConvex(aimbotColor);
                    ImVec2 t = ImGui::CalcTextSize(XorString("Aimbot"));
                    drawList->AddText({ p.x - t.x / 2, p.y - t.y / 2 }, textColor, XorString("Aimbot"));
                }
            }
        }
    }
} AimViewer;
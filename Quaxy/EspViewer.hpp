struct DemoUserstruct {
    int Id = 100;
    std::string Name = "User";
    std::string WeaponName = "Assault Rifle";
    int Distance = 300;
    int Health = 180;
    int Armor = 70;
}DemoUser;

namespace Texture {
    extern ID3D11ShaderResourceView* previewSlow;
    extern ID3D11ShaderResourceView* rect;
}

class EspViewer {
public:
    void DrawSkeleton(ImDrawList* drawList, const ImVec2& boxPos, const ImVec2& boxSize, bool drawOutline) {
        ImVec2 center(boxPos.x + boxSize.x / 2 - 2, boxPos.y + boxSize.y / 2);
        ImVec2 headTop(center.x, boxPos.y + 10);
        ImVec2 headBottom(center.x, boxPos.y + 50);
        ImVec2 bodyBottom(center.x, boxPos.y + 180);
        ImVec2 lShoulder(center.x - 30, headBottom.y + 20), rShoulder(center.x + 30, headBottom.y + 20);
        ImVec2 lElbow(center.x - 60, headBottom.y + 45), rElbow(center.x + 60, headBottom.y + 45);
        ImVec2 lHand(center.x - 105, headBottom.y + 70), rHand(center.x + 105, headBottom.y + 70);
        ImVec2 lHip(center.x - 20, bodyBottom.y), rHip(center.x + 20, bodyBottom.y);
        ImVec2 lFoot(center.x - 37, boxPos.y + 330), rFoot(center.x + 32, boxPos.y + 330);
        auto drawBone = [&](ImVec2 a, ImVec2 b, ImU32 color, float thickness) {
            drawList->AddLine(a, b, color, thickness);
        };

        ImU32 mainColor = Features::Players::VisualMarkers::DrawSkeleton::Color;
        float mainThickness = Features::Players::VisualMarkers::GlobalSettings::LineWeight;
        if (drawOutline) {
            ImU32 outlineColor = IM_COL32(0, 0, 0, 255);
            float outlineThickness = mainThickness + 2.0f;
            drawBone(headTop, headBottom, outlineColor, outlineThickness);
            drawBone(headBottom, bodyBottom, outlineColor, outlineThickness);
            drawBone(headBottom, lShoulder, outlineColor, outlineThickness);
            drawBone(headBottom, rShoulder, outlineColor, outlineThickness);
            drawBone(lShoulder, lElbow, outlineColor, outlineThickness);
            drawBone(rShoulder, rElbow, outlineColor, outlineThickness);
            drawBone(lElbow, lHand, outlineColor, outlineThickness);
            drawBone(rElbow, rHand, outlineColor, outlineThickness);
            drawBone(bodyBottom, lHip, outlineColor, outlineThickness);
            drawBone(bodyBottom, rHip, outlineColor, outlineThickness);
            drawBone(lHip, lFoot, outlineColor, outlineThickness);
            drawBone(rHip, rFoot, outlineColor, outlineThickness);
        }
        drawBone(headTop, headBottom, mainColor, mainThickness);
        drawBone(headBottom, bodyBottom, mainColor, mainThickness);
        drawBone(headBottom, lShoulder, mainColor, mainThickness);
        drawBone(headBottom, rShoulder, mainColor, mainThickness);
        drawBone(lShoulder, lElbow, mainColor, mainThickness);
        drawBone(rShoulder, rElbow, mainColor, mainThickness);
        drawBone(lElbow, lHand, mainColor, mainThickness);
        drawBone(rElbow, rHand, mainColor, mainThickness);
        drawBone(bodyBottom, lHip, mainColor, mainThickness);
        drawBone(bodyBottom, rHip, mainColor, mainThickness);
        drawBone(lHip, lFoot, mainColor, mainThickness);
        drawBone(rHip, rFoot, mainColor, mainThickness);
    }

    void DrawTextElement(ImDrawList* drawList, const ImVec2& pos, const ImVec2& size, const std::string& text, bool enabled, int locationType, float locationTop, float locationBottom, int font, int fontType, ImU32 color) {
        if (!enabled) {
            return;
        }

        float location = 0.0f;
        ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
        ImVec2 textPos;

        if (locationType == 0) {
            location = locationTop;
            textPos.x = pos.x + 5 + (size.x - textSize.x) * 0.5f;
            textPos.y = pos.y - textSize.y - location;
        }
        else if (locationType == 1) {
            location = locationBottom;
            textPos.x = pos.x + 5 + (size.x - textSize.x) * 0.5f;
            textPos.y = pos.y + size.y + location;
        }

        if (font == 0) { ImGui::PushFont(RobotoEsp); }
        else if (font == 1) { ImGui::PushFont(PriceDownEsp); }

        if (fontType == 0) {
            drawList->AddText(textPos, color, text.c_str());
        }
        else {
            ImVec2 outlineOffset = ImVec2(2.0f, 2.0f);
            drawList->AddText(ImVec2(textPos.x - outlineOffset.x, textPos.y - outlineOffset.y), ImColor(0, 0, 0, 255), text.c_str());
            drawList->AddText(ImVec2(textPos.x + outlineOffset.x, textPos.y - outlineOffset.y), ImColor(0, 0, 0, 255), text.c_str());
            drawList->AddText(ImVec2(textPos.x - outlineOffset.x, textPos.y + outlineOffset.y), ImColor(0, 0, 0, 255), text.c_str());
            drawList->AddText(ImVec2(textPos.x + outlineOffset.x, textPos.y + outlineOffset.y), ImColor(0, 0, 0, 255), text.c_str());
            drawList->AddText(ImVec2(textPos.x, textPos.y - outlineOffset.y), ImColor(0, 0, 0, 255), text.c_str());
            drawList->AddText(ImVec2(textPos.x, textPos.y + outlineOffset.y), ImColor(0, 0, 0, 255), text.c_str());
            drawList->AddText(ImVec2(textPos.x - outlineOffset.x, textPos.y), ImColor(0, 0, 0, 255), text.c_str());
            drawList->AddText(ImVec2(textPos.x + outlineOffset.x, textPos.y), ImColor(0, 0, 0, 255), text.c_str());
            drawList->AddText(textPos, color, text.c_str());
        }

        ImGui::PopFont();
    }

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

        if (Features::Players::VisualMarkers::DrawSkeleton::Enabled) {
            if (Features::Players::VisualMarkers::GlobalSettings::SelectedLineType == 0) {
                DrawSkeleton(drawList, boxPos, boxSize, false);
            }
            if (Features::Players::VisualMarkers::GlobalSettings::SelectedLineType == 1) {
                DrawSkeleton(drawList, boxPos, boxSize, true);
            }
        }

        if (Features::Players::VisualMarkers::DrawBox::Enabled) {
            if (Features::Players::VisualMarkers::DrawBox::SelectedType == 0) {
                ImU32 fillColor = IM_COL32(0, 0, 0, 50);
                ImU32 borderColor = IM_COL32(0, 0, 0, 255);
                ImVec2 newTopLeft = ImVec2(boxPos.x - (boxSize.x - boxSize.x) / 2.0f, boxPos.y);
                ImVec2 newBottomRight = ImVec2(newTopLeft.x + boxSize.x, boxPos.y + boxSize.y);
                drawList->AddRectFilled(newTopLeft, newBottomRight, fillColor);
                drawList->AddRect(newTopLeft, newBottomRight, borderColor, 0.0f, 0, 1.0f);
                float cornerSize = boxSize.x * 0.3f;
                drawList->AddLine(newTopLeft, ImVec2(newTopLeft.x + cornerSize, newTopLeft.y), Features::Players::VisualMarkers::DrawBox::Color, 2.0f);
                drawList->AddLine(newTopLeft, ImVec2(newTopLeft.x, newTopLeft.y + cornerSize), Features::Players::VisualMarkers::DrawBox::Color, 2.0f);
                drawList->AddLine(ImVec2(newBottomRight.x, newTopLeft.y), ImVec2(newBottomRight.x - cornerSize, newTopLeft.y), Features::Players::VisualMarkers::DrawBox::Color, 2.0f);
                drawList->AddLine(ImVec2(newBottomRight.x, newTopLeft.y), ImVec2(newBottomRight.x, newTopLeft.y + cornerSize), Features::Players::VisualMarkers::DrawBox::Color, 2.0f);
                drawList->AddLine(ImVec2(newTopLeft.x, newBottomRight.y), ImVec2(newTopLeft.x + cornerSize, newBottomRight.y), Features::Players::VisualMarkers::DrawBox::Color, 2.0f);
                drawList->AddLine(ImVec2(newTopLeft.x, newBottomRight.y), ImVec2(newTopLeft.x, newBottomRight.y - cornerSize), Features::Players::VisualMarkers::DrawBox::Color, 2.0f);
                drawList->AddLine(newBottomRight, ImVec2(newBottomRight.x - cornerSize, newBottomRight.y), Features::Players::VisualMarkers::DrawBox::Color, 2.0f);
                drawList->AddLine(newBottomRight, ImVec2(newBottomRight.x, newBottomRight.y - cornerSize), Features::Players::VisualMarkers::DrawBox::Color, 2.0f);
            }
            if (Features::Players::VisualMarkers::DrawBox::SelectedType == 1) {
                ImVec2 topLeft = boxPos;
                ImVec2 bottomRight = ImVec2(boxPos.x + boxSize.x, boxPos.y + boxSize.y);
                drawList->AddRect(topLeft, bottomRight, Features::Players::VisualMarkers::DrawBox::Color, 0.f, 0, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
            }
            else if (Features::Players::VisualMarkers::DrawBox::SelectedType == 2) {
                ImVec2 offset(5, -5);
                ImVec2 p1 = boxPos;
                ImVec2 p2(p1.x + boxSize.x, p1.y);
                ImVec2 p3(p2.x, p2.y + boxSize.y);
                ImVec2 p4(p1.x, p1.y + boxSize.y);
                ImVec2 p5(p1.x + offset.x, p1.y + offset.y);
                ImVec2 p6(p2.x + offset.x, p2.y + offset.y);
                ImVec2 p7(p3.x + offset.x, p3.y + offset.y);
                ImVec2 p8(p4.x + offset.x, p4.y + offset.y);
                auto drawLine = [&](ImVec2 a, ImVec2 b) {
                    drawList->AddLine(a, b, Features::Players::VisualMarkers::DrawBox::Color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
                };
                drawLine(p1, p2); drawLine(p2, p3); drawLine(p3, p4); drawLine(p4, p1);
                drawLine(p5, p6); drawLine(p6, p7); drawLine(p7, p8); drawLine(p8, p5);
                drawLine(p1, p5); drawLine(p2, p6); drawLine(p3, p7); drawLine(p4, p8);
            }
        }

        if (Features::Players::VisualMarkers::DrawLine::Enabled) {
            ImVec2 startPos;
            ImVec2 endPos;
            startPos.x = boxPos.x + boxSize.x / 2;
            startPos.y = boxPos.y + boxSize.y / 2;
            if (Features::Players::VisualMarkers::DrawLine::SelectedLocation == 0) {
                endPos.x = startPos.x;
                endPos.y = ImGui::GetWindowPos().y;
            }
            else if (Features::Players::VisualMarkers::DrawLine::SelectedLocation == 1) {
                endPos.x = ImGui::GetWindowPos().x;
                endPos.y = ImGui::GetWindowPos().y + ImGui::GetWindowSize().y / 2;
            }
            else if (Features::Players::VisualMarkers::DrawLine::SelectedLocation == 2) {
                endPos.x = startPos.x;
                endPos.y = ImGui::GetWindowPos().y + ImGui::GetWindowSize().y;
            }
            if (Features::Players::VisualMarkers::GlobalSettings::SelectedLineType == 1) {
                float outlineWeight = Features::Players::VisualMarkers::GlobalSettings::LineWeight + 2.0f;
                drawList->AddLine(startPos, endPos, IM_COL32(0, 0, 0, 255), outlineWeight);
            }
            drawList->AddLine(startPos, endPos, Features::Players::VisualMarkers::DrawLine::Color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
        }

        std::string topLabels[4];
        std::string bottomLabels[4];
        int topCount = 0;
        int bottomCount = 0;

        auto addLabel = [&](int location, const std::string& label) {
            if (location == 0)
                topLabels[topCount++] = label;
            else if (location == 1)
                bottomLabels[bottomCount++] = label;
            };

        addLabel(Features::Players::PlayerInfo::DrawId::SelectedLocation, XorString("DrawId"));
        addLabel(Features::Players::PlayerInfo::DrawName::SelectedLocation, XorString("DrawName"));
        addLabel(Features::Players::PlayerInfo::DrawWeaponName::SelectedLocation, XorString("DrawWeaponName"));
        addLabel(Features::Players::PlayerInfo::DrawDistance::SelectedLocation, XorString("DrawDistance"));

        if (topCount > 0) {
            float yOffset = 7.0f;
            float yStep = 15.0f;
            for (int i = topCount - 1; i >= 0; --i) {
                if (topLabels[i] == XorString("DrawDistance") && Features::Players::PlayerInfo::DrawDistance::Enabled) {
                    DrawTextElement(drawList, boxPos, boxSize, std::string(XorString("[") + std::to_string(DemoUser.Distance) + XorString("m]")), Features::Players::PlayerInfo::DrawDistance::Enabled, Features::Players::PlayerInfo::DrawDistance::SelectedLocation, yOffset, 0.0f, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, Features::Players::PlayerInfo::DrawDistance::Color);
                    yOffset += yStep;
                    continue;
                }
                if (topLabels[i] == XorString("DrawWeaponName") && Features::Players::PlayerInfo::DrawWeaponName::Enabled) {
                    DrawTextElement(drawList, boxPos, boxSize, DemoUser.WeaponName, Features::Players::PlayerInfo::DrawWeaponName::Enabled, Features::Players::PlayerInfo::DrawWeaponName::SelectedLocation, yOffset, 0.0f, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, Features::Players::PlayerInfo::DrawWeaponName::Color);
                    yOffset += yStep;
                    continue;
                }
                if (topLabels[i] == XorString("DrawName") && Features::Players::PlayerInfo::DrawName::Enabled) {
                    DrawTextElement(drawList, boxPos, boxSize, DemoUser.Name, Features::Players::PlayerInfo::DrawName::Enabled, Features::Players::PlayerInfo::DrawName::SelectedLocation, yOffset, 0.0f, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, Features::Players::PlayerInfo::DrawName::Color);
                    yOffset += yStep;
                    continue;
                }
                if (topLabels[i] == XorString("DrawId") && Features::Players::PlayerInfo::DrawId::Enabled) {
                    DrawTextElement(drawList, boxPos, boxSize, std::to_string(DemoUser.Id), Features::Players::PlayerInfo::DrawId::Enabled, Features::Players::PlayerInfo::DrawId::SelectedLocation, yOffset, 0.0f, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, Features::Players::PlayerInfo::DrawId::Color);
                    yOffset += yStep;
                    continue;
                }
            }
        }

        if (bottomCount > 0) {
            float yOffset = 7.0f;
            float yStep = 15.0f;
            for (int i = 0; i < bottomCount; ++i) {
                if (bottomLabels[i] == XorString("DrawDistance") && Features::Players::PlayerInfo::DrawDistance::Enabled) {
                    DrawTextElement(drawList, boxPos, boxSize, std::string(XorString("[") + std::to_string(DemoUser.Distance) + XorString("m]")), Features::Players::PlayerInfo::DrawDistance::Enabled, Features::Players::PlayerInfo::DrawDistance::SelectedLocation, 0, yOffset, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, Features::Players::PlayerInfo::DrawDistance::Color);
                    yOffset += yStep;
                    continue;
                }
                if (bottomLabels[i] == XorString("DrawWeaponName") && Features::Players::PlayerInfo::DrawWeaponName::Enabled) {
                    DrawTextElement(drawList, boxPos, boxSize, DemoUser.WeaponName, Features::Players::PlayerInfo::DrawWeaponName::Enabled, Features::Players::PlayerInfo::DrawWeaponName::SelectedLocation, 0, yOffset, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, Features::Players::PlayerInfo::DrawWeaponName::Color);
                    yOffset += yStep;
                    continue;
                }
                if (bottomLabels[i] == XorString("DrawName") && Features::Players::PlayerInfo::DrawName::Enabled) {
                    DrawTextElement(drawList, boxPos, boxSize, DemoUser.Name, Features::Players::PlayerInfo::DrawName::Enabled, Features::Players::PlayerInfo::DrawName::SelectedLocation, 0, yOffset, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, Features::Players::PlayerInfo::DrawName::Color);
                    yOffset += yStep;
                    continue;
                }
                if (bottomLabels[i] == XorString("DrawId") && Features::Players::PlayerInfo::DrawId::Enabled) {
                    DrawTextElement(drawList, boxPos, boxSize, std::to_string(DemoUser.Id), Features::Players::PlayerInfo::DrawId::Enabled, Features::Players::PlayerInfo::DrawId::SelectedLocation, 0, yOffset, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, Features::Players::PlayerInfo::DrawId::Color);
                    yOffset += yStep;
                    continue;
                }
            }
        }

        if (Features::Players::StatusBars::DrawHealthBar::Enabled) {
            float newWidth = boxSize.x * 1.5f;
            float halfWidth = newWidth * 0.5f;
            ImVec2 topLeft, bottomRight;
            ImVec2 barTopLeft, barBottomRight;
            float barHeight = boxSize.y;
            float healthRatio = DemoUser.Health / static_cast<float>(Features::Players::StatusBars::GlobalSettings::MaxHealth);

            if (Features::Players::StatusBars::DrawHealthBar::SelectedLocation == 0) {
                topLeft = ImVec2(boxPos.x - halfWidth, boxPos.y);
                bottomRight = ImVec2(boxPos.x + halfWidth, boxPos.y + boxSize.y);
                barTopLeft = ImVec2(bottomRight.x - 175.0f, topLeft.y);
                barBottomRight = ImVec2(barTopLeft.x + 8.0f, barTopLeft.y + barHeight);
            }
            else if (Features::Players::StatusBars::DrawHealthBar::SelectedLocation == 1) {
                topLeft = ImVec2(boxPos.x - halfWidth, boxPos.y);
                bottomRight = ImVec2(boxPos.x + halfWidth, boxPos.y + boxSize.y);
                barTopLeft = ImVec2(bottomRight.x + 60.0f, topLeft.y);
                barBottomRight = ImVec2(barTopLeft.x + 8.0f, barTopLeft.y + barHeight);
            }

            drawList->AddRectFilled(barTopLeft, barBottomRight, IM_COL32(50, 50, 50, 150));
            int red = 255, green = 0;
            if (healthRatio >= 0.8f) {
                red = (int)(255 * (1.0f - (healthRatio - 0.8f) * 5));
                green = 255;
            }
            else if (healthRatio >= 0.6f) {
                red = 128;
                green = 255;
            }
            else if (healthRatio >= 0.4f) {
                green = 192;
            }
            else if (healthRatio >= 0.2f) {
                green = 100;
            }

            ImU32 barFillColor = IM_COL32(red, green, 0, 255);
            float filledHeight = barHeight * healthRatio;
            ImVec2 fillTopLeft(barTopLeft.x, barBottomRight.y - filledHeight);
            ImVec2 fillBottomRight(barBottomRight);
            drawList->AddRectFilled(fillTopLeft, fillBottomRight, barFillColor);
            drawList->AddRect(barTopLeft, barBottomRight, IM_COL32(0, 0, 0, 255), 0.0f, 0, 2.0f);
        }

        if (Features::Players::StatusBars::DrawArmorBar::Enabled) {
            float newWidth = boxSize.x * 1.5f;
            float halfWidth = newWidth * 0.5f;
            ImVec2 topLeft, bottomRight;
            ImVec2 barTopLeft, barBottomRight;
            float barHeight = boxSize.y;
            float armorRatio = DemoUser.Armor / static_cast<float>(Features::Players::StatusBars::GlobalSettings::MaxArmor);

            if (Features::Players::StatusBars::DrawArmorBar::SelectedLocation == 0) {
                topLeft = ImVec2(boxPos.x - halfWidth, boxPos.y);
                bottomRight = ImVec2(boxPos.x + halfWidth, boxPos.y + boxSize.y);
                barTopLeft = ImVec2(bottomRight.x - 175.0f, topLeft.y);
                barBottomRight = ImVec2(barTopLeft.x + 8.0f, barTopLeft.y + barHeight);
            }
            else if (Features::Players::StatusBars::DrawArmorBar::SelectedLocation == 1) {
                topLeft = ImVec2(boxPos.x - halfWidth, boxPos.y);
                bottomRight = ImVec2(boxPos.x + halfWidth, boxPos.y + boxSize.y);
                barTopLeft = ImVec2(bottomRight.x + 60.0f, topLeft.y);
                barBottomRight = ImVec2(barTopLeft.x + 8.0f, barTopLeft.y + barHeight);
            }

            drawList->AddRectFilled(barTopLeft, barBottomRight, IM_COL32(50, 50, 50, 150));
            int blue = 0, green = 0, red = 0;
            if (armorRatio >= 0.8f) {
                blue = 255;
                green = (int)(255 * (armorRatio - 0.8f) * 5);
            }
            else if (armorRatio >= 0.6f) {
                blue = 255;
                green = 128;
            }
            else if (armorRatio >= 0.4f) {
                blue = 192;
                green = 64;
            }
            else if (armorRatio >= 0.2f) {
                blue = 128;
                green = 32;
            }
            else {
                blue = 64;
                green = 0;
            }

            ImU32 barFillColor = IM_COL32(red, green, blue, 255);
            float filledHeight = barHeight * armorRatio;
            ImVec2 fillTopLeft(barTopLeft.x, barBottomRight.y - filledHeight);
            ImVec2 fillBottomRight(barBottomRight);
            drawList->AddRectFilled(fillTopLeft, fillBottomRight, barFillColor);
            drawList->AddRect(barTopLeft, barBottomRight, IM_COL32(0, 0, 0, 255), 0.0f, 0, 2.0f);
        }
    }
} EspViewer;
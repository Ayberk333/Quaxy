void SetupMenuFonts(ID3D11Device* pDevice) {
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    Roboto = io.Fonts->AddFontFromMemoryTTF(&RobotoCondensedRegularData, sizeof RobotoCondensedRegularData, 19.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Roboto2 = io.Fonts->AddFontFromMemoryTTF(&RobotoCondensedRegularData, sizeof RobotoCondensedRegularData, 17.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    PermanentMarker = io.Fonts->AddFontFromMemoryTTF(&PermanentMarkerData, sizeof PermanentMarkerData, 30.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    RobotoBold = io.Fonts->AddFontFromMemoryTTF(&RobotoCondensedBoldData, sizeof RobotoCondensedBoldData, 15.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    RobotoEsp = io.Fonts->AddFontFromMemoryTTF(&RobotoCondensedRegularData, sizeof RobotoCondensedRegularData, 16.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    RobotoEsp2 = io.Fonts->AddFontFromMemoryTTF(&RobotoCondensedRegularData, sizeof RobotoCondensedRegularData, 12.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    PriceDownEsp = io.Fonts->AddFontFromMemoryTTF(&PriceDownData, sizeof PriceDownData, 16.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    CreateTextures(pDevice);
    ImGui::SetNextWindowPos(ImVec2(100, 100));
}

void MenuHeader() {
    ImGuiStyle* style = &ImGui::GetStyle();
    const auto& pos = ImGui::GetWindowPos();
    const ImVec2& region = ImGui::GetContentRegionMax();
    CustomStyleColor();

    ImGui::GetForegroundDrawList()->AddShadowRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + region.x, pos.y + 47), ImGui::GetColorU32(MenuSetting::Shadow), 16.f, ImVec2(0, 0), ImDrawFlags_RoundCornersTop);
    ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + region.x, pos.y + 47), ImGui::GetColorU32(MenuSetting::DecorativeRectBackground), 18.f, ImDrawFlags_RoundCornersTop);
    ImGui::GetForegroundDrawList()->AddShadowRect(ImVec2(pos.x, pos.y + region.y - 75), ImVec2(pos.x + region.x, pos.y + region.y), ImGui::GetColorU32(MenuSetting::Shadow), 18.f, ImVec2(0, 0), ImDrawFlags_RoundCornersBottom);
    ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(pos.x, pos.y + region.y - 75), ImVec2(pos.x + region.x, pos.y + region.y), ImGui::GetColorU32(MenuSetting::Color4), 16.f, ImDrawFlags_RoundCornersBottom);

    tabAlpha = ImLerp(tabAlpha, (currentPage == activeTab) ? 1.f : 0.f, 15.f * ImGui::GetIO().DeltaTime);
    if (tabAlpha < 0.01f && tabAdd < 0.01f) activeTab = currentPage;

    pageAlpha = ImLerp(pageAlpha, (totalPages == pageTab) ? 1.f : 0.f, 15.f * ImGui::GetIO().DeltaTime);
    if (pageAlpha < 0.01f && pageAdd < 0.01f) pageTab = totalPages;

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, pageAlpha * style->Alpha);
}

void Tab1/*Aim Assist*/() {
    ImGui::SetCursorPos(ImVec2(380, 63));
    ImGui::BeginGroup();
    {
        if (ImGui::Switch_Tab(XorString("Silent"), 0 == pageSwitchTab, ImVec2(141, 27))) pageSwitchTab = 0;
        ImGui::SameLine(0, 10);
        if (ImGui::Switch_Tab(XorString("Aimbot"), 1 == pageSwitchTab, ImVec2(141, 27))) pageSwitchTab = 1;
        ImGui::SameLine(0, 10);
        if (ImGui::Switch_Tab(XorString("TriggerBot"), 2 == pageSwitchTab, ImVec2(141, 27))) pageSwitchTab = 2;
    }
    ImGui::EndGroup();
    ImGui::SetCursorPos(ImVec2(567, 15));
    ImGui::Name_Category(XorString("Aim Assist"), Images::AimIcon, 0, ImVec2(48, 18));

    if (pageSwitchTab == 0) {
        ImGui::SetCursorPos(ImVec2(15, 109));
        ImGui::BeginChild(XorString("Aim Assist Settings"), Images::SettingsIcon, ImVec2(276, 500), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {

                    ImGui::Checkbox(XorString("Crosshair"), &Features::AimAssist::Settings::Crosshair);
                    if (Features::AimAssist::Settings::Crosshair) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::Combo(XorString("Crosshair Type"), &Features::AimAssist::Settings::CrosshairSelectedType, Features::AimAssist::Settings::CrosshairType, IM_ARRAYSIZE(Features::AimAssist::Settings::CrosshairType), 2);
                            ImGui::Separator();
                            ImGui::ColorEdit4(XorString("Crosshair Color"), (float*)&Features::AimAssist::Settings::CrosshairColor.Value, ImGuiColorEditFlags_NoInputs);
                            ImGui::Separator();
                            ImGui::SliderInt(XorString("Crosshair Size"), &Features::AimAssist::Settings::CrosshairSize, 10, 100, XorString("%d%"));
                            ImGui::Separator();
                            ImGui::Checkbox(XorString("Dynamic Crosshair Color"), &Features::AimAssist::Settings::DynamicCrosshairColor);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Ped"), &Features::AimAssist::Settings::IgnorePed);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Death"), &Features::AimAssist::Settings::IgnoreDeath);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Invisible"), &Features::AimAssist::Settings::IgnoreInvisible);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Infinite Ammo"), &Features::AimAssist::Settings::InfiniteAmmo);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("No Recoil"), &Features::AimAssist::Settings::NoRecoil);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("No Spread"), &Features::AimAssist::Settings::NoSpread);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("No Reload"), &Features::AimAssist::Settings::NoReload);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("No Range"), &Features::AimAssist::Settings::NoRange);
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(306, 154));
        ImGui::BeginChild(XorString("Features"), Images::FeaturesIcon, ImVec2(276, 455), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    ImGui::Checkbox(XorString("Silent"), &Features::AimAssist::Silent::Enabled);
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(597, 154));
        ImGui::BeginChild(XorString("Additional Options"), Images::AdditionalIcon, ImVec2(276, 455), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    ImGui::Combo(XorString("Target Area"), &Features::AimAssist::Silent::SelectedArea, Features::AimAssist::Silent::TargetArea, IM_ARRAYSIZE(Features::AimAssist::Silent::TargetArea), 2, true);
                    ImGui::Separator();
                    ImGui::Keybind(XorString("Hot Key"), &Features::AimAssist::Silent::HotKey);
                    ImGui::Separator();
                    ImGui::SliderInt(XorString("Fov"), &Features::AimAssist::Silent::Fov, 1, 100, XorString("%d%"));
                    ImGui::Separator();
                    ImGui::SliderInt(XorString("Max Distance"), &Features::AimAssist::Silent::MaxDistance, 1, 1000, XorString("%d%m"));
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Draw Fov"), &Features::AimAssist::Silent::DrawFov);
                    if (Features::AimAssist::Silent::DrawFov) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::ColorEdit4(XorString("Fov Color"), (float*)&Features::AimAssist::Silent::FovColor.Value, ImGuiColorEditFlags_NoInputs);
                            ImGui::Separator();
                            ImGui::SliderInt(XorString("Fov Weight"), &Features::AimAssist::Silent::FovWeight, 1, 10, XorString("%d%"));
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Draw Target"), &Features::AimAssist::Silent::DrawTarget);
                    if (Features::AimAssist::Silent::DrawTarget) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::Combo(XorString("Draw Target Type"), &Features::AimAssist::Silent::SelectedDrawTargetType, Features::AimAssist::Silent::DrawTargetTypes, IM_ARRAYSIZE(Features::AimAssist::Silent::DrawTargetTypes), 2);
                            ImGui::Separator();
                            ImGui::ColorEdit4(XorString("Draw Target Color"), (float*)&Features::AimAssist::Silent::DrawTargetColor.Value, ImGuiColorEditFlags_NoInputs);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(889, 108));
        ImGui::BeginChild(XorString("Rendering"), Images::PlayersIcon, ImVec2(276, 500), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    AimViewer.Draw();
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
    }

    if (pageSwitchTab == 1) {
        ImGui::SetCursorPos(ImVec2(15, 109));
        ImGui::BeginChild(XorString("Aim Assist Settings"), Images::SettingsIcon, ImVec2(276, 500), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {

                    ImGui::Checkbox(XorString("Crosshair"), &Features::AimAssist::Settings::Crosshair);
                    if (Features::AimAssist::Settings::Crosshair) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::Combo(XorString("Crosshair Type"), &Features::AimAssist::Settings::CrosshairSelectedType, Features::AimAssist::Settings::CrosshairType, IM_ARRAYSIZE(Features::AimAssist::Settings::CrosshairType), 2);
                            ImGui::Separator();
                            ImGui::ColorEdit4(XorString("Crosshair Color"), (float*)&Features::AimAssist::Settings::CrosshairColor.Value, ImGuiColorEditFlags_NoInputs);
                            ImGui::Separator();
                            ImGui::SliderInt(XorString("Crosshair Size"), &Features::AimAssist::Settings::CrosshairSize, 10, 100, XorString("%d%"));
                            ImGui::Separator();
                            ImGui::Checkbox(XorString("Dynamic Crosshair Color"), &Features::AimAssist::Settings::DynamicCrosshairColor);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Ped"), &Features::AimAssist::Settings::IgnorePed);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Death"), &Features::AimAssist::Settings::IgnoreDeath);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Invisible"), &Features::AimAssist::Settings::IgnoreInvisible);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Infinite Ammo"), &Features::AimAssist::Settings::InfiniteAmmo);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("No Recoil"), &Features::AimAssist::Settings::NoRecoil);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("No Spread"), &Features::AimAssist::Settings::NoSpread);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("No Reload"), &Features::AimAssist::Settings::NoReload);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("No Range"), &Features::AimAssist::Settings::NoRange);
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(306, 154));
        ImGui::BeginChild(XorString("Features"), Images::FeaturesIcon, ImVec2(276, 455), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    ImGui::Checkbox(XorString("Aimbot"), &Features::AimAssist::Aimbot::Enabled);
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(597, 154));
        ImGui::BeginChild(XorString("Additional Options"), Images::AdditionalIcon, ImVec2(276, 455), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    ImGui::Combo(XorString("Target Area"), &Features::AimAssist::Aimbot::SelectedArea, Features::AimAssist::Aimbot::TargetArea, IM_ARRAYSIZE(Features::AimAssist::Aimbot::TargetArea), 2, true);
                    ImGui::Separator();
                    ImGui::Keybind(XorString("Hot Key"), &Features::AimAssist::Aimbot::HotKey);
                    ImGui::Separator();
                    ImGui::SliderInt(XorString("Smooth"), &Features::AimAssist::Aimbot::Smooth, 1, 50, XorString("%d%"));
                    ImGui::Separator();
                    ImGui::SliderInt(XorString("Fov"), &Features::AimAssist::Aimbot::Fov, 1, 100, XorString("%d%"));
                    ImGui::Separator();
                    ImGui::SliderInt(XorString("Max Distance"), &Features::AimAssist::Aimbot::MaxDistance, 1, 1000, XorString("%d%m"));
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Draw Fov"), &Features::AimAssist::Aimbot::DrawFov);
                    if (Features::AimAssist::Aimbot::DrawFov) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::ColorEdit4(XorString("Fov Color"), (float*)&Features::AimAssist::Aimbot::FovColor.Value, ImGuiColorEditFlags_NoInputs);
                            ImGui::Separator();
                            ImGui::SliderInt(XorString("Fov Weight"), &Features::AimAssist::Aimbot::FovWeight, 1, 10, XorString("%d%"));
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Draw Target"), &Features::AimAssist::Aimbot::DrawTarget);
                    if (Features::AimAssist::Aimbot::DrawTarget) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::Combo(XorString("Draw Target Type"), &Features::AimAssist::Aimbot::SelectedDrawTargetType, Features::AimAssist::Aimbot::DrawTargetTypes, IM_ARRAYSIZE(Features::AimAssist::Aimbot::DrawTargetTypes), 2);
                            ImGui::Separator();
                            ImGui::ColorEdit4(XorString("Draw Target Color"), (float*)&Features::AimAssist::Aimbot::DrawTargetColor.Value, ImGuiColorEditFlags_NoInputs);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(889, 108));
        ImGui::BeginChild(XorString("Rendering"), Images::PlayersIcon, ImVec2(276, 500), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    AimViewer.Draw();
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
    }

    if (pageSwitchTab == 2) {
        ImGui::SetCursorPos(ImVec2(15, 109));
        ImGui::BeginChild(XorString("Aim Assist Settings"), Images::SettingsIcon, ImVec2(276, 500), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {

                    ImGui::Checkbox(XorString("Crosshair"), &Features::AimAssist::Settings::Crosshair);
                    if (Features::AimAssist::Settings::Crosshair) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::Combo(XorString("Crosshair Type"), &Features::AimAssist::Settings::CrosshairSelectedType, Features::AimAssist::Settings::CrosshairType, IM_ARRAYSIZE(Features::AimAssist::Settings::CrosshairType), 2);
                            ImGui::Separator();
                            ImGui::ColorEdit4(XorString("Crosshair Color"), (float*)&Features::AimAssist::Settings::CrosshairColor.Value, ImGuiColorEditFlags_NoInputs);
                            ImGui::Separator();
                            ImGui::SliderInt(XorString("Crosshair Size"), &Features::AimAssist::Settings::CrosshairSize, 10, 100, XorString("%d%"));
                            ImGui::Separator();
                            ImGui::Checkbox(XorString("Dynamic Crosshair Color"), &Features::AimAssist::Settings::DynamicCrosshairColor);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Ped"), &Features::AimAssist::Settings::IgnorePed);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Death"), &Features::AimAssist::Settings::IgnoreDeath);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Invisible"), &Features::AimAssist::Settings::IgnoreInvisible);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Infinite Ammo"), &Features::AimAssist::Settings::InfiniteAmmo);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("No Recoil"), &Features::AimAssist::Settings::NoRecoil);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("No Spread"), &Features::AimAssist::Settings::NoSpread);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("No Reload"), &Features::AimAssist::Settings::NoReload);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("No Range"), &Features::AimAssist::Settings::NoRange);
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(306, 154));
        ImGui::BeginChild(XorString("Features"), Images::FeaturesIcon, ImVec2(276, 455), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    ImGui::Checkbox(XorString("Triggerbot"), &Features::AimAssist::Triggerbot::Enabled);
                    ImGui::Separator();
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(597, 154));
        ImGui::BeginChild(XorString("Additional Options"), Images::AdditionalIcon, ImVec2(276, 455), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    ImGui::SliderInt(XorString("Delay"), &Features::AimAssist::Triggerbot::Delay, 1, 50, XorString("%d%"));
                    ImGui::Separator();
                    ImGui::SliderInt(XorString("Max Distance"), &Features::AimAssist::Triggerbot::MaxDistance, 1, 1000, "%d%m");
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(889, 108));
        ImGui::BeginChild(XorString("Rendering"), Images::PlayersIcon, ImVec2(276, 500), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    AimViewer.Draw();
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
    }
}

void Tab2/*Players*/() {
    ImGui::SetCursorPos(ImVec2(380, 63));
    ImGui::BeginGroup();
    {
        if (ImGui::Switch_Tab(XorString("Visual Markers"), 0 == pageSwitchTab2, ImVec2(141, 27))) pageSwitchTab2 = 0;
        ImGui::SameLine(0, 10);
        if (ImGui::Switch_Tab(XorString("Player Info"), 1 == pageSwitchTab2, ImVec2(141, 27))) pageSwitchTab2 = 1;
        ImGui::SameLine(0, 10);
        if (ImGui::Switch_Tab(XorString("Status Bars"), 2 == pageSwitchTab2, ImVec2(141, 27))) pageSwitchTab2 = 2;

    }
    ImGui::EndGroup();
    ImGui::SetCursorPos(ImVec2(567, 15));
    ImGui::Name_Category(XorString("Players"), Images::AimIcon, 0, ImVec2(48, 18));

    if (pageSwitchTab2 == 0) {
        ImGui::SetCursorPos(ImVec2(15, 109));
        ImGui::BeginChild(XorString("Players Settings"), Images::SettingsIcon, ImVec2(276, 500), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    ImGui::Checkbox(XorString("Ignore Ped"), &Features::Players::Settings::IgnorePed);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Death"), &Features::Players::Settings::IgnoreDeath);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Invisible"), &Features::Players::Settings::IgnoreInvisible);
                    ImGui::Separator();
                    ImGui::SliderInt(XorString("Max Distance"), &Features::Players::Settings::MaxDistance, 1, 1000, XorString("%d%m"));
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(306, 154));
        ImGui::BeginChild(XorString("Features"), Images::FeaturesIcon, ImVec2(276, 455), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    ImGui::Checkbox(XorString("Draw Skeleton"), &Features::Players::VisualMarkers::DrawSkeleton::Enabled);
                    if (Features::Players::VisualMarkers::DrawSkeleton::Enabled) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::ColorEdit4(XorString("Skeleton Color"), (float*)&Features::Players::VisualMarkers::DrawSkeleton::Color.Value, ImGuiColorEditFlags_NoInputs);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Draw Box"), &Features::Players::VisualMarkers::DrawBox::Enabled);
                    if (Features::Players::VisualMarkers::DrawBox::Enabled) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::ColorEdit4(XorString("Box Color"), (float*)&Features::Players::VisualMarkers::DrawBox::Color.Value, ImGuiColorEditFlags_NoInputs);
                            ImGui::Separator();
                            ImGui::Combo(XorString("Box Type"), &Features::Players::VisualMarkers::DrawBox::SelectedType, Features::Players::VisualMarkers::DrawBox::Types, IM_ARRAYSIZE(Features::Players::VisualMarkers::DrawBox::Types), 2);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Draw Line"), &Features::Players::VisualMarkers::DrawLine::Enabled);
                    if (Features::Players::VisualMarkers::DrawLine::Enabled) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::ColorEdit4(XorString("Line Color"), (float*)&Features::Players::VisualMarkers::DrawLine::Color.Value, ImGuiColorEditFlags_NoInputs);
                            ImGui::Separator();
                            ImGui::Combo(XorString("Line Location"), &Features::Players::VisualMarkers::DrawLine::SelectedLocation, Features::Players::VisualMarkers::DrawLine::Locations, IM_ARRAYSIZE(Features::Players::VisualMarkers::DrawLine::Locations), 2);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(597, 154));
        ImGui::BeginChild(XorString("Additional Options"), Images::AdditionalIcon, ImVec2(276, 455), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    ImGui::Combo(XorString("Line Type"), &Features::Players::VisualMarkers::GlobalSettings::SelectedLineType, Features::Players::VisualMarkers::GlobalSettings::LineTypes, IM_ARRAYSIZE(Features::Players::VisualMarkers::GlobalSettings::LineTypes), 2);
                    ImGui::Separator();
                    ImGui::SliderInt(XorString("Line Weight"), &Features::Players::VisualMarkers::GlobalSettings::LineWeight, 1, 10, XorString("%d%"));
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(889, 108));
        ImGui::BeginChild(XorString("Rendering"), Images::PlayersIcon, ImVec2(276, 500), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    EspViewer.Draw();
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
    }

    if (pageSwitchTab2 == 1) {
        ImGui::SetCursorPos(ImVec2(15, 109));
        ImGui::BeginChild(XorString("Players Settings"), Images::SettingsIcon, ImVec2(276, 500), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    ImGui::Checkbox(XorString("Ignore Ped"), &Features::Players::Settings::IgnorePed);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Death"), &Features::Players::Settings::IgnoreDeath);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Invisible"), &Features::Players::Settings::IgnoreInvisible);
                    ImGui::Separator();
                    ImGui::SliderInt(XorString("Max Distance"), &Features::Players::Settings::MaxDistance, 1, 1000, XorString("%d%m"));
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(306, 154));
        ImGui::BeginChild(XorString("Features"), Images::FeaturesIcon, ImVec2(276, 455), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Draw Id"), &Features::Players::PlayerInfo::DrawId::Enabled);
                    if (Features::Players::PlayerInfo::DrawId::Enabled) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::ColorEdit4(XorString("Id Color"), (float*)&Features::Players::PlayerInfo::DrawId::Color.Value, ImGuiColorEditFlags_NoInputs);
                            ImGui::Separator();
                            ImGui::Combo(XorString("Id Location"), &Features::Players::PlayerInfo::DrawId::SelectedLocation, Features::Players::PlayerInfo::DrawId::Locations, IM_ARRAYSIZE(Features::Players::PlayerInfo::DrawId::Locations), 2);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Draw Name"), &Features::Players::PlayerInfo::DrawName::Enabled);
                    if (Features::Players::PlayerInfo::DrawName::Enabled) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::ColorEdit4(XorString("Name Color"), (float*)&Features::Players::PlayerInfo::DrawName::Color.Value, ImGuiColorEditFlags_NoInputs);
                            ImGui::Separator();
                            ImGui::Combo(XorString("Name Location"), &Features::Players::PlayerInfo::DrawName::SelectedLocation, Features::Players::PlayerInfo::DrawName::Locations, IM_ARRAYSIZE(Features::Players::PlayerInfo::DrawName::Locations), 2);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Draw Weapon Name"), &Features::Players::PlayerInfo::DrawWeaponName::Enabled);
                    if (Features::Players::PlayerInfo::DrawWeaponName::Enabled) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::ColorEdit4(XorString("Weapon Color"), (float*)&Features::Players::PlayerInfo::DrawWeaponName::Color.Value, ImGuiColorEditFlags_NoInputs);
                            ImGui::Separator();
                            ImGui::Combo(XorString("Weapon Name Location"), &Features::Players::PlayerInfo::DrawWeaponName::SelectedLocation, Features::Players::PlayerInfo::DrawWeaponName::Locations, IM_ARRAYSIZE(Features::Players::PlayerInfo::DrawWeaponName::Locations), 2);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Draw Distance"), &Features::Players::PlayerInfo::DrawDistance::Enabled);
                    if (Features::Players::PlayerInfo::DrawDistance::Enabled) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::ColorEdit4(XorString("Distance Color"), (float*)&Features::Players::PlayerInfo::DrawDistance::Color.Value, ImGuiColorEditFlags_NoInputs);
                            ImGui::Separator();
                            ImGui::Combo(XorString("Distance Location"), &Features::Players::PlayerInfo::DrawDistance::SelectedLocation, Features::Players::PlayerInfo::DrawDistance::Locations, IM_ARRAYSIZE(Features::Players::PlayerInfo::DrawDistance::Locations), 2);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(597, 154));
        ImGui::BeginChild(XorString("Additional Options"), Images::AdditionalIcon, ImVec2(276, 455), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    ImGui::Combo(XorString("Font"), &Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::Fonts, IM_ARRAYSIZE(Features::Players::PlayerInfo::GlobalSettings::Fonts), 2);
                    ImGui::Separator();
                    ImGui::Combo(XorString("Font Type"), &Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, Features::Players::PlayerInfo::GlobalSettings::FontTypes, IM_ARRAYSIZE(Features::Players::PlayerInfo::GlobalSettings::FontTypes), 2);
                    ImGui::Separator();
                    ImGui::SliderInt(XorString("Max Distance"), &Features::Players::PlayerInfo::GlobalSettings::MaxDistance, 1, 1000, XorString("%d%m"));
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(889, 108));
        ImGui::BeginChild(XorString("Rendering"), Images::PlayersIcon, ImVec2(276, 500), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    EspViewer.Draw();
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
    }

    if (pageSwitchTab2 == 2) {
        ImGui::SetCursorPos(ImVec2(15, 109));
        ImGui::BeginChild(XorString("Players Settings"), Images::SettingsIcon, ImVec2(276, 500), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    ImGui::Checkbox(XorString("Ignore Ped"), &Features::Players::Settings::IgnorePed);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Death"), &Features::Players::Settings::IgnoreDeath);
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Ignore Invisible"), &Features::Players::Settings::IgnoreInvisible);
                    ImGui::Separator();
                    ImGui::SliderInt(XorString("Max Distance"), &Features::Players::Settings::MaxDistance, 1, 1000, XorString("%d%m"));
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(306, 154));
        ImGui::BeginChild(XorString("Features"), Images::FeaturesIcon, ImVec2(276, 455), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    ImGui::Checkbox(XorString("Draw Health Bar"), &Features::Players::StatusBars::DrawHealthBar::Enabled);
                    if (Features::Players::StatusBars::DrawHealthBar::Enabled) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            if (ImGui::Combo(XorString("Health Bar Location"), &Features::Players::StatusBars::DrawHealthBar::SelectedLocation, Features::Players::StatusBars::DrawHealthBar::Locations, IM_ARRAYSIZE(Features::Players::StatusBars::DrawHealthBar::Locations), 2)) {
                                if (Features::Players::StatusBars::DrawHealthBar::SelectedLocation == 1 && Features::Players::StatusBars::DrawArmorBar::SelectedLocation == 1) {
                                    Features::Players::StatusBars::DrawArmorBar::SelectedLocation = 0;
                                }
                            }
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Draw Armor Bar"), &Features::Players::StatusBars::DrawArmorBar::Enabled);
                    if (Features::Players::StatusBars::DrawArmorBar::Enabled) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            if (ImGui::Combo(XorString("Armor Bar Location"), &Features::Players::StatusBars::DrawArmorBar::SelectedLocation, Features::Players::StatusBars::DrawArmorBar::Locations, IM_ARRAYSIZE(Features::Players::StatusBars::DrawArmorBar::Locations), 2)) {
                                if (Features::Players::StatusBars::DrawArmorBar::SelectedLocation == 1 && Features::Players::StatusBars::DrawHealthBar::SelectedLocation == 1) {
                                    Features::Players::StatusBars::DrawHealthBar::SelectedLocation = 0;
                                }
                            }
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(597, 154));
        ImGui::BeginChild(XorString("Additional Options"), Images::AdditionalIcon, ImVec2(276, 455), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    //ImGui::SliderInt(XorString("Max Health"), &Features::Players::StatusBars::GlobalSettings::MaxHealth, 1, 500, XorString("%d%m"));
                    //ImGui::Separator();
                    //ImGui::SliderInt(XorString("Max Armor"), &Features::Players::StatusBars::GlobalSettings::MaxArmor, 1, 500, XorString("%d%m"));
                    ImGui::Checkbox(XorString("Health Boost"), &Features::Players::StatusBars::HealthBoost::Enabled);
                    if (Features::Players::StatusBars::HealthBoost::Enabled) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::SliderInt(XorString("Health Boost Value"), &Features::Players::StatusBars::HealthBoost::Value, 1, 200, XorString("%d%"));
                            ImGui::Separator();
                            ImGui::Keybind(XorString("Health Boost Hot Key"), &Features::Players::StatusBars::HealthBoost::HotKey);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                    ImGui::Separator();
                    ImGui::Checkbox(XorString("Armor Boost"), &Features::Players::StatusBars::ArmorBoost::Enabled);
                    if (Features::Players::StatusBars::ArmorBoost::Enabled) {
                        ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                        ImGui::BeginGroup();
                        {
                            ImGui::SliderInt(XorString("Armor Boost Value"), &Features::Players::StatusBars::ArmorBoost::Value, 1, 100, XorString("%d%"));
                            ImGui::Separator();
                            ImGui::Keybind(XorString("Armor Boost Hot Key"), &Features::Players::StatusBars::ArmorBoost::HotKey);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                    }
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(889, 108));
        ImGui::BeginChild(XorString("Rendering"), Images::PlayersIcon, ImVec2(276, 500), false);
        {
            ImGui::SetCursorPos(ImVec2(15, 0));
            ImGui::BeginGroup();
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
                {
                    EspViewer.Draw();
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
    }
}

void Tab3/*Vehicles*/() {
    ImGui::SetCursorPos(ImVec2(567, 15));
    ImGui::Name_Category(XorString("Vehicles"), Images::VehiclesIcon, 0, ImVec2(48, 18));
    ImGui::SetCursorPos(ImVec2(15, 109));
    ImGui::BeginChild(XorString("Vehicle Settings"), Images::SettingsIcon, ImVec2(276, 500), false);
    {
        ImGui::SetCursorPos(ImVec2(15, 0));
        ImGui::BeginGroup();
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
            {
                ImGui::Checkbox(XorString("Ignore Local Vehicle"), &Features::Vehicles::Settings::IgnoreLocalVehicle);
                ImGui::Separator();
                ImGui::SliderInt(XorString("Max Vehicle Count"), &Features::Vehicles::Settings::MaxVehicleCount, 1, 5000, XorString("%d%"));
                ImGui::Separator();
                ImGui::SliderInt(XorString("Max Distance"), &Features::Vehicles::Settings::MaxDistance, 1, 1000, XorString("%d%m"));
            }
            ImGui::PopStyleVar();
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SetCursorPos(ImVec2(300, 109));
    ImGui::BeginChild(XorString("Features"), Images::FeaturesIcon, ImVec2(865, 500), false);
    {
        ImGui::SetCursorPos(ImVec2(15, 0));
        ImGui::BeginGroup();
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
            {
                ImGui::Checkbox(XorString("Draw Point"), &Features::Vehicles::DrawPoint::Enabled);
                if (Features::Vehicles::DrawPoint::Enabled) {
                    ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                    ImGui::BeginGroup();
                    {
                        ImGui::ColorEdit4(XorString("Point Color"), (float*)&Features::Vehicles::DrawPoint::Color.Value, ImGuiColorEditFlags_NoInputs);
                        ImGui::Separator();
                        ImGui::Combo(XorString("Point Type"), &Features::Vehicles::DrawPoint::SelectedLineType, Features::Vehicles::DrawPoint::LineTypes, IM_ARRAYSIZE(Features::Vehicles::DrawPoint::LineTypes), 2);
                        ImGui::Separator();
                        ImGui::SliderInt(XorString("Point Size"), &Features::Vehicles::DrawPoint::Size, 1, 50, XorString("%d%"));
                    }
                    ImGui::EndGroup();
                    ImGui::PopStyleVar();
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                }
                ImGui::Separator();
                ImGui::Checkbox(XorString("Draw Line"), &Features::Vehicles::DrawLine::Enabled);
                if (Features::Vehicles::DrawLine::Enabled) {
                    ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                    ImGui::BeginGroup();
                    {
                        ImGui::ColorEdit4(XorString("Line Color"), (float*)&Features::Vehicles::DrawLine::Color.Value, ImGuiColorEditFlags_NoInputs);
                        ImGui::Separator();
                        ImGui::Combo(XorString("Line Type"), &Features::Vehicles::DrawLine::SelectedLineType, Features::Vehicles::DrawLine::LineTypes, IM_ARRAYSIZE(Features::Vehicles::DrawLine::LineTypes), 2);
                        ImGui::Separator();
                        ImGui::Combo(XorString("Line Location"), &Features::Vehicles::DrawLine::SelectedLocation, Features::Vehicles::DrawLine::Locations, IM_ARRAYSIZE(Features::Vehicles::DrawLine::Locations), 2);
                    }
                    ImGui::EndGroup();
                    ImGui::PopStyleVar();
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                }
                ImGui::Separator();
                ImGui::Checkbox(XorString("Draw Distance"), &Features::Vehicles::DrawDistance::Enabled);
                if (Features::Vehicles::DrawDistance::Enabled) {
                    ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                    ImGui::BeginGroup();
                    {
                        ImGui::ColorEdit4(XorString("Distance Color"), (float*)&Features::Vehicles::DrawDistance::Color.Value, ImGuiColorEditFlags_NoInputs);
                        ImGui::Separator();
                        ImGui::Combo(XorString("Font"), &Features::Vehicles::DrawDistance::SelectedFont, Features::Vehicles::DrawDistance::Fonts, IM_ARRAYSIZE(Features::Vehicles::DrawDistance::Fonts), 2);
                        ImGui::Separator();
                        ImGui::Combo(XorString("Font Type"), &Features::Vehicles::DrawDistance::SelectedFontType, Features::Vehicles::DrawDistance::FontTypes, IM_ARRAYSIZE(Features::Vehicles::DrawDistance::FontTypes), 2);
                    }
                    ImGui::EndGroup();
                    ImGui::PopStyleVar();
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                }
                ImGui::Separator();
                ImGui::Checkbox(XorString("Draw Health Bar"), &Features::Vehicles::DrawHealthBar::Enabled);
                ImGui::Separator();
                ImGui::Checkbox(XorString("Vehicle Fix"), &Features::Vehicles::VehicleFix::Enabled);
                if (Features::Vehicles::VehicleFix::Enabled) {
                    ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                    ImGui::BeginGroup();
                    {
                        ImGui::Keybind(XorString("Hot Key"), &Features::Vehicles::VehicleFix::HotKey);
                    }
                    ImGui::EndGroup();
                    ImGui::PopStyleVar();
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                }
            }
            ImGui::PopStyleVar();
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();
}

void Tab4/*World*/() {
    ImGui::SetCursorPos(ImVec2(567, 15));
    ImGui::Name_Category(XorString("World"), Images::SettingsIcon, 0, ImVec2(48, 18));
    ImGui::SetCursorPos(ImVec2(15, 109));
    ImGui::BeginChild(XorString("Features"), Images::SettingsIcon, ImVec2(800, 500), false);
    {
        ImGui::SetCursorPos(ImVec2(15, 0));
        ImGui::BeginGroup();
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
            {
                ImGui::Checkbox(XorString("No Clip"), &Features::World::NoClip::Enabled);
                if (Features::World::NoClip::Enabled) {
                    ImGui::SetCursorPos(ImVec2(40, ImGui::GetCursorPos().y + 10));
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 0));
                    ImGui::BeginGroup();
                    {
                        ImGui::SliderInt(XorString("Movement Speed"), &Features::World::NoClip::MovementSpeed, 1, 10, XorString("%d%"));
                        ImGui::Separator();
                        ImGui::Keybind(XorString("Forward Hotkey"), &Features::World::NoClip::ForwardKey);
                        ImGui::Separator();
                        ImGui::Keybind(XorString("Backward Hotkey"), &Features::World::NoClip::BackwardKey);
                        ImGui::Separator();
                        ImGui::Keybind(XorString("Left Hotkey"), &Features::World::NoClip::LeftKey);
                        ImGui::Separator();
                        ImGui::Keybind(XorString("Right Hotkey"), &Features::World::NoClip::RightKey);
                        ImGui::Separator();
                        ImGui::Keybind(XorString("Up Hotkey"), &Features::World::NoClip::UpKey);
                        ImGui::Separator();
                        ImGui::Keybind(XorString("Down Hotkey"), &Features::World::NoClip::DownKey);
                    }
                    ImGui::EndGroup();
                    ImGui::PopStyleVar();
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
                }
                ImGui::Separator();
                ImGui::Checkbox(XorString("Semi God Mode"), &Features::World::SemiGodMode::Enabled);
                ImGui::Separator();
            }
            ImGui::PopStyleVar();
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SetCursorPos(ImVec2(820, 109));
    ImGui::BeginChild(XorString("Teleport"), Images::SettingsIcon, ImVec2(350, 500), false);
    {
        ImGui::SetCursorPos(ImVec2(15, 0));
        ImGui::BeginGroup();
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
            {
                static int selectedRow = -1;
                const int locationCount = sizeof(Features::World::Teleport::teleportData) / sizeof(Features::World::Teleport::teleportData[0]);
                if (ImGui::BeginTable("", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY, ImVec2(0, 440)))
                {
                    ImGui::TableSetupColumn(XorString("Location"));
                    ImGui::TableHeadersRow();
                    for (int i = 0; i < locationCount; i++) {
                        ImGui::TableNextRow();
                        if (selectedRow == i) {
                            for (int col = 0; col < 1; col++) {
                                ImGui::TableSetColumnIndex(col);
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::ColorConvertFloat4ToU32(MenuSetting::Color1));
                            }
                        }
                        ImGui::TableSetColumnIndex(0);
                        char rowLabel[64];
                        sprintf(rowLabel, XorString("%s"), Features::World::Teleport::teleportData[i].Name);
                        if (ImGui::Selectable(rowLabel, selectedRow == i, ImGuiSelectableFlags_SpanAllColumns)) {
                            selectedRow = i;
                        }
                    }
                    ImGui::EndTable();
                }
                ImGui::SetCursorPos(ImVec2(15, 450));
                if (ImGui::Button(XorString("Teleport Location"), ImVec2(160, 35), MenuSetting::Color2, MenuSetting::Color3) && selectedRow >= 0 && selectedRow < locationCount) {
                    auto targetPosition = Features::World::Teleport::teleportData[selectedRow].Position;
                    PositionTeleport(Vector3(targetPosition.x, targetPosition.y, targetPosition.z));
                }
                ImGui::SameLine(165, 0);
                if (ImGui::Button(XorString("Teleport Waypoint"), ImVec2(160, 35), MenuSetting::Color2, MenuSetting::Color3)) {
                    TeleportWaypoint();
                }         
            }
            ImGui::PopStyleVar();
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();
}

void Tab5/*Settings*/() {
    ImGui::SetCursorPos(ImVec2(567, 15));
    ImGui::Name_Category(XorString("Settings"), Images::SettingsIcon, 0, ImVec2(48, 18));
    ImGui::SetCursorPos(ImVec2(15, 109));
    ImGui::BeginChild(XorString("Settings"), Images::SettingsIcon, ImVec2(1150, 500), false);
    {
        ImGui::SetCursorPos(ImVec2(15, 0));
        ImGui::BeginGroup();
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 0));
            {
                ImGui::Checkbox(XorString("Stream Proof"), &Features::Settings::StreamProof);
                ImGui::Separator();
                ImGui::Keybind(XorString("Menu Key"), &Features::Settings::MenuKey);
                ImGui::Separator();
                if (ImGui::Button(XorString("Exit"), ImVec2(1120, 50), MenuSetting::Color2, MenuSetting::Color3)) {
                    keepRunning = false;
                    PostQuitMessage(0);
                }
            }
            ImGui::PopStyleVar();
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();
}

void RenderMenu(HWND hWnd) {
    ImGui::SetNextWindowSize(MenuSetting::Size);
    ImGui::Begin(XorString("QuaxyMenu"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings);
    {
        MenuHeader();
        {
            if (pageTab == 0) {
                ImGui::SetCursorPos(ImVec2(430, ImGui::GetContentRegionMax().y - 65));
                ImGui::BeginGroup();
                {
                    if (ImGui::Tab(XorString("Aim Assist"), Images::AimIcon, 0 == currentPage, ImVec2(44, 65))) currentPage = 0;
                    if (ImGui::Tab(XorString("Players"), Images::PlayersIcon, 1 == currentPage, ImVec2(44, 65))) currentPage = 1;
                    if (ImGui::Tab(XorString("Vehicles"), Images::VehiclesIcon, 2 == currentPage, ImVec2(44, 65))) currentPage = 2;
                    if (ImGui::Tab(XorString("World"), Images::WorldIcon, 3 == currentPage, ImVec2(44, 65))) currentPage = 3;
                    if (ImGui::Tab(XorString("Settings"), Images::SettingsIcon, 4 == currentPage, ImVec2(44, 65))) currentPage = 4;
                }
                ImGui::EndGroup();
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tabAlpha * ImGui::GetStyle().Alpha);
                {
                    switch (activeTab) {
                    case 0:
                        Tab1();
                        break;
                    case 1:
                        Tab2();
                        break;
                    case 2:
                        Tab3();
                        break;
                    case 3:
                        Tab4();
                        break;
                    case 4:
                        Tab5();
                        break;
                    }
                }
                ImGui::PopStyleVar();
            }
            ImGui::PopStyleVar();
        }
    }
    ImGui::End();
}

void RenderFrame(HWND hWnd, bool isMenuVisible) {
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    {
        DrawOverlayFeatures();
        if (isMenuVisible) {
            RenderMenu(hWnd);
        }
    }
    ImGui::Render();
}

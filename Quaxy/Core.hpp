static uintptr_t GameWorld;
static uintptr_t GameViewPort;
static uintptr_t GameReplayInterface;
static Matrix ViewMatrix;

static Ped LocalPlayer;
static Ped* SilentPed = nullptr;
static Ped* AimbotPed = nullptr;
static std::vector<Ped> PedList;
static std::vector<Vehicle> VehicleList;

bool WorldToScreen(const Matrix& viewMatrix, const Vector3& vWorld, Vector2& vOut) {
	Matrix v = viewMatrix.Transpose();
	Vector4 row2 = Vector4(v._21, v._22, v._23, v._24);
	Vector4 row3 = Vector4(v._31, v._32, v._33, v._34);
	Vector4 row4 = Vector4(v._41, v._42, v._43, v._44);

	Vector3 proj;
	proj.x = (row2.x * vWorld.x) + (row2.y * vWorld.y) + (row2.z * vWorld.z) + row2.w;
	proj.y = (row3.x * vWorld.x) + (row3.y * vWorld.y) + (row3.z * vWorld.z) + row3.w;
	proj.z = (row4.x * vWorld.x) + (row4.y * vWorld.y) + (row4.z * vWorld.z) + row4.w;
	if (proj.z <= 0.1f)
		return false;

	float invZ = 1.0f / proj.z;
	proj.x *= invZ;
	proj.y *= invZ;

	float screenW = static_cast<float>(Game.lpRect.right);
	float screenH = static_cast<float>(Game.lpRect.bottom);
	float halfW = screenW * 0.5f;
	float halfH = screenH * 0.5f;
	vOut.x = halfW + (0.5f * proj.x * screenW);
	vOut.y = halfH - (0.5f * proj.y * screenH);
	return true;
}

bool IsTargetInCrosshair(const Vector2& screenPosition) {
	const float crosshairX = (float)Game.lpRect.right / 2;
	const float crosshairY = (float)Game.lpRect.bottom / 2;
	return (abs(screenPosition.x - crosshairX) <= 10 && abs(screenPosition.y - crosshairY) <= 10);
}

void UpdatePeds() {
	while (keepRunning) {
		std::vector<Ped> updatedPedList;
		GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);	
		GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
		GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
		LocalPlayer.Pointer = ReadMemory<uintptr_t>(GameWorld + Offsets.LocalPlayer);

		uintptr_t entityListPtr = ReadMemory<uintptr_t>(GameReplayInterface + 0x18);
		uintptr_t entityList = ReadMemory<uintptr_t>(entityListPtr + 0x100);

		for (int i = 0; i < maxPlayerCount; i++) {
			Ped ped;
			uintptr_t player = ReadMemory<uintptr_t>(entityList + (i * 0x10));
			if (player == LocalPlayer.Pointer) {
				continue;
			}
			else if (!ped.GetPlayer(player)) {
				continue;
			}
			else if (!ped.Update()) {
				continue;
			}
			updatedPedList.push_back(ped);
		}
		PedList = updatedPedList;
		Sleep(10);
	}
}

void UpdateVehicles() {
	while (keepRunning) {
		if (Features::Vehicles::DrawPoint::Enabled || Features::Vehicles::DrawLine::Enabled || Features::Vehicles::DrawDistance::Enabled || Features::Vehicles::DrawHealthBar::Enabled) {
			std::vector<Vehicle> updatedVehicleList;
			uintptr_t vehicleInterface = ReadMemory<DWORD64>(GameReplayInterface + 0x10);
			uintptr_t vehicleList = ReadMemory<DWORD64>(vehicleInterface + 0x180);
			int vehicleListCount = ReadMemory<int>(vehicleInterface + 0x188);
			if (vehicleListCount > Features::Vehicles::Settings::MaxVehicleCount) {
				Sleep(10);
				continue;
			}

			for (int i = 0; i < vehicleListCount; ++i) {
				uintptr_t vehiclePointer = ReadMemory<uintptr_t>(vehicleList + (i * 0x10));
				if (!vehiclePointer) {
					continue;
				}

				float vehicleHealth = ReadMemory<float>(vehiclePointer + Offsets.Health);
				Vector3 getCordLocal = ReadMemory<Vector3>(LocalPlayer.Pointer + 0x90);
				Matrix viewMatrix = ReadMemory<Matrix>(GameViewPort + 0x24C);
				Vector3 getCorrdinateVehicle = ReadMemory<Vector3>(vehiclePointer + 0x90);
				Vector2 vehicleLocation;
				if (!WorldToScreen(viewMatrix, getCorrdinateVehicle, vehicleLocation)) {
					continue;
				}

				Vector3 distanceCalculation = getCordLocal - getCorrdinateVehicle;
				double pDistance = sqrtf(distanceCalculation.x * distanceCalculation.x + distanceCalculation.y * distanceCalculation.y + distanceCalculation.z * distanceCalculation.z);
				if (pDistance > Features::Vehicles::Settings::MaxDistance) {
					continue;
				}

				if ((int)pDistance == 0 && Features::Vehicles::Settings::IgnoreLocalVehicle) {
					continue;
				}

				if (vehicleLocation.x == 0 || vehicleLocation.y == 0) {
					continue;
				}

				Vehicle vehicle;
				vehicle.Pointer = vehiclePointer;
				vehicle.Location = vehicleLocation;
				vehicle.Health = vehicleHealth;
				vehicle.Distance = pDistance;
				updatedVehicleList.push_back(vehicle);
			}
			VehicleList = updatedVehicleList;
		}
		Sleep(10);
	}
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

void DrawOverlayFeatures() {
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	if (Features::AimAssist::Silent::DrawFov) {
		drawList->AddCircle(ImVec2(Game.lpRect.right / 2.f, Game.lpRect.bottom / 2.f), Features::AimAssist::Silent::Fov, Features::AimAssist::Silent::FovColor, 100, Features::AimAssist::Silent::FovWeight);
	}

	if (Features::AimAssist::Aimbot::DrawFov) {
		drawList->AddCircle(ImVec2(Game.lpRect.right / 2.f, Game.lpRect.bottom / 2.f), Features::AimAssist::Aimbot::Fov, Features::AimAssist::Aimbot::FovColor, 100, Features::AimAssist::Aimbot::FovWeight);
	}


	Matrix ViewMatrix = ReadMemory<Matrix>(GameViewPort + 0x24C);
	if (!LocalPlayer.Update()) {
		return;
	}

	DemoUser.Id = LocalPlayer.GetId();
	DemoUser.Name = LocalPlayer.GetName();
	DemoUser.WeaponName = LocalPlayer.GetWeaponName();
	DemoUser.Health = LocalPlayer.GetHealth();
	DemoUser.Armor = LocalPlayer.GetArmor();

	for (auto& ped : PedList) {
		if (!ped.Update()) {
			continue;
		}

		Vector2 pBase{}, pHead{}, pNeck{}, pLeftFoot{}, pRightFoot{};
		if (!WorldToScreen(ViewMatrix, ped.Position, pBase) ||
			!WorldToScreen(ViewMatrix, ped.BoneList[Head], pHead) ||
			!WorldToScreen(ViewMatrix, ped.BoneList[Neck], pNeck) ||
			!WorldToScreen(ViewMatrix, ped.BoneList[LeftFoot], pLeftFoot) ||
			!WorldToScreen(ViewMatrix, ped.BoneList[RightFoot], pRightFoot)) {
			continue;
		}

		float HeadToNeck = pNeck.y - pHead.y;
		float pTop = pHead.y - (HeadToNeck * 2.5f);
		float pBottom = (pLeftFoot.y > pRightFoot.y ? pLeftFoot.y : pRightFoot.y) * 1.001f;
		float pHeight = pBottom - pTop;
		float pWidth = pHeight / 3.5f;
		float pScale = pWidth / 1.5f;
		float pDistance = GetDistance(ped.Position, LocalPlayer.Position);

		if (Features::AimAssist::Settings::Crosshair) {
			ImVec2 center = ImVec2(Game.lpRect.right / 2.f, Game.lpRect.bottom / 2.f);
			ImU32 color = Features::AimAssist::Settings::CrosshairColor;
			int size = Features::AimAssist::Settings::CrosshairSize;
			int type = Features::AimAssist::Settings::CrosshairSelectedType;
			float thickness = 1.5f;

			if (Features::AimAssist::Settings::DynamicCrosshairColor && (IsTargetInCrosshair(pBase) || IsTargetInCrosshair(pHead) || IsTargetInCrosshair(pNeck) || IsTargetInCrosshair(pLeftFoot) || IsTargetInCrosshair(pRightFoot))) {
				color = DarkenColor(Features::AimAssist::Settings::CrosshairColor, 0.3f);
			}

			if (type == 0) {
				float halfSize = size * 0.5f;
				drawList->AddLine(ImVec2(center.x - halfSize, center.y), ImVec2(center.x + halfSize, center.y), color, thickness);
				drawList->AddLine(ImVec2(center.x, center.y - halfSize), ImVec2(center.x, center.y + halfSize), color, thickness);

			}
			else if (type == 1) {
				float offset = size * 0.5f;
				float length = size * 0.6f;
				drawList->AddLine(ImVec2(center.x - offset - length, center.y), ImVec2(center.x - offset, center.y), color, thickness);
				drawList->AddLine(ImVec2(center.x + offset, center.y), ImVec2(center.x + offset + length, center.y), color, thickness);
				drawList->AddLine(ImVec2(center.x, center.y - offset - length), ImVec2(center.x, center.y - offset), color, thickness);
				drawList->AddLine(ImVec2(center.x, center.y + offset), ImVec2(center.x, center.y + offset + length), color, thickness);

			}
			else if (type == 2) {
				drawList->AddCircle(center, size * 0.75f, color, 32, thickness);

			}
			else if (type == 3) {
				float halfSize = size * 0.5f;
				drawList->AddRect(ImVec2(center.x - halfSize, center.y - halfSize), ImVec2(center.x + halfSize, center.y + halfSize), color, 0.0f, 0, thickness);

			}
			else if (type == 4) {
				drawList->AddTriangle(
					ImVec2(center.x, center.y - size),
					ImVec2(center.x - size * 0.8f, center.y + size * 0.8f),
					ImVec2(center.x + size * 0.8f, center.y + size * 0.8f),
					color, thickness
				);

			}
			else if (type == 5) {
				float size1 = size;
				float size2 = size * 0.4f;
				drawList->AddLine(ImVec2(center.x - size1, center.y), ImVec2(center.x + size1, center.y), color, thickness);
				drawList->AddLine(ImVec2(center.x, center.y - size1), ImVec2(center.x, center.y + size1), color, thickness);
				drawList->AddLine(ImVec2(center.x - size2, center.y), ImVec2(center.x + size2, center.y), IM_COL32(255, 0, 0, 255), thickness + 0.5f);
				drawList->AddLine(ImVec2(center.x, center.y - size2), ImVec2(center.x, center.y + size2), IM_COL32(255, 0, 0, 255), thickness + 0.5f);

			}
			else if (type == 6) {
				drawList->AddCircleFilled(center, size * 0.25f, color, 16);

			}
			else if (type == 7) {
				drawList->AddCircle(center, size, color, 32, thickness);
				drawList->AddCircleFilled(center, size * 0.25f, IM_COL32(255, 0, 0, 255), 16);

			}
			else if (type == 8) {
				float halfSize = size * 0.5f;
				drawList->AddLine(ImVec2(center.x - halfSize, center.y - halfSize), ImVec2(center.x + halfSize, center.y + halfSize), color, thickness);
				drawList->AddLine(ImVec2(center.x + halfSize, center.y - halfSize), ImVec2(center.x - halfSize, center.y + halfSize), color, thickness);

			}
			else if (type == 9) {
				float radius = size * 1.0f;
				drawList->AddCircle(center, radius, color, 64, thickness);
				drawList->AddLine(ImVec2(center.x - radius, center.y), ImVec2(center.x + radius, center.y), color, thickness);
				drawList->AddLine(ImVec2(center.x, center.y - radius), ImVec2(center.x, center.y + radius), color, thickness);
			}
		}

		if (SilentPed != nullptr) {
			if ((GetAsyncKeyState(Features::AimAssist::Silent::HotKey) & 0x8000) && ped.GetId() == SilentPed->GetId()) {
				if (Features::AimAssist::Silent::SelectedDrawTargetType == 0) {
					ImVec2 pointPos(pBase.x, (pTop + pBottom) / 2.0f);
					float pointRadius = 5.0f;
					drawList->AddCircleFilled(pointPos, pointRadius, Features::AimAssist::Silent::DrawTargetColor);
				}
				else if (Features::AimAssist::Silent::SelectedDrawTargetType == 1) {
					float newWidth = pWidth * 1.5f;
					drawList->AddRect(ImVec2(pBase.x - newWidth / 2.0f, pTop), ImVec2(pBase.x + newWidth / 2.0f, pBottom), Features::AimAssist::Silent::DrawTargetColor, 0.0f, 0, 2);
				}
				else if (Features::AimAssist::Silent::SelectedDrawTargetType == 2) {
					ImGui::PushFont(RobotoEsp);
					const char* label = XorString("Silent Target");
					ImVec2 textSize = ImGui::CalcTextSize(label);
					ImVec2 textPos(pBase.x - textSize.x / 2.0f, (pTop + pBottom) / 2.0f - textSize.y / 2.0f);
					drawList->AddText(textPos, Features::AimAssist::Silent::DrawTargetColor, label);
					ImGui::PopFont();
				}
			}
		}

		if (AimbotPed != nullptr) {
			if ((GetAsyncKeyState(Features::AimAssist::Aimbot::HotKey) & 0x8000) && ped.GetId() == AimbotPed->GetId()) {
				if (Features::AimAssist::Aimbot::SelectedDrawTargetType == 0) {
					ImVec2 pointPos(pBase.x, (pTop + pBottom) / 2.0f);
					float pointRadius = 5.0f;
					drawList->AddCircleFilled(pointPos, pointRadius, Features::AimAssist::Aimbot::DrawTargetColor);
				}
				else if (Features::AimAssist::Aimbot::SelectedDrawTargetType == 1) {
					float newWidth = pWidth * 1.5f;
					drawList->AddRect(ImVec2(pBase.x - newWidth / 2.0f, pTop), ImVec2(pBase.x + newWidth / 2.0f, pBottom), Features::AimAssist::Aimbot::DrawTargetColor, 0.0f, 0, 2);
				}
				else if (Features::AimAssist::Aimbot::SelectedDrawTargetType == 2) {
					ImGui::PushFont(RobotoEsp);
					const char* label = XorString("Aimbot Target");
					ImVec2 textSize = ImGui::CalcTextSize(label);
					ImVec2 textPos(pBase.x - textSize.x / 2.0f, (pTop + pBottom) / 2.0f - textSize.y / 2.0f);
					drawList->AddText(textPos, Features::AimAssist::Aimbot::DrawTargetColor, label);
					ImGui::PopFont();
				}
			}
		}

		if (pDistance > Features::Players::Settings::MaxDistance) {
			continue;
		}

		if (Features::Players::Settings::IgnorePed && !ped.IsPlayer()) {
			continue;
		}

		if (Features::Players::Settings::IgnoreDeath && ped.IsDead()) {
			continue;
		}

		if (Features::Players::Settings::IgnoreInvisible && !ped.IsVisible()) {
			continue;
		}

		if (IsFriend(ped.GetId())) {
			continue;
		}

		if (Features::Players::VisualMarkers::DrawSkeleton::Enabled) {
			ImColor color = ped.IsVisible() ? Features::Players::VisualMarkers::DrawSkeleton::Color : DarkenColor(Features::Players::VisualMarkers::DrawSkeleton::Color, 0.3f);
			const Vector3 skeletonList[][2] = {
				{ ped.BoneList[Neck], ped.BoneList[Hip] },
				{ ped.BoneList[Neck], ped.BoneList[LeftHand] },
				{ ped.BoneList[Neck], ped.BoneList[RightHand] },
				{ ped.BoneList[Hip],  ped.BoneList[LeftFoot] },
				{ ped.BoneList[Hip],  ped.BoneList[RightFoot] }
			};

			for (auto& bones : skeletonList) {
				Vector2 s1, s2;
				if (Vec3Empty(bones[0]) || Vec3Empty(bones[1])) {
					continue;
				}
				if (!WorldToScreen(ViewMatrix, bones[0], s1) || !WorldToScreen(ViewMatrix, bones[1], s2)) {
					continue;
				}

				if (Features::Players::VisualMarkers::GlobalSettings::SelectedLineType == 1) {
					drawList->AddLine({ s1.x, s1.y }, { s2.x, s2.y }, ImColor(0, 0, 0, 255), Features::Players::VisualMarkers::GlobalSettings::LineWeight + 2.0f);
				}
				drawList->AddLine({ s1.x, s1.y }, { s2.x, s2.y }, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight - 1);
			}
		}

		if (Features::Players::VisualMarkers::DrawBox::Enabled) {
			ImColor color = ped.IsVisible() ? Features::Players::VisualMarkers::DrawBox::Color : DarkenColor(Features::Players::VisualMarkers::DrawBox::Color, 0.3f);
			if (Features::Players::VisualMarkers::DrawBox::SelectedType == 0) {
				float newWidth = pWidth * 1.5f;
				ImU32 fillColor = IM_COL32(0, 0, 0, 50);
				ImU32 borderColor = IM_COL32(0, 0, 0, 255);
				ImVec2 topLeft = ImVec2(pBase.x - newWidth / 2.0f, pTop);
				ImVec2 bottomRight = ImVec2(pBase.x + newWidth / 2.0f, pBottom);
				drawList->AddRectFilled(topLeft, bottomRight, fillColor);
				drawList->AddRect(topLeft, bottomRight, borderColor, 0.0f, 0, 1.0f);
				float cornerSize = newWidth * 0.3f;
				drawList->AddLine(topLeft, ImVec2(topLeft.x + cornerSize, topLeft.y), color, 2.0f);
				drawList->AddLine(topLeft, ImVec2(topLeft.x, topLeft.y + cornerSize), color, 2.0f);
				drawList->AddLine(ImVec2(bottomRight.x, topLeft.y), ImVec2(bottomRight.x - cornerSize, topLeft.y), color, 2.0f);
				drawList->AddLine(ImVec2(bottomRight.x, topLeft.y), ImVec2(bottomRight.x, topLeft.y + cornerSize), color, 2.0f);
				drawList->AddLine(ImVec2(topLeft.x, bottomRight.y), ImVec2(topLeft.x + cornerSize, bottomRight.y), color, 2.0f);
				drawList->AddLine(ImVec2(topLeft.x, bottomRight.y), ImVec2(topLeft.x, bottomRight.y - cornerSize), color, 2.0f);
				drawList->AddLine(bottomRight, ImVec2(bottomRight.x - cornerSize, bottomRight.y), color, 2.0f);
				drawList->AddLine(bottomRight, ImVec2(bottomRight.x, bottomRight.y - cornerSize), color, 2.0f);
			}
			else if (Features::Players::VisualMarkers::DrawBox::SelectedType == 1) {
				float newWidth = pWidth * 1.5f;
				if (Features::Players::VisualMarkers::GlobalSettings::SelectedLineType == 1) {
					drawList->AddRect(ImVec2(pBase.x - newWidth / 2.0f, pTop), ImVec2(pBase.x + newWidth / 2.0f, pBottom), ImColor(0, 0, 0, 255), 0.0f, 0, Features::Players::VisualMarkers::GlobalSettings::LineWeight + 5.0f);
				}
				drawList->AddRect(ImVec2(pBase.x - newWidth / 2.0f, pTop), ImVec2(pBase.x + newWidth / 2.0f, pBottom), color, 0.0f, 0, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
			}
			else if (Features::Players::VisualMarkers::DrawBox::SelectedType == 2) {
				Vector3 head3D = ped.BoneList[Head];
				Vector3 foot3D = (ped.BoneList[LeftFoot] + ped.BoneList[RightFoot]) * 0.5f;

				float height3D = head3D.z - foot3D.z;
				float width3D = height3D / 3.5f;
				float hw = width3D * 0.5f;

				std::array<Vector3, 8> corners = {
					Vector3{ head3D.x - hw, head3D.y - hw, head3D.z },
					Vector3{ head3D.x + hw, head3D.y - hw, head3D.z },
					Vector3{ head3D.x + hw, head3D.y + hw, head3D.z },
					Vector3{ head3D.x - hw, head3D.y + hw, head3D.z },
					Vector3{ foot3D.x - hw, foot3D.y - hw, foot3D.z },
					Vector3{ foot3D.x + hw, foot3D.y - hw, foot3D.z },
					Vector3{ foot3D.x + hw, foot3D.y + hw, foot3D.z },
					Vector3{ foot3D.x - hw, foot3D.y + hw, foot3D.z }
				};

				Vector2 sc[8];
				bool visible = true;
				for (int i = 0; i < 8; i++) {
					if (!WorldToScreen(ViewMatrix, corners[i], sc[i])) {
						visible = false;
						break;
					}
				}

				if (Features::Players::VisualMarkers::GlobalSettings::SelectedLineType == 1) {
					for (int i = 0; i < 8; i++) {
						int next = (i + 1) % 4;
						drawList->AddLine({ sc[i].x, sc[i].y }, { sc[next].x, sc[next].y }, ImColor(0, 0, 0, 255), Features::Players::VisualMarkers::GlobalSettings::LineWeight + 5.0f);
					}
				}
				if (visible) {
					drawList->AddLine({ sc[0].x, sc[0].y }, { sc[1].x, sc[1].y }, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
					drawList->AddLine({ sc[1].x, sc[1].y }, { sc[2].x, sc[2].y }, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
					drawList->AddLine({ sc[2].x, sc[2].y }, { sc[3].x, sc[3].y }, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
					drawList->AddLine({ sc[3].x, sc[3].y }, { sc[0].x, sc[0].y }, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
					drawList->AddLine({ sc[4].x, sc[4].y }, { sc[5].x, sc[5].y }, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
					drawList->AddLine({ sc[5].x, sc[5].y }, { sc[6].x, sc[6].y }, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
					drawList->AddLine({ sc[6].x, sc[6].y }, { sc[7].x, sc[7].y }, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
					drawList->AddLine({ sc[7].x, sc[7].y }, { sc[4].x, sc[4].y }, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
					drawList->AddLine({ sc[0].x, sc[0].y }, { sc[4].x, sc[4].y }, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
					drawList->AddLine({ sc[1].x, sc[1].y }, { sc[5].x, sc[5].y }, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
					drawList->AddLine({ sc[2].x, sc[2].y }, { sc[6].x, sc[6].y }, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
					drawList->AddLine({ sc[3].x, sc[3].y }, { sc[7].x, sc[7].y }, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
				}
			}
		}

		if (Features::Players::VisualMarkers::DrawLine::Enabled) {
			ImColor color = ped.IsVisible() ? Features::Players::VisualMarkers::DrawLine::Color : DarkenColor(Features::Players::VisualMarkers::DrawLine::Color, 0.3f);
			ImVec2 target(pBase.x, (pTop + pBottom) * 0.5f);
			float screenW = (float)Game.lpRect.right;
			float screenH = (float)Game.lpRect.bottom;
			float halfW = screenW * 0.5f;
			float halfH = screenH * 0.5f;

			ImVec2 start;
			if (Features::Players::VisualMarkers::DrawLine::SelectedLocation == 0) {
				start = ImVec2(halfW, 0.0f);
			}
			else if (Features::Players::VisualMarkers::DrawLine::SelectedLocation == 1) {
				start = ImVec2(halfW, halfH);
			}
			else {
				start = ImVec2(halfW, screenH);
			}

			if (Features::Players::VisualMarkers::GlobalSettings::SelectedLineType == 0) {
				drawList->AddLine(start, target, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
			}
			else if (Features::Players::VisualMarkers::GlobalSettings::SelectedLineType == 1) {
				drawList->AddLine(start, target, ImColor(0, 0, 0, 255), Features::Players::VisualMarkers::GlobalSettings::LineWeight + 5.0f);
				drawList->AddLine(start, target, color, Features::Players::VisualMarkers::GlobalSettings::LineWeight);
			}
		}

		if (pDistance <= Features::Players::PlayerInfo::GlobalSettings::MaxDistance && (Features::Players::PlayerInfo::DrawId::Enabled || Features::Players::PlayerInfo::DrawName::Enabled || Features::Players::PlayerInfo::DrawWeaponName::Enabled || Features::Players::PlayerInfo::DrawDistance::Enabled)) {
			float playerHeight = pBottom - pTop;
			float playerWidth = playerHeight / 3.5f;
			ImVec2 pos(pBase.x - playerWidth / 2.0f, pTop);
			ImVec2 size(playerWidth, playerHeight);

			std::string topLabels[4];
			std::string bottomLabels[4];
			int topCount = 0;
			int bottomCount = 0;

			auto addLabel = [&](int location, const std::string& label)
				{
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
						ImColor color = ped.IsVisible() ? Features::Players::PlayerInfo::DrawDistance::Color : DarkenColor(Features::Players::PlayerInfo::DrawDistance::Color, 0.3f);
						DrawTextElement(drawList, pos, size, XorString("[") + std::to_string(static_cast<int>(pDistance)) + XorString("m]"), true, Features::Players::PlayerInfo::DrawDistance::SelectedLocation, yOffset, 0.0f, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, color);
						yOffset += yStep;
						continue;
					}
					if (topLabels[i] == XorString("DrawWeaponName") && Features::Players::PlayerInfo::DrawWeaponName::Enabled) {
						ImColor color = ped.IsVisible() ? Features::Players::PlayerInfo::DrawWeaponName::Color : DarkenColor(Features::Players::PlayerInfo::DrawWeaponName::Color, 0.3f);
						DrawTextElement(drawList, pos, size, ped.GetWeaponName(), true, Features::Players::PlayerInfo::DrawWeaponName::SelectedLocation, yOffset, 0.0f, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, color);
						yOffset += yStep;
						continue;
					}
					if (topLabels[i] == XorString("DrawName") && Features::Players::PlayerInfo::DrawName::Enabled) {
						ImColor color = ped.IsVisible() ? Features::Players::PlayerInfo::DrawName::Color : DarkenColor(Features::Players::PlayerInfo::DrawName::Color, 0.3f);
						DrawTextElement(drawList, pos, size, ped.GetName(), true, Features::Players::PlayerInfo::DrawName::SelectedLocation, yOffset, 0.0f, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, color);
						yOffset += yStep;
						continue;
					}
					if (topLabels[i] == XorString("DrawId") && Features::Players::PlayerInfo::DrawId::Enabled) {
						ImColor color = ped.IsVisible() ? Features::Players::PlayerInfo::DrawId::Color : DarkenColor(Features::Players::PlayerInfo::DrawId::Color, 0.3f);
						DrawTextElement(drawList, pos, size, std::to_string(ped.GetId()), true, Features::Players::PlayerInfo::DrawId::SelectedLocation, yOffset, 0.0f, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, color);
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
						ImColor color = ped.IsVisible() ? Features::Players::PlayerInfo::DrawDistance::Color : DarkenColor(Features::Players::PlayerInfo::DrawDistance::Color, 0.3f);
						DrawTextElement(drawList, pos, size, XorString("[") + std::to_string(static_cast<int>(GetDistance(ped.Position, LocalPlayer.Position))) + XorString("m]"), true, Features::Players::PlayerInfo::DrawDistance::SelectedLocation, 0.0f, yOffset, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, color);
						yOffset += yStep;
						continue;
					}
					if (bottomLabels[i] == XorString("DrawWeaponName") && Features::Players::PlayerInfo::DrawWeaponName::Enabled) {
						ImColor color = ped.IsVisible() ? Features::Players::PlayerInfo::DrawWeaponName::Color : DarkenColor(Features::Players::PlayerInfo::DrawWeaponName::Color, 0.3f);
						DrawTextElement(drawList, pos, size, ped.GetWeaponName(), true, Features::Players::PlayerInfo::DrawWeaponName::SelectedLocation, 0.0f, yOffset, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, color);
						yOffset += yStep;
						continue;
					}
					if (bottomLabels[i] == XorString("DrawName") && Features::Players::PlayerInfo::DrawName::Enabled) {
						ImColor color = ped.IsVisible() ? Features::Players::PlayerInfo::DrawName::Color : DarkenColor(Features::Players::PlayerInfo::DrawName::Color, 0.3f);
						DrawTextElement(drawList, pos, size, ped.GetName(), true, Features::Players::PlayerInfo::DrawName::SelectedLocation, 0.0f, yOffset, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, color);
						yOffset += yStep;
						continue;
					}
					if (bottomLabels[i] == XorString("DrawId") && Features::Players::PlayerInfo::DrawId::Enabled) {
						ImColor color = ped.IsVisible() ? Features::Players::PlayerInfo::DrawId::Color : DarkenColor(Features::Players::PlayerInfo::DrawId::Color, 0.3f);
						DrawTextElement(drawList, pos, size, std::to_string(ped.GetId()), true, Features::Players::PlayerInfo::DrawId::SelectedLocation, 0.0f, yOffset, Features::Players::PlayerInfo::GlobalSettings::SelectedFont, Features::Players::PlayerInfo::GlobalSettings::SelectedFontType, color);
						yOffset += yStep;
						continue;
					}
				}
			}
		}

		if (Features::Players::StatusBars::DrawHealthBar::Enabled) {
			float newWidth = pWidth * 1.5f;
			float halfWidth = newWidth * 0.5f;
			ImVec2 topLeft = ImVec2(pBase.x - halfWidth + 4.0f, pTop);
			ImVec2 bottomRight = ImVec2(pBase.x + halfWidth, pBottom);
			float barHeight = (pBottom - pTop) + 2.0f;

			float baseBarWidth = 4.0f;
			float distanceScale = pDistance / 100.0f;
			if (distanceScale > baseBarWidth - 1.0f) distanceScale = baseBarWidth - 3.0f;
			float barWidth = baseBarWidth - distanceScale;

			float healthRatio = ped.GetHealth() / (float)Features::Players::StatusBars::GlobalSettings::MaxHealth;
			float offsetX = (Features::Players::StatusBars::DrawHealthBar::SelectedLocation == 0) ? -9.0f : 3.0f;
			ImVec2 barTopLeft = (Features::Players::StatusBars::DrawHealthBar::SelectedLocation == 0) ? ImVec2(topLeft.x + offsetX, topLeft.y) : ImVec2(bottomRight.x + offsetX, topLeft.y);
			ImVec2 barBottomRight = ImVec2(barTopLeft.x + barWidth, barTopLeft.y + barHeight);

			drawList->AddRectFilled(barTopLeft, barBottomRight, IM_COL32(50, 50, 50, 150));

			int red = 255, green = 0;
			if (healthRatio >= 0.8f) { red = (int)(255 * (1.0f - (healthRatio - 0.8f) * 5)); green = 255; }
			else if (healthRatio >= 0.6f) { red = 128; green = 255; }
			else if (healthRatio >= 0.4f) { green = 192; }
			else if (healthRatio >= 0.2f) { green = 100; }

			ImU32 barFillColor = IM_COL32(red, green, 0, 255);
			float filledHeight = barHeight * healthRatio;
			drawList->AddRectFilled(ImVec2(barTopLeft.x, barBottomRight.y - filledHeight), barBottomRight, barFillColor);

			if (pDistance <= 60) {
				drawList->AddRect(barTopLeft, barBottomRight, IM_COL32(0, 0, 0, 255), 0.0f, 0, 1.5f);
			}
			if (pDistance <= 30) {
				ImGui::PushFont(RobotoEsp2);
				char healthText[16];
				int healthPercent = static_cast<int>(healthRatio * 100.0f);
				snprintf(healthText, sizeof(healthText), "%d%%", healthPercent);
				ImVec2 textPos = (Features::Players::StatusBars::DrawHealthBar::SelectedLocation == 0) ? ImVec2(barTopLeft.x - 30.0f, barTopLeft.y) : ImVec2(barBottomRight.x + 5.0f, barTopLeft.y);
				drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), healthText);
				ImGui::PopFont();
			}
		}

		if (Features::Players::StatusBars::DrawArmorBar::Enabled) {
			float newWidth = pWidth * 1.5f;
			float halfWidth = newWidth * 0.5f;
			ImVec2 topLeft = ImVec2(pBase.x - halfWidth, pTop);
			ImVec2 bottomRight = ImVec2(pBase.x + halfWidth - 1.0f, pBottom);
			float barHeight = (pBottom - pTop) + 2.0f;

			float baseBarWidth = 4.0f;
			float distanceScale = pDistance / 100.0f;
			if (distanceScale > baseBarWidth - 1.0f) distanceScale = baseBarWidth - 3.0f;
			float barWidth = baseBarWidth - distanceScale;

			float armorRatio = ped.GetArmor() / (float)Features::Players::StatusBars::GlobalSettings::MaxArmor;
			float offsetX = (Features::Players::StatusBars::DrawArmorBar::SelectedLocation == 0) ? -9.0f : 3.0f;
			ImVec2 barTopLeft = (Features::Players::StatusBars::DrawArmorBar::SelectedLocation == 0) ? ImVec2(topLeft.x + offsetX, topLeft.y) : ImVec2(bottomRight.x + offsetX, topLeft.y);
			ImVec2 barBottomRight = ImVec2(barTopLeft.x + barWidth, barTopLeft.y + barHeight);

			drawList->AddRectFilled(barTopLeft, barBottomRight, IM_COL32(50, 50, 50, 150));

			int red = 0, green = 0, blue = 0;
			if (armorRatio >= 0.8f) { blue = 255; green = (int)(255 * (armorRatio - 0.8f) * 5); }
			else if (armorRatio >= 0.6f) { blue = 255; green = 128; }
			else if (armorRatio >= 0.4f) { blue = 192; green = 64; }
			else if (armorRatio >= 0.2f) { blue = 128; green = 32; }
			else { blue = 64; green = 0; }

			ImU32 barFillColor = IM_COL32(red, green, blue, 255);
			float filledHeight = barHeight * armorRatio;
			drawList->AddRectFilled(ImVec2(barTopLeft.x, barBottomRight.y - filledHeight), barBottomRight, barFillColor);

			if (pDistance <= 60) {
				drawList->AddRect(barTopLeft, barBottomRight, IM_COL32(0, 0, 0, 255), 0.0f, 0, 1.5f);
			}
			if (pDistance <= 30) {
				ImGui::PushFont(RobotoEsp2);
				char armorText[16];
				int armorPercent = static_cast<int>(armorRatio * 100.0f);
				snprintf(armorText, sizeof(armorText), "%d%%", armorPercent);
				ImVec2 textPos = (Features::Players::StatusBars::DrawArmorBar::SelectedLocation == 0) ? ImVec2(barTopLeft.x - 35.0f, barTopLeft.y) : ImVec2(barBottomRight.x + 5.0f, barTopLeft.y);
				drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), armorText);
				ImGui::PopFont();
			}
		}
	}

	if (Features::Vehicles::DrawPoint::Enabled || Features::Vehicles::DrawLine::Enabled || Features::Vehicles::DrawDistance::Enabled || Features::Vehicles::DrawHealthBar::Enabled) {
		for (int i = 0; i < VehicleList.size(); ++i) {
			Vehicle vehicle = VehicleList[i];
			if (Features::Vehicles::DrawLine::Enabled) {
				float screenW = (float)Game.lpRect.right;
				float screenH = (float)Game.lpRect.bottom;
				float halfW = screenW * 0.5f;
				float halfH = screenH * 0.5f;

				ImVec2 start;
				if (Features::Vehicles::DrawLine::SelectedLocation == 0) {
					start = ImVec2(halfW, 0.0f);
				}
				else if (Features::Vehicles::DrawLine::SelectedLocation == 1) {
					start = ImVec2(halfW, halfH);
				}
				else {
					start = ImVec2(halfW, screenH);
				}

				if (Features::Vehicles::DrawLine::SelectedLineType == 0) {
					drawList->AddLine(start, ImVec2(vehicle.Location.x, vehicle.Location.y), Features::Vehicles::DrawLine::Color, 1.5f);
				}
				else {
					drawList->AddLine(start, ImVec2(vehicle.Location.x, vehicle.Location.y), IM_COL32(0, 0, 0, 255), 3.0f);
					drawList->AddLine(start, ImVec2(vehicle.Location.x, vehicle.Location.y), Features::Vehicles::DrawLine::Color, 1.5f);
				}
			}

			if (Features::Vehicles::DrawPoint::Enabled) {
				if (Features::Vehicles::DrawPoint::Enabled) {
					if (Features::Vehicles::DrawPoint::SelectedLineType == 0) {
						drawList->AddCircleFilled(ImVec2(vehicle.Location.x, vehicle.Location.y), Features::Vehicles::DrawPoint::Size, Features::Vehicles::DrawPoint::Color);
					}
					else {
						drawList->AddCircleFilled(ImVec2(vehicle.Location.x, vehicle.Location.y), Features::Vehicles::DrawPoint::Size, Features::Vehicles::DrawPoint::Color);
						drawList->AddCircle(ImVec2(vehicle.Location.x, vehicle.Location.y), Features::Vehicles::DrawPoint::Size + 1, IM_COL32(0, 0, 0, 255), 32, 1.5f);
					}
				}
			}

			if (Features::Vehicles::DrawDistance::Enabled) {
				if (Features::Vehicles::DrawDistance::SelectedFont == 0) {
					ImGui::PushFont(RobotoEsp);
				}
				else if (Features::Vehicles::DrawDistance::SelectedFont == 1) {
					ImGui::PushFont(PriceDownEsp);
				}

				char distanceText[32];
				snprintf(distanceText, sizeof(distanceText), "%.0fm", vehicle.Distance);

				float xOffset = -10.0f;
				float yOffset = 30.0f;

				if (Features::Vehicles::DrawDistance::SelectedFontType == 0) {
					drawList->AddText(ImVec2(vehicle.Location.x + xOffset, vehicle.Location.y - 10 - yOffset), Features::Vehicles::DrawDistance::Color, distanceText);
				}
				else {
					drawList->AddText(ImVec2(vehicle.Location.x - 1 + xOffset, vehicle.Location.y - 11 - yOffset), IM_COL32(0, 0, 0, 255), distanceText);
					drawList->AddText(ImVec2(vehicle.Location.x + 1 + xOffset, vehicle.Location.y - 9 - yOffset), IM_COL32(0, 0, 0, 255), distanceText);
					drawList->AddText(ImVec2(vehicle.Location.x + xOffset, vehicle.Location.y - 10 - yOffset), Features::Vehicles::DrawDistance::Color, distanceText);
				}
				ImGui::PopFont();
			}

			if (Features::Vehicles::DrawHealthBar::Enabled) {
				float healthRatio = vehicle.Health / 1000.0f;
				int red = 255, green = 0;
				if (healthRatio >= 0.8f) { red = (int)(255 * (1.0f - (healthRatio - 0.8f) * 5)); green = 255; }
				else if (healthRatio >= 0.6f) { red = 128; green = 255; }
				else if (healthRatio >= 0.4f) { green = 192; }
				else if (healthRatio >= 0.2f) { green = 100; }
				ImU32 barFillColor = IM_COL32(red, green, 0, 255);
				float barWidth = 50.0f;
				float barHeight = 5.0f;
				float filledWidth = barWidth * healthRatio;
				ImColor backgroundColor = ImColor(0, 0, 0, 255);
				ImVec2 barPosition(vehicle.Location.x - barWidth / 2, vehicle.Location.y - 20);
				ImVec2 barFilled(barPosition.x + filledWidth, barPosition.y + barHeight);
				drawList->AddRectFilled(barPosition, ImVec2(barPosition.x + barWidth, barPosition.y + barHeight), backgroundColor);
				drawList->AddRectFilled(barPosition, barFilled, barFillColor);
			}
		}
	}
}

Ped FindTarget(int Fov, int Distance, int Area) {
	Ped bestTarget;
	float minFov = 9999.f;
	for (auto& ped : PedList) {
		if (!LocalPlayer.Update()) {
			break;
		}

		if (!ped.Update()) {
			continue;
		}

		float pDistance = GetDistance(ped.Position, LocalPlayer.Position);
		if (pDistance > Distance) {
			continue;
		}

		if (Features::AimAssist::Settings::IgnorePed && !ped.IsPlayer()) {
			continue;
		}

		if (Features::AimAssist::Settings::IgnoreDeath && ped.IsDead()) {
			continue;
		}

		if (Features::AimAssist::Settings::IgnoreInvisible && !ped.IsVisible()) {
			continue;
		}

		if (IsFriend(ped.GetId())) {
			continue;
		}

		Vector2 screenPosition;
		Matrix viewMatrix = ReadMemory<Matrix>(GameViewPort + 0x24C);
		if (!WorldToScreen(viewMatrix, ped.BoneList[Area], screenPosition)) {
			continue;
		}

		float currentFov = abs((Vector2(Game.lpRect.right / 2.f, Game.lpRect.bottom / 2.f) - screenPosition).Length());
		if (currentFov < Fov) {
			if (currentFov < minFov) {
				bestTarget = ped;
				minFov = currentFov;
				continue;
			}
		}
	}
	return bestTarget;
}

Vector3 CalcAngle(Vector3 localCam, Vector3 toPoint) {
	Vector3 vOut{};
	float distance = GetDistance(localCam, toPoint);
	vOut.x = (toPoint.x - localCam.x) / distance;
	vOut.y = (toPoint.y - localCam.y) / distance;
	vOut.z = (toPoint.z - localCam.z) / distance;
	return vOut;
}

void NormalizeAngles(Vector3& angle) {
	while (angle.x > 89.0f)
		angle.x -= 180.f;

	while (angle.x < -89.0f)
		angle.x += 180.f;

	while (angle.y > 180.f)
		angle.y -= 360.f;

	while (angle.y < -180.f)
		angle.y += 360.f;
}

Vector3 EndBulletPos;
void ApplySilent() {
	static uint64_t HandleBulletAddress = Offsets.GameBase + Offsets.HandleBullet;
	static uint64_t AllocPtr = Offsets.GameBase + 0x34E;

	auto CalculateRelativeOffset = [](uint64_t CurrentAddress, uint64_t TargetAddress, int Offset = 5) {
		intptr_t RelativeOffset = static_cast<intptr_t>(TargetAddress - (CurrentAddress + Offset));
		return static_cast<uint32_t>(RelativeOffset);
	};

	union
	{
		float f;
		uint32_t i;
	} EndPosX, EndPosY, EndPosZ;

	EndPosX.f = EndBulletPos.x;
	EndPosY.f = EndBulletPos.y;
	EndPosZ.f = EndBulletPos.z;

	{
		std::vector<uint8_t> ReWriteTable =
		{
			0xE9, 0x00, 0x00, 0x00, 0x00
		};

		uint32_t JmpOffset = CalculateRelativeOffset(HandleBulletAddress, AllocPtr);
		ReWriteTable[1] = static_cast<uint8_t>(JmpOffset & 0xFF);
		ReWriteTable[2] = static_cast<uint8_t>((JmpOffset >> 8) & 0xFF);
		ReWriteTable[3] = static_cast<uint8_t>((JmpOffset >> 16) & 0xFF);
		ReWriteTable[4] = static_cast<uint8_t>((JmpOffset >> 24) & 0xFF);
		WriteBytes(HandleBulletAddress, &ReWriteTable[0], ReWriteTable.size());
	}

	{
		uintptr_t currentAddress = (uintptr_t)AllocPtr;
		uintptr_t targetAddress = (uintptr_t)(HandleBulletAddress);
		intptr_t relativeOffset = static_cast<intptr_t>(targetAddress - (currentAddress + 28));
		uint32_t jmpOffset = static_cast<uint32_t>(relativeOffset);

		std::vector<uint8_t> ReWriteTable =
		{
			0x41, 0xC7, 0x01, static_cast<uint8_t>(EndPosX.i), static_cast<uint8_t>(EndPosX.i >> 8), static_cast<uint8_t>(EndPosX.i >> 16), static_cast<uint8_t>(EndPosX.i >> 24),
			0x41, 0xC7, 0x41, 0x04, static_cast<uint8_t>(EndPosY.i), static_cast<uint8_t>(EndPosY.i >> 8), static_cast<uint8_t>(EndPosY.i >> 16), static_cast<uint8_t>(EndPosY.i >> 24),
			0x41, 0xC7, 0x41, 0x08, static_cast<uint8_t>(EndPosZ.i), static_cast<uint8_t>(EndPosZ.i >> 8), static_cast<uint8_t>(EndPosZ.i >> 16), static_cast<uint8_t>(EndPosZ.i >> 24),
			0xF3, 0x41, 0x0F, 0x10, 0x19,
			0xE9, 0x00, 0x00, 0x00, 0x00
		};

		ReWriteTable[29] = static_cast<uint8_t>(jmpOffset & 0xFF);
		ReWriteTable[30] = static_cast<uint8_t>((jmpOffset >> 8) & 0xFF);
		ReWriteTable[31] = static_cast<uint8_t>((jmpOffset >> 16) & 0xFF);
		ReWriteTable[32] = static_cast<uint8_t>((jmpOffset >> 24) & 0xFF);
		WriteBytes(AllocPtr, &ReWriteTable[0], ReWriteTable.size());
	}
}

void RestoreSilent() {
	std::vector<uint8_t> ReWriteTable = { 0xF3, 0x41, 0x0F, 0x10, 0x19,0xF3, 0x41, 0x0F, 0x10, 0x41, 0x04,0xF3, 0x41, 0x0F, 0x10, 0x51, 0x08 };
	WriteBytes(Offsets.GameBase + Offsets.HandleBullet, &ReWriteTable[0], ReWriteTable.size());
	std::vector<uint8_t> AngleReWriteTable = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00 };
	WriteBytes(Offsets.GameBase + 0x34E, &AngleReWriteTable[0], AngleReWriteTable.size());
}

void AimAssist() {
	while (keepRunning) {
		if (Features::AimAssist::Silent::Enabled) {
			int SelectedAreaMask = Features::AimAssist::Silent::SelectedArea;
			std::vector<int> possibleAreas;
			if (SelectedAreaMask & (1 << 0)) possibleAreas.push_back(Head);
			if (SelectedAreaMask & (1 << 1)) possibleAreas.push_back(Neck);
			if (SelectedAreaMask & (1 << 2)) possibleAreas.push_back(Hip);
			if (SelectedAreaMask & (1 << 3)) possibleAreas.push_back(LeftHand);
			if (SelectedAreaMask & (1 << 4)) possibleAreas.push_back(RightHand);
			if (SelectedAreaMask & (1 << 5)) possibleAreas.push_back(LeftFoot);
			if (SelectedAreaMask & (1 << 6)) possibleAreas.push_back(RightFoot);
			if (SelectedAreaMask & (1 << 7)) possibleAreas.push_back(LeftAnkle);
			if (SelectedAreaMask & (1 << 8)) possibleAreas.push_back(RightAnkle);
			if (possibleAreas.empty()) {
				Sleep(10);
				continue;
			}

			int Area = possibleAreas[rand() % possibleAreas.size()];
			Ped targetPed = FindTarget(Features::AimAssist::Silent::Fov, Features::AimAssist::Silent::MaxDistance, Area);
			Vector3 selectedArea = targetPed.BoneList[Area];
			uintptr_t camera = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.Camera);
			Vector3 viewAngle = ReadMemory<Vector3>(camera + 0x3D0);
			Vector3 cameraPosition = ReadMemory<Vector3>(camera + 0x60);

			if (Vec3Empty(selectedArea)) {
				RestoreSilent();
			}
			else {
				Vector3 angle = CalcAngle(cameraPosition, selectedArea);
				Vector3 delta = angle - viewAngle;
				NormalizeAngles(delta);
				float fovDistance = sqrtf(delta.x * delta.x + delta.y * delta.y);
				if (fovDistance <= Features::AimAssist::Silent::Fov) {
					if (GetAsyncKeyState(Features::AimAssist::Silent::HotKey) & 0x8000) {
						EndBulletPos = selectedArea;
						SilentPed = &targetPed;
						ApplySilent();
					}
				}
				else {
					SilentPed = nullptr;
					RestoreSilent();
				}
			}
		}

		if (Features::AimAssist::Aimbot::Enabled) {
			int SelectedAreaMask = Features::AimAssist::Aimbot::SelectedArea;
			std::vector<int> possibleAreas;
			if (SelectedAreaMask & (1 << 0)) possibleAreas.push_back(Head);
			if (SelectedAreaMask & (1 << 1)) possibleAreas.push_back(Neck);
			if (SelectedAreaMask & (1 << 2)) possibleAreas.push_back(Hip);
			if (SelectedAreaMask & (1 << 3)) possibleAreas.push_back(LeftHand);
			if (SelectedAreaMask & (1 << 4)) possibleAreas.push_back(RightHand);
			if (SelectedAreaMask & (1 << 5)) possibleAreas.push_back(LeftFoot);
			if (SelectedAreaMask & (1 << 6)) possibleAreas.push_back(RightFoot);
			if (SelectedAreaMask & (1 << 7)) possibleAreas.push_back(LeftAnkle);
			if (SelectedAreaMask & (1 << 8)) possibleAreas.push_back(RightAnkle);
			if (possibleAreas.empty()) {
				Sleep(10);
				continue;
			}

			int Area = possibleAreas[rand() % possibleAreas.size()];
			Ped targetPed = FindTarget(Features::AimAssist::Aimbot::Fov, Features::AimAssist::Aimbot::MaxDistance, Area);
			if (GetAsyncKeyState(Features::AimAssist::Aimbot::HotKey) & 0x8000) {
				if (!Vec3Empty(targetPed.BoneList[Head])) {
					uintptr_t camera = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.Camera);
					Vector3 viewAngle = ReadMemory<Vector3>(camera + 0x3D0);
					Vector3 cameraPosition = ReadMemory<Vector3>(camera + 0x60);
					Vector3 angle;

					if (Vec3Empty(targetPed.BoneList[Area])) {
						continue;
					}
					angle = CalcAngle(cameraPosition, targetPed.BoneList[Area]);

					NormalizeAngles(angle);
					Vector3 delta = angle - viewAngle;
					NormalizeAngles(delta);
					Vector3 writeAngle = viewAngle + (Features::AimAssist::Aimbot::Smooth ? delta / Features::AimAssist::Aimbot::Smooth : delta);
					NormalizeAngles(writeAngle);
					if (!Vec3Empty(writeAngle)) {
						WriteMemory<Vector3>(camera + 0x3D0, writeAngle);
						AimbotPed = &targetPed;
					}
				}
			}
		}

		if (Features::AimAssist::Triggerbot::Enabled) {
			for (auto& ped : PedList) {
				if (!ped.Update()) {
					continue;
				}

				Vector2 pBase{}, pHead{}, pNeck{}, pLeftFoot{}, pRightFoot{};
				if (!WorldToScreen(ViewMatrix, ped.Position, pBase) ||
					!WorldToScreen(ViewMatrix, ped.BoneList[Head], pHead) ||
					!WorldToScreen(ViewMatrix, ped.BoneList[Neck], pNeck) ||
					!WorldToScreen(ViewMatrix, ped.BoneList[LeftFoot], pLeftFoot) ||
					!WorldToScreen(ViewMatrix, ped.BoneList[RightFoot], pRightFoot)) {
					continue;
				}

				float pDistance = GetDistance(ped.Position, LocalPlayer.Position);
				if (pDistance > Features::AimAssist::Triggerbot::MaxDistance) {
					continue;
				}

				if (Features::AimAssist::Settings::IgnorePed && !ped.IsPlayer()) {
					continue;
				}

				if (Features::AimAssist::Settings::IgnoreDeath && ped.IsDead()) {
					continue;
				}

				if (Features::AimAssist::Settings::IgnoreInvisible && !ped.IsVisible()) {
					continue;
				}

				if (!ped.IsVisible()) {
					continue;
				}

				if (IsFriend(ped.GetId())) {
					continue;
				}

				if (Features::AimAssist::Triggerbot::Enabled && (IsTargetInCrosshair(pBase) || IsTargetInCrosshair(pHead) || IsTargetInCrosshair(pNeck) || IsTargetInCrosshair(pLeftFoot) || IsTargetInCrosshair(pRightFoot))) {
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					Sleep(Features::AimAssist::Triggerbot::Delay);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				}
			}
		}
		Sleep(10);
	}
}

Vector3 GetPositionByID(int targetID) {
	for (auto& ped : PedList) {
		if (ped.GetId() == targetID) {
			return ped.Position;
		}
	}
}

void TeleportObject(uintptr_t Object, uintptr_t Navigation, uintptr_t ModelInfo, Vector3 Position, Vector3 VisualPosition, bool Stop) {
	float BackupMagic = 0.f;
	if (Stop) {
		BackupMagic = ReadMemory<float>(ModelInfo + 0x2C);
		WriteMemory(ModelInfo + 0x2C, 0.f);
	}

	WriteMemory(Object + 0x90, VisualPosition);
	WriteMemory(Navigation + 0x50, Position);
	if (Stop) {
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
		WriteMemory(ModelInfo + 0x2C, BackupMagic);
	}
}

void PositionTeleport(Vector3 position) {
	if (LocalPlayer.Pointer) {
		uintptr_t Navigation = ReadMemory<uint64_t>(LocalPlayer.Pointer + 0x30);
		uintptr_t ModelInfo = ReadMemory<uint64_t>(LocalPlayer.Pointer + 0x20);
		if (Navigation && ModelInfo) {
			Vector3 TeleportPos = Vector3(position.x, position.y, position.z);
			TeleportObject(LocalPlayer.Pointer, Navigation, ModelInfo, TeleportPos, TeleportPos, true);
		}
	}
}

void TeleportWaypoint() {
	for (int i = 0; i < 2000; i++) {
		uint64_t blip = ReadMemory<uint64_t>(Offsets.GameBase + Offsets.Waypoint + (i * 8));
		if (!blip) {
			continue;
		}

		int blipIcon = ReadMemory<int>(blip + 0x40);
		int blipColor = ReadMemory<int>(blip + 0x48);
		if ((blipColor != 84) || (blipIcon != 8)) {
			continue;
		}

		Vector2 waypointPos = ReadMemory<Vector2>(blip + 0x10);
		if (waypointPos.x != 0 && waypointPos.y != 0) {
			uint64_t objectPtr = NULL;
			uint64_t navigationPtr = NULL;
			uint64_t modelInfoPtr = NULL;
			if (!objectPtr || !navigationPtr || !modelInfoPtr) {
				objectPtr = (uint64_t)LocalPlayer.Pointer;
				navigationPtr = ReadMemory<uint64_t>(LocalPlayer.Pointer + 0x30);
				modelInfoPtr = ReadMemory<uint64_t>(LocalPlayer.Pointer + 0x20);
			}
			Vector3 teleportPos = Vector3(waypointPos.x, waypointPos.y, -210.f);
			PositionTeleport(teleportPos);
		}
	}
}

void Exploits() {
	while (keepRunning) {
		{
			static bool noClipOn = false;
			static bool magicInitialized = false;
			static float magicValue = 0.0f;
			if (!magicInitialized) {
				if (LocalPlayer.Pointer) {
					uintptr_t tpModelInfo = ReadMemory<uintptr_t>(LocalPlayer.Pointer + 0x20);
					if (tpModelInfo) {
						magicValue = ReadMemory<float>(tpModelInfo + 0x2C);
						magicInitialized = true;
					}
				}
			}

			if (Features::World::NoClip::Enabled) {
				noClipOn = true;
				uintptr_t camera = ReadMemory<uintptr_t>(ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.BlipList) + 0x3C0);
				Vector3 cameraPos = ReadMemory<Vector3>(camera + 0x40);
				uintptr_t tpModelInfo = ReadMemory<uintptr_t>(LocalPlayer.Pointer + 0x20);
				Vector3 tpPosition = ReadMemory<Vector3>(LocalPlayer.Pointer + 0x90);
				uintptr_t tpNavigation = ReadMemory<uintptr_t>(LocalPlayer.Pointer + 0x30);

				if (!LocalPlayer.Pointer || !tpModelInfo) {
					Sleep(10);
					continue;
				}

				Vector3 forward = Normalize({ cameraPos.x - tpPosition.x, cameraPos.y - tpPosition.y, cameraPos.z - tpPosition.z });
				Vector3 right = Normalize(forward.Cross({ 0,0,1 }));
				Vector3 up = { 0,0,1 };

				Vector3 newPosition = tpPosition;
				float speed = Features::World::NoClip::MovementSpeed * 0.1f;

				if (GetAsyncKeyState(Features::World::NoClip::ForwardKey)) newPosition = newPosition - right * speed;
				if (GetAsyncKeyState(Features::World::NoClip::BackwardKey)) newPosition = newPosition + right * speed;
				if (GetAsyncKeyState(Features::World::NoClip::LeftKey)) newPosition = newPosition - forward * speed;
				if (GetAsyncKeyState(Features::World::NoClip::RightKey)) newPosition = newPosition + forward * speed;
				if (GetAsyncKeyState(Features::World::NoClip::UpKey)) newPosition = newPosition + up * speed;
				if (GetAsyncKeyState(Features::World::NoClip::DownKey)) newPosition = newPosition - up * speed;
				TeleportObject(LocalPlayer.Pointer, tpNavigation, 0, newPosition, newPosition, false);
			}
			else {
				if (LocalPlayer.Pointer && noClipOn) {
					uintptr_t tpModelInfo = ReadMemory<uintptr_t>(LocalPlayer.Pointer + 0x20);
					if (tpModelInfo)
						WriteMemory<float>(tpModelInfo + 0x2C, magicValue);
				}
			}
		}

		{
			static bool infiniteAmmoApplied = false;
			static float oldAmmoValue = 0.0f;

			static bool noRecoilApplied = false;
			static float oldRecoilValue = 0.0f;

			static bool noSpreadApplied = false;
			static float oldSpreadValue = 0.0f;

			static bool noReloadApplied = false;
			static float oldReloadValue = 0.0f;

			static bool noRangeApplied = false;
			static float oldRangeValue = 0.0f;

			uintptr_t weaponManager = ReadMemory<uintptr_t>(LocalPlayer.Pointer + Offsets.WeaponManager);
			uintptr_t weaponInfo = ReadMemory<uintptr_t>(weaponManager + 0x20);
			if (Features::AimAssist::Settings::InfiniteAmmo) {
				uintptr_t ammoInfo = ReadMemory<uintptr_t>(weaponInfo + 0x60);
				uintptr_t ammoCount = ReadMemory<uintptr_t>(ammoInfo + 0x8);
				uintptr_t ammoCount2 = ReadMemory<uintptr_t>(ammoCount + 0x0);

				if (!infiniteAmmoApplied) {
					oldAmmoValue = ReadMemory<float>(ammoCount2 + 0x18);
					infiniteAmmoApplied = true;
				}
				WriteMemory<float>(ammoCount2 + 0x18, 30);
			}
			else if (infiniteAmmoApplied) {
				uintptr_t ammoInfo = ReadMemory<uintptr_t>(weaponInfo + 0x60);
				uintptr_t ammoCount = ReadMemory<uintptr_t>(ammoInfo + 0x8);
				uintptr_t ammoCount2 = ReadMemory<uintptr_t>(ammoCount + 0x0);
				WriteMemory<float>(ammoCount2 + 0x18, oldAmmoValue);
				infiniteAmmoApplied = false;
			}

			if (Features::AimAssist::Settings::NoRecoil) {
				if (!noRecoilApplied) {
					oldRecoilValue = ReadMemory<float>(weaponInfo + 0x2F4);
					noRecoilApplied = true;
				}
				WriteMemory<float>(weaponInfo + 0x2F4, 0.f);
			}
			else if (noRecoilApplied) {
				WriteMemory<float>(weaponInfo + 0x2F4, oldRecoilValue);
				noRecoilApplied = false;
			}

			if (Features::AimAssist::Settings::NoSpread) {
				if (!noSpreadApplied) {
					oldSpreadValue = ReadMemory<float>(weaponInfo + 0x84);
					noSpreadApplied = true;
				}
				WriteMemory<float>(weaponInfo + 0x84, 0.0f);
			}
			else if (noSpreadApplied) {
				WriteMemory<float>(weaponInfo + 0x84, oldSpreadValue);
				noSpreadApplied = false;
			}

			if (Features::AimAssist::Settings::NoReload) {
				if (!noReloadApplied) {
					oldReloadValue = ReadMemory<float>(weaponInfo + 0x134);
					noReloadApplied = true;
				}
				WriteMemory<float>(weaponInfo + 0x134, 1000);
			}
			else if (noReloadApplied) {
				WriteMemory<float>(weaponInfo + 0x134, oldReloadValue);
				noReloadApplied = false;
			}

			if (Features::AimAssist::Settings::NoRange) {
				if (!noRangeApplied) {
					oldRangeValue = ReadMemory<float>(weaponInfo + 0x28C);
					noRangeApplied = true;
				}
				WriteMemory<float>(weaponInfo + 0x28C, 1000.f);
			}
			else if (noRangeApplied) {
				WriteMemory<float>(weaponInfo + 0x28C, oldRangeValue);
				noRangeApplied = false;
			}
		}

		if (Features::Vehicles::VehicleFix::Enabled) {
			if (GetAsyncKeyState(Features::Vehicles::VehicleFix::HotKey) & 1) {
				uintptr_t vehiclePtr = ReadMemory<uintptr_t>(LocalPlayer.Pointer + Offsets.Vehicle);
				if (vehiclePtr) {
					WriteMemory<uint8_t>(vehiclePtr + 0x972, 0x17);
				}
			}
		}

		if (Features::Players::StatusBars::HealthBoost::Enabled) {
			if (GetAsyncKeyState(Features::Players::StatusBars::HealthBoost::HotKey) & 1) {
				float currentHealth = ReadMemory<float>(LocalPlayer.Pointer + Offsets.Health);
				float newHealth = currentHealth + static_cast<float>(Features::Players::StatusBars::HealthBoost::Value);
				if (newHealth > 199.0f) newHealth = 198.0f;
				WriteMemory<float>(LocalPlayer.Pointer + Offsets.Health, newHealth);
			}
		}

		if (Features::Players::StatusBars::ArmorBoost::Enabled) {
			if (GetAsyncKeyState(Features::Players::StatusBars::ArmorBoost::HotKey) & 1) {
				float currentArmor = ReadMemory<float>(LocalPlayer.Pointer + Offsets.Armor);
				float newArmor = currentArmor + static_cast<float>(Features::Players::StatusBars::ArmorBoost::Value);
				if (newArmor > 99.0f) newArmor = 98.0f;
				WriteMemory<float>(LocalPlayer.Pointer + Offsets.Armor, newArmor);
			}
		}

		if (Features::World::SemiGodMode::Enabled) {
			float currentHealth = ReadMemory<float>(LocalPlayer.Pointer + Offsets.Health);
			if (currentHealth < 170.0f) {
				WriteMemory<float>(LocalPlayer.Pointer + Offsets.Health, 198.0f);
			}
		}
		Sleep(10);
	}
}

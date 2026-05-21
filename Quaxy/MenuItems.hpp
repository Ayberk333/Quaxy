ImFont* Roboto = nullptr;
ImFont* Roboto2 = nullptr;
ImFont* RobotoBold = nullptr;
ImFont* PermanentMarker = nullptr;
ImFont* RobotoEsp = nullptr;
ImFont* RobotoEsp2 = nullptr;
ImFont* PriceDownEsp = nullptr;

static float pageAlpha = 0.f;
static float tabAlpha = 0.f;
static float pageAdd;
static float tabAdd;
static int currentPage = 0;
static int activeTab = 0;
static int pageSwitchTab = 0;
static int pageSwitchTab2 = 0;
static int pageSwitch1 = 0;
static int totalPages = 0;
static int pageTab = 0;

namespace Images {
	ID3D11ShaderResourceView* PreviewSlow = nullptr;
	ID3D11ShaderResourceView* ArrowsIcon = nullptr;
	ID3D11ShaderResourceView* AimIcon = nullptr;
	ID3D11ShaderResourceView* PlayersIcon = nullptr;
	ID3D11ShaderResourceView* VehiclesIcon = nullptr;
	ID3D11ShaderResourceView* WorldIcon = nullptr;
	ID3D11ShaderResourceView* ServerIcon = nullptr;
	ID3D11ShaderResourceView* SettingsIcon = nullptr;
	ID3D11ShaderResourceView* FeaturesIcon = nullptr;
	ID3D11ShaderResourceView* AdditionalIcon = nullptr;
	ID3D11ShaderResourceView* UserIcon = nullptr;
}

void CreateTextures(ID3D11Device* d3dDevice) {
	D3DX11_IMAGE_LOAD_INFO info{};
	ID3DX11ThreadPump* pump{ nullptr };
	if (Images::PreviewSlow == nullptr) D3DX11CreateShaderResourceViewFromMemory(d3dDevice, PreviewImageData, sizeof(PreviewImageData), &info, pump, &Images::PreviewSlow, 0);
	if (Images::ArrowsIcon == nullptr) D3DX11CreateShaderResourceViewFromMemory(d3dDevice, ArrowsIconData, sizeof(ArrowsIconData), &info, pump, &Images::ArrowsIcon, 0);
	if (Images::AimIcon == nullptr) D3DX11CreateShaderResourceViewFromMemory(d3dDevice, AimIconData, sizeof(AimIconData), &info, pump, &Images::AimIcon, 0);
	if (Images::PlayersIcon == nullptr) D3DX11CreateShaderResourceViewFromMemory(d3dDevice, PlayersIconData, sizeof(PlayersIconData), &info, pump, &Images::PlayersIcon, 0);
	if (Images::VehiclesIcon == nullptr) D3DX11CreateShaderResourceViewFromMemory(d3dDevice, VehiclesIconData, sizeof(VehiclesIconData), &info, pump, &Images::VehiclesIcon, 0);
	if (Images::WorldIcon == nullptr) D3DX11CreateShaderResourceViewFromMemory(d3dDevice, WorldIconData, sizeof(WorldIconData), &info, pump, &Images::WorldIcon, 0);
	if (Images::ServerIcon == nullptr) D3DX11CreateShaderResourceViewFromMemory(d3dDevice, ServerIconData, sizeof(ServerIconData), &info, pump, &Images::ServerIcon, 0);
	if (Images::SettingsIcon == nullptr) D3DX11CreateShaderResourceViewFromMemory(d3dDevice, SettingsIconData, sizeof(SettingsIconData), &info, pump, &Images::SettingsIcon, 0);
	if (Images::FeaturesIcon == nullptr) D3DX11CreateShaderResourceViewFromMemory(d3dDevice, FeaturesIconData, sizeof(FeaturesIconData), &info, pump, &Images::FeaturesIcon, 0);
	if (Images::AdditionalIcon == nullptr) D3DX11CreateShaderResourceViewFromMemory(d3dDevice, AdditionalIconData, sizeof(AdditionalIconData), &info, pump, &Images::AdditionalIcon, 0);
	if (Images::UserIcon == nullptr) D3DX11CreateShaderResourceViewFromMemory(d3dDevice, UserIconData, sizeof(UserIconData), &info, pump, &Images::UserIcon, 0);
}

void ReleaseTextures() {
	auto releaseTexture = [](ID3D11ShaderResourceView*& texture) {
		if (texture) {
			texture->Release();
			texture = nullptr;
		}
	};

	releaseTexture(Images::PreviewSlow);
	releaseTexture(Images::ArrowsIcon);
	releaseTexture(Images::AimIcon);
	releaseTexture(Images::PlayersIcon);
	releaseTexture(Images::VehiclesIcon);
	releaseTexture(Images::WorldIcon);
	releaseTexture(Images::ServerIcon);
	releaseTexture(Images::SettingsIcon);
	releaseTexture(Images::FeaturesIcon);
	releaseTexture(Images::AdditionalIcon);
	releaseTexture(Images::UserIcon);
}

void CustomStyleColor() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiContext& context = *GImGui;
	style.ChildRounding = 10.f;
	style.WindowRounding = 16.f;
	style.WindowPadding = ImVec2(0, 0);
	style.Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
	style.Colors[ImGuiCol_WindowBg] = ImLerp(style.Colors[ImGuiCol_WindowBg], ImColor(12, 18, 29, 255), context.IO.DeltaTime * 10.f);
	style.Colors[ImGuiCol_Header] = MenuSetting::Color1;
	style.Colors[ImGuiCol_HeaderHovered] = MenuSetting::Color1;
	style.Colors[ImGuiCol_HeaderActive] = MenuSetting::Color1;
	MenuSetting::Color1 = ImLerp(MenuSetting::Color1, ImColor(74, 144, 194, 255), context.IO.DeltaTime * 10.f);
	MenuSetting::Color2 = ImLerp(MenuSetting::Color2, ImColor(16, 26, 40, 255), context.IO.DeltaTime * 10.f);
	MenuSetting::Color3 = ImLerp(MenuSetting::Color3, ImColor(24, 42, 62, 255), context.IO.DeltaTime * 10.f);
	MenuSetting::Color4 = ImLerp(MenuSetting::Color4, ImColor(9, 16, 27, 255), context.IO.DeltaTime * 10.f);
	MenuSetting::DecorativeRectBackground = ImLerp(MenuSetting::DecorativeRectBackground, ImColor(18, 31, 47, 255), context.IO.DeltaTime * 10.f);
	MenuSetting::ChildRect = ImLerp(MenuSetting::ChildRect, ImColor(11, 20, 32, 255), context.IO.DeltaTime * 10.f);
	MenuSetting::Black = ImLerp(MenuSetting::Black, ImColor(225, 239, 255, 255), context.IO.DeltaTime * 10.f);
	MenuSetting::Black1 = ImLerp(MenuSetting::Black1, ImColor(204, 224, 244, 255), context.IO.DeltaTime * 10.f);
	MenuSetting::BlackIn = ImLerp(MenuSetting::BlackIn, ImColor(162, 190, 217, 190), context.IO.DeltaTime * 10.f);
	MenuSetting::BorderChild = ImLerp(MenuSetting::BorderChild, ImColor(58, 101, 135, 64), context.IO.DeltaTime * 10.f);
	MenuSetting::LineChild = ImLerp(MenuSetting::LineChild, ImColor(85, 166, 219, 210), context.IO.DeltaTime * 10.f);
	MenuSetting::ShadowTab = ImLerp(MenuSetting::ShadowTab, ImColor(45, 122, 181, 145), context.IO.DeltaTime * 10.f);
	MenuSetting::CheckboxBackground = ImLerp(MenuSetting::CheckboxBackground, ImColor(74, 144, 194, 255), context.IO.DeltaTime * 10.f);
	MenuSetting::CheckboxInBackground = ImLerp(MenuSetting::CheckboxInBackground, ImColor(20, 34, 51, 255), context.IO.DeltaTime * 10.f);
	MenuSetting::CircleCheckbox = ImLerp(MenuSetting::CircleCheckbox, ImColor(227, 244, 255, 255), context.IO.DeltaTime * 10.f);
	MenuSetting::CircleCheckboxIn = ImLerp(MenuSetting::CircleCheckboxIn, ImColor(159, 189, 215, 150), context.IO.DeltaTime * 10.f);
	MenuSetting::Separator = ImLerp(MenuSetting::Separator, ImColor(121, 157, 190, 48), context.IO.DeltaTime * 10.f);
	MenuSetting::SwitchInActive = ImLerp(MenuSetting::SwitchInActive, ImColor(20, 34, 51, 255), context.IO.DeltaTime * 10.f);
	MenuSetting::SwitchText = ImLerp(MenuSetting::SwitchText, ImColor(225, 239, 255, 255), context.IO.DeltaTime * 10.f);
}

ImColor DarkenColor(const ImColor& color, float factor = 0.7f) {
	float r = ImClamp(color.Value.x * factor, 0.0f, 1.0f);
	float g = ImClamp(color.Value.y * factor, 0.0f, 1.0f);
	float b = ImClamp(color.Value.z * factor, 0.0f, 1.0f);
	float a = color.Value.w;
	return ImColor(r, g, b, a);
}

namespace Features {
	namespace AimAssist {
		namespace Settings {
			bool Crosshair = false;
			int CrosshairSelectedType = 0;
			const char* CrosshairType[10]{ "Type 1", "Type 2", "Type 3", "Type 4", "Type 5", "Type 6", "Type 7", "Type 8", "Type 9", "Type 10" };
			ImColor CrosshairColor = ImColor(255, 255, 255);
			int CrosshairSize = 15;
			bool DynamicCrosshairColor = false;
			bool IgnorePed = true;
			bool IgnoreDeath = true;
			bool IgnoreInvisible = false;
			bool InfiniteAmmo = false;
			bool NoRecoil = false;
			bool NoSpread = false;
			bool NoReload = false;
			bool NoRange = false;
		}

		namespace Silent {
			bool Enabled = false;
			int HotKey = 0;
			int Fov = 20;
			int SelectedArea = 0;
			const char* TargetArea[9]{ "Head", "Neck", "Hip", "Left Hand", "Right Hand", "Left Foot", "Right Foot", "Left Ankle", "Right Ankle" };
			int MaxDistance = 300;
			bool DrawFov = false;
			ImColor FovColor = ImColor(255, 255, 255);
			int FovWeight = 2;
			bool DrawTarget = false;
			ImColor DrawTargetColor = ImColor(255, 255, 255);
			int SelectedDrawTargetType = 0;
			const char* DrawTargetTypes[3]{ "Point", "Box", "Text" };
		}

		namespace Aimbot {
			bool Enabled = false;
			int HotKey = 0;
			int Fov = 20;
			int Smooth = 5;
			int SelectedArea = 0;
			const char* TargetArea[9]{ "Head", "Neck", "Hip", "Left Hand", "Right Hand", "Left Foot", "Right Foot", "Left Ankle", "Right Ankle" };
			int MaxDistance = 300;
			bool DrawFov = false;
			ImColor FovColor = ImColor(255, 255, 255);
			int FovWeight = 2;
			bool DrawTarget = false;
			ImColor DrawTargetColor = ImColor(255, 255, 255);
			int SelectedDrawTargetType = 0;
			const char* DrawTargetTypes[3]{ "Point", "Box", "Text" };
		}

		namespace Triggerbot {
			bool Enabled = false;
			int Delay = 5;
			int MaxDistance = 300;
		}
	}

	namespace Players {
		namespace Settings {
			bool IgnorePed = true;
			bool IgnoreDeath = true;
			bool IgnoreInvisible = false;
			int MaxDistance = 300;
		}

		namespace VisualMarkers {
			namespace GlobalSettings {
				int SelectedLineType = 1;
				const char* LineTypes[2]{ "Basic", "Outlined" };
				int LineWeight = 2;
			}

			namespace DrawSkeleton {
				bool Enabled = false;
				ImColor Color = ImColor(255, 255, 255);
			}

			namespace DrawBox {
				bool Enabled = false;
				ImColor Color = ImColor(92, 157, 209);
				int SelectedType = 0;
				const char* Types[3]{ "Corner", "2D", "3D"};
			}

			namespace DrawLine {
				bool Enabled = false;
				ImColor Color = ImColor(84, 151, 205);
				int SelectedLocation = 2;
				const char* Locations[3]{ "Top", "Center", "Bottom" };
			}
		}

		namespace PlayerInfo {
			namespace GlobalSettings {
				int SelectedFont = 0;
				const char* Fonts[2] = {"Roboto", "Price Down"};
				int SelectedFontType = 0;
				const char* FontTypes[2]{ "Basic", "Outlined" };
				int MaxDistance = 70;
			}

			namespace DrawId {
				bool Enabled = false;
				ImColor Color = ImColor(255, 255, 255);
				int SelectedLocation = 0;
				const char* Locations[2]{ "Top", "Bottom" };
			}

			namespace DrawName {
				bool Enabled = false;
				ImColor Color = ImColor(100, 196, 190);
				int SelectedLocation = 0;
				const char* Locations[2]{ "Top", "Bottom" };
			}

			namespace DrawWeaponName {
				bool Enabled = false;
				ImColor Color = ImColor(255, 255, 255);
				int SelectedLocation = 1;
				const char* Locations[2]{ "Top", "Bottom" };
			}

			namespace DrawDistance {
				bool Enabled = false;
				ImColor Color = ImColor(126, 174, 219);
				int SelectedLocation = 1;
				const char* Locations[2]{ "Top", "Bottom" };
			}
		}

		namespace StatusBars {
			namespace GlobalSettings {
				int MaxHealth = 200;
				int MaxArmor = 100;
			}

			namespace DrawHealthBar {
				bool Enabled = false;
				int SelectedLocation = 0;
				const char* Locations[2]{ "Left", "Right" };
			}

			namespace HealthBoost {
				bool Enabled = false;
				int Value = 50;
				int HotKey = 0;
			}

			namespace DrawArmorBar {
				bool Enabled = false;
				int SelectedLocation = 1;
				const char* Locations[2]{ "Left", "Right" };
			}

			namespace ArmorBoost {
				bool Enabled = false;
				int Value = 50;
				int HotKey = 0;
			}
		}
	}

	namespace Vehicles {
		namespace Settings {
			bool IgnoreLocalVehicle = true;
			int MaxVehicleCount = 500;
			int MaxDistance = 300;
		}

		namespace DrawPoint {
			bool Enabled = false;
			ImColor Color = ImColor(98, 169, 220);
			int SelectedLineType = 1;
			const char* LineTypes[2]{ "Basic", "Outlined" };
			int Size = 7;
		}

		namespace DrawLine {
			bool Enabled = false;
			ImColor Color = ImColor(84, 142, 188);
			int SelectedLocation = 2;
			const char* Locations[3]{ "Top", "Center", "Bottom" };
			int SelectedLineType = 1;
			const char* LineTypes[2]{ "Basic", "Outlined" };
		}

		namespace DrawDistance {
			bool Enabled = false;
			ImColor Color = ImColor(255, 255, 255);
			int SelectedFont = 0;
			const char* Fonts[2] = { "Roboto", "Price Down" };
			int SelectedFontType = 0;
			const char* FontTypes[2]{ "Basic", "Outlined" };
			int MaxDistance = 70;
		}

		namespace DrawHealthBar {
			bool Enabled = false;
		}

		namespace VehicleFix {
			bool Enabled = false;
			int HotKey = 0;
		}
	}

	namespace World {
		namespace NoClip{
			bool Enabled = false;
			int MovementSpeed = 5;
			int ForwardKey = 0x57;
			int BackwardKey = 0x53;
			int LeftKey = 0x41;
			int RightKey = 0x44;
			int UpKey = 0x20;
			int DownKey = 0xA2;
		}

		namespace SemiGodMode {
			bool Enabled = false;
		}

		namespace Teleport {
			struct Vector3 {
				float x, y, z;
			};

			struct TeleportLocation {
				const char* Name;
				Vector3 Position;
			};

			const TeleportLocation teleportData[] = {
				{ "Legion Square", { 190.52f, -873.23f, 31.5f } },
				{ "Paleto Bay", { -138.52f, 6356.99f, 31.49f } },
				{ "Main LS Customs", { -365.425f, -131.809f, 37.873f } },
				{ "IAA Roof", { 134.085f, -637.859f, 262.851f } },
				{ "FIB Roof", { -150.126f, -754.591f, 262.865f } },
				{ "Maze Bank", { -75.015f, -818.215f, 326.176f } },
				{ "Mount Chiliad", { 495.0f, 5589.0f, 795.0f } },
				{ "Casino", { 911.957f, 38.3382f, 80.7207f } },
				{ "Prison", { 1702.08f, 2650.51f, 45.5649f } },
				{ "Military Base", { -2751.12f, 3316.4f, 32.8117f } },
				{ "Void", { 15000.0f, 15000.0f, 0.0f } },
				{ "Central Los Santos Hospital", { 339.85f, -1394.56f, 32.51f } },
				{ "Pillbox Hill Medical Center", { 307.87f, -595.55f, 43.28f } },
				{ "Mount Zonah Medical Center", { -450.99f, -340.91f, 34.50f } },
				{ "Sandy Shores Medical Center", { 1839.6f, 3672.93f, 34.28f } },
				{ "Paleto Bay Medical Center", { -247.76f, 6325.57f, 32.43f } },
				{ "Rockford Hills Medical Center", { -874.51f, -307.55f, 39.58f } },
				{ "Vinewood Medical Center", { 639.11f, 1.12f, 82.79f } },
				{ "Eclipse Medical Tower", { -691.28f, 293.03f, 83.16f } },
				{ "Davis Medical Center", { 360.65f, -582.14f, 28.82f } },
				{ "El Burro Heights Hospital", { 1154.02f, -1520.58f, 34.84f } },
				{ "Hawick Avenue Clinic", { 305.24f, -1434.0f, 29.8f } },
				{ "Bayview Lodge Hospital", { -245.18f, 6229.42f, 31.49f } },
				{ "Thomson Scrapyard Clinic", { 2487.47f, 4960.48f, 44.37f } },
				{ "Grapeseed Medical Clinic", { 1826.9f, 3685.8f, 34.27f } },
				{ "Torture Room X", { 147.170f, -2201.804f, 4.688f } },
				{ "Trevor's Meth Lab", { 1391.773f, 3608.716f, 38.942f } },
				{ "Pacific Standard Bank Vault", { 255.851f, 217.030f, 101.683f } },
				{ "Lester's House", { 1273.898f, -1719.304f, 54.771f } },
				{ "Strip Club DJ Booth", { 126.135f, -1278.583f, 29.270f } },
				{ "Blaine County Savings Bank", { -109.299f, 6464.035f, 31.627f } },
				{ "Police Station", { 436.491f, -982.172f, 30.699f } },
				{ "Humane Labs Entrance", { 3619.749f, 2742.740f, 28.690f } },
				{ "Burnt FIB Building", { 160.868f, -745.831f, 250.063f } },
				{ "10 Car Garage Back Room", { 223.193f, -967.322f, 99.000f } },
				{ "Humane Labs Tunnel", { 3525.495f, 3705.301f, 20.992f } },
				{ "Floyd's Apartment", { -1150.703f, -1520.713f, 10.633f } }
			};
		}
	}

	namespace Settings {
		bool StreamProof = false;
		int MenuKey = 45;
		int SelectedPlayerCountType = 0;
		const char* PlayerCountTypes[2]{ "Fetch Server", "Static" };
		int MaxPlayerCount = 500;
	}
}

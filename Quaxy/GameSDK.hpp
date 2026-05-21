#include "SimpleMath.h"

static struct GameStruct {
	DWORD pID;
	HANDLE hProcess;
	HWND hWnd;
	LPCSTR lpClassName = "grcWindow";
	std::string Path;
	std::string Version;
	RECT lpRect;
	POINT lpPoint;
}Game;

struct OffsetsStruct {
	uintptr_t GameBase;
	uintptr_t CitizenPlayernamesBase;
	uintptr_t NetBase;
	uintptr_t GameWorld;
	uintptr_t ReplayInterface;
	uintptr_t ViewPort;
	uintptr_t Camera;
	uintptr_t BoneList;
	uintptr_t WeaponManager;
	uintptr_t HandleBullet;
	uintptr_t VisibleFlag;
	uintptr_t BlipList;
	uintptr_t Vehicle;
	uintptr_t Waypoint;
	uintptr_t LocalPlayer;
	uintptr_t PlayerInfo;
	uintptr_t Id;
	uintptr_t Health;
	uintptr_t MaxHealth;
	uintptr_t Armor;
} Offsets;

#include "Memory.hpp"

bool FetchOffsetsAndVersion() {
	while (!Offsets.GameBase) {
		Debug(XorString("Game Base is empty. Attempting to retrieve Game Base..."), LOG_INFO);
		Offsets.GameBase = GetBaseAddress();
	}

	char buffer[100];
	sprintf(buffer, XorString("Game Base retrieved successfully. Address: 0x%llX"), (unsigned long long)Offsets.GameBase);
	Debug(buffer, LOG_SUCCESSFUL);

	while (!Offsets.CitizenPlayernamesBase) {
		Debug(XorString("Citizen Playernames Base is empty. Attempting to retrieve Citizen Playernames Base..."), LOG_INFO);
		Offsets.CitizenPlayernamesBase = GetBaseAddress(XorString("citizen-playernames-five.dll"));
	}

	sprintf(buffer, XorString("Citizen Playernames Base retrieved successfully. Address: 0x%llX"), (unsigned long long)Offsets.CitizenPlayernamesBase);
	Debug(buffer, LOG_SUCCESSFUL);

	while (!Offsets.NetBase) {
		Debug(XorString("Net Base is empty. Attempting to retrieve Net Base..."), LOG_INFO);
		Offsets.NetBase = GetBaseAddress(XorString("net.dll"));
	}

	sprintf(buffer, XorString("Net Base retrieved successfully. Address: 0x%llX"), (unsigned long long)Offsets.NetBase);
	Debug(buffer, LOG_SUCCESSFUL);

	bool findVersion = false;
	while (true) {
		Debug(XorString("Game Offsets is empty. Attempting to retrieve Game Offsets..."), LOG_INFO);
		Game.Version = XorString("3507");
		{
			Offsets.GameWorld = 0x25EC580;
			Offsets.ReplayInterface = 0x1FB0418;
			Offsets.ViewPort = 0x2058BA0;
			Offsets.Camera = 0x2059778;
			Offsets.BoneList = 0x410;
			Offsets.WeaponManager = 0x10B8;
			Offsets.HandleBullet = 0x102D550;
			Offsets.VisibleFlag = 0x145C;
			Offsets.BlipList = 0x2061870;
			Offsets.Vehicle = 0x0D10;
			Offsets.Waypoint = 0x2047D50;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10A8;
			Offsets.Id = 0xE8;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x284;
			Offsets.Armor = 0x150C;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {

				char buffer[100];
				sprintf(buffer, XorString("Game World retrieved successfully. Address: 0x%llX"), (unsigned long long)GameWorld);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game ViewPort retrieved successfully. Address: 0x%llX"), (unsigned long long)GameViewPort);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game Replay Interface retrieved successfully. Address: 0x%llX"), (unsigned long long)GameReplayInterface);
				Debug(buffer, LOG_SUCCESSFUL);

				Debug(XorString("Game version successfully found: ") + Game.Version, LOG_SUCCESSFUL);
				findVersion = true;
				break;
			}
		}
		Game.Version = XorString("3570");
		{
			Offsets.GameWorld = 0x25D7108;
			Offsets.ReplayInterface = 0x1F9A9D8;
			Offsets.ViewPort = 0x20431C0;
			Offsets.Camera = 0x2043DF8;
			Offsets.BoneList = 0x410;
			Offsets.WeaponManager = 0x10B8;
			Offsets.HandleBullet = 0x102FF89;
			Offsets.VisibleFlag = 0x145C;
			Offsets.BlipList = 0x20440C8;
			Offsets.Vehicle = 0x0D10;
			Offsets.Waypoint = 0x2047D50;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10A8;
			Offsets.Id = 0xE8;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x284;
			Offsets.Armor = 0x150C;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {

				char buffer[100];
				sprintf(buffer, XorString("Game World retrieved successfully. Address: 0x%llX"), (unsigned long long)GameWorld);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game ViewPort retrieved successfully. Address: 0x%llX"), (unsigned long long)GameViewPort);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game Replay Interface retrieved successfully. Address: 0x%llX"), (unsigned long long)GameReplayInterface);
				Debug(buffer, LOG_SUCCESSFUL);

				Debug(XorString("Game version successfully found: ") + Game.Version, LOG_SUCCESSFUL);
				findVersion = true;
				break;
			}
		}

		Game.Version = XorString("3323");
		{
			Offsets.GameWorld = 0x25C15B0;
			Offsets.ReplayInterface = 0x1F85458;
			Offsets.ViewPort = 0x202DC50;
			Offsets.Camera = 0x202E878;
			Offsets.BoneList = 0x410;
			Offsets.WeaponManager = 0x10B8;
			Offsets.HandleBullet = 0x1026CAD;
			Offsets.VisibleFlag = 0x145C;
			Offsets.BlipList = 0x2002888;
			Offsets.Vehicle = 0x0D10;
			Offsets.Waypoint = 0x2022DE0;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10A8;
			Offsets.Id = 0xE8;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x284;
			Offsets.Armor = 0x150C;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {

				char buffer[100];
				sprintf(buffer, XorString("Game World retrieved successfully. Address: 0x%llX"), (unsigned long long)GameWorld);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game ViewPort retrieved successfully. Address: 0x%llX"), (unsigned long long)GameViewPort);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game Replay Interface retrieved successfully. Address: 0x%llX"), (unsigned long long)GameReplayInterface);
				Debug(buffer, LOG_SUCCESSFUL);

				Debug(XorString("Game version successfully found: ") + Game.Version, LOG_SUCCESSFUL);
				findVersion = true;
				break;
			}
		}

		Game.Version = XorString("3258");
		{
			Offsets.GameWorld = 0x25B14B0;
			Offsets.ReplayInterface = 0x1FBD4F0;
			Offsets.ViewPort = 0x201DBA0;
			Offsets.Camera = 0x201E7D0;
			Offsets.BoneList = 0x410;
			Offsets.WeaponManager = 0x10B8;
			Offsets.HandleBullet = 0x101A65D;
			Offsets.VisibleFlag = 0x145C;
			Offsets.BlipList = 0x2002FA0;
			Offsets.Vehicle = 0x0D10;
			Offsets.Waypoint = 0x2023400;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10A8;
			Offsets.Id = 0xE8;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x284;
			Offsets.Armor = 0x150C;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {

				char buffer[100];
				sprintf(buffer, XorString("Game World retrieved successfully. Address: 0x%llX"), (unsigned long long)GameWorld);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game ViewPort retrieved successfully. Address: 0x%llX"), (unsigned long long)GameViewPort);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game Replay Interface retrieved successfully. Address: 0x%llX"), (unsigned long long)GameReplayInterface);
				Debug(buffer, LOG_SUCCESSFUL);

				Debug(XorString("Game version successfully found: ") + Game.Version, LOG_SUCCESSFUL);
				findVersion = true;
				break;
			}
		}

		Game.Version = XorString("3095");
		{
			Offsets.GameWorld = 0x2593320;
			Offsets.ReplayInterface = 0x1F58B58;
			Offsets.ViewPort = 0x20019E0;
			Offsets.Camera = 0x20025B8;
			Offsets.BoneList = 0x410;
			Offsets.WeaponManager = 0x10B8;
			Offsets.HandleBullet = 0x100F5A4;
			Offsets.VisibleFlag = 0x145C;
			Offsets.BlipList = 0x2002888;
			Offsets.Vehicle = 0x0D10;
			Offsets.Waypoint = 0x2002FA0;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10A8;
			Offsets.Id = 0xE8;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x284;
			Offsets.Armor = 0x150C;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {

				char buffer[100];
				sprintf(buffer, XorString("Game World retrieved successfully. Address: 0x%llX"), (unsigned long long)GameWorld);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game ViewPort retrieved successfully. Address: 0x%llX"), (unsigned long long)GameViewPort);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game Replay Interface retrieved successfully. Address: 0x%llX"), (unsigned long long)GameReplayInterface);
				Debug(buffer, LOG_SUCCESSFUL);

				Debug(XorString("Game version successfully found: ") + Game.Version, LOG_SUCCESSFUL);
				findVersion = true;
				break;
			}
		}

		Game.Version = XorString("2944");
		{
			Offsets.GameWorld = 0x257BEA0;
			Offsets.ReplayInterface = 0x1F42068;
			Offsets.ViewPort = 0x1FEAAC0;
			Offsets.Camera = 0x1FEB968;
			Offsets.BoneList = 0x410;
			Offsets.WeaponManager = 0x10B8;
			Offsets.HandleBullet = 0x1003F80;
			Offsets.VisibleFlag = 0x145C;
			Offsets.BlipList = 0x1FEB968;
			Offsets.Vehicle = 0x0D10;
			Offsets.Waypoint = 0x1FF3130;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10A8;
			Offsets.Id = 0xE8;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x284;
			Offsets.Armor = 0x150C;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {

				char buffer[100];
				sprintf(buffer, XorString("Game World retrieved successfully. Address: 0x%llX"), (unsigned long long)GameWorld);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game ViewPort retrieved successfully. Address: 0x%llX"), (unsigned long long)GameViewPort);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game Replay Interface retrieved successfully. Address: 0x%llX"), (unsigned long long)GameReplayInterface);
				Debug(buffer, LOG_SUCCESSFUL);

				Debug(XorString("Game version successfully found: ") + Game.Version, LOG_SUCCESSFUL);
				findVersion = true;
				break;
			}
		}

		Game.Version = XorString("2802");
		{
			Offsets.GameWorld = 0x254D448;
			Offsets.ReplayInterface = 0x1F5B820;
			Offsets.ViewPort = 0x1FBC100;
			Offsets.Camera = 0x1FBCCD8;
			Offsets.BoneList = 0x410;
			Offsets.WeaponManager = 0x10B8;
			Offsets.HandleBullet = 0xFF716C;
			Offsets.VisibleFlag = 0x145C;
			Offsets.BlipList = 0x1FBCFA8;
			Offsets.Vehicle = 0x0D10;
			Offsets.Waypoint = 0x1FBD6E0;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10A8;
			Offsets.Id = 0xE8;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x284;
			Offsets.Armor = 0x150C;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {

				char buffer[100];
				sprintf(buffer, XorString("Game World retrieved successfully. Address: 0x%llX"), (unsigned long long)GameWorld);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game ViewPort retrieved successfully. Address: 0x%llX"), (unsigned long long)GameViewPort);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game Replay Interface retrieved successfully. Address: 0x%llX"), (unsigned long long)GameReplayInterface);
				Debug(buffer, LOG_SUCCESSFUL);

				Debug(XorString("Game version successfully found: ") + Game.Version, LOG_SUCCESSFUL);
				findVersion = true;
				break;
			}
		}

		Game.Version = XorString("2699");
		{
			Offsets.GameWorld = 0x26684D8;
			Offsets.ReplayInterface = 0x20304C8;
			Offsets.ViewPort = 0x20D8C90;
			Offsets.Camera = 0x20D9868;
			Offsets.BoneList = 0x430;
			Offsets.WeaponManager = 0x10D8;
			Offsets.HandleBullet = 0xFF9D90;
			Offsets.VisibleFlag = 0x147C;
			Offsets.BlipList = 0x20D9B38;
			Offsets.Vehicle = 0xD30;
			Offsets.Waypoint = 0x20E1420;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10C8;
			Offsets.Id = 0x88;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x2A0;
			Offsets.Armor = 0x1530;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {

				char buffer[100];
				sprintf(buffer, XorString("Game World retrieved successfully. Address: 0x%llX"), (unsigned long long)GameWorld);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game ViewPort retrieved successfully. Address: 0x%llX"), (unsigned long long)GameViewPort);
				Debug(buffer, LOG_SUCCESSFUL);

				sprintf(buffer, XorString("Game Replay Interface retrieved successfully. Address: 0x%llX"), (unsigned long long)GameReplayInterface);
				Debug(buffer, LOG_SUCCESSFUL);

				Debug(XorString("Game Version successfully found: ") + Game.Version, LOG_SUCCESSFUL);
				findVersion = true;
				break;
			}
		}
	}
	return true;
}

bool AttachToGameProcess() {
	Debug(XorString("Searching for the game window..."), LOG_INFO);
	Game.hWnd = FindWindow(Game.lpClassName, NULL);
	if (Game.hWnd) {
		Debug(XorString("Game window successfully found."), LOG_SUCCESSFUL);

		if (!GetWindowThreadProcessId(Game.hWnd, &Game.pID)) {
			Debug(XorString("Failed to retrieve Process ID from the window handle."), LOG_ERROR);
			return false;
		}

		Game.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Game.pID);
		if (Game.hProcess == NULL) {
			Debug(XorString("Failed to open game process. Error code: ") + std::to_string(GetLastError()), LOG_ERROR);
			Game.hWnd = NULL;
			return false;
		}

		Debug(XorString("Game process successfully located. Process ID: ") + std::to_string(Game.pID), LOG_SUCCESSFUL);
		return true;
	}
}

bool FetchGamePathAndVersion() {
	if (Game.Path.empty()) {
		Debug(XorString("Game Path is empty. Attempting to retrieve module base name..."), LOG_INFO);
		char gamePath[MAX_PATH] = { 0 };
		if (GetModuleFileNameExA(Game.hProcess, NULL, gamePath, MAX_PATH) > 0) {
			Game.Path = gamePath;
			Debug(XorString("Game Path successfully retrieved."), LOG_SUCCESSFUL);
		}
		else {
			Debug(XorString("Failed to retrieve game path. Error code: ") + std::to_string(GetLastError()), LOG_ERROR);
		}
	}

	if (Game.Version.empty()) {
		Debug(XorString("Game version is empty. Attempting to read from configuration file..."), LOG_INFO);
		size_t pos = Game.Path.find(XorString("data\\cache\\subprocess"));
		if (pos != std::string::npos) {
			std::string basePath = Game.Path.substr(0, pos);
			std::string iniPath = basePath + XorString("CitizenFX.ini");

			char gameVersion[32] = { 0 };
			if (GetPrivateProfileString(XorString("Game"), XorString("SavedBuildNumber"), "", gameVersion, sizeof(gameVersion), iniPath.c_str()) > 0) {
				Game.Version = gameVersion;
				Debug(XorString("Game version successfully found: ") + Game.Version, LOG_SUCCESSFUL);
			}
			else {
				Game.Version = XorString("3258");
				Debug(XorString("Failed to read game version from configuration file, defaulting to 3258"), LOG_WARNING);
			}
		}
		else {
			Debug(XorString("Expected substring 'data\\cache\\subprocess' not found in path."), LOG_WARNING);
		}
	}

	if (!Game.Path.empty() && !Game.Version.empty()) {
		return true;
	}
	return false;
}

float GetDistance(Vector3 value1, Vector3 value2) {
	float num = value1.x - value2.x;
	float num2 = value1.y - value2.y;
	float num3 = value1.z - value2.z;
	return sqrt(num * num + num2 * num2 + num3 * num3);
}

Vector3 Vec3Transform(Vector3* vIn, Matrix* mIn) {
	Vector3 vOut{};
	vOut.x = vIn->x * mIn->_11 + vIn->y * mIn->_21 + vIn->z * mIn->_31 + 1.f * mIn->_41;
	vOut.y = vIn->x * mIn->_12 + vIn->y * mIn->_22 + vIn->z * mIn->_32 + 1.f * mIn->_42;
	vOut.z = vIn->x * mIn->_13 + vIn->y * mIn->_23 + vIn->z * mIn->_33 + 1.f * mIn->_43;
	return vOut;
};

bool Vec3Empty(const Vector3& value) {
	return value == Vector3(0.f, 0.f, 0.f);
}

Vector3 Normalize(const Vector3& v) {
	float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length == 0.0f) return { 0,0,0 };
	return { v.x / length, v.y / length, v.z / length };
}

std::string GetPedName(int pedId) {
    std::string pedName = XorString("Player");
    if (!Offsets.CitizenPlayernamesBase) {
        return pedName;
    }

    uint64_t playerNames = ReadMemory<uint64_t>(Offsets.CitizenPlayernamesBase + 0x30D38);
    if (!playerNames) {
        return pedName;
    }

    uintptr_t list = ReadMemory<uintptr_t>(playerNames + 0x8);
    for (uint32_t i = 0; i < maxPlayerCount; ++i) {
        if (!list) {
            break;
        }

        if (pedId == ReadMemory<int>(list + 0x10)) {
            pedName = ReadString(list + 0x18);
            break;
        }
        list = ReadMemory<uintptr_t>(list + 0x8);
    }
    return pedName;
}

struct Bones {
	Vector3 Head;
	char Padding0[0x4]{};
	Vector3 LeftFoot;
	char Padding1[0x4]{};
	Vector3 RightFoot;
	char Padding2[0x4]{};
	Vector3 LeftAnkle;
	char Padding3[0x4]{};
	Vector3 RightAnkle;
	char Padding4[0x4]{};
	Vector3 LeftHand;
	char Padding5[0x4]{};
	Vector3 RightHand;
	char Padding6[0x4]{};
	Vector3 Neck;
	char Padding7[0x4]{};
	Vector3 Hip;
};

enum Bone : int {
	Head,
	LeftFoot,
	RightFoot,
	LeftAnkle,
	RightAnkle,
	LeftHand,
	RightHand,
	Neck,
	Hip
};

enum VisibilityFlags : BYTE {
	InvisibleFlag1 = 36,
	InvisibleFlag2 = 0,
	InvisibleFlag3 = 4,
	IntersectMissionEntityAndTrain = 2,
	IntersectPeds1 = 4,
	IntersectVehicles = 10,
	IntersectVegetation = 256,
	FrustumCulling = 512,
	OcclusionCulling = 1024,
	DistanceCulling = 2048
};

class Ped {
public:
	uintptr_t Pointer;
	uintptr_t PlayerInfo;
	uintptr_t WeaponManager;
	Vector3 Position;
	Matrix BoneMatrix;
	Vector3 BoneList[9]{};
	bool IsFriend = false;

	bool GetPlayer(uintptr_t& Base) {
		Pointer = Base;
		return Pointer != NULL;
	}

	bool Update() {
		PlayerInfo = ReadMemory<uintptr_t>(Pointer + Offsets.PlayerInfo);
		Position = ReadMemory<Vector3>(Pointer + 0x90);
		BoneMatrix = ReadMemory<Matrix>(Pointer + 0x60);
		UpdateBones();
		return true;
	}

	void UpdateBones() {
		Bones BonesData = ReadMemory<Bones>(Pointer + Offsets.BoneList);
		BoneList[Head] = Vec3Transform(&BonesData.Head, &BoneMatrix);
		BoneList[LeftFoot] = Vec3Transform(&BonesData.LeftFoot, &BoneMatrix);
		BoneList[RightFoot] = Vec3Transform(&BonesData.RightFoot, &BoneMatrix);
		BoneList[LeftAnkle] = Vec3Transform(&BonesData.LeftAnkle, &BoneMatrix);
		BoneList[RightAnkle] = Vec3Transform(&BonesData.RightAnkle, &BoneMatrix);
		BoneList[LeftHand] = Vec3Transform(&BonesData.LeftHand, &BoneMatrix);
		BoneList[RightHand] = Vec3Transform(&BonesData.RightHand, &BoneMatrix);
		BoneList[Neck] = Vec3Transform(&BonesData.Neck, &BoneMatrix);
		BoneList[Hip] = Vec3Transform(&BonesData.Hip, &BoneMatrix);
	}

	int GetId() {
		return ReadMemory<int>(ReadMemory<uint64_t>(Pointer + Offsets.PlayerInfo) + Offsets.Id);
	}

	std::string GetName() {
		return GetPedName(this->GetId());
	}

	std::string GetWeaponName() {
		uintptr_t WeaponManager = ReadMemory<uintptr_t>(Pointer + Offsets.WeaponManager);
		uintptr_t Step1 = ReadMemory<uintptr_t>(WeaponManager + 0x20);
		return ReadString(ReadMemory<uintptr_t>(Step1 + 0x5F0));
	}

	int GetHealth() {
		return ReadMemory<float>(Pointer + Offsets.Health);
	}

	int GetArmor() {
		return ReadMemory<float>(Pointer + Offsets.Armor);
	}

	bool IsPlayer() {
		if (GetId() <= 0) {
			return false;
		}
		return PlayerInfo != NULL;
	}

	bool IsDead() {
		return Position == Vector3(0.f, 0.f, 0.f);
	}

	bool IsVisible() {
		BYTE VisibilityFlag = ReadMemory<BYTE>(Pointer + Offsets.VisibleFlag);
		if (VisibilityFlag == InvisibleFlag1 ||
			VisibilityFlag == InvisibleFlag2 ||
			VisibilityFlag == InvisibleFlag3 ||
			VisibilityFlag == IntersectMissionEntityAndTrain ||
			VisibilityFlag == IntersectPeds1 ||
			VisibilityFlag == IntersectVehicles ||
			VisibilityFlag == IntersectVegetation ||
			(VisibilityFlag & FrustumCulling) ||
			(VisibilityFlag & OcclusionCulling) ||
			(VisibilityFlag & DistanceCulling))
		{
			return false;
		}

		return true;
	}
};

struct Friend {
	int id;
};

std::vector<Friend> friendList;

bool IsFriend(int searchId) {
	try {
		if (searchId < 0) {
			return false;
		}

		if (friendList.empty()) {
			return false;
		}

		for (const Friend& f : friendList) {
			if (f.id == searchId) {
				return true;
			}
		}
		return false;
	}
	catch (...) {
		return false;
	}
}

void RemoveFriend(int removeId) {
	try {
		if (removeId < 0) {
			return;
		}

		if (friendList.empty()) {
			return;
		}

		friendList.erase(std::remove_if(friendList.begin(), friendList.end(), [removeId](const Friend& f) { return f.id == removeId; }), friendList.end());
	}
	catch (...) {}
}

struct Vehicle {
	uintptr_t Pointer;
	Vector2 Location;
	float Health;
	float Distance;
};


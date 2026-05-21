template <class t>
t ReadMemory(uintptr_t address) {
	t read;
	ReadProcessMemory(Game.hProcess, (LPVOID)address, &read, sizeof(t), NULL);
	return read;
}

template <class T>
void WriteMemory(uintptr_t address, T value) {
	WriteProcessMemory(Game.hProcess, (LPVOID)address, &value, sizeof(T), NULL);
}

void WriteBytes(uintptr_t address, uint8_t* patch, size_t size) {
	SIZE_T bytesWritten;
	WriteProcessMemory(Game.hProcess, reinterpret_cast<LPVOID>(address), patch, size, &bytesWritten);
}

std::string ReadString(uintptr_t Addr) {
	const int bufferSize = 256;
	char buffer[bufferSize];
	int bytesRead = 0;
	bool success = true;

	while (bytesRead < bufferSize) {
		char character;
		ReadProcessMemory(Game.hProcess, (LPVOID)(Addr + bytesRead), &character, sizeof(char), NULL);
		buffer[bytesRead] = character;
		if (character == '\0') { break; }
		bytesRead++;
	}

	if (bytesRead == bufferSize) {
		success = false;
	}

	if (!success) {
		return "";
	}
	return std::string(buffer);
}

uintptr_t GetBaseAddress() {
	if (Game.pID == 0) {
		return 0;
	}

	HANDLE moduleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Game.pID);
	if (moduleSnapshot == INVALID_HANDLE_VALUE) {
		return 0;
	}

	MODULEENTRY32 moduleEntry;
	moduleEntry.dwSize = sizeof(moduleEntry);
	if (!Module32First(moduleSnapshot, &moduleEntry)) {
		CloseHandle(moduleSnapshot);
		return 0;
	}

	do {
		std::string currentModuleName(moduleEntry.szModule);
		if (currentModuleName.compare(mjLib::Process::GetName(Game.pID)) == 0) {
			uintptr_t baseAddress = (uintptr_t)moduleEntry.modBaseAddr;
			CloseHandle(moduleSnapshot);
			return baseAddress;
		}
	} while (Module32Next(moduleSnapshot, &moduleEntry));
	CloseHandle(moduleSnapshot);
	return 0;
}

uintptr_t GetBaseAddress(const std::string& moduleName) {
	if (Game.pID == 0) {
		return 0;
	}

	HANDLE moduleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Game.pID);
	if (moduleSnapshot == INVALID_HANDLE_VALUE) {
		return 0;
	}

	MODULEENTRY32 moduleEntry;
	moduleEntry.dwSize = sizeof(moduleEntry);
	if (!Module32First(moduleSnapshot, &moduleEntry)) {
		CloseHandle(moduleSnapshot);
		return 0;
	}

	do {
		std::string currentModuleName(moduleEntry.szModule);
		if (currentModuleName.compare(moduleName) == 0) {
			uintptr_t baseAddress = (uintptr_t)moduleEntry.modBaseAddr;
			CloseHandle(moduleSnapshot);
			return baseAddress;
		}
	} while (Module32Next(moduleSnapshot, &moduleEntry));
	CloseHandle(moduleSnapshot);
	return 0;
}
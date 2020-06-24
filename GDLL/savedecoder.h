#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace Decoder {
	std::string Values[26] = { "graphic_detail", "sfx_vol", "defaultScrollProgress", "music_vol", "swearFilter", "fullscreen", "pass_update", "tankid_password_chk2", "legal_progress", "name", "lastworld", "tankid_checkbox", "tankid_name", "tankid_password", "enter", "defaultInventoryHeight", "defaultLogHeight", "Client", "meta", "rid", "touch", "rememberZoom", "sendSkinColor", "zoomSave", "addJump", "skinColor" };
	std::string pTypes[6] = { "checkbox", "slider", "edit", "color", "password", "unknown" };
	int pType[26] = { 5, 5, 5, 5/*1, 1, 5, 1*/, 5, 0, 5, 5, 5, 2, 2, 0, 2, 4, 0, 5, 5, 5, 2, 2, 0, 0, 5, 5, 0, 5 };
	int defValues[6] = { 1, 4, 16, 64, 256, 1024 };
	std::string pChars = "";
	int pSize = 0;
	std::vector<int> Positions;
	std::vector<int> PositionLength;
	bool useFilter = true;

	std::vector<std::pair<std::string, std::vector<std::string>>> Exec(std::wstring path);
	bool openFile(std::wstring path);
	bool ValidateChar(char t);
	bool ValidateString(std::string text);
	int GetIntFromSliderBytes(int bytesOffset);
	std::vector<std::pair<std::string, std::vector<std::string>>> DecodeFile();
	std::vector<std::string> ListTrigger(int value);
	std::string toLowercase(std::string text);
	int customIndexOf(std::vector<std::string> arr, std::string regex);
	std::vector<std::string> decodePassword(std::string password, bool file);

	std::string fillString(int size) {
		std::string buf = "";
		for (int i = 0; i < size; i++) buf += " ";
		return buf;
	}

	std::string strcpy(std::string from, int size) {
		std::string buf = fillString(size);
		for (int i = 0; i < size; i++) buf[i] = from[i];
		return buf;
	}

	std::string makeSomeMagicToMakeItVisible(std::string text) {
		for (int i = 0; i < (int)text.length(); i++) if ((int)text[i] < 65 || (int)text[i] > 122) text[i] = '_';
	}

	std::vector<std::pair<std::string, std::vector<std::string>>> Exec(std::wstring path) {
		if (!openFile(path) || !pSize) return { { "Error", { "An error occurred while reading save file!" } } };
		return DecodeFile();
	}

	bool openFile(std::wstring path) {
		if (!path.length()) return false;
		std::ifstream ifs(path, std::ios::binary | std::ios::ate);
		if (!ifs.good()) return false;
		std::streampos size = ifs.tellg();
		char * p = new char[(int)size];
		ifs.seekg(0, std::ios::beg);
		ifs.read(p, size);
		ifs.close();
		pSize = (int)size;
		pChars = fillString(pSize);
		for (int i = 0; i < pSize; i++) pChars[i] = p[i];
		delete p;
		return true;
	}

	bool ValidateChar(char t) {
		if (!useFilter) return true;
		if (t >= 0x40 && t <= 0x5A || t >= 0x61 && t <= 0x7A || t >= 0x30 && t <= 0x39 || t >= 0x2B && t <= 0x2E || t >= 0x2B && t <= 0x2E) return true;
		return false;
	}
		
	bool ValidateString(std::string text) {
		for (unsigned int i = 0; i < text.length(); i++) if (!ValidateChar(text[i])) return false; return true;
	}

	/*int GetIntFromSliderBytes(int bytesOffset) {
		int buffer = 0;
		if (bytesOffset + 3 < pSize && pChars[bytesOffset + 3] >= 0x3A) {
			int offset1 = pChars[bytesOffset + 3] - 0x3A;
			assert(offset1 < 6);
			buffer = defValues[offset1];
			buffer -= (((128 - pChars[bytesOffset + 2]) * 4) / (1024 / defValues[offset1]));
		}
		else return 1024;
		return buffer;
	}*/

	std::vector<std::pair<std::string, std::vector<std::string>>> DecodeFile() {
		if (!pChars.length() || pChars.find("tankid_password") == std::string::npos) return { { "Error", { "An error occurred while searching for tankid_password" } } };
		Positions.clear();
		PositionLength.clear();
		for (int i = 0; i < 26; i++) {
			std::size_t start = pChars.find(Values[i]);
			if (start != std::string::npos && Values[i] == "tankid_password" && pChars[start + Values[i].length()] == '_') {
				std::size_t buf = pChars.substr(start + 1).find(Values[i]);
				if(buf != std::string::npos) start += buf + 1;
				else start = std::string::npos;
			}
			Positions.push_back(start);
		}
		for (int i = 0; i < 26; i++) {
			if (Positions[i] != std::string::npos) {
				int PosEndBuffer = pChars.length();
				for (int j = 0; j < 26; j++) if (Positions[j] > Positions[i] && Positions[j] < PosEndBuffer) PosEndBuffer = Positions[j];
				PositionLength.push_back(PosEndBuffer);
			}
			else PositionLength.push_back(0);
		}

		for (int i = 0; i < 26; i++) if (Positions[i] != std::string::npos) { Positions[i] += Values[i].length(); PositionLength[i] -= Positions[i]; }
		std::vector<std::pair<std::string, std::vector<std::string>>> content;
		
		/*for (int i = 0; i < 26; i++) {
			content.push_back({ std::to_string(i), { std::to_string(Positions[i]) + ":" + std::to_string(PositionLength[i]) } });
		}*/
		
		for (int i = 0; i < 26; i++) 
			if (Positions[i] != std::string::npos && pType[i] != 5) 
				content.push_back({ Values[i], ListTrigger(i) });
		return content;
	};

	std::vector<std::string> ListTrigger(int value) {
		if (value >= 26) return { "Value pointer overflow" };
		switch (pType[value]) {
			case 0: 
				if (pChars[Positions[value]] == 0x01) return { "true" }; else return { "false" };
			case 1: {
				return { "unknown type id!" };
				//int sliderValue = GetIntFromSliderBytes(Positions[value]);
				//return { std::to_string(int(double(sliderValue) / 10.24)) + "%" };
			}
			case 2: {
				int stringLength = (int)pChars[Positions[value]];
				return { pChars.substr(Positions[value] + 4, stringLength) };
			}
			case 4: {
				int stringLength = pChars[Positions[value]];
				std::string passwordBuffer = pChars.substr(Positions[value] + 4, stringLength);
				return decodePassword(passwordBuffer, 1);
			} default: return { "unknown type id!" };
		}
	}

	std::string toLowercase(std::string text) {
		for (int i = 0; i < (int)text.length(); i++) text[i] = tolower(text[i]);
		return text;
	}

	int customIndexOf(std::vector<std::string> arr, std::string regex) {
		for (unsigned int i = 0; i < arr.size(); i++) if (toLowercase(arr[i]) == toLowercase(regex)) return i; return -1;
	}

	std::vector<std::string> decodePassword(std::string password, bool file) {
		std::vector<std::string> result;
		std::string buffer = "";
		char cbuffer = 0;
		int pbuffer = -1;
		for (int offset = -128; offset < 128; offset++) {
			buffer = "";
			for (int i = 0; i < (int)password.length(); i++) {
				if (!ValidateChar( (cbuffer = ((int)password[i] + offset - (file ? i : 0) % 255)))) break;
				buffer += cbuffer;
			}
			if (buffer.length() < password.length()) continue;
			if (!useFilter || (pbuffer = customIndexOf(result, buffer)) == -1) result.push_back(buffer);
			else if (useFilter && pbuffer != -1 && toLowercase(result[pbuffer]) == buffer) result[pbuffer] = buffer;
		} 
		return result;
	}
}
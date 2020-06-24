#pragma once
#ifndef SAFETY__H_
#define SAFETY__H_

#include <string>
#include <windows.h>
#include <vector>
#include <locale>
#include <codecvt>
#include <intrin.h>      

namespace Safety {
	std::string InsertKey(std::string text, std::string Key, int seed);
	std::string RemoveKey(std::string text, std::string Key, int seed);
	std::string toBits(std::string text);
	std::string bitsToHex4(std::string bits, int set_index);

	std::string VI2S(int v) {
		std::string b = "";
		for (int i = 100; i <= 1000000; i*= 100) {
			b = std::to_string(i == 100 ? (v%i) : (((v%i) - (v % (i / 100))) / (i / 100))) + "." + b;
		}
		return b.length() ? b.substr(0, b.length() - 1) : "";
	}

	/*int VS2I(std::string v) {
		
	}*/

	bool testString(std::string ctx) {
		if (ctx == "   ") return true;
		for (unsigned int i = 0; i < ctx.length(); i++) {
			if (ctx[i] >= 34 && ctx[i] <= 39) return false;
			if (ctx[i] >= 91 && ctx[i] <= 94 || ctx[i] == 96) return false;
			if (ctx[i] >= 33 && ctx[i] <= 122) continue;
			return false;
		}
		return true;
	}

	bool testDir(std::wstring path) {
		std::ifstream ifs(path);
		if (!ifs.good()) return false;
		ifs.close();
		return true;
	}
	
	unsigned getVolumeHash()
	{
		DWORD serialNum = 0;
  
		GetVolumeInformation(L"c:\\", NULL, 0, &serialNum, NULL, NULL, NULL, NULL);
		unsigned hash = (unsigned)((serialNum + (serialNum >> 16)) & 0xFFFF);

		return hash;
	}

	DWORD getVolumeSerial(std::wstring vol = L"c:\\") {
		DWORD sn = 0;
		GetVolumeInformation(vol.c_str(), NULL, NULL, &sn, NULL, NULL, NULL, NULL);
		return sn;
	}

	std::string getUniqueKey() {
		long long buffer = 0;
		std::wstring drive = L"c://";
		int z = 0;
		for (int i = 0; i < 15; i++) {
			z = Safety::getVolumeSerial(drive);
			if (z != 0) buffer += z + i * 15474;
			else break;
			drive[0]++;
		}
		return md5(bitsToHex4(toBits(std::to_string(buffer) + std::to_string(getVolumeHash())), 1));
	}

	std::wstring s2ws(const std::string& str) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;
		return converterX.from_bytes(str);
	}

	std::string ws2s(const std::wstring& wstr) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;
		return converterX.to_bytes(wstr);
	}

	bool isValidHex4(std::string text, int set_index) {
		std::string set[2] = { "1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM-_", "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_" };
		for (unsigned int i = 0; i < text.length(); i++) if (set[set_index].find(text[i]) == std::string::npos) return false;
		return true;
	}

	std::string toBits(std::string text) {
		std::string buffer = "";
		for (unsigned int i = 0; i < text.length(); i++) {
			for (int j = 7; j >= 0; j--) {
				buffer += (text[i] & (1 << j) ? '1' : '0');
			}
		}
		return buffer;
	}

	std::string Hex4ToBits(std::string text, int set_index) {
		std::string set[3] = { "1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM-_", "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_", "0123456789abcdef" };
		std::string buffer = "";
		std::size_t pos;
		for (unsigned int i = 0; i < text.length(); i++) {
			if ((pos = set[set_index].find(text[i])) == std::string::npos) continue;
			for (int j = 5; j >= 0; j--) {
				buffer += (pos & (1 << j) ? '1' : '0');
			}
		}
		return buffer;
	}

	std::string bitsToANSII(std::string bits) {
		std::string buffer = "";
		int index = 0;
		int size = bits.length();
		while (index <= size - 7) {
			int c = 0;
			for (int i = 0; i < 8 && index + i < size; i++) {
				if (bits[i + index] == '1') c += 1 << (7 - i);
			}
			buffer += char(c);
			index += 8;
		}
		return buffer;
	}

	std::string bitsToHex4(std::string bits, int set_index = 0) {
		std::string set[3] = { "1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM-_", "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_", "0123456789abcdef" };
		std::string buffer = "";
		while (bits.length() % 6 != 0) bits += '0';
		int size = bits.length();
		int index = 0;
		while (index < size) {
			unsigned char c = 0;
			for (int i = 0; i < 6; i++) {
				if (bits[i + index] == '1') c += (1 << (5 - i));
			}
			buffer += set[set_index][c%set[set_index].length()];
			index += 6;
		}
		return buffer;
	}

	std::string toDec(char numb) {
		std::string buffer = "";
		std::string buf = ""; buf += (int)numb;
		int buflength = buf.length();
		while (buflength < 3) { buffer += '0'; buflength++; }
		buffer += (int)numb;
		return buffer;
	}

	std::string CutKey(std::string Key, int width, int offset = 0, int offsetMultiplier = 1) {
		if (offsetMultiplier == 0) offsetMultiplier = 1;
		std::string buffer = "";
		for (unsigned int i = 0; i < Key.length(); i++) {
			buffer += char((Key[i] + (offset * offsetMultiplier)) % width);
		}
		return buffer;
	}

	std::string InsertKeyX(std::string text, std::string Key, int seed = 0)
	{
		std::string buffer = text;
		std::string bufkey = Key;
		for (unsigned int i = 0; i < buffer.length(); i++) {
			if (i % 2 == 0)
				buffer[i] = unsigned char((buffer[i] + bufkey[i%bufkey.length()]) % 256);
			else buffer[i] = unsigned char((buffer[i] - bufkey[i%bufkey.length()]) % 256);
		}
		bufkey = CutKey(bufkey, 128, seed *(seed + 1), seed);
		for (unsigned int i = 0; i < buffer.length(); i++) {
			buffer[i] = unsigned char((buffer[i] + bufkey[i%bufkey.length()]) % 256);
		}
		bufkey = CutKey(bufkey, 64, seed + 1, seed * seed);
		for (unsigned int i = 0; i < buffer.length(); i++) {
			buffer[i] = unsigned char((buffer[i] + bufkey[i%bufkey.length()]) % 256);
		}
		bufkey = CutKey(bufkey, 32, seed + 1, seed * seed + 62);
		for (unsigned int i = 0; i < buffer.length(); i++) {
			buffer[i] = unsigned char((buffer[i] + bufkey[i%bufkey.length()]) % 256);
		}
		return buffer;
	}

	std::string RemoveKeyX(std::string text, std::string Key, int seed = 0)
	{
		std::string Key_1 = Key;
		std::string Key_2 = CutKey(Key_1, 128, seed *(seed + 1), seed);
		std::string Key_3 = CutKey(Key_2, 64, seed + 1, seed * seed);
		std::string Key_4 = CutKey(Key_3, 32, seed + 1, seed * seed + 62);
		std::string buffer = text;

		for (unsigned int i = 0; i < buffer.length(); i++)
		{
			buffer[i] = unsigned char((buffer[i] - Key_4[i%Key_4.length()])%256);
		}
		for (unsigned int i = 0; i < buffer.length(); i++) {
			buffer[i] = unsigned char((buffer[i] - Key_3[i%Key_3.length()]) % 256);
		}
		for (unsigned int i = 0; i < buffer.length(); i++) {
			buffer[i] = unsigned char((buffer[i] - Key_2[i%Key_2.length()]) % 256);
		}
		for (unsigned int i = 0; i < buffer.length(); i++) {
			if (i % 2 == 0)
				buffer[i] = unsigned char((buffer[i] - Key_1[i%Key_1.length()]) % 256);
			else buffer[i] = unsigned char((buffer[i] + Key_1[i%Key_1.length()]) % 256);
		}
		return buffer;
	}

	std::string InsertKey(std::string text, std::string Key, int seed) {
		text = InsertKeyX(text, Key, seed);
		std::string buffer = toBits(text);
		buffer = bitsToHex4(buffer, 0);
		return buffer;
	}

	std::string RemoveKey(std::string text, std::string Key, int seed = 0) {
		text = Hex4ToBits(text, 0);
		text = bitsToANSII(text);
		return RemoveKeyX(text, Key, seed);
	}

	std::string RandomKey(int length, int table = 0)
	{
		std::string buffer = "";
		static const char alphanum0[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		static const char alphanum1[] =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		static const char alphanum2[] =
			"0123456789";
		static const char alphanum3[] =
			"0123456789abdef";

		switch (table) {
			case 1: for (int i = 0; i < length; i++) buffer += alphanum1[rand() % (sizeof(alphanum1) - 1)]; break;
			case 2: for (int i = 0; i < length; i++) buffer += alphanum2[rand() % (sizeof(alphanum2) - 1)]; break;
			case 3: for (int i = 0; i < length; i++) buffer += alphanum3[rand() % (sizeof(alphanum3) - 1)]; break;
			default: for (int i = 0; i < length; i++) buffer += alphanum0[rand() % (sizeof(alphanum0) - 1)]; break;
		}
		return buffer;
	}

	bool IsUserAdmin() {
		struct Data {
			PACL pACL;
			PSID psidAdmin;
			HANDLE hToken;
			HANDLE hImpersonationToken;
			PSECURITY_DESCRIPTOR psdAdmin;
			Data() : pACL(NULL), psidAdmin(NULL), hToken(NULL), hImpersonationToken(NULL), psdAdmin(NULL) {}
			~Data() {
				if (pACL) LocalFree(pACL);
				if (psdAdmin) LocalFree(psdAdmin);
				if (psidAdmin) FreeSid(psidAdmin);
				if (hImpersonationToken) CloseHandle(hImpersonationToken);
				if (hToken) CloseHandle(hToken);
			}
		} data;

		BOOL fReturn = FALSE;
		DWORD dwStatus;
		DWORD dwAccessMask;
		DWORD dwAccessDesired;
		DWORD dwACLSize;
		DWORD dwStructureSize = sizeof(PRIVILEGE_SET);

		PRIVILEGE_SET ps;
		GENERIC_MAPPING GenericMapping;
		SID_IDENTIFIER_AUTHORITY SystemSidAuthority = SECURITY_NT_AUTHORITY;

		const DWORD ACCESS_READ = 1;
		const DWORD ACCESS_WRITE = 2;

		if (!OpenThreadToken(GetCurrentThread(), TOKEN_DUPLICATE | TOKEN_QUERY, TRUE, &data.hToken)) {
			if (GetLastError() != ERROR_NO_TOKEN) return false;
			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_DUPLICATE | TOKEN_QUERY, &data.hToken)) return false;
		}

		if (!DuplicateToken(data.hToken, SecurityImpersonation, &data.hImpersonationToken)) return false;
		if (!AllocateAndInitializeSid(&SystemSidAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &data.psidAdmin)) return false;

		data.psdAdmin = LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		if (data.psdAdmin == NULL) return false;

		if (!InitializeSecurityDescriptor(data.psdAdmin, SECURITY_DESCRIPTOR_REVISION)) return false;

		// Compute size needed for the ACL.
		dwACLSize = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) + GetLengthSid(data.psidAdmin) - sizeof(DWORD);

		data.pACL = (PACL)LocalAlloc(LPTR, dwACLSize);
		if (data.pACL == NULL) return false;

		if (!InitializeAcl(data.pACL, dwACLSize, ACL_REVISION2)) return false;

		dwAccessMask = ACCESS_READ | ACCESS_WRITE;

		if (!AddAccessAllowedAce(data.pACL, ACL_REVISION2, dwAccessMask, data.psidAdmin)) return false;

		if (!SetSecurityDescriptorDacl(data.psdAdmin, TRUE, data.pACL, FALSE)) return false;

		// AccessCheck validates a security descriptor somewhat; set the group
		// and owner so that enough of the security descriptor is filled out 
		// to make AccessCheck happy.

		SetSecurityDescriptorGroup(data.psdAdmin, data.psidAdmin, FALSE);
		SetSecurityDescriptorOwner(data.psdAdmin, data.psidAdmin, FALSE);

		if (!IsValidSecurityDescriptor(data.psdAdmin)) return false;

		dwAccessDesired = ACCESS_READ;

		GenericMapping.GenericRead = ACCESS_READ;
		GenericMapping.GenericWrite = ACCESS_WRITE;
		GenericMapping.GenericExecute = 0;
		GenericMapping.GenericAll = ACCESS_READ | ACCESS_WRITE;

		if (!AccessCheck(data.psdAdmin, data.hImpersonationToken, dwAccessDesired, &GenericMapping, &ps, &dwStructureSize, &dwStatus, &fReturn)) return false;
		
		return fReturn;
	}

	int filesize(wchar_t* path) {
		std::wifstream wifs(path, std::ifstream::ate | std::ifstream::binary);
		unsigned size = wifs.tellg();
		wifs.close();
		return size;
	}

	int GetSelfSize() {
		wchar_t dir[MAX_PATH];
		GetModuleFileName(NULL, dir, MAX_PATH);
		return sizeof(dir) ? filesize(dir) : 0;
	}
}

#endif
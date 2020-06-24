#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iomanip>

void cout(std::wstring ctx);
void cout(std::string ctx);

std::string url_encode(const std::string &value) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
		std::string::value_type c = (*i);

		// Keep alphanumeric and other accepted characters intact
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
			continue;
		}

		// Any other characters are percent-encoded
		escaped << std::uppercase;
		escaped << '%' << std::setw(2) << int((unsigned char)c);
		escaped << std::nouppercase;
	}

	return escaped.str();
}

std::string urlDecode(std::string &SRC) {
	std::string ret;
	char ch;
	int i, ii;
	for (i = 0; i < (int)SRC.length(); i++) {
		if (int(SRC[i]) == 37) {
			sscanf_s(SRC.substr(i + 1, 2).c_str(), "%x", &ii);
			ch = static_cast<char>(ii);
			ret += ch;
			i = i + 2;
		}
		else {
			ret += SRC[i];
		}
	}
	return (ret);
}

std::unordered_map<std::string, std::string> WebDataToMap(std::string data) {
	std::unordered_map<std::string, std::string> result;
	std::string pair[2] = { "", "" };
	int ptr = 0;
	std::string::size_type end = data.find("||");
	if (end == std::string::npos) end = data.length();

	for (int i = 0; i <= (int)end; i++) {
		if (i == end || data[i] == '|' || data[i] == '\r' || data[i] == '\n') {
			if (pair[0].length() && ptr == 1) {
				result.insert({ pair[0], pair[1] });
			}
			pair[0] = "", pair[1] = "";
			ptr = 0;
		}
		else if (data[i] == ':') {
			ptr = 1;
		}
		else {
			pair[ptr] += data[i];
		}
	}
	return result;
	//return { {"KEYA", "test"} };
}

std::string WebCutHeader(std::string data) {
	std::string::size_type pos = data.find("\r\n\r\n");
	return (pos != std::string::npos ? data.substr(pos + 4) : data);
}

std::string WebGet(std::string weburl, std::string content = "", std::string suburl = "/", std::string port = "80") {
	std::string inbufA = "POST " + suburl + " HTTP/1.1\r\n";
	inbufA += "Host: " + weburl + "\r\n";
	inbufA += "Connection: close\r\n";
	inbufA += "Content-type: application/x-www-form-urlencoded\r\n";
	inbufA += "Content-length: " + std::to_string(content.length()) + "\r\n\r\n";
	inbufA += content;

	//MessageBoxA(NULL, url_encode(content).c_str(), "test", MB_OK);

	WSADATA wsaData;
	SOCKET sock = INVALID_SOCKET;
	addrinfo * result, *ptr, hints;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout(L"WSAStartup failed!");
		return "";
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(weburl.c_str(), port.c_str(), &hints, &result) != 0) {
		WSACleanup();
		cout(L"getaddrinfo failed!");
		return "";
	}

	const char *sendbuf = inbufA.c_str();
	char * recvbuf = new char[4096];
	int recvbuflen = 4096;

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			WSACleanup();
			cout(L"getaddrinfo failed!");
			return "";
		}

		if (connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
			closesocket(sock);
			sock = INVALID_SOCKET;
			cout(L"connect failed!");
			continue;
		}
		break;
	}

	if (sock == INVALID_SOCKET) {
		WSACleanup();
		cout(L"INVALID_SOCKET!");
		return "";
	}

	freeaddrinfo(result);

	int res = send(sock, sendbuf, (int)strlen(sendbuf), 0);
	if (res == SOCKET_ERROR) {
		closesocket(sock);
		WSACleanup();
		cout(L"send failed!");
		return "";
	}
	//std::cout << "Bytes sent: " << res << std::endl;

	std::string clearData = "";
	do {
		res = recv(sock, recvbuf, recvbuflen, 0);
		if (res > 0) {
			for (int i = 0; i < res; i++) clearData += recvbuf[i];
		}
		else if (res != 0) {
			cout(L"recv failed!");
			return "";
		}

	} while (res > 0);

	res = shutdown(sock, SD_SEND);
	if (res == SOCKET_ERROR) {
		closesocket(sock);
		WSACleanup();
		cout(L"shutdown failed!");
		return "";
	}

	closesocket(sock);
	WSACleanup();
	return WebCutHeader(clearData);
}

std::string KeyFromPage(std::string key, std::unordered_map<std::string, std::string> page) {
	auto passkey = page.find(key);
	return passkey == page.end() ? "" : passkey->second;
}

std::string KeyFromPage(std::string key, std::string ip, std::string subdir = "/", std::string port = "80") {
	std::string page = WebGet(ip, subdir, port);
	page = WebCutHeader(page);
	std::unordered_map<std::string, std::string> map = WebDataToMap(page);
	return KeyFromPage(key, map);
}
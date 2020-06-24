#pragma once
#include <Windows.h>
#include <cassert>
#include <chrono>
#include <fstream>
#include <string>
#include "handles.h"
#include "klog.h"

void cout(std::wstring);
void triggerWindowUpdate();

ULONG getMs() {
	return (ULONG)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

class WND {
public:
	WND(std::wstring filepath);
	~WND();

	void setTitle(std::wstring title);
	void bringToFront();
	void changeVisibility();

	void Kill();
	void Suspend();
	void Resume();

	inline void setMB(bool state = true) { this->multiboxing = state; }

	bool isClosed();

	inline bool isSuspended() { return this->suspended; }
	inline bool isVisible() { return this->visible; }
	inline bool isMB() { return this->multiboxing; }
	inline bool mutexCaught() { return this->mutex_caught; }
	inline HWND getHWND() { return this->wnd; }

private:
	HWND wnd = NULL;

	ULONG created = getMs();
	bool suspended = false;
	bool mutex_caught = false;
	bool visible = true;
	bool multiboxing = false;

	STARTUPINFO SI = { sizeof(SI) };
	PROCESS_INFORMATION PI;
};

WND::WND(std::wstring filepath) {
	if (CreateProcess(filepath.c_str(), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &this->SI, &this->PI)) {
		int limit = 100;
		HWND HWNDResult = NULL;
		while (HWNDResult == NULL && limit > 0) {
			HWNDResult = EnumWindowsMyGt(this->PI.dwProcessId);
			limit++;
			Sleep(100);
		}
		if (HWNDResult != NULL) {
			this->wnd = HWNDResult;
		}
		else {
			this->Kill();
			this->wnd = NULL;
			return;
		}

		DWORD exit_code;
		GetExitCodeProcess(this->PI.hProcess, &exit_code);
		if (exit_code != 259) return;

		this->mutex_caught = false;
		if (EnumerateHandles(this->PI.dwProcessId, this->PI.hProcess) != 0) {
			this->Kill();
		}
		else this->mutex_caught = true;
	}
}

WND::~WND() {
	if (this->PI.hProcess != NULL) TerminateProcess(this->PI.hProcess, 0);
	CloseHandle(this->PI.hThread);
}

void WND::setTitle(std::wstring title) {
	SetWindowText(this->wnd, title.c_str());
}

void WND::bringToFront() {
	SetForegroundWindow(this->wnd);
}

void WND::changeVisibility() {
	ShowWindow(this->wnd, this->visible ? SW_HIDE : SW_SHOW);
	this->visible = !this->visible;
}

void WND::Kill() {
	TerminateProcess(this->PI.hProcess, 9);
	CloseHandle(this->PI.hThread);
	this->wnd = NULL;
	this->mutex_caught = false;
}

void WND::Suspend() {
	if (this->PI.hThread != NULL) {
		SuspendThread(this->PI.hThread);
		this->suspended = true;
	}
}

void WND::Resume() {
	if (this->PI.hThread != NULL) {
		ResumeThread(this->PI.hThread);
		this->suspended = false;
	}
}

bool WND::isClosed() {
	if (this->PI.hProcess == NULL) {
		if (this->PI.dwProcessId == NULL) {
			if (this->wnd == NULL) return true;
			GetWindowThreadProcessId(this->wnd, &this->PI.dwProcessId);
			if (this->PI.dwProcessId == NULL) return true;
		}
		HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, this->PI.dwProcessId);
		if (process == NULL) return true;
		DWORD ret = WaitForSingleObject(process, 0);
		CloseHandle(process);
		return ret != WAIT_TIMEOUT;
	}
	DWORD eCode = NULL;
	if(SUCCEEDED(GetExitCodeProcess(this->PI.hProcess, &eCode))) return eCode != STILL_ACTIVE;
	return true;
}

void ReceiveFnct(std::string data);

class WNDMGR {
public:
	WNDMGR(std::wstring path);
	~WNDMGR();

	void NewWindow();
	void CloseWindow(int id);
	void CloseAll();
	void bringToFront(int id);
	void changeVisibility(int id);

	inline void update() { this->updated = true; }
	inline bool isUpdated() { return this->updated; }
	inline bool isClosed() { return this->closed; }

	void eventLoop() {
		for (unsigned int i = 0; i < this->list.size(); i++) {
			if (this->list[i].second->isClosed()) {
				delete this->list[i].second;
				this->list.erase(this->list.begin() + i);
				i--;
				this->updated = true;
			}
		}
		if (updated) {
			updated = false;
			triggerWindowUpdate();
		}
	}

	bool GetPublicMutex() { return this->PUBLIC_MUTEX != NULL; }
	void SetPublicMutex(HANDLE new_mutex) { if (this->PUBLIC_MUTEX != NULL) CloseHandle(this->PUBLIC_MUTEX); this->PUBLIC_MUTEX = new_mutex; }
	inline HANDLE NewPublicMutex() { return CreateMutex(NULL, FALSE, L"Growtopia"); }
	
	bool wndFocused();
	int wndBelongsTo(HWND hWnd);
	int wndBelongsToMB(HWND hWnd);

	WND * getWindow(int id);
	int freeID();

	int length() { return this->list.size(); }

	std::vector<HWND> getMBTargets(HWND mainWnd = NULL);
	std::vector<HWND> getHWNDs();
	void setMB(bool set);
	void MBInit();
	void MBLoop();
	inline const bool isMBInitialised() { return this->mb_init; }
	POINT mb_lastpos = { 0, 0 };

	std::vector<std::pair<int, std::vector<std::string>>> toWndArray();

	std::string LoadSave(std::string filename = "GABB.ini");
	bool WriteSave(std::string ctx, std::string filename = "GABB.ini");
private:
	HINSTANCE hInstance = GetModuleHandle(NULL);
	bool updated = false, allowCreating = true, isUpdating = false;
	std::vector<std::pair<int, WND *>> list;
	std::wstring filepath = L"";
	HANDLE PUBLIC_MUTEX;
	bool closed = false;

	bool mb_init = false;
	HWND mb_wnd = NULL;
	const wchar_t * mb_className = L"MBWnd";

	UINT mb_lbtt = WM_LBUTTONUP, mb_mbtt = WM_MBUTTONUP, mb_rbtt = WM_RBUTTONUP;

	winpos mb_eventpos;
	int mb_init_count = 0;
};

WNDMGR::WNDMGR(std::wstring path) {
	this->filepath = path;
}

WNDMGR::~WNDMGR() {
	allowCreating = false;
	for (unsigned int i = 0; i < list.size(); i++) if(list[i].second != NULL) delete list[i].second;
}

void WNDMGR::NewWindow() {
	this->isUpdating = true;
	cout(L"Creating");
	//suspend all;
	for (unsigned int i = 0; i < list.size(); i++) if (!list[i].second->isSuspended()) list[i].second->Suspend();
	//delete mutex;
	this->SetPublicMutex(NULL);
	//add window
	WND * p = new WND(this->filepath);

	if (!p->mutexCaught()) {
		p->Kill();
		delete p;
		this->update();
		return;
	}
	this->SetPublicMutex(this->NewPublicMutex());
	this->list.push_back({ this->freeID(), p });
	for (unsigned int i = 0; i < list.size(); i++) if (list[i].second->isSuspended()) list[i].second->Resume();
	this->isUpdating = false;
	if (this->list.size()) this->list[this->list.size() - 1].second->setTitle(L"Growtopia [" + std::to_wstring(this->list[this->list.size() - 1].first) + L"]");
	this->update();
}

void WNDMGR::CloseWindow(int id) {
	for (unsigned int i = 0; i < this->list.size(); i++) {
		if (this->list[i].first == id) {
			delete this->list[i].second;
			this->list.erase(this->list.begin() + i);
			this->update();
			return;
		}
	}
}

void WNDMGR::CloseAll() {
	while (list.size()) {
		delete this->list[0].second;
		this->list.erase(this->list.begin());
	}
	this->update();
}

void WNDMGR::bringToFront(int id) {
	for (unsigned int i = 0; i < this->list.size(); i++) {
		if (this->list[i].first == id) {
			this->list[i].second->bringToFront();
			this->update();
			return;
		}
	}
}

void WNDMGR::changeVisibility(int id) {
	for (unsigned int i = 0; i < this->list.size(); i++) {
		if (this->list[i].first == id) {
			this->list[i].second->changeVisibility();
			this->update();
			return;
		}
	}
}

bool WNDMGR::wndFocused() {
	HWND ww = GetForegroundWindow();
	for (unsigned int i = 0; i < this->list.size(); i++) if (this->list[i].second->getHWND() == ww) return true; return false;
}

int WNDMGR::wndBelongsTo(HWND hWnd) {
	for (unsigned int i = 0; i < this->list.size(); i++) if (this->list[i].second->getHWND() == hWnd) return this->list[i].first; return -1;
}

int WNDMGR::wndBelongsToMB(HWND hWnd) {
	for (unsigned int i = 0; i < this->list.size(); i++) if (this->list[i].second->getHWND() == hWnd) {
			if(this->list[i].second->isMB()) return this->list[i].first;
			return -1;
		}
	return -1;
}

WND * WNDMGR::getWindow(int id) {
	for (unsigned int i = 0; i < this->list.size(); i++) if (this->list[i].first == id) return this->list[i].second; return NULL;
}

int WNDMGR::freeID() {
	srand((unsigned)time(NULL));
	int numb = 1;
	while (this->getWindow(numb) != NULL) numb += 1;
	return numb;
}

std::vector<HWND> WNDMGR::getMBTargets(HWND mainWnd) {
	std::vector<HWND> res;
	HWND buf;
	for (unsigned int i = 0; i < this->list.size(); i++) {
		if (this->list[i].second->isMB() && (buf = this->list[i].second->getHWND()) != mainWnd) res.push_back(buf);
	}
	return res;
}

std::vector<HWND> WNDMGR::getHWNDs() {
	std::vector<HWND> res;
	HWND buf;
	for (unsigned int i = 0; i < this->list.size(); i++) if ((buf = this->list[i].second->getHWND()) != NULL)res.push_back(buf);
	return res;
}

void WNDMGR::setMB(bool set) {
	for (unsigned int i = 0; i < this->list.size(); i++) if(this->list[i].second != NULL) this->list[i].second->setMB(set);
}

void WNDMGR::MBInit() {
	this->mb_init_count++;
	if (this->hInstance == NULL) return;
	WNDCLASSEX wc;
	//MSG msg;

	// register window class
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = this->hInstance;
	wc.lpszClassName = this->mb_className;
	wc.cbClsExtra = 1;

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// create message-only window
	this->mb_wnd = CreateWindowEx(0, mb_className, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, this->hInstance, &(this->mb_eventpos));
	if (!this->mb_wnd) {
		MessageBox(NULL, (L"Window Creation Failed!: " + std::to_wstring(GetLastError())).c_str(), L"Error!", MB_ICONEXCLAMATION | MB_OK);
		UnregisterClassW(this->mb_className, this->hInstance);
		return;
	}
	SetClassLongPtr(this->mb_wnd, 1, (int)&(this->mb_eventpos));

	//std::async(std::launch::async, MSGLoop, &msg);

	this->mb_init = true;
}

void WNDMGR::MBLoop() {
	if (!this->mb_init) {
		this->MBInit();
		if (this->mb_init_count > 10) {
			MessageBox(NULL, L"Failed to initialise multiboxing window!", L"Error!", MB_OK | MB_ICONEXCLAMATION);
			this->closed = true;
		}
		this->mb_eventpos.clear();
		return;
	}

	HWND forwnd = GetForegroundWindow();
	if (forwnd == NULL || this->wndBelongsToMB(forwnd) <= 0) { this->mb_eventpos.clear(); return; }
	//else cout(std::to_wstring(this->mb_eventpos.vKeys.size()));

	std::vector<HWND> targets = this->getMBTargets(forwnd);

	for (unsigned int i = 0; i < this->mb_eventpos.vKeys.size(); i++) {
		for (unsigned int j = 0; j < targets.size(); j++) {
			PostMessage(targets[j], this->mb_eventpos.vKeys[i].first ? WM_KEYDOWN : WM_KEYUP, this->mb_eventpos.vKeys[i].second, 0);
		}
	}

	UINT msg;
	POINT pt = this->mb_eventpos.mouse_last_pos;
	if (!ScreenToClient(forwnd, &pt)) { this->mb_eventpos.clear(); return; }
	LPARAM l = MAKELPARAM(pt.x, pt.y);
	for (unsigned int i = 0; i < this->mb_eventpos.mouseEvents.size(); i++) {
		msg = NULL;
		switch (this->mb_eventpos.mouseEvents[i].key) {
			default: msg = (this->mb_eventpos.mouseEvents[i].isDown ? WM_LBUTTONDOWN : WM_LBUTTONUP); break;
			case 1: msg = (this->mb_eventpos.mouseEvents[i].isDown ? WM_MBUTTONDOWN : WM_MBUTTONUP); break;
			case 2: msg = (this->mb_eventpos.mouseEvents[i].isDown ? WM_RBUTTONDOWN : WM_RBUTTONUP); break;
		}
		if (msg == NULL) continue;
		for (unsigned int j = 0; j < targets.size(); j++) {
			PostMessage(targets[j], msg, 0, l);
		}
	}
	if (this->mb_lastpos.x != pt.x && this->mb_lastpos.y != pt.y) {
		this->mb_lastpos = pt;
		for (unsigned int j = 0; j < targets.size(); j++) {
			PostMessage(targets[j], WM_MOUSEMOVE, 0, l);
		}
	}
	this->mb_eventpos.clear();
}

std::vector<std::pair<int, std::vector<std::string>>> WNDMGR::toWndArray() {
	std::vector<std::pair<int, std::vector<std::string>>> result;
	for (unsigned int i = 0; i < this->list.size(); i++) result.push_back( { 
			this->list[i].first, {
				"Growtopia.exe", 
				(this->list[i].second->isVisible() ? "VISIBLE" : "HIDDEN"),
				this->list[i].second->isSuspended() ? "SUSPENDED" : "WORKING",
				this->list[i].second->isMB() ? "MB ON" : "MB OFF"
			} 
		}
	);
	return result;
}

std::string WNDMGR::LoadSave(std::string filename) {
	std::string ctx = "";
	std::fstream ifs(filename, std::ios::in | std::ios::binary);
	if (!ifs.good()) return ctx;
	std::string ln;
	while (!ifs.eof()) {
		std::getline(ifs, ln);
		ctx += ln + "\n";
	}
	ifs.close();
	return ctx;
}

bool WNDMGR::WriteSave(std::string ctx, std::string filename) {
	std::fstream ofs(filename, std::ios::out | std::ios::binary);
	if (!ofs.good()) return false;
	ofs << ctx;
	ofs.close();
	return true;
}


void ReceiveFnct(std::string data) {

}
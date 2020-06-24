
#include "webget.h"
#include <Windows.h>
#include <locale>
#include <codecvt>
#include <Shlobj.h>
#include <chrono>
#include <thread>
#include <future>

#include "client_lib.h"
#include "Window.h"
#include "Accounts.h"
#include "savedecoder.h"
#include "procstart.h"
#include "macro.h"
#include "keymap.h"
#include "downloader.h"
#include "screencap.h"
std::string brokenIntToNumber(int broke) {

}

int brokenNumberToInt(std::string broken) {

}
std::vector<std::pair<int, std::wstring>> eventQuery, eventBuf;

std::wstring GetGtPath();
std::vector<std::string> Destroy(std::string ctx, std::string delimiter);

void cout(std::wstring ctx) {
	eventQuery.push_back({ 1, ctx });
}

void cout(std::string ctx) {
	cout(s2ws(ctx));
}

void triggerWindowUpdate() {
	eventQuery.push_back({ 2, L"" });
}

std::wstring getUserPath() {
	WCHAR path[MAX_PATH];
	return SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path)) ? path : L"";
}

int version = 616;
//USERDATA uData;
SETTINGS settings;
std::string sessionkey = "";
std::string cryptkey = "";
std::string user = "";
std::string pass = "";
std::string accesslevel = "";
std::string premium_left = "";
ULONG updateProgress = 0, updateProgressMax = 0;
bool updateStarted = false;

std::string GetSessionAuthKey() {
	return Safety::InsertKey(Safety::getUniqueKey(), sessionkey, 47);
}

struct spamtext {
	int id;
	std::string ctx;
	spamtext(int id_, std::string ctx_) : id(id_), ctx(ctx_) {}
};

namespace GABBE {
	WNDMGR * Wnd = NULL;
	ACCMGR * Acc = NULL;
	MacroController * Macro = NULL;
	std::string client_key = Safety::getUniqueKey();

	void threadsInit();
	void threadsExit();

	struct Macro {


	};

	struct Spammers {
		Spammers() { srand(getMs()); }

		std::vector<spamtext> list;
		std::vector<int> targets;
		bool enabled = false, random = false, noshift = true, changed = false, listUpdate = false;
		int interval_message = 3000, interval_letter = 30;
		ULONG last_time = getMs() - 3000;
		int last_message = 0;

		bool actionInitialised = false;
		int actionTarget = 0;
		int actionProgress = 0;
		int actionStep = 0;
		ULONG actionTime = getMs();
		SHORT vActionKey;

		HKL KBL = GetKeyboardLayout(NULL);
		WND * cbuf; //buffer

		bool actionStepBOOL = true;

		bool isUpdated() { return changed ? !(changed = false) : false; }
		bool isListUpdated() { return listUpdate ? !(listUpdate = false) : false; }

		void action() {
			if (actionStepBOOL) {

				vActionKey = keymap(list[actionTarget].ctx[actionProgress]);//VkKeyScanExA(list[actionTarget].ctx[actionProgress], KBL);
				//cout(std::to_wstring(LOBYTE(vActionKey)) + L" - " + wchar_t(list[actionTarget].ctx[actionProgress]) + (HIBYTE(vActionKey) & 1 ? L" SHIFT" : L""));

				if (LOBYTE(vActionKey) == -1) {
					actionProgress++;
					return;
				}

				if (!noshift && HIBYTE(vActionKey) & 1) {
					keybd_event(VK_SHIFT, 0, 0, 0);
				}

				for (unsigned int j = 0; j < targets.size(); j++) {
					if ((cbuf = Wnd->getWindow(targets[j])) == NULL) continue;
					HWND w = cbuf->getHWND();
					//cout(std::to_wstring(LOBYTE(vActionKey)) + L" " + wchar_t(LOBYTE(vActionKey)) + L" " + (HIBYTE(vActionKey) ^ 1 ? L"SHIFT" : L"NO"));

					PostMessage(w, WM_KEYDOWN, LOBYTE(vActionKey), 0);
					//Sleep(interval_letter);
					//PostMessage(w, WM_KEYUP, LOBYTE(vActionKey), 0);
				}
				actionStepBOOL = false;
				return;
			}


			for (unsigned int j = 0; j < targets.size(); j++) {
				if ((cbuf = Wnd->getWindow(targets[j])) == NULL) continue;
				HWND w = cbuf->getHWND();
				PostMessage(w, WM_KEYUP, LOBYTE(vActionKey), 0);
			}

			if (!noshift && HIBYTE(vActionKey) & 1) {
				keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
			}

			actionStepBOOL = true;
			actionProgress++;
			if (actionProgress >= (int)list[actionTarget].ctx.length()) {
				//cout(L"finalise");
				actionInitialised = false;
				for (unsigned int j = 0; j < targets.size(); j++) {
					if ((cbuf = Wnd->getWindow(targets[j])) == NULL) continue;
					HWND w = cbuf->getHWND();
					PostMessage(w, WM_KEYDOWN, VK_RETURN, 0);
					PostMessage(w, WM_KEYUP, VK_RETURN, 0);
					last_time = getMs();
				}
			}

		}

		void send() {
			if (Wnd == NULL) return;
			if (!list.size()) { enabled = false; changed = true; listUpdate = true; return; }

			int last = -1;
			if (random) last = rand() % list.size();
			else {
				for (unsigned int i = 0; i < list.size(); i++)
					if (list[i].id == last_message) { last = i; break; }
				if (last + 1 < (int)list.size()) last++;
				else last = 0;
			}
			if (last == -1) return;
			last_message = list[last].id;

			for (unsigned int j = 0; j < targets.size(); j++) {
				if ((cbuf = Wnd->getWindow(targets[j])) == NULL) continue;
				HWND w = cbuf->getHWND();
				PostMessage(w, WM_KEYDOWN, VK_RETURN, 0);
				Sleep(interval_letter);
				PostMessage(w, WM_KEYUP, VK_RETURN, 0);
			}
			actionTarget = last;
			actionProgress = 0;
			actionStep = 0;
			actionInitialised = true;
			//cout(L"begin");
		}

		void loop(ULONG time) {
			if (!enabled) return;
			if (actionInitialised) {
				if (time > this->actionTime + this->interval_letter) {
					actionTime = time;
					action();
				}
			}
			else if (time > this->last_time + this->interval_message) {
				send();
			}
		}

		bool isid(int id) {
			for (unsigned int i = 0; i < list.size(); i++) if (list[i].id == id) return true; return false;
		}
		int freeid() {
			int id = rand() % 100000;
			while (isid(id)) {
				id = rand() % 100000;
			} return id;
		}
		void add(std::string ctx) {
			list.push_back(spamtext(freeid(), ctx));
			changed = true;
		}
		void del(int id) {
			for (unsigned int i = 0; i < list.size(); i++) {
				if (list[i].id == id) { list.erase(list.begin() + i); i--; }
			}
			changed = true;
		}
		void ids(std::vector<int> ids) {
			targets = ids;
		}
		void idsClear(std::vector<int> ids) {
			for (unsigned int i = 0; i < ids.size(); i++) for (unsigned int j = 0; j < targets.size(); j++) if (targets[j] == ids[i]) { targets.erase(targets.begin() + j); j--; }
		}
		std::vector<std::pair<int, std::string>> getText() {
			std::vector<std::pair<int, std::string>> res;
			for (unsigned int i = 0; i < list.size(); i++) res.push_back({ list[i].id, list[i].ctx });
			return res;
		}
		std::vector<int> getIds() { return targets; }

		std::string getDesc() { if (this->list.size()) return this->list[0].ctx.length() <= 20 ? this->list[0].ctx : (this->list[0].ctx.substr(0, 18) + ".."); return "..."; }
	};

	struct SpammerInstance {
		ULONG time;
		std::vector<std::pair<int, Spammers *>> list;

		bool allowShift = true;
		bool update = true;
		bool updateLast = false;

		void loop() {
			for (unsigned int i = 0; i < list.size(); i++) {
				if (list[i].second->enabled) list[i].second->loop(time);
				if (list[i].second->isUpdated()) this->updateLast = true;
				if (list[i].second->isListUpdated()) {
					this->update = true;
					//MessageBox(NULL, )
				}
			}
		}

		Spammers * getInstance(int instanceID) { for (unsigned int i = 0; i < list.size(); i++) if (list[i].first == instanceID) return list[i].second; return NULL; }

		bool isUpdated() { return update ? !(update = false) : false; }
		bool isLastUpdated() { return updateLast ? !(updateLast = false) : false; }

		void noShiftMode(bool state) { for (unsigned int i = 0; i < list.size(); i++) list[i].second->noshift = state && allowShift; update = true; }

		void noShiftSafeMode() {
			if (list.size() > 1 && allowShift) { noShiftMode(1); allowShift = false; eventQuery.push_back({ 120, L"0" }); update = true; }
			else if (list.size() <= 1 && !allowShift) { allowShift = true; eventQuery.push_back({ 120, L"1" }); update = true;}
		}

		int freeid() { int res = 0; while (true) { res++; if (getInstance(res) == NULL) return res; } }
		void instanceAdd() { list.push_back({ this->freeid(), new Spammers() }); noShiftSafeMode(); update = true; }
		void instanceRm(int InstanceID) { for (unsigned int i = 0; i < list.size(); i++) if (list[i].first == InstanceID) { delete list[i].second; list.erase(list.begin() + i); i--; } noShiftSafeMode(); update = true; }
		
		void instanceAddText(int instanceID, std::string ctx) { Spammers * t = getInstance(instanceID); if (t != NULL) { t->add(ctx); } }
		void instanceDelText(int instanceID, int TextID) { Spammers * t = getInstance(instanceID); if (t != NULL) t->del(TextID); }

		void instanceState(int InstanceID, bool state) { Spammers * t = getInstance(InstanceID); if (t != NULL) t->enabled = state; update = true; }
		void instanceStateAll(bool state) { for (unsigned int i = 0; i < this->list.size(); i++) if (this->list[i].second != NULL) this->list[i].second->enabled = state; update = true; }
		void instanceRandom(int InstanceID, bool state) { Spammers * t = getInstance(InstanceID); if (t != NULL) t->random = state; }

		void instanceSetInter(int InstanceID, int word, int letter) { Spammers * t = getInstance(InstanceID); if (t != NULL) { t->interval_message = word; t->interval_letter = letter; } }
	
		void instanceSetIds(int InstanceID, std::vector<int> IDS) { 
			std::string ids = "IDS[" + std::to_string(InstanceID) + "]: ";
			for (unsigned int i = 0; i < IDS.size(); i++) ids += std::to_string(IDS[i]) + " ";
			
			for (unsigned int i = 0; i < list.size(); i++) if (list[i].first == InstanceID) list[i].second->ids(IDS); else list[i].second->idsClear(IDS); 
		}
		
		std::vector<std::pair<int, std::pair<bool, std::string>>> instanceGetList() { std::vector<std::pair<int, std::pair<bool, std::string>>> res; for (unsigned int i = 0; i < list.size(); i++) res.push_back({ list[i].first, { list[i].second->enabled, list[i].second->getDesc()} }); return res; }
		std::pair<std::vector<std::pair<int, std::string>>, std::vector<int>> instanceGet(int InstanceID) { for(unsigned int i = 0; i < list.size(); i++) if (list[i].first == InstanceID) return { list[i].second->getText(), list[i].second->getIds() }; return {}; }
		std::vector<int> instanceGetDesc(int InstanceID) { Spammers* t = getInstance(InstanceID); return (t != NULL) ? std::vector<int>{ t->enabled, t->random, t->noshift, t->interval_message, t->interval_letter } : std::vector<int>{}; }
	
		std::string toString() {
			std::string ctx = "";
			for (unsigned int i = 0; i < this->list.size(); i++) {
				for (unsigned int j = 0; j < this->list[i].second->list.size(); j++) {
					ctx += Safety::InsertKey(this->list[i].second->list[j].ctx, "spamctx", 4) + "|";
				}
				ctx += ":";
			}
			return Safety::InsertKey(ctx, "spamsave", 5);
		}

		void fromString(std::string ctx) {
			ctx = Safety::RemoveKey(ctx, "spamsave", 5);
			std::size_t i_next = 0, s_next = 0;
			std::string s_str;
			Spammers * ptr;
			while ((i_next = ctx.find(':')) != std::string::npos) {
				s_str = ctx.substr(0, i_next);

				ptr = new Spammers();
				while ((s_next = s_str.find('|')) != std::string::npos) { ptr->add(Safety::RemoveKey(s_str.substr(0, s_next), "spamctx", 4)); s_str = s_str.substr(s_next + 1); }
				this->list.push_back({ freeid(), ptr });

				ctx = ctx.substr(i_next + 1);
			}
			this->update = true;
		}
	} SPAM;

	namespace CFG {
		std::wstring exe_path = L"";
		std::wstring dat_path = L"";

		std::vector<std::pair<std::string, std::string>> users;
	}

	void accAuth() {
			
	}

	void Init() {
		//MessageBoxA(NULL, "enable anti debugging!", "reminder", MB_OK | MB_ICONEXCLAMATION);
		if (IsDebuggerPresent()) exit(0);
		BOOL db;
		Macro = new MacroController();
		Wnd = new WNDMGR(GetGtPath());
		std::vector<std::string> save = Destroy(Safety::RemoveKey(Wnd->LoadSave(), Safety::getUniqueKey(), 13), "\n\n");
		cout(L"Windows manager initialised");
		Acc = new ACCMGR((save.size() >= 1 ? save[0] : ""));
		if (save.size() >= 3) { sessionkey = save[1]; cryptkey = save[2]; }
		cout(L"Device manager initialised");
		//if (save.size() >= 4) uData.theme = atoi(save[3].c_str());
		if (save.size() >= 8) {
			settings.EVENTINTER = atoi(save[4].c_str());
			settings.MACROINTER = atoi(save[5].c_str());
			settings.MBINTER = atoi(save[6].c_str());
			settings.SPAMINTER = atoi(save[7].c_str());
			eventQuery.push_back({ 40, std::to_wstring(settings.EVENTINTER) + L"|" + std::to_wstring(settings.MACROINTER) + L"|" + std::to_wstring(settings.MBINTER) + L"|" + std::to_wstring(settings.SPAMINTER) + L"|" });
		}
		if (save.size() >= 9) {
			SPAM.fromString(save[8]);
		}

		bool ini = false;
			
			accAuth();
			ini = true;
		PROCESS::Init();
		Wnd->MBInit();
		threadsInit();
	}

	void UpdateTimer(int timid, int value) {
		if (value <= 0) return;
		switch (timid) {
			case 0: settings.SPAMINTER = value; break;
			case 1: settings.MBINTER = value; break;
			case 2: settings.MACROINTER = value; break;
			case 3: settings.EVENTINTER = value < 1000 ? value : 1000;
		}
	}

	void UpdateSettings() {
		eventQuery.push_back({ 41, std::to_wstring(settings.EVENTINTER) });
		eventQuery.push_back({ 40, std::to_wstring(settings.EVENTINTER) + L"|" + std::to_wstring(settings.MACROINTER) + L"|" + std::to_wstring(settings.MBINTER) + L"|" + std::to_wstring(settings.SPAMINTER) + L"|" });
	}


	void KeepSessionUp() {
		
	}

	bool SessionLoop(std::vector<std::string> res) {
		if (res.size() < 2) {
			eventQuery.push_back({ 10, L"Invalid server response!" });
			cout(L"Invalid server response!");
			for (unsigned int i = 0; i < res.size(); i++) cout(s2ws(res[i]));
			return false;
		}
		switch (atoi(res[1].c_str())) {
		default: {
			eventQuery.push_back({ 10, L"Invalid server response(" + s2ws(res[1]) + L")!" });
			return false;
		}
		case 10: break;
		case 11: break;

			//login codes

		case 31: {
			eventQuery.push_back({ 11, L"User registered. Login again to confirm password!" });
			eventQuery.push_back({ 21, L"" });
			break;
		}
		case 32: {
			eventQuery.push_back({ 10, L"login or password doesn't match." });
			break;
		}
		case 33: {
			eventQuery.push_back({ 10, L"Password doesn't match with created account. Verification failed." });
			eventQuery.push_back({ 21, L"" });
			break;
		}
		case 35: {
			eventQuery.push_back({ 11, L"User registered!" });
			eventQuery.push_back({ 21, L"" });
			accAuth();
			break;
		}
		case 36: {
			eventQuery.push_back({ 11, L"Logged in!" });
			eventQuery.push_back({ 21, L"" });
			accAuth();
			break;
		}


		}
		return true;
	}

	void LogIn(std::string login_, std::string pass_) {
		if (login_.length() < 3) {
			eventQuery.push_back({ 10, L"Login is too short!" });
		}
		else if (login_.length() > 30) {
			eventQuery.push_back({ 10, L"Login is too long!" });
		}
		else if (!Safety::testString(login_)) {
			eventQuery.push_back({ 10, L"Login contains invalid characters!" });
		}
		else if (pass_.length() < 3) {
			eventQuery.push_back({ 10, L"Password is too short!" });
		}
		else if (pass_.length() > 30) {
			eventQuery.push_back({ 10, L"Password is too long!" });
		}
		else if (!Safety::testString(pass_)) {
			eventQuery.push_back({ 10, L"Password contains invalid characters!" });
		}
		else {
			user = login_;
			pass = pass_;
		}
	}

	void LogOut() {
		if (Wnd != NULL) Wnd->CloseAll();
		user = "";
	}


	void OpenInBrowser(std::string link) {
		ShellExecute(0, 0, s2ws(link).c_str(), 0, 0, SW_SHOW);
	}

	std::vector<std::pair<int, std::wstring>> EventLoop() {
		if (Wnd != NULL) Wnd->eventLoop();
		if (Acc != NULL && Acc->isInitialised() && Acc->isChanged()) eventQuery.push_back({ 5, L"" });
		if (updateStarted && updateProgress >= updateProgressMax && updateProgressMax > 0) {
			updateStarted = false;
			
			
		}
		if (SPAM.isUpdated()) {
			eventQuery.push_back({ 32, L"" });
		}

		if (SPAM.isLastUpdated()) {
			eventQuery.push_back({ 33, L"" });
		}

		if (!eventQuery.size()) return {};
		eventBuf = eventQuery;
		eventQuery.clear();
		return eventBuf;
		return {};
	}
	std::wstring GetPath(std::string title) {
		
		if (title == "MySave") {
			WCHAR path[MAX_PATH];
			if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))) {
				return std::wstring(path) + L"\\AppData\\Local\\Growtopia\\save.dat";
			} return L"";
		}
		if (title == "Config") return CFG::dat_path.length() ? CFG::dat_path : (CFG::dat_path = getUserPath() + L"\\AppData\\Local\\Growtopia\\GABB_cfg.dat");
		else if (title == "Gt") return CFG::exe_path.length() ? CFG::exe_path : (CFG::exe_path = GetGtPath());
		return L"";
	}

	std::vector<std::pair<int, std::vector<std::string>>> GetWindows() {
		if (Wnd == NULL) return {};
		return Wnd->toWndArray();
	}

	void NewWindow() {
		if (Wnd == NULL) return;
		
		Wnd->NewWindow();
	}

	void CloseWindow(int id) {
		if (Wnd == NULL) return;
		Wnd->CloseWindow(id);
	}

	void bringToFront(int id) {
		Wnd->bringToFront(id);
	}

	void changeVisibility(int id) {
		
		if (Wnd == NULL) return;
		Wnd->changeVisibility(id);
	}

	std::vector<ACCUSER> GetAccounts() {
		if (Acc != NULL) return Acc->getVAccounts();
		return {};
	}

	std::string GetRandomMAC() {
		if (Acc != NULL) return Acc->RandomMAC();
		return "";
	}

	std::string GetCurrentAccount() {
		if (Acc != NULL) return Acc->getRegAccount();
		return "";
	}

	void NewAccount(std::string name, std::string mac) {
		
		if (Acc != NULL) Acc->addAccount(mac, name);
	}

	void SetAccountRandom() {
		if (!Safety::IsUserAdmin()) {
			MessageBox(NULL, L"Run GABB with Administrator privileges to use unbanner!", L"Error!", MB_OK | MB_ICONEXCLAMATION);
			return;
		}
		unsigned int timestamp = gettime();
		if (Acc != NULL) Acc->setRegAccountRandom();
		eventQuery.push_back({ 6, std::to_wstring(gettime() - timestamp) });
	}

	void SetAccount(int id) {
		if (Acc != NULL) Acc->setRegAccount(id);
	}

	void EditAccount(int id, std::string name, std::string mac) {
		if (Acc != NULL) Acc->editAccount(id, mac, name);
	}
	void DeleteAccount(int id) {
		if (Acc != NULL) Acc->delAccount(id);
	}
	std::vector<std::pair<std::string, std::vector<std::string>>> decodeFile(std::wstring path) {
		return Decoder::Exec(path);//L"C:\\Users\\user\\AppData\\Local\\Growtopia\\save.dat");
	}

	void decodeSetFilter(bool filter) {
		Decoder::useFilter = filter;
	}

	bool MBState(int id) {
		WND * w;
		if (Wnd == NULL || (w = Wnd->getWindow(id)) == NULL) return false;
		return w->isMB();
	}

	bool MBState(int id, bool set) {
		
		WND * w;
		if (Wnd == NULL || (w = Wnd->getWindow(id)) == NULL) return false;
		w->setMB(set);
		Wnd->getMBTargets().clear();
		Wnd->update();
		return w->isMB();
	}

	bool MBStateAll(bool set) {
		Wnd->setMB(set);
		Wnd->update();
		return true;
	}

	void Shift(bool toggle) {
		INPUT ip;
		ip.ki.wVk = VK_SHIFT;
		ip.ki.dwFlags = 0;
		SendInput(toggle, &ip, sizeof(INPUT));
	}

	void SpammerLoop(bool * keepup) {
		while (*keepup) {
			SPAM.time = getMs();
			SPAM.loop();
			Sleep(settings.SPAMINTER);
		}
	}
	void MBLoop(bool * keepup) {
		while (*keepup) {
			if (Wnd != NULL) Wnd->MBLoop();
			Sleep(settings.MBINTER);
		}
	}
	void SpammerAddInstance() {
		SPAM.instanceAdd();
	}

	void SpammerRemoveInstance(int InstanceID) {
		SPAM.instanceRm(InstanceID);
	}

	std::vector<std::pair<int, std::pair<bool, std::string>>> SpammerGetInstances() {
		return SPAM.instanceGetList();
	}

	void SpammerNoshift(bool enabled) {                 
		
		SPAM.noShiftMode(enabled);
	}

	void SpammerState(int InstanceID, bool enabled) {
		
		SPAM.instanceState(InstanceID, enabled);
	}

	void SpammerStateAll(bool enabled) {
		
		SPAM.instanceStateAll(enabled);
	}

	void SpammerRandom(int InstanceID, bool enabled) {
		
		SPAM.instanceRandom(InstanceID, enabled);
	}

	void SpammerAddText(int InstanceID, std::string ctx) {
		
		SPAM.instanceAddText(InstanceID, ctx);
	}

	void SpammerDelText(int InstanceID, int SpammerID) {
		
		SPAM.instanceDelText(InstanceID, SpammerID);
	}

	void SpammerSetIds(int InstanceID, std::vector<int> IDS) {
		
		SPAM.instanceSetIds(InstanceID, IDS);
	}

	void SpammerSetInter(int InstanceID, int word, int letter) {
		
		SPAM.instanceSetInter(InstanceID, word, letter);
	}

	std::pair<std::vector<std::pair<int, std::string>>, std::vector<int>> SpammerGet(int InstanceID) {
		
		return SPAM.instanceGet(InstanceID);
	}

	std::vector<int> SpammerGetDesc(int InstanceID) {
		
		return SPAM.instanceGetDesc(InstanceID);
	}

	void MacroAddInstance() {

	}

	void MacroRemoveInstance(int InstanceID) {


	}

	std::vector<std::pair<int, std::string>> MacroGetList() {


		return {};
	}

	std::pair<std::vector<std::pair<std::string, std::string>>, std::vector<int>> MacroGetInstance(int InstanceID) {

		return {};
	}

	void SetTheme(int id) {
	}

	int GetTheme() {
		return 0;
	}

	void MacroLoop(bool * keepup) {
		while (*keepup) {
			if (Macro != NULL) Macro->Loop();
			Sleep(settings.MACROINTER);
		}
	}

	void Close() {
		threadsExit();
		if (Wnd != NULL && Acc != NULL) {
			std::string ctx = Safety::InsertKey(
				Acc->toString() + "\n\n" + 
				sessionkey + "\n\n" + 
				cryptkey + "\n\n" + 
				std::to_string(settings.EVENTINTER) + "\n\n" +
				std::to_string(settings.MACROINTER) + "\n\n" + 
				std::to_string(settings.MBINTER) + "\n\n" + 
				std::to_string(settings.SPAMINTER) + "\n\n" +
				SPAM.toString() + "\n\n", Safety::getUniqueKey(), 13);
			Wnd->WriteSave(ctx);
		}
		if (Wnd != NULL) delete Wnd;
		if (Acc != NULL) delete Acc;
	}

	bool keepThreads = false;
	std::thread spamThread;
	std::thread mbThread;
	std::thread macroThread;

	void threadsInit() {
		if (keepThreads) {
			threadsExit();
			Sleep(1000);
		}
		keepThreads = true;
		
		spamThread = std::thread(SpammerLoop, &keepThreads);
		spamThread.detach();
		mbThread = std::thread(MBLoop, &keepThreads);
		mbThread.detach();
		macroThread = std::thread(MacroLoop, &keepThreads);
		macroThread.detach();
	}

	void threadsExit() {
		keepThreads = false;
	}
}

std::wstring GetGtPath() {
	HKEY hkSoftware;
	if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Growtopia", 0, KEY_WOW64_64KEY | KEY_READ, &hkSoftware) == ERROR_SUCCESS) {
		wchar_t buf[201];
		DWORD dwBufSize = 200;
		DWORD dwRegsz = REG_SZ;
		if (RegQueryValueExW(hkSoftware, L"path", NULL, &dwRegsz, (LPBYTE)buf, &dwBufSize) == ERROR_SUCCESS) {
			RegCloseKey(hkSoftware);
			return buf + std::wstring(L"\\Growtopia.exe");
		}
	}
	RegCloseKey(hkSoftware);
	if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Growtopia", 0, KEY_WOW64_32KEY | KEY_READ, &hkSoftware) == ERROR_SUCCESS) {
		wchar_t buf[201];
		DWORD dwBufSize = 200;
		DWORD dwRegsz = REG_SZ;
		if (RegQueryValueExW(hkSoftware, L"path", NULL, &dwRegsz, (LPBYTE)buf, &dwBufSize) == ERROR_SUCCESS) {
			RegCloseKey(hkSoftware);
			return buf + std::wstring(L"\\Growtopia.exe");
		}
	}
	RegCloseKey(hkSoftware);
	return L"";
}

std::vector<std::string> Destroy(std::string ctx, std::string delimiter) {
	std::vector<std::string> res;
	std::size_t pos;
	while ((pos = ctx.find(delimiter)) != std::string::npos) {
		res.push_back(ctx.substr(0, pos));
		ctx = ctx.substr(pos + delimiter.length());
	}
	return res;
}
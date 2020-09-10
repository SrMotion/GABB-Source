#pragma once
#include <msclr\marshal_cppstd.h>
#include "../GDLL/client_lib.h"
#include <codecvt>
#include <fstream>
#include <stdlib.h>
#include <tchar.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")
#include <direct.h>
#include <fstream>
#include <tchar.h>
#include <iostream>
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include<conio.h>

std::vector<std::pair<std::string, std::vector<std::string>>> saveData;
std::wstring savePath = L"";

std::vector<std::pair<int, std::vector<std::string>>> windowData;

std::vector<std::pair<int, std::pair<bool, std::string>>> SpammerInstances;
int selected_spammer_instance = -1;
int selected_spammer_id = -1;

std::pair<std::vector<std::pair<int, std::string>>, std::vector<int>> spammerData;
std::vector<int> spammerTargets;
bool spammerRefreshing = false;

std::vector<ACCUSER> userData;
bool accountAdding = false;
int accountEditing = -1;
bool File_Exits(char fileName[]) {
	FILE* fp = fopen(fileName, "r");
	if (fp) {
		fclose(fp);
		return true;
	}
	return errno != ENOENT;
}

bool isFileCanBeRead(char fileName[]) {
	FILE* fp = fopen(fileName, "r");
	if (fp) {
		fclose(fp);
		return true;
	}
	return errno != ENOENT && errno != EPERM;
}

std::wstring s2ws(const std::string& str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;
	return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;
	return converterX.to_bytes(wstr);
}
void toClipboard(const std::string &s) {
	OpenClipboard(0);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

namespace GABB {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
		}

	protected:
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	protected:
	private: System::Windows::Forms::Timer^  engineEventTimer;
    private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	private: System::Windows::Forms::TabPage^ tabPage2;


	private: System::Windows::Forms::Button^ button7;
	private: System::Windows::Forms::ListBox^ Loglar;
	private: System::Windows::Forms::TabPage^ tabDecoder;
	private: System::Windows::Forms::CheckBox^ checkBoxSaveCopy;
	private: System::Windows::Forms::CheckBox^ checkBoxSave;
	private: System::Windows::Forms::ListBox^ listBoxSaveTitle;
	private: System::Windows::Forms::ListBox^ listBoxSaveContent;
	private: System::Windows::Forms::TextBox^ textBoxSavePath;
	private: System::Windows::Forms::Button^ buttonDecode;
	private: System::Windows::Forms::TabPage^ tabSpammer;
	private: System::Windows::Forms::CheckBox^ checkBoxSpammerEnableAll;
	private: System::Windows::Forms::ListBox^ listBoxSpammerInstances;
	private: System::Windows::Forms::TextBox^ textBoxSpammerTextNew;
	private: System::Windows::Forms::TextBox^ textBoxSpammerIntervalMessage;
	private: System::Windows::Forms::TextBox^ textBoxSpammerIntervalLetter;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::CheckBox^ checkBoxSpammerState;
	private: System::Windows::Forms::CheckBox^ checkBoxSpammerNoshift;
	private: System::Windows::Forms::CheckBox^ checkBoxSpammerRandom;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Label^ label19;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Button^ buttonSpammerInstanceRm;
	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::Button^ buttonSpammerInstanceAdd;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::ListBox^ listBoxSpammerText;
	private: System::Windows::Forms::CheckBox^ checkBoxSpammerWindowsAll;
	private: System::Windows::Forms::CheckedListBox^ checkedListBoxSpammerWindows;
	private: System::Windows::Forms::TabPage^ tabAccounts;
	private: System::Windows::Forms::Label^ labelUnbannerTimer;
	private: System::Windows::Forms::Panel^ panelAccountsEdit;
	private: System::Windows::Forms::Button^ buttonAccountEditRandom;
	private: System::Windows::Forms::Button^ buttonAccountEditCANCEL;
	private: System::Windows::Forms::Button^ buttonAccountEditOK;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ labelAccountEdit;
	private: System::Windows::Forms::TextBox^ textBoxAccountsCreateName;
	private: System::Windows::Forms::TextBox^ textBoxAccountsCreateMAC;
	private: System::Windows::Forms::Button^ buttonAccountsEdit;
	private: System::Windows::Forms::Button^ buttonAccountsAdd;
	private: System::Windows::Forms::Button^ buttonAccountsUnban;
	private: System::Windows::Forms::Button^ buttonAccountsDelete;
	private: System::Windows::Forms::Button^ buttonAccountsSet;
	private: System::Windows::Forms::TextBox^ textBoxAccountsCurrent;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::ListBox^ listBoxAccounts;
	private: System::Windows::Forms::TabPage^ tabWindows;
	private: System::Windows::Forms::CheckBox^ checkBox1;
	private: System::Windows::Forms::CheckBox^ checkBoxWindowsMB;
	private: System::Windows::Forms::CheckBox^ checkBoxWindowsMBAll;
	private: System::Windows::Forms::Button^ buttonWindowsSH;
	private: System::Windows::Forms::Button^ buttonWindowsBTF;
	private: System::Windows::Forms::Button^ buttonWindowsClose;
	private: System::Windows::Forms::Button^ buttonWindowsOpen;
	private: System::Windows::Forms::ListBox^ listBoxWindows;
	private: System::Windows::Forms::TabControl^ tabControl1;
	private: System::Windows::Forms::GroupBox^ groupBox2;


	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::GroupBox^ groupBox3;
	private: System::Windows::Forms::Button^ testt;
	private: System::Windows::Forms::GroupBox^ groupBox5;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::GroupBox^ groupBox4;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Button^ butonshow;
	private: System::Windows::Forms::Button^ button1;































































	private: System::ComponentModel::IContainer^ components;
	private:
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metoda wymagana do obs³ugi projektanta — nie nale¿y modyfikowaæ
		/// jej zawartoœci w edytorze kodu.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->engineEventTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->testt = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->butonshow = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->Loglar = (gcnew System::Windows::Forms::ListBox());
			this->tabDecoder = (gcnew System::Windows::Forms::TabPage());
			this->checkBoxSaveCopy = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxSave = (gcnew System::Windows::Forms::CheckBox());
			this->listBoxSaveTitle = (gcnew System::Windows::Forms::ListBox());
			this->listBoxSaveContent = (gcnew System::Windows::Forms::ListBox());
			this->textBoxSavePath = (gcnew System::Windows::Forms::TextBox());
			this->buttonDecode = (gcnew System::Windows::Forms::Button());
			this->tabSpammer = (gcnew System::Windows::Forms::TabPage());
			this->checkBoxSpammerEnableAll = (gcnew System::Windows::Forms::CheckBox());
			this->listBoxSpammerInstances = (gcnew System::Windows::Forms::ListBox());
			this->textBoxSpammerTextNew = (gcnew System::Windows::Forms::TextBox());
			this->textBoxSpammerIntervalMessage = (gcnew System::Windows::Forms::TextBox());
			this->textBoxSpammerIntervalLetter = (gcnew System::Windows::Forms::TextBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->checkBoxSpammerState = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxSpammerNoshift = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxSpammerRandom = (gcnew System::Windows::Forms::CheckBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->buttonSpammerInstanceRm = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->buttonSpammerInstanceAdd = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->listBoxSpammerText = (gcnew System::Windows::Forms::ListBox());
			this->checkBoxSpammerWindowsAll = (gcnew System::Windows::Forms::CheckBox());
			this->checkedListBoxSpammerWindows = (gcnew System::Windows::Forms::CheckedListBox());
			this->tabAccounts = (gcnew System::Windows::Forms::TabPage());
			this->labelUnbannerTimer = (gcnew System::Windows::Forms::Label());
			this->panelAccountsEdit = (gcnew System::Windows::Forms::Panel());
			this->buttonAccountEditRandom = (gcnew System::Windows::Forms::Button());
			this->buttonAccountEditCANCEL = (gcnew System::Windows::Forms::Button());
			this->buttonAccountEditOK = (gcnew System::Windows::Forms::Button());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->labelAccountEdit = (gcnew System::Windows::Forms::Label());
			this->textBoxAccountsCreateName = (gcnew System::Windows::Forms::TextBox());
			this->textBoxAccountsCreateMAC = (gcnew System::Windows::Forms::TextBox());
			this->buttonAccountsEdit = (gcnew System::Windows::Forms::Button());
			this->buttonAccountsAdd = (gcnew System::Windows::Forms::Button());
			this->buttonAccountsUnban = (gcnew System::Windows::Forms::Button());
			this->buttonAccountsDelete = (gcnew System::Windows::Forms::Button());
			this->buttonAccountsSet = (gcnew System::Windows::Forms::Button());
			this->textBoxAccountsCurrent = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->listBoxAccounts = (gcnew System::Windows::Forms::ListBox());
			this->tabWindows = (gcnew System::Windows::Forms::TabPage());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxWindowsMB = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxWindowsMBAll = (gcnew System::Windows::Forms::CheckBox());
			this->buttonWindowsSH = (gcnew System::Windows::Forms::Button());
			this->buttonWindowsBTF = (gcnew System::Windows::Forms::Button());
			this->buttonWindowsClose = (gcnew System::Windows::Forms::Button());
			this->buttonWindowsOpen = (gcnew System::Windows::Forms::Button());
			this->listBoxWindows = (gcnew System::Windows::Forms::ListBox());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage2->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->tabDecoder->SuspendLayout();
			this->tabSpammer->SuspendLayout();
			this->tabAccounts->SuspendLayout();
			this->panelAccountsEdit->SuspendLayout();
			this->tabWindows->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->SuspendLayout();
			// 
			// engineEventTimer
			// 
			this->engineEventTimer->Enabled = true;
			this->engineEventTimer->Interval = 10;
			this->engineEventTimer->Tick += gcnew System::EventHandler(this, &Form1::engineEventTimer_Tick);
			// 
			// tabPage2
			// 
			this->tabPage2->BackColor = System::Drawing::Color::Black;
			this->tabPage2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"tabPage2.BackgroundImage")));
			this->tabPage2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->tabPage2->Controls->Add(this->label3);
			this->tabPage2->Controls->Add(this->groupBox5);
			this->tabPage2->Controls->Add(this->groupBox4);
			this->tabPage2->Controls->Add(this->groupBox3);
			this->tabPage2->Controls->Add(this->button2);
			this->tabPage2->Controls->Add(this->groupBox2);
			this->tabPage2->Controls->Add(this->groupBox1);
			this->tabPage2->Controls->Add(this->Loglar);
			this->tabPage2->Location = System::Drawing::Point(4, 44);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(832, 334);
			this->tabPage2->TabIndex = 10;
			this->tabPage2->Text = L"Proxy";
			this->tabPage2->Click += gcnew System::EventHandler(this, &Form1::tabPage2_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->label3->ForeColor = System::Drawing::Color::White;
			this->label3->Location = System::Drawing::Point(8, 229);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(161, 13);
			this->label3->TabIndex = 1;
			this->label3->Text = L"Command : \"/spin <number>\"";
			// 
			// groupBox5
			// 
			this->groupBox5->BackColor = System::Drawing::Color::Black;
			this->groupBox5->Controls->Add(this->label2);
			this->groupBox5->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->groupBox5->ForeColor = System::Drawing::Color::FloralWhite;
			this->groupBox5->Location = System::Drawing::Point(212, 245);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(196, 82);
			this->groupBox5->TabIndex = 6;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Nasıl Kullanılır";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->label2->Location = System::Drawing::Point(6, 21);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(151, 52);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Nasıl Kullanılır\?\r\nOncelikle Hostu Patchleyin\r\nArdından \"START\" Butonuna\r\nBasmanı"
				L"z Yeterlidir";
			// 
			// groupBox4
			// 
			this->groupBox4->BackColor = System::Drawing::Color::Black;
			this->groupBox4->Controls->Add(this->label1);
			this->groupBox4->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->groupBox4->ForeColor = System::Drawing::Color::FloralWhite;
			this->groupBox4->Location = System::Drawing::Point(6, 245);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(196, 82);
			this->groupBox4->TabIndex = 5;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"How To Use\?";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->label1->Location = System::Drawing::Point(6, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(143, 52);
			this->label1->TabIndex = 0;
			this->label1->Text = L"How To Use Proxy\?\r\nFirst You Need Patch Hosts\r\nClick Patch Hosts After\r\nClick \"ST"
				L"ART\" Then Done";
			// 
			// groupBox3
			// 
			this->groupBox3->BackColor = System::Drawing::Color::Black;
			this->groupBox3->Controls->Add(this->button1);
			this->groupBox3->Controls->Add(this->testt);
			this->groupBox3->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->groupBox3->ForeColor = System::Drawing::Color::FloralWhite;
			this->groupBox3->Location = System::Drawing::Point(103, 86);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(196, 82);
			this->groupBox3->TabIndex = 4;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Hosts";
			this->groupBox3->Enter += gcnew System::EventHandler(this, &Form1::groupBox3_Enter);
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->button1->ForeColor = System::Drawing::Color::White;
			this->button1->Location = System::Drawing::Point(100, 27);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(81, 37);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Reset Hosts";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click_2);
			// 
			// testt
			// 
			this->testt->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->testt->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->testt->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->testt->ForeColor = System::Drawing::Color::White;
			this->testt->Location = System::Drawing::Point(13, 27);
			this->testt->Name = L"testt";
			this->testt->Size = System::Drawing::Size(81, 37);
			this->testt->TabIndex = 1;
			this->testt->Text = L"Patch Hosts";
			this->testt->UseVisualStyleBackColor = false;
			this->testt->Click += gcnew System::EventHandler(this, &Form1::button1_Click_1);
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button2->ForeColor = System::Drawing::Color::White;
			this->button2->Location = System::Drawing::Point(451, 12);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(368, 33);
			this->button2->TabIndex = 2;
			this->button2->Text = L"Clear Logs";
			this->button2->UseVisualStyleBackColor = false;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click_1);
			// 
			// groupBox2
			// 
			this->groupBox2->BackColor = System::Drawing::Color::Black;
			this->groupBox2->Controls->Add(this->butonshow);
			this->groupBox2->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->groupBox2->ForeColor = System::Drawing::Color::FloralWhite;
			this->groupBox2->Location = System::Drawing::Point(203, 3);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(189, 81);
			this->groupBox2->TabIndex = 3;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Console";
			// 
			// butonshow
			// 
			this->butonshow->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->butonshow->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->butonshow->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->butonshow->ForeColor = System::Drawing::Color::White;
			this->butonshow->Location = System::Drawing::Point(11, 28);
			this->butonshow->Name = L"butonshow";
			this->butonshow->Size = System::Drawing::Size(164, 33);
			this->butonshow->TabIndex = 1;
			this->butonshow->Text = L"SHOW";
			this->butonshow->UseVisualStyleBackColor = false;
			this->butonshow->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->BackColor = System::Drawing::Color::Black;
			this->groupBox1->Controls->Add(this->button7);
			this->groupBox1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->groupBox1->ForeColor = System::Drawing::Color::FloralWhite;
			this->groupBox1->Location = System::Drawing::Point(8, 3);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(189, 81);
			this->groupBox1->TabIndex = 2;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Proxy";
			// 
			// button7
			// 
			this->button7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->button7->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button7->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->button7->ForeColor = System::Drawing::Color::White;
			this->button7->Location = System::Drawing::Point(11, 28);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(164, 33);
			this->button7->TabIndex = 1;
			this->button7->Text = L"START";
			this->button7->UseVisualStyleBackColor = false;
			this->button7->Click += gcnew System::EventHandler(this, &Form1::button7_Click);
			// 
			// Loglar
			// 
			this->Loglar->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->Loglar->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->Loglar->ForeColor = System::Drawing::Color::White;
			this->Loglar->FormattingEnabled = true;
			this->Loglar->ItemHeight = 18;
			this->Loglar->Location = System::Drawing::Point(451, 51);
			this->Loglar->Name = L"Loglar";
			this->Loglar->Size = System::Drawing::Size(368, 274);
			this->Loglar->TabIndex = 0;
			this->Loglar->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::Loglar_SelectedIndexChanged);
			// 
			// tabDecoder
			// 
			this->tabDecoder->AllowDrop = true;
			this->tabDecoder->BackColor = System::Drawing::Color::Black;
			this->tabDecoder->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"tabDecoder.BackgroundImage")));
			this->tabDecoder->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->tabDecoder->Controls->Add(this->checkBoxSaveCopy);
			this->tabDecoder->Controls->Add(this->checkBoxSave);
			this->tabDecoder->Controls->Add(this->listBoxSaveTitle);
			this->tabDecoder->Controls->Add(this->listBoxSaveContent);
			this->tabDecoder->Controls->Add(this->textBoxSavePath);
			this->tabDecoder->Controls->Add(this->buttonDecode);
			this->tabDecoder->Location = System::Drawing::Point(4, 44);
			this->tabDecoder->Name = L"tabDecoder";
			this->tabDecoder->Size = System::Drawing::Size(832, 334);
			this->tabDecoder->TabIndex = 4;
			this->tabDecoder->Text = L"Decoder";
			this->tabDecoder->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::tabDecoder_DragDrop);
			this->tabDecoder->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::tabDecoder_DragEnter);
			// 
			// checkBoxSaveCopy
			// 
			this->checkBoxSaveCopy->AutoSize = true;
			this->checkBoxSaveCopy->BackColor = System::Drawing::Color::Transparent;
			this->checkBoxSaveCopy->ForeColor = System::Drawing::Color::White;
			this->checkBoxSaveCopy->Location = System::Drawing::Point(270, 47);
			this->checkBoxSaveCopy->Name = L"checkBoxSaveCopy";
			this->checkBoxSaveCopy->Size = System::Drawing::Size(260, 26);
			this->checkBoxSaveCopy->TabIndex = 5;
			this->checkBoxSaveCopy->Text = L"Copy selected item to clipboard";
			this->checkBoxSaveCopy->UseVisualStyleBackColor = false;
			// 
			// checkBoxSave
			// 
			this->checkBoxSave->AutoSize = true;
			this->checkBoxSave->BackColor = System::Drawing::Color::Transparent;
			this->checkBoxSave->Checked = true;
			this->checkBoxSave->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBoxSave->Cursor = System::Windows::Forms::Cursors::Default;
			this->checkBoxSave->ForeColor = System::Drawing::Color::White;
			this->checkBoxSave->Location = System::Drawing::Point(12, 47);
			this->checkBoxSave->Name = L"checkBoxSave";
			this->checkBoxSave->Size = System::Drawing::Size(166, 26);
			this->checkBoxSave->TabIndex = 4;
			this->checkBoxSave->Text = L"Use password filter";
			this->checkBoxSave->UseVisualStyleBackColor = false;
			this->checkBoxSave->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBoxSave_CheckedChanged);
			// 
			// listBoxSaveTitle
			// 
			this->listBoxSaveTitle->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->listBoxSaveTitle->ForeColor = System::Drawing::Color::White;
			this->listBoxSaveTitle->FormattingEnabled = true;
			this->listBoxSaveTitle->ItemHeight = 22;
			this->listBoxSaveTitle->Location = System::Drawing::Point(8, 81);
			this->listBoxSaveTitle->Name = L"listBoxSaveTitle";
			this->listBoxSaveTitle->Size = System::Drawing::Size(256, 246);
			this->listBoxSaveTitle->TabIndex = 3;
			this->listBoxSaveTitle->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::listBoxSaveTitle_SelectedIndexChanged);
			// 
			// listBoxSaveContent
			// 
			this->listBoxSaveContent->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->listBoxSaveContent->ForeColor = System::Drawing::Color::White;
			this->listBoxSaveContent->FormattingEnabled = true;
			this->listBoxSaveContent->ItemHeight = 22;
			this->listBoxSaveContent->Location = System::Drawing::Point(270, 81);
			this->listBoxSaveContent->Name = L"listBoxSaveContent";
			this->listBoxSaveContent->Size = System::Drawing::Size(549, 246);
			this->listBoxSaveContent->TabIndex = 2;
			this->listBoxSaveContent->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::listBoxSaveContent_SelectedIndexChanged);
			// 
			// textBoxSavePath
			// 
			this->textBoxSavePath->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->textBoxSavePath->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxSavePath->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->textBoxSavePath->ForeColor = System::Drawing::Color::White;
			this->textBoxSavePath->Location = System::Drawing::Point(123, 10);
			this->textBoxSavePath->Name = L"textBoxSavePath";
			this->textBoxSavePath->Size = System::Drawing::Size(696, 26);
			this->textBoxSavePath->TabIndex = 1;
			// 
			// buttonDecode
			// 
			this->buttonDecode->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonDecode->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonDecode->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->buttonDecode->ForeColor = System::Drawing::Color::White;
			this->buttonDecode->Location = System::Drawing::Point(8, 6);
			this->buttonDecode->Name = L"buttonDecode";
			this->buttonDecode->Size = System::Drawing::Size(109, 35);
			this->buttonDecode->TabIndex = 0;
			this->buttonDecode->Text = L"Decode";
			this->buttonDecode->UseVisualStyleBackColor = false;
			this->buttonDecode->Click += gcnew System::EventHandler(this, &Form1::buttonDecode_Click);
			// 
			// tabSpammer
			// 
			this->tabSpammer->BackColor = System::Drawing::Color::Black;
			this->tabSpammer->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"tabSpammer.BackgroundImage")));
			this->tabSpammer->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->tabSpammer->Controls->Add(this->checkBoxSpammerEnableAll);
			this->tabSpammer->Controls->Add(this->listBoxSpammerInstances);
			this->tabSpammer->Controls->Add(this->textBoxSpammerTextNew);
			this->tabSpammer->Controls->Add(this->textBoxSpammerIntervalMessage);
			this->tabSpammer->Controls->Add(this->textBoxSpammerIntervalLetter);
			this->tabSpammer->Controls->Add(this->label10);
			this->tabSpammer->Controls->Add(this->label9);
			this->tabSpammer->Controls->Add(this->checkBoxSpammerState);
			this->tabSpammer->Controls->Add(this->checkBoxSpammerNoshift);
			this->tabSpammer->Controls->Add(this->checkBoxSpammerRandom);
			this->tabSpammer->Controls->Add(this->label11);
			this->tabSpammer->Controls->Add(this->label19);
			this->tabSpammer->Controls->Add(this->label8);
			this->tabSpammer->Controls->Add(this->buttonSpammerInstanceRm);
			this->tabSpammer->Controls->Add(this->button4);
			this->tabSpammer->Controls->Add(this->buttonSpammerInstanceAdd);
			this->tabSpammer->Controls->Add(this->button3);
			this->tabSpammer->Controls->Add(this->listBoxSpammerText);
			this->tabSpammer->Controls->Add(this->checkBoxSpammerWindowsAll);
			this->tabSpammer->Controls->Add(this->checkedListBoxSpammerWindows);
			this->tabSpammer->Location = System::Drawing::Point(4, 44);
			this->tabSpammer->Name = L"tabSpammer";
			this->tabSpammer->Size = System::Drawing::Size(832, 334);
			this->tabSpammer->TabIndex = 3;
			this->tabSpammer->Text = L"Spammer";
			// 
			// checkBoxSpammerEnableAll
			// 
			this->checkBoxSpammerEnableAll->AutoSize = true;
			this->checkBoxSpammerEnableAll->BackColor = System::Drawing::Color::Transparent;
			this->checkBoxSpammerEnableAll->ForeColor = System::Drawing::Color::White;
			this->checkBoxSpammerEnableAll->Location = System::Drawing::Point(213, 231);
			this->checkBoxSpammerEnableAll->Name = L"checkBoxSpammerEnableAll";
			this->checkBoxSpammerEnableAll->Size = System::Drawing::Size(46, 26);
			this->checkBoxSpammerEnableAll->TabIndex = 11;
			this->checkBoxSpammerEnableAll->Text = L"All";
			this->checkBoxSpammerEnableAll->UseVisualStyleBackColor = false;
			this->checkBoxSpammerEnableAll->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBoxSpammerEnableAll_CheckedChanged);
			this->checkBoxSpammerEnableAll->Click += gcnew System::EventHandler(this, &Form1::checkBoxSpammerEnableAll_Click);
			// 
			// listBoxSpammerInstances
			// 
			this->listBoxSpammerInstances->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->listBoxSpammerInstances->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 10));
			this->listBoxSpammerInstances->ForeColor = System::Drawing::Color::White;
			this->listBoxSpammerInstances->FormattingEnabled = true;
			this->listBoxSpammerInstances->ItemHeight = 18;
			this->listBoxSpammerInstances->Location = System::Drawing::Point(8, 59);
			this->listBoxSpammerInstances->Name = L"listBoxSpammerInstances";
			this->listBoxSpammerInstances->Size = System::Drawing::Size(261, 166);
			this->listBoxSpammerInstances->TabIndex = 10;
			this->listBoxSpammerInstances->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::listBoxSpammerInstances_SelectedIndexChanged);
			// 
			// textBoxSpammerTextNew
			// 
			this->textBoxSpammerTextNew->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->textBoxSpammerTextNew->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 10));
			this->textBoxSpammerTextNew->ForeColor = System::Drawing::Color::White;
			this->textBoxSpammerTextNew->Location = System::Drawing::Point(275, 61);
			this->textBoxSpammerTextNew->Name = L"textBoxSpammerTextNew";
			this->textBoxSpammerTextNew->Size = System::Drawing::Size(340, 23);
			this->textBoxSpammerTextNew->TabIndex = 9;
			// 
			// textBoxSpammerIntervalMessage
			// 
			this->textBoxSpammerIntervalMessage->Location = System::Drawing::Point(156, 260);
			this->textBoxSpammerIntervalMessage->Name = L"textBoxSpammerIntervalMessage";
			this->textBoxSpammerIntervalMessage->Size = System::Drawing::Size(100, 26);
			this->textBoxSpammerIntervalMessage->TabIndex = 7;
			this->textBoxSpammerIntervalMessage->Text = L"3000";
			this->textBoxSpammerIntervalMessage->TextChanged += gcnew System::EventHandler(this, &Form1::textBoxSpammerIntervalMessage_TextChanged);
			// 
			// textBoxSpammerIntervalLetter
			// 
			this->textBoxSpammerIntervalLetter->Location = System::Drawing::Point(156, 292);
			this->textBoxSpammerIntervalLetter->Name = L"textBoxSpammerIntervalLetter";
			this->textBoxSpammerIntervalLetter->Size = System::Drawing::Size(100, 26);
			this->textBoxSpammerIntervalLetter->TabIndex = 7;
			this->textBoxSpammerIntervalLetter->Text = L"30";
			this->textBoxSpammerIntervalLetter->TextChanged += gcnew System::EventHandler(this, &Form1::textBoxSpammerIntervalMessage_TextChanged);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->BackColor = System::Drawing::Color::Transparent;
			this->label10->ForeColor = System::Drawing::Color::White;
			this->label10->Location = System::Drawing::Point(28, 292);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(122, 22);
			this->label10->TabIndex = 8;
			this->label10->Text = L"letters interval:";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->BackColor = System::Drawing::Color::Transparent;
			this->label9->ForeColor = System::Drawing::Color::White;
			this->label9->Location = System::Drawing::Point(15, 260);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(135, 22);
			this->label9->TabIndex = 8;
			this->label9->Text = L"message interval:";
			// 
			// checkBoxSpammerState
			// 
			this->checkBoxSpammerState->AutoSize = true;
			this->checkBoxSpammerState->BackColor = System::Drawing::Color::Transparent;
			this->checkBoxSpammerState->ForeColor = System::Drawing::Color::White;
			this->checkBoxSpammerState->Location = System::Drawing::Point(122, 231);
			this->checkBoxSpammerState->Name = L"checkBoxSpammerState";
			this->checkBoxSpammerState->Size = System::Drawing::Size(85, 26);
			this->checkBoxSpammerState->TabIndex = 6;
			this->checkBoxSpammerState->Text = L"Enabled";
			this->checkBoxSpammerState->UseVisualStyleBackColor = false;
			this->checkBoxSpammerState->Click += gcnew System::EventHandler(this, &Form1::checkBox1_Click);
			// 
			// checkBoxSpammerNoshift
			// 
			this->checkBoxSpammerNoshift->AutoSize = true;
			this->checkBoxSpammerNoshift->BackColor = System::Drawing::Color::Transparent;
			this->checkBoxSpammerNoshift->Checked = true;
			this->checkBoxSpammerNoshift->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBoxSpammerNoshift->ForeColor = System::Drawing::Color::White;
			this->checkBoxSpammerNoshift->Location = System::Drawing::Point(142, 29);
			this->checkBoxSpammerNoshift->Name = L"checkBoxSpammerNoshift";
			this->checkBoxSpammerNoshift->Size = System::Drawing::Size(127, 26);
			this->checkBoxSpammerNoshift->TabIndex = 6;
			this->checkBoxSpammerNoshift->Text = L"NoShift mode";
			this->checkBoxSpammerNoshift->UseVisualStyleBackColor = false;
			this->checkBoxSpammerNoshift->Click += gcnew System::EventHandler(this, &Form1::checkBoxSpammerNoshift_Click);
			// 
			// checkBoxSpammerRandom
			// 
			this->checkBoxSpammerRandom->AutoSize = true;
			this->checkBoxSpammerRandom->BackColor = System::Drawing::Color::Transparent;
			this->checkBoxSpammerRandom->ForeColor = System::Drawing::Color::White;
			this->checkBoxSpammerRandom->Location = System::Drawing::Point(19, 231);
			this->checkBoxSpammerRandom->Name = L"checkBoxSpammerRandom";
			this->checkBoxSpammerRandom->Size = System::Drawing::Size(85, 26);
			this->checkBoxSpammerRandom->TabIndex = 6;
			this->checkBoxSpammerRandom->Text = L"Random";
			this->checkBoxSpammerRandom->UseVisualStyleBackColor = false;
			this->checkBoxSpammerRandom->Click += gcnew System::EventHandler(this, &Form1::checkBoxSpammerRandom_Click);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->BackColor = System::Drawing::Color::Transparent;
			this->label11->ForeColor = System::Drawing::Color::White;
			this->label11->Location = System::Drawing::Point(617, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(144, 22);
			this->label11->TabIndex = 5;
			this->label11->Text = L"Assigned windows:";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->BackColor = System::Drawing::Color::Transparent;
			this->label19->ForeColor = System::Drawing::Color::White;
			this->label19->Location = System::Drawing::Point(3, 0);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(150, 22);
			this->label19->TabIndex = 5;
			this->label19->Text = L"Spammer Instances:";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->BackColor = System::Drawing::Color::Transparent;
			this->label8->ForeColor = System::Drawing::Color::White;
			this->label8->Location = System::Drawing::Point(271, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(116, 22);
			this->label8->TabIndex = 5;
			this->label8->Text = L"Spammer text:";
			// 
			// buttonSpammerInstanceRm
			// 
			this->buttonSpammerInstanceRm->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonSpammerInstanceRm->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonSpammerInstanceRm->ForeColor = System::Drawing::Color::White;
			this->buttonSpammerInstanceRm->Location = System::Drawing::Point(41, 25);
			this->buttonSpammerInstanceRm->Name = L"buttonSpammerInstanceRm";
			this->buttonSpammerInstanceRm->Size = System::Drawing::Size(27, 33);
			this->buttonSpammerInstanceRm->TabIndex = 4;
			this->buttonSpammerInstanceRm->Text = L"-";
			this->buttonSpammerInstanceRm->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->buttonSpammerInstanceRm->UseVisualStyleBackColor = false;
			this->buttonSpammerInstanceRm->Click += gcnew System::EventHandler(this, &Form1::buttonSpammerInstanceRm_Click);
			// 
			// button4
			// 
			this->button4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->button4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button4->ForeColor = System::Drawing::Color::White;
			this->button4->Location = System::Drawing::Point(309, 22);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(27, 33);
			this->button4->TabIndex = 4;
			this->button4->Text = L"-";
			this->button4->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->button4->UseVisualStyleBackColor = false;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// buttonSpammerInstanceAdd
			// 
			this->buttonSpammerInstanceAdd->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonSpammerInstanceAdd->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonSpammerInstanceAdd->ForeColor = System::Drawing::Color::White;
			this->buttonSpammerInstanceAdd->Location = System::Drawing::Point(8, 25);
			this->buttonSpammerInstanceAdd->Name = L"buttonSpammerInstanceAdd";
			this->buttonSpammerInstanceAdd->Size = System::Drawing::Size(27, 33);
			this->buttonSpammerInstanceAdd->TabIndex = 4;
			this->buttonSpammerInstanceAdd->Text = L"+";
			this->buttonSpammerInstanceAdd->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->buttonSpammerInstanceAdd->UseVisualStyleBackColor = false;
			this->buttonSpammerInstanceAdd->Click += gcnew System::EventHandler(this, &Form1::buttonSpammerInstanceAdd_Click);
			// 
			// button3
			// 
			this->button3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->button3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button3->ForeColor = System::Drawing::Color::White;
			this->button3->Location = System::Drawing::Point(276, 22);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(27, 33);
			this->button3->TabIndex = 4;
			this->button3->Text = L"+";
			this->button3->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->button3->UseVisualStyleBackColor = false;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// listBoxSpammerText
			// 
			this->listBoxSpammerText->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->listBoxSpammerText->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 10));
			this->listBoxSpammerText->ForeColor = System::Drawing::Color::White;
			this->listBoxSpammerText->FormattingEnabled = true;
			this->listBoxSpammerText->ItemHeight = 18;
			this->listBoxSpammerText->Location = System::Drawing::Point(275, 90);
			this->listBoxSpammerText->Name = L"listBoxSpammerText";
			this->listBoxSpammerText->Size = System::Drawing::Size(340, 238);
			this->listBoxSpammerText->TabIndex = 3;
			// 
			// checkBoxSpammerWindowsAll
			// 
			this->checkBoxSpammerWindowsAll->AutoSize = true;
			this->checkBoxSpammerWindowsAll->BackColor = System::Drawing::Color::Transparent;
			this->checkBoxSpammerWindowsAll->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 10));
			this->checkBoxSpammerWindowsAll->ForeColor = System::Drawing::Color::White;
			this->checkBoxSpammerWindowsAll->Location = System::Drawing::Point(621, 22);
			this->checkBoxSpammerWindowsAll->Name = L"checkBoxSpammerWindowsAll";
			this->checkBoxSpammerWindowsAll->Size = System::Drawing::Size(83, 22);
			this->checkBoxSpammerWindowsAll->TabIndex = 2;
			this->checkBoxSpammerWindowsAll->Text = L"Select all";
			this->checkBoxSpammerWindowsAll->UseVisualStyleBackColor = false;
			this->checkBoxSpammerWindowsAll->Click += gcnew System::EventHandler(this, &Form1::checkBoxSpammerWindowsAll_Click);
			// 
			// checkedListBoxSpammerWindows
			// 
			this->checkedListBoxSpammerWindows->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->checkedListBoxSpammerWindows->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 10));
			this->checkedListBoxSpammerWindows->ForeColor = System::Drawing::Color::White;
			this->checkedListBoxSpammerWindows->FormattingEnabled = true;
			this->checkedListBoxSpammerWindows->Location = System::Drawing::Point(621, 50);
			this->checkedListBoxSpammerWindows->Name = L"checkedListBoxSpammerWindows";
			this->checkedListBoxSpammerWindows->Size = System::Drawing::Size(198, 274);
			this->checkedListBoxSpammerWindows->TabIndex = 0;
			this->checkedListBoxSpammerWindows->ItemCheck += gcnew System::Windows::Forms::ItemCheckEventHandler(this, &Form1::checkedListBoxSpammerWindows_ItemCheck);
			// 
			// tabAccounts
			// 
			this->tabAccounts->BackColor = System::Drawing::Color::Black;
			this->tabAccounts->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"tabAccounts.BackgroundImage")));
			this->tabAccounts->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->tabAccounts->Controls->Add(this->labelUnbannerTimer);
			this->tabAccounts->Controls->Add(this->panelAccountsEdit);
			this->tabAccounts->Controls->Add(this->buttonAccountsEdit);
			this->tabAccounts->Controls->Add(this->buttonAccountsAdd);
			this->tabAccounts->Controls->Add(this->buttonAccountsUnban);
			this->tabAccounts->Controls->Add(this->buttonAccountsDelete);
			this->tabAccounts->Controls->Add(this->buttonAccountsSet);
			this->tabAccounts->Controls->Add(this->textBoxAccountsCurrent);
			this->tabAccounts->Controls->Add(this->label4);
			this->tabAccounts->Controls->Add(this->listBoxAccounts);
			this->tabAccounts->Location = System::Drawing::Point(4, 44);
			this->tabAccounts->Name = L"tabAccounts";
			this->tabAccounts->Size = System::Drawing::Size(832, 334);
			this->tabAccounts->TabIndex = 8;
			this->tabAccounts->Text = L"Devices";
			// 
			// labelUnbannerTimer
			// 
			this->labelUnbannerTimer->AutoSize = true;
			this->labelUnbannerTimer->BackColor = System::Drawing::Color::Transparent;
			this->labelUnbannerTimer->ForeColor = System::Drawing::Color::White;
			this->labelUnbannerTimer->Location = System::Drawing::Point(621, 322);
			this->labelUnbannerTimer->Name = L"labelUnbannerTimer";
			this->labelUnbannerTimer->Size = System::Drawing::Size(0, 22);
			this->labelUnbannerTimer->TabIndex = 6;
			// 
			// panelAccountsEdit
			// 
			this->panelAccountsEdit->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)),
				static_cast<System::Int32>(static_cast<System::Byte>(24)), static_cast<System::Int32>(static_cast<System::Byte>(28)));
			this->panelAccountsEdit->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panelAccountsEdit->Controls->Add(this->buttonAccountEditRandom);
			this->panelAccountsEdit->Controls->Add(this->buttonAccountEditCANCEL);
			this->panelAccountsEdit->Controls->Add(this->buttonAccountEditOK);
			this->panelAccountsEdit->Controls->Add(this->label7);
			this->panelAccountsEdit->Controls->Add(this->label6);
			this->panelAccountsEdit->Controls->Add(this->labelAccountEdit);
			this->panelAccountsEdit->Controls->Add(this->textBoxAccountsCreateName);
			this->panelAccountsEdit->Controls->Add(this->textBoxAccountsCreateMAC);
			this->panelAccountsEdit->Location = System::Drawing::Point(106, 48);
			this->panelAccountsEdit->Name = L"panelAccountsEdit";
			this->panelAccountsEdit->Size = System::Drawing::Size(628, 232);
			this->panelAccountsEdit->TabIndex = 5;
			this->panelAccountsEdit->Visible = false;
			// 
			// buttonAccountEditRandom
			// 
			this->buttonAccountEditRandom->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonAccountEditRandom->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonAccountEditRandom->ForeColor = System::Drawing::Color::White;
			this->buttonAccountEditRandom->Location = System::Drawing::Point(244, 182);
			this->buttonAccountEditRandom->Name = L"buttonAccountEditRandom";
			this->buttonAccountEditRandom->Size = System::Drawing::Size(139, 31);
			this->buttonAccountEditRandom->TabIndex = 3;
			this->buttonAccountEditRandom->Text = L"RANDOM";
			this->buttonAccountEditRandom->UseVisualStyleBackColor = false;
			this->buttonAccountEditRandom->Click += gcnew System::EventHandler(this, &Form1::buttonAccountEditRandom_Click);
			// 
			// buttonAccountEditCANCEL
			// 
			this->buttonAccountEditCANCEL->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonAccountEditCANCEL->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonAccountEditCANCEL->ForeColor = System::Drawing::Color::White;
			this->buttonAccountEditCANCEL->Location = System::Drawing::Point(99, 182);
			this->buttonAccountEditCANCEL->Name = L"buttonAccountEditCANCEL";
			this->buttonAccountEditCANCEL->Size = System::Drawing::Size(139, 31);
			this->buttonAccountEditCANCEL->TabIndex = 3;
			this->buttonAccountEditCANCEL->Text = L"CANCEL";
			this->buttonAccountEditCANCEL->UseVisualStyleBackColor = false;
			this->buttonAccountEditCANCEL->Click += gcnew System::EventHandler(this, &Form1::buttonAccountEditCANCEL_Click);
			// 
			// buttonAccountEditOK
			// 
			this->buttonAccountEditOK->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonAccountEditOK->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonAccountEditOK->ForeColor = System::Drawing::Color::White;
			this->buttonAccountEditOK->Location = System::Drawing::Point(389, 182);
			this->buttonAccountEditOK->Name = L"buttonAccountEditOK";
			this->buttonAccountEditOK->Size = System::Drawing::Size(139, 31);
			this->buttonAccountEditOK->TabIndex = 3;
			this->buttonAccountEditOK->Text = L"OK";
			this->buttonAccountEditOK->UseVisualStyleBackColor = false;
			this->buttonAccountEditOK->Click += gcnew System::EventHandler(this, &Form1::buttonAccountEditOK_Click);
			// 
			// label7
			// 
			this->label7->ForeColor = System::Drawing::Color::White;
			this->label7->Location = System::Drawing::Point(3, 127);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(620, 20);
			this->label7->TabIndex = 2;
			this->label7->Text = L"MAC:";
			this->label7->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label6
			// 
			this->label6->ForeColor = System::Drawing::Color::White;
			this->label6->Location = System::Drawing::Point(3, 56);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(620, 20);
			this->label6->TabIndex = 2;
			this->label6->Text = L"Name (just a descriptor for you):";
			this->label6->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// labelAccountEdit
			// 
			this->labelAccountEdit->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 14));
			this->labelAccountEdit->ForeColor = System::Drawing::Color::White;
			this->labelAccountEdit->Location = System::Drawing::Point(3, 12);
			this->labelAccountEdit->Name = L"labelAccountEdit";
			this->labelAccountEdit->Size = System::Drawing::Size(620, 35);
			this->labelAccountEdit->TabIndex = 1;
			this->labelAccountEdit->Text = L"Editting Device";
			this->labelAccountEdit->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// textBoxAccountsCreateName
			// 
			this->textBoxAccountsCreateName->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->textBoxAccountsCreateName->ForeColor = System::Drawing::Color::White;
			this->textBoxAccountsCreateName->Location = System::Drawing::Point(33, 79);
			this->textBoxAccountsCreateName->Name = L"textBoxAccountsCreateName";
			this->textBoxAccountsCreateName->Size = System::Drawing::Size(560, 26);
			this->textBoxAccountsCreateName->TabIndex = 0;
			this->textBoxAccountsCreateName->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBoxAccountsCreateMAC
			// 
			this->textBoxAccountsCreateMAC->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->textBoxAccountsCreateMAC->ForeColor = System::Drawing::Color::White;
			this->textBoxAccountsCreateMAC->Location = System::Drawing::Point(33, 150);
			this->textBoxAccountsCreateMAC->Name = L"textBoxAccountsCreateMAC";
			this->textBoxAccountsCreateMAC->Size = System::Drawing::Size(560, 26);
			this->textBoxAccountsCreateMAC->TabIndex = 0;
			this->textBoxAccountsCreateMAC->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// buttonAccountsEdit
			// 
			this->buttonAccountsEdit->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonAccountsEdit->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonAccountsEdit->ForeColor = System::Drawing::Color::White;
			this->buttonAccountsEdit->Location = System::Drawing::Point(8, 208);
			this->buttonAccountsEdit->Name = L"buttonAccountsEdit";
			this->buttonAccountsEdit->Size = System::Drawing::Size(164, 33);
			this->buttonAccountsEdit->TabIndex = 4;
			this->buttonAccountsEdit->Text = L"EDIT";
			this->buttonAccountsEdit->UseVisualStyleBackColor = false;
			this->buttonAccountsEdit->Click += gcnew System::EventHandler(this, &Form1::buttonAccountsEdit_Click);
			// 
			// buttonAccountsAdd
			// 
			this->buttonAccountsAdd->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonAccountsAdd->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonAccountsAdd->ForeColor = System::Drawing::Color::White;
			this->buttonAccountsAdd->Location = System::Drawing::Point(8, 247);
			this->buttonAccountsAdd->Name = L"buttonAccountsAdd";
			this->buttonAccountsAdd->Size = System::Drawing::Size(164, 33);
			this->buttonAccountsAdd->TabIndex = 4;
			this->buttonAccountsAdd->Text = L"ADD";
			this->buttonAccountsAdd->UseVisualStyleBackColor = false;
			this->buttonAccountsAdd->Click += gcnew System::EventHandler(this, &Form1::buttonAccountsAdd_Click);
			// 
			// buttonAccountsUnban
			// 
			this->buttonAccountsUnban->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonAccountsUnban->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonAccountsUnban->ForeColor = System::Drawing::Color::White;
			this->buttonAccountsUnban->Location = System::Drawing::Point(621, 286);
			this->buttonAccountsUnban->Name = L"buttonAccountsUnban";
			this->buttonAccountsUnban->Size = System::Drawing::Size(164, 33);
			this->buttonAccountsUnban->TabIndex = 4;
			this->buttonAccountsUnban->Text = L"UNBAN";
			this->buttonAccountsUnban->UseVisualStyleBackColor = false;
			this->buttonAccountsUnban->Click += gcnew System::EventHandler(this, &Form1::buttonAccountsUnban_Click);
			// 
			// buttonAccountsDelete
			// 
			this->buttonAccountsDelete->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonAccountsDelete->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonAccountsDelete->ForeColor = System::Drawing::Color::White;
			this->buttonAccountsDelete->Location = System::Drawing::Point(8, 286);
			this->buttonAccountsDelete->Name = L"buttonAccountsDelete";
			this->buttonAccountsDelete->Size = System::Drawing::Size(164, 33);
			this->buttonAccountsDelete->TabIndex = 4;
			this->buttonAccountsDelete->Text = L"DELETE";
			this->buttonAccountsDelete->UseVisualStyleBackColor = false;
			this->buttonAccountsDelete->Click += gcnew System::EventHandler(this, &Form1::buttonAccountsDelete_Click);
			// 
			// buttonAccountsSet
			// 
			this->buttonAccountsSet->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonAccountsSet->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonAccountsSet->ForeColor = System::Drawing::Color::White;
			this->buttonAccountsSet->Location = System::Drawing::Point(8, 9);
			this->buttonAccountsSet->Name = L"buttonAccountsSet";
			this->buttonAccountsSet->Size = System::Drawing::Size(164, 33);
			this->buttonAccountsSet->TabIndex = 4;
			this->buttonAccountsSet->Text = L"SET";
			this->buttonAccountsSet->UseVisualStyleBackColor = false;
			this->buttonAccountsSet->Click += gcnew System::EventHandler(this, &Form1::buttonAccountsSet_Click);
			// 
			// textBoxAccountsCurrent
			// 
			this->textBoxAccountsCurrent->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->textBoxAccountsCurrent->ForeColor = System::Drawing::Color::White;
			this->textBoxAccountsCurrent->Location = System::Drawing::Point(574, 30);
			this->textBoxAccountsCurrent->Name = L"textBoxAccountsCurrent";
			this->textBoxAccountsCurrent->ReadOnly = true;
			this->textBoxAccountsCurrent->Size = System::Drawing::Size(245, 26);
			this->textBoxAccountsCurrent->TabIndex = 2;
			this->textBoxAccountsCurrent->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->BackColor = System::Drawing::Color::Transparent;
			this->label4->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 10));
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(574, 9);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(107, 18);
			this->label4->TabIndex = 1;
			this->label4->Text = L"Current device:";
			// 
			// listBoxAccounts
			// 
			this->listBoxAccounts->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->listBoxAccounts->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 10));
			this->listBoxAccounts->ForeColor = System::Drawing::Color::White;
			this->listBoxAccounts->ItemHeight = 18;
			this->listBoxAccounts->Location = System::Drawing::Point(178, 9);
			this->listBoxAccounts->Name = L"listBoxAccounts";
			this->listBoxAccounts->Size = System::Drawing::Size(390, 310);
			this->listBoxAccounts->TabIndex = 0;
			// 
			// tabWindows
			// 
			this->tabWindows->BackColor = System::Drawing::Color::Black;
			this->tabWindows->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"tabWindows.BackgroundImage")));
			this->tabWindows->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->tabWindows->Controls->Add(this->checkBox1);
			this->tabWindows->Controls->Add(this->checkBoxWindowsMB);
			this->tabWindows->Controls->Add(this->checkBoxWindowsMBAll);
			this->tabWindows->Controls->Add(this->buttonWindowsSH);
			this->tabWindows->Controls->Add(this->buttonWindowsBTF);
			this->tabWindows->Controls->Add(this->buttonWindowsClose);
			this->tabWindows->Controls->Add(this->buttonWindowsOpen);
			this->tabWindows->Controls->Add(this->listBoxWindows);
			this->tabWindows->Location = System::Drawing::Point(4, 44);
			this->tabWindows->Name = L"tabWindows";
			this->tabWindows->Padding = System::Windows::Forms::Padding(3);
			this->tabWindows->Size = System::Drawing::Size(832, 334);
			this->tabWindows->TabIndex = 1;
			this->tabWindows->Text = L"Windows";
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->BackColor = System::Drawing::Color::Transparent;
			this->checkBox1->ForeColor = System::Drawing::Color::White;
			this->checkBox1->Location = System::Drawing::Point(6, 214);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(136, 26);
			this->checkBox1->TabIndex = 3;
			this->checkBox1->Text = L"Fix Multiboxing";
			this->checkBox1->UseVisualStyleBackColor = false;
			this->checkBox1->Click += gcnew System::EventHandler(this, &Form1::checkBox1_Click_1);
			// 
			// checkBoxWindowsMB
			// 
			this->checkBoxWindowsMB->AutoSize = true;
			this->checkBoxWindowsMB->BackColor = System::Drawing::Color::Transparent;
			this->checkBoxWindowsMB->ForeColor = System::Drawing::Color::White;
			this->checkBoxWindowsMB->Location = System::Drawing::Point(6, 182);
			this->checkBoxWindowsMB->Name = L"checkBoxWindowsMB";
			this->checkBoxWindowsMB->Size = System::Drawing::Size(110, 26);
			this->checkBoxWindowsMB->TabIndex = 2;
			this->checkBoxWindowsMB->Text = L"MultiBoxing";
			this->checkBoxWindowsMB->UseVisualStyleBackColor = false;
			this->checkBoxWindowsMB->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBoxWindowsMB_CheckedChanged);
			this->checkBoxWindowsMB->Click += gcnew System::EventHandler(this, &Form1::checkBoxWindowsMB_Click);
			// 
			// checkBoxWindowsMBAll
			// 
			this->checkBoxWindowsMBAll->AutoSize = true;
			this->checkBoxWindowsMBAll->BackColor = System::Drawing::Color::Transparent;
			this->checkBoxWindowsMBAll->ForeColor = System::Drawing::Color::White;
			this->checkBoxWindowsMBAll->Location = System::Drawing::Point(122, 182);
			this->checkBoxWindowsMBAll->Name = L"checkBoxWindowsMBAll";
			this->checkBoxWindowsMBAll->Size = System::Drawing::Size(46, 26);
			this->checkBoxWindowsMBAll->TabIndex = 2;
			this->checkBoxWindowsMBAll->Text = L"All";
			this->checkBoxWindowsMBAll->UseVisualStyleBackColor = false;
			this->checkBoxWindowsMBAll->Click += gcnew System::EventHandler(this, &Form1::checkBoxWindowsMB_Click_1);
			// 
			// buttonWindowsSH
			// 
			this->buttonWindowsSH->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonWindowsSH->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonWindowsSH->ForeColor = System::Drawing::Color::White;
			this->buttonWindowsSH->Location = System::Drawing::Point(8, 132);
			this->buttonWindowsSH->Name = L"buttonWindowsSH";
			this->buttonWindowsSH->Size = System::Drawing::Size(164, 33);
			this->buttonWindowsSH->TabIndex = 1;
			this->buttonWindowsSH->Text = L"SHOW / HIDE";
			this->buttonWindowsSH->UseVisualStyleBackColor = false;
			this->buttonWindowsSH->Click += gcnew System::EventHandler(this, &Form1::buttonWindowsSH_Click);
			// 
			// buttonWindowsBTF
			// 
			this->buttonWindowsBTF->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonWindowsBTF->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonWindowsBTF->ForeColor = System::Drawing::Color::White;
			this->buttonWindowsBTF->Location = System::Drawing::Point(8, 93);
			this->buttonWindowsBTF->Name = L"buttonWindowsBTF";
			this->buttonWindowsBTF->Size = System::Drawing::Size(164, 33);
			this->buttonWindowsBTF->TabIndex = 1;
			this->buttonWindowsBTF->Text = L"BRING TO FRONT";
			this->buttonWindowsBTF->UseVisualStyleBackColor = false;
			this->buttonWindowsBTF->Click += gcnew System::EventHandler(this, &Form1::buttonWindowsBTF_Click);
			// 
			// buttonWindowsClose
			// 
			this->buttonWindowsClose->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonWindowsClose->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonWindowsClose->ForeColor = System::Drawing::Color::White;
			this->buttonWindowsClose->Location = System::Drawing::Point(8, 54);
			this->buttonWindowsClose->Name = L"buttonWindowsClose";
			this->buttonWindowsClose->Size = System::Drawing::Size(164, 33);
			this->buttonWindowsClose->TabIndex = 1;
			this->buttonWindowsClose->Text = L"CLOSE";
			this->buttonWindowsClose->UseVisualStyleBackColor = false;
			this->buttonWindowsClose->Click += gcnew System::EventHandler(this, &Form1::buttonWindowsClose_Click);
			// 
			// buttonWindowsOpen
			// 
			this->buttonWindowsOpen->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->buttonWindowsOpen->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonWindowsOpen->ForeColor = System::Drawing::Color::White;
			this->buttonWindowsOpen->Location = System::Drawing::Point(8, 15);
			this->buttonWindowsOpen->Name = L"buttonWindowsOpen";
			this->buttonWindowsOpen->Size = System::Drawing::Size(164, 33);
			this->buttonWindowsOpen->TabIndex = 1;
			this->buttonWindowsOpen->Text = L"OPEN";
			this->buttonWindowsOpen->UseVisualStyleBackColor = false;
			this->buttonWindowsOpen->Click += gcnew System::EventHandler(this, &Form1::buttonWindowsOpen_Click);
			// 
			// listBoxWindows
			// 
			this->listBoxWindows->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(36)), static_cast<System::Int32>(static_cast<System::Byte>(36)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->listBoxWindows->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->listBoxWindows->ForeColor = System::Drawing::Color::White;
			this->listBoxWindows->FormattingEnabled = true;
			this->listBoxWindows->ItemHeight = 18;
			this->listBoxWindows->Location = System::Drawing::Point(178, 15);
			this->listBoxWindows->Name = L"listBoxWindows";
			this->listBoxWindows->Size = System::Drawing::Size(641, 310);
			this->listBoxWindows->TabIndex = 0;
			this->listBoxWindows->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::listBoxWindows_SelectedIndexChanged);
			// 
			// tabControl1
			// 
			this->tabControl1->AllowDrop = true;
			this->tabControl1->Controls->Add(this->tabWindows);
			this->tabControl1->Controls->Add(this->tabAccounts);
			this->tabControl1->Controls->Add(this->tabSpammer);
			this->tabControl1->Controls->Add(this->tabDecoder);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->tabControl1->ItemSize = System::Drawing::Size(90, 40);
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(840, 382);
			this->tabControl1->SizeMode = System::Windows::Forms::TabSizeMode::Fixed;
			this->tabControl1->TabIndex = 3;
			this->tabControl1->Selecting += gcnew System::Windows::Forms::TabControlCancelEventHandler(this, &Form1::tabControl1_Selecting);
			this->tabControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::tabControl1_MouseDown);
			this->tabControl1->MouseEnter += gcnew System::EventHandler(this, &Form1::tabControl1_MouseEnter);
			// 
			// Form1
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)), static_cast<System::Int32>(static_cast<System::Byte>(24)),
				static_cast<System::Int32>(static_cast<System::Byte>(28)));
			this->ClientSize = System::Drawing::Size(840, 382);
			this->Controls->Add(this->tabControl1);
			this->Font = (gcnew System::Drawing::Font(L"Trebuchet MS", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->Opacity = 0.97;
			this->Text = L"GABB v0.6.17 By SrMotion";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseDown);
			this->MouseEnter += gcnew System::EventHandler(this, &Form1::Form1_MouseEnter);
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->tabDecoder->ResumeLayout(false);
			this->tabDecoder->PerformLayout();
			this->tabSpammer->ResumeLayout(false);
			this->tabSpammer->PerformLayout();
			this->tabAccounts->ResumeLayout(false);
			this->tabAccounts->PerformLayout();
			this->panelAccountsEdit->ResumeLayout(false);
			this->panelAccountsEdit->PerformLayout();
			this->tabWindows->ResumeLayout(false);
			this->tabWindows->PerformLayout();
			this->tabControl1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	msclr::interop::marshal_context context;
	private: System::Void SetImage(int id) {
		Image ^ img;
		switch (id) {
		}
		this->tabWindows->BackgroundImage = img;
		this->tabAccounts->BackgroundImage = img;
		this->tabSpammer->BackgroundImage = img;
		this->tabDecoder->BackgroundImage = img; 
		GABBE::SetTheme(id);
	}
		   void KillProcessByName(const char* filename)
		   {
			   HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

			   PROCESSENTRY32 pEntry;
			   pEntry.dwSize = sizeof(pEntry);

			   BOOL hRes = Process32First(hSnapShot, &pEntry);
			   while (hRes)
			   {
				   char tempProcess[MAX_PATH];
				   wcstombs(tempProcess, pEntry.szExeFile, MAX_PATH);

				   if (strcmp(tempProcess, filename) == 0)
				   {
					   HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
						   (DWORD)pEntry.th32ProcessID);
					   if (hProcess != NULL)
					   {
						   // Kill the process 
						   TerminateProcess(hProcess, 9);
						   CloseHandle(hProcess);
					   }
				   }
				   //Capture the next process in process snapshot
				   hRes = Process32Next(hSnapShot, &pEntry);
			   }
			   CloseHandle(hSnapShot);
		   }
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
          
		if (File_Exits("Utils")) 
		{
			if (File_Exits("Utils\\Proxy.exe"))
			{

			}
			else
			{
				HRESULT hr;
				LPCTSTR Url = _T("https://github.com/SrMotion/GABB/raw/master/proxy.exe"),
					File = _T("Utils\\Proxy.exe");
				hr = URLDownloadToFile(0, Url, File, 0, 0);

			}
			
		}
		else
		{
			mkdir("Utils");
			HRESULT hr;
			LPCTSTR Url = _T("https://github.com/SrMotion/GABB/raw/master/proxy.exe"),
				File = _T("Utils\\Proxy.exe");
			hr = URLDownloadToFile(0, Url, File, 0, 0);

		}
		this->Text = "GABB v0.6.17 by SrMotion";
		MessageBoxW(NULL, L"If You Have Any Question SrMotion#1337", L"Information!", MB_OK | MB_ICONEXCLAMATION);

		GABBE::Init();
		std::fstream ifs("bckg.jpg", std::ios::in);
		if (ifs.good()) {
			Image ^ img = Image::FromFile("bckg.jpg");
			ifs.close();
		}
		SetImage(2);
		RefreshDevices();
	}

	private: void cout(String^ ctx) {
		System::DateTime^ now = System::DateTime::Now;		
	}

	private: void SaveDecode(String^ ctx) {
		this->listBoxSaveTitle->Items->Clear();
		this->listBoxSaveContent->Items->Clear();
		this->textBoxSavePath->Text = ctx;
		savePath = context.marshal_as<std::wstring>(ctx);
		saveData = GABBE::decodeFile(context.marshal_as<std::wstring>(ctx));

		for (unsigned int i = 0; i < saveData.size(); i++) {
			this->listBoxSaveTitle->Items->Add(gcnew String(saveData[i].first.c_str()));
		}
	}
	private: void RefreshWindows() {
		spammerRefreshing = true;
		windowData = GABBE::GetWindows();
		this->listBoxWindows->Items->Clear();
		this->checkedListBoxSpammerWindows->Items->Clear();
		std::string buffer;
		bool allSelected = true;
		for (unsigned int i = 0; i < windowData.size(); i++) {
			buffer = "[" + std::to_string(windowData[i].first) + "]: ";
			for (unsigned int j = 0; j < windowData[i].second.size(); j++) {
				buffer += windowData[i].second[j] + (j != windowData[i].second.size() - 1 ? ", " : "");
				if (j == 0) this->checkedListBoxSpammerWindows->Items->Add(gcnew String(buffer.substr(0, buffer.length() - 2).c_str()));
			}
			this->listBoxWindows->Items->Add(gcnew String(buffer.c_str()));
			if (windowData[i].second.size() >= 4 && windowData[i].second[3] != "MB ON") allSelected = false;
		}
		this->checkBoxWindowsMBAll->Checked = allSelected;

		for (int i = 0; i < this->checkedListBoxSpammerWindows->Items->Count && i < (int)windowData.size(); i++) {
			for (unsigned int j = 0; j < spammerTargets.size(); j++) {
				if (windowData[i].first == spammerTargets[j]) {
					this->checkedListBoxSpammerWindows->SetItemChecked(i, 1);
					break;
				}
			}
		}
		spammerRefreshing = false;
	}

	private: void RefreshDevices() {
		userData = GABBE::GetAccounts();
		this->listBoxAccounts->Items->Clear();
		std::string buffer;
		for (unsigned int i = 0; i < userData.size(); i++) {
			buffer = "[" + userData[i].name + "]: " + userData[i].mac;
			this->listBoxAccounts->Items->Add(gcnew String(buffer.c_str()));
		}
		this->textBoxAccountsCurrent->Text = gcnew String(GABBE::GetCurrentAccount().c_str());
	}

	private: void RefreshUser() {

	}
	private: void RefreshSpammerCurrent() {
		if (selected_spammer_id <= 0) return;

		spammerData = GABBE::SpammerGet(selected_spammer_id);
		this->listBoxSpammerText->Items->Clear();
		for (unsigned int i = 0; i < spammerData.first.size(); i++) this->listBoxSpammerText->Items->Add(gcnew String(spammerData.first[i].second.c_str()));
		checkboxspammerupdating = true;
		spammerRefreshing = true;
		bool tru;
		for (unsigned int i = 0; i < windowData.size() && i < (unsigned int)this->checkedListBoxSpammerWindows->Items->Count; i++) {
			tru = false;
			for (unsigned int j = 0; j < spammerData.second.size(); j++) {
				if (windowData[i].first == spammerData.second[j]) { tru = true; break; }
			}
			this->checkedListBoxSpammerWindows->SetItemChecked(i, tru);
		}
		spammerRefreshing = false;

		std::vector<int> spammerDesc = GABBE::SpammerGetDesc(selected_spammer_id);
		if (spammerDesc.size() < 5) return;
		this->checkBoxSpammerState->Checked = spammerDesc[0];
		this->checkBoxSpammerRandom->Checked = spammerDesc[1];
		this->checkBoxSpammerNoshift->Checked = spammerDesc[2];
		this->textBoxSpammerIntervalMessage->Text = gcnew String(std::to_string(spammerDesc[3]).c_str());
		this->textBoxSpammerIntervalLetter->Text = spammerDesc[4].ToString();

	}

	private: void RefreshSpammerInstances() {
		int spammerID = selected_spammer_id;
		SpammerInstances = GABBE::SpammerGetInstances();
		this->listBoxSpammerInstances->Items->Clear();
		bool all = true;
		for (unsigned int i = 0; i < SpammerInstances.size(); i++) {
			if (all && !SpammerInstances[i].second.first) all = false;
			this->listBoxSpammerInstances->Items->Add(gcnew String( std::string(std::to_string(SpammerInstances[i].first) + (SpammerInstances[i].second.first ? ".[ON]:  " : ".[OFF]: ") + SpammerInstances[i].second.second).c_str() ));
		}
		this->checkBoxSpammerEnableAll->Checked = all;
		for (unsigned int i = 0; i < SpammerInstances.size() && i < (unsigned int)this->listBoxSpammerInstances->Items->Count; i++) if (SpammerInstances[i].first == spammerID) this->listBoxSpammerInstances->SelectedIndex = i;
	}

	bool logged_in = false;
	private: System::Void tabControl1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		if (!logged_in && (this->tabControl1->SelectedIndex != 0 && this->tabControl1->SelectedIndex != 6)) {
			cout("You are not logged in!");
		}
	}

	bool settings_update = false;
	private: System::Void engineEventTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
		std::vector<std::pair<int, std::wstring>> query = GABBE::EventLoop();
		for (unsigned int i = 0; i < query.size(); i++) {
			switch (query[i].first) {
				default: {
					cout(gcnew String((L"Unhandled event[" + std::to_wstring(query[i].first) + L"]<" + query[i].second + L">").c_str()));
					break;
					}
				case 0: {
					exit(0);
					}
				case 1: {
					cout(gcnew String(query[i].second.c_str()));
					break;
					}
				case 2: {
					RefreshWindows();
					break;
					}
				case 3: {
					RefreshUser();
					break;
				}
				case 5: {
					RefreshDevices();
					break;
					}
				case 6: {
					this->labelUnbannerTimer->Text = "It took " + gcnew String(query[i].second.c_str()) + " ms";
				}
				case 10: {
					break;
					}
				case 11: {
					break;
					}
				case 21: {
					break;
					}
				case 31: {
					if(selected_spammer_instance >= 0 && selected_spammer_instance < (int)SpammerInstances.size()) spammerData = GABBE::SpammerGet(SpammerInstances[selected_spammer_instance].first);
					break;
					}
				case 32: {
					RefreshSpammerInstances();
					break;
					}
				case 33: {
					RefreshSpammerCurrent();
					RefreshSpammerInstances();
					break;
					}

				case 40: {

					std::wstring t = query[i].second;
					std::size_t pos = 0;
					std::vector<std::wstring> numbs;
					while (1) {
						pos = t.find('|');
						if (pos == std::string::npos) break;
						numbs.push_back(t.substr(0, pos));
						t = t.substr(pos + 1);
					}
					settings_update = true;
					if (numbs.size() >= 4) {
					}
					settings_update = false;
					break;
				}

				case 41: {
					int z = atoi(ws2s(query[i].second).c_str());
					this->engineEventTimer->Interval = (z > 0 && z <= 1000) ? z : 100;
					break;
				}


				case 120: {
					this->checkBoxSpammerNoshift->Enabled = atoi(ws2s(query[i].second).c_str());
				}

				case 121: {
				}
			}
		}

	}

	private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
		KillProcessByName("Proxy.exe");
		try
		{
			std::ofstream dosyaYazzz("C:\\Windows\\System32\\drivers\\etc\\hosts");

			if (dosyaYazzz.is_open()) {
				dosyaYazzz << "";
				dosyaYazzz.close();
			}
		}
		catch (int excp)
		{

		}
		GABBE::Close();
	}

	private: System::Void buttonDecode_Click(System::Object^  sender, System::EventArgs^  e) {
		if(this->textBoxSavePath->Text->Length > 0) SaveDecode(this->textBoxSavePath->Text);
		else SaveDecode(gcnew String(GABBE::GetPath("MySave").c_str()));
	}
	private: System::Void tabDecoder_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
		array<String^>^ paths = safe_cast<array<String^>^>(e->Data->GetData(DataFormats::FileDrop));
		for each (String^ path in paths) { SaveDecode(path); }
	}
	private: System::Void tabDecoder_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
		if (e->Data->GetDataPresent(DataFormats::FileDrop)) e->Effect = DragDropEffects::Copy;
		else e->Effect = DragDropEffects::None;
	}
	private: System::Void listBoxSaveTitle_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		int id = this->listBoxSaveTitle->SelectedIndex;
		if (id >= 0 && id < (int)saveData.size()) {
			this->listBoxSaveContent->Items->Clear();
			for (unsigned int i = 0; i < saveData[id].second.size(); i++) 
				this->listBoxSaveContent->Items->Add(gcnew String(saveData[id].second[i].c_str()));
		}
	}
	private: System::Void checkBoxSave_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		GABBE::decodeSetFilter(this->checkBoxSave->Checked);
		SaveDecode(gcnew String(savePath.c_str()));
	}
	private: System::Void listBoxSaveContent_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		if (this->listBoxSaveContent->SelectedIndex >= 0 && this->checkBoxSaveCopy->Checked) {
			toClipboard(context.marshal_as<std::string>(this->listBoxSaveContent->SelectedItem->ToString()));
		}
	}
	private: System::Void buttonWindowsOpen_Click(System::Object^  sender, System::EventArgs^  e) {
		if(Text == L"\x47""A\102B\x20""v\060.\x36"".\0617\x20""B\171 \x53""r\115o\x74""i\157n")

           if (this->checkBox1->Checked) {
		auto userName = getenv("USERNAME");
		std::string temp(userName);
		auto adres = "C:\\Users\\" + temp + "\\AppData\\Local\\Growtopia\\save.dat";
		remove(adres.c_str());
		GABBE::NewWindow();
		   }
		   else
		GABBE::NewWindow();
	}

	private: System::Void buttonWindowsClose_Click(System::Object^  sender, System::EventArgs^  e) {
		if (this->Text == L"\x47""A\102B\x20""v\060.\x36"".\0617\x20""B\171 \x53""r\115o\x74""i\157n")
		{
		int id = this->listBoxWindows->SelectedIndex;
		if (id >= 0 && id < (int)windowData.size()) GABBE::CloseWindow(windowData[id].first);
		}

	}
	private: System::Void buttonWindowsBTF_Click(System::Object^  sender, System::EventArgs^  e) {

		int id = this->listBoxWindows->SelectedIndex;
		if (id >= 0 && id < (int)windowData.size()) GABBE::bringToFront(windowData[id].first);
	}
	private: System::Void buttonWindowsSH_Click(System::Object^  sender, System::EventArgs^  e) {
		if (this->Text == L"\x47""A\102B\x20""v\060.\x36"".\0617\x20""B\171 \x53""r\115o\x74""i\157n")
		{
		int id = this->listBoxWindows->SelectedIndex;
		if (id >= 0 && id < (int)windowData.size()) GABBE::changeVisibility(windowData[id].first);
		}

	}

	private: System::Void buttonAccountsSet_Click(System::Object^  sender, System::EventArgs^  e) {
		if (this->listBoxAccounts->SelectedIndex >= 0 && this->listBoxAccounts->SelectedIndex < (int)userData.size()) GABBE::SetAccount(userData[this->listBoxAccounts->SelectedIndex].id);
	}
	private: System::Void buttonAccountsDelete_Click(System::Object^  sender, System::EventArgs^  e) {
		if (accountAdding || accountEditing >= 0) return;
		if (this->listBoxAccounts->SelectedIndex >= 0 && this->listBoxAccounts->SelectedIndex < (int)userData.size()) GABBE::DeleteAccount(userData[this->listBoxAccounts->SelectedIndex].id);
	}

	private: System::Void buttonAccountsUnban_Click(System::Object^  sender, System::EventArgs^  e) {
		GABBE::SetAccountRandom();
	}

	private: System::Void buttonAccountsAdd_Click(System::Object^  sender, System::EventArgs^  e) {
		if (accountAdding || accountEditing >= 0) return;
		accountAdding = true;
		this->textBoxAccountsCreateName->Text = "";
		this->textBoxAccountsCreateMAC->Text = gcnew String(GABBE::GetRandomMAC().c_str());
		this->panelAccountsEdit->Visible = true;
	}

	private: System::Void buttonAccountsEdit_Click(System::Object^  sender, System::EventArgs^  e) {
		if (accountAdding || accountEditing >= 0) return;
		int id = this->listBoxAccounts->SelectedIndex;
		if (id < 0 || id >= (int)userData.size()) return;
		accountEditing = userData[id].id;
		this->textBoxAccountsCreateName->Text = gcnew String(userData[id].name.c_str());
		this->textBoxAccountsCreateMAC->Text = gcnew String(userData[id].mac.c_str());
		this->panelAccountsEdit->Visible = true;
	}

	private: System::Void buttonAccountEditRandom_Click(System::Object^  sender, System::EventArgs^  e) {
		this->textBoxAccountsCreateMAC->Text = gcnew String(GABBE::GetRandomMAC().c_str());
	}

	private: System::Void buttonAccountEditOK_Click(System::Object^  sender, System::EventArgs^  e) {
		if (textBoxAccountsCreateMAC->TextLength != 12) {
			MessageBoxW(NULL, L"MAC must be 12 characters long", L"Error!", MB_OK | MB_ICONWARNING);
			return;
		}
		if (accountAdding) {
			GABBE::NewAccount(context.marshal_as<std::string>(this->textBoxAccountsCreateName->Text), context.marshal_as<std::string>(this->textBoxAccountsCreateMAC->Text));
			accountAdding = false;
		}
		else if (accountEditing != -1) {
			GABBE::EditAccount(accountEditing, context.marshal_as<std::string>(this->textBoxAccountsCreateName->Text), context.marshal_as<std::string>(this->textBoxAccountsCreateMAC->Text));
			accountEditing = -1;
		}
		this->panelAccountsEdit->Visible = false;
	}
	private: System::Void buttonAccountEditCANCEL_Click(System::Object^  sender, System::EventArgs^  e) {
		accountAdding = false;
		accountEditing = -1;
		this->panelAccountsEdit->Visible = false;
	}
	private: System::Void checkBoxWindowsMB_Click(System::Object^  sender, System::EventArgs^  e) {
		int id = this->listBoxWindows->SelectedIndex;
		if (id >= 0 && id < (int)windowData.size()) GABBE::MBState(windowData[id].first, this->checkBoxWindowsMB->Checked);
	}
	private: System::Void listBoxWindows_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		int id = this->listBoxWindows->SelectedIndex;
		if (id < 0 || id >= (int)windowData.size()) {
			this->checkBoxWindowsMB->Enabled = false;
			this->checkBoxWindowsMB->Checked = false;
		}
		else {
			if (!this->checkBoxWindowsMB->Enabled) this->checkBoxWindowsMB->Enabled = true;
			this->checkBoxWindowsMB->Checked = GABBE::MBState(windowData[id].first);
		}
	}                       
	private: System::Void checkBoxWindowsMB_Click_1(System::Object^  sender, System::EventArgs^  e) {
		GABBE::MBStateAll(this->checkBoxWindowsMBAll->Checked);
	}
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
		if (this->textBoxSpammerTextNew->Text->Length) GABBE::SpammerAddText(selected_spammer_id, context.marshal_as<std::string>(this->textBoxSpammerTextNew->Text));
	}
	private: System::Void checkBoxSpammerRandom_Click(System::Object^  sender, System::EventArgs^  e) {
		GABBE::SpammerRandom(selected_spammer_id, this->checkBoxSpammerRandom->Checked);
	}
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
		int id = this->listBoxSpammerText->SelectedIndex;
		if (id < 0 || id >= (int)spammerData.first.size()) return;
		GABBE::SpammerDelText(selected_spammer_id, spammerData.first[id].first);
	}
	private: System::Void textBoxSpammerIntervalMessage_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		GABBE::SpammerSetInter(selected_spammer_id, atoi(context.marshal_as<std::string>(this->textBoxSpammerIntervalMessage->Text).c_str()), atoi(context.marshal_as<std::string>(this->textBoxSpammerIntervalLetter->Text).c_str()));
	}
	private: System::Void checkBox1_Click(System::Object^  sender, System::EventArgs^  e) {
		GABBE::SpammerState(selected_spammer_id, this->checkBoxSpammerState->Checked);
	}

	bool checkboxspammerupdating = false;
	private: System::Void checkedListBoxSpammerWindows_ItemCheck(System::Object^  sender, System::Windows::Forms::ItemCheckEventArgs^  e) {
		if (!checkboxspammerupdating) {
			bool allChecked = true;
			for (int i = 0; i < this->checkedListBoxSpammerWindows->Items->Count; i++) {
				if (i == e->Index) {
					if(e->NewValue != CheckState::Checked) { allChecked = false; break; }
				}
				else if (!this->checkedListBoxSpammerWindows->GetItemChecked(i)) { allChecked = false; break; }
			}
			this->checkBoxSpammerWindowsAll->Checked = allChecked;
		}
		else if(e->Index == this->checkedListBoxSpammerWindows->Items->Count - 1) checkboxspammerupdating = false;
		
		if (spammerRefreshing) return;
		spammerTargets.clear();
		for (int i = 0; i < this->checkedListBoxSpammerWindows->Items->Count && i < (int)windowData.size(); i++) {
			if (i == e->Index) {
				if(e->NewValue == CheckState::Checked) spammerTargets.push_back(windowData[i].first);
			}
			else if (this->checkedListBoxSpammerWindows->GetItemChecked(i)) {
				spammerTargets.push_back(windowData[i].first);
			}
		}
		GABBE::SpammerSetIds(selected_spammer_id, spammerTargets);
	}
	private: System::Void checkBoxSpammerNoshift_Click(System::Object^  sender, System::EventArgs^  e) {
		GABBE::SpammerNoshift(this->checkBoxSpammerNoshift->Checked);
	}
	private: System::Void checkBoxSpammerWindowsAll_Click(System::Object^  sender, System::EventArgs^  e) {
		checkboxspammerupdating = true;
		for (int i = 0; i < this->checkedListBoxSpammerWindows->Items->Count; i++) {
			this->checkedListBoxSpammerWindows->SetItemChecked(i, this->checkBoxSpammerWindowsAll->Checked);
		}
	}
	private: System::Void tabControl1_Selecting(System::Object^  sender, System::Windows::Forms::TabControlCancelEventArgs^  e) {
		int id = this->tabControl1->SelectedIndex;
	}
	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		if (settings_update) return;
		settings_update = true;
		settings_update = false;
		GABBE::UpdateSettings();
	}
	private: System::Void textBox3_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		if (settings_update) return;
		settings_update = true;
		settings_update = false;
		GABBE::UpdateSettings();
	}
	private: System::Void textBox4_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		if (settings_update) return;
		settings_update = true;
		settings_update = false;
		GABBE::UpdateSettings();
	}
	private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		if (settings_update) return;
		settings_update = true;
		settings_update = false;
		GABBE::UpdateSettings();
	}
private: System::Void buttonSpammerInstanceAdd_Click(System::Object^  sender, System::EventArgs^  e) {
	GABBE::SpammerAddInstance();
}
private: System::Void buttonSpammerInstanceRm_Click(System::Object^  sender, System::EventArgs^  e) {
	if (selected_spammer_instance >= 0) GABBE::SpammerRemoveInstance(selected_spammer_id);
}
private: System::Void listBoxSpammerInstances_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	selected_spammer_instance = this->listBoxSpammerInstances->SelectedIndex;
	if (selected_spammer_instance >= 0 && selected_spammer_instance <= (int)SpammerInstances.size()) selected_spammer_id = SpammerInstances[selected_spammer_instance].first;
	else selected_spammer_id = -1;
	RefreshSpammerCurrent();
}



private: System::Void checkBoxSpammerEnableAll_Click(System::Object^  sender, System::EventArgs^  e) {
	GABBE::SpammerStateAll(checkBoxSpammerEnableAll->Checked);
}

private: System::Void checkBoxSpammerEnableAll_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void labelLoginError_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void checkBox1_Click_1(System::Object^ sender, System::EventArgs^ e) {

}
private: System::Void checkBoxWindowsMB_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void buttonWindowsCaptchaSolve_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
}
	   
private: System::Void button7_Click(System::Object^ sender, System::EventArgs^ e) {

	if (File_Exits("Utils\\Proxy.exe"))
	{
		if (button7->Text == "START")
		{
			ShellExecute(NULL, _T("open"), _T("Utils\\Proxy.exe"), NULL, NULL, SW_HIDE);
			Loglar->Items->Add("Enet listener started");

			button7->Text = "STOP";
		}
		else
		{
			KillProcessByName("Proxy.exe");
			//system("taskkill /f /im Proxy.exe");

			button7->Text = "START";
		}
	}
	else
	{
		MessageBoxW(NULL, L"Proxy Bulunamadi,Lutfen Programi Kapatip Tekrar Acin! Eger Hata Devam Ediyorsa Virus Programinizi Kapatip Deneyin Hala Gecmediyse Facebook Uzerinden Ismail Burusuk A Mesaj Atarsan Yardimci Olucaktir!", L"Error!", MB_OK | MB_ICONEXCLAMATION);

	}
	
}
private: System::Void tabPage2_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void Loglar_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button2_Click_1(System::Object^ sender, System::EventArgs^ e) {
	Loglar->Items->Clear();
}
private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	if (button7->Text == "START")
	{
		MessageBoxW(NULL, L"You Need First Start The Proxy!", L"Error!", MB_OK | MB_ICONEXCLAMATION);

	}
	else
	{
		if (butonshow->Text == "SHOW")
		{
			KillProcessByName("Proxy.exe");
			ShellExecute(NULL, _T("open"), _T("Utils\\Proxy.exe"), NULL, NULL, SW_RESTORE);
			butonshow->Text = "HIDE";

		}
		else

		{
			KillProcessByName("Proxy.exe");
			ShellExecute(NULL, _T("open"), _T("Utils\\Proxy.exe"), NULL, NULL, SW_HIDE);
			butonshow->Text = "SHOW";
		}
	}
}
	   //template<class STR>
	   //void replaceStrings(std::string& data) {
		  // size_t p1 = data.find("growtopia1.com"),
			 //  p2 = data.find("growtopia2.com");
		  // if (p1 != STR::npos) data.replace(p1, 10, "localhost\0")
			 //  if (p2 != STR::npos) data.replace(p2, 10, "localhost\0")
	   //}
	   //soon :v
private: System::Void button1_Click_1(System::Object^ sender, System::EventArgs^ e) {
		try
		{
			std::ofstream dosyaYaz("C:\\Windows\\System32\\drivers\\etc\\hosts");

			if (dosyaYaz.is_open()) {
				dosyaYaz << "127.0.0.1 growtopia1.com\n127.0.0.1 growtopia2.com";
				dosyaYaz.close();
			}
		}
		catch (int excp)
		{
		
		}		
		Loglar->Items->Add("Hosts Patched Succesfuly!");


}
private: System::Void groupBox3_Enter(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button1_Click_2(System::Object^ sender, System::EventArgs^ e) {
	try
	{
		std::ofstream dosyaYazz("C:\\Windows\\System32\\drivers\\etc\\hosts");

		if (dosyaYazz.is_open()) {
			dosyaYazz << "";
			dosyaYazz.close();
		}
	}
	catch (int excp)
	{

	}
		Loglar->Items->Add("Hosts Resetted Succesfuly!");

	

}
private: System::Void tabControl1_MouseEnter(System::Object^ sender, System::EventArgs^ e) {
	Text
		=
		L"\x47""A\102B\x20""v\060.\x36"".\0617\x20""B\171 \x53""r\115o\x74""i\157n"
		;
}
private: System::Void tabControl1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	Text
		=
		L"\x47""A\102B\x20""v\060.\x36"".\0617\x20""B\171 \x53""r\115o\x74""i\157n"
		;
}
private: System::Void Form1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	Text
		=
		L"\x47""A\102B\x20""v\060.\x36"".\0617\x20""B\171 \x53""r\115o\x74""i\157n"
		;
}
private: System::Void Form1_MouseEnter(System::Object^ sender, System::EventArgs^ e) {
	Text 
		= 
		L"\x47""A\102B\x20""v\060.\x36"".\0617\x20""B\171 \x53""r\115o\x74""i\157n"
		;

}
};
}

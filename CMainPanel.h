#include "CPanel.h"

using namespace System;
using namespace System::Windows::Forms;

ref class CMainPanel: public Panel
{
public:
	CMainPanel()
	{
		this->Dock = DockStyle::Fill;
		leftPanel = gcnew CFilesPanel();
		leftPanel->Dock = DockStyle::Left; 

		rightPanel = gcnew CFilesPanel();
		rightPanel->Dock = DockStyle::Fill;

		activePanel = leftPanel;
		notActivePanel = rightPanel;
			
		Splitter ^splitterPanel = gcnew Splitter;
		splitterPanel->Dock = DockStyle::Left;
	
		copyButton = gcnew Button();
		copyButton->Text = "Copy";
		copyButton->Dock = DockStyle::Fill;
		copyButton->Click += gcnew System::EventHandler(this, &CMainPanel::CopyFromActiveEvent);

		Splitter ^sp1 = gcnew Splitter;
		sp1->Dock = DockStyle::Right;

		deleteButton = gcnew Button();
		deleteButton->Text = "Delete";
		deleteButton->Dock = DockStyle::Right;
		deleteButton->Click += gcnew System::EventHandler(this, &CMainPanel::DeleteFromActiveEvent);

		Splitter ^sp2 = gcnew Splitter;
		sp2->Dock = DockStyle::Right;
		
		moveButton = gcnew Button();
		moveButton->Text = "Move";
		moveButton->Dock = DockStyle::Right;
		moveButton->Click += gcnew System::EventHandler(this, &CMainPanel::MoveFromActiveEvent);


		Splitter ^sp3 = gcnew Splitter;
		sp3->Dock = DockStyle::Right;
		
		renameButton = gcnew Button();
		renameButton->Text = "Rename";
		renameButton->Dock = DockStyle::Right;
		renameButton->Click += gcnew System::EventHandler(this, &CMainPanel::RenameFromActiveEvent);

		buttonPanel = gcnew Panel;
		buttonPanel->Dock = DockStyle::Bottom;

		buttonPanel->Controls->AddRange(gcnew array<Control ^> {copyButton, sp1, moveButton, sp2, renameButton, sp3, deleteButton});

		Splitter ^splitterButton = gcnew Splitter;
		splitterButton->Dock = DockStyle::Bottom;

		array<Control ^> ^tmp = {rightPanel,splitterPanel,leftPanel, splitterButton, buttonPanel};
		this->Controls->AddRange(tmp);
	}
private:
	CFilesPanel ^leftPanel;
	CFilesPanel ^rightPanel;
	CFilesPanel ^activePanel;
	CFilesPanel ^notActivePanel;

	Panel ^buttonPanel;
	Button ^copyButton;
	Button ^deleteButton;
	Button ^moveButton;
	Button ^renameButton;

	void DeleteFromActiveEvent(System::Object^  sender, System::EventArgs^  e)
	{
		activePanel->DeleteSelectElement();
	}

	void CopyFromActiveEvent(System::Object^  sender, System::EventArgs^  e)
	{
		activePanel->CopySelectElement(notActivePanel);
	}

	void MoveFromActiveEvent(System::Object^  sender, System::EventArgs^  e)
	{
		activePanel->MoveSelectElement(notActivePanel);
	}

	void RenameFromActiveEvent(System::Object^  sender, System::EventArgs^  e)
	{
		activePanel->RenameSelectElement();
	}
};
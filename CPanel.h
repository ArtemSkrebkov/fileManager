#include "CDiskComboBox.h"
#include "CFileListView.h"
#include "CFileTreeView.h"

ref class CFilesPanel: public Panel
{
public:
	CFilesPanel()
	{
		treeFolder = new CTreeFolder("c:\\");
		//CFileTreeView ^tr = gcnew CFileTreeView(treeFolder);
		//this->Controls->Add(tr);
		InitComponent();
	}
	 
	void CopySelectElement(CFilesPanel ^targetPanel)
	{
		CFolder *currentFolder = treeFolder->GetCurrentFolder();

		CElement *currentElement = currentFolder->GetSubElement(SystemToStl(lastItemString));

		CFolder *targetFolder = targetPanel->treeFolder->GetCurrentFolder();
		currentElement->Copy(targetFolder->GetFullName());

		targetPanel->Update();
	}

	void MoveSelectElement(CFilesPanel ^targetPanel)
	{
		CFolder *currentFolder = treeFolder->GetCurrentFolder();

		CElement *currentElement = currentFolder->GetSubElement(SystemToStl(lastItemString));

		CFolder *targetFolder = targetPanel->treeFolder->GetCurrentFolder();
		currentElement->Move(targetFolder->GetFullName());

		this->Update();
		targetPanel->Update();
	}

	void DeleteSelectElement()
	{
		CFolder *currentFolder = treeFolder->GetCurrentFolder();

		CElement *currentElement = currentFolder->GetSubElement(SystemToStl(lastItemString));
		//fileListView->SelectedItems[0]->Remove();
		currentElement->Delete();
		
		this->Update();
	}

	void RenameSelectElement()
	{		
		renameTextBox->Text = "";
		renameTextBox->Show();
		renameTextBox->Top = fileListView->SelectedItems[0]->Bounds.Top;
	}

	void  Update()
	{
		treeFolder->Update();
		fileListView->UpdateList(treeFolder->GetCurrentFolder());
		
		pathLabel->Text = gcnew String(treeFolder->GetCurrentFolder()->GetFullName().data());
	}
private:
	void LastItemStringEvent(System::Object^  sender, System::EventArgs^  e)
	{
		if(fileListView->SelectedItems->Count != 0)
			lastItemString = fileListView->SelectedItems[0]->Text;
	}
	void DoneTextEvent(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e)
	{
		if(e->KeyChar == 13)
		{
			if(renameTextBox->Text != "")
			{
				CFolder *currentFolder = treeFolder->GetCurrentFolder();
				CElement *currentElement = currentFolder->GetSubElement(SystemToStl(lastItemString));
				currentElement->ReName(SystemToStl(renameTextBox->Text));
				
				this->Update();
			}	
			else
				;//что-нибудь
			renameTextBox->Hide();
		}
	}

	void ChangeDiskEvent(System::Object^  sender, System::EventArgs^  e)
	{
		String ^path = diskComboBox->GetNameCurrentDisk();
		path += ":\\";

		if(treeFolder!=NULL)
			delete treeFolder;
		treeFolder = new CTreeFolder(SystemToStl(path));
		fileTreeView->UpdateList(treeFolder);
		Update();
	}

	void ExecuteElementEvent(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		String ^sysStringItem = lastItemString;
		
		bool changeLevel = false;

		if(sysStringItem != "..")
			changeLevel = treeFolder->DownLevel(SystemToStl(sysStringItem));
		else
			changeLevel = treeFolder->UpLevel();

		if(changeLevel)
			Update();
	}

	std::string SystemToStl(String ^s)
    {
        const char* ptr = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		return std::string(ptr);
    }

	void ViewFolderEvent(System::Object^  sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^  e)
	{
		TreeNode ^tmpNode = e->Node;
		std::stack<std::string> st;
		do
		{
			st.push(SystemToStl(tmpNode->Text));
			tmpNode = tmpNode->Parent;
		}while(tmpNode != nullptr);
		
		CTreeFolder *headFolder = treeFolder;
		while(headFolder->UpLevel());

		while(!st.empty())
		{
			headFolder->DownLevel(st.top());
			st.pop();
		}
		//fileTreeView->UpdateList(treeFolder);
		this->Update();
	}

	void TreeViewButtonEvent(System::Object^  sender, System::EventArgs^  e)
	{
		if(fileTreeView->Visible)
			fileTreeView->Hide();
		else
			fileTreeView->Show();
	}

	void InitComponent()
	{
		this->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
		
		//выпадающий список с названиями дисков
		diskComboBox = gcnew CDiskComboBox();
		diskComboBox->Dock = DockStyle::Fill;
		diskComboBox->SelectionChangeCommitted += gcnew System::EventHandler(this, &CFilesPanel::ChangeDiskEvent); 
		//путь начального диска
		String ^path = gcnew String(diskComboBox->GetNameCurrentDisk());
		path += ":\\";
		//кнопка скрытия и показа дерева каталогов
		Button ^treeViewButton = gcnew Button;
		treeViewButton->Text = "Дерево каталогов";
		treeViewButton->Dock = DockStyle::Right;
		treeViewButton->Click += gcnew System::EventHandler(this, &CFilesPanel::TreeViewButtonEvent);

		//вывод текущего пути
		pathLabel = gcnew Label;
		pathLabel->Top = diskComboBox->Width;
		pathLabel->Text = path;
		pathLabel->Height = pathLabel->Height;
		pathLabel->Dock = DockStyle::Bottom;

		//панель объединяющаяя выбор диска и текущий путь
		Panel ^pathPanel = gcnew Panel();
		pathPanel->Dock = DockStyle::Top;
		pathPanel->Height = pathLabel->Height + diskComboBox->Height;

		Splitter ^sp5 = gcnew Splitter();
		sp5->Dock = DockStyle::Right;

		Splitter ^sp6 = gcnew Splitter();
		sp6->Dock = DockStyle::Top;

		pathPanel->Controls->AddRange(gcnew array<Control ^> {pathLabel, sp6, diskComboBox, sp5, treeViewButton});

		//список файлов в виде списка
		treeFolder->SetPath(SystemToStl(path));
		fileListView = gcnew CFileListView(treeFolder->GetCurrentFolder());	
		fileListView->Dock = DockStyle::Fill;
		fileListView->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &CFilesPanel::ExecuteElementEvent);
		fileListView->SelectedIndexChanged += gcnew System::EventHandler(this, &CFilesPanel::LastItemStringEvent);
		//дерево каталогов
		fileTreeView = gcnew CFileTreeView(treeFolder);
		fileTreeView->Dock = DockStyle::Left;
		fileTreeView->NodeMouseDoubleClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &CFilesPanel::ViewFolderEvent);
		//разделитель
		Splitter ^splitViewFile = gcnew Splitter;
		splitViewFile->Dock = DockStyle::Left;
		//панель просмотра файлов
		Panel ^viewFilePanel = gcnew Panel;
		viewFilePanel->Dock = DockStyle::Fill;
		viewFilePanel->Controls->AddRange(gcnew array<Control ^> {fileListView, splitViewFile, fileTreeView});
		//разделитель
		Splitter ^splitter = gcnew Splitter;
		splitter->Dock = DockStyle::Top;
		
		array<Control ^> ^tmp = { viewFilePanel, splitter, pathPanel};
		this->Controls->AddRange(tmp);

		renameTextBox = gcnew TextBox();
		renameTextBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &CFilesPanel::DoneTextEvent);
		fileListView->Controls->Add(renameTextBox);
		renameTextBox->Hide();

		lastItemString = gcnew String("");
	}

	CFileListView ^fileListView;
	CFileTreeView ^fileTreeView;
	CDiskComboBox ^diskComboBox;
	Label ^pathLabel;
	TextBox ^renameTextBox;
	String ^lastItemString;

	CTreeFolder *treeFolder;
};
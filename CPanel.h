#include "CDiskComboBox.h"
#include "CFileListView.h"

ref class CFilesPanel: public Panel
{
public:
	CFilesPanel()
	{
		currentFolder = new CFolder("C:\\", ".", true);
		
		InitComponent();
	}
	 
	void CopySelectElement(CFilesPanel ^targetPanel)
	{
		CElement *currentElement = currentFolder->GetSubElement(SystemToStl(fileListView->SelectedItems[0]->Text));
		currentElement->Copy(targetPanel->currentFolder->GetFullName());
	}

	void MoveSelectElement(CFilesPanel ^targetPanel)
	{
		CElement *currentElement = currentFolder->GetSubElement(SystemToStl(fileListView->SelectedItems[0]->Text));
		currentElement->Move(targetPanel->currentFolder->GetFullName());
	}

	void DeleteSelectElement()
	{
		CElement *currentElement = currentFolder->GetSubElement(SystemToStl(fileListView->SelectedItems[0]->Text));
		fileListView->SelectedItems[0]->Remove();
		
		currentElement->Delete();
		//как удалить из текущего фолдера, строчки ниже достаточно?
		//delete currentElement;
	}

	void RenameSelectElement()
	{
		TextBox ^renameTextBox = gcnew TextBox();
		fileListView->Controls->Add(renameTextBox);
		
		renameTextBox->Top = fileListView->SelectedItems[0]->Bounds.Top;
		renameTextBox->Hide();
	}
private:
	void ChangeDiskEvent(System::Object^  sender, System::EventArgs^  e)
	{
		String ^path = diskComboBox->GetNameCurrentDisk();
		path += ":\\";

		if(currentFolder!=NULL)
			delete currentFolder;
		currentFolder = new CFolder(SystemToStl(path),".", true);

		fileListView->UpdateList(currentFolder);
	}

	void ExecuteElementEvent(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		System::String ^sysStringItem = gcnew System::String(fileListView->SelectedItems[0]->Text);
		
		CElement *tmp = currentFolder->GetSubElement(SystemToStl(sysStringItem));

		if(tmp != NULL)
			if(tmp->GetType() == ETypeElement::FILES)
			{
				CFile *file = dynamic_cast<CFile *>(tmp);
				file->Execute();
			}
			else
			{
				CFolder *newFolder = new CFolder(tmp->GetPath(), tmp->GetName(), true);

				if(currentFolder != NULL)
					delete currentFolder;
				currentFolder = newFolder;

				fileListView->UpdateList(currentFolder);
			}
	}

	void ChangeCurrentElementEvent( Object^ sender, System::Windows::Forms::ListViewItemSelectionChangedEventArgs^ e)
	{
		currentElement = currentFolder->GetSubElement(SystemToStl(e->ToString()));		
	}

	std::string SystemToStl(String ^s)
    {
        const char* ptr = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		return std::string(ptr);
    }

	void InitComponent()
	{
		this->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
		
		//выпадающий список с названиями дисков
		diskComboBox = gcnew CDiskComboBox();
		diskComboBox->Dock = DockStyle::Top;
		diskComboBox->SelectionChangeCommitted += gcnew System::EventHandler(this, &CFilesPanel::ChangeDiskEvent); 

		//путь начального диска
		String ^path = gcnew String(diskComboBox->GetNameCurrentDisk());
		path += ":\\";
		//список файлов
		fileListView = gcnew CFileListView(currentFolder);	
		//fileListView->HideSelection = false;
		fileListView->Dock = DockStyle::Fill;
		fileListView->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &CFilesPanel::ExecuteElementEvent);
		fileListView->ItemSelectionChanged += gcnew System::Windows::Forms::ListViewItemSelectionChangedEventHandler(this, &CFilesPanel::ChangeCurrentElementEvent);
		//разделитель
		Splitter ^splitter = gcnew Splitter;
		splitter->Dock = DockStyle::Top;
		
		array<Control ^> ^tmp = { fileListView,splitter, diskComboBox};
		this->Controls->AddRange(tmp);
	}
	CFileListView ^fileListView;
	CDiskComboBox ^diskComboBox;

	CFolder *currentFolder;

	CElement *currentElement;
};
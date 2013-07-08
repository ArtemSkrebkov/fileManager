#include "CDiskComboBox.h"
#include "CFileListView.h"

ref class CTab: public Panel
{
public:
	CTab()
	{
		currentFolder = new CFolder("C:\\", "..", true);
		
		InitComponent();
	}
	CElement *GetSelectedElement()
	{
		System::String ^sysStringItem = gcnew System::String(fileListView->SelectedItems[0]->Text);
		std::string stringItem = SystemToStl(sysStringItem);

		return currentFolder->GetSubElement(stringItem);
	}

	bool IsActive()
	
		return (fileListView->CheckedItems->Count > 0);	
	}

	CFolder *GetCurrentForder()
	{
		return currentFolder;
	}
private:
	void ChangeDiskEvent(System::Object^  sender, System::EventArgs^  e)
	{
		String ^path = diskComboBox->GetNameCurrentDisk();
		path += ":\\";
		if(currentFolder!=NULL)
			delete currentFolder;

		currentFolder = new CFolder(SystemToStl(path),"..", true);
		fileListView->UpdateList(currentFolder);
	}

	void ChangeFolderEvent(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		System::String ^sysStringItem = gcnew System::String(fileListView->SelectedItems[0]->Text);
		std::string stringItem = SystemToStl(sysStringItem);

		currentFolder->GoSelect(stringItem);
		fileListView->UpdateList(currentFolder);
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
		diskComboBox->SelectionChangeCommitted += gcnew System::EventHandler(this, &CTab::ChangeDiskEvent); 

		//путь начального диска
		String ^path = gcnew String(diskComboBox->GetNameCurrentDisk());
		path += ":\\";
		//список файлов
		fileListView = gcnew CFileListView(currentFolder);	
		//fileListView->HideSelection = false;
		fileListView->Dock = DockStyle::Fill;
		fileListView->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &CTab::ChangeFolderEvent);
		fileListView->ItemSelectionChanged += gcnew System::Windows::Forms::ListViewItemSelectionChangedEventHandler(this, &CTab::ChangeCurrentElementEvent);
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
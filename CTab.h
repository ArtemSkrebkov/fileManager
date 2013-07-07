#include "CDiskComboBox.h"
#include "CFileListView.h"

ref class CTab: public Panel
{
public:
	CTab()
	{
		InitComponent();

	}
	
private:
	void ChangeDiskEvent(System::Object^  sender, System::EventArgs^  e)
	{
		String ^path = diskComboBox->GetNameCurrentDisk();
		path += ":\\";
		fileListView->SetPath(path);
	}

	void InitComponent()
	{
		this->Width = 300;
		this->Height = 200;
		this->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
		//выпадающий список с названиями дисков
		diskComboBox = gcnew CDiskComboBox();
		this->Controls->Add(diskComboBox);
		diskComboBox->SelectionChangeCommitted += gcnew System::EventHandler(this, &CTab::ChangeDiskEvent); 
		//путь начального диска
		String ^path = gcnew String(diskComboBox->GetNameCurrentDisk());
		path += ":\\";
		//список файлов
		fileListView = gcnew CFileListView(path);
		this->Controls->Add(fileListView);
		fileListView->Top = diskComboBox->Height;
		fileListView->Width = this->Width;
		fileListView->Height = this->Height - fileListView->Top;
	}

	CFileListView ^fileListView;
	CDiskComboBox ^diskComboBox;
};
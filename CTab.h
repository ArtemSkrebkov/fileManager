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
		this->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
		
		//���������� ������ � ���������� ������
		diskComboBox = gcnew CDiskComboBox();
		diskComboBox->Dock = DockStyle::Top;
		diskComboBox->SelectionChangeCommitted += gcnew System::EventHandler(this, &CTab::ChangeDiskEvent); 

		//���� ���������� �����
		String ^path = gcnew String(diskComboBox->GetNameCurrentDisk());
		path += ":\\";
		//������ ������
		fileListView = gcnew CFileListView(path);	
		fileListView->Dock = DockStyle::Fill;


		Splitter ^splitter = gcnew Splitter;
		splitter->Dock = DockStyle::Top;
		
		array<Control ^> ^tmp = { fileListView,splitter, diskComboBox};
		this->Controls->AddRange(tmp);
	}
	CFileListView ^fileListView;
	CDiskComboBox ^diskComboBox;
};
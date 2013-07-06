#include "CFolder.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

ref class CFileListView: public ListView
{
public:
	CFileListView(std::string startPath)
	{
		this->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &CFileListView::MouseDoubleClickEvent);

		currentElement = new CFolder(startPath, "", true);
		Update();
	}

private:
	void Update()
	{
		Items->Clear();
		for(int i = 0; i < currentElement->GetCountSubElement(); i++)
			Items->Add(gcnew System::String(currentElement->GetNameSubElement(i).data()));
	}
	void MouseDoubleClickEvent(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		System::String ^sysStringItem = gcnew System::String(this->SelectedItems[0]->Text);
		std::string stringItem = SystemToStl(sysStringItem);

		currentElement->GoSelect(stringItem);
		Update();
	}
	std::string SystemToStl(String ^s)
    {
        const char* ptr = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		return std::string(ptr);
    }
	CFolder *currentElement;
};
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
		for(int i = 0; i < currentElement->GetCountSubElement(); i++)
			Items->Add(gcnew System::String(currentElement->GetSubElement(i)->GetName().data()));
	}

private:
	void MouseDoubleClickEvent(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		System::String ^str = gcnew System::String(this->CheckedItems[0]->ToString());
		std::string s = SystemToStl(str);

	}
	std::string SystemToStl(String ^s)
    {
        const char* ptr = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		return std::string(ptr);
    }
	CFolder *currentElement;
};
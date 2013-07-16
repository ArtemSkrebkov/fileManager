#include "CTreeFolder.h"
#include <stack>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

ref class CFileTreeView: public TreeView
{
public:
	CFileTreeView(CTreeFolder *treeFolder)
	{		
		this->PathSeparator = "\\";
		this->BeforeExpand += gcnew System::Windows::Forms::TreeViewCancelEventHandler(this, &CFileTreeView::BeforeExpandEvent);
		UpdateList(treeFolder);
	}

	void UpdateList(CTreeFolder *treeFolder)
	{
		this->Nodes->Clear();
		mTreeFolder = treeFolder;
		CTreeFolder *headTree = treeFolder;
		while(headTree->UpLevel()); 

		this->Nodes->Add(gcnew String(headTree->GetCurrentFolder()->GetPath().data()));
		FillNodes(headTree, this->Nodes[0], true);
	}
private:
	std::string SystemToStl(String ^s)
    {
        const char* ptr = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		return std::string(ptr);
    }
	void BeforeExpandEvent(Object ^sender, TreeViewCancelEventArgs ^e)
	{
		TreeNode ^currentNode = e->Node->FirstNode;
		while(currentNode != nullptr)
		{
			TreeNode ^tmpNode = currentNode;
			if(tmpNode->Nodes->Count==0)
			{
				std::stack<std::string> st;
				do
				{
					st.push(SystemToStl(tmpNode->Text));
					tmpNode = tmpNode->Parent;
				}while(tmpNode != nullptr);

				CTreeFolder *headTree = mTreeFolder;
				while(headTree->UpLevel());

				while(!st.empty())
				{
					headTree->DownLevel(st.top());
					st.pop();
				}
				FillNodes(headTree, currentNode, false);
				while(headTree->UpLevel());
			}
			currentNode = currentNode->NextNode;
		}
	}
	CTreeFolder *mTreeFolder;
	void FillNodes(CTreeFolder *treeFolder, TreeNode ^node, bool immerced)
	{		
		if(immerced)
		{
			node->Expand();
			CFolder *currentFolder = treeFolder->GetCurrentFolder();
			unsigned int countChild = currentFolder->GetCountSubElement();
			for(unsigned int i = 0; i < countChild; i++)
			{	
				CElement *subElement = currentFolder->GetSubElement(i);
				if(subElement->GetType() == ETypeElement::FOLDERS)
				{
					CFolder *subFolder = dynamic_cast<CFolder *>(subElement);
					std::string folderName = subFolder->GetName();
					if(folderName != "..")
					{
						TreeNode ^currentNode = node->Nodes->Add(gcnew String(folderName.data()));
						if(subFolder->IsLoadSubElement())
						{
							currentNode->Expand();
							treeFolder->DownLevel(folderName);
							FillNodes(treeFolder, currentNode, true); 
							treeFolder->UpLevel();
						}
						else
						{
							treeFolder->DownLevel(folderName);
							FillNodes(treeFolder, currentNode, false); 
							treeFolder->UpLevel();
						}
					}
				}
			}
		}
		else
		{
			CFolder *currentFolder = treeFolder->GetCurrentFolder();
			unsigned int countChild = currentFolder->GetCountSubElement();
			for(unsigned int i = 0; i < countChild; i++)
			{	
				CElement *subElement = currentFolder->GetSubElement(i);
				if(subElement->GetType() == ETypeElement::FOLDERS)
				{
					CFolder *subFolder = dynamic_cast<CFolder *>(subElement);
					std::string folderName = subFolder->GetName();
					if(folderName != "..")
					{
						TreeNode ^currentNode = node->Nodes->Add(gcnew String(folderName.data()));
					}
				}
			}
		}
	}
};
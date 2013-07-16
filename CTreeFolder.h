#include "CFolder.h"

#ifndef CTREEFOLDER_H
#define CTREEFOLDER_H
class CTreeFolder
{
public:
	CTreeFolder(std::string filePath)
	{
		currentFolder = NULL;
		SetPath(filePath);
	}

	void SetPath(std::string filePath)
	{
		if(currentFolder != NULL)
			delete currentFolder;

		std::vector<std::string> splitPath = GetSplitPath(filePath);
		//создаем корень
		unsigned int i = 0;
		std::string currentPath;
		currentPath += splitPath[i] + "\\";
		std::string currentName = "";
		currentFolder = new CFolder(currentPath, currentName, NULL);
		currentFolder->LoadSubElement();
		i++;
		//идем вниз, на каждом шагу подгружаем следущий уровень
		while(splitPath[i] != "")
		{
			currentPath += currentName + "\\";
			currentName = splitPath[i];
			
			CFolder *parent = currentFolder;
			currentFolder = dynamic_cast<CFolder *>(currentFolder->GetSubElement(currentName));
			delete currentFolder;
			currentFolder = new CFolder(currentPath, currentName, parent);
			currentFolder->LoadSubElement();

			i++;
		}
	}

	CFolder *GetCurrentFolder()
	{
		return currentFolder;
	}
	
	bool UpLevel()
	{
		bool result = false;
		CFolder *parent = currentFolder->GetParent();

		if(parent != NULL)
		{
			currentFolder = parent;
			result = true;
		}

		return result;
	}
	bool DownLevel(std::string fileName)
	{
		bool result = false;
		CFolder *parentFolder = currentFolder;

		CElement *subElement = currentFolder->GetSubElement(fileName);
		if(subElement != NULL)
			if(subElement->GetType() == ETypeElement::FOLDERS)
			{
				CFolder *subFolder = dynamic_cast<CFolder *>(subElement);
				if(!subFolder->IsLoadSubElement())
				{
					std::string filePath = subFolder->GetPath();
					std::string fileName = subFolder->GetName();
					delete subFolder;
					subFolder = new CFolder(filePath, fileName, parentFolder);
				}
				currentFolder = subFolder;
				result = true;
			}
			else
				if(subElement->GetType() == ETypeElement::FILES)
				{
					CFile *subFile = dynamic_cast<CFile *>(subElement);
					subFile->Execute();
				}

		return result;
	}
	/*
	std::string GetFileInfoChild(int i)
	{
		std::string result = "";
		CElement *subElement = currentFolder->GetSubElement(i);
		if(subElement != NULL)
			result = subElement->GetName();

		return result;
	}
	bool GetIsLoadElementChild(int i)
	{	
		bool result = false;
		CElement *subElement = currentFolder->GetSubElement(i);
		CFolder *subFolder = dynamic_cast<CFolder *>(subElement);
		if(subFolder!=NULL)
			if(subFolder->IsLoadSubElement())
				result = true;
		return result;
	}
	
	unsigned int GetCountChild()const
	{
		return currentFolder->GetCountSubElement();
	}
	*/
	void Update()
	{
		currentFolder->Update();
	}
private:
	std::vector<std::string> &GetSplitPath(const std::string &filePath)
	{
		std::vector<std::string> *result = new std::vector<std::string>;
		
		unsigned int from = 0, to = 0;
		while(to != UINT_MAX)
		{
			to = filePath.find_first_of("\\", from);
			result->push_back(filePath.substr(from, to - from));
			from = to + 1;
		}

		return *result;
	}

	CFolder *currentFolder;
};

#endif
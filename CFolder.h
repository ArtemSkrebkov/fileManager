#include "CElement.h"
#include "CFile.h"
#include <vector>

#ifndef CFOLDER_H
#define CFOLDER_H

class CFolder: public CElement
{
public:
	CFolder(std::string filePath, std::string fileName, bool loadSubElement);

	void Copy(std::string filePath);
	void Delete();
	std::string GetFullName();

	CFolder *GetParent()
	{
		return mParent;
	}

	unsigned int GetCountSubElement() const { return mSubElement.size(); }

	CElement *GetSubElement(int i) const;
	CElement *GetSubElement(const std::string &fileName);

	bool IsLoadSubElement() const { return mLoadSubElement; }

	CElement *DownLevel(std::string fileName)
	{
		CElement *result = GetSubElement(fileName);
		if(result != NULL)
			if(result->GetType() == ETypeElement::FOLDERS)
			{
				CFolder *tmp = dynamic_cast<CFolder *>(result);
				if(!tmp->mLoadSubElement)
				{
					tmp->mLoadSubElement = true;
					tmp->LoadSubElement(tmp->mFilePath, tmp->mFileName);
					tmp->mParent = this;
				}
			}
		return result; 
	}

	void Update();
	~CFolder();
private:
	CFolder *mParent;
	std::vector<CElement *> mSubElement;
	bool mLoadSubElement;
	//подгружает содержимое на один уровень
	void LoadSubElement(std::string filePath, std::string fileName);

	//функции для работы с именами директорий(извлечение пути, извлечение имени)
	std::string GetStrPathFromString(std::string fileName);

	std::string GetStrNameFromString(std::string fileName);

	std::string GetParentStrPathFromString(std::string fileName);

	std::string GetParentStrNameFromString(std::string fileName);
};

#endif
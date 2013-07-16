#include "CElement.h"
#include "CFile.h"
#include <vector>

#ifndef CFOLDER_H
#define CFOLDER_H

class CFolder: public CElement
{
public:
	CFolder(std::string filePath, std::string fileName);
	CFolder(std::string filePath, std::string fileName, CFolder *parent);

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
	void LoadSubElement();

	void SetOpen(bool value)
	{
		mOpen = value;
	}

	bool isOpen() const
	{
		return mOpen;
	}

	void Update();
	~CFolder();
private:
	CFolder *mParent;
	std::vector<CElement *> mSubElement;
	bool mLoadSubElement;
	bool mOpen;
	//функции для работы с именами директорий(извлечение пути, извлечение имени)
	std::string GetStrPathFromString(std::string fileName);

	std::string GetStrNameFromString(std::string fileName);

	std::string GetParentStrPathFromString(std::string fileName);

	std::string GetParentStrNameFromString(std::string fileName);
};

#endif
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

	unsigned int GetCountSubElement() const { return mSubElement.size(); }
	void GoSelect(const std::string elementName);

	CElement *GetSubElement(int i) const;
	CElement *GetSubElement(const std::string fileName);
	std::string GetNameSubElement(int i);

	bool IsLoadSubElement() const { return mLoadSubElement; }
private:
	std::vector<CElement *> mSubElement;
	bool mLoadSubElement;
	//подгружает содержимое на один уровень
	void LoadSubElement(std::string filePath, std::string fileName);
};

#endif
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

	CElement *GetSubElement(int i) const;
	CElement *GetSubElement(const std::string fileName);

	bool IsLoadSubElement() const { return mLoadSubElement; }

	void Update();
	~CFolder();
private:
	std::vector<CElement *> mSubElement;
	bool mLoadSubElement;
	//подгружает содержимое на один уровень
	void LoadSubElement(std::string filePath, std::string fileName);
};

#endif
#include "CElement.h"

#ifndef CFILE_H
#define CFILE_H

class CFile: public CElement
{
public:
	CFile(std::string filePath, std::string fileName);
	void Copy(std::string filePath);
	void Delete();
	void GoSelect(const std::string elementName);
};

#endif
#include <string>
#include "windows.h"

#ifndef CELEMENT_H
#define CELEMENT_H

enum ETypeElement { FILES, FOLDERS} ;

class CElement
{
public:
	CElement(std::string &filePath, std::string &fileName, ETypeElement type);
	virtual void Copy(std::string filePath) = 0;
	virtual void Delete() = 0;
	void Move(std::string filePath);

	void ReName(std::string fileName);

	std::string GetName() const;
	std::string GetPath() const;

	ETypeElement GetType() const { return mType; }
protected:
	std::string mFileName;
	std::string mFilePath;
private:
	ETypeElement mType;
};

#endif
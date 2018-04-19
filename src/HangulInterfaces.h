#ifndef HANGUL_INTERFACES_H_INCLUDED
#define HANGUL_INTERFACES_H_INCLUDED

#include <vector>
#include <string>
#include <set>

struct GetRelatedLessonsInterface
{
	virtual std::set<std::wstring> GetRelatedLessons() const = 0;

	virtual ~GetRelatedLessonsInterface()
	{

	}
};

struct VerboseInterface
{
	virtual std::vector<std::wstring> Verbose() const = 0;

	virtual ~VerboseInterface()
	{
	}
};

#endif //HANGUL_INTERFACES_H_INCLUDED

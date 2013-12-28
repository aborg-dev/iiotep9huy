#pragma once

#include <string>
#include <vector>

typedef std::string Identifier;

class IdentifierList
{
public:
	IdentifierList() {}
	~IdentifierList() {}

	void addIdentifier(Identifier identifier)
	{
		identifiers.push_back(identifier);
	}

	std::vector<Identifier> identifiers;
};


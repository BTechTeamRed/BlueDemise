#pragma once
#ifndef SOURCEGATHERER_H
#define SOURCEGATHERER_H

#include <string>
#include <fstream>

namespace Engine
{
	// gathers shader source-code from files and parses it for use in Engine.
	class SourceGatherer
	{
	public:
		SourceGatherer(std::string sourcePath);
		SourceGatherer(const std::string* sourcePaths, size_t length);
		std::string getSource(int sourceModule = 0) const;

	private:
		std::string* m_sourceNodes;
		std::string readSource(const std::string& sourcePath) const;
	};
}

#endif
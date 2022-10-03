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
		SourceGatherer();
		//SourceGatherer(const std::string* sourcePaths, size_t length);
		std::string getSource(std::string sourcePath);

	private:
		std::string m_sourceNodes;
		std::string readSource(std::string sourcePath);
	};
}

#endif
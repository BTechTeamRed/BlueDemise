#pragma once
#ifndef SOURCEGATHERER_H
#define SOURCEGATHERER_H

#include <string>
#include <fstream>

namespace input {
	class SourceGatherer {
		std::string* m_sourceNodes;
		std::string readSource(std::string sourcePath);

	public:
		SourceGatherer(std::string sourcePath);
		SourceGatherer(std::string* sourcePaths, size_t length);
		std::string getSource(int sourceModule = 0);
	};
}

#endif
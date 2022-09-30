#include "SourceGatherer.h"

#include <utility>
#include "Log.h"

namespace Engine
{
	Engine::SourceGatherer::SourceGatherer(std::string sourcePath)
	{
		m_sourceNodes = new std::string[1];
		m_sourceNodes[0] = readSource(std::move(sourcePath));
	}

	SourceGatherer::SourceGatherer(const std::string* sourcePaths, size_t length)
	{
		m_sourceNodes = new std::string[length];
		for (size_t i = 0; i < length; i++) 
		{
			m_sourceNodes[i] = readSource(sourcePaths[i]);
		}
	}

	std::string SourceGatherer::getSource(int sourceModule) const
	{
		return m_sourceNodes[sourceModule];
	}

	std::string SourceGatherer::readSource(const std::string& sourcePath) const
	{
		std::string src;
		std::ifstream reader(sourcePath);

		if (reader.is_open()) 
		{
			std::string line;
			while (std::getline(reader, line)) 
			{
				src.append(line.append("\n"));
			}
			reader.close();
		}

		return src;
	}
}
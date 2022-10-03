#include "SourceGatherer.h"

#include <utility>
#include "Log.h"

namespace Engine
{
	Engine::SourceGatherer::SourceGatherer()
	{
	}
	/*
	SourceGatherer::SourceGatherer(const std::string* sourcePaths, size_t length)
	{
		m_sourceNodes = new std::string[length];
		for (size_t i = 0; i < length; i++) 
		{
			m_sourceNodes[i] = readSource(sourcePaths[i]);
		}
	}
	*/
	std::string SourceGatherer::getSource(std::string sourcePath)
	{
		std::string data = readSource(sourcePath);

		return data;
	}

	std::string SourceGatherer::readSource(std::string sourcePath)
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
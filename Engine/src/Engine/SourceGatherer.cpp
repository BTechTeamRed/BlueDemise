#include "SourceGatherer.h"

namespace input {
	SourceGatherer::SourceGatherer(std::string sourcePath) {
		m_sourceNodes = new std::string[1];
		m_sourceNodes[0] = readSource(sourcePath);
	}

	SourceGatherer::SourceGatherer(std::string* sourcePaths, size_t length) {
		m_sourceNodes = new std::string[length];
		for (int i = 0; i < length; i++) {
			m_sourceNodes[i] = readSource(sourcePaths[i]);
		}
	}

	std::string SourceGatherer::getSource(int sourceModule) {
		return m_sourceNodes[sourceModule];
	}

	std::string SourceGatherer::readSource(std::string sourcePath) {
		std::string line, src = "";
		std::ifstream reader(sourcePath);

		if (reader.is_open()) {
			while (std::getline(reader, line)) {
				src.append(line.append("\n"));
			}
			reader.close();
		}

		return src;
	}
}
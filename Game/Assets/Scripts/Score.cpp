#include "Score.h"

namespace Engine
{

	void Score::setScore(int num)
	{
		score = num;
	}

	void Score::addOne()
	{
		score++;
		m_entity.getScene()->score = score;
	}

	std::string Score::getScore()
	{
		return std::to_string(score);
	}
}
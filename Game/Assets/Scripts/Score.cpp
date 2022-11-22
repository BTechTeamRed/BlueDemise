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

	void Score::onUpdate(const Engine::DeltaTime& dt)
	{
		//GE_CORE_WARN("Score Score: {0} | Char score: {1}", m_entity.getScene()->score, static_cast<char>(m_entity.getScene()->score));
		//m_entity.getComponent<TextComponent>().text = static_cast<char>(m_entity.getScene()->score);
	}
}
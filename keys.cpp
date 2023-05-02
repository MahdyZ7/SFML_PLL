# include "waves.hpp"
void key_press(state_vector *st)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			st->frequency_offset += 0.05f;
			std::cout << "freq_offset: " << st->frequency_offset << std::endl;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			st->frequency_offset -= 0.05f;
			// std::bitset<32> y(st->frequency_offset);
			std::cout << "freq_offset: " << st->frequency_offset << std::endl;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			st->phase_offset -= 1.5f;
			std::cout << "phase_offset: " << st->phase_offset << std::endl;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			st->phase_offset += 1.5f;
			std::cout << "phase_offset: " << st->phase_offset << std::endl;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			st->noise_flag = 1;
			std::cout << "noise_flag: " << st->noise_flag << std::endl;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			st->noise_flag = 0;
			std::cout << "noise_flag: " << st->noise_flag << std::endl;
		}
}
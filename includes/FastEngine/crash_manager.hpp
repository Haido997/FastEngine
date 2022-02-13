#ifndef _FGE_CRASH_MANAGER_HPP_INCLUDED
#define _FGE_CRASH_MANAGER_HPP_INCLUDED

#include <FastEngine/fastengine_extern.hpp>
#include <SFML/Graphics.hpp>
#include <string>

namespace fge
{
namespace crash
{

FGE_API void Init(sf::RenderWindow& screen, sf::Font& font);

FGE_API void SetInfoText(const std::string& txt);

}//end crash
}//end fge

#endif // _FGE_CRASH_MANAGER_HPP_INCLUDED

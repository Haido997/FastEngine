#ifndef _FGE_C_FONT_HPP_INCLUDED
#define _FGE_C_FONT_HPP_INCLUDED

#include <FastEngine/fastengine_extern.hpp>
#include <FastEngine/font_manager.hpp>

namespace fge
{

/**
 * \class Font
 * \ingroup graphics
 * \brief This class is a wrapper for the font manger to allow easy manipulation
 */
class FGE_API Font
{
public:
    Font();
    /**
     * \brief Get the font data by its name
     *
     * \param name The name of the loaded font
     */
    Font( const std::string& name );
    Font( const char* name );
    /**
     * \brief Copy a custom font data pointer.
     *
     * \param data The custom font data pointer
     */
    Font( const fge::font::FontDataPtr& data );

    /**
     * \brief Clear the font data
     *
     * This function clear the font data by setting it to the default font.
     */
    void clear();

    /**
     * \brief Check if the font is valid (not unloaded)
     *
     * \return True if the font is valid, false otherwise
     */
    [[nodiscard]] bool valid() const;

    /**
     * \brief Get the font data
     *
     * \return The font data
     */
    [[nodiscard]] const fge::font::FontDataPtr& getData() const;
    /**
     * \brief Get the name of the font
     *
     * \return The name of the font
     */
    [[nodiscard]] const std::string& getName() const;

    /**
     * \brief Get the font data by its name
     *
     * \param name The name of the loaded font
     */
    fge::Font& operator =( const std::string& name );
    fge::Font& operator =( const char* name );
    /**
     * \brief Copy a custom font data pointer.
     *
     * \param data The custom font data pointer
     */
    fge::Font& operator =( const fge::font::FontDataPtr& data );

    explicit operator sf::Font*();
    explicit operator const sf::Font*() const;

    /**
     * \brief Directly get the SFML font
     *
     * \return The SFML font
     */
    operator sf::Font&();
    operator const sf::Font&() const;

    operator std::string&();
    operator const std::string&() const;

private:
    fge::font::FontDataPtr g_data;
    std::string g_name;
};

}//end fge

#endif // _FGE_C_FONT_HPP_INCLUDED

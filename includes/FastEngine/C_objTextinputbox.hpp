#ifndef _FGE_C_OBJTEXTINPUTBOX_HPP_INCLUDED
#define _FGE_C_OBJTEXTINPUTBOX_HPP_INCLUDED

#include <FastEngine/fastengine_extern.hpp>
#include <FastEngine/C_object.hpp>
#include <FastEngine/C_flag.hpp>
#include <FastEngine/C_objText.hpp>

#define FGE_OBJTEXTINBOX_CLASSNAME "FGE:OBJ:TEXTINBOX"

namespace fge
{

class FGE_API ObjTextInputBox : public fge::Object
{
public:
    ObjTextInputBox();
    explicit ObjTextInputBox(const fge::Font& font, uint16_t maxLength = 10, const sf::Vector2f& pos = sf::Vector2f());

    FGE_OBJ_DEFAULT_COPYMETHOD(fge::ObjTextInputBox)

    void setString(const sf::String& string);
    void setCharacterSize(unsigned int size);
    void setHideTextFlag(bool flag);
    void setMaxLength(uint16_t length);

    void setActiveStat(bool active);

    void setBoxSize(const sf::Vector2f& size);
    void setBoxSize(float w, float h);

    void setBoxColor(const sf::Color& color);
    void setBoxOutlineColor(const sf::Color& color);
    void setTextColor(const sf::Color& color);

    const sf::String& getString() const;
    unsigned int getCharacterSize() const;
    bool isTextHide() const;
    uint16_t getMaxLength() const;

    bool getActiveStat() const;

    const sf::Vector2f& getBoxSize() const;

    const sf::Color& getBoxColor() const;
    const sf::Color& getBoxOutlineColor() const;
    const sf::Color& getTextColor() const;

    void update(sf::RenderWindow& screen, fge::Event& event, const std::chrono::milliseconds& deltaTime, fge::Scene* scene_ptr) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void save(nlohmann::json& jsonObject, fge::Scene* scene_ptr) override;
    void load(nlohmann::json& jsonObject, fge::Scene* scene_ptr) override;
    void pack(fge::net::Packet& pck) override;
    void unpack(fge::net::Packet& pck) override;

    std::string getClassName() const override;
    std::string getReadableClassName() const override;

    sf::FloatRect getGlobalBounds() const override;
    sf::FloatRect getLocalBounds() const override;

private:
    uint16_t g_cursor = 0;
    uint16_t g_maxLength = 10;
    bool g_hide = false;

    sf::Color g_colorBox = sf::Color::White;
    sf::Color g_colorBoxOutline = sf::Color::Black;
    sf::Color g_colorText = sf::Color::Black;

    sf::String g_string;
    mutable fge::ObjText g_text;
    mutable sf::RectangleShape g_box;

    sf::Vector2f g_boxSize = sf::Vector2f(120, 18);

    bool g_statActive = false;
    fge::Flag g_flagMouse;
};

}//end fge

#endif // _FGE_C_OBJTEXTINPUTBOX_HPP_INCLUDED

#include "FastEngine/C_objTextinputbox.hpp"
#include <FastEngine/extra_function.hpp>

namespace fge
{

FGE_API ObjTextInputBox::ObjTextInputBox()
{
    this->g_text.setFillColor(this->g_colorText);
    this->g_box.setFillColor(this->g_colorBox);
    this->g_box.setOutlineColor(this->g_colorBoxOutline);
    this->g_box.setOutlineThickness(1.0f);
    this->g_box.setSize(this->g_boxSize);

    this->g_text.setCharacterSize(12);
}
FGE_API ObjTextInputBox::ObjTextInputBox(const fge::Font& font, uint16_t maxLength, const sf::Vector2f& pos) :
    g_maxLength(maxLength),
    g_text(font)
{
    this->g_text.setFillColor(this->g_colorText);
    this->g_box.setFillColor(this->g_colorBox);
    this->g_box.setOutlineColor(this->g_colorBoxOutline);
    this->g_box.setOutlineThickness(1.0f);
    this->g_box.setSize(this->g_boxSize);

    this->g_text.setCharacterSize(12);

    this->setPosition(pos);
}

void FGE_API ObjTextInputBox::setString(const sf::String& string)
{
    this->g_string = string;
}
void FGE_API ObjTextInputBox::setCharacterSize(unsigned int size)
{
    this->g_text.setCharacterSize(size);
}
void FGE_API ObjTextInputBox::setHideTextFlag(bool flag)
{
    this->g_hide = flag;
}
void FGE_API ObjTextInputBox::setMaxLength(uint16_t length)
{
    this->g_maxLength = length;
}

void FGE_API ObjTextInputBox::setActiveStat(bool active)
{
    this->g_statActive = active;
}

void FGE_API ObjTextInputBox::setBoxSize(const sf::Vector2f& size)
{
    this->g_boxSize = size;
    this->g_box.setSize(this->g_boxSize);
}
void FGE_API ObjTextInputBox::setBoxSize(float w, float h)
{
    this->g_boxSize.x = w;
    this->g_boxSize.y = h;
    this->g_box.setSize(this->g_boxSize);
}

void FGE_API ObjTextInputBox::setBoxColor(const sf::Color& color)
{
    this->g_colorBox = color;
    this->g_box.setFillColor(color);
}
void FGE_API ObjTextInputBox::setBoxOutlineColor(const sf::Color& color)
{
    this->g_colorBoxOutline = color;
    this->g_box.setOutlineColor(color);
}
void FGE_API ObjTextInputBox::setTextColor(const sf::Color& color)
{
    this->g_colorText = color;
    this->g_text.setFillColor(color);
}

const sf::String& FGE_API ObjTextInputBox::getString() const
{
    return this->g_string;
}
unsigned int FGE_API ObjTextInputBox::getCharacterSize() const
{
    return this->g_text.getCharacterSize();
}
bool FGE_API ObjTextInputBox::isTextHide() const
{
    return this->g_hide;
}
uint16_t FGE_API ObjTextInputBox::getMaxLength() const
{
    return this->g_maxLength;
}

bool FGE_API ObjTextInputBox::getActiveStat() const
{
    return this->g_statActive;
}

const sf::Vector2f& FGE_API ObjTextInputBox::getBoxSize() const
{
    return this->g_boxSize;
}

const sf::Color& FGE_API ObjTextInputBox::getBoxColor() const
{
    return this->g_colorBox;
}
const sf::Color& FGE_API ObjTextInputBox::getBoxOutlineColor() const
{
    return this->g_colorBoxOutline;
}
const sf::Color& FGE_API ObjTextInputBox::getTextColor() const
{
    return this->g_colorText;
}

void FGE_API ObjTextInputBox::update(sf::RenderWindow& screen, fge::Event& event, const std::chrono::milliseconds& deltaTime, fge::Scene* scene_ptr)
{
    if ( this->g_flagMouse.check(event.isMouseButtonPressed(sf::Mouse::Left)) )
    {
        if ( fge::IsMouseOn( screen.mapPixelToCoords(event.getMousePixelPos()), this->getGlobalBounds() ) )
        {
            this->g_statActive = true;
        }
        else
        {
            this->g_statActive = false;
        }
    }

    if ( event.isEventType(sf::Event::EventType::KeyPressed) )
    {
        if ( this->g_statActive )
        {
            uint32_t key = event.getKeyUnicode();

            if ( event.isKeyPressed(sf::Keyboard::Return) )
            {
                this->g_statActive = false;
                return;
            }
            if ( event.isKeyPressed(sf::Keyboard::Left) )
            {
                this->g_cursor = this->g_cursor ? (this->g_cursor-1) : 0;
                return;
            }
            if ( event.isKeyPressed(sf::Keyboard::Right) )
            {
                this->g_cursor = (this->g_cursor < this->g_string.getSize()) ? (this->g_cursor+1) : this->g_string.getSize();
                return;
            }

            //BackSpace
            if (key == 8)
            {
                if ( this->g_string.getSize() && this->g_cursor )
                {
                    this->g_string.erase(this->g_cursor-1);
                    --this->g_cursor;
                }
                return;
            }
            //Delete
            if ( event.isKeyPressed(sf::Keyboard::Key::Delete) )
            {
                if ( this->g_cursor < this->g_string.getSize() )
                {
                    this->g_string.erase(this->g_cursor);
                }
                return;
            }

            //Ignore Unicode control char
            if ( (key < 32) || ((key > 127) && (key < 161)) )
            {
                return;
            }

            //Insert Unicode char
            if ( this->g_string.getSize() < this->g_maxLength )
            {
                if (this->g_cursor >= this->g_string.getSize())
                {
                    this->g_string += key;
                }
                else
                {
                    this->g_string.insert(this->g_cursor, key);
                }
                ++this->g_cursor;
            }
        }
    }
}
void FGE_API ObjTextInputBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::String tmpString;

    if ( this->g_hide )
    {
        tmpString = std::string(this->g_string.getSize(), '*');
    }
    else
    {
        tmpString = this->g_string;
    }

    if ( this->g_statActive )
    {
        if (this->g_cursor >= this->g_string.getSize())
        {
            tmpString += '|';
        }
        else
        {
            tmpString.insert(this->g_cursor, '|');
        }
    }

    this->g_text.setString( tmpString );

    this->g_box.setFillColor( this->g_statActive ? (this->g_colorBox - sf::Color(50,50,50,0)) : this->g_colorBox );

    states.transform *= this->getTransform();
    target.draw(this->g_box, states);
    target.draw(this->g_text, states);
}

void FGE_API ObjTextInputBox::save(nlohmann::json& jsonObject, fge::Scene* scene_ptr)
{
    fge::Object::save(jsonObject, scene_ptr);

    jsonObject["cursor"] = this->g_cursor;
    jsonObject["maxLength"] = this->g_maxLength;
    jsonObject["hide"] = this->g_hide;

    jsonObject["colorBox"] = this->g_colorBox.toInteger();
    jsonObject["colorBoxOutline"] = this->g_colorBoxOutline.toInteger();
    jsonObject["colorText"] = this->g_colorText.toInteger();

    std::basic_string<uint32_t> tmpString = this->g_string.toUtf32();
    jsonObject["string"] = tmpString;

    jsonObject["characterSize"] = this->g_text.getCharacterSize();
    jsonObject["font"] = this->g_text.getFont();

    jsonObject["boxSizeX"] = this->g_boxSize.x;
    jsonObject["boxSizeY"] = this->g_boxSize.y;

    jsonObject["statActive"] = this->g_statActive;
}
void FGE_API ObjTextInputBox::load(nlohmann::json& jsonObject, fge::Scene* scene_ptr)
{
    fge::Object::load(jsonObject, scene_ptr);

    this->g_cursor = jsonObject.value<uint16_t>("cursor", 0);
    this->g_maxLength = jsonObject.value<uint16_t>("maxLength", 10);
    this->g_hide = jsonObject.value<bool>("hide", false);

    this->g_colorBox = sf::Color( jsonObject.value<uint32_t>("colorBox", 0xFFFFFFFF) );
    this->g_colorBoxOutline = sf::Color( jsonObject.value<uint32_t>("colorBoxOutline", 0) );
    this->g_colorText = sf::Color( jsonObject.value<uint32_t>("colorText", 0) );
    this->g_box.setFillColor(this->g_colorBox);
    this->g_box.setOutlineColor(this->g_colorBoxOutline);
    this->g_text.setFillColor(this->g_colorText);

    std::basic_string<uint32_t> tmpString = jsonObject.value<std::basic_string<uint32_t> >("string", std::basic_string<uint32_t>());
    this->g_string = tmpString;

    unsigned int charSize = jsonObject.value<uint32_t>("characterSize", 12);
    this->g_text.setCharacterSize(charSize);
    this->g_text.setFont( jsonObject.value<std::string>("font", FGE_FONT_BAD) );

    this->g_boxSize.x = jsonObject.value<float>("boxSizeX", 120);
    this->g_boxSize.y = jsonObject.value<float>("boxSizeY", 18);
    this->g_box.setSize(this->g_boxSize);

    this->g_statActive = jsonObject.value<bool>("statActive", false);
}

void FGE_API ObjTextInputBox::pack(fge::net::Packet& pck)
{
    fge::Object::pack(pck);

    pck << this->g_cursor << this->g_maxLength << this->g_hide;

    pck << this->g_colorBox << this->g_colorBoxOutline << this->g_colorText;

    pck << this->g_string;

    pck << static_cast<uint16_t>(this->g_text.getCharacterSize()) << this->g_text.getFont();

    pck << this->g_boxSize;

    pck << this->g_statActive;
}
void FGE_API ObjTextInputBox::unpack(fge::net::Packet& pck)
{
    fge::Object::unpack(pck);

    pck >> this->g_cursor >> this->g_maxLength >> this->g_hide;

    pck >> this->g_colorBox >> this->g_colorBoxOutline >> this->g_colorText;
    this->g_box.setFillColor(this->g_colorBox);
    this->g_box.setOutlineColor(this->g_colorBoxOutline);
    this->g_text.setFillColor(this->g_colorText);

    pck >> this->g_string;

    uint16_t tmpCharSize=12; fge::Font tmpFont;
    pck >> tmpCharSize >> tmpFont;
    this->g_text.setCharacterSize(tmpCharSize);
    this->g_text.setFont(tmpFont);

    pck >> this->g_boxSize;
    this->g_box.setSize(this->g_boxSize);

    pck >> this->g_statActive;
}

std::string FGE_API ObjTextInputBox::getClassName() const
{
    return FGE_OBJTEXTINBOX_CLASSNAME;
}
std::string FGE_API ObjTextInputBox::getReadableClassName() const
{
    return "text input box";
}

sf::FloatRect FGE_API ObjTextInputBox::getGlobalBounds() const
{
    return this->getTransform().transformRect( this->g_box.getLocalBounds() );
}
sf::FloatRect FGE_API ObjTextInputBox::getLocalBounds() const
{
    return this->g_box.getLocalBounds();
}

}//end fge

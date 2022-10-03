/*
 * Copyright 2022 Guillaume Guillet
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _FGE_C_OBJTEXTNEW_HPP_INCLUDED
#define _FGE_C_OBJTEXTNEW_HPP_INCLUDED

#include <FastEngine/fastengine_extern.hpp>
#include <FastEngine/C_object.hpp>
#include <FastEngine/C_font.hpp>
#include <vector>

#define FGE_OBJTEXTNEW_CLASSNAME "FGE:OBJ:TEXTNEW"

namespace fge
{

struct FGE_API Character : public sf::Transformable
{
    void clear();
    void addLine(bool outlineVertices, float lineLength, float lineTop, const sf::Color& color, float offset, float thickness, float outlineThickness = 0.0f);
    void addGlyphQuad(bool outlineVertices, const sf::Vector2f& position, const sf::Color& color, const sf::Glyph& glyph, float italicShear);

    uint32_t _character{0};
    sf::VertexArray _vertices{sf::PrimitiveType::Triangles};            //!< Vertex array containing the fill geometry
    sf::VertexArray _outlineVertices{sf::PrimitiveType::Triangles};     //!< Vertex array containing the outline geometry
};

class FGE_API ObjTextNew : public fge::Object
{
public:
    enum Style
    {
        Regular       = 0,      //!< Regular characters, no style
        Bold          = 1 << 0, //!< Bold characters
        Italic        = 1 << 1, //!< Italic characters
        Underlined    = 1 << 2, //!< Underlined characters
        StrikeThrough = 1 << 3  //!< Strike through characters
    };

    ObjTextNew();
    ObjTextNew(const sf::String& string, const fge::Font& font, const sf::Vector2f& position = {}, unsigned int characterSize = 30);

    FGE_OBJ_DEFAULT_COPYMETHOD(fge::ObjTextNew)

    void setFont(const fge::Font& font);
    const fge::Font& getFont() const;

    void setString(const sf::String& string);

    void setCharacterSize(unsigned int size);

    void setLineSpacing(float spacingFactor);
    void setLetterSpacing(float spacingFactor);

    void setStyle(uint32_t style);

    void setFillColor(const sf::Color& color);
    void setOutlineColor(const sf::Color& color);

    void setOutlineThickness(float thickness);

    const sf::String& getString() const;

    unsigned int getCharacterSize() const;

    float getLetterSpacing() const;
    float getLineSpacing() const;

    uint32_t getStyle() const;

    const sf::Color& getFillColor() const;
    const sf::Color& getOutlineColor() const;

    float getOutlineThickness() const;

    sf::Vector2f findCharacterPos(std::size_t index) const;

    FGE_OBJ_DRAW_DECLARE

    void save(nlohmann::json& jsonObject, fge::Scene* scene) override;
    void load(nlohmann::json& jsonObject, fge::Scene* scene) override;
    void pack(fge::net::Packet& pck) override;
    void unpack(fge::net::Packet& pck) override;

    const char* getClassName() const override;
    const char* getReadableClassName() const override;

    sf::FloatRect getGlobalBounds() const override;
    sf::FloatRect getLocalBounds() const override;

protected:
    void ensureGeometryUpdate() const;

private:
    sf::String              g_string;              //!< String to display
    fge::Font g_font;                //!< Font used to display the string
    unsigned int        g_characterSize;       //!< Base size of characters, in pixels
    float               g_letterSpacingFactor{1.0f}; //!< Spacing factor between letters
    float               g_lineSpacingFactor{1.0f};   //!< Spacing factor between lines
    uint32_t             g_style{Regular};               //!< Text style (see Style enum)
    sf::Color               g_fillColor{255,255,255};           //!< Text fill color
    sf::Color               g_outlineColor{0,0,0};        //!< Text outline color
    float               g_outlineThickness{0.0f};    //!< Thickness of the text's outline
    //mutable sf::VertexArray g_vertices;            //!< Vertex array containing the fill geometry
    //mutable sf::VertexArray g_outlineVertices;     //!< Vertex array containing the outline geometry
    mutable std::vector<Character> g_characters;
    mutable sf::FloatRect   g_bounds;              //!< Bounding rectangle of the text (in local coordinates)
    mutable bool        g_geometryNeedUpdate{false};  //!< Does the geometry need to be recomputed?
    mutable uint64_t      g_fontTextureId{0};       //!< The font texture id
};

}//end fge

#endif // _FGE_C_OBJTEXTNEW_HPP_INCLUDED

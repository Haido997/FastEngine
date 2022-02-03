#ifndef _FGE_C_OBJRENDERMAP_HPP_INCLUDED
#define _FGE_C_OBJRENDERMAP_HPP_INCLUDED

#include <FastEngine/fastengine_extern.hpp>
#include <FastEngine/C_object.hpp>
#include <FastEngine/C_tunnel.hpp>
#include <FastEngine/C_lightObstacle.hpp>

#define FGE_OBJLIGHTMAP_CLASSNAME "FGE:OBJ:RENDERMAP"

namespace fge
{

class FGE_API ObjRenderMap : public fge::Object, public fge::Subscriber
{
public:
    ObjRenderMap();
    ObjRenderMap(const fge::ObjRenderMap& r) :
        g_colorClear(r.g_colorClear)
    {};
    ObjRenderMap(fge::ObjRenderMap& r) :
        g_colorClear(r.g_colorClear)
    {};

    fge::Object* copy() override
    {
        return new fge::ObjRenderMap();
    }

    void onClear(const fge::Scene* scene, sf::RenderTarget& target, const sf::Color& color);

    void setClearColor(const sf::Color& color);
    const sf::Color& getClearColor() const;

    void first(fge::Scene* scene_ptr=FGE_OBJ_NOSCENE) override;
    void update(sf::RenderWindow& screen, fge::Event& event, const std::chrono::milliseconds& deltaTime, fge::Scene* scene_ptr=FGE_OBJ_NOSCENE) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void removed(fge::Scene* scene_ptr=FGE_OBJ_NOSCENE) override;

    void save(nlohmann::json& jsonObject, fge::Scene* scene_ptr=FGE_OBJ_NOSCENE) override;
    void load(nlohmann::json& jsonObject, fge::Scene* scene_ptr=FGE_OBJ_NOSCENE) override;
    void pack(fge::net::Packet& pck) override;
    void unpack(fge::net::Packet& pck) override;

    std::string getClassName() const override;
    std::string getReadableClassName() const override;

    sf::FloatRect getGlobalBounds() const override;
    sf::FloatRect getLocalBounds() const override;

    mutable sf::RenderTexture _renderTexture;

private:

    void updatePositions();
    void updateTexCoords();

    sf::Color g_colorClear = sf::Color::White;

    sf::Vertex g_vertices[4];
    sf::View g_windowView;
    sf::Vector2u g_windowSize;
};

}//end fge

#endif // _FGE_C_OBJRENDERMAP_HPP_INCLUDED

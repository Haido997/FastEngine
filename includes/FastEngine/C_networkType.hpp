#ifndef _FGE_C_NETWORKTYPE_HPP_INCLUDED
#define _FGE_C_NETWORKTYPE_HPP_INCLUDED

#include <FastEngine/fastengine_extern.hpp>
#include <FastEngine/C_packet.hpp>
#include <FastEngine/C_propertyList.hpp>
#include <FastEngine/C_callback.hpp>
#include <FastEngine/C_identity.hpp>
#include <FastEngine/C_smoothFloat.hpp>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace fge
{

class Scene;
class TagList;

namespace net
{

using NetworkPerClientConfigByte = uint8_t;
enum NetworkPerClientConfigByteMasks : NetworkPerClientConfigByte
{
    CONFIG_BYTE_MODIFIED_CHECK = 1 << 0,
    CONFIG_BYTE_EXPLICIT_UPDATE = 1 << 1
};

using NetworkPerClientModificationTable = std::unordered_map<fge::net::Identity, fge::net::NetworkPerClientConfigByte, fge::net::IdentityHash>;

class ClientList;

class FGE_API NetworkTypeBase
{
protected:
    NetworkTypeBase() = default;

public:
    virtual ~NetworkTypeBase() = default;

    virtual void* getSource() const = 0;

    virtual bool applyData(fge::net::Packet& pck) = 0;
    virtual void packData(fge::net::Packet& pck, const fge::net::Identity& id) = 0;
    virtual void packData(fge::net::Packet& pck) = 0;

    virtual bool clientsCheckup(const fge::net::ClientList& clients);

    virtual bool checkClient(const fge::net::Identity& id) const;
    virtual void forceCheckClient(const fge::net::Identity& id);
    virtual void forceUncheckClient(const fge::net::Identity& id);
    virtual void requireExplicitUpdateClient(const fge::net::Identity& id);

    virtual bool check() const = 0;
    virtual void forceCheck() = 0;
    virtual void forceUncheck() = 0;
    [[nodiscard]] bool isForced() const;

    void clearNeedUpdateFlag();
    void needUpdate();
    bool isNeedingUpdate() const;

    fge::CallbackHandler<> _onApplied;

protected:
    fge::net::NetworkPerClientModificationTable _g_tableId;
    bool _g_needUpdate{false};
    bool _g_force{false};
};

template<class T>
class NetworkType : public NetworkTypeBase
{
public:
    NetworkType(T* source);
    ~NetworkType() override = default;

    void* getSource() const override;

    bool applyData(fge::net::Packet& pck) override;
    void packData(fge::net::Packet& pck, const fge::net::Identity& id) override;
    void packData(fge::net::Packet& pck) override;

    bool check() const override;
    void forceCheck() override;
    void forceUncheck() override;

private:
    T g_typeCopy;
    T* g_typeSource;
};

class FGE_API NetworkTypeScene : public NetworkTypeBase
{
public:
    NetworkTypeScene(fge::Scene* source);
    ~NetworkTypeScene() override = default;

    void* getSource() const override;

    bool applyData(fge::net::Packet& pck) override;
    void packData(fge::net::Packet& pck, const fge::net::Identity& id) override;
    void packData(fge::net::Packet& pck) override;

    bool clientsCheckup(const fge::net::ClientList& clients) override;

    bool checkClient(const fge::net::Identity& id) const override;
    void forceCheckClient(const fge::net::Identity& id) override;
    void forceUncheckClient(const fge::net::Identity& id) override;

    bool check() const override;
    void forceCheck() override;
    void forceUncheck() override;

private:
    fge::Scene* g_typeSource;
};

class FGE_API NetworkTypeTag : public NetworkTypeBase
{
public:
    NetworkTypeTag(fge::TagList* source, std::string tag);
    ~NetworkTypeTag() override = default;

    void* getSource() const override;

    bool applyData(fge::net::Packet& pck) override;
    void packData(fge::net::Packet& pck, const fge::net::Identity& id) override;
    void packData(fge::net::Packet& pck) override;

    bool check() const override;
    void forceCheck() override;
    void forceUncheck() override;

private:
    fge::TagList* g_typeSource;
    std::string g_tag;
};

class FGE_API NetworkTypeSmoothVec2Float : public NetworkTypeBase
{
public:
    NetworkTypeSmoothVec2Float(fge::net::SmoothVec2Float* source);
    ~NetworkTypeSmoothVec2Float() override = default;

    void* getSource() const override;

    bool applyData(fge::net::Packet& pck) override;
    void packData(fge::net::Packet& pck, const fge::net::Identity& id) override;
    void packData(fge::net::Packet& pck) override;

    bool check() const override;
    void forceCheck() override;
    void forceUncheck() override;

private:
    fge::net::SmoothVec2Float* g_typeSource;
    sf::Vector2f g_typeCopy;
};

class FGE_API NetworkTypeSmoothFloat : public NetworkTypeBase
{
public:
    NetworkTypeSmoothFloat(fge::net::SmoothFloat* source);
    ~NetworkTypeSmoothFloat() override = default;

    void* getSource() const override;

    bool applyData(fge::net::Packet& pck) override;
    void packData(fge::net::Packet& pck, const fge::net::Identity& id) override;
    void packData(fge::net::Packet& pck) override;

    bool check() const override;
    void forceCheck() override;
    void forceUncheck() override;

private:
    fge::net::SmoothFloat* g_typeSource;
    float g_typeCopy;
};

template <class T>
class NetworkTypeProperty : public NetworkTypeBase
{
public:
    NetworkTypeProperty(fge::Property* source);
    ~NetworkTypeProperty() override = default;

    void* getSource() const override;

    bool applyData(fge::net::Packet& pck) override;
    void packData(fge::net::Packet& pck, const fge::net::Identity& id) override;
    void packData(fge::net::Packet& pck) override;

    bool check() const override;
    void forceCheck() override;
    void forceUncheck() override;

private:
    fge::Property* g_typeSource;
};

template <class T>
class NetworkTypePropertyList : public NetworkTypeBase
{
public:
    NetworkTypePropertyList(fge::PropertyList* source, const std::string& vname);
    ~NetworkTypePropertyList() override = default;

    void* getSource() const override;

    bool applyData(fge::net::Packet& pck) override;
    void packData(fge::net::Packet& pck, const fge::net::Identity& id) override;
    void packData(fge::net::Packet& pck) override;

    bool check() const override;
    void forceCheck() override;
    void forceUncheck() override;

    const std::string& getValueName() const;

private:
    fge::PropertyList* g_typeSource;
    std::string g_vname;
};

template<class T>
class NetworkTypeManual : public NetworkTypeBase
{
public:
    NetworkTypeManual(T* source);
    ~NetworkTypeManual() override = default;

    void* getSource() const override;

    bool applyData(fge::net::Packet& pck) override;
    void packData(fge::net::Packet& pck, const fge::net::Identity& id) override;
    void packData(fge::net::Packet& pck) override;

    bool check() const override;
    void forceCheck() override;
    void forceUncheck() override;

    void trigger();

private:
    T* g_typeSource;
    bool g_trigger;
};

class FGE_API NetworkTypeContainer
{
public:
    NetworkTypeContainer() = default;
    ~NetworkTypeContainer() = default;

    //Copy function that does nothing
    NetworkTypeContainer(const NetworkTypeContainer& n){};
    NetworkTypeContainer(NetworkTypeContainer& n){};
    NetworkTypeContainer& operator=(const NetworkTypeContainer& n){return *this;};
    NetworkTypeContainer& operator=(NetworkTypeContainer& n){return *this;};

    void clear();

    void clientsCheckup(const fge::net::ClientList& clients);
    void forceCheckClient(const fge::net::Identity& id);
    void forceUncheckClient(const fge::net::Identity& id);

    void push(fge::net::NetworkTypeBase* newNet);

    void reserve(size_t n);

    std::size_t packNeededUpdate(fge::net::Packet& pck);
    void unpackNeededUpdate(fge::net::Packet& pck, const fge::net::Identity& id);

    inline size_t size() const
    {
        return this->g_data.size();
    }
    inline fge::net::NetworkTypeBase* at(size_t index)
    {
        return this->g_data.at(index).get();
    }
    inline fge::net::NetworkTypeBase* operator[](size_t index)
    {
        return this->g_data[index].get();
    }
    inline fge::net::NetworkTypeBase* back()
    {
        return this->g_data.back().get();
    }
    inline fge::net::NetworkTypeBase* front()
    {
        return this->g_data.front().get();
    }

private:
    std::vector<std::unique_ptr<fge::net::NetworkTypeBase> > g_data;
};

}//end net
}//end fge

#include <FastEngine/C_networkType.inl>

#endif // _FGE_C_NETWORKTYPE_HPP_INCLUDED

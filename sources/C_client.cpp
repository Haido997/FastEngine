#include "FastEngine/C_client.hpp"
#include "FastEngine/C_random.hpp"
#include <limits>

#define _FGE_NET_CLIENT_TIMESTAMP_MODULO 65536

namespace fge
{
namespace net
{

FGE_API Client::Client() :
    g_latency_ms(FGE_NET_DEFAULT_LATENCY),
    g_lastPacketTimePoint( std::chrono::steady_clock::now() ),
    g_skey(FGE_NET_BAD_SKEY)
{
}
FGE_API Client::Client(fge::net::Client::Latency_ms latency) :
    g_latency_ms(latency),
    g_lastPacketTimePoint( std::chrono::steady_clock::now() ),
    g_skey(FGE_NET_BAD_SKEY)
{
}

fge::net::Skey FGE_API Client::GenerateSkey()
{
    return fge::__random.range<fge::net::Skey>(1, std::numeric_limits<fge::net::Skey>::max());
}
void FGE_API Client::setSkey(fge::net::Skey key)
{
    this->g_skey = key;
}
fge::net::Skey FGE_API Client::getSkey() const
{
    return this->g_skey;
}

void FGE_API Client::setLatency_ms(fge::net::Client::Latency_ms t)
{
    this->g_latency_ms = t;
}
fge::net::Client::Latency_ms FGE_API Client::getLatency_ms() const
{
    return this->g_latency_ms;
}

void FGE_API Client::resetLastPacketTimePoint()
{
    std::lock_guard<std::recursive_mutex> lck(this->g_mutex);
    this->g_lastPacketTimePoint = std::chrono::steady_clock::now();
}
fge::net::Client::Latency_ms FGE_API Client::getLastPacketElapsedTime()
{
    std::lock_guard<std::recursive_mutex> lck(this->g_mutex);

    std::chrono::steady_clock::time_point timeNow = std::chrono::steady_clock::now();
    uint64_t t = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - this->g_lastPacketTimePoint).count();
    return (t >= std::numeric_limits<fge::net::Client::Latency_ms>::max()) ? std::numeric_limits<fge::net::Client::Latency_ms>::max() : static_cast<fge::net::Client::Latency_ms>(t);
}

fge::net::Client::Timestamp FGE_API Client::getTimestamp_ms()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % _FGE_NET_CLIENT_TIMESTAMP_MODULO;
}
fge::net::Client::Latency_ms FGE_API Client::computeLatency_ms(const fge::net::Client::Timestamp& startedTime,
                                                                const fge::net::Client::Timestamp& returnedTime )
{
    int32_t t = static_cast<int32_t>(returnedTime) - static_cast<int32_t>(startedTime);
    if (t<0)
        t += _FGE_NET_CLIENT_TIMESTAMP_MODULO;

    return static_cast<fge::net::Client::Latency_ms>(t);
}
fge::net::Client::Latency_ms FGE_API Client::computePing_ms(const fge::net::Client::Timestamp& startedTime)
{
    fge::net::Client::Timestamp now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % _FGE_NET_CLIENT_TIMESTAMP_MODULO;
    int32_t t = static_cast<int32_t>(now) - static_cast<int32_t>(startedTime);
    if (t<0)
        t += _FGE_NET_CLIENT_TIMESTAMP_MODULO;

    return static_cast<fge::net::Client::Latency_ms>(t);
}

void FGE_API Client::clearPackets()
{
    std::lock_guard<std::recursive_mutex> lck(this->g_mutex);

    for (std::size_t i=0; i<this->g_pendingTransmitPackets.size(); ++i)
    {
        this->g_pendingTransmitPackets.pop();
    }
}
void FGE_API Client::pushPacket(const std::shared_ptr<fge::net::Packet>& pck)
{
    std::lock_guard<std::recursive_mutex> lck(this->g_mutex);

    this->g_pendingTransmitPackets.push(pck);
}
std::shared_ptr<fge::net::Packet> FGE_API Client::popPacket()
{
    std::lock_guard<std::recursive_mutex> lck(this->g_mutex);

    if (this->g_pendingTransmitPackets.empty())
    {
        return nullptr;
    }
    std::shared_ptr<fge::net::Packet> tmp = this->g_pendingTransmitPackets.front();
    this->g_pendingTransmitPackets.pop();
    return tmp;
}
bool FGE_API Client::isPendingPacketsEmpty()
{
    std::lock_guard<std::recursive_mutex> lck(this->g_mutex);
    return this->g_pendingTransmitPackets.empty();
}

}//end net
}//end fge

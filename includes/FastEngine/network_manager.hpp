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

#ifndef _FGE_NETWORK_MANAGER_HPP_INCLUDED
#define _FGE_NETWORK_MANAGER_HPP_INCLUDED

#include <FastEngine/fastengine_extern.hpp>
#include <FastEngine/C_scene.hpp>
#include <FastEngine/C_object.hpp>
#include <FastEngine/C_packet.hpp>
#include <FastEngine/C_client.hpp>
#include <optional>

#define FGE_NET_BAD_HEADER 0

#define FGE_NET_RULES_START {auto chainedArgs_=
#define FGE_NET_RULES_CHECK_EXTRACT chainedArgs_.checkExtract()
#define FGE_NET_RULES_TRY if (!FGE_NET_RULES_CHECK_EXTRACT) {return;}
#define FGE_NET_RULES_TRY_ELSE(else_) if (!FGE_NET_RULES_CHECK_EXTRACT) {else_}
#define FGE_NET_RULES_RESULT std::move(chainedArgs_._value.value())
#define FGE_NET_RULES_RESULT_N chainedArgs_._value.value()
#define FGE_NET_RULES_AFFECT(var_) FGE_NET_RULES_TRY if constexpr (std::is_move_constructible<decltype(var_)>::value){(var_) = FGE_NET_RULES_RESULT;}else{(var_) = FGE_NET_RULES_RESULT_N;}
#define FGE_NET_RULES_SETTER(setter_) FGE_NET_RULES_TRY setter_(FGE_NET_RULES_RESULT);
#define FGE_NET_RULES_AFFECT_ELSE(var_, else_) FGE_NET_RULES_TRY_ELSE(else_) if constexpr (std::is_move_constructible<decltype(var_)>::value){(var_) = FGE_NET_RULES_RESULT;}else{(var_) = FGE_NET_RULES_RESULT_N;}
#define FGE_NET_RULES_SETTER_ELSE(setter_, else_) FGE_NET_RULES_TRY_ELSE(else_) setter_(FGE_NET_RULES_RESULT);
#define FGE_NET_RULES_END }
#define FGE_NET_RULES_AFFECT_END(var_) FGE_NET_RULES_AFFECT(var_) FGE_NET_RULES_END
#define FGE_NET_RULES_SETTER_END(setter_) FGE_NET_RULES_SETTER(setter_) FGE_NET_RULES_END
#define FGE_NET_RULES_AFFECT_END_ELSE(var_, else_) FGE_NET_RULES_AFFECT_ELSE(var_, else_) FGE_NET_RULES_END
#define FGE_NET_RULES_SETTER_END_ELSE(setter_, else_) FGE_NET_RULES_SETTER_ELSE(setter_, else_) FGE_NET_RULES_END

namespace fge::net
{

using PacketHeader = uint16_t;

FGE_API uint32_t GetSceneChecksum(fge::Scene& scene);

FGE_API bool WritePacketDataToFile(fge::net::Packet& pck, const std::string& file);


inline fge::net::Packet& SetHeader(fge::net::Packet& pck, fge::net::PacketHeader header);
inline fge::net::PacketHeader GetHeader(fge::net::Packet& pck);

inline bool CheckSkey(fge::net::Packet& pck, fge::net::Skey skey);
inline fge::net::Skey GetSkey(fge::net::Packet& pck);

namespace rules
{

template<class TValue>
struct ChainedArguments
{
    ChainedArguments() = default;
    ChainedArguments(const fge::net::Packet& pck) :
            _pck(&pck)
    {}
    ChainedArguments(const fge::net::Packet* pck) :
            _pck(pck)
    {}

    const fge::net::Packet* _pck;
    std::optional<TValue> _value{std::nullopt};

    bool checkExtract();
    TValue& extract();
    template<class TPeek>
    TPeek peek();
};

template<class TValue>
bool CheckExtract(fge::net::rules::ChainedArguments<TValue>& args);
template<class TValue>
TValue& Extract(fge::net::rules::ChainedArguments<TValue>& args);
template<class TValue>
TValue Peek(fge::net::rules::ChainedArguments<TValue>& args);

template<class TValue, bool TInvertResult=false>
fge::net::rules::ChainedArguments<TValue> RRange(const TValue& min, const TValue& max, fge::net::rules::ChainedArguments<TValue> args);

template<class TValue, bool TInvertResult=false>
fge::net::rules::ChainedArguments<TValue> RMustEqual(const TValue& a, fge::net::rules::ChainedArguments<TValue> args);

template<class TValue, bool TInvertResult=false>
fge::net::rules::ChainedArguments<TValue> RStrictLess(TValue less, fge::net::rules::ChainedArguments<TValue> args);

template<class TValue, bool TInvertResult=false>
fge::net::rules::ChainedArguments<TValue> RLess(TValue less, fge::net::rules::ChainedArguments<TValue> args);

template<class TValue, bool TInvertResult=false>
fge::net::rules::ChainedArguments<TValue> RSizeRange(fge::net::SizeType min, fge::net::SizeType max, fge::net::rules::ChainedArguments<TValue> args);

template<class TValue, bool TInvertResult=false>
fge::net::rules::ChainedArguments<TValue> RSizeMustEqual(fge::net::SizeType a, fge::net::rules::ChainedArguments<TValue> args);

template<class TValue, bool TInvertResult=false>
fge::net::rules::ChainedArguments<TValue> RMustValidUtf8(fge::net::rules::ChainedArguments<TValue> args);

}//end rules

}//end fge::net

#include <FastEngine/network_manager.inl>

#endif // _FGE_NETWORK_MANAGER_HPP_INCLUDED

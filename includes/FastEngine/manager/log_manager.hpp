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

#ifndef _FGE_LOG_MANAGER_HPP_INCLUDED
#define _FGE_LOG_MANAGER_HPP_INCLUDED

#include "FastEngine/fastengine_extern.hpp"

#include <string>

namespace fge
{
namespace log
{

FGE_API const std::string& SetDefaultFolder (const std::string& default_folder);

FGE_API bool Remove (const std::string& name);
FGE_API bool Clean (const std::string& name);
FGE_API bool Rename (const std::string& name, const std::string& new_name);
FGE_API bool Write (const std::string& name, const std::string& text, const std::string& desc=std::string());

}//end log
}//end fge

#endif // _FGE_LOG_MANAGER_HPP_INCLUDED

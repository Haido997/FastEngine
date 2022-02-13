#include "FastEngine/reg_manager.hpp"
#include "FastEngine/extra_string.hpp"

#include <unordered_map>
#include <vector>
#include "FastEngine/C_scene.hpp"

namespace fge
{
namespace reg
{

namespace
{

using ClassNameMapType = std::unordered_map<std::string, fge::reg::ClassId>;
using ClassIdMapType = std::vector<std::unique_ptr<fge::reg::BaseStamp> >;

ClassNameMapType _dataClassNameMap;
ClassIdMapType _dataClassIdMap;

}//end

void ClearAll()
{
    _dataClassIdMap.clear();
    _dataClassNameMap.clear();
}

bool RegisterNewClass(std::unique_ptr<fge::reg::BaseStamp>&& newStamp)
{
    if ( fge::reg::Check( newStamp->getClassName() ) )
    {
        return false;
    }

    _dataClassNameMap[newStamp->getClassName()] = static_cast<fge::reg::ClassId>(_dataClassIdMap.size());
    _dataClassIdMap.push_back(std::move(newStamp));

    return true;
}

bool Check(const std::string& className)
{
    return _dataClassNameMap.find(className) != _dataClassNameMap.cend();
}
bool Check(fge::reg::ClassId classId)
{
    return classId < _dataClassIdMap.size();
}

fge::Object* Duplicate(const fge::Object* obj)
{
    auto it = _dataClassNameMap.find(obj->getClassName());

    if (it != _dataClassNameMap.cend())
    {
        return _dataClassIdMap[it->second]->duplicate(obj);
    }
    return new fge::Object();
}

bool Replace(const std::string& className, std::unique_ptr<fge::reg::BaseStamp>&& newStamp)
{
    auto it = _dataClassNameMap.find(className);

    if (it != _dataClassNameMap.cend())
    {
        _dataClassIdMap[it->second] = std::move(newStamp);
        return true;
    }
    return false;
}
bool Replace(fge::reg::ClassId classId, std::unique_ptr<fge::reg::BaseStamp>&& newStamp)
{
    if (classId < _dataClassIdMap.size())
    {
        _dataClassIdMap[classId] = std::move(newStamp);
        return true;
    }
    return false;
}

std::size_t GetRegisterSize()
{
    return _dataClassIdMap.size();
}

fge::Object* GetNewClassOf(const std::string& className)
{
    auto it = _dataClassNameMap.find(className);

    if (it != _dataClassNameMap.cend())
    {
        return _dataClassIdMap[it->second]->createNew();
    }
    return new fge::Object();
}
fge::Object* GetNewClassOf(fge::reg::ClassId classId)
{
    if (classId < _dataClassIdMap.size())
    {
        return _dataClassIdMap[classId]->createNew();
    }
    return new fge::Object();
}

fge::reg::ClassId GetClassId(const std::string& className)
{
    auto it = _dataClassNameMap.find(className);

    if (it != _dataClassNameMap.cend())
    {
        return it->second;
    }
    return FGE_REG_BADCLASSID;
}
std::string GetClassName(fge::reg::ClassId classId)
{
    if (classId < _dataClassIdMap.size())
    {
        return _dataClassIdMap[classId]->getClassName();
    }
    return FGE_OBJ_BADCLASSNAME;
}

fge::reg::BaseStamp* GetStampOf(const std::string& className)
{
    auto it = _dataClassNameMap.find(className);

    if (it != _dataClassNameMap.cend())
    {
        return _dataClassIdMap[it->second].get();
    }
    return nullptr;
}
fge::reg::BaseStamp* GetStampOf(fge::reg::ClassId classId)
{
    if (classId < _dataClassIdMap.size())
    {
        return _dataClassIdMap[classId].get();
    }
    return nullptr;
}

}//end reg
}//end fge

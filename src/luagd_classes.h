#pragma once

#include <lua.h>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/templates/vector.hpp>

#include "luagd_bindings.h"

using namespace godot;

struct LuauProperty
{
    const char *getter_name;
    const char *setter_name;
};

typedef HashMap<StringName, LuauProperty> PropertyMap;

struct ClassInfo
{
    const char *class_name;
    int parent_idx = -1;

    MethodMap methods;
    MethodMap static_funcs;

    PropertyMap properties;
};

typedef Vector<ClassInfo> ClassRegistry;

// ! engine_ptrcall.hpp
template <class O, class... Args>
O *_call_native_mb_ret_obj_arr(const GDExtensionMethodBindPtr mb, void *instance, const GDExtensionTypePtr *args)
{
    GodotObject *ret = nullptr;
    internal::gde_interface->object_method_bind_ptrcall(mb, instance, args, &ret);

    if (ret == nullptr)
        return nullptr;

    return reinterpret_cast<O *>(internal::gde_interface->object_get_instance_binding(ret, internal::token, &O::___binding_callbacks));
}

int luaGD_class_ctor(lua_State *L);
int luaGD_class_no_ctor(lua_State *L);

int luaGD_class_namecall(lua_State *L);
int luaGD_class_global_index(lua_State *L);

int luaGD_class_index(lua_State *L);
int luaGD_class_newindex(lua_State *L);

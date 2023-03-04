#pragma once

#include "ModuleNames.h"
#include "LuaImport.h"

#define LUAUX_TOSTRING(str) #str

#define LuauxRegisterModules(L, ...) \
	Luaux::RegisterModules( \
		L, \
		LUAUX_TOSTRING(__VA_ARGS__), \
		Luaux::Modules{__VA_ARGS__} \
	)

namespace Luaux {

	typedef std::vector<LoadModuleFunc> Modules;

	static void RegisterModules(
		Lua::Context& lua,
		const std::string& nameList,
		Modules& modules
	) {
		ModuleNames names(nameList);

		auto moduleMapper = std::make_shared<ModuleMapper>();

		for (const auto& loadModule : modules) {

			if (names.atEnd()) {
				break;
			}

			std::string name = names.extract();

			if (loadModule && !name.empty()) {
				(*moduleMapper)[name] = loadModule;
			}

		}

		lua.global("import").in() << LuaImport(moduleMapper);

		/*
		lua.global("import").in() << Lua::MakeFunc(
			[moduleMapper] (Lua::Args args, Lua::Lua lua)
			{
				std::string name, ns;

				args.in() >> name;

				if (!args.in().atEnd()) {
					args.in() >> ns;
				}

				auto module = moduleMapper->find(name);

				if (module == moduleMapper->end()) {
					throw std::logic_error("imported module '" + name + "' not found");
				}

				if (module->second) {
					module->second(lua.state().getL(), ns.c_str());
					module->second = nullptr;
				}
			}
		);
		*/
	}

	static void RegisterModules(
		Lua::State& state,
		const std::string& nameList,
		Modules& modules
	) {
		RegisterModules(
			Lua::AuxContextFromState{state},
			nameList,
			modules
		);
	}

	static void RegisterModules(
		lua_State* L,
		const std::string& nameList,
		Modules& modules
	) {
		RegisterModules(
			Lua::AuxContextFromL{L},
			nameList,
			modules
		);
	}

}
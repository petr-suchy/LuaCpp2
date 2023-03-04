#pragma once

#include <luacpp.h>
#include <vector>
#include <map>

namespace Luaux {
	
	typedef void(*LoadModuleFunc) (void*, const char*);
	typedef std::map<std::string, LoadModuleFunc> ModuleMapper;
	typedef std::shared_ptr<ModuleMapper> SharedModuleMapper;

	class LuaImport {
	public:

		Lua::Pointer<SharedModuleMapper> _mapper;
		Lua::Function __call;

		LuaImport(SharedModuleMapper mapper = nullptr) :

			_mapper(mapper),

			__call(
				Lua::MakeFunc(
					[mapper] (Lua::Args args, Lua::Lua lua)
					{
						std::string name, ns;

						args.in() >> Lua::Ignore >> name;

						if (!args.in().atEnd()) {
							args.in() >> ns;
						}

						auto module = mapper->find(name);

						if (module == mapper->end()) {
							throw std::logic_error("imported module '" + name + "' not found");
						}

						if (module->second) {
							module->second(lua.state().getL(), ns.c_str());
						}
					}
				)
			)

		{}
	};
	
}

defhost_struct(
	Luaux::LuaImport,
		m._mapper,
		m.__call
)
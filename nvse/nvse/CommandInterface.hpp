#pragma once
#include "nvse\CoreInterface.hpp"
//From Plugins+

struct CommandInfo;
struct ParamInfo;
class TESObjectREFR;
class Script;
class TESForm;
struct ScriptEventList;
struct ArrayKey;
namespace PluginAPI { class ArrayAPI; }

/**** command table API docs *******************************************************
*
*	Command table API gives plugins limited access to NVSE's internal command table.
*	The provided functionality mirrors that defined in the CommandTable class and
*	should be fairly self-explanatory. You may note that Start() and End() could be
*	used to alter the command table in memory. It probably needn't be said, but
*	doing so would be a Very Bad Idea.
*
*	GetRequiredNVSEVersion returns the minimum major release of NVSE required by
*	the specified command, i.e. the release in which it first appeared.
*	For non-NVSE commands, returns 0. For plugin commands, returns -1.
*
************************************************************************************/

namespace NVSE {

	struct CommandTableInterface
	{
		enum {
			kVersion = 1
		};

		UInt32	version;
		const CommandInfo* (*Start)(void);
		const CommandInfo* (*End)(void);
		CommandInfo* (*GetByOpcode)(UInt32 opcode);
		CommandInfo* (*GetByName)(const char* name);
		UInt32(*GetReturnType)(const CommandInfo* cmd);		// return type enum defined in CommandTable.h
		UInt32(*GetRequiredNVSEVersion)(const CommandInfo* cmd);
		const PluginInfo* (*GetParentPlugin)(const CommandInfo* cmd);	// returns a pointer to the PluginInfo of the NVSE plugin that adds the command, if any. returns NULL otherwise
		const PluginInfo* (*GetPluginInfoByName)(const char* pluginName);	// Returns a pointer to the PluginInfo of the NVSE plugin of the specified name; returns NULL is the plugin is not loaded.

		void operator=(const NVSE::CommandTableInterface& rhs) { __movsb((UInt8*)(this), (const UInt8*)(&rhs), sizeof(NVSE::CommandTableInterface)); }

	};

	extern CommandTableInterface cmdTableInterface;

}


#pragma once


#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Engine
{
	class ENGINE_API Log
	{
	public:
		static void Init();

		inline static Engine::Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Engine::Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Engine::Ref<spdlog::logger>  s_CoreLogger;
		static Engine::Ref<spdlog::logger>  s_ClientLogger;
	};
}

// Core log macros
#define EG_CORE_TRACE(...)    ::Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EG_CORE_TRACE_VEC3(vec) EG_CORE_TRACE("x: {0} y: {1} z: {2}", vec.x, vec.y, vec.z);
#define EG_CORE_TRACE_VEC2(vec) EG_CORE_TRACE("x: {0} y: {1}", vec.x, vec.y);
#define EG_CORE_INFO(...)     ::Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EG_CORE_WARN(...)     ::Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EG_CORE_ERROR(...)    ::Engine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EG_CORE_CRITICAL(...) ::Engine::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define EG_TRACE(...)         ::Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EG_TRACE_VEC3(vec) EG_CORE_TRACE("x: {0} y: {1} z: {2}", vec.x, vec.y, vec.z);
#define EG_TRACE_VEC2(vec) EG_CORE_TRACE("x: {0} y: {1}", vec.x, vec.y);
#define EG_INFO(...)          ::Engine::Log::GetClientLogger()->info(__VA_ARGS__)
#define EG_WARN(...)          ::Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EG_ERROR(...)         ::Engine::Log::GetClientLogger()->error(__VA_ARGS__)
#define EG_CRITICAL(...)      ::Engine::Log::GetClientLogger()->critical(__VA_ARGS__)
#pragma once

#include <memory>

#ifdef EG_PLATFORM_WINDOWS
#ifdef EG_DYNAMIC_LINK
	#ifdef EG_BUILD_DLL
		#define ENGINE_API __declspec(dllexport)
	#else
		#define ENGINE_API __declspec(dllimport)
	#endif
#else
#define ENGINE_API 
#endif
#else 
	#error Engine only supports Windows!
#endif

#ifdef EG_ENABLE_ASSERTS
	#define EG_ASSERT(x, ...) { if(!(x)) { EG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define EG_CORE_ASSERT(x, ...) { if(!(x)) { EG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define EG_ASSERT(x, ...)
	#define EG_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define EG_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define PROFILING 0

#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)

#else
#define PROFILE_SCOPE(name) 
#define PROFILE_FUNCTION() 
#endif

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

namespace Engine
{
	template <typename T>
	using Scope = std::unique_ptr<T>;
	template <typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args) ...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args) ...);
	}


}
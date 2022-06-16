#ifdef QUARTZ_DLL

#ifdef _WIN32

#define QUARTZ_API __declspec(dllexport)

#elif defined __GNUC__

#define QUARTZ_API __attribute__((visibility("default")))

#else

#define QUARTZ_API

#endif

#else

#ifdef _WIN32

#define QUARTZ_API __declspec(dllimport)

#elif defined __GNUC__

#define QUARTZ_API

#else

#define QUARTZ_API

#endif

#endif
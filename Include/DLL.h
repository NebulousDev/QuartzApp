#ifdef QUARTZAPP_DLL

#ifdef _WIN32

#define QUARTZAPP_API __declspec(dllexport)

#elif defined __GNUC__

#define QUARTZAPP_API __attribute__((visibility("default")))

#else

#define QUARTZAPP_API

#endif

#else

#ifdef _WIN32

#define QUARTZAPP_API __declspec(dllimport)

#elif defined __GNUC__

#define QUARTZAPP_API

#else

#define QUARTZAPP_API

#endif

#endif
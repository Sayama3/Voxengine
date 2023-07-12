
#ifndef VXM_API_H
#define VXM_API_H

#ifdef VXM_STATIC_DEFINE
#  define VXM_API
#  define VXM_NO_EXPORT
#else
#  ifndef VXM_API
#    ifdef VXM_APIS
        /* We are building this library */
#      define VXM_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define VXM_API __declspec(dllimport)
#    endif
#  endif

#  ifndef VXM_NO_EXPORT
#    define VXM_NO_EXPORT
#  endif
#endif

#ifndef VXM_DEPRECATED
#  define VXM_DEPRECATED __declspec(deprecated)
#endif

#ifndef VXM_DEPRECATED_EXPORT
#  define VXM_DEPRECATED_EXPORT VXM_API VXM_DEPRECATED
#endif

#ifndef VXM_DEPRECATED_NO_EXPORT
#  define VXM_DEPRECATED_NO_EXPORT VXM_NO_EXPORT VXM_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VXM_NO_DEPRECATED
#    define VXM_NO_DEPRECATED
#  endif
#endif

#endif /* VXM_API_H */

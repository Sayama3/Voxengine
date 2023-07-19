//
// Created by ianpo on 16/07/2023.
//

#ifndef VOXYMORE_MACROS_HPP
#define VOXYMORE_MACROS_HPP

// Defining the "_USE_MATH_DEFINES" for visual studio in the main hpp
#ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif

//#define BIND_EVENT_FN(x) [this](auto && PH1) { x(std::forward<decltype(PH1)>(PH1)); }
#define BIND_EVENT_FN(func, ...)	std::bind(&func, this, __VA_ARGS__)
//#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#endif //VOXYMORE_MACROS_HPP

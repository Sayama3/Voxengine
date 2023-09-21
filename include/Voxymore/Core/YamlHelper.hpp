//
// Created by ianpo on 21/09/2023.
//

#pragma once

#include <yaml-cpp/yaml.h>

#define KEY(x) YAML::Key << x
#define VAL(x) YAML::Value << x
#define KEYVAL(x, y) KEY(x) << VAL(y)

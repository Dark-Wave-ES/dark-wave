#pragma once

#include <sstream>

#include "app.h"

std::shared_ptr<slint::VectorModel<slint::SharedString>> pick_files();

slint::SharedString pick_directory();

std::string execute_pick_command(const std::string& pick_command);
#pragma once

#include <fstream>

#include "config.h"
#include "app.h"
#include "kdf.h"
#include "aes-gcm.h"
#include "symmetric-encryption-service.h"

void applyCipher(const std::shared_ptr<slint::Model<slint::SharedString>>& filePaths, const slint::SharedString& passphrase,
    const slint::SharedString& directory, AESMode mode);
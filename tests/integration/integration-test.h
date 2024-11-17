#pragma once

#include <filesystem>
#include <gtest/gtest.h>
#include <gmock/gmock-more-matchers.h>

#include "config.h"
#include "app.h"
#include "passphrase-checker.h"
#include "cipher-callback.h"

class IntegrationTest: public testing::TestWithParam<int> {
protected:
    void SetUp() override;
    void TearDown() override;

    slint::ComponentHandle<App> ui = App::create();
    std::vector<std::string> filePaths, encryptedFilePaths, decryptedFilePaths;
    std::string passphrase, directory;
};
#include "integration-test.h"

void IntegrationTest::SetUp() {
    int filesCount = GetParam(), fileNameLength = 16, fileContentLength = 256;
    const std::string testFileFormat = ".txt";
    filePaths.reserve(filesCount);

    directory = std::filesystem::current_path();

    for (int i = 0; i < filesCount; ++i) {
        std::string fileName = SaltGenerator::generateSalt(fileNameLength),
        fileContent = SaltGenerator::generateSalt(fileContentLength);
        fileName += testFileFormat;
        std::string filePath = std::string(directory) + "/" + fileName;
        filePaths.push_back(filePath);

        if (std::ofstream outputFile(filePath, std::ios::binary); !outputFile) {
            throw std::runtime_error("File " + directory + "/" + fileName + " could not be created!\n");
        } else {
            outputFile.write(fileContent.data(), fileContent.size());
            outputFile.close();
        }
    }

    passphrase = "NerwDCx{<m%F5c?la{Rvm6L%_=2Q}>8(M^_@2hyl%;o_fu{c%3";

    ui->on_check_passphrase([&](const slint::SharedString& passphrase){
        ui->set_strong_passphrase(isPassphraseStrong(std::string(passphrase)));
    });

    ui->on_apply_cipher([&](const std::shared_ptr<slint::Model<slint::SharedString>>& filePaths,
        const slint::SharedString& passphrase, const slint::SharedString& directory, AESMode mode) {
        applyCipher(filePaths, passphrase, directory, mode);
    });
}

void IntegrationTest::TearDown() {
    for (const auto& filePath: filePaths) {
        std::remove(filePath.c_str());
    }

    for (const auto& encryptedFilePath: encryptedFilePaths) {
        std::remove(encryptedFilePath.c_str());
    }

    for (const auto& decryptedFilePath: decryptedFilePaths) {
        std::remove(decryptedFilePath.c_str());
    }

    std::remove(std::string(directory + "/" + LOG_FILENAME).c_str());

    filePaths.clear();
    encryptedFilePaths.clear();
    decryptedFilePaths.clear();
}

INSTANTIATE_TEST_SUITE_P(Default, IntegrationTest, testing::Values(1, 3, 9));
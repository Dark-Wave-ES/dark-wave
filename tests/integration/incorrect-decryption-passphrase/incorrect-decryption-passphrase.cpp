#include "integration-test.h"

TEST_P(IntegrationTest, IncorrectDecryptionPassphrase) {
    auto slintFilePaths = std::make_shared<slint::VectorModel<slint::SharedString>>();
    auto filesCount = filePaths.size();

    for (int i = 0; i < filesCount; ++i) {
        slintFilePaths->push_back(slint::SharedString(filePaths[i]));
    }

    ui->set_files(slintFilePaths);
    ui->set_mode(AESMode::Encryption);
    ui->invoke_enter_passphrase(slint::SharedString(passphrase));
    ui->set_directory(slint::SharedString(directory));
    ui->invoke_confirm_applying();

    ui->invoke_restart_applying();
    auto encryptedSlintFilePaths = std::make_shared<slint::VectorModel<slint::SharedString>>();
    encryptedFilePaths.reserve(filesCount);

    for (const auto& filePath: filePaths) {
        const std::string fileName(filePath.begin() + filePath.find_last_of('/'), filePath.end());
        auto dotPosition = fileName.find_last_of('.');
        const std::string encryptedFilePath = std::string(directory) + fileName.substr(0, dotPosition) +
            ENCRYPTION_POSTFIX + fileName.substr(dotPosition);
        encryptedFilePaths.push_back(encryptedFilePath);
        encryptedSlintFilePaths->push_back(slint::SharedString(encryptedFilePath));
    }

    int randomPassphraseLength = 64;
    const std::string randomPassphrase = SaltGenerator::generateSalt(randomPassphraseLength);

    ui->set_files(encryptedSlintFilePaths);
    ui->set_mode(AESMode::Decryption);
    ui->invoke_enter_passphrase(slint::SharedString(randomPassphrase));
    ui->set_directory(slint::SharedString(directory));
    ui->invoke_confirm_applying();

    decryptedFilePaths = encryptedFilePaths;
    for (auto& decryptedFilePath: decryptedFilePaths) {
        const std::string fileName(decryptedFilePath.begin() + decryptedFilePath.find_last_of('/'), decryptedFilePath.end());
        auto dotPosition = fileName.find_last_of('.');
        decryptedFilePath = std::string(directory) + fileName.substr(0, dotPosition) + APPLYING_FAILED + DECRYPTION_POSTFIX + fileName.substr(dotPosition);
    }

    const std::string expectedDecryptionResult = "Decryption failed";
    size_t resultEndIndex = expectedDecryptionResult.size();

    for (int i = 0; i < filesCount; ++i) {
        if (std::ifstream inputFile(decryptedFilePaths[i], std::ios::binary); !inputFile) {
            throw std::runtime_error("Failed to open file " + decryptedFilePaths[i]);
        } else {
            std::string decryptedFileContent((std::istreambuf_iterator(inputFile)), std::istreambuf_iterator<char>());
            inputFile.close();
            EXPECT_EQ(decryptedFileContent.substr(0, resultEndIndex), expectedDecryptionResult);
        }
    }
}
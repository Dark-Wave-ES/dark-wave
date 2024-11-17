#include "integration-test.h"

TEST_P(IntegrationTest, CorrectEncryptionDecryption) {
    std::vector<std::string> filesData;
    auto slintFilePaths = std::make_shared<slint::VectorModel<slint::SharedString>>();
    auto filesCount = filePaths.size();
    filesData.reserve(filesCount);

    for (int i = 0; i < filesCount; ++i) {
        if (std::ifstream inputFile(filePaths[i], std::ios::binary); !inputFile) {
            throw std::runtime_error("Failed to open file " + filePaths[i]);
        } else {
            std::string fileContent((std::istreambuf_iterator(inputFile)), std::istreambuf_iterator<char>());
            inputFile.close();
            filesData.push_back(fileContent);
            slintFilePaths->push_back(slint::SharedString(filePaths[i]));
        }
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

    ui->set_files(encryptedSlintFilePaths);
    ui->set_mode(AESMode::Decryption);
    ui->invoke_enter_passphrase(slint::SharedString(passphrase));
    ui->set_directory(slint::SharedString(directory));
    ui->invoke_confirm_applying();

    decryptedFilePaths = encryptedFilePaths;
    for (auto& decryptedFilePath: decryptedFilePaths) {
        const std::string fileName(decryptedFilePath.begin() + decryptedFilePath.find_last_of('/'), decryptedFilePath.end());
        auto dotPosition = fileName.find_last_of('.');
        decryptedFilePath = std::string(directory) + fileName.substr(0, dotPosition) + DECRYPTION_POSTFIX + fileName.substr(dotPosition);
    }

    for (int i = 0; i < filesCount; ++i) {
        if (std::ifstream inputFile(decryptedFilePaths[i], std::ios::binary); !inputFile) {
            throw std::runtime_error("Failed to open file " + decryptedFilePaths[i]);
        } else {
            std::string decryptedFileContent((std::istreambuf_iterator(inputFile)), std::istreambuf_iterator<char>());
            inputFile.close();
            EXPECT_EQ(filesData[i], decryptedFileContent);
        }
    }
}
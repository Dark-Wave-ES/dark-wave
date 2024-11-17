#include "integration-test.h"

TEST_P(IntegrationTest, IncorrectDecryptionCipherText) {
    auto slintFilePaths = std::make_shared<slint::VectorModel<slint::SharedString>>();
    auto filesCount = filePaths.size();

    for (int i = 0; i < filesCount; ++i) {
        slintFilePaths->push_back(slint::SharedString(filePaths[i]));
    }

    ui->set_files(slintFilePaths);
    ui->set_mode(AESMode::Decryption);
    ui->invoke_enter_passphrase(slint::SharedString(passphrase));
    ui->set_directory(slint::SharedString(directory));
    ui->invoke_confirm_applying();

    auto decryptedSlintFilePaths = std::make_shared<slint::VectorModel<slint::SharedString>>();
    decryptedFilePaths.reserve(filesCount);

    for (const auto& filePath: filePaths) {
        const std::string fileName(filePath.begin() + filePath.find_last_of('/'), filePath.end());
        auto dotPosition = fileName.find_last_of('.');
        const std::string decryptedFilePath = std::string(directory) + fileName.substr(0, dotPosition) +
            APPLYING_FAILED + DECRYPTION_POSTFIX + fileName.substr(dotPosition);
        decryptedFilePaths.push_back(decryptedFilePath);
        decryptedSlintFilePaths->push_back(slint::SharedString(decryptedFilePath));
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
#include "pickers.h"

std::shared_ptr<slint::VectorModel<slint::SharedString>> pick_files() {
    const std::string PICK_FILES_COMMAND = R"(zenity --file-selection --multiple --title="Choose files" --separator="|")";

    if (const std::string filePaths = execute_pick_command(PICK_FILES_COMMAND); !filePaths.empty()) {
        auto files = std::make_shared<slint::VectorModel<slint::SharedString>>();
        std::stringstream ss(filePaths);
        std::string file;
        while (std::getline(ss, file, '|')) {
            files->push_back(slint::SharedString(file));
        }

        return files;
    }

    return {};
}

slint::SharedString pick_directory() {
    const std::string PICK_DIRECTORY_COMMAND = R"(zenity --file-selection --directory --title="Choose a directory")";

    if (const std::string directoryPath = execute_pick_command(PICK_DIRECTORY_COMMAND); !directoryPath.empty()) {
        return {directoryPath};
    }

    return {};
}

std::string execute_pick_command(const std::string& pick_command) {
    FILE* pipe = popen(pick_command.c_str(), "r");

    if (!pipe) {
        return {};
    }

    int BUFFER_SIZE = 1024;
    std::string path;

    if (char buffer[BUFFER_SIZE]; fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        path = buffer;
        path.erase(path.find_last_not_of(" \n\r\t") + 1);
    }

    pclose(pipe);

    return path;
}

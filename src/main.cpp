#include "app.h"
#include "pickers.h"
#include "passphrase-checker.h"
#include "cipher-callback.h"

int main(int argc, char** argv) {
    auto ui = App::create();

    ui->on_pick_files([&]{
        ui->set_files(pick_files());
    });

    ui->on_pick_directory([&]{
        ui->set_directory(pick_directory());
    });

    ui->on_check_passphrase([&](const slint::SharedString& passphrase){
        ui->set_strong_passphrase(isPassphraseStrong(std::string(passphrase)));
    });

    ui->run();
    return 0;
}

#include "app.h"

int main(int argc, char** argv) {
    auto ui = App::create();

    ui->run();
    return 0;
}

#include "app.h"

int main() {
    App app;

    app.load_sources_debug();

    return app.run();
}
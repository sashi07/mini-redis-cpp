#include "server.h"

int main() {

    MiniRedisServer server(6379);

    if (!server.start()) {
        return 1;
    }

    server.run();

    return 0;
}
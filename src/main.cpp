#include "Application.hpp"

int main() {
    int blockAmount = 10; // block amount(without the genesis block)
    Application app(blockAmount);
    app.run();

    return 0;
}

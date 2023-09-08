#include <unistd.h>

int main() {
    const char *target = "target_file";
    const char *link_name = "hardLink2";
    
    link(target, link_name);
    return 0;
}

// ln <target> <link_name>

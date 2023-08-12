#include <unistd.h>

int main() {
    const char *target = "target_file";
    const char *link_name = "softLink2";
    
    symlink(target, link_name);
    return 0;
}

//ln -s <target> <link_name>

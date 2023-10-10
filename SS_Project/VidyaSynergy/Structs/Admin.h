#define MAX_USERNAME_LEN 50

struct Admin{
    int admin_id; // Unique identifier for the admin
    char username[MAX_USERNAME_LEN];
    unsigned char password[SHA256_DIGEST_LENGTH];
};


// Define the commands
enum Command {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    STOP
};

// Structure to hold the command data
struct CommandData {
    Command command;
    int value; // Additional data if needed
};


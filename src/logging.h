
enum LogLevel {
    DEBUG,
    INFO,
    ERROR,
}; 

void log(char const *message, LogLevel level=LogLevel::INFO);
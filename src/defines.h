#define REMOVE_LAST_LINE    "\033[F\033[K"
#define RESET               "\033[0m"

#define BLACK(string)       "\033[30m" string RESET              /* Black */
#define RED(string)         "\033[31m" string RESET              /* Red */
#define GREEN(string)       "\033[32m" string RESET              /* Green */
#define YELLOW(string)      "\033[33m" string RESET              /* Yellow */
#define BLUE(string)        "\033[34m" string RESET              /* Blue */
#define MAGENTA(string)     "\033[35m" string RESET              /* Magenta */
#define CYAN(string)        "\033[36m" string RESET              /* Cyan */
#define WHITE(string)       "\033[37m" string RESET              /* White */
#define BOLDBLACK(string)   "\033[1m\033[30m" string RESET       /* Bold Black */
#define BOLDRED(string)     "\033[1m\033[31m" string RESET       /* Bold Red */
#define BOLDGREEN(string)   "\033[1m\033[32m" string RESET       /* Bold Green */
#define BOLDYELLOW(string)  "\033[1m\033[33m" string RESET       /* Bold Yellow */
#define BOLDBLUE(string)    "\033[1m\033[34m" string RESET       /* Bold Blue */
#define BOLDMAGENTA(string) "\033[1m\033[35m" string RESET       /* Bold Magenta */
#define BOLDCYAN(string)    "\033[1m\033[36m" string RESET       /* Bold Cyan */
#define BOLDWHITE(string)   "\033[1m\033[37m" string RESET       /* Bold White */


#define MAX_NAME_SIZE 128
#define MAX_PASSWORD_SIZE 128
#define PMAN CYAN("[pman] ")
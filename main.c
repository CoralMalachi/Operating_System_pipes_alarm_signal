


#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

#define ERROR_SYSTEM_CALL "Error in system call"
#define STDERR 2
#define STDIN 0
#define STDOUT 1
#define GAME "./draw.out"

// game directions
#define QUIT_GAME 'q'
#define TURN_LEFT 'a'
#define TURN_RIGHT 'd'
#define GO_DOWN 's'
#define TRANSPOSE 'w'

 int isCharValid(char user_input);

char getch();
 void error_sys_call();


void error_sys_call()
{
    //call system - error
    write(STDERR,ERROR_SYSTEM_CALL,21);

}


int main()
{
    pid_t m_pid;
    //fd[0] - read from pipe, fd[1] - write to pipe
    int fd[2];
    //create a pipe - way to transform information between process (must have family relationship)
    pipe(fd);

    //create a child process
    m_pid = fork();

    //if fork system call failed
    if (m_pid < 0)
    {
      error_sys_call();
    } else if (m_pid == 0)
    {
        //land in child process
        //stdin point to read from pipe - input from user will go directly to pipe
        dup2(fd[0],STDIN);
        //run the ./draw.out program through execlp system call
        execlp(GAME,GAME,NULL);
        //if execlp system call failed we'll get here
        error_sys_call();
    }
    //if m_pid> 0 it's father process
    else
    {
        char user_input;
        int flag = 1;
        //as long as the user didnt quit
        while (flag)
        {
            //get input from user
            user_input = getch();
            //check if the user input is valid
            if (isCharValid(user_input))
            {
                //try to write the input key into the pipe
                //in case of failed - print error system call
                if (write(fd[1],&user_input,STDOUT) < 0)
                    error_sys_call();
                //communication between process
                kill(m_pid,SIGUSR2);
                if (QUIT_GAME== user_input)
                    break;
            }
            //if the user input is not valid wait for next input
            else
            {
                continue;
            }
        }
    }
    return 0;
}

//function supplyed
char getch()
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}

int isCharValid(char user_input)
{
    if (user_input == TRANSPOSE || user_input == TURN_LEFT || user_input == TURN_RIGHT
            || user_input == GO_DOWN || user_input == QUIT_GAME)
        return 1;
    return 0;
}

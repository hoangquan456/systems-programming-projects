#include <tsh.h>
#include <string>

using namespace std;

/**
 * @brief
 * Helper function to print the PS1 pormpt.
 *
 * Linux has multiple promt levels
 * (https://wiki.archlinux.org/title/Bash/Prompt_customization): PS0 is
 * displayed after each command, before any output. PS1 is the primary prompt
 * which is displayed before each command. PS2 is the secondary prompt displayed
 * when a command needs more input (e.g. a multi-line command). PS3 is not very
 * commonly used
 */
void display_prompt() { cout << "$ " << flush; }

/**
 * @brief Cleans up allocated resources to prevent memory leaks.
 *
 * This function deletes all elements in the provided list of Process objects,
 * clears the list, and frees the memory allocated for the input line.
 *
 * @param process_list A pointer to a list of Process pointers to be cleaned up.
 * @param input_line A pointer to the dynamically allocated memory for user
 * input. This memory is freed to avoid memory leaks.
 */
void cleanup(list<Process *> &process_list, char *input_line) {
  for (Process *p : process_list) {
    delete p;
  }
  process_list.clear();
  free(input_line);
  input_line = nullptr;
}

/**
 * @brief Main loop for the shell, facilitating user interaction and command
 * execution.
 *
 * This function initializes a list to store Process objects, reads user input
 * in a loop, and executes the entered commands. The loop continues until the
 * user decides to quit.
 *
 * @note The function integrates several essential components:
 *   1. Displaying the shell prompt to the user.
 *   2. Reading user input using the read_input function.
 *   3. Parsing the input into a list of Process objects using parse_input.
 *   4. Executing the commands using run_commands.
 *   5. Cleaning up allocated resources to prevent memory leaks with the cleanup
 * function.
 *   6. Breaking out of the loop if the user enters the quit command.
 *   7. Continuously prompting the user for new commands until an exit condition
 * is met.
 *
 * @warning
 *  Initialize Necessary Variables:
 *      Declare variables for storing the list of Process objects, the input
 * line, and a flag for quitting. Read User Input in a Loop: Utilize a while
 * loop to continuously read user input until a termination condition is met.
 * Check for a valid input line and skip empty lines. Clean Up Resources: After
 * executing the commands, clean up allocated resources using the cleanup
 *      function to avoid memory leaks.
 *  Check for Quit Command:
 *      Determine if the user entered the quit command. If so, set the exit flag
 * to true and break out of the loop. Repeat the Process: If the user did not
 * quit, continue prompting for input by displaying the prompt again.
 *  Considerations:
 *      Handle edge cases such as empty input lines or unexpected errors
 * gracefully. Ensure proper error handling and informative messages for the
 * user.
 */
void run() {
  list<Process *> process_list;
  char *input_line;
  bool is_quit = false;
  while(!is_quit) {
    display_prompt();
    input_line = read_input();
    if (input_line == nullptr)
      break;
    sanitize(input_line); 
    parse_input(input_line, process_list);
    is_quit = run_commands(process_list);

    // if (*input_line != '\0') 
    //   cout << endl;
    process_list.clear();
  }
}

/**
 * @brief Reads input from the standard input (stdin) in chunks and dynamically
 *        allocates memory to store the entire input.
 *
 * This function reads input from the standard input or file in chunks of size
 * MAX_LINE. It dynamically allocates memory to store the entire input, resizing
 * the memory as needed. The input is stored as a null-terminated string. The
 * function continues reading until a newline character is encountered or an
 * error occurs.
 *
 * @return A pointer to the dynamically allocated memory containing the input
 * string. The caller is responsible for freeing this memory when it is no
 * longer needed. If an error occurs or EOF is reached during input, the
 * function returns NULL.
 *
 * @warning Ensure that the memory allocated by this function is freed using
 * free() to avoid memory leaks.
 */
char *read_input() {
  char *input = NULL;
  char tempbuf[MAX_LINE];
  size_t inputlen = 0, templen = 0;

  do {
    char *a = fgets(tempbuf, MAX_LINE, stdin);
    if (a == nullptr) {
      return input;
    }
    templen = strlen(tempbuf);
    input = (char *)realloc(input, inputlen + templen + 1);
    strcpy(input + inputlen, tempbuf);
    inputlen += templen;
  } while (templen == MAX_LINE - 1 && tempbuf[MAX_LINE - 2] != '\n');

  return input;
}

/**
 * @brief
 * removes the new line char of the end in cmd.
 */
void sanitize(char *cmd) {
  if (cmd == nullptr) 
    return;
  size_t len = strlen(cmd);
  while (len > 0 && cmd[len - 1] == '\n') {
    cmd[len - 1] = '\0';  
  }
}

/**
 * Parses the given command string and populates a list of Process objects.
 *
 * This function takes a command string and a reference to a list of Process
 * pointers. It tokenizes the command based on the delimiters "|; " and creates
 * a new Process object for each token. The created Process objects are added to
 * the provided process_list. Additionally, it sets pipe flags for each Process
 * based on the presence of pipe delimiters '|' in the original command string.
 *
 * @param cmd The command string to be parsed.
 * @param process_list A reference to a list of Process pointers where the
 * created Process objects will be stored.
 *
 * Hints for students:
 * - The 'delimiters' we need to handle are '|', ';', and '[space]'
 * - 'pipe_in_val' is a flag indicating whether the current Process should take
 *      input from a previous Process (1 if true, 0 if false).
 * - 'currProcess' is a pointer to the current Process being created and added
 *      to the list.
 * - When a delim is a space it calls currProcess->add_token()
 * - When the delimiter is a ";" or "|", a new Process object is created with
 *       relevant information, and the pipe flags are set based on the presence
 *       of '|' in the original command.
 * - The created Process objects are added to the process_list.
 */
void parse_input(char *cmd, list<Process *> &process_list) {
  if (cmd == nullptr)
    return;
  int pipe_in_val = 0;
  Process *currProcess = nullptr;
  char *s = (char*) malloc(sizeof(char) * 256000);
  int len = 0;
  s[0] = '\0';

  for(int i = 0; cmd[i] != '\0'; ++i) {
    if (cmd[i] == ' ') {
      if (s[0] != '\0') {
        if (currProcess == nullptr) {
          currProcess = new Process(pipe_in_val, false);
          pipe_in_val = 0; 
        }
        sanitize(s);
        currProcess->add_token(strdup(s));
      }
      len = 0;
      s[0] = '\0';
    }
    else if (cmd[i] == '|' || cmd[i] == ';') {
      if (s[0] != '\0') {
        if (currProcess == nullptr) {
          currProcess = new Process(pipe_in_val, false);
          pipe_in_val = 0; 
        }
        sanitize(s);
        currProcess->add_token(strdup(s));
      }
      len = 0;
      s[0] = '\0';
      if (cmd[i] == '|') {
        pipe_in_val = 1; 
        currProcess->pipe_out = 1; 
      } 
      currProcess->add_token(nullptr);
      process_list.push_back(currProcess);
      // cout << endl;
      currProcess = nullptr;
    }
    else {
      s[len++] = cmd[i]; 
      s[len] = '\0';
    }
  }

  if (s[0] != '\0') {
    if (currProcess == nullptr)
      currProcess = new Process(pipe_in_val, false);
    sanitize(s);
    currProcess->add_token(strdup(s));
  }
  if (currProcess != nullptr) {
    currProcess->add_token(nullptr);
    process_list.push_back(currProcess);
  }
}

/**
 * Check if the given command represents a quit request.
 *
 * This function compares the first token of the provided command with the
 * string "quit" to determine if the command is a quit request.
 *
 * Parameters:
 *   - p: A pointer to a Process structure representing the command.
 *
 * Returns:
 *   - true if the command is a quit request (the first token is "quit").
 *   - false otherwise.
 */
bool isQuit(Process *p) {
  return strcmp(p->cmdTokens[0], "quit") == 0;  
}

/**
 * @brief Execute a list of commands using processes and pipes.
 *
 * This function takes a list of processes and executes them sequentially,
 * connecting their input and output through pipes if needed. It handles forking
 * processes, creating pipes, and waiting for child processes to finish.
 *
 * @param command_list A list of Process pointers representing the commands to
 * execute. Each Process object contains information about the command, such as
 *                     command tokens, pipe settings, and file descriptors.
 *
 * @return A boolean indicating whether a quit command was encountered during
 * execution. If true, the execution was terminated prematurely due to a quit
 * command; otherwise, false.
 *
 * @details
 * The function iterates through the provided list of processes and performs the
 * following steps:
 * 1. Check if a quit command is encountered. If yes, terminate execution.
 * 2. Create pipes and fork a child process for each command.
 * 3. In the parent process, close unused pipes, wait for child processes to
 * finish if necessary, and continue to the next command.
 * 4. In the child process, set up pipes for input and output, execute the
 * command using execvp, and handle errors if the command is invalid.
 * 5. Cleanup final process and wait for all child processes to finish.
 *
 * @note
 * - The function uses Process objects, which contain information about the
 * command and pipe settings.
 * - It handles sequential execution of commands, considering pipe connections
 * between them.
 * - The function exits with an error message if execvp fails to execute the
 * command.
 * - Make sure to properly manage file descriptors, close unused pipes, and wait
 * for child processes.
 * - The function returns true if a quit command is encountered during
 * execution; otherwise, false.
 *
 * @warning
 * - Ensure that the Process class is properly implemented and contains
 * necessary information about the command, such as command tokens and pipe
 * settings.
 * - The function relies on proper implementation of the isQuit function for
 * detecting quit commands.
 * - Students should understand the basics of forking, pipes, and process
 * execution in Unix-like systems.
 */
bool run_commands(list<Process *> &command_list) {
  bool is_quit = false;
  int max_process = 0;
  int min_process = 0;
  int size = command_list.size();
  pid_t pids[size];
  Process *prev = nullptr;

  for(Process* cur: command_list) {
    if (isQuit(cur)) {
      is_quit = true;
      break;
    }
    // if (prev != nullptr && prev->pipe_out && !cur->pipe_in) {
    //   cout << "ditme doi!!!" << endl;
    // }
    
    if (!cur->pipe_in) {
      while(min_process < max_process) {
        waitpid(pids[min_process], NULL, 0); 
        ++min_process;
      }
      if (prev && prev->pipe_out) {
        close(prev->pipe_fd[0]);
        close(prev->pipe_fd[1]);
      }
      prev = nullptr;
    }

    if (cur->pipe_out) {
      pipe(cur->pipe_fd); 
    }
    pids[max_process++] = fork(); 
    if (pids[max_process - 1] < 0) { //error 
      exit(1);
    }
    else if (pids[max_process - 1] != 0) { // parent
      // close(cur->pipe_fd[0]); 
      // close(cur->pipe_fd[1]); 
      if (cur->pipe_in && prev) {
        close(prev->pipe_fd[0]);
        close(prev->pipe_fd[1]);
      }
    }
    else { //child
      // dup2(cur->pipe_fd[1], STDOUT_FILENO);
      // close(cur->pipe_fd[0]); 
      // close(cur->pipe_fd[1]); 
      if (cur->pipe_in && prev) {  //receive input from previous 
        dup2(prev->pipe_fd[0], STDIN_FILENO); 
        close(prev->pipe_fd[0]);
        close(prev->pipe_fd[1]);
      }
      if (cur->pipe_out) {
        dup2(cur->pipe_fd[1], STDOUT_FILENO);
        close(cur->pipe_fd[0]);
        close(cur->pipe_fd[1]);
      }
      execvp(cur->cmdTokens[0], cur->cmdTokens); 
      perror("execvp failed");
    }

    prev = cur; 
  }
  while(min_process < max_process) {
    waitpid(pids[min_process], NULL, 0); 
    ++min_process;
  }
  if (prev && prev->pipe_out) {
    close(prev->pipe_fd[0]);
    close(prev->pipe_fd[1]);
  }
  return is_quit;
}

/**
 * @brief Constructor for Process class.
 *
 * Initializes a Process object with the provided plags
 *
 * @param _pipe_in 1: The process takes input form prev, 0: if not
 * @param _pipe_out 1: The output of current proches is piped to next, 0: if not
 */
Process::Process(bool _pipe_in_flag, bool _pipe_out_flag) {
  pipe_in = _pipe_in_flag;
  pipe_out = _pipe_out_flag;
  tok_index = 0;
}

/**
 * @brief Destructor for Process class.
 */
Process::~Process() {}

/**
 * @brief add a pointer to a command or flags to cmdTokens
 *
 * @param tok
 */
void Process::add_token(char *tok) {
  cmdTokens[tok_index++] = tok;
}
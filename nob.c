#define NOB_IMPLEMENTATION
#include "lib/nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"
#define LIB_FOLDER   "lib/"

int main(int argc, char **argv){
    // This line enables the self-rebuilding. It detects when nob.c is updated and auto rebuilds it then
    // runs it again.
    NOB_GO_REBUILD_URSELF(argc, argv);

    // It's better to keep all the building artifacts in a separate build folder. Let's create it if it
    // does not exist yet.
    //
    // Majority of the nob command return bool which indicates whether operation has failed or not (true -
    // success, false - failure). If the operation returned false you don't need to log anything, the
    // convention is usually that the function logs what happened to itself. Just do
    // `if (!nob_function()) return;`
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    // The working horse of nob is the Nob_Cmd structure. It's a Dynamic Array of strings which represent
    // command line that you want to execute.
    Nob_Cmd cmd = {0};

    // Précompilation de la.h : on ne la recompile que si la.h a changé.
    if (nob_needs_rebuild1(BUILD_FOLDER "la.h.gch", LIB_FOLDER "la.h")) {
        nob_cc(&cmd);
        nob_cc_flags(&cmd);
        nob_cmd_append(&cmd, "-Wno-unused-function", "-x", "c-header");
        nob_cc_output(&cmd, BUILD_FOLDER "la.h.gch");
        nob_cc_inputs(&cmd, LIB_FOLDER "la.h");
        if (!nob_cmd_run(&cmd)) return 1;
    }
    

    // nob.h ships with a bunch of nob_cc_* macros that try abstract away the specific compiler.
    // They are verify basic and not particularly flexible, but you can redefine them if you need to
    // or not use them at all and create your own abstraction on top of Nob_Cmd.
    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cmd_append(&cmd, "-Wno-unused-function");
    // -I build avant -I lib : le compilateur trouve build/la.h.gch et réutilise
    // le precompiled header au lieu de reparser lib/la.h.
    nob_cmd_append(&cmd, "-I", BUILD_FOLDER, "-I", LIB_FOLDER);
    nob_cc_output(&cmd, BUILD_FOLDER "main");
    nob_cc_inputs(&cmd, SRC_FOLDER "main.c");
    nob_cmd_append(&cmd,LIB_FOLDER "libraylib.a", "-lGL" ,"-lm", "-lpthread", "-ldl","-lrt","-lX11");
    if (!nob_cmd_run(&cmd)) return 1;
// -lGL -lm -lpthread -ldl -lrt -lX11

    return 0;

}

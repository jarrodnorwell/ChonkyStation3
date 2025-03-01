#pragma once

#include <common.hpp>
#include <logger.hpp>

#include <PPUTypes.hpp>
#include <MemoryConstants.hpp>


using namespace PPUTypes;

// Circular dependency
class ThreadManager;

class Thread {
public:
    Thread(u64 entry, u64 stack_size, u64 arg, s32 prio, const u8* name, u32 id, u32 tls_vaddr, u32 tls_filesize, u32 tls_memsize, ThreadManager* mgr);
    ThreadManager* mgr;
    PPUTypes::State state;
    s32 prio;
    u64 stack;
    u64 stack_size;
    std::vector<u64> args;
    std::vector<u64> env;
    std::string name;
    u32 id;

    // Used by the thread manager.
    // If this thread is allowed to execute a number of times that exceeds a limit set in the thread manager, this
    // flag will be set to allow other threads to execute.
    bool low_prio = false;
    u64 exec_cnt = 0;
    
    // ID of the thread that called sys_ppu_thread_join on this thread, or 0 if none.
    // At most 1 thread can attempt to join another thread.
    u32 waiter = 0;
    u32 vptr = 0;   // Pointer to store the exit status in when terminated  (v is u64)

    void addArg(u64 arg);
    void finalizeArgs();
    void addEnv(u64 env);
    void finalizeEnv();
    void finalizeArgsAndEnv();  // To be called after finalizeArgs and finalizeEnv (used to align the stack)

    MAKE_LOG_FUNCTION(log, thread);

    enum class ThreadStatus {
        Running,
        Sleeping,
        Waiting,
        Terminated
    };
    ThreadStatus status = ThreadStatus::Running;
    int exit_status = 0;

    void reschedule(u64 cycles = 0);
    void sleep(u64 us);
    void sleepForCycles(u64 cycles);
    void wait();
    void wakeUp();
    void join(u32 id, u32 vptr);
    void exit(u64 exit_status);

    static std::string threadStatusToString(ThreadStatus status) {
        switch (status) {
        case ThreadStatus::Running:    return "Running";
        case ThreadStatus::Sleeping:   return "Sleeping";
        case ThreadStatus::Waiting:    return "Waiting";
        case ThreadStatus::Terminated: return "Terminated";
        }
    }
};
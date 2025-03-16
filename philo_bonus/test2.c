The POSIX semaphore mechanism is one of three IPC mechanisms that originated with
the real-time extensions to POSIX.1. The Single UNIX Specification placed the three
mechanisms (message queues, semaphores, and shared memory) in option classes.
Prior to SUSv4, the POSIX semaphore interfaces were included in the semaphores
option. In SUSv4, these interfaces were moved to the base specification, but the
message queue and shared memory interfaces remained optional.
The POSIX semaphore interfaces were meant to address several deficiencies with
the XSI semaphore interfaces:
• The POSIX semaphore interfaces allow for higher-performance implementations
compared to XSI semaphores.
• The POSIX semaphore interfaces are simpler to use: there are no semaphore sets,
and several of the interfaces are patterned after familiar file system operations.
Although there is no requirement that they be implemented in the file system,
some systems do take this approach.
• The POSIX semaphores behave more gracefully when removed. Recall that
when an XSI semaphore is removed, operations using the same semaphore
identifier fail with errno set to EIDRM. With POSIX semaphores, operations
continue to work normally until the last reference to the semaphore is released.
POSIX semaphores are available in two flavors: named and unnamed. They differ
in how they are created and destroyed, but otherwise work the same. Unnamed
semaphores exist in memory only and require that processes have access to the memory
to be able to use the semaphores. This means they can be used only by threads in the
same process or threads in different processes that have mapped the same memory
extent into their address spaces. Named semaphores, in contrast, are accessed by name
and can be used by threads in any processes that know their names.


-------------------




To create a new named semaphore or use an existing one, we call the sem_open
function.
#include <semaphore.h>
sem_t *sem_open(const char *name, int oflag, ... /* mode_t mode,
unsigned int value */ );
Returns: Pointer to semaphore if OK, SEM_FAILEDon error
When using an existing named semaphore, we specify only two arguments: the name of
the semaphore and a zero value for the oflag argument. When the oflag argument has
the O_CREAT flag set, we create a new named semaphore if it does not yet exist. If it
already exists, it is opened for use, but no additional initialization takes place.
When we specify the O_CREATflag, we need to provide two additional arguments.
The mode argument specifies who can access the semaphore. It can take on the same
values as the permission bits for opening a file: user-read, user-write, user-execute,
group-read, group-write, group-execute, other-read, other-write, and other-execute.
The resulting permissions assigned to the semaphore are modified by the caller’s file
580 Interprocess Communication Chapter 15
creation mask (Sections 4.5 and 4.8). Note, however, that only read and write access
matter, but the interfaces don’t allow us to specify the mode when we open an existing
semaphore. Implementations usually open semaphores for both reading and writing.
The value argument is used to specify the initial value for the semaphore when we
create it. It can take on any value from 0 to SEM_VALUE_MAX(Figure 2.9).
If we want to ensure that we are creating the semaphore, we can set the oflag
argument to O_CREAT|O_EXCL. This will cause sem_open to fail if the semaphore
already exists.
To promote portability, we must follow certain conventions when selecting a
semaphore name.
• The first character in the name should be a slash (/). Although there is no
requirement that an implementation of POSIX semaphores uses the file system,
if the file system is used, we want to remove any ambiguity as to the starting
point from which the name is interpreted.
• The name should contain no other slashes to avoid implementation-defined
behavior. For example, if the file system is used, the names /mysem and
//mysem would evaluate to the same filename, but if the implementation
doesn’t use the file system, the two names could be treated as different (consider
what would happen if the implementation hashed the name to an integer value
used to identify the semaphore).
• The maximum length of the semaphore name is implementation defined. The
name should be no longer than _POSIX_NAME_MAX (Figure 2.8) characters,
because this is the minimum acceptable limit to the maximum name length if the
implementation does use the file system.


----------------------


The sem_open function returns a semaphore pointer that we can pass to other
semaphore functions when we want to operate on the semaphore. When we are done
with the semaphore, we can call the sem_close function to release any resources
associated with the semaphore.
#include <semaphore.h>
int sem_close(sem_t *sem);
Returns: 0 if OK,−1 on error
If our process exits without having first called sem_close, the kernel will close any
open semaphores automatically. Note that this doesn’t affect the state of the semaphore
value — if we have incremented its value, this doesn’t change just because we exit.
Similarly, if we call sem_close, the semaphore value is unaffected. There is no
mechanism equivalent to the SEM_UNDOflag found with XSI semaphores.

-------------------------


To destroy a named semaphore, we can use the sem_unlinkfunction.
#include <semaphore.h>
int sem_unlink(const char *name);
Returns: 0 if OK,−1 on error
Section 15.10 POSIX Semaphores 581
The sem_unlink function removes the name of the semaphore. If there are no open
references to the semaphore, then it is destroyed. Otherwise, destruction is deferred
until the last open reference is closed.
Unlike with XSI semaphores, we can only adjust the value of a POSIX semaphore by
one with a single function call. Decrementing the count is analogous to locking a binary
semaphore or acquiring a resource associated with a counting semaphore.
Note that there is no distinction of semaphore type with POSIX semaphores. Whether we use
a binary semaphore or a counting semaphore depends on how we initialize and use the
semaphore. If a semaphore only ever has a value of 0 or 1, then it is a binary semaphore.
When a binary semaphore has a value of 1, we say that it is ‘‘unlocked;’’ when it has a value of
0, we say that it is ‘‘locked.’’

----------------------------


To decrement the value of a semaphore, we can use either the sem_wait or
sem_trywaitfunction.
#include <semaphore.h>
int sem_wait(sem_t *sem);
Both return: 0 if OK,−1 on error
With the sem_wait function, we will block if the semaphore count is 0. We won’t
return until we have successfully decremented the semaphore count or are interrupted
by a signal. We can use the sem_trywaitfunction to avoid blocking. If the semaphore
count is zero when we call sem_trywait, it will return−1 with errno set to EAGAIN
instead of blocking.


-------------------------

To increment the value of a semaphore, we call the sem_post function. This is
analogous to unlocking a binary semaphore or releasing a resource associated with a
counting semaphore.
582 Interprocess Communication Chapter 15
#include <semaphore.h>
int sem_post(sem_t *sem);
Returns: 0 if OK,−1 on error
If a process is blocked in a call to sem_wait (or sem_timedwait) when we call
sem_post, the process is awakened and the semaphore count that was just
incremented by sem_postis decremented by sem_wait(or sem_timedwait).


---------------------------


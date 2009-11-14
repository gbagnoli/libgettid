#ifndef __LIBGETTID_H__
#define __LIBGETTID_H__
#include <sys/types.h>
#include <pthread.h>


/** @addtogroup LIBGETTID Library to get Linux TID from pthread_t 
 *
 * Allow application to get the linux unique thread identificator 
 * from the POSIX thread library pthread_t handle.
 * @{
 */

/** @file
 * @brief libgettid Application Programming Interface 
 */

/** Get the thread id from the pthread handle 
 *  @param thread 
 *    The POSIX thread handler as returned from pthread_create
 *  @params *tid
 *    Pointer to a pid_t variable that is filled with the thread TID.
 *
 *  @return
 *    0 on success, otherwise an error code
 */
int gettid(pthread_t thread);

/** Convert an error returned from @gettid function to an human redable
 *  string describing the error occurred
 *  
 *  @param errno
 *    The error code to convert to string
 *  @return
 *    Pointer to a statically allocated string with a human
 *    readable description of the error coded.
 */
char *gettid_strerror(int errno);

/** @} */

#define GETTID_E_UNIMPLEMENTED 1

#endif /* __LIBGETTID_H__ */

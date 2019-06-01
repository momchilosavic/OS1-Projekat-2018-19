// File: lock.h


#ifndef LOCK_H_
#define LOCK_H_

#define LOCK asm{pushf;cli}

#define UNLOCK asm popf


#endif /* LOCK_H_ */

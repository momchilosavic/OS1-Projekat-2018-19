// File: lock.h


#ifndef LOCK_H_
#define LOCK_H_

#define LOCK asm cli;//asm{pushf;cli}

#define UNLOCK asm sti;//asm popf


#endif /* LOCK_H_ */

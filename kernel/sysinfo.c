#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "sysinfo.h"
#include "proc.h"

int
get_sysinfo(uint64 addr)
{
  struct proc *p = myproc();
  struct sysinfo st;
  st.freemem = freemem();
  st.nproc = nproc();

  if(copyout(p->pagetable, addr, (char *)&st, sizeof(st)) < 0)
    return -1;
  return 0;
}
#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//实现trace系统调用,获取系统调用参数的方法在syscall.c中
//意思就是，只有调用了trace，才会传递mask，进而打印
uint64
sys_trace(void)
{
  int mask;
  if(argint(0,&mask)<0){// 判断参数是否获取成功
    return -1;
  }
//为该字段进行赋值，赋值的 mask 为系统调用传过来的参数，放在了 a0 寄存器中
//argint() 函数可以从对应的寄存器中取出参数并转成 int 类型
//myproc()获取进程号
  myproc()->mask = mask;
  return 0;
}

uint64
sys_sysinfo(void)
{
  struct sysinfo info;
  uint64 addr;
  // 获取用户态传入的sysinfo结构体
  if (argaddr(0, &addr) < 0) 
    return -1;
  struct proc* p = myproc();
  info.freemem = freememory();
  info.nproc = proc_size();
  // 将内核态中的info复制到用户态
  if (copyout(p->pagetable, addr, (char*)&info, sizeof(info)) < 0)
    return -1;
  return 0;
}
# GNU-Linux_KernelModule_MMU
User-level Memory Mapping Utility Tool for GNU-Linux Systems

I so far made it for 32bit 64bit arm and x86-x64.
Please note that I used different GNU-Linux Distros to code these modules.
On the ARM modules I used "struct proc_ops" whereas on the x86 modules I used "struct file_operations".
That's because on ARM I was using Debian (which uses the "proc_ops" struct). On the x86 I was using Ubuntu, (which uses the "file_operations" struct).
If you have compatibility problems, you may change the code to fit your OS. (Note that "file_operations" also requires "linux/uaccess.h" header).

The modules uses two kinds of files:

Makefile and _mod.c are the module files themselves. Unless you want to understand in depth how this module work, you won't need to understand these files.
You just need to compile them using the linux kernel headers to create the .ko (kernel object) file, and then install it using "insmod".

_usr.c and _usr.h are the user link files. Those are used to link your user application to the module. 
You will need to add them to your user application project.

Warning:
_usr.h adds two bool functions: "mmu_init()" and "mmu_is_active".
"mmu_init()" must be called by the application before calling any other functions in _usr.h .
"mmu_init()" returns "true" if module init was successful.
"mmu_is_active()" returns "true" if "mmu_init()" has already been called.

You'll need the kernel headers to compile those codes.
On Debian based distros, the kernel headers package can be installed with "sudo apt install linux-headers-$(uname -r)"
On specifically Raspberry Pi OS (Raspbian), the kernel headers package can be installed with "sudo apt install raspberrypi-kernel-headers"

I'm not a professional developer. I make these codes just for fun. Don't expect professional performance from them.

Author: Rafael Sabe
Email: rafaelmsabe@gmail.com

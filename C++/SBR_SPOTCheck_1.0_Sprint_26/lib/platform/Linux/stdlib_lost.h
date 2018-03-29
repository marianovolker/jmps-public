// These function prototypes are excluded in stdlib.h by macros
// under normal build using uClinux coldfire toolchain (SPOT M3).
// So this prototypes must be included for this platform only.
extern int  getpt (void);
extern int  grantpt (int fd);
extern int  unlockpt (int fd);
extern char *ptsname (int fd);
#import <UIKit/UIKit.h>

struct hal_callback;
struct rt_env;

// Pointers for initialization.
extern bool (*hal_bootstrap_ptr)(char **, int *, int *, struct hal_callback *);
extern bool (*pf_init_hook_ptr)(int, int);
extern bool (*pf_init_aot_code_ptr)(struct rt_env *);

// Defined in Playfield Engine.
bool hal_bootstrap(char **, int *, int *, struct hal_callback *);

// Defined in StratoHAL.
int hal_main(int argc, char *argv[]);

// Defined in library.c
bool init_aot_code(struct rt_env *env);

//
// Use this for registering custom APIs.
//
static bool init_hook(int width, int height)
{
	return true;
}

//
// Main.
//
int main(int argc, char *argv[])
{
	hal_bootstrap_ptr = hal_bootstrap;
	pf_init_hook_ptr = init_hook;
	pf_init_aot_code_ptr = init_aot_code;

	return hal_main(argc, argv);
}

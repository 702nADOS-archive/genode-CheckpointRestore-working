static jmp_buf fork_jmp_buf;
static Genode::Capability<Genode::Parent>::Raw new_parent;

extern "C" void stdout_reconnect(); /* provided by 'log_console.cc' */


/*
 * The new process created via fork will start its execution here.
 */
extern "C" void fork_trampoline()
{
	/* reinitialize environment */
	using namespace Genode;
	env()->reinit(new_parent.dst, new_parent.local_name);

	/* reinitialize standard-output connection */
	stdout_reconnect();

	/* reinitialize config */
	Genode::Config *config = Genode::config();
	construct_at<Genode::Config>(config);

	/* reinitialize noux connection */
	construct_at<Noux_connection>(noux_connection());

	/* reinitialize main-thread object which implies reinit of context area */
	auto context_area_rm = noux_connection()->context_area_rm_session();
	env()->reinit_main_thread(context_area_rm);

	/* apply processor state that the forker had when he did the fork */
	longjmp(fork_jmp_buf, 1);
}


extern "C" pid_t fork(void)
{
	/* stack used for executing 'fork_trampoline' */
	enum { STACK_SIZE = 8 * 1024 };
	static long stack[STACK_SIZE];

	if (setjmp(fork_jmp_buf)) {

		/*
		 * We got here via longjmp from 'fork_trampoline'.
		 */
		return 0;

	} else {

		/* got here during the normal control flow of the fork call */
		sysio()->fork_in.ip              = (Genode::addr_t)(&fork_trampoline);
		sysio()->fork_in.sp              = (Genode::addr_t)(&stack[STACK_SIZE]);
		sysio()->fork_in.parent_cap_addr = (Genode::addr_t)(&new_parent);

		if (!noux_syscall(Noux::Session::SYSCALL_FORK)) {
			PERR("fork error %d", sysio()->error.general);
		}

		return sysio()->fork_out.pid;
	}
}

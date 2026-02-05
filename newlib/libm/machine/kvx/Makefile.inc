%C%_src = \
	%D%/feclearexcept.c \
	%D%/fegetenv.c \
	%D%/fegetexceptflag.c \
	%D%/fegetround.c \
	%D%/feholdexcept.c \
	%D%/feraiseexcept.c \
	%D%/fesetenv.c \
	%D%/fesetexceptflag.c \
	%D%/fesetround.c \
	%D%/fetestexcept.c \
	%D%/feupdateenv.c \
	%D%/s_fdim.c \
	%D%/sf_fdim.c \
	%D%/wf_exp.c \
	%D%/w_pow.c

libm_a_SOURCES += $(%C%_src)
